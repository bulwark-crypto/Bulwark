// Copyright (c) 2011-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2017-2018 The Bulwark developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "overviewpage.h"
#include "ui_overviewpage.h"

#include "bitcoinunits.h"
#include "clientmodel.h"
#include "guiconstants.h"
#include "guiutil.h"
#include "init.h"
#include "obfuscation.h"
#include "obfuscationconfig.h"
#include "optionsmodel.h"
#include "transactionfilterproxy.h"
#include "transactiontablemodel.h"
#include "walletmodel.h"

#include <QAbstractItemDelegate>
#include <QPainter>
#include <QSettings>
#include <QTimer>

#define DECORATION_SIZE 48
#define ICON_OFFSET 16
#define NUM_ITEMS 5

class TxViewDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    TxViewDelegate() : QAbstractItemDelegate(), unit(BitcoinUnits::BWK)
    {
    }

    inline void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        painter->save();

        QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
        QRect mainRect = option.rect;
        mainRect.moveLeft(ICON_OFFSET);
        QRect decorationRect(mainRect.topLeft(), QSize(DECORATION_SIZE, DECORATION_SIZE));
        int xspace = DECORATION_SIZE + 8;
        int ypad = 6;
        int halfheight = (mainRect.height() - 2 * ypad) / 2;
        QRect amountRect(mainRect.left() + xspace, mainRect.top() + ypad, mainRect.width() - xspace - ICON_OFFSET, halfheight);
        QRect addressRect(mainRect.left() + xspace, mainRect.top() + ypad + halfheight, mainRect.width() - xspace, halfheight);
        icon.paint(painter, decorationRect);

        QDateTime date = index.data(TransactionTableModel::DateRole).toDateTime();
        QString address = index.data(Qt::DisplayRole).toString();
        qint64 amount = index.data(TransactionTableModel::AmountRole).toLongLong();
        bool confirmed = index.data(TransactionTableModel::ConfirmedRole).toBool();
        QVariant value = index.data(Qt::ForegroundRole);
        QColor foreground = COLOR_BLACK;
        if (value.canConvert<QBrush>()) {
            QBrush brush = qvariant_cast<QBrush>(value);
            foreground = brush.color();
        }

        painter->setPen(foreground);
        QRect boundingRect;
        painter->drawText(addressRect, Qt::AlignLeft | Qt::AlignVCenter, address, &boundingRect);

        if (index.data(TransactionTableModel::WatchonlyRole).toBool()) {
            QIcon iconWatchonly = qvariant_cast<QIcon>(index.data(TransactionTableModel::WatchonlyDecorationRole));
            QRect watchonlyRect(boundingRect.right() + 5, mainRect.top() + ypad + halfheight, 16, halfheight);
            iconWatchonly.paint(painter, watchonlyRect);
        }

        if (amount < 0) {
            foreground = COLOR_NEGATIVE;
        } else if (!confirmed) {
            foreground = COLOR_UNCONFIRMED;
        } else {
            foreground = COLOR_BLACK;
        }
        painter->setPen(foreground);
        QString amountText = BitcoinUnits::formatWithUnit(unit, amount, true, BitcoinUnits::separatorAlways);
        if (!confirmed) {
            amountText = QString("[") + amountText + QString("]");
        }
        painter->drawText(amountRect, Qt::AlignRight | Qt::AlignVCenter, amountText);

        painter->setPen(COLOR_BLACK);
        painter->drawText(amountRect, Qt::AlignLeft | Qt::AlignVCenter, GUIUtil::dateTimeStr(date));

        painter->restore();
    }

    inline QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        return QSize(DECORATION_SIZE, DECORATION_SIZE);
    }

    int unit;
};
#include "overviewpage.moc"

