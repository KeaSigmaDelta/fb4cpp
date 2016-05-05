#include "Node.h"
#include "Session.h"
#include "SyncHTTPRequest.h"
#include "Settings.h"
#include "PageNode.h"

#include "json/json.h"

#include <assert.h>

using namespace fl4cpp;

Node::Node( Session* session, ProfileNode* rootNode, std::string nodeType, bool deletable ) :
_session( session ),
_rootNode( rootNode ),
_nodeType( nodeType ),
_deletable( deletable ),
_nodeId( "" )
{
    assert( "Session is null." && session );
    
    if ( !IsRootNode() )
        assert( "Root node is null" && rootNode );
}

bool Node::initFromGraphApi( std::string nodeId )
{
    assert( "Can't init from graph api without node id." && nodeId.size() );
    
    std::string accessToken;
    
    if ( getRootNode()->getNodeType() == kPageNodeType )
        accessToken = getSession()->getPageAccessToken( getRootNode()->getNodeId() );
    else
        accessToken = getSession()->getAccessToken();
    
    assert( "Can't init from graph api without access token." && accessToken.size() );

    std::string uri;
    uri.append( Settings::shared()->getGraphApiUrl() );
    uri.append( "/" + nodeId );
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
        
        initWithJson( &value );
        return true;
    }
    
    std::cout << "No data was received.\n";
    return false;
}

bool Node::initWithJson( Json::Value* nodeData )
{
    if ( !(*nodeData)[ "id" ].isNull() )
    {
        setNodeId( (*nodeData)[ "id" ].asString() );
    }
    else
    {
        std::cout << "No value for key \"id\", can't initialize node.";
        return false;
    }
    
    return true;
}

bool Node::refresh()
{
    return initFromGraphApi( getNodeId() );
}

bool Node::deleteFromGraphApi( std::string rootNodeId )
{
    if ( !_deletable )
    {
        std::cout << "Node with type \"" << getNodeType() << "\" is not a deletable node.";
        return false;
    }
    
    assert( "Can't delete node from graph api without access token." && getSession()->getAccessToken().size() );
    assert( "Can't delete node from graph api without knowing node id." && getNodeId().size() );
    assert( "Can't delete node from graph api without knowing root node id." && rootNodeId.size() );
    
    std::string uri;
    uri.append( Settings::shared()->getGraphApiUrl() );
    uri.append( "/" + rootNodeId );
    uri.append( "_" + getNodeId() );
    uri.append( "?access_token=" + getSession()->getAccessToken() );
    
    std::string receivedData;
    
    SyncHTTPRequest request;
    if ( request.performDelete( uri, &receivedData ) )
    {
        if ( receivedData == "true" )
            return true;
    }

    std::cout << "Failed to delete node with id: " << getNodeId() << ". Remember, that application can only delete nodes created by it.";
    return false;
}

bool Node::getNodeFromGraphApi( Session* session, ProfileNode* rootNode, std::string nodeId, Json::Value* receivedData, bool withMetadata, Json::Value* metadata )
{
    std::string accessToken;
    
    if ( rootNode->getNodeType() == kPageNodeType )
        accessToken = session->getPageAccessToken( rootNode->getNodeId() );
    else
        accessToken = session->getAccessToken();
    
    assert( "Can't init from graph api without access token." && accessToken.size() );
    assert( "Can't get node from graph api without node id." && nodeId.size() );
    
    std::string uri;
    uri.append( Settings::shared()->getGraphApiUrl() );
    uri.append( "/" + nodeId );
    uri.append( "?access_token=" + accessToken );
    if ( withMetadata )
        uri.append( "&metadata=1" );
    
    std::string receivedDataString;
    
    SyncHTTPRequest request;
    if ( request.performGet( uri, &receivedDataString ) )
    {
        Json::Reader reader;
        
        if ( !reader.parse( receivedDataString, *receivedData, false ) )
        {
            std::cout << "Failed to parse json data.\n";
            return false;
        }
        
        if ( !withMetadata )
            return true;
        
        if ( (*receivedData)[ "metadata" ].isNull() )
        {
            std::cout << "No metadata is present.";
            return false;
        }
        
        *metadata = Json::Value( (*receivedData)[ "metadata" ] );
        return true;
    }
    
    std::cout << "No data was received.";
    return false;
}