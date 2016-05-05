#ifndef __FL4CPP_ALBUMS_EDGE__
#define __FL4CPP_ALBUMS_EDGE__

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
    class AlbumNode;
    class ProfileNode;
    
    /** Albums Edge type string.
     */
    static const char kAlbumsEdgeType[] = "albums";
    
    /** Represents Albums Edge of the facebook Graph Api.
     * You can find detailed description of functions and fields
     * of this edge in the Graph Api documentation.
     * See: https://developers.facebook.com/docs/graph-api/reference/v2.0/user/albums
     */
    class AlbumsEdge : public Edge
    {
    public:
        /** Constructs AlbumsEdge object.
         *
         * @param session Pointer to a current session.
         * @param ownerNode Pointer to a node that hold this edge.
         * @param rootNode Pointer to a root node
         * (Profile Node on behalf of which all actions with this edge will be performed).
         */
        AlbumsEdge( Session* session, Node* ownerNode, ProfileNode* rootNode );
        
        virtual ~AlbumsEdge();
        
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
        
        /** Publishes new album.
         *
         * @param albumName Name of al album.
         * @param albumDescription Description of an album.
         * @param privacyType Privacy level.
         *
         * @param Returns true if album published successfully,
         * otherwise - false.
         */
        bool publish( std::string albumName, std::string albumDiscription, PrivacySettingTypes::types privacyType );
        
        // nodes
        
        /** Albums provided by this edge.
         *
         * @return Pointer to a list of pointers to AlbumNode objects.
         */
        std::vector< AlbumNode* >* getAlbumNodes() { return &_albumNodes; };

    private:
        // nodes
        
        /** List of album nodes provided by this edge.
         */
        std::vector< AlbumNode* > _albumNodes;
    };
}

#endif // __FL4CPP_ALBUMS_EDGE__
