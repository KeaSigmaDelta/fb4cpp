#include "PhotosEdge.h"
#include "Settings.h"
#include "SyncHTTPRequest.h"
#include "Session.h"
#include "PhotoNode.h"

#include "json/json.h"

#include <assert.h>

using namespace fl4cpp;

PhotosEdge::PhotosEdge( Session* session, Node* ownerNode, ProfileNode* rootNode ) :
Edge( session, ownerNode, rootNode, kPhotosEdgeType ),
_photoNodes()
{
}

PhotosEdge::~PhotosEdge()
{
    clearReferredNodes( (std::vector< Node* >*)&_photoNodes );
}

bool PhotosEdge::initWithJson( Json::Value* edgeData )
{
    if ( !Edge::initWithJson( edgeData ) )
        return false;
    
    if ( !isEdgeDataContainsReferredNodesData( edgeData ) )
        return false;
    
    clearReferredNodes( (std::vector< Node* >*)&_photoNodes );
    
    Json::Value nodesData = (*edgeData)[ "data" ];
    
    for ( unsigned int i = 0; i < nodesData.size(); i++ )
    {
        PhotoNode* photoNode = new PhotoNode( getSession(), getRootNode() );
        if ( photoNode->initWithJson( &nodesData[ i ] ) )
        {
            _photoNodes.push_back( photoNode );
        }
        else
        {
            std::cout << "Failed to initialize Status node.";
            delete photoNode;
        }
    }
    
    return true;
}

bool PhotosEdge::publishUrl( std::string imageUrl, std::string imageDescription, bool noStory )
{
    assert( "Can't publish without owner node id." && getOwnerNode()->getNodeId().size() );
    assert( "Can't publish without root node id." && getRootNode()->getNodeId().size() );
    
    std::string accessToken;
    
    if ( getRootNode()->getNodeType() == kPageNodeType )
        accessToken = getSession()->getPageAccessToken( getRootNode()->getNodeId() );
    else
        accessToken = getSession()->getAccessToken();
    
    assert( "Can't publish without access token." && accessToken.size() );
    
    std::replace( imageDescription.begin(), imageDescription.end(), ' ', '+' );
    
    std::string uri;
    uri.append( Settings::shared()->getGraphApiUrl() );
    uri.append( "/" + getOwnerNode()->getNodeId() );
    uri.append( "/" + getEdgeType() );
    uri.append( "?url=" + imageUrl );
    uri.append( "&message=" + imageDescription );
    uri.append( "&access_token=" + accessToken );
    uri.append( "&no_story=" );
    if ( noStory )
        uri.append( "true" );
    else
        uri.append( "false" );
    
    std::string dataToSend;
    std::string receivedData;
    
    SyncHTTPRequest request;
    if ( request.performPost( uri, &dataToSend, &receivedData ) )
    {
        Json::Value value;
        Json::Reader reader;
        
        if ( !reader.parse( receivedData, value, false ) )
        {
            std::cout << "Failed to parse json data.\n";
            return false;
        }
            
        if ( value[ "id" ].isNull() )
        {
            std::cout << "No value for key \"id\".\n";
            return false;
        }
            
        return true;
    }
    
    return false;
}

bool PhotosEdge::publishData( void* imageData, unsigned long dataSize, std::string imageDescription, bool noStory )
{
    assert( "Can't publish without owner node id." && getOwnerNode()->getNodeId().size() );
    assert( "Can't publish without root node id." && getRootNode()->getNodeId().size() );
    
    std::string accessToken;
    
    if ( getRootNode()->getNodeType() == kPageNodeType )
        accessToken = getSession()->getPageAccessToken( getRootNode()->getNodeId() );
    else
        accessToken = getSession()->getAccessToken();
    
    assert( "Can't publish without access token." && accessToken.size() );
    
    std::replace( imageDescription.begin(), imageDescription.end(), ' ', '+' );
    
    std::string uri;
    uri.append( Settings::shared()->getGraphApiUrl() );
    uri.append( "/" + getOwnerNode()->getNodeId() );
    uri.append( "/" + getEdgeType() );
    uri.append( "?message=" + imageDescription );
    uri.append( "&access_token=" + accessToken );
    uri.append( "&no_story=" );
    if ( noStory )
        uri.append( "true" );
    else
        uri.append( "false" );
    
    std::string dataToSend;
    std::string receivedData;
    
    SyncHTTPRequest request;
    if ( request.performPostFormWithData( uri, "source", imageData, dataSize, "multipart/form-data", &receivedData ) )
    {
        Json::Value value;
        Json::Reader reader;
        
        if ( !reader.parse( receivedData, value, false ) )
        {
            std::cout << "Failed to parse json data.\n";
            return false;
        }
        
        if ( value[ "id" ].isNull() )
        {
            std::cout << "No value for key \"id\".\n";
            std::cout << "Json content: " << value.toStyledString() << "\n";
            return false;
        }
        
        return true;
    }

    return false;
}