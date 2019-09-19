// Copyright (c) 2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "qt/bulwark/dashboardwidget.h"
#include "qt/bulwark/forms/ui_dashboardwidget.h"
#include "qt/bulwark/sendconfirmdialog.h"
#include "qt/bulwark/txrow.h"
#include "qt/bulwark/qtutils.h"
#include "guiutil.h"
#include "walletmodel.h"
#include "clientmodel.h"
#include "optionsmodel.h"
#include "utiltime.h"
#include <QPainter>
#include <QModelIndex>
#include <QList>
#include <QGraphicsLayout>

#define DECORATION_SIZE 65
#define NUM_ITEMS 3
#define SHOW_EMPTY_CHART_VIEW_THRESHOLD 4000
#define REQUEST_LOAD_TASK 1
#define CHART_LOAD_MIN_TIME_INTERVAL 15

DashboardWidget::DashboardWidget(BWKGUI* parent) :
    PWidget(parent),
    ui(new Ui::DashboardWidget)
{
    ui->setupUi(this);

    txHolder = new TxViewHolder(isLightTheme());
    txViewDelegate = new FurAbstractListItemDelegate(
        DECORATION_SIZE,
        txHolder,
        this
    );

    this->setStyleSheet(parent->styleSheet());
    this->setContentsMargins(0,0,0,0);

    // Containers
    setCssProperty({this, ui->left}, "container");
    ui->left->setContentsMargins(0,0,0,0);
    setCssProperty(ui->right, "container-right");
    ui->right->setContentsMargins(20,20,20,0);

    // Title
    ui->labelTitle2->setText(tr("Staking Rewards"));
    setCssTitleScreen(ui->labelTitle);
    setCssTitleScreen(ui->labelTitle2);

    /* Subtitle */
    ui->labelSubtitle->setText(tr("You can view your account's history"));
    setCssSubtitleScreen(ui->labelSubtitle);

    // Staking Information
    ui->labelMessage->setText(tr("Amount of BWK and zBWK staked."));
    setCssSubtitleScreen(ui->labelMessage);
    setCssProperty(ui->labelSquarePiv, "square-chart-piv");
    setCssProperty(ui->labelSquarezTelos, "square-chart-zbwk");
    setCssProperty(ui->labelPiv, "text-chart-piv");
    setCssProperty(ui->labelZTelos, "text-chart-zbwk");

    // Staking Amount
    QFont fontBold;
    fontBold.setWeight(QFont::Bold);

    setCssProperty(ui->labelChart, "legend-chart");

    ui->labelAmountZTelos->setText("0 zBWK");
    ui->labelAmountPiv->setText("0 BWK");
    setCssProperty(ui->labelAmountPiv, "text-stake-piv-disable");
    setCssProperty(ui->labelAmountZTelos, "text-stake-zbwk-disable");

    setCssProperty({ui->pushButtonAll,  ui->pushButtonMonth, ui->pushButtonYear}, "btn-check-time");
    setCssProperty({ui->comboBoxMonths,  ui->comboBoxYears}, "btn-combo-chart-selected");

    ui->comboBoxMonths->setView(new QListView());
    ui->comboBoxMonths->setStyleSheet("selection-background-color:transparent; selection-color:transparent;");
    ui->comboBoxYears->setView(new QListView());
    ui->comboBoxYears->setStyleSheet("selection-background-color:transparent; selection-color:transparent;");
    ui->pushButtonYear->setChecked(true);

    setCssProperty(ui->pushButtonChartArrow, "btn-chart-arrow");

    connect(ui->comboBoxYears, SIGNAL(currentIndexChanged(const QString&)), this,SLOT(onChartYearChanged(const QString&)));

    // Sort Transactions
    SortEdit* lineEdit = new SortEdit(ui->comboBoxSort);
    initComboBox(ui->comboBoxSort, lineEdit);
    connect(lineEdit, &SortEdit::Mouse_Pressed, [this](){ui->comboBoxSort->showPopup();});
    ui->comboBoxSort->addItem("Date desc", SortTx::DATE_DESC);
    ui->comboBoxSort->addItem("Date asc", SortTx::DATE_ASC);
    ui->comboBoxSort->addItem("Amount desc", SortTx::AMOUNT_ASC);
    ui->comboBoxSort->addItem("Amount asc", SortTx::AMOUNT_DESC);
    connect(ui->comboBoxSort, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onSortChanged(const QString&)));

    // Sort type
    SortEdit* lineEditType = new SortEdit(ui->comboBoxSortType);
    initComboBox(ui->comboBoxSortType, lineEditType);
    connect(lineEditType, &SortEdit::Mouse_Pressed, [this](){ui->comboBoxSortType->showPopup();});

    QSettings settings;
    ui->comboBoxSortType->addItem(tr("All"), TransactionFilterProxy::ALL_TYPES);
    ui->comboBoxSortType->addItem(tr("Received"), TransactionFilterProxy::TYPE(TransactionRecord::RecvWithAddress) | TransactionFilterProxy::TYPE(TransactionRecord::RecvFromOther));
    ui->comboBoxSortType->addItem(tr("Sent"), TransactionFilterProxy::TYPE(TransactionRecord::SendToAddress) | TransactionFilterProxy::TYPE(TransactionRecord::SendToOther));
    ui->comboBoxSortType->addItem(tr("Mined"), TransactionFilterProxy::TYPE(TransactionRecord::Generated));
    ui->comboBoxSortType->addItem(tr("Minted"), TransactionFilterProxy::TYPE(TransactionRecord::StakeMint));
    ui->comboBoxSortType->addItem(tr("MN reward"), TransactionFilterProxy::TYPE(TransactionRecord::MNReward));
    ui->comboBoxSortType->addItem(tr("To yourself"), TransactionFilterProxy::TYPE(TransactionRecord::SendToSelf));
    ui->comboBoxSortType->setCurrentIndex(0);
    connect(ui->comboBoxSortType, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onSortTypeChanged(const QString&)));

    // Transactions
    setCssProperty(ui->listTransactions, "container");
    ui->listTransactions->setItemDelegate(txViewDelegate);
    ui->listTransactions->setIconSize(QSize(DECORATION_SIZE, DECORATION_SIZE));
    ui->listTransactions->setMinimumHeight(NUM_ITEMS * (DECORATION_SIZE + 2));
    ui->listTransactions->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->listTransactions->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Sync Warning
    ui->layoutWarning->setVisible(true);
    ui->lblWarning->setText(tr("Please wait until the wallet is fully synced to see your correct balance"));
    setCssProperty(ui->lblWarning, "text-warning");
    setCssProperty(ui->imgWarning, "ic-warning");

    //Empty List
    ui->emptyContainer->setVisible(false);
    setCssProperty(ui->pushImgEmpty, "img-empty-transactions");

    ui->labelEmpty->setText(tr("No transactions yet"));
    setCssProperty(ui->labelEmpty, "text-empty");
    setCssProperty(ui->chartContainer, "container-chart");
    setCssProperty(ui->pushImgEmptyChart, "img-empty-staking-on");

    ui->btnHowTo->setText(tr("How to get BWK or zBWK"));
    setCssBtnSecondary(ui->btnHowTo);


    setCssProperty(ui->labelEmptyChart, "text-empty");
    ui->labelMessageEmpty->setText(tr("You can verify the staking activity in the status bar at the top right of the wallet.\nIt will start automatically as soon as the wallet has enough confirmations on any unspent balances, and the wallet has synced."));
    setCssSubtitleScreen(ui->labelMessageEmpty);

    // Chart State
    ui->layoutChart->setVisible(false);
    ui->emptyContainerChart->setVisible(true);
    setShadow(ui->layoutShadow);

    connect(ui->listTransactions, SIGNAL(clicked(QModelIndex)), this, SLOT(handleTransactionClicked(QModelIndex)));
    if (window)
        connect(window, SIGNAL(windowResizeEvent(QResizeEvent*)), this, SLOT(windowResizeEvent(QResizeEvent*)));

