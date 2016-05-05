#include "PhotoNode.h"
#include "ProfileNode.h"

#include "json/json.h"

#include <assert.h>

using namespace fl4cpp;

PhotoNode::PhotoNode( Session* session, ProfileNode* rootNode ) :
Node( session, rootNode, kPhotoNodeType, true ),
_album( session, rootNode ),
_backdatedTime( "" ),
_backdatedTimeGranularity( "" ),
_createdTime( "" ),
_from( 0 ),
_height( 0 ),
_icon( "" ),
_images(),
_link( "" ),
_name( "" ),
_nameTags(),
_pageStoryId( "" ),
_picture( "" ),
_place( session ),
_source( "" ),
_updatedTime( "" ),
_width( 0 ),
_fromNodeId( "" ),
_fromName( "" )
{
}

PhotoNode::~PhotoNode()
{
    delete _from;
    _images.clear();
    _nameTags.clear();
}

bool PhotoNode::initWithJson( Json::Value* nodeData )
{
    if ( !Node::initWithJson( nodeData ) )
        return false;
    
    if ( !(*nodeData)[ "album" ].isNull() )
    {
        _album.initWithJson( &(*nodeData)[ "album" ] );
    }
    
    if ( !(*nodeData)[ "backdated_time" ].isNull() )
        setBackdatedTime( (*nodeData)[ "backdated_time" ].asString() );
    
    if ( !(*nodeData)[ "backdated_time_granularity" ].isNull() )
        setBackdatedTimeGranularity( (*nodeData)[ "backdated_time_granularity" ].asString() );
    
    if ( !(*nodeData)[ "created_time" ].isNull() )
        setCreatedTime( (*nodeData)[ "created_time" ].asString() );
    
    if ( !(*nodeData)[ "from" ].isNull() )
    {
        Json::Value fromField = (*nodeData)[ "from" ];
        setFromNodeId( fromField[ "id" ].asString() );
        setFromName( fromField[ "name" ].asString() );
    }
    
    if ( !(*nodeData)[ "height" ].isNull() )
        setHeight( (*nodeData)[ "height" ].asInt() );
    
    if ( !(*nodeData)[ "icon" ].isNull() )
        setIcon( (*nodeData)[ "icon" ].asString() );
    
    if ( !(*nodeData)[ "images" ].isNull() && (*nodeData)[ "images" ].isArray() )
    {
        Json::Value imagesArray = (*nodeData)[ "images" ];
        
        for ( unsigned int i = 0; i < imagesArray.size(); i++ )
        {
            Json::Value imageSource = imagesArray[ i ];
            
            if ( !imageSource[ "width" ].isNull() && !imageSource[ "width" ].isNull() && !imageSource[ "width" ].isNull() )
            {
                _images.push_back( ImageSource( imageSource[ "width" ].asInt(),
                                               imageSource[ "height" ].asInt(),
                                               imageSource[ "source" ].asString() ) );
            }
        }
    }
    
    if ( !(*nodeData)[ "link" ].isNull() )
        setLink( (*nodeData)[ "link" ].asString() );
    
    if ( !(*nodeData)[ "name" ].isNull() )
        setName( (*nodeData)[ "name" ].asString() );
    
    /// Api documentation says that this key should be "name_tags" but in reality it is "tags"
    /// and has different internal structure
    if ( !(*nodeData)[ "tags" ].isNull() )
    {
        Json::Value nameTagsObject = (*nodeData)[ "tags" ];
        Json::Value tagsDataArray = nameTagsObject[ "data" ];
        
        if ( tagsDataArray.isArray() )
        {
            for ( unsigned int i = 0; i < tagsDataArray.size(); i++ )
            {
                Json::Value nameTag = tagsDataArray[ i ];
                
                if ( !nameTag[ "id" ].isNull() &&
                    !nameTag[ "name" ].isNull() &&
                    !nameTag[ "created_time" ].isNull() &&
                    !nameTag[ "x" ].isNull() &&
                    !nameTag[ "y" ].isNull() )
                {
                    _nameTags.push_back( NameTag( nameTag[ "id" ].asString(),
                                                 nameTag[ "name" ].asString(),
                                                 nameTag[ "created_time" ].asString(),
                                                 nameTag[ "x" ].asInt(),
                                                 nameTag[ "y" ].asInt() ) );
                }

            }
        }
    }
    
    if ( !(*nodeData)[ "page_story_id" ].isNull() )
        setPageStoryId( (*nodeData)[ "page_story_id" ].asString() );
    
    if ( !(*nodeData)[ "picture" ].isNull() )
        setPicture( (*nodeData)[ "picture" ].asString() );
    
    if ( !(*nodeData)[ "place" ].isNull() )
    {
        _place.initWithJson( &(*nodeData)[ "place" ] );
    }
    
    if ( !(*nodeData)[ "source" ].isNull() )
        setSource( (*nodeData)[ "source" ].asString() );
    
    if ( !(*nodeData)[ "updated_time" ].isNull() )
        setUpdatedTime( (*nodeData)[ "updated_time" ].asString() );
    
    if ( !(*nodeData)[ "width" ].isNull() )
        setWidth( (*nodeData)[ "width" ].asInt() );
    
    return true;
}

Node* PhotoNode::getFrom( bool initFromGraphApi /* = true */ )
{
    if ( !getFromNodeId().size() )
    {
        std::cout << "Photo node doesn't contain node id of Photo author.";
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