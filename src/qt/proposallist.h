// Copyright (c) 2018 The Phore developers
// Copyright (c) 2018 The Curium developers
// Copyright (c) 2018-2019 The Bulwark Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BULWARK_QT_PROPOSALLIST_H
#define BULWARK_QT_PROPOSALLIST_H

#include "columnalignedlayout.h"
#include "guiutil.h"
#include "proposaltablemodel.h"

#include <QKeyEvent>
#include <QTimer>
#include <QWidget>

class ProposalFilterProxy;

QT_BEGIN_NAMESPACE
class QComboBox;
class QDateTimeEdit;
class QFrame;
class QItemSelectionModel;
class QLineEdit;
class QMenu;
class QModelIndex;
class QSignalMapper;
class QTableView;
QT_END_NAMESPACE

#define PROPOSALLIST_UPDATE_SECONDS 30

class ProposalList : public QWidget {
    Q_OBJECT

  public:
    explicit ProposalList(QWidget *parent = 0);

    void setModel();

    enum DateEnum {
        All,
        Today,
        ThisWeek,
        ThisMonth,
        LastMonth,
        ThisYear,
        Range
    };

    enum ColumnWidths {
        PROPOSAL_COLUMN_WIDTH = 150,
        AMOUNT_COLUMN_WIDTH = 100,
        START_DATE_COLUMN_WIDTH = 110,
        END_DATE_COLUMN_WIDTH = 110,
        TOTAL_PAYMENT_COLUMN_WIDTH = 80,
        REMAINING_PAYMENT_COLUMN_WIDTH = 80,
        YES_VOTES_COLUMN_WIDTH = 60,
        NO_VOTES_COLUMN_WIDTH = 60,
        ABSTAIN_COLUMN_WIDTH = 60,
        PERCENTAGE_COLUMN_WIDTH = 80,
        MINIMUM_COLUMN_WIDTH = 23
    };

  private:
    ProposalTableModel *proposalTableModel;
    ProposalFilterProxy *proposalProxyModel;
    QTableView *proposalList;
    int64_t nLastUpdate = 0;

    QLineEdit *proposalWidget;
    QLineEdit *startDateWidget;
    QLineEdit *endDateWidget;
    QLineEdit *totalPaymentCountWidget;
    QLineEdit *remainingPaymentCountWidget;
    QTimer *timer;

    QLineEdit *yesVotesWidget;
    QLineEdit *noVotesWidget;
    QLineEdit *abstainVotesWidget;
    QLineEdit *amountWidget;
    QLineEdit *percentageWidget;
    QLabel *secondsLabel;

    QMenu *contextMenu;

    QLineEdit *proposalStartDate;

    QLineEdit *proposalEndDate;
    ColumnAlignedLayout *hlayout;

    /* Header - Info/Projection */
    QComboBox *proposalTypeCombo;
    QHBoxLayout *headLayout;
    /* End Header - Info/Projection */

    void vote_click_handler(const std::string voteString);

    GUIUtil::TableViewLastColumnResizingFixer *columnResizingFixer;

    virtual void resizeEvent(QResizeEvent* event);

  private Q_SLOTS:
    void createProposal();
    void proposalType(int type);
    void contextualMenu(const QPoint &);
    void voteYes();
    void voteNo();
    void voteAbstain();
    void copyProposalUrl();
    void invalidateAlignedLayout();

  Q_SIGNALS:
    void doubleClicked(const QModelIndex&);

  public Q_SLOTS:
    void refreshProposals(bool force = false);
    void changedProposal(const QString &proposal);
    void chooseStartDate(const QString &startDate);
    void chooseEndDate(const QString &endDate);
    void changedTotalPaymentCount(const QString &totalPaymentCount);
    void changedRemainingPaymentCount(const QString &remainingPaymentCount);
    void changedYesVotes(const QString &minYesVotes);
    void changedNoVotes(const QString &minNoVotes);
    void changedAbstainVotes(const QString &minAbstainVotes);
    void changedPercentage(const QString &minPercentage);
    void changedAmount(const QString &minAmount);
};

#endif // BULWARK_QT_PROPOSALLIST_H
