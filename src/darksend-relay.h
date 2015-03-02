
// Copyright (c) 2014-2015 The Darkcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DARKSEND_RELAY_H
#define DARKSEND_RELAY_H

#include "core.h"
#include "main.h"
#include "activemasternode.h"
#include "masternodeman.h"


class CDarkSendRelay
{
public:
	CTxIn vinMasternode;
    vector<unsigned char> vchSig;
    int nBlockHeight;
    int nRelayType;
    CTxIn in;
    CTxOut out;

	CDarkSendRelay(CTxIn& vinMasternodeIn, vector<unsigned char>& vchSigIn,int nBlockHeightIn, int nRelayTypeIn, CTxIn& in2, CTxOut& out2)
    {
        vinMasternode = vinMasternodeIn;
        vchSig = vchSigIn;
        nBlockHeight = nBlockHeightIn;
        nRelayType = nRelayTypeIn;
        in = in2;
        out = out2;
	}

    IMPLEMENT_SERIALIZE
    (
    	READWRITE(vinMasternode);
    	READWRITE(vchSig);
		READWRITE(nBlockHeight);
		READWRITE(nRelayType);
		READWRITE(in);
		READWRITE(out);
    )

    void Relay()
    {
        for(int i = 0; i <= 1; i++) //relay through two peers
        {
            int nRank = rand() % 20; 
            CMasternode* pmn = mnodeman.GetMasternodeByRank(nRank, nBlockHeight, MIN_POOL_PEER_PROTO_VERSION);

            if(pmn){
                if(ConnectNode((CAddress)pmn->addr, NULL, true)){
                    CNode* pNode = FindNode(pmn->addr);
                    if(pNode)
                    {
                        pNode->PushMessage("dsr", (*this));
                        return;
                    }
                }
            }
        }
    }
};



#endif