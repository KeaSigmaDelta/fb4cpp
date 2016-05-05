#include "AlbumsEdge.h"
#include "Settings.h"
#include "SyncHTTPRequest.h"
#include "Session.h"
#include "AlbumNode.h"

#include "json/json.h"

#include <assert.h>

using namespace fl4cpp;

AlbumsEdge::AlbumsEdge( Session* session, Node* ownerNode, ProfileNode* rootNode ) :
Edge( session, ownerNode, rootNode, kAlbumsEdgeType ),
_albumNodes()
{
}

AlbumsEdge::~AlbumsEdge()
{
    clearReferredNodes( (std::vector< Node* >*)&_albumNodes );
}

bool AlbumsEdge::initWithJson( Json::Value* edgeData )
{
    if ( !Edge::initWithJson( edgeData ) )
        return false;
    
    if ( !isEdgeDataContainsReferredNodesData( edgeData ) )
        return false;
    
    clearReferredNodes( (std::vector< Node* >*)&_albumNodes );
    
    Json::Value nodesData = (*edgeData)[ "data" ];
    
    for ( unsigned int i = 0; i < nodesData.size(); i++ )
    {
        AlbumNode* albumNode = new AlbumNode( getSession(), getRootNode() );
        if ( albumNode->initWithJson( &nodesData[ i ] ) )
        {
            _albumNodes.push_back( albumNode );
        }
        else
        {
            std::cout << "Failed to initialize Status node.";
            delete albumNode;
        }
    }
    
    return true;
}

bool AlbumsEdge::publish( std::string albumName, std::string albumDiscription, PrivacySettingTypes::types privacyType )
{
    assert( "Can't publish without owner node id." && getOwnerNode()->getNodeId().size() );
    assert( "Can't publish without root node id." && getRootNode()->getNodeId().size() );
    
    std::string accessToken;
    
    if ( getRootNode()->getNodeType() == kPageNodeType )
        accessToken = getSession()->getPageAccessToken( getRootNode()->getNodeId() );
    else
        accessToken = getSession()->getAccessToken();
    
    assert( "Can't publish without access token." && accessToken.size() );
    
    std::replace( albumName.begin(), albumName.end(), ' ', '+' );
    std::replace( albumDiscription.begin(), albumDiscription.end(), ' ', '+' );
    
    std::string uri;
    uri.append( Settings::shared()->getGraphApiUrl() );
    uri.append( "/" + getOwnerNode()->getNodeId() );
    uri.append( "/" + getEdgeType() );
    uri.append( "?name=" + albumName );
    uri.append( "&message=" + albumDiscription );
    uri.append( "&access_token=" + accessToken );
    
    if ( privacyType != PrivacySettingTypes::DEFAULT )
    {
        Json::Value privasySettingsObject;
        privasySettingsObject[ "value" ] = PrivacySettings::shared()->getPrivacySettingString( privacyType );
        
        Json::FastWriter fastWriter;
        
        uri.append( "&privacy=" );
        uri.append( fastWriter.write( privasySettingsObject ) );
    }
    
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
            std::cout << "Json content: " << value.toStyledString() << "\n";
            return false;
        }
            
        return true;
    }

    return false;
}