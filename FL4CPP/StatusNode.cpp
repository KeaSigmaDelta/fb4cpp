#include "StatusNode.h"
#include "PageNode.h"
#include "Settings.h"
#include "Session.h"
#include "SyncHTTPRequest.h"
#include "UserNode.h"
#include "PageNode.h"
#include "ProfileNode.h"

#include "json/json.h"

#include <assert.h>

using namespace fl4cpp;

StatusNode::StatusNode( Session* session, ProfileNode* rootNode ) :
Node( session, rootNode, kStatusNodeType, true ),
_from( 0 ),
_message( "" ),
_place( session ),
_updatedTime( "" ),
_fromNodeId( "" ),
_fromName( "" )
{
}

StatusNode::~StatusNode()
{
    delete _from;
}

bool StatusNode::initWithJson( Json::Value* nodeData )
{
    if ( !Node::initWithJson( nodeData ) )
        return false;
    
    if ( !(*nodeData)[ "from" ].isNull() )
    {
        Json::Value fromField = (*nodeData)[ "from" ];
        setFromNodeId( fromField[ "id" ].asString() );
        setFromName( fromField[ "name" ].asString() );
    }
    
    if ( !(*nodeData)[ "message" ].isNull() )
        setMessage( (*nodeData)[ "message" ].asString() );
    
    if ( !(*nodeData)[ "place" ].isNull() )
    {
        _place.initWithJson( &(*nodeData)[ "place" ] );
    }
    
    if ( !(*nodeData)[ "updated_time" ].isNull() )
        setUpdatedTime( (*nodeData)[ "updated_time" ].asString() );
    
    return true;
}

Node* StatusNode::getFrom( bool initFromGraphApi /* = true */ )
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