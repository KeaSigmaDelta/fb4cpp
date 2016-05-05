#include "StatusesEdge.h"
#include "StatusNode.h"
#include "Settings.h"
#include "SyncHTTPRequest.h"
#include "Session.h"

#include "json/json.h"

#include <assert.h>

using namespace fl4cpp;

StatusesEdge::StatusesEdge( Session* session, Node* ownerNode, ProfileNode* rootNode ) :
Edge( session, ownerNode, rootNode, kStatusesEdgeType ),
_statusNodes()
{
}

StatusesEdge::~StatusesEdge()
{
    clearReferredNodes( (std::vector< Node* >*)&_statusNodes );
}

bool StatusesEdge::initWithJson( Json::Value* edgeData )
{
    if ( !Edge::initWithJson( edgeData ) )
        return false;
    
    if ( !isEdgeDataContainsReferredNodesData( edgeData ) )
        return false;
    
    clearReferredNodes( (std::vector< Node* >*)&_statusNodes );
    
    Json::Value nodesData = (*edgeData)[ "data" ];
    
    for ( unsigned int i = 0; i < nodesData.size(); i++ )
    {
        StatusNode* statusNode = new StatusNode( getSession(), getRootNode() );
        if ( statusNode->initWithJson( &nodesData[ i ] ) )
        {
            _statusNodes.push_back( statusNode );
        }
        else
        {
            std::cout << "Failed to initialize Status node.";
            delete statusNode;
        }
    }
    
    return true;
}