OverviewPage::OverviewPage(QWidget* parent) : QWidget(parent),
                                              ui(new Ui::OverviewPage),
                                              clientModel(0),
                                              walletModel(0),
                                              currentBalance(-1),
                                              currentUnconfirmedBalance(-1),
                                              currentImmatureBalance(-1),
                                              currentWatchOnlyBalance(-1),
                                              currentWatchUnconfBalance(-1),
                                              currentWatchImmatureBalance(-1),
                                              txdelegate(new TxViewDelegate()),
                                              filter(0)
{
    nDisplayUnit = 0; // just make sure it's not unitialized
    ui->setupUi(this);

    // Recent transactions
    ui->listTransactions->setItemDelegate(txdelegate);
    ui->listTransactions->setIconSize(QSize(DECORATION_SIZE, DECORATION_SIZE));
    ui->listTransactions->setMinimumHeight(NUM_ITEMS * (DECORATION_SIZE + 2));
    ui->listTransactions->setAttribute(Qt::WA_MacShowFocusRect, false);

    connect(ui->listTransactions, SIGNAL(clicked(QModelIndex)), this, SLOT(handleTransactionClicked(QModelIndex)));


    // init "out of sync" warning labels
    ui->labelWalletStatus->setText("(" + tr("out of sync") + ")");
    ui->labelObfuscationSyncStatus->setText("(" + tr("out of sync") + ")");
    ui->labelTransactionsStatus->setText("(" + tr("out of sync") + ")");

    if (fLiteMode) {
        ui->frameObfuscation->setVisible(false);
    } else {
        if (fMasterNode) {
            ui->toggleObfuscation->setText("(" + tr("Disabled") + ")");
            ui->obfuscationAuto->setText("(" + tr("Disabled") + ")");
            ui->obfuscationReset->setText("(" + tr("Disabled") + ")");
            ui->frameObfuscation->setEnabled(false);
        } else {
            if (!fEnableObfuscation) {
                ui->toggleObfuscation->setText(tr("Start Obfuscation"));
            } else {
                ui->toggleObfuscation->setText(tr("Stop Obfuscation"));
            }
            timer = new QTimer(this);
            connect(timer, SIGNAL(timeout()), this, SLOT(obfuScationStatus()));
            timer->start(1000);
        }
    }

    // start with displaying the "out of sync" warnings
    showOutOfSyncWarning(true);
}

void OverviewPage::handleTransactionClicked(const QModelIndex& index)
{
    if (filter)
        emit transactionClicked(filter->mapToSource(index));
}

OverviewPage::~OverviewPage()
{
    if (!fLiteMode && !fMasterNode) disconnect(timer, SIGNAL(timeout()), this, SLOT(obfuScationStatus()));
    delete ui;
}

void OverviewPage::setBalance(const CAmount& balance, const CAmount& unconfirmedBalance, const CAmount& immatureBalance, const CAmount& anonymizedBalance, const CAmount& watchOnlyBalance, const CAmount& watchUnconfBalance, const CAmount& watchImmatureBalance)
{
    currentBalance = balance;
    currentUnconfirmedBalance = unconfirmedBalance;
    currentImmatureBalance = immatureBalance;
    currentAnonymizedBalance = anonymizedBalance;
    currentWatchOnlyBalance = watchOnlyBalance;
    currentWatchUnconfBalance = watchUnconfBalance;
    currentWatchImmatureBalance = watchImmatureBalance;


    ui->labelBalance->setText(BitcoinUnits::floorHtmlWithUnit(nDisplayUnit, balance, false, BitcoinUnits::separatorAlways));
    ui->labelUnconfirmed->setText(BitcoinUnits::floorHtmlWithUnit(nDisplayUnit, unconfirmedBalance, false, BitcoinUnits::separatorAlways));
    ui->labelImmature->setText(BitcoinUnits::floorHtmlWithUnit(nDisplayUnit, immatureBalance, false, BitcoinUnits::separatorAlways));
    ui->labelAnonymized->setText(BitcoinUnits::floorHtmlWithUnit(nDisplayUnit, anonymizedBalance, false, BitcoinUnits::separatorAlways));
    ui->labelTotal->setText(BitcoinUnits::floorHtmlWithUnit(nDisplayUnit, balance + unconfirmedBalance, false, BitcoinUnits::separatorAlways));


    ui->labelWatchAvailable->setText(BitcoinUnits::floorHtmlWithUnit(nDisplayUnit, watchOnlyBalance, false, BitcoinUnits::separatorAlways));
    ui->labelWatchPending->setText(BitcoinUnits::floorHtmlWithUnit(nDisplayUnit, watchUnconfBalance, false, BitcoinUnits::separatorAlways));
    ui->labelWatchImmature->setText(BitcoinUnits::floorHtmlWithUnit(nDisplayUnit, watchImmatureBalance, false, BitcoinUnits::separatorAlways));
    ui->labelWatchTotal->setText(BitcoinUnits::floorHtmlWithUnit(nDisplayUnit, watchOnlyBalance + watchUnconfBalance + watchImmatureBalance, false, BitcoinUnits::separatorAlways));

    // only show immature (newly mined) balance if it's non-zero, so as not to complicate things
    // for the non-mining users
    bool showImmature = immatureBalance != 0;
    bool showWatchOnlyImmature = watchImmatureBalance != 0;

    // for symmetry reasons also show immature label when the watch-only one is shown
    ui->labelImmature->setVisible(showImmature || showWatchOnlyImmature);
    ui->labelImmatureText->setVisible(showImmature || showWatchOnlyImmature);
    ui->labelWatchImmature->setVisible(showWatchOnlyImmature); // show watch-only immature balance

    updateObfuscationProgress();

    static int cachedTxLocks = 0;

    if (cachedTxLocks != nCompleteTXLocks) {
        cachedTxLocks = nCompleteTXLocks;
        ui->listTransactions->update();
    }
}

