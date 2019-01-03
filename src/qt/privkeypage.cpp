// Copyright (c) 2018 The Phore developers
// Copyright (c) 2018 The Curium developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/bulwark-config.h"
#endif

#include "privkeypage.h"
#include "ui_privkeypage.h"

#include "bitcoingui.h"
#include "csvmodelwriter.h"
#include "editaddressdialog.h"
#include "guiutil.h"
#include "masternode-budget.h"
#include "masternode-payments.h"
#include "masternodeconfig.h"
#include "masternodeman.h"

#include <univalue.h>
#include <QIcon>
#include <QMenu>
#include <QString>
#include <QMessageBox>
#include <QSortFilterProxyModel>

PrivKeyPage::PrivKeyPage(QWidget* parent) : QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::PrivKeyPage) {
    ui->setupUi(this);

#ifdef Q_OS_MAC // Icons on push buttons are very uncommon on Mac
    ui->copyAddress->setIcon(QIcon());
#endif
    key = PrivKeyPage::createmasternodekey();
    ui->payTo->setText(key);

    // Context menu actions
    QAction* copyAddressAction = new QAction(tr("&Copy Address"), this);

    // Build context menu
    contextMenu = new QMenu();
    contextMenu->addAction(copyAddressAction);
    contextMenu->addSeparator();

    // Connect signals for context menu actions
    connect(copyAddressAction, SIGNAL(triggered()), this, SLOT(on_copyAddress_clicked()));

    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(accept()));
}



PrivKeyPage::~PrivKeyPage() {
    delete ui;
}


void PrivKeyPage::on_copyAddress_clicked() {
    GUIUtil::setClipboard(ui->payTo->text());

}

QString PrivKeyPage::createmasternodekey() {
    CKey secret;
    secret.MakeNewKey(false);

    return QString::fromStdString(CBitcoinSecret(secret).ToString());
}
