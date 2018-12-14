// Copyright (c) 2018 The Phore developers
// Copyright (c) 2018 The Curium developers
// Copyright (c) 2017-2018 The Bulwark Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BULWARK_QT_PROPOSALTABLEMODEL_H
#define BULWARK_QT_PROPOSALTABLEMODEL_H

#include "bitcoinunits.h"

#include <QAbstractTableModel>
#include <QStringList>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include "masternode-budget.h"
#include "masternode-payments.h"
#include "masternodeconfig.h"
#include "masternodeman.h"
#include "rpcserver.h"

#include "obfuscation.h"

class ProposalRecord;
class ProposalTablePriv;

class ProposalTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ProposalTableModel( QObject *parent = 0);
    ~ProposalTableModel();

    enum ColumnIndex {
        Proposal = 0,
        Amount = 1,
        StartDate = 2,
        EndDate = 3,
        TotalPaymentCount = 4,
        RemainingPaymentCount = 5,
        YesVotes = 6,
        NoVotes = 7,
        AbstainVotes = 8,
        Percentage = 9
    };

    enum RoleIndex {
        ProposalRole = Qt::UserRole,
        AmountRole,
        StartDateRole,
        EndDateRole,
        TotalPaymentCountRole,
        RemainingPaymentCountRole,
        YesVotesRole,
        NoVotesRole,
        AbstainVotesRole,
        PercentageRole,
        ProposalUrlRole,
        ProposalHashRole
    };

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    void refreshProposals();
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    void setProposalType(const int &type);

private:
    QNetworkAccessManager *networkManager;
    QNetworkReply *currentReply;

    QList<ProposalRecord*> proposalRecords;
    QStringList columns;

    int proposalType = 0;

public Q_SLOTS:
    void onResult(QNetworkReply *result);
};

#endif // BULWARK_QT_PROPOSALTABLEMODEL_H