bool hasCharts = false;
#ifdef USE_QTCHARTS
    hasCharts = true;
    setChartShow(YEAR);
    connect(ui->pushButtonYear, &QPushButton::clicked, [this](){setChartShow(YEAR);});
    connect(ui->pushButtonMonth, &QPushButton::clicked, [this](){setChartShow(MONTH);});
    connect(ui->pushButtonAll, &QPushButton::clicked, [this](){setChartShow(ALL);});
#endif

    if (hasCharts) {
        ui->labelEmptyChart->setText(tr("You have no staking rewards"));
    } else {
        ui->labelEmptyChart->setText(tr("No charts library"));
    }
}

void DashboardWidget::handleTransactionClicked(const QModelIndex &index){

    ui->listTransactions->setCurrentIndex(index);
    QModelIndex rIndex = filter->mapToSource(index);

    window->showHide(true);
    TxDetailDialog *dialog = new TxDetailDialog(window, false);
    dialog->setData(walletModel, rIndex);
    dialog->adjustSize();
    openDialogWithOpaqueBackgroundY(dialog, window, 3, 17);

    // Back to regular status
    ui->listTransactions->scrollTo(index);
    ui->listTransactions->clearSelection();
    ui->listTransactions->setFocus();
    dialog->deleteLater();
}

