#include "FeedEdge.h"
#include "SyncHTTPRequest.h"
#include "Session.h"
#include "PageNode.h"

#include "json/json.h"

#include <assert.h>
#include <algorithm>

using namespace fl4cpp;

FeedEdge::FeedEdge( Session* session, Node* ownerNode, ProfileNode* rootNode ) :
Edge( session, ownerNode, rootNode, kFeedEdgeType )
{
}

FeedEdge::~FeedEdge()
{
}

bool FeedEdge::initFromGraphApi()
{
    // nothing to init
    return true;
}

bool FeedEdge::initWithJson( Json::Value *edgeData )
{
    // nothing to init
    return true;
}

bool FeedEdge::publish( std::string message, PrivacySettingTypes::types privacyType )
{
    assert( "Can't publish without owner node id." && getOwnerNode()->getNodeId().size() );
    assert( "Can't publish without root node id." && getRootNode()->getNodeId().size() );
    
    std::string accessToken;
    
    if ( getRootNode()->getNodeType() == kPageNodeType )
        accessToken = getSession()->getPageAccessToken( getRootNode()->getNodeId() );
    else
        accessToken = getSession()->getAccessToken();
    
    assert( "Can't publish without access token." && accessToken.size() );
    
    std::replace( message.begin(), message.end(), ' ', '+' );
    
    std::string uri;
    uri.append( Settings::shared()->getGraphApiUrl() );
    uri.append( "/" + getOwnerNode()->getNodeId() );
    uri.append( "/" + getEdgeType() );
    uri.append( "?message=" + message );
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