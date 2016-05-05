#include "AlbumNode.h"
#include "PhotosEdge.h"
#include "UserNode.h"
#include "ProfileNode.h"

#include "json/json.h"

#include <assert.h>

using namespace fl4cpp;

AlbumNode::AlbumNode( Session* session, ProfileNode* rootNode ) :
Node( session, rootNode, kAlbumNodeType, false ),
_canUpload( false ),
_count( 0 ),
_coverPhoto( "" ),
_createdTime( "" ),
_description( "" ),
_from( 0 ),
_link( "" ),
_location( "" ),
_name( "" ),
_place( session ),
_privacy( "" ),
_type( "" ),
_updatedTime( "" ),
_photosEdge( session, this, rootNode )
{
}

AlbumNode::~AlbumNode()
{
    delete _from;
}

bool AlbumNode::initWithJson( Json::Value* nodeData )
{
    if ( !Node::initWithJson( nodeData ) )
        return false;
    
    if ( !(*nodeData)[ "can_upload" ].isNull() )
        setCanUpload( (*nodeData)[ "can_upload" ].asBool() );
    
    if ( !(*nodeData)[ "count" ].isNull() )
        setCount( (*nodeData)[ "count" ].asInt() );
    
    if ( !(*nodeData)[ "cover_photo" ].isNull() )
        setCoverPhoto( (*nodeData)[ "cover_photo" ].asString() );
    
    if ( !(*nodeData)[ "created_time" ].isNull() )
        setCreatedTime( (*nodeData)[ "created_time" ].asString() );
    
    if ( !(*nodeData)[ "description" ].isNull() )
        setDescription( (*nodeData)[ "description" ].asString() );
    
    if ( !(*nodeData)[ "from" ].isNull() )
    {
        Json::Value fromField = (*nodeData)[ "from" ];
        setFromNodeId( fromField[ "id" ].asString() );
        setFromName( fromField[ "name" ].asString() );
    }
    
    if ( !(*nodeData)[ "link" ].isNull() )
        setLink( (*nodeData)[ "link" ].asString() );
    
    if ( !(*nodeData)[ "location" ].isNull() )
        setLocation( (*nodeData)[ "location" ].asString() );
    
    if ( !(*nodeData)[ "name" ].isNull() )
        setName( (*nodeData)[ "name" ].asString() );
    
    if ( !(*nodeData)[ "place" ].isNull() )
    {
        _place.initWithJson( &(*nodeData)[ "place" ] );
    }
    
    if ( !(*nodeData)[ "privacy" ].isNull() )
        setPrivacy( (*nodeData)[ "privacy" ].asString() );
    
    if ( !(*nodeData)[ "type" ].isNull() )
        setPrivacy( (*nodeData)[ "type" ].asString() );
    
    if ( !(*nodeData)[ "updated_time" ].isNull() )
        setUpdatedTime( (*nodeData)[ "updated_time" ].asString() );
    
    return true;
}

Node* AlbumNode::getFrom( bool initFromGraphApi /* = true */ )
{
    if ( !getFromNodeId().size() )
    {
        std::cout << "Status node doesn't contain node id of status author.";
        return 0;
    }
    
    if ( !initFromGraphApi && _from )
    {
        return _from;
    }
    
    Json::Value receivedData;
    Json::Value metaData;
    
    if ( !getNodeFromGraphApi( getSession(), getRootNode(), getFromNodeId(), &receivedData, true, &metaData ) )
    {
        std::cout << "Failed to get author node from graph api.";
        return 0;
    }
    
    _from = ProfileNodeBuilder::build( getSession(), &receivedData, &metaData );
    
    return _from;
}

PhotosEdge* AlbumNode::getPhotosEdge( bool initFromGraphApi /* = true */ )
{
    assert( "Can't get edge from node without node id." && getNodeId().size() );
    
    if ( initFromGraphApi )
    {
        if ( !_photosEdge.initFromGraphApi() )
        {
            std::cout << "PhotosEdge::initFromGraphApi() failed.\n";
            return 0;
        }
    }
    
    return &_photosEdge;
}