// Copyright (c) 2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CONTACTSDROPDOWN_H
#define CONTACTSDROPDOWN_H

#include "addresstablemodel.h"
#include "qt/bulwark/pwidget.h"
#include "qt/bulwark/contactdropdownrow.h"
#include "qt/bulwark/furabstractlistitemdelegate.h"
#include "qt/bulwark/addressfilterproxymodel.h"
#include <QListView>
#include <QObject>
#include <QWidget>


class ContactsViewDelegate;
class ContViewHolder;
class WalletModel;


QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

class ContactsDropdown : public PWidget
{
    Q_OBJECT
public:
    explicit ContactsDropdown(int minWidth, int minHeight, PWidget *parent = nullptr);

    void resizeList(int minWidth, int mintHeight);
    void setWalletModel(WalletModel* _model, QString type);
    void changeTheme(bool isLightTheme, QString& theme) override;
signals:
    void contactSelected(QString address, QString label);
private:
    FurAbstractListItemDelegate* delegate;
    AddressTableModel* model;
    AddressFilterProxyModel *filter = nullptr;
    QListView *list;
    QFrame *frameList;
private slots:
    void handleClick(const QModelIndex &index);
};

#endif // CONTACTSDROPDOWN_H
