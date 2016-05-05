#ifndef __FL4CPP_NODE__
#define __FL4CPP_NODE__

#include <string>

namespace Json
{
    class Value;
}

namespace fl4cpp
{
    class Session;
    class ProfileNode;
    
    /** Base class for all classes representing Graph Api nodes.
     * See: https://developers.facebook.com/docs/graph-api/quickstart/v2.0
     *
     * NOTE: Setter methods of the Node class are used for initialization 
     * of the local copy, and has no affect on Graph Api copy.
     */
    class Node
    {
    public:
        /** Constructs Node object.
         *
         * @param session Pointer to a current Session.
         * @param rootNode Pointer to a root node
         * (Profile Node on behalf of which all actions with this node will be performed).
         * @param nodeType Node type string.
         * @param deletable Tells that Node can be deleted from Grapn API by the application.
         */
        Node( Session* session, ProfileNode* rootNode, std::string nodeType, bool deletable );
        
        virtual ~Node() {};
        
        /** Requests Node data from Graph Api and 
         * initializes Node object with received Json data.
         *
         * @param nodeId Node Id of the desired node.
         *
         * @return Returns true if node data successfully received
         * and node object initialized correctly,
         * otherwise - false.
         */
        bool initFromGraphApi( std::string nodeId );
        
        /** Initializes Node object with Json data.
         *
         * Should be implemented in derived class, since
         * nodes usually contain more fields
         * than just "id".
         *
         * @param nodeData Json object with Node data,
         * formatted as in usual response from Graph Api
         * on requesting a particular node.
         *
         * @return Returns true if node object initialized successfully,
         * otherwise - false.
         */
        virtual bool initWithJson( Json::Value* nodeData );
        
        /** Requests Node data from Graph Api
         * for Node with current node id.
         *
         * @return Returns true if node data successfully received
         * and node object initialized correctly,
         * otherwise - false.
         */
        bool refresh();
        
        /** Gets node type string.
         *
         * @return Node type.
         */
        std::string getNodeType() const { return _nodeType; };
        
        /** Tries to delete node with current node id from Graph Api.
         * Not all node types can be deleted. And more than that:
         * application can only delete nodes that was created
         * by this application.
         *
         * @param rootNodeId Node Id of a Profile Node on behalf of which
         * application tries to delete a node.
         *
         * @return Returns true if node deleted successfully, 
         * otherwise - false.
         */
        bool deleteFromGraphApi( std::string rootNodeId );
        
        /** Tells if node is a root node.
         * By default only a Profile Node can be a root one.
         *
         * @return Returns true if root and false if not.
         */
        virtual bool IsRootNode() { return false; };
        
        // fields
        
        /** Gets Node Id.
         * 
         * @return Node Id string.
         */
        std::string getNodeId() const { return _nodeId; };
        
        /** Sets Node Id.
         *
         * @param nodeId Node Id string.
         */
        void setNodeId( std::string nodeId ) { _nodeId = nodeId; };
        
    protected:
        
        /** Gets current Session object.
         *
         * @return Pointer to a Session object.
         */
        Session* getSession() const { return _session; };
        
        /** Gets current root node.
         * 
         * @return Pointer to a root node.
         */
        ProfileNode* getRootNode() const { return _rootNode; };
        
        /** Helper function that requests a specific node
         * from the Graph Api, passing over fl4cpp classes hierarchy.
         *
         * @param session Pointer to a Session object.
         * @param rootNode Pointer to a root node
         * (Profile Node on behalf of which all actions with this node will be performed).
         * @param nodeId Node Id of desired node.
         * @param receivedData Pointer to a buffer for received data.
         * @param wihtMetaData If set to true - functon requests node metadata as well.
         * @param metadata Node metadata (if it was requested).
         *
         * @return Returns true if node data was received successfully,
         * otherwise - false.
         */
        static bool getNodeFromGraphApi( Session* session,
                                        ProfileNode* rootNode,
                                        std::string nodeId,
                                        Json::Value* receivedData,
                                        bool withMetadata,
                                        Json::Value* metadata );

    private:
        /** Pointer to a current Session object.
         */
        Session* _session;
        
        /** Pointer to a current root node.
         * (Profile Node on behalf of which all actions with this node will be performed).
         */
        ProfileNode* _rootNode;
      
        /** Type of this node.
         */
        std::string _nodeType;
        
        /** Flag that tells if this node can be deleted.
         */
        bool _deletable;

        // fields
        
        /** Content of "id" field.
         */
        std::string _nodeId;
    };
}

#endif // __FL4CPP_NODE__
