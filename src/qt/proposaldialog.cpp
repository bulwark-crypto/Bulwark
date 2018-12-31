// Copyright (c) 2017-2018 The Bulwark Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include <config/bulwark-config.h>
#endif

#include <fstream>
#include <iostream>
#include <string>

#include <base58.h>
#include <bitcoingui.h>
#include <boost/tokenizer.hpp>
#include <guiutil.h>
#include <main.h>
#include <masternode-budget.h>
#include <masternode-sync.h>
#include <proposaldialog.h>
#include <qtmaterialflatbutton.h>
#include <ui_proposaldialog.h>
#include <univalue.h>
#include <utilstrencodings.h>
#include <wallet.h>

#include <QMessageBox>
#include <QString>
#include <QTimer>

ProposalDialog::ProposalDialog(Mode mode, QWidget* parent) : QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint), ui(new Ui::ProposalDialog), mapper(0), mode(mode), counter(0) {
    ui->setupUi(this);

    switch (mode) {
    case PrepareProposal:
        setWindowTitle(tr("Prepare Proposal"));
        ui->confirmLabel->setVisible(false);
        ui->hashEdit->setVisible(false);
        ui->hashLabel->setVisible(false);
        break;
    case SubmitProposal:
        setWindowTitle(tr("Submit Proposal"));
        ui->confirmLabel->setVisible(true);
        ui->hashEdit->setVisible(true);
        ui->hashLabel->setVisible(true);
        break;
    }

    ui->nameEdit->setFont(GUIUtil::bitcoinAddressFont());
    ui->nameEdit->setPlaceholderText(tr("Provide a proposal name, keep it short"));
    ui->nameEdit->setToolTip(tr("Provide a proposal name, keep it short"));

    ui->urlEdit->setFont(GUIUtil::bitcoinAddressFont());
    ui->urlEdit->setPlaceholderText(tr("Valid http or https URL (include http:// or https://"));
    ui->urlEdit->setToolTip(tr("Valid http or https URL (include http:// or https://"));

    ui->paymentsEdit->setFont(GUIUtil::bitcoinAddressFont());
    ui->paymentsEdit->setPlaceholderText(tr("How many monthly payments, 1 per budget cycle"));
    ui->paymentsEdit->setToolTip(tr("How many monthly payments, 1 per budget cycle"));
    ui->paymentsEdit->setValidator(new QIntValidator(1, 100));

    ui->blockEdit->setFont(GUIUtil::bitcoinAddressFont());
    ui->blockEdit->setPlaceholderText(tr("The governance budget block for the desired cycle (default to next)"));
    ui->blockEdit->setToolTip(tr("The governance budget block for the desired cycle (default to next)"));
    ui->blockEdit->setValidator(new QIntValidator(1, INT_MAX));

    ui->addressEdit->setFont(GUIUtil::bitcoinAddressFont());
    ui->addressEdit->setPlaceholderText(tr("The Bulwark public address that will receive the funds"));
    ui->addressEdit->setToolTip(tr("The Bulwark public address that will receive the funds"));

    ui->amountEdit->setFont(GUIUtil::bitcoinAddressFont());
    ui->amountEdit->setPlaceholderText(tr("The amount to be paid per month or cycle"));
    ui->amountEdit->setToolTip(tr("The amount to be paid per month or cycle"));
    ui->amountEdit->setValidator(new QIntValidator(10, INT_MAX));

    ui->hashEdit->setFont(GUIUtil::bitcoinAddressFont());
    ui->hashEdit->setPlaceholderText(tr("The TXID of the proposal hash, must be confirmed before use"));
    ui->hashEdit->setToolTip(tr("The TXID of the proposal hash, must be confirmed before use"));

    ui->confirmLabel->setWordWrap(true);
    ui->infoLabel->setWordWrap(true);

    // Load next superblock number.
    CBlockIndex* pindexPrev = chainActive.Tip();
    if (!pindexPrev) return;
    int nNext = pindexPrev->nHeight - pindexPrev->nHeight % GetBudgetPaymentCycleBlocks() + GetBudgetPaymentCycleBlocks();
    ui->blockEdit->setText(QString::number(nNext));

    // Start periodic updates to handle submit block depth validation.
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkProposalTX()));
}

