#ifndef __FL4CPP_STATUSES_EDGE__
#define __FL4CPP_STATUSES_EDGE__

#include "Edge.h"

#include <string>
#include <vector>

namespace Json
{
    class Value;
}

namespace fl4cpp
{ 
    class Session;
    class StatusNode;
    class ProfileNode;
    
    /** Statuses Edge type string.
     */
    static const char kStatusesEdgeType[] = "statuses";
    
    /** Represents Statuses Edge of the facebook Graph Api.
     * You can find detailed description of functions and fields
     * of this edge in the Graph Api documentation.
     * See: https://developers.facebook.com/docs/graph-api/reference/v2.0/user/feed/
     */
    class StatusesEdge : public Edge
    {
    public:
        /** Constructs StatusesEdge object.
         *
         * @param session Pointer to a current session.
         * @param ownerNode Pointer to a node that hold this edge.
         * @param rootNode Pointer to a root node
         * (Profile Node on behalf of which all actions with this edge will be performed).
         */
        StatusesEdge( Session* session, Node* ownerNode, ProfileNode* rootNode );
        
        virtual ~StatusesEdge();
        
        /** Initializes Edge object with Json data.
         *
         * @param edgeData Json object with Edge data,
         * formatted as in usual response from Graph Api
         * on requesting a particular edge.
         *
         * @return Returns true if edge object initialized successfully,
         * otherwise - false.
         */
        virtual bool initWithJson( Json::Value* edgeData );
        
        // nodes
        
        /** Gets status nodes of this Profile Node.
         *
         * @return Pointer to a list of pointers to StatusNode pbjects.
         */
        std::vector< StatusNode* >* getStatusNodes() { return &_statusNodes; };
        
    private:
        // nodes
        
        /** List of status nodes provided by this edege.
         */
        std::vector< StatusNode* > _statusNodes;
    };
}

#endif // __FL4CPP_STATUSES__
