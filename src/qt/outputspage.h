// Copyright (c) 2018 The Phore developers
// Copyright (c) 2018 The Curium developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_OUTPUTSPAGE_H
#define BITCOIN_QT_OUTPUTSPAGE_H

#include <QAbstractButton>
#include <QAction>
#include <QDialog>
#include <QList>
#include <QMenu>
#include <QPoint>
#include <QString>
#include <QTreeWidgetItem>
#include <univalue.h>

class WalletModel;

namespace Ui
{
class OutPutsPage;
}

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

/** Widget that shows a list of sending or receiving addresses.
  */
class OutPutsPage : public QDialog
{
    Q_OBJECT

public:

    explicit OutPutsPage(QWidget* parent);
    ~OutPutsPage();
	QString  getmasternodeoutputs();
    const QString& getReturnValue() const { return returnValue; }

public slots:

private:
    Ui::OutPutsPage* ui;
    QMenu* contextMenu;
	QString key;
    QString returnValue;

private slots:
    /** Copy address of currently selected address entry to clipboard */

signals:
    void getOutPuts(QString addr);
};

#endif // BITCOIN_QT_ADDRESSBOOKPAGE_H