void DashboardWidget::loadWalletModel(){
    if (walletModel && walletModel->getOptionsModel()) {
        txModel = walletModel->getTransactionTableModel();
        // Set up transaction list
        filter = new TransactionFilterProxy();
        filter->setDynamicSortFilter(true);
        filter->setSortCaseSensitivity(Qt::CaseInsensitive);
        filter->setFilterCaseSensitivity(Qt::CaseInsensitive);
        filter->setSortRole(Qt::EditRole);
        filter->setSourceModel(txModel);
        filter->sort(TransactionTableModel::Date, Qt::DescendingOrder);
        txHolder->setFilter(filter);
        ui->listTransactions->setModel(filter);
        ui->listTransactions->setModelColumn(TransactionTableModel::ToAddress);

        if(txModel->size() == 0){
            ui->emptyContainer->setVisible(true);
            ui->listTransactions->setVisible(false);
            ui->comboBoxSortType->setVisible(false);
            ui->comboBoxSort->setVisible(false);
        }

        connect(ui->pushImgEmpty, SIGNAL(clicked()), window, SLOT(openFAQ()));
        connect(ui->btnHowTo, SIGNAL(clicked()), window, SLOT(openFAQ()));
        connect(txModel, &TransactionTableModel::txArrived, this, &DashboardWidget::onTxArrived);

        // Notification pop-up for new transaction
        connect(txModel, SIGNAL(rowsInserted(QModelIndex, int, int)),
                this, SLOT(processNewTransaction(QModelIndex, int, int)));
#ifdef USE_QTCHARTS
        // chart filter
        stakesFilter = new TransactionFilterProxy();
        stakesFilter->setSourceModel(txModel);
        stakesFilter->sort(TransactionTableModel::Date, Qt::AscendingOrder);
        stakesFilter->setOnlyStakes(true);
        loadChart();
#endif
    }
    // update the display unit, to not use the default ("BWK")
    updateDisplayUnit();
}

void DashboardWidget::onTxArrived(const QString& hash) {
    showList();
#ifdef USE_QTCHARTS
    if (walletModel->isCoinStakeMine(hash))
        tryChartRefresh();
#endif
}

void DashboardWidget::showList(){
    if (ui->emptyContainer->isVisible()) {
        ui->emptyContainer->setVisible(false);
        ui->listTransactions->setVisible(true);
        ui->comboBoxSortType->setVisible(true);
        ui->comboBoxSort->setVisible(true);
    }
}

void DashboardWidget::updateDisplayUnit() {
    if (walletModel && walletModel->getOptionsModel()) {
        nDisplayUnit = walletModel->getOptionsModel()->getDisplayUnit();
        txHolder->setDisplayUnit(nDisplayUnit);
        ui->listTransactions->update();
    }
}

void DashboardWidget::onSortChanged(const QString& value){
    if (!filter) return;
    int columnIndex = 0;
    Qt::SortOrder order = Qt::DescendingOrder;
    if(!value.isNull()) {
        switch (ui->comboBoxSort->itemData(ui->comboBoxSort->currentIndex()).toInt()) {
            case SortTx::DATE_ASC:{
                columnIndex = TransactionTableModel::Date;
                order = Qt::AscendingOrder;
                break;
            }
            case SortTx::DATE_DESC:{
                columnIndex = TransactionTableModel::Date;
                break;
            }
            case SortTx::AMOUNT_ASC:{
                columnIndex = TransactionTableModel::Amount;
                order = Qt::AscendingOrder;
                break;
            }
            case SortTx::AMOUNT_DESC:{
                columnIndex = TransactionTableModel::Amount;
                break;
            }

        }
    }
    filter->sort(columnIndex, order);
    ui->listTransactions->update();
}

