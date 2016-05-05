#include "UserNode.h"
#include "SyncHTTPRequest.h"
#include "Settings.h"
#include "Session.h"

#include "json/json.h"

#include <assert.h>

using namespace fl4cpp;

UserNode::UserNode( Session* session ) :
ProfileNode( session, this, kUserNodeType, false ),
_ageRange( "", "" ),
_birthday( "" ),
_cover( "", "", 0 ),
_email( "" ),
_firstName( "" ),
_gender( "" ),
_lastName( "" ),
_link( "" ),
_locale( "" ),
_location( "" ),
_middleName( "" ),
_name( "" ),
_timeZone( 0 ),
_accountsEdge( session, this, this ),
_albumsEdge( session, this, this ),
_feedEdge( session, this, this ),
_statusesEdge( session, this, this ),
_pictureEdge( session, this, this )
{
}

bool UserNode::initWithJson( Json::Value* nodeData )
{
    if ( !Node::initWithJson( nodeData ) )
        return false;

    if ( !(*nodeData)[ "age_range" ].isNull() )
    {
        Json::Value ageRangeObject = (*nodeData)[ "age_range" ];
        setAgeRange( AgeRange( ageRangeObject[ "min" ].asString(), ageRangeObject[ "max" ].asString() ) );
    }
    
    if ( !(*nodeData)[ "birthday" ].isNull() )
        setBirthday( (*nodeData)[ "birthday" ].asString() );
    
    /// Seems deprecated. Use "picture" edge instead.
    if ( !(*nodeData)[ "cover" ].isNull() )
    {
        Json::Value coverObject = (*nodeData)[ "cover" ];
        
        if( !coverObject[ "id" ].isNull() && !coverObject[ "source" ] && !coverObject[ "offset_y" ].isNull() )
        {
            _cover = CoverPhoto( coverObject[ "id" ].asString(), coverObject[ "source" ].asString(), coverObject[ "offset_y" ].asInt() );
        }
    }
    
    if ( !(*nodeData)[ "email" ].isNull() )
        setEmail( (*nodeData)[ "email" ].asString() );
    
    if ( !(*nodeData)[ "first_name" ].isNull() )
        setFirstName( (*nodeData)[ "first_name" ].asString() );
    
    if ( !(*nodeData)[ "gender" ].isNull() )
        setGender( (*nodeData)[ "gender" ].asString() );
    
    if ( !(*nodeData)[ "last_name" ].isNull() )
        setLastName( (*nodeData)[ "last_name" ].asString() );
    
    if ( !(*nodeData)[ "link" ].isNull() )
        setLink( (*nodeData)[ "link" ].asString() );
    
    if ( !(*nodeData)[ "locale" ].isNull() )
        setLocale( (*nodeData)[ "locale" ].asString() );
    
    if ( !(*nodeData)[ "location" ].isNull() )
        setLocation( (*nodeData)[ "location" ].asString() );
    
    if ( !(*nodeData)[ "middle_name" ].isNull() )
        setMiddleName( (*nodeData)[ "middle_name" ].asString() );
    
    if ( !(*nodeData)[ "name" ].isNull() )
        setName( (*nodeData)[ "name" ].asString() );
    
    if ( !(*nodeData)[ "timezone" ].isNull() )
        setTimeZone( (*nodeData)[ "timezone" ].asInt() );
    
    return true;
}

AccountsEdge* UserNode::getAccountsEdge( bool initFromGraphApi /* = true */ )
{
    assert( "Can't get edge from node without node id." && getNodeId().size() );

    if ( initFromGraphApi )
    {
        if ( !_accountsEdge.initFromGraphApi() )
        {
            std::cout << "AccountsEdge::initFromGraphApi() failed.\n";
            return 0;
        }
    }
    
    return &_accountsEdge;
}

AlbumsEdge* UserNode::getAlbumsEdge( bool initFromGraphApi /* = true */ )
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

FeedEdge* UserNode::getFeedEdge( bool initFromGraphApi /* = true */ )
{
    assert( "Can't get edge from node without node id." && getNodeId().size() );
    
    if ( initFromGraphApi )
    {
        if ( !_feedEdge.initFromGraphApi() )
        {
            std::cout << "FeedEdge::initFromGraphApi() failed.\n";
            return 0;
        }
    }
    
    return &_feedEdge;
}

StatusesEdge* UserNode::getStatusesEdge( bool initFromGraphApi /* = true */ )
{
    assert( "Can't get edge from node without node id." && getNodeId().size() );
    
    if ( initFromGraphApi )
    {
        if ( !_statusesEdge.initFromGraphApi() )
        {
            std::cout << "StatusesEdge::initFromGraphApi() failed.\n";
            return 0;
        }
    }
    
    return &_statusesEdge;
}

PictureEdge* UserNode::getPictureEdge( bool initFromGraphApi /* = true */ )
{
    assert( "Can't get edge from node without node id." && getNodeId().size() );
    
    if ( initFromGraphApi )
    {
        if ( !_pictureEdge.initFromGraphApi() )
        {
            std::cout << "PictureEdge::initFromGraphApi( \"" << getNodeId() << "\" ) failed.\n";
            return 0;
        }
    }
    
    return &_pictureEdge;
}
