// Copyright (c) 2018 The Phore developers
// Copyright (c) 2018 The Curium developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_CONFIGUREMASTERNODEPAGE_H
#define BITCOIN_QT_CONFIGUREMASTERNODEPAGE_H

#include "masternodelist.h"
#include "wallet.h"

#include <QAbstractButton>
#include <QAction>
#include <QDialog>
#include <QList>
#include <QMenu>
#include <string>
#include <QPoint>
#include <QString>
#include <QTreeWidgetItem>
#include <univalue.h>

class WalletModel;
class MasternodeList;

namespace Ui {
class ConfigureMasternodePage;
}

QT_BEGIN_NAMESPACE
class QDataWidgetMapper;
QT_END_NAMESPACE

/** Dialog for editing an address and associated information.
 */
class ConfigureMasternodePage : public QDialog {
    Q_OBJECT

  public:
    enum Mode {
        NewConfigureMasternode,
        EditConfigureMasternode
    };

    explicit ConfigureMasternodePage(Mode mode, QWidget* parent);
    ~ConfigureMasternodePage();

    void counter(int counter);
    void MNAliasCache(QString MnAliasCache);
    void loadAlias(QString strAlias);
    void loadIP(QString strIP);
    void loadPrivKey(QString strPrivKey);
    void loadTxHash(QString strTxHash);
    void loadOutputIndex(QString strOutputIndex);
    void updateAlias(std::string Alias, std::string IP, std::string PrivKey, std::string TxHash, std::string OutputIndex, std::string mnAlias);
    int getCounters() {
        return counters;
    }

    void setCounters(int counter) {
        counters = counter;
    }

    QString getMnAliasCache() {
        return mnAliasCache;
    }

    void setMnAliasCache(QString mnAliasCaches) {
        mnAliasCache = mnAliasCaches;
    }

    QString getAddress() const;
    void setAddress(const QString& address);

  public slots:
    void on_acceptButton_clicked();
    void on_cancelButton_clicked();
    void on_AutoFillPrivKey_clicked();
    void on_AutoFillOutputs_clicked();

  private:
    void saveCurrentRow();
    int counters;
    QString mnAliasCache;
    Ui::ConfigureMasternodePage* ui;
    QDataWidgetMapper* mapper;
    Mode mode;

    QString address;
};

#endif // BITCOIN_QT_EDITADDRESSDIALOG_H