void DashboardWidget::onSortTypeChanged(const QString& value){
    if (!filter) return;
    int filterByType = ui->comboBoxSortType->itemData(ui->comboBoxSortType->currentIndex()).toInt();
    filter->setTypeFilter(filterByType);
    ui->listTransactions->update();

    if (filter->rowCount() == 0){
        ui->emptyContainer->setVisible(true);
        ui->listTransactions->setVisible(false);
    } else {
        showList();
    }

    // Store settings
    QSettings settings;
    settings.setValue("transactionType", filterByType);
}

void DashboardWidget::walletSynced(bool sync){
    if (this->isSync != sync) {
        this->isSync = sync;
        ui->layoutWarning->setVisible(!this->isSync);
#ifdef USE_QTCHARTS
        tryChartRefresh();
#endif
    }
}

void DashboardWidget::changeTheme(bool isLightTheme, QString& theme){
    static_cast<TxViewHolder*>(this->txViewDelegate->getRowFactory())->isLightTheme = isLightTheme;
#ifdef USE_QTCHARTS
    if (chart) this->changeChartColors();
#endif
}

#ifdef USE_QTCHARTS

void DashboardWidget::tryChartRefresh() {
    if (hasStakes()) {
        // Check for min update time to not reload the UI so often if the node is syncing.
        int64_t now = GetTime();
        if (lastRefreshTime + CHART_LOAD_MIN_TIME_INTERVAL < now) {
            lastRefreshTime = now;
            refreshChart();
        }
    }
}

void DashboardWidget::setChartShow(ChartShowType type) {
    this->chartShow = type;
    if (chartShow == MONTH) {
        ui->containerChartArrow->setVisible(true);
    } else {
        ui->containerChartArrow->setVisible(false);
    }
    if (isChartInitialized) refreshChart();
}

const QStringList monthsNames = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

void DashboardWidget::loadChart(){
    if (hasStakes()) {
        if (!chart) {
            showHideEmptyChart(false, false);
            initChart();
            QDate currentDate = QDate::currentDate();
            monthFilter = currentDate.month();
            yearFilter = currentDate.year();
            for (int i = 1; i < 13; ++i) ui->comboBoxMonths->addItem(QString(monthsNames[i-1]), QVariant(i));
            ui->comboBoxMonths->setCurrentIndex(monthFilter - 1);
            connect(ui->comboBoxMonths, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onChartMonthChanged(const QString&)));
            connect(ui->pushButtonChartArrow, SIGNAL(clicked()), this, SLOT(onChartArrowClicked()));
        }
        refreshChart();
        changeChartColors();
    } else {
        showHideEmptyChart(true, false);
    }
}

void DashboardWidget::showHideEmptyChart(bool showEmpty, bool loading, bool forceView) {
    if (stakesFilter->rowCount() > SHOW_EMPTY_CHART_VIEW_THRESHOLD || forceView) {
        if (!ui->layoutChart->isVisible()) {
            ui->layoutChart->setVisible(!showEmpty);
            ui->emptyContainerChart->setVisible(showEmpty);
        }
    }
    ui->labelEmptyChart->setText(loading ? tr("Loading chart..") : tr("You have no staking rewards"));
}

void DashboardWidget::initChart() {
    chart = new QChart();
    axisX = new QBarCategoryAxis();
    axisY = new QValueAxis();

    // Chart style
    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignTop);
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setMargins({0, 0, 0, 0});
    chart->setBackgroundRoundness(0);
    // Axis
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignRight);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setRubberBand( QChartView::HorizontalRubberBand );
    chartView->setContentsMargins(0,0,0,0);

    QHBoxLayout *baseScreensContainer = new QHBoxLayout(this);
    baseScreensContainer->setMargin(0);
    baseScreensContainer->addWidget(chartView);
    ui->chartContainer->setLayout(baseScreensContainer);
    ui->chartContainer->setContentsMargins(0,0,0,0);
    setCssProperty(ui->chartContainer, "container-chart");
}

