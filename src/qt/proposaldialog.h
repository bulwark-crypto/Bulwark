// Copyright (c) 2017-2018 The Bulwark Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_CREATEPROPOSALDIALOG_H
#define BITCOIN_QT_CREATEPROPOSALDIALOG_H

#include <proposallist.h>
#include <qtmaterialflatbutton.h>
#include <string>
#include <univalue.h>
#include <wallet.h>

#include <QAction>
#include <QDialog>
#include <QString>
#include <QTimer>

class ProposalModel;

namespace Ui {
class ProposalDialog;
}

QT_BEGIN_NAMESPACE
class QDataWidgetMapper;
QT_END_NAMESPACE

/**
 * Dialog for creating a proposal.
 */
class ProposalDialog : public QDialog {
    Q_OBJECT

  public:
    enum Mode {
        PrepareProposal,
        SubmitProposal
    };

    explicit ProposalDialog(Mode mode, QWidget* parent);
    ~ProposalDialog();

  public slots:
    void checkProposalTX();
    void on_acceptButton_clicked();
    void on_cancelButton_clicked();

  private:
    Ui::ProposalDialog *ui;
    QDataWidgetMapper *mapper;
    Mode mode;
    QTimer *timer;
    CWalletTx wtx;
    int counter;

    void prepareProposal();
    void submitProposal();
    bool validateProposal();
};

#endif // BITCOIN_QT_CREATEPROPOSALDIALOG_H
