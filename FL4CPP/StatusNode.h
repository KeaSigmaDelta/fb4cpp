#ifndef __FL4CPP_STATUS_NODE__
#define __FL4CPP_STATUS_NODE__

#include "Node.h"
#include "PageNode.h"

#include <string>

namespace Json
{
    class Value;
}

namespace fl4cpp
{
    class Session;
    class ProfileNode;
    
    /** Status Node type string.
     */
    static const char kStatusNodeType[] = "status";
    
    /** Represents Status Node of the facebook Graph Api.
     * You can find detailed description of the fields and edges
     * of this node in the Graph Api documentation.
     * See: https://developers.facebook.com/docs/graph-api/reference/v2.0/status
     *
     * NOTE: Setter methods of the StatusNode class are used for initialization
     * of the local copy, and has no affect on Graph Api copy.
     */
    class StatusNode : public Node
    {
    public:
        /** Constructs StatusNode object.
         *
         * @param session Pointer to a current Session object.
         * @param rootNode Pointer to a root node.
         * (Profile Node on behalf of which all actions with this node will be performed).
         */
        StatusNode( Session* session, ProfileNode* rootNode );
        
        virtual ~StatusNode();
        
        /** Initializes Node object with Json data.
         *
         * @param nodeData Json object with Node data,
         * formatted as in usual response from Graph Api
         * on requesting a particular node.
         *
         * @return Returns true if node object initialized successfully,
         * otherwise - false.
         */
        virtual bool initWithJson( Json::Value* nodeData );
        
        // fields
        // nodeId resides in base Node class
        
        /**  Requests a Node of the author of this node
         * from the Graph Api server.
         *
         * @param initFromGraphApi Flag, that if set to false will result in
         * returning a pointer to a local copy of author Node without
         * sending a request to a Facebook server.
         * If no local copy of author Node is present - it will be requested
         * from Graph Api.
         *
         * @return Pointer to an author node if Node was
         * received and initialized correctly,
         * otherwise - null.
         */
        Node* getFrom( bool initFromGraphApi = true );
        
        /** Sets author node.
         *
         * @param Pointer to an author node.
         */
        void setFrom( Node* from ) { _from = from; };
        
        /** Gets message.
         *
         * @return Message.
         */
        std::string getMessage() const { return _message; };
        
        /** Sets message.
         *
         * @param message Message.
         */
        void setMessage( std::string message ) { _message = message; };
        
        /** Gets place.
         *
         * @return Pointer to a Page Node.
         */
        PageNode* getPlace() { return &_place; };
        
        /** Sets place.
         *
         * @param place Pointer to a Page Node.
         */
        void setPlace( PageNode* place ) { _place = *place; };
        
        /** Gets updated time.
         *
         * @return Updated time.
         */
        std::string getUpdatedTime() const { return _updatedTime; };
        
        /** Sets updated time.
         *
         * @param updatedTime Updated time.
         */
        void setUpdatedTime( std::string updatedTime ) { _updatedTime = updatedTime; };
        
        // additional info
        
        /** Gets author Node Id.
         *
         * @return Author Node Id.
         */
        std::string getFromNodeId() const { return _fromNodeId; };
        
        /* Sets author Node Id.
         *
         * @param fromNodeId Author Node Id.
         */
        void setFromNodeId( std::string fromNodeId ) { _fromNodeId = fromNodeId; };
        
        /** Gets author name.
         *
         * @return Author name.
         */
        std::string getFromName() const { return _fromName; };
        
        /* Sets author name.
         *
         * @param fromName Author name.
         */
        void setFromName( std::string fromName ) { _fromName = fromName; };

    private:
        // fields
        // nodeId resides in base Node class
        
        /** Pointer to an author Node.
         */
        Node* _from;
        
        /** Contents of "message" field.
         */
        std::string _message;
        
        /** Contents of "place" field.
         */
        PageNode _place;
        
        /** Contents of "updated_time" field.
         */
        std::string _updatedTime;
        
        // additional info
        
        /** Node Id of an author Node (taken from "from" field).
         */
        std::string _fromNodeId;
        
        /** Name of an author (taken from "from" field).
         */
        std::string _fromName;
    };
}


#endif // __FL4CPP_STATUS_NODE__
