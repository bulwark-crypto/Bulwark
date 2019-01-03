#ifndef MASTERNODELIST_H
#define MASTERNODELIST_H

#include "masternode.h"
#include "platformstyle.h"
#include "sync.h"
#include "util.h"
#include "wallet.h"

#include <QMenu>
#include <QTimer>
#include <QWidget>

#include "walletmodel.h"

#include <QDialog>
#include <QString>

#define MY_MASTERNODELIST_UPDATE_SECONDS 60
#define MASTERNODELIST_UPDATE_SECONDS 15
#define MASTERNODELIST_FILTER_COOLDOWN_SECONDS 3

namespace Ui {
class MasternodeList;
}

class ClientModel;
class WalletModel;

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

/** Masternode Manager page widget */
class MasternodeList : public QWidget {
    Q_OBJECT

  public:
    explicit MasternodeList(QWidget* parent = 0);
    ~MasternodeList();

    void setClientModel(ClientModel* clientModel);
    void setWalletModel(WalletModel* walletModel);
    void StartAlias(std::string strAlias);
    void StartAll(std::string strCommand = "start-all");
    void deleteAlias(std::string Alias);

  private:
    QMenu* contextMenu;
    int64_t nTimeFilterUpdated;
    bool fFilterUpdated;

  public Q_SLOTS:
    void updateMyMasternodeInfo(QString strAlias, QString strAddr, CMasternode* pmn);
    void updateMyNodeList(bool fForce = false);

  Q_SIGNALS:

  private:
    QTimer* timer;
    Ui::MasternodeList* ui;
    ClientModel* clientModel;
    WalletModel* walletModel;
    CCriticalSection cs_mnlistupdate;
    QString strCurrentFilter;

  private Q_SLOTS:
    void showContextMenu(const QPoint&);
    void deleteAlias();
    void copyAlias();
    void on_startButton_clicked();
    void on_editConfigureMasternode_clicked();
    void on_startAllButton_clicked();
    void on_startMissingButton_clicked();
    void on_configureMasternodeButton_clicked();
    void openEditConfigureMasternodePage(QString strAlias, QString strIP, QString strPrivKey, QString strTxHash, QString strOutputIndex, int count);
    void on_getMNPrivKeyButton_clicked();
    void on_getOutputsButton_clicked();
    void on_tableWidgetMyMasternodes_itemSelectionChanged();
    void on_UpdateButton_clicked();
};
#endif // MASTERNODELIST_H
