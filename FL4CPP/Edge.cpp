#include "Edge.h"
#include "Settings.h"
#include "Session.h"
#include "SyncHTTPRequest.h"
#include "PageNode.h"

#include <assert.h>

#include "json/json.h"

using namespace fl4cpp;

Edge::Edge( Session* session, Node* ownerNode, ProfileNode* rootNode, std::string edgeType ) :
_session( session ),
_ownerNode( ownerNode ),
_rootNode( rootNode ),
_edgeType( edgeType )
{
    assert( "Session is null." && session );
    assert( "Owner node is null." && ownerNode );
    assert( "Root node is null." && rootNode );
}

bool Edge::initFromGraphApi()
{
    assert( "Can't init from graph api without owner node id." && getOwnerNode()->getNodeId().size() );
    assert( "Can't init from graph api without root node id." && getRootNode()->getNodeId().size() );
    
    std::string accessToken;
    
    if ( getRootNode()->getNodeType() == kPageNodeType )
        accessToken = getSession()->getPageAccessToken( getRootNode()->getNodeId() );
    else
        accessToken = getSession()->getAccessToken();
    
    assert( "Can't init from graph api without access token." && accessToken.size() );
    
    std::string uri;
    uri.append( Settings::shared()->getGraphApiUrl() );
    uri.append( "/" + getOwnerNode()->getNodeId() );
    uri.append( "/" + getEdgeType() );
    uri.append( "?access_token=" + getSession()->getAccessToken() );
    
    std::string receivedData;
    
    SyncHTTPRequest request;
    if ( request.performGet( uri, &receivedData ) )
    {
        Json::Value value;
        Json::Reader reader;
        
        if ( !reader.parse( receivedData, value, false ) )
        {
            std::cout << "Failed to parse json data.\n";
            return false;
        }
        
        return initWithJson( &value );
    }
    
    std::cout << "No data was received.\n";
    return false;
}

bool Edge::initWithJson( Json::Value* edgeData )
{
    return true;
}

void Edge::clearReferredNodes( std::vector< Node* >* nodes )
{
    for ( unsigned int i = 0; i < nodes->size(); i++ )
    {
        delete nodes->at( i );
    }
    
    nodes->clear();
}

bool Edge::isEdgeDataContainsReferredNodesData( Json::Value* edgeData )
{
    if ( (*edgeData)[ "data" ].isNull() )
    {
        std::cout << "No value for key \"data\".\n";
        std::cout << "Json content: " << edgeData->toStyledString() << "\n";
        return false;
    }

    if ( !(*edgeData)[ "data" ].isArray() )
    {
        std::cout << "Value for key \"data\" is not an array.\n";
        return false;
    }
    
    return true;
}