ProposalDialog::~ProposalDialog() {
    timer->stop();
    delete ui;
}

void ProposalDialog::prepareProposal() {
    std::string strError = "";

    if (pwalletMain->IsLocked()) {
        strError = "Error: Please enter the wallet passphrase with walletpassphrase first.";
        QMessageBox::critical(this, "Prepare Proposal Error", QString::fromStdString(strError));
        return;
    }

    std::string strProposalName = SanitizeString(ui->nameEdit->text().toStdString());
    std::string strURL = SanitizeString(ui->urlEdit->text().toStdString());
    int nPaymentCount = ui->paymentsEdit->text().toInt();
    int nBlockStart = ui->blockEdit->text().toInt();
    CBitcoinAddress address(ui->addressEdit->text().toStdString());
    CScript scriptPubKey = GetScriptForDestination(address.Get());
    CAmount nAmount = AmountFromValue(ui->amountEdit->text().toInt());

    //*************************************************************************

    // create the proposal incase we're the first to make it
    CBudgetProposalBroadcast budgetProposalBroadcast(strProposalName, strURL, nPaymentCount, scriptPubKey, nAmount, nBlockStart, 0);
    std::string err;
    if (!budgetProposalBroadcast.IsValid(err, false)) strError = "Proposal is not valid - " + budgetProposalBroadcast.GetHash().ToString() + " - " + err;

    bool useIX = false;
    if (strError.empty() && !pwalletMain->GetBudgetSystemCollateralTX(wtx, budgetProposalBroadcast.GetHash(), useIX)) {
        strError = "Error making collateral transaction for proposal. Please check your wallet balance.";
    }

    // make our change address
    CReserveKey reservekey(pwalletMain);
    // send the tx to the network
    if (strError.empty() && !pwalletMain->CommitTransaction(wtx, reservekey, useIX ? "ix" : "tx")) {
        strError = "Unable to commit proposal transaction.";
    }

    if (!strError.empty()) {
        std::cout << strError << std::endl;
        QMessageBox::critical(this, "Prepare Proposal Error", QString::fromStdString(strError));
        return;
    }

    // update the local view with submit view
    ui->cancelButton->setDisabled(true);
    ui->nameEdit->setDisabled(true);
    ui->urlEdit->setDisabled(true);
    ui->paymentsEdit->setDisabled(true);
    ui->blockEdit->setDisabled(true);
    ui->addressEdit->setDisabled(true);
    ui->amountEdit->setDisabled(true);
    ui->hashEdit->setDisabled(true);

    ui->acceptButton->setDisabled(true);
    ui->acceptButton->setText(tr("Waiting..."));

    ui->confirmLabel->setVisible(true);
    ui->confirmLabel->setText(QString::fromStdString("Waiting for confirmations..."));

    ui->hashEdit->setText(QString::fromStdString(wtx.GetHash().ToString()));
    ui->hashEdit->setVisible(true);
    ui->hashLabel->setVisible(true);

    mode = SubmitProposal;
    setWindowTitle(tr("Submit Proposal"));

    timer->start(1000);
    counter = chainActive.Tip()->nHeight + 1;
}

void ProposalDialog::submitProposal() {
    std::string strError = "";

    std::string strProposalName = SanitizeString(ui->nameEdit->text().toStdString());
    std::string strURL = SanitizeString(ui->urlEdit->text().toStdString());
    int nPaymentCount = ui->paymentsEdit->text().toInt();
    int nBlockStart = ui->blockEdit->text().toInt();
    CBitcoinAddress address(ui->addressEdit->text().toStdString());
    CScript scriptPubKey = GetScriptForDestination(address.Get());
    CAmount nAmount = AmountFromValue(ui->amountEdit->text().toInt());
    uint256 hash = ParseHashV(ui->hashEdit->text().toStdString(), "parameter 1");

    //*************************************************************************

    // create the proposal incase we're the first to make it
    int nConf = 0;
    std::string err = "";
    CBudgetProposalBroadcast budgetProposalBroadcast(strProposalName, strURL, nPaymentCount, scriptPubKey, nAmount, nBlockStart, hash);
    if (!IsBudgetCollateralValid(hash, budgetProposalBroadcast.GetHash(), err, budgetProposalBroadcast.nTime, nConf)) {
        strError = "Proposal FeeTX is not valid - " + hash.ToString() + " - " + err;
    }

    if (strError.empty() && !budget.AddProposal(budgetProposalBroadcast)) strError = "Invalid proposal, see debug.log for details.";

    if (!strError.empty()) {
        QMessageBox::critical(this, tr("Submit Proposal Error"), QString::fromStdString(strError));
        return;
    }

    budget.mapSeenMasternodeBudgetProposals.insert(make_pair(budgetProposalBroadcast.GetHash(), budgetProposalBroadcast));
    budgetProposalBroadcast.Relay();

    this->accept();
}

