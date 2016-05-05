#include "ProfileNode.h"
#include "Node.h"
#include "UserNode.h"
#include "PageNode.h"
#include "Session.h"

#include "json/json.h"

#include <assert.h>

using namespace fl4cpp;

ProfileNode::ProfileNode( Session* session, ProfileNode* rootNode, std::string nodeType, bool deletable ) :
Node( session, rootNode, nodeType, deletable )
{
    
}

ProfileNode* ProfileNodeBuilder::build( Session* session, Json::Value* nodeData, Json::Value* metadata )
{
    if ( (*metadata)[ "type" ].isNull() )
    {
        std::cout << "No value for key \"type\", can't initialize profile node.";
        return 0;
    }
    
    std::string nodeType = (*metadata)[ "type" ].asString();
    
    ProfileNode* profileNode = 0;
    
    if ( nodeType == "user" )
    {
        profileNode = new UserNode( session );
    }
    else if ( nodeType == "page" )
    {
        profileNode = new PageNode( session );
    }
    else
    {
        std::cout << "Node type not supported yet, can't initialize profile node.";
        return 0;
    }
    
    if ( profileNode->initWithJson( nodeData ) )
        return profileNode;
    else
    {
        std::cout << "Failed to initialize profile node.";
        return 0;
    }
}