void DashboardWidget::changeChartColors(){
    QColor gridLineColorX;
    QColor linePenColorY;
    QColor backgroundColor;
    QColor gridY;
    if(isLightTheme()){
        gridLineColorX = QColor(255,255,255);
        linePenColorY = gridLineColorX;
        backgroundColor = linePenColorY;
        axisY->setGridLineColor(QColor("#1a000000"));
    }else{
        gridY = QColor("#40ffffff");
        axisY->setGridLineColor(gridY);
        gridLineColorX = QColor(15,11,22);
        linePenColorY =  gridLineColorX;
        backgroundColor = linePenColorY;
    }

    axisX->setGridLineColor(gridLineColorX);
    axisY->setLinePenColor(linePenColorY);
    chart->setBackgroundBrush(QBrush(backgroundColor));
    if (set0) set0->setBorderColor(gridLineColorX);
    if (set1) set1->setBorderColor(gridLineColorX);
}

// pair BWK, zBWK
QMap<int, std::pair<qint64, qint64>> DashboardWidget::getAmountBy() {
    if (chartShow != ALL) {
        bool filterByMonth = false;
        if (monthFilter != 0 && chartShow == MONTH) {
            filterByMonth = true;
        }
        if (yearFilter != 0) {
            if (filterByMonth) {
                QDate monthFirst = QDate(yearFilter, monthFilter, 1);
                stakesFilter->setDateRange(
                        QDateTime(monthFirst),
                        QDateTime(QDate(yearFilter, monthFilter, monthFirst.daysInMonth()))
                );
            } else {
                stakesFilter->setDateRange(
                        QDateTime(QDate(yearFilter, 1, 1)),
                        QDateTime(QDate(yearFilter, 12, 31))
                );
            }
        } else if (filterByMonth) {
            QDate currentDate = QDate::currentDate();
            QDate monthFirst = QDate(currentDate.year(), monthFilter, 1);
            stakesFilter->setDateRange(
                    QDateTime(monthFirst),
                    QDateTime(QDate(currentDate.year(), monthFilter, monthFirst.daysInMonth()))
            );
            ui->comboBoxYears->setCurrentText(QString::number(currentDate.year()));
        } else {
            stakesFilter->clearDateRange();
        }
    } else {
        stakesFilter->clearDateRange();
    }
    int size = stakesFilter->rowCount();
    QMap<int, std::pair<qint64, qint64>> amountBy;
    // Get all of the stakes
    for (int i = 0; i < size; ++i) {
        QModelIndex modelIndex = stakesFilter->index(i, TransactionTableModel::ToAddress);
        qint64 amount = llabs(modelIndex.data(TransactionTableModel::AmountRole).toLongLong());
        QDate date = modelIndex.data(TransactionTableModel::DateRole).toDateTime().date();
        bool isPiv = modelIndex.data(TransactionTableModel::TypeRole).toInt() != TransactionRecord::StakeZBWK;

        int time = 0;
        switch (chartShow) {
            case YEAR: {
                time = date.month();
                break;
            }
            case ALL: {
                time = date.year();
                break;
            }
            case MONTH: {
                time = date.day();
                break;
            }
            default:
                inform(tr("Error loading chart, invalid show option"));
                return amountBy;
        }
        if (amountBy.contains(time)) {
            if (isPiv) {
                amountBy[time].first += amount;
            } else
                amountBy[time].second += amount;
        } else {
            if (isPiv) {
                amountBy[time] = std::make_pair(amount, 0);
            } else {
                amountBy[time] = std::make_pair(0, amount);
                hasZTelosStakes = true;
            }
        }
    }
    return amountBy;
}