bool ProposalDialog::validateProposal() {
    std::string strError = "";

    if (!masternodeSync.IsBlockchainSynced()) strError = "Must wait for client to sync with masternode network. Try again in a minute or so.";

    std::string strProposalName = SanitizeString(ui->nameEdit->text().toStdString());
    if (strProposalName.size() > 20) strError = "Invalid proposal name, limit of 20 characters.";

    std::string strURL = SanitizeString(ui->urlEdit->text().toStdString());
    if (strURL.size() > 64) strError = "Invalid url, limit of 64 characters.";

    int nPaymentCount = ui->paymentsEdit->text().toInt();
    if (nPaymentCount < 1) strError = "Invalid payment count, must be more than zero.";

    // Start must be in the next budget cycle
    int nBlockMin = 0;
    CBlockIndex* pindexPrev = chainActive.Tip();
    if (pindexPrev != NULL) nBlockMin = pindexPrev->nHeight - pindexPrev->nHeight % GetBudgetPaymentCycleBlocks() + GetBudgetPaymentCycleBlocks();

    int nBlockStart = ui->blockEdit->text().toInt();
    if (nBlockStart < nBlockMin) strError = "Invalid block start, must be more than current height.";
    if (nBlockStart % GetBudgetPaymentCycleBlocks() != 0) {
        int nNext = pindexPrev->nHeight - pindexPrev->nHeight % GetBudgetPaymentCycleBlocks() + GetBudgetPaymentCycleBlocks();
        strError = strprintf("Invalid block start - must be a budget cycle block. Next valid block: %d", nNext);
    }

    int nBlockEnd = nBlockStart + (GetBudgetPaymentCycleBlocks() * nPaymentCount); // End must be AFTER current cycle
    if (nBlockEnd < pindexPrev->nHeight) strError = "Invalid ending block, starting block + (payment_cycle*payments) must be more than current height.";

    CBitcoinAddress address(ui->addressEdit->text().toStdString());
    if (!address.IsValid()) strError = "Invalid Bulwark address";

    if (!strError.empty()) {
        QMessageBox::critical(this, tr("Submit Proposal Error"), QString::fromStdString(strError));
        return false;
    }

    return true;
}

void ProposalDialog::checkProposalTX() {
    if (mode != SubmitProposal) return;

    int nConf = Params().Budget_Fee_Confirmations();
    int nDepth = (chainActive.Tip()->nHeight + 1) - counter;
    if (nDepth > nConf) {
        ui->acceptButton->setDisabled(false);
        ui->acceptButton->setText("Finish Proposal");
        ui->confirmLabel->setText(tr("Click on Finish Proposal to complete the proposal and start voting."));

        timer->stop();
    } else if (nDepth == nConf) {
        ui->confirmLabel->setText(QString::fromStdString("Waiting for final confirmation..."));
    } else if (nDepth > 0) {
        ui->confirmLabel->setText(QString::fromStdString(strprintf("Currently %d of %d confirmations...", nDepth, (nConf + 1)).c_str()));
    }
}

void ProposalDialog::on_acceptButton_clicked() {
    if (!validateProposal()) return;

    if (mode == PrepareProposal) {
        prepareProposal();
    } else if (mode == SubmitProposal) {
        submitProposal();
    }
}

void ProposalDialog::on_cancelButton_clicked() {
    this->reject();
}

