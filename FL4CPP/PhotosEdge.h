#ifndef __FL4CPP_PHOTOS_EDGE__
#define __FL4CPP_PHOTOS_EDGE__

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
    class PhotoNode;
    class ProfileNode;
    
    /** Photos Edge type string.
     */
    static const char kPhotosEdgeType[] = "photos";
    
    /** Represents Photos Edge of the facebook Graph Api.
     * You can find detailed description of functions and fields
     * of this edge in the Graph Api documentation.
     * See: https://developers.facebook.com/docs/graph-api/reference/v2.0/user/photos
     */
    class PhotosEdge : public Edge
    {
    public:
        /** Constructs PhotosEdge object.
         *
         * @param session Pointer to a current session.
         * @param ownerNode Pointer to a node that hold this edge.
         * @param rootNode Pointer to a root node
         * (Profile Node on behalf of which all actions with this edge will be performed).
         */
        PhotosEdge( Session* session, Node* ownerNode, ProfileNode* rootNode );
        
        virtual ~PhotosEdge();
        
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
        
        /** Publishes a photo by its corresponding url.
         *
         * @param imageUrl Url of an image.
         * @param imageDescription Description of a photo.
         * @param noStory If set to true - photo will not be 
         * published on the timeline, but only in the album.
         *
         * @param Returns true if photo was published successfully,
         * otherwise - false.
         */
        bool publishUrl( std::string imageUrl, std::string imageDescription, bool noStory );
        
        /** Publishes a photo by its binary image data.
         *
         * @param imageData Pointer to an image data buffer.
         * @param dataSize Size of the data.
         * @param imageDescription Description of a photo.
         * @param noStory If set to true - photo will not be
         * published on the timeline, but only in the album.
         */
        bool publishData( void* imageData, unsigned long dataSize, std::string imageDescription, bool noStory );
        
        // nodes
        
        /** Gets photos provided by this edge.
         *
         * @return Pointer to a list of pointers to a PhotoNode objects..
         */
        std::vector< PhotoNode* >* getPhotoNodes() { return &_photoNodes; };
        
    private:
        // nodes
        
        /** Photo nodes provided by this edge.
         */
        std::vector< PhotoNode* > _photoNodes;
    };
}

#endif // __FL4CPP_PHOTOS_EDGE__
