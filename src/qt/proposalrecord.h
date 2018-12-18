// Copyright (c) 2018 The Phore developers
// Copyright (c) 2018 The Curium developers
// Copyright (c) 2017-2018 The Bulwark Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BULWARK_QT_PROPOSALRECORD_H
#define BULWARK_QT_PROPOSALRECORD_H

#include "amount.h"
#include "uint256.h"

#include <QList>
#include <QString>

class CWallet;

class ProposalRecord
{
public:
    ProposalRecord() :
        hash(""),
        start_epoch(0),
        end_epoch(0),
        totalPaymentCount(0),
        remainingPaymentCount(0),
        url(""),
        name(""),
        yesVotes(0),
        noVotes(0),
        abstainVotes(0),
        amount(0),
        percentage(0)
    {}

    ProposalRecord(QString hash, const CAmount& start_epoch, const CAmount& end_epoch, const int& totalPaymentCount, const int& remainingPaymentCount, QString url, QString name, const CAmount& yesVotes, const CAmount& noVotes, const CAmount& abstainVotes, const CAmount& amount, const CAmount& percentage) :
        hash(hash),
        start_epoch(start_epoch),
        end_epoch(end_epoch),
        totalPaymentCount(totalPaymentCount),
        remainingPaymentCount(remainingPaymentCount),
        url(url),
        name(name),
        yesVotes(yesVotes),
        noVotes(noVotes),
        abstainVotes(abstainVotes),
        amount(amount),
        percentage(percentage)
    {}

    QString hash;
    int start_epoch;
    int end_epoch;
    int totalPaymentCount;
    int remainingPaymentCount;
    QString url;
    QString name;
    int yesVotes;
    int noVotes;
    int abstainVotes;
    CAmount amount;
    double percentage;
};

#endif // BULWARK_QT_PROPOSALRECORD_H
