#ifndef __FL4CPP_PICTURE_EDGE__
#define __FL4CPP_PICTURE_EDGE__

#include "Edge.h"
#include <string>

namespace fl4cpp
{
    /** Represents data provided by Picture Edge.
     * See: https://developers.facebook.com/docs/graph-api/reference/v2.0/user/picture/
     */
    class PictureInfo
    {
    public:
        /** Constructs PictureInfo object.
         *
         * @param url Url of a picture.
         * @param silhouette Indicates whether the profile photo is the default 'silhouette' picture, 
         * or has been replaced.
         * @param width image width.
         * @param height image height.
         */
        PictureInfo( std::string url, bool silhouette, int width, int height );
        
        /** Gets url of a picture.
         *
         * @return Url.
         */
        std::string getUrl() const { return _url; };
        
        /** Sets url of a picture.
         *
         * @param url Url.
         */
        void setUrl( std::string url ) { _url = url; };
        
        /** Gets silhouette indicator.
         *
         * @return True if profile photo is the default 'silhouette' picture.
         */
        bool isSilhouette() const { return _silhouette; };
        
        /** Sets silhouette indicator.
         *
         * @param silhouette Silhouette indicator.
         */
        void setSilhouette( bool silhouette ) { _silhouette = silhouette; };
        
        /** Gets image width.
         *
         * @return Width.
         */
        int getWidth() const { return _width; };
        
        /** Sets image width.
         *
         * @param width Width.
         */
        void setWidth( int width ) { _width = width; };
        
        /** Gets image height.
         * 
         * @return Height.
         */
        int  getHeight() const { return _height; };
        
        /** Sets image height.
         *
         * @param height Height.
         */
        void setHeight( int height ) { _height = height; };
        
    private:
        
        /** Contents of "url" field.
         */
        std::string _url;
        
        /** Contents of "is_silhouette" field.
         */
        bool _silhouette;
        
        /** Contents of "width" field.
         */
        int _width;
        
        /** Contents of "height" field.
         */
        int _height;
    };
  
    /** Picture types ot request.
     */
    namespace PictureTypes
    {
        enum types
        {
            SQUARE,
            SMALL,
            NORMAL,
            LARGE
        };
    }

    class Session;
    class ProfileNode;
    
    /** Picture Edge type string.
     */
    static const char kPictureEdgeType[] = "picture";
    
    /** Represents Picture Edge of the facebook Graph Api.
     * You can find detailed description of functions and fields
     * of this edge in the Graph Api documentation.
     * See: https://developers.facebook.com/docs/graph-api/reference/v2.0/user/picture/
     */
    class PictureEdge : public Edge
    {
    public:
        /** Constructs PictureEdge object.
         *
         * @param session Pointer to a current session.
         * @param ownerNode Pointer to a node that hold this edge.
         * @param rootNode Pointer to a root node
         * (Profile Node on behalf of which all actions with this edge will be performed).
         */
        PictureEdge( Session* session, Node* ownerNode, ProfileNode* rootNode );
        
        virtual ~PictureEdge();
        
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
    
        /**
        * Reading binary image data procedure is described in Graph API documentation, but in fact not working.
        * So only indirect image data is present in this edge implementation.
        */
        
        /** Requests a profile picture info of a root Profile node.
         *
         * @param pictureType Picture type.
         * @param info Pointer to a buffer for received picture nfo.
         *
         * @return Returns true if picture info was received successfuly,
         * otherwise - false.
         */
        bool readInfo( PictureTypes::types pictureType, PictureInfo* info );
        
        /** Requests a profile picture info of a root Profile node.
         *
         * @param pictureType Picture type.
         * @param width Desired picture width.
         * @param height Desired picture height.
         * @param info Pointer to a buffer for received picture nfo.
         *
         * @return Returns true if picture info was received successfuly,
         * otherwise - false.
         */
        bool readInfo( PictureTypes::types pictureType, int width, int height, PictureInfo* info );
        
    private:
        
        /** Helper function that converts image type enum element to a string.
         *
         * @param type PictyreType enum element.
         *
         * @return Picture type string value.
         */
        static std::string pictureTypeToString( PictureTypes::types type );
    };
}

#endif // __FL4CPP_PICTURE_EDGE__