void DashboardWidget::loadChartData(bool withMonthNames) {

    if (chartData) {
        delete chartData;
        chartData = nullptr;
    }

    chartData = new ChartData();

    chartData->amountsByCache = getAmountBy(); // pair BWK, zBWK
    std::pair<int,int> range = getChartRange(chartData->amountsByCache);
    bool isOrderedByMonth = chartShow == MONTH;
    int daysInMonth = QDate(yearFilter, monthFilter, 1).daysInMonth();

    for (int j = range.first; j < range.second; j++) {
        int num = (isOrderedByMonth && j > daysInMonth) ? (j % daysInMonth) : j;
        qreal piv = 0;
        qreal zbwk = 0;
        if (chartData->amountsByCache.contains(num)) {
            std::pair <qint64, qint64> pair = chartData->amountsByCache[num];
            piv = (pair.first != 0) ? pair.first / 100000000 : 0;
            zbwk = (pair.second != 0) ? pair.second / 100000000 : 0;
            chartData->totalPiv += pair.first;
            chartData->totalZTelos += pair.second;
        }

        chartData->xLabels << ((withMonthNames) ? monthsNames[num - 1] : QString::number(num));

        chartData->valuesPiv.append(piv);
        chartData->valueszTelos.append(zbwk);

        int max = std::max(piv, zbwk);
        if (max > chartData->maxValue) {
            chartData->maxValue = max;
        }
    }
}

void DashboardWidget::onChartYearChanged(const QString& yearStr) {
    if (isChartInitialized) {
        int newYear = yearStr.toInt();
        if (newYear != yearFilter) {
            yearFilter = newYear;
            refreshChart();
        }
    }
}

void DashboardWidget::onChartMonthChanged(const QString& monthStr) {
    if (isChartInitialized) {
        int newMonth = ui->comboBoxMonths->currentData().toInt();
        if (newMonth != monthFilter) {
            monthFilter = newMonth;
            refreshChart();
#ifndef Q_OS_MAC
        // quick hack to re paint the chart view.
        chart->removeSeries(series);
        chart->addSeries(series);
#endif
        }
    }
}

bool DashboardWidget::refreshChart(){
    isChartMin = width() < 1300;
    isChartInitialized = false;
    showHideEmptyChart(true, true);
    return execute(REQUEST_LOAD_TASK);
}

void DashboardWidget::onChartRefreshed() {
    if (chart) {
        if(series){
            series->clear();
            series->detachAxis(axisX);
            series->detachAxis(axisY);
        }
        axisX->clear();
    }
    // init sets
    set0 = new QBarSet("BWK");
    set1 = new QBarSet("zBWK");
    set0->setColor(QColor(92,75,125));
    set1->setColor(QColor(176,136,255));

    if(!series) {
        series = new QBarSeries();
        chart->addSeries(series);
    }
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    set0->append(chartData->valuesPiv);
    set1->append(chartData->valueszTelos);

    // Total
    nDisplayUnit = walletModel->getOptionsModel()->getDisplayUnit();
    if (chartData->totalPiv > 0 || chartData->totalZTelos > 0) {
        setCssProperty(ui->labelAmountPiv, "text-stake-piv");
        setCssProperty(ui->labelAmountZTelos, "text-stake-zbwk");
    } else {
        setCssProperty(ui->labelAmountPiv, "text-stake-piv-disable");
        setCssProperty(ui->labelAmountZTelos, "text-stake-zbwk-disable");
    }
    forceUpdateStyle({ui->labelAmountPiv, ui->labelAmountZTelos});
    ui->labelAmountPiv->setText(GUIUtil::formatBalance(chartData->totalPiv, nDisplayUnit));
    ui->labelAmountZTelos->setText(GUIUtil::formatBalance(chartData->totalZTelos, nDisplayUnit, true));

    series->append(set0);
    if(hasZTelosStakes)
        series->append(set1);

    // bar width
    if (chartShow == YEAR)
        series->setBarWidth(0.8);
    else {
        series->setBarWidth(0.3);
    }
    axisX->append(chartData->xLabels);
    axisY->setRange(0, chartData->maxValue);

    // Controllers
    switch (chartShow) {
        case ALL: {
            ui->container_chart_dropboxes->setVisible(false);
            break;
        }
        case YEAR: {
            ui->container_chart_dropboxes->setVisible(true);
            ui->containerBoxMonths->setVisible(false);
            break;
        }
        case MONTH: {
            ui->container_chart_dropboxes->setVisible(true);
            ui->containerBoxMonths->setVisible(true);
            break;
        }
        default: break;
    }

    // Refresh years filter, first address created is the start
    int yearStart = QDateTime::fromTime_t(static_cast<uint>(walletModel->getCreationTime())).date().year();
    int currentYear = QDateTime::currentDateTime().date().year();

    QString selection;
    if (ui->comboBoxYears->count() > 0) {
        selection = ui->comboBoxYears->currentText();
        isChartInitialized = false;
    }
    ui->comboBoxYears->clear();
    if (yearStart == currentYear) {
        ui->comboBoxYears->addItem(QString::number(currentYear));
    } else {
        for (int i = yearStart; i < (currentYear + 1); ++i)ui->comboBoxYears->addItem(QString::number(i));
    }

    if (!selection.isEmpty()) {
        ui->comboBoxYears->setCurrentText(selection);
        isChartInitialized = true;
    } else {
        ui->comboBoxYears->setCurrentText(QString::number(currentYear));
    }

    // back to normal
    isChartInitialized = true;
    showHideEmptyChart(false, false, true);
}

