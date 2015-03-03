
#include "darksend-relay.h"


CDarkSendRelay::CDarkSendRelay()
{
    vinMasternode = CTxIn();
    nBlockHeight = 0;
    nRelayType = 0;
    in = CTxIn();
    out = CTxOut();
}

CDarkSendRelay::CDarkSendRelay(CTxIn& vinMasternodeIn, vector<unsigned char>& vchSigIn, int nBlockHeightIn, int nRelayTypeIn, CTxIn& in2, CTxOut& out2)
{
    vinMasternode = vinMasternodeIn;
    vchSig = vchSigIn;
    nBlockHeight = nBlockHeightIn;
    nRelayType = nRelayTypeIn;
    in = in2;
    out = out2;
}

std::string CDarkSendRelay::ToString()
{
    std::ostringstream info;

    info << "vin: " << vinMasternode.ToString() <<
        " nBlockHeight: " << (int)nBlockHeight <<
        " nRelayType: "  << (int)nRelayType <<
        " in " << in.ToString() <<
        " out " << out.ToString();
        
    return info.str();   
}

void CDarkSendRelay::Relay()
{
    int nCount = std::min(mnodeman.CountEnabled(), 20);
    int nRank1 = (rand() % nCount)+1; 
    int nRank2 = (rand() % nCount)+1; 

    //keep picking another second number till we get one that doesn't match
    while(nRank1 == nRank2) nRank2 = (rand() % nCount)+1;

    //printf("rank 1 - rank2 %d %d \n", nRank1, nRank2);

    //relay this message through 2 separate nodes for redundancy
    RelayThroughNode(nRank1);
    RelayThroughNode(nRank2);
    RelayThroughNode((rand() % nCount)+1);
    RelayThroughNode((rand() % nCount)+1);
    RelayThroughNode((rand() % nCount)+1);
    RelayThroughNode((rand() % nCount)+1);
    RelayThroughNode((rand() % nCount)+1);
}

void CDarkSendRelay::RelayThroughNode(int nRank)
{
    CMasternode* pmn = mnodeman.GetMasternodeByRank(nRank, nBlockHeight, MIN_POOL_PEER_PROTO_VERSION);

    if(pmn){
        //printf("RelayThroughNode %s\n", pmn->addr.ToString().c_str());
        if(ConnectNode((CAddress)pmn->addr, NULL, true)){
            //printf("Connected\n");
            CNode* pNode = FindNode(pmn->addr);
            if(pNode)
            {
                //printf("Found\n");
                pNode->PushMessage("dsr", (*this));
                return;
            }
        }
    } else {
        //printf("RelayThroughNode NULL\n");
    }
}