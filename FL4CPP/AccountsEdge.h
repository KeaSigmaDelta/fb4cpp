#ifndef __FL4CPP_ACCOUNTS_EDGE__
#define __FL4CPP_ACCOUNTS_EDGE__

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
    class PageNode;
    class ProfileNode;
    
    /** Accounts Edge type string.
     */
    static const char kAccountsEdgeType[] = "accounts";
    
    /** Represents Accounts Edge of the facebook Graph Api.
     * You can find detailed description of functions and fields
     * of this edge in the Graph Api documentation.
     * See: https://developers.facebook.com/docs/graph-api/reference/v2.0/user/accounts/
     */
    class AccountsEdge : public Edge
    {
    public:
        /** Constructs AccountsEdge object.
         *
         * @param session Pointer to a current session.
         * @param ownerNode Pointer to a node that hold this edge.
         * @param rootNode Pointer to a root node
         * (Profile Node on behalf of which all actions with this edge will be performed).
         */
        AccountsEdge( Session* session, Node* ownerNode, ProfileNode* rootNode );
        
        virtual ~AccountsEdge();
        
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
        
        /** Gets page nodes of this Profile Node.
         *
         * @return Pointer to a list of pointers to Page Node objects.
         */
        std::vector< PageNode* >* getPageNodes() { return &_pageNodes; };
        
    private:
        // nodes
        
        /** List of Page Nodes provided by this edge.
         */
        std::vector< PageNode* > _pageNodes;
    };
}

#endif // __FL4CPP_ACCOUNTS_EDGE__
