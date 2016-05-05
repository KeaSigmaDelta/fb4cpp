#ifndef __FL4CPP_FEED_EDGE__
#define __FL4CPP_FEED_EDGE__

#include "Edge.h"
#include "Settings.h"

#include <string>
#include <vector>

namespace Json
{
    class Value;
}

namespace fl4cpp
{
    class Session;
    class ProfileNode;
    
    /** Feed Edge type string.
     */
    static const char kFeedEdgeType[] = "feed";
    
    /** Represents Feed Edge of the facebook Graph Api.
     * You can find detailed description of functions and fields
     * of this edge in the Graph Api documentation.
     * See: https://developers.facebook.com/docs/graph-api/reference/v2.0/user/feed
     */
    class FeedEdge : public Edge
    {
    public:
        /** Constructs FeedEdge object.
         *
         * @param session Pointer to a current session.
         * @param ownerNode Pointer to a node that hold this edge.
         * @param rootNode Pointer to a root node
         * (Profile Node on behalf of which all actions with this edge will be performed).
         */
        FeedEdge( Session* session, Node* ownerNode, ProfileNode* rootNode );
        
        virtual ~FeedEdge();
        
        /** Overrides method of a base class and does nothing.
         *
         * @return Returns true.
         */
        virtual bool initFromGraphApi();
        
        /** Overrides method of a base class and does nothing.
         *
         * @peram edgeData Not used.
         *
         * @return Returns true.
         */
        virtual bool initWithJson( Json::Value* edgeData );
        
        /** Publishes a status on a timeline of a Profile Node.
         *
         * @param message Message.
         * @param privacyType Privacy level.
         *
         * @return Returns true if status was posted successfully,
         * otherwise - false.
         */
        bool publish( std::string message, PrivacySettingTypes::types privacyType );
    };
}

#endif // __FL4CPP_FEED_EDGE__