// show/hide watch-only labels
void OverviewPage::updateWatchOnlyLabels(bool showWatchOnly)
{
    ui->labelSpendable->setVisible(showWatchOnly);      // show spendable label (only when watch-only is active)
    ui->labelWatchonly->setVisible(showWatchOnly);      // show watch-only label
    ui->lineWatchBalance->setVisible(showWatchOnly);    // show watch-only balance separator line
    ui->labelWatchAvailable->setVisible(showWatchOnly); // show watch-only available balance
    ui->labelWatchPending->setVisible(showWatchOnly);   // show watch-only pending balance
    ui->labelWatchTotal->setVisible(showWatchOnly);     // show watch-only total balance

    if (!showWatchOnly) {
        ui->labelWatchImmature->hide();
    } else {
        ui->labelBalance->setIndent(20);
        ui->labelUnconfirmed->setIndent(20);
        ui->labelImmature->setIndent(20);
        ui->labelTotal->setIndent(20);
    }
}

void OverviewPage::setClientModel(ClientModel* model)
{
    this->clientModel = model;
    if (model) {
        // Show warning if this is a prerelease version
        connect(model, SIGNAL(alertsChanged(QString)), this, SLOT(updateAlerts(QString)));
        updateAlerts(model->getStatusBarWarnings());
    }
}

void OverviewPage::setWalletModel(WalletModel* model)
{
    this->walletModel = model;
    if (model && model->getOptionsModel()) {
        // Set up transaction list
        filter = new TransactionFilterProxy();
        filter->setSourceModel(model->getTransactionTableModel());
        filter->setLimit(NUM_ITEMS);
        filter->setDynamicSortFilter(true);
        filter->setSortRole(Qt::EditRole);
        filter->setShowInactive(false);
        filter->sort(TransactionTableModel::Date, Qt::DescendingOrder);

        ui->listTransactions->setModel(filter);
        ui->listTransactions->setModelColumn(TransactionTableModel::ToAddress);

        // Keep up to date with wallet
        setBalance(model->getBalance(), model->getUnconfirmedBalance(), model->getImmatureBalance(), model->getAnonymizedBalance(),
            model->getWatchBalance(), model->getWatchUnconfirmedBalance(), model->getWatchImmatureBalance());
        connect(model, SIGNAL(balanceChanged(CAmount, CAmount, CAmount, CAmount, CAmount, CAmount, CAmount)), this, SLOT(setBalance(CAmount, CAmount, CAmount, CAmount, CAmount, CAmount, CAmount)));

        connect(model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(updateDisplayUnit()));

        connect(ui->obfuscationAuto, SIGNAL(clicked()), this, SLOT(obfuscationAuto()));
        connect(ui->obfuscationReset, SIGNAL(clicked()), this, SLOT(obfuscationReset()));
        connect(ui->toggleObfuscation, SIGNAL(clicked()), this, SLOT(toggleObfuscation()));
        updateWatchOnlyLabels(model->haveWatchOnly());
        connect(model, SIGNAL(notifyWatchonlyChanged(bool)), this, SLOT(updateWatchOnlyLabels(bool)));
    }

    // update the display unit, to not use the default ("BWK")
    updateDisplayUnit();
}

