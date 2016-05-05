#include "AccountsEdge.h"
#include "PageNode.h"
#include "Settings.h"
#include "SyncHTTPRequest.h"
#include "Session.h"

#include "json/json.h"

#include <assert.h>

using namespace fl4cpp;

AccountsEdge::AccountsEdge( Session* session, Node* ownerNode, ProfileNode* rootNode ) :
Edge( session, ownerNode, rootNode, kAccountsEdgeType ),
_pageNodes()
{
}

AccountsEdge::~AccountsEdge()
{
    clearReferredNodes( (std::vector< Node* >*)&_pageNodes );
}

bool AccountsEdge::initWithJson( Json::Value* edgeData )
{
    if ( !Edge::initWithJson( edgeData ) )
        return false;
    
    if ( !isEdgeDataContainsReferredNodesData( edgeData ) )
        return false;
    
    clearReferredNodes( (std::vector< Node* >*)&_pageNodes );
    
    Json::Value nodesData = (*edgeData)[ "data" ];
    
    for ( unsigned int i = 0; i < nodesData.size(); i++ )
    {
        PageNode* pageNode = new PageNode( getSession() );
        if ( pageNode->initWithJson( &nodesData[ i ] ) )
        {
            /// we should redownload the page node beacuse accounts edge doesn't return all available fields.
            pageNode->refresh();
            
            _pageNodes.push_back( pageNode );
        }
        else
        {
            std::cout << "Failed to initialize Page node.";
            delete pageNode;
        }
    }
    
    return true;
}
