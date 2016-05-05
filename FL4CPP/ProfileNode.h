#ifndef __FL4CPP_PROFILE_NODE__
#define __FL4CPP_PROFILE_NODE__

#include "Node.h"

namespace Json
{
    class Value;
}

namespace fl4cpp
{
    class Session;
    
    /** Intermediate class, that restricts derived Nodes
     * to be one of the following type: User, Page, Group or Event.
     * See: https://developers.facebook.com/docs/graph-api/reference/v2.0/profile/
     */
    class ProfileNode : public Node
    {
    public:
        /** Constructs ProfileNode.
         *
         * @param session Pointer to a current Session object.
         * @param rootNode Pointer to a root node 
         * (Profile Node on behalf of which all actions with this node will be performed).
         * @param nodeType Node type string.
         * @param deletable Tells that Node can be deleted from Grapn API by the application.
         */
        ProfileNode( Session* session, ProfileNode* rootNode, std::string nodeType, bool deletable );
        
        virtual ~ProfileNode() {};
        
        /** Tells if node is a Root node.
         * All Profile nodes can be root nodes.
         *
         * WARNING: NOT SURE ABOUT Group and Event. !!!
         *
         * @return True if node is Root node.
         */
        virtual bool isRootNode() const { return true; };
    };
      
    class Session;
    class Node;
    class ProfileNode;
    
    /** Helper class that creates Profile Node 
     * based on metadata received from Graph API.
     *
     * Sometimes Graph API gives you full Profile node data,
     * but dont tells which Profile node it is (User or a Page...).
     * So you have to request metadata explicitly and parse 
     * node data in concordance with it.
     */
    class ProfileNodeBuilder
    {
    public:
        /** Creates a Profile node, depending on metadata
         * and fills it with node data.
         *
         * NOTE: Group and Event nodes not supported yet.
         *
         * @param session Pointer to a current Session object.
         * @param nodeData Node data, received from Facebook server.
         * @param nodeMetadata Node metadata.
         *
         * @return Returns pointer to a newly created Profile node,
         * or zero if node data or its metadata is errorous.
         */
        static ProfileNode* build( Session* session, Json::Value* nodeData, Json::Value* nodeMetadata );
    };
}

#endif // __FL4CPP_PROFILE_NODE__
