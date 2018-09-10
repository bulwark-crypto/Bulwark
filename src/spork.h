// Copyright (c) 2014-2016 The Dash developers
// Copyright (c) 2016-2017 The PIVX developers
// Copyright (c) 2017-2018 The Bulwark developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SPORK_H
#define SPORK_H

#include "base58.h"
#include "key.h"
#include "main.h"
#include "net.h"
#include "sync.h"
#include "util.h"

#include "obfuscation.h"
#include "protocol.h"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

/*
    Don't ever reuse these IDs for other sporks
    - This would result in old clients getting confused about which spork is for what
*/
#define SPORK_START 10001
#define SPORK_END 10021

#define SPORK_2_SWIFTTX 10001
#define SPORK_3_SWIFTTX_BLOCK_FILTERING 10002
#define SPORK_5_MAX_VALUE 10004
#define SPORK_7_MASTERNODE_SCANNING 10006
#define SPORK_8_MASTERNODE_PAYMENT_ENFORCEMENT 10007
#define SPORK_9_MASTERNODE_BUDGET_ENFORCEMENT 10008
#define SPORK_10_MASTERNODE_PAY_UPDATED_NODES 10009
#define SPORK_11_RESET_BUDGET 10010
#define SPORK_12_RECONSIDER_BLOCKS 10011
#define SPORK_13_ENABLE_SUPERBLOCKS 10012
#define SPORK_14_NEW_PROTOCOL_ENFORCEMENT 10013
#define SPORK_15_NEW_PROTOCOL_ENFORCEMENT_2 10014
#define SPORK_16_MN_WINNER_MINIMUM_AGE 10015
#define SPORK_17_NEW_PROTOCOL_ENFORCEMENT_3 10016
#define SPORK_18_NEW_PROTOCOL_ENFORCEMENT_4 10017
#define SPORK_19_POW_ROLLBACK 10018
#define SPORK_20_NEW_PROTOCOL_DYNAMIC 10019
#define SPORK_21_ENABLE_ZEROCOIN 10020
#define SPORK_22_ZEROCOIN_MAINTENANCE_MODE 10021

#define SPORK_2_SWIFTTX_DEFAULT 978307200                         //2001-1-1
#define SPORK_3_SWIFTTX_BLOCK_FILTERING_DEFAULT 1424217600        //2015-2-18
#define SPORK_5_MAX_VALUE_DEFAULT 1000                            //1000 PIV
#define SPORK_7_MASTERNODE_SCANNING_DEFAULT 978307200             //2001-1-1
#define SPORK_8_MASTERNODE_PAYMENT_ENFORCEMENT_DEFAULT 4070908800 //OFF
#define SPORK_9_MASTERNODE_BUDGET_ENFORCEMENT_DEFAULT 4070908800  //OFF
#define SPORK_10_MASTERNODE_PAY_UPDATED_NODES_DEFAULT 1529804022  //ON
#define SPORK_11_RESET_BUDGET_DEFAULT 0
#define SPORK_12_RECONSIDER_BLOCKS_DEFAULT 0
#define SPORK_13_ENABLE_SUPERBLOCKS_DEFAULT 4070908800            //OFF
#define SPORK_14_NEW_PROTOCOL_ENFORCEMENT_DEFAULT 1512087450      //ON
#define SPORK_15_NEW_PROTOCOL_ENFORCEMENT_2_DEFAULT 1512087450    //ON
#define SPORK_16_MN_WINNER_MINIMUM_AGE_DEFAULT 8000                // Age in seconds. This should be > MASTERNODE_REMOVAL_SECONDS to avoid
                                                                   // misconfigured new nodes in the list. 
                                                                   // Set this to zero to emulate classic behaviour
#define SPORK_17_NEW_PROTOCOL_ENFORCEMENT_3_DEFAULT 1529303404    //ON
#define SPORK_18_NEW_PROTOCOL_ENFORCEMENT_4_DEFAULT 4070908800    //OFF
#define SPORK_19_POW_ROLLBACK_DEFAULT 4070908800                  //OFF
#define SPORK_20_NEW_PROTOCOL_DYNAMIC_DEFAULT 4070908800          //OFF
                                                                   // Will be whatever value is provided during spork update.
                                                                   // Example `spork SPORK_20_NEW_PROTOCOL_DYNAMIC 70850` will set active
                                                                   // protocol version to `70850`.
#define SPORK_21_ENABLE_ZEROCOIN_DEFAULT 4070908800               //OFF
#define SPORK_22_ZEROCOIN_MAINTENANCE_MODE_DEFAULT 4070908800     //OFF

class CSporkMessage;
class CSporkManager;

extern std::map<uint256, CSporkMessage> mapSporks;
extern std::map<int, CSporkMessage> mapSporksActive;
extern CSporkManager sporkManager;

void LoadSporksFromDB();
void ProcessSpork(CNode* pfrom, std::string& strCommand, CDataStream& vRecv);
int64_t GetSporkValue(int nSporkID);
bool IsSporkActive(int nSporkID);
void ExecuteSpork(int nSporkID, int nValue);
void ReprocessBlocks(int nBlocks);

//
// Spork Class
// Keeps track of all of the network spork settings
//

class CSporkMessage
{
public:
    std::vector<unsigned char> vchSig;
    int nSporkID;
    int64_t nValue;
    int64_t nTimeSigned;

    uint256 GetHash()
    {
        uint256 n = Nist5(BEGIN(nSporkID), END(nTimeSigned));
        return n;
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(nSporkID);
        READWRITE(nValue);
        READWRITE(nTimeSigned);
        READWRITE(vchSig);
    }
};


class CSporkManager
{
private:
    std::vector<unsigned char> vchSig;
    std::string strMasterPrivKey;

public:
    CSporkManager()
    {
    }

    std::string GetSporkNameByID(int id);
    int GetSporkIDByName(std::string strName);
    bool UpdateSpork(int nSporkID, int64_t nValue);
    bool SetPrivKey(std::string strPrivKey);
    bool CheckSignature(CSporkMessage& spork);
    bool Sign(CSporkMessage& spork);
    void Relay(CSporkMessage& msg);
};

#endif