std::pair<int, int> DashboardWidget::getChartRange(QMap<int, std::pair<qint64, qint64>> amountsBy) {
    switch (chartShow) {
        case YEAR:
            return std::make_pair(1, 13);
        case ALL: {
            QList<int> keys = amountsBy.uniqueKeys();
            qSort(keys);
            return std::make_pair(keys.first(), keys.last() + 1);
        }
        case MONTH:
            return std::make_pair(dayStart, dayStart + 9);
        default:
            inform(tr("Error loading chart, invalid show option"));
            return std::make_pair(0, 0);
    }
}

void DashboardWidget::updateAxisX(const QStringList* args) {
    axisX->clear();
    QStringList months;
    std::pair<int,int> range = getChartRange(chartData->amountsByCache);
    if (args) {
        months = *args;
    } else {
        for (int i = range.first; i < range.second; i++) months << QString::number(i);
    }
    axisX->append(months);
}

void DashboardWidget::onChartArrowClicked() {
    dayStart--;
    if (dayStart == 0) {
        dayStart = QDate(yearFilter, monthFilter, 1).daysInMonth();
    }
    refreshChart();
}

void DashboardWidget::windowResizeEvent(QResizeEvent *event){
    if (hasStakes() > 0 && axisX) {
        if (width() > 1300) {
            if (isChartMin) {
                isChartMin = false;
                switch (chartShow) {
                    case YEAR: {
                        updateAxisX(&monthsNames);
                        break;
                    }
                    case ALL: break;
                    case MONTH: {
                        updateAxisX();
                        break;
                    }
                    default:
                        inform(tr("Error loading chart, invalid show option"));
                        return;
                }
                chartView->repaint();
            }
        } else {
            if (!isChartMin) {
                updateAxisX();
                isChartMin = true;
            }
        }
    }
}

bool DashboardWidget::hasStakes() {
    return stakesFilter->rowCount() > 0;
}

#endif

void DashboardWidget::run(int type) {
#ifdef USE_QTCHARTS
    if (type == REQUEST_LOAD_TASK) {
        bool withMonthNames = !isChartMin && (chartShow == YEAR);
        loadChartData(withMonthNames);
        QMetaObject::invokeMethod(this, "onChartRefreshed", Qt::QueuedConnection);
    }
#endif
}
void DashboardWidget::onError(int type, QString error) {
    inform(tr("Error loading chart: %1").arg(error));
}

void DashboardWidget::processNewTransaction(const QModelIndex& parent, int start, int /*end*/) {
    // Prevent notifications-spam when initial block download is in progress
    if (!walletModel || !clientModel || clientModel->inInitialBlockDownload())
        return;

    if (!txModel || txModel->processingQueuedTransactions())
        return;

    QString date = txModel->index(start, TransactionTableModel::Date, parent).data().toString();
    qint64 amount = txModel->index(start, TransactionTableModel::Amount, parent).data(Qt::EditRole).toULongLong();
    QString type = txModel->index(start, TransactionTableModel::Type, parent).data().toString();
    QString address = txModel->index(start, TransactionTableModel::ToAddress, parent).data().toString();

    emit incomingTransaction(date, walletModel->getOptionsModel()->getDisplayUnit(), amount, type, address);
}

DashboardWidget::~DashboardWidget(){
#ifdef USE_QTCHARTS
    delete chart;
#endif
    delete ui;
}
