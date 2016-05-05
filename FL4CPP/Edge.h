#ifndef __FL4CPP_EDGE__
#define __FL4CPP_EDGE__

#include <string>
#include <vector>

namespace Json
{
    class Value;
}

namespace fl4cpp
{
    class Session;
    class Node;
    class ProfileNode;
    
    /** Base class for all classes representing Graph Api edges.
     * See: https://developers.facebook.com/docs/graph-api/quickstart/v2.0
     *
     * NOTE: Setter methods of the Edge class are used for initialization
     * of the local copy, and has no affect on Graph Api copy.
     */
    class Edge
    {
    public:
        /** Constructs Edge object.
         *
         * @param session Pointer to a current Session.
         * @param ownerNode Pointer to a node that hold this edge.
         * @param rootNode Pointer to a root node
         * (Profile Node on behalf of which all actions with this edge will be performed).
         * @param edgeType Edge type string.
         */
        Edge( Session* session, Node* ownerNode, ProfileNode* rootNode, std::string edgeType );
        
        virtual ~Edge() {};
        
        /** Gets edge type.
         *
         * @return Edge type.
         */
        std::string getEdgeType() const { return _edgeType; };
        
        /** Sets edge type.
         *
         * @param edgeType Edge type.
         */
        void setEdgeType( std::string edgeType ) { _edgeType = edgeType; };
        
        /** Requests Edge data from Graph Api and
         * initializes Edge object with received Json data.
         *
         * @return Returns true if edge data successfully received
         * and edge object initialized correctly,
         * otherwise - false.
         */
        virtual bool initFromGraphApi();
        
        /** Initializes Edge object with Json data.
         *
         * Should be implemented in derived class, since
         * base edges dont have any common fields.
         *
         * @param edgeData Json object with Edge data,
         * formatted as in usual response from Graph Api
         * on requesting a particular edge.
         *
         * @return Returns true if edge object initialized successfully,
         * otherwise - false.
         */
        virtual bool initWithJson( Json::Value* edgeData );
        
    protected:
        /** Gets current session.
         *
         * @return Pointer to a current session object.
         */
        Session*        getSession() const { return _session; };
        
        /** Gets owner node.
         *
         * @retun Pointer to a node that hold this edge.
         */
        Node*           getOwnerNode() const { return _ownerNode; };
        
        /** Gets root node.
         *
         * @return Pointer to a root node.
         * (Profile Node on behalf of which all actions with this edge will be performed).
         */
        ProfileNode*    getRootNode() const { return _rootNode; };
        
        /** Helper functon that clears a a list of node pointers.
         *
         * @param nodes Pointer to a list of node pointers.
         */
        static void clearReferredNodes( std::vector< Node* >* nodes );
        
        /** Helper function that tells if Json data of an edge contains an
         * array of node objects. Some edges used only for publishing and
         * dont hold any info, and others can hold data of some nodes that
         *
         * @param edgeData Json edge data to check.
         *
         * @return Returns true if json edge data contains array of
         * node objects.
         */
        static bool isEdgeDataContainsReferredNodesData( Json::Value* edgeData );
        
    private:
        
        /** Pointer to a current Session object.
         */
        Session*        _session;
        
        /** Pointer to an owner node.
         */
        Node*           _ownerNode;
        
        /** Pointer to a root node.
         */
        ProfileNode*    _rootNode;
        
        /** Edge type string.
         */
        std::string _edgeType;
    };
}

#endif // __FL4CPP_EDGE__
