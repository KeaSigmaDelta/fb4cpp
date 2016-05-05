#include "PictureEdge.h"
#include "Settings.h"
#include "SyncHTTPRequest.h"
#include "Session.h"

#include "json/json.h"

#include <assert.h>
#include <sstream>

using namespace fl4cpp;

PictureInfo::PictureInfo( std::string url, bool silhouette, int width, int height ) :
_url( url ),
_silhouette( silhouette ),
_width( width ),
_height( height )
{
    
}

PictureEdge::PictureEdge( Session* session, Node* ownerNode, ProfileNode* rootNode ) :
Edge( session, ownerNode, rootNode, kPictureEdgeType )
{
}

PictureEdge::~PictureEdge()
{
}

bool PictureEdge::initFromGraphApi()
{
    // nothing to init
    return true;
}

bool PictureEdge::initWithJson( Json::Value* edgeData )
{
    // nothing to init
    return true;
}

bool PictureEdge::readInfo( PictureTypes::types pictureType, PictureInfo *info )
{
    return readInfo( pictureType, 0, 0, info );
}

bool PictureEdge::readInfo( PictureTypes::types pictureType, int width, int height, PictureInfo *info )
{
    assert( "Can't read without owner node id." && getOwnerNode()->getNodeId().size() );
    assert( "Can't read without root node id." && getRootNode()->getNodeId().size() );
    
    /* this action does not require any access token
    std::string accessToken;
    
    if ( getRootNode()->getNodeType() == kPageNodeType )
        accessToken = ((PageNode*)getRootNode())->getPageAccessToken();
    else
        accessToken = getSession()->getAccessToken();
    
    assert( "Can't publish without access token." && accessToken.size() );
    */
    
    std::string uri;
    uri.append( Settings::shared()->getGraphApiUrl() );
    uri.append( "/" + getOwnerNode()->getNodeId() );
    uri.append( "/" + getEdgeType() );
    uri.append( "?type=" + pictureTypeToString( pictureType ) );
    uri.append( "&redirect=false" );
    
    if ( width )
    {
        std::stringstream widthStr;
        widthStr << width;
        uri.append( "&width=" + widthStr.str() );
    }
    if ( height )
    {
        std::stringstream heightStr;
        heightStr << height;
        uri.append( "&height=" + heightStr.str() );
    }
    
    //uri.append( "&access_token=" + accessToken);
    
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
        
        if ( value[ "data" ].isNull() )
        {
            std::cout << "No value for key \"data\".\n";
            std::cout << "Json content: " << value.toStyledString() << "\n";
            return false;
        }
        
        Json::Value infoObject = value[ "data" ];
        
        if ( !infoObject[ "url" ].isNull() )
            info->setUrl( infoObject[ "url" ].asString() );
        
        if ( !infoObject[ "is_silhouette" ].isNull() )
            info->setSilhouette( infoObject[ "is_silhouette" ].asBool() );
        
        if ( !infoObject[ "width" ].isNull() )
            info->setWidth( infoObject[ "width" ].asInt() );
        
        if ( !infoObject[ "height" ].isNull() )
            info->setHeight( infoObject[ "height" ].asInt() );
        
        return true;
    }
     
    std::cout << "No data was received.\n";
    return false;
    
}

std::string PictureEdge::pictureTypeToString( PictureTypes::types type )
{
    switch ( type )
    {
        case PictureTypes::SQUARE: return "square";
        case PictureTypes::SMALL: return "small";
        case PictureTypes::NORMAL: return "normal";
        case PictureTypes::LARGE: return "large";
        default: return "unknown_enum_value";
    }
}