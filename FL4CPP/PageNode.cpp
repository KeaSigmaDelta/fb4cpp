#include "PageNode.h"

#include "json/json.h"

#include <assert.h>

using namespace fl4cpp;

PageNode::PageNode( Session* session ) :
ProfileNode( session, this, kPageNodeType, false ),
_category( "" ),
_checkins( 0 ),
_description( "" ),
_likes( 0 ),
_link( "" ),
_name( "" ),
_username( "" ),
_albumsEdge( session, this, this )
{
}

bool PageNode::initWithJson( Json::Value* nodeData )
{
    if ( !Node::initWithJson( nodeData ) )
        return false;
    
    if ( !(*nodeData)[ "category" ].isNull() )
        setCategory( (*nodeData)[ "category" ].asString() );
    
    if ( !(*nodeData)[ "checkins" ].isNull() )
        setCheckins( (*nodeData)[ "checkins" ].asInt() );
    
    if ( !(*nodeData)[ "description" ].isNull() )
        setDescription( (*nodeData)[ "description" ].asString() );
    
    if ( !(*nodeData)[ "likes" ].isNull() )
        setLikes( (*nodeData)[ "likes" ].asInt() );
    
    if ( !(*nodeData)[ "link" ].isNull() )
        setLink( (*nodeData)[ "link" ].asString() );
    
    if ( !(*nodeData)[ "name" ].isNull() )
        setName( (*nodeData)[ "name" ].asString() );
    
    if ( !(*nodeData)[ "username" ].isNull() )
        setUsername( (*nodeData)[ "username" ].asString() );

    return true;
}

AlbumsEdge* PageNode::getAlbumsEdge( bool initFromGraphApi /* = true */ )
{
    assert( "Can't get edge from node without node id." && getNodeId().size() );
    
    if ( initFromGraphApi )
    {
        if ( !_albumsEdge.initFromGraphApi() )
        {
            std::cout << "AlbumsEdge::initFromGraphApi() failed.\n";
            return 0;
        }
    }
    
    return &_albumsEdge;
}