void OverviewPage::updateDisplayUnit()
{
    if (walletModel && walletModel->getOptionsModel()) {
        nDisplayUnit = walletModel->getOptionsModel()->getDisplayUnit();
        if (currentBalance != -1)
            setBalance(currentBalance, currentUnconfirmedBalance, currentImmatureBalance, currentAnonymizedBalance,
                currentWatchOnlyBalance, currentWatchUnconfBalance, currentWatchImmatureBalance);

        // Update txdelegate->unit with the current unit
        txdelegate->unit = nDisplayUnit;

        ui->listTransactions->update();
    }
}

void OverviewPage::updateAlerts(const QString& warnings)
{
    this->ui->labelAlerts->setVisible(!warnings.isEmpty());
    this->ui->labelAlerts->setText(warnings);
}

void OverviewPage::showOutOfSyncWarning(bool fShow)
{
    ui->labelWalletStatus->setVisible(fShow);
    ui->labelObfuscationSyncStatus->setVisible(fShow);
    ui->labelTransactionsStatus->setVisible(fShow);
}

void OverviewPage::updateObfuscationProgress()
{
    if (!masternodeSync.IsBlockchainSynced() || ShutdownRequested()) return;

    if (!pwalletMain) return;

    QString strAmountAndRounds;
    QString strAnonymizeBulwarkAmount = BitcoinUnits::formatHtmlWithUnit(nDisplayUnit, nAnonymizeBulwarkAmount * COIN, false, BitcoinUnits::separatorAlways);

    if (currentBalance == 0) {
        ui->obfuscationProgress->setValue(0);
        ui->obfuscationProgress->setToolTip(tr("No inputs detected"));

        // when balance is zero just show info from settings
        strAnonymizeBulwarkAmount = strAnonymizeBulwarkAmount.remove(strAnonymizeBulwarkAmount.indexOf("."), BitcoinUnits::decimals(nDisplayUnit) + 1);
        strAmountAndRounds = strAnonymizeBulwarkAmount + " / " + tr("%n Rounds", "", nObfuscationRounds);

        ui->labelAmountRounds->setToolTip(tr("No inputs detected"));
        ui->labelAmountRounds->setText(strAmountAndRounds);
        return;
    }

    CAmount nDenominatedConfirmedBalance;
    CAmount nDenominatedUnconfirmedBalance;
    CAmount nAnonymizableBalance;
    CAmount nNormalizedAnonymizedBalance;
    double nAverageAnonymizedRounds;

    {
        TRY_LOCK(cs_main, lockMain);
        if (!lockMain) return;

        nDenominatedConfirmedBalance = pwalletMain->GetDenominatedBalance();
        nDenominatedUnconfirmedBalance = pwalletMain->GetDenominatedBalance(true);
        nAnonymizableBalance = pwalletMain->GetAnonymizableBalance();
        nNormalizedAnonymizedBalance = pwalletMain->GetNormalizedAnonymizedBalance();
        nAverageAnonymizedRounds = pwalletMain->GetAverageAnonymizedRounds();
    }

    CAmount nMaxToAnonymize = nAnonymizableBalance + currentAnonymizedBalance + nDenominatedUnconfirmedBalance;

    // If it's more than the anon threshold, limit to that.
    if (nMaxToAnonymize > nAnonymizeBulwarkAmount * COIN) nMaxToAnonymize = nAnonymizeBulwarkAmount * COIN;

    if (nMaxToAnonymize == 0) return;

    if (nMaxToAnonymize >= nAnonymizeBulwarkAmount * COIN) {
        ui->labelAmountRounds->setToolTip(tr("Found enough compatible inputs to anonymize %1")
                                              .arg(strAnonymizeBulwarkAmount));
        strAnonymizeBulwarkAmount = strAnonymizeBulwarkAmount.remove(strAnonymizeBulwarkAmount.indexOf("."), BitcoinUnits::decimals(nDisplayUnit) + 1);
        strAmountAndRounds = strAnonymizeBulwarkAmount + " / " + tr("%n Rounds", "", nObfuscationRounds);
    } else {
        QString strMaxToAnonymize = BitcoinUnits::formatHtmlWithUnit(nDisplayUnit, nMaxToAnonymize, false, BitcoinUnits::separatorAlways);
        ui->labelAmountRounds->setToolTip(tr("Not enough compatible inputs to anonymize <span style='color:red;'>%1</span>,<br>"
                                             "will anonymize <span style='color:red;'>%2</span> instead")
                                              .arg(strAnonymizeBulwarkAmount)
                                              .arg(strMaxToAnonymize));
        strMaxToAnonymize = strMaxToAnonymize.remove(strMaxToAnonymize.indexOf("."), BitcoinUnits::decimals(nDisplayUnit) + 1);
        strAmountAndRounds = "<span style='color:red;'>" +
                             QString(BitcoinUnits::factor(nDisplayUnit) == 1 ? "" : "~") + strMaxToAnonymize +
                             " / " + tr("%n Rounds", "", nObfuscationRounds) + "</span>";
    }
    ui->labelAmountRounds->setText(strAmountAndRounds);

    // calculate parts of the progress, each of them shouldn't be higher than 1
    // progress of denominating
    float denomPart = 0;
    // mixing progress of denominated balance
    float anonNormPart = 0;
    // completeness of full amount anonimization
    float anonFullPart = 0;

    CAmount denominatedBalance = nDenominatedConfirmedBalance + nDenominatedUnconfirmedBalance;
    denomPart = (float)denominatedBalance / nMaxToAnonymize;
    denomPart = denomPart > 1 ? 1 : denomPart;
    denomPart *= 100;

    anonNormPart = (float)nNormalizedAnonymizedBalance / nMaxToAnonymize;
    anonNormPart = anonNormPart > 1 ? 1 : anonNormPart;
    anonNormPart *= 100;

    anonFullPart = (float)currentAnonymizedBalance / nMaxToAnonymize;
    anonFullPart = anonFullPart > 1 ? 1 : anonFullPart;
    anonFullPart *= 100;

    // apply some weights to them ...
    float denomWeight = 1;
    float anonNormWeight = nObfuscationRounds;
    float anonFullWeight = 2;
    float fullWeight = denomWeight + anonNormWeight + anonFullWeight;
    // ... and calculate the whole progress
    float denomPartCalc = ceilf((denomPart * denomWeight / fullWeight) * 100) / 100;
    float anonNormPartCalc = ceilf((anonNormPart * anonNormWeight / fullWeight) * 100) / 100;
    float anonFullPartCalc = ceilf((anonFullPart * anonFullWeight / fullWeight) * 100) / 100;
    float progress = denomPartCalc + anonNormPartCalc + anonFullPartCalc;
    if (progress >= 100) progress = 100;

    ui->obfuscationProgress->setValue(progress);

    QString strToolPip = ("<b>" + tr("Overall progress") + ": %1%</b><br/>" +
                          tr("Denominated") + ": %2%<br/>" +
                          tr("Mixed") + ": %3%<br/>" +
                          tr("Anonymized") + ": %4%<br/>" +
                          tr("Denominated inputs have %5 of %n rounds on average", "", nObfuscationRounds))
                             .arg(progress)
                             .arg(denomPart)
                             .arg(anonNormPart)
                             .arg(anonFullPart)
                             .arg(nAverageAnonymizedRounds);
    ui->obfuscationProgress->setToolTip(strToolPip);
}


void OverviewPage::obfuScationStatus()
{
    static int64_t nLastDSProgressBlockTime = 0;

    int nBestHeight = chainActive.Tip()->nHeight;

    // we we're processing more then 1 block per second, we'll just leave
    if (((nBestHeight - obfuScationPool.cachedNumBlocks) / (GetTimeMillis() - nLastDSProgressBlockTime + 1) > 1)) return;
    nLastDSProgressBlockTime = GetTimeMillis();

    if (!fEnableObfuscation) {
        if (nBestHeight != obfuScationPool.cachedNumBlocks) {
            obfuScationPool.cachedNumBlocks = nBestHeight;
            updateObfuscationProgress();

            ui->obfuscationEnabled->setText(tr("Disabled"));
            ui->obfuscationStatus->setText("");
            ui->toggleObfuscation->setText(tr("Start Obfuscation"));
        }

        return;
    }

    // check obfuscation status and unlock if needed
    if (nBestHeight != obfuScationPool.cachedNumBlocks) {
        // Balance and number of transactions might have changed
        obfuScationPool.cachedNumBlocks = nBestHeight;
        updateObfuscationProgress();

        ui->obfuscationEnabled->setText(tr("Enabled"));
    }

    QString strStatus = QString(obfuScationPool.GetStatus().c_str());

    QString s = tr("Last Obfuscation message:\n") + strStatus;

    if (s != ui->obfuscationStatus->text())
        LogPrintf("Last Obfuscation message: %s\n", strStatus.toStdString());

    ui->obfuscationStatus->setText(s);

    if (obfuScationPool.sessionDenom == 0) {
        ui->labelSubmittedDenom->setText(tr("N/A"));
    } else {
        std::string out;
        obfuScationPool.GetDenominationsToString(obfuScationPool.sessionDenom, out);
        QString s2(out.c_str());
        ui->labelSubmittedDenom->setText(s2);
    }
}

void OverviewPage::obfuscationAuto()
{
    obfuScationPool.DoAutomaticDenominating();
}

void OverviewPage::obfuscationReset()
{
    obfuScationPool.Reset();

    QMessageBox::warning(this, tr("Obfuscation"),
        tr("Obfuscation was successfully reset."),
        QMessageBox::Ok, QMessageBox::Ok);
}

void OverviewPage::toggleObfuscation()
{
    QSettings settings;
    // Popup some information on first mixing
    QString hasMixed = settings.value("hasMixed").toString();
    if (hasMixed.isEmpty()) {
        QMessageBox::information(this, tr("Obfuscation"),
            tr("If you don't want to see internal Obfuscation fees/transactions select \"Most Common\" as Type on the \"Transactions\" tab."),
            QMessageBox::Ok, QMessageBox::Ok);
        settings.setValue("hasMixed", "hasMixed");
    }
    if (!fEnableObfuscation) {
        int64_t balance = currentBalance;
        float minAmount = 14.90 * COIN;
        if (balance < minAmount) {
            QString strMinAmount(BitcoinUnits::formatWithUnit(nDisplayUnit, minAmount));
            QMessageBox::warning(this, tr("Obfuscation"),
                tr("Obfuscation requires at least %1 to use.").arg(strMinAmount),
                QMessageBox::Ok, QMessageBox::Ok);
            return;
        }

        // if wallet is locked, ask for a passphrase
        if (walletModel->getEncryptionStatus() == WalletModel::Locked) {
            WalletModel::UnlockContext ctx(walletModel->requestUnlock(false));
            if (!ctx.isValid()) {
                //unlock was cancelled
                obfuScationPool.cachedNumBlocks = std::numeric_limits<int>::max();
                QMessageBox::warning(this, tr("Obfuscation"),
                    tr("Wallet is locked and user declined to unlock. Disabling Obfuscation."),
                    QMessageBox::Ok, QMessageBox::Ok);
                if (fDebug) LogPrintf("Wallet is locked and user declined to unlock. Disabling Obfuscation.\n");
                return;
            }
        }
    }

    fEnableObfuscation = !fEnableObfuscation;
    obfuScationPool.cachedNumBlocks = std::numeric_limits<int>::max();

    if (!fEnableObfuscation) {
        ui->toggleObfuscation->setText(tr("Start Obfuscation"));
        obfuScationPool.UnlockCoins();
    } else {
        ui->toggleObfuscation->setText(tr("Stop Obfuscation"));

        /* show obfuscation configuration if client has defaults set */

        if (nAnonymizeBulwarkAmount == 0) {
            ObfuscationConfig dlg(this);
            dlg.setModel(walletModel);
            dlg.exec();
        }
    }
}
