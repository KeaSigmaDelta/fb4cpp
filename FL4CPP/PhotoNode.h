#ifndef __FL4CPP_PHOTO_NODE__
#define __FL4CPP_PHOTO_NODE__

#include "Node.h"
#include "AlbumNode.h"
#include "CommonObjects.h"
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
    
    /** Photo Node type string.
     */
    static const char kPhotoNodeType[] = "photo";
    
    /** Represents Photo Node of the facebook Graph Api.
     * You can find detailed description of the fields and edges
     * of this node in the Graph Api documentation.
     * See: https://developers.facebook.com/docs/graph-api/reference/v2.0/photo
     *
     * NOTE: Setter methods of the PhotoNode class are used for initialization
     * of the local copy, and has no affect on Graph Api copy.
     */
    class PhotoNode : public Node
    {
    public:
        /** Constructs PhotoNode object.
         *
         * @param session Pointer to a current Session object.
         * @param rootNode Pointer to a root node.
         * (Profile Node on behalf of which all actions with this node will be performed).
         */
        PhotoNode( Session* session, ProfileNode* rootNode );
        
        virtual ~PhotoNode();
        
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
        
        /** Gets Album Node.
         * 
         * @return Pointer to an Album Node.
         */
        AlbumNode*  getAlbum() { return &_album; };
        
        /** Sets Album Node.
         *
         * @param album Pointer to an Album Node.
         */
        void setAlbum( AlbumNode* album ) { _album = *album; };
        
        /** Gets backdated time.
         *
         * @return Backdated time string.
         */
        std::string getBackdatedTime() const { return _backdatedTime; };
        
        /* Sets backdated time.
         *
         * @param backdataedTime Backdated time.
         */
        void setBackdatedTime( std::string backdatedTime ) { _backdatedTime = backdatedTime; };
        
        /** Gets backdated time granularity.
         *
         * @return Backdated time granularity.
         */
        std::string getBackdatedTimeGranularity() const { return _backdatedTimeGranularity; };
        
        /* Sets Backdated time granularity.
         *
         * @param backdatedTimeGranularity Backdated time granularity.
         */
        void setBackdatedTimeGranularity( std::string backdatedTimeGranularity ) { _backdatedTimeGranularity = backdatedTimeGranularity; };
        
        /** Gets created time.
         *
         * @return Created time string.
         */
        std::string getCreatedTime() const { return _createdTime; };
        
        /* Sets created time.
         *
         * @param createdTime Created time.
         */
        void setCreatedTime( std::string createdTime ) { _createdTime = createdTime; };
        
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
        
        /** Sets author Node.
         *
         * @param from Pointer to an author Node.
         */
        void setFrom( Node* from ) { _from = from; };
        
        /** Gets image height.
         *
         * @return Image height.
         */
        int getHeight() const { return _height; };
        
        /* Sets image height.
         *
         * @param height Image Height.
         */
        void setHeight( int height ) { _height = height; };
        
        /** Gets link to an icon.
         *
         * @return Link to an icon.
         */
        std::string getIcon() const { return _icon; };
        
        /* Sets link to an icon.
         *
         * @param icon Link to an icon.
         */
        void setIcon( std::string icon ) { _icon = icon; };
        
        /** Gets image sources.
         *
         * @return Pointer to a list of ImageSource objects.
         */
        std::vector< ImageSource >* getImages() { return &_images; };
        
        /* Sets image sources.
         *
         * @param images Pointer to a list of IamgeSource objects.
         */
        void setImages( std::vector< ImageSource >* images ) { _images = *images; };
        
        /** Gets link to this image.
         *
         * @return Link to this image.
         */
        std::string getLink() const { return _link; };
        
        /* Sets link to this image.
         *
         * @param Link to this image.
         */
        void setLink( std::string link ) { _link = link; };
        
        /** Gets name of this image.
         *
         * @return Name of this image.
         */
        std::string getName() const { return _name; };
        
        /* Sets name of this image.
         *
         * @param name Name of this image.
         */
        void setName( std::string name ) { _name = name; };
        
        /** Gets tagged names.
         *
         * @return Pointer to a list of NameTag objects.
         */
        std::vector< NameTag >* getNameTags() { return &_nameTags; };
        
        /* Sets tagged names.
         *
         * @param nameTags pointer to a list of NameTag objects.
         */
        void setNameTags( std::vector< NameTag >* nameTags ) { _nameTags = *nameTags; };
        
        /** Gets page story id.
         *
         * @return Page story id.
         */
        std::string getPageStoryId() const { return _pageStoryId; };
        
        /* Sets page story id.
         *
         * @param pageStoryId Page story id.
         */
        void setPageStoryId( std::string pageStoryId ) { _pageStoryId = pageStoryId; };
        
        /** Gets picture.
         *
         * @return Picture.
         */
        std::string getPicture() const { return _picture; };
        
        /* Sets picture.
         *
         * @param picture Picture.
         */
        void setPicture( std::string picture ) { _picture = picture; };
        
        /** Gets place.
         *
         * @return Pointer to a Page Node.
         */
        PageNode*   getPlace() { return &_place; };
        
        /* Sets place.
         *
         * @param place Pointer to a Page Node.
         */
        void setPlace( PageNode* place ) { _place = *place; };
        
        /** Gets source.
         *
         * @return Source.
         */
        std::string getSource() const { return _source; };
        
        /* Sets source.
         *
         * @param source Source.
         */
        void setSource( std::string source ) { _source = source; };
        
        /** Gets updated time.
         *
         * @return Updated time.
         */
        std::string getUpdatedTime() const { return _updatedTime; };
        
        /* Sets updated time.
         *
         * @param Updated time.
         */
        void setUpdatedTime( std::string updatedTime ) {  _updatedTime = updatedTime; };
        
        /** Gets image width.
         *
         * @return Image width.
         */
        int getWidth() const { return _width; };
        
        /* Sets image width.
         *
         * @param width Image width.
         */
        void setWidth( int width ) { _width = width; };
        
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
        
        /** Contents of "album" field.
         */
        AlbumNode _album;
        
        /** Contents of "backdated_time" field.
         */
        std::string _backdatedTime;
        
        /** Contents of "backdated_time_granularity" field.
         */
        std::string _backdatedTimeGranularity;
        
        /** Contents of "created_time" field.
         */
        std::string _createdTime;
        
        /** Pointer to an author Node.
         */
        Node* _from;
        
        /** Contents of "height" field.
         */
        int  _height;
        
        /** Contents of "icon" field.
         */
        std::string _icon;
        
        /** Contents of "images" field.
         */
        std::vector< ImageSource > _images;
        
        /** Contents of "link" field.
         */
        std::string _link;
        
        /** Contents of "name" field.
         */
        std::string _name;
        
        /** Contents of "tags" field. (mentioned as "name_tags" in documentation, but in reality its "tags").
         */
        std::vector< NameTag > _nameTags;
        
        /** Contents of "page_story_id" field.
         */
        std::string _pageStoryId;
        
        /** Contents of "picture" field.
         */
        std::string _picture;
        
        /** Contents of "place" field.
         */
        PageNode    _place;
        
        /** Contents of "source" field.
         */
        std::string _source;
        
        /** Contents of "updated_time" field.
         */
        std::string _updatedTime;
        
        /** Contents of "width" field.
         */
        int         _width;
        
        // additional info
        
        /** Node Id of an author Node (taken from "from" field).
         */
        std::string     _fromNodeId;
        
        /** Name of an author (taken from "from" field).
         */
        std::string     _fromName;
    };
}

#endif // __FL4CPP_PHOTO_NODE__
