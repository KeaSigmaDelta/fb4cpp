#ifndef __FL4CPP_ALBUM_NODE__
#define __FL4CPP_ALBUM_NODE__

#include "Node.h"
#include "PhotosEdge.h"
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
    
    /** Album Node type string.
     */
    static const char kAlbumNodeType[] = "album";
    
    /** Represents Album Node of the facebook Graph Api.
     * You can find detailed description of the fields and edges
     * of this node in the Graph Api documentation.
     * See: https://developers.facebook.com/docs/graph-api/reference/v2.0/album
     *
     * NOTE: Setter methods of the AlbumNode class are used for initialization
     * of the local copy, and has no affect on Graph Api copy.
     */
    class AlbumNode : public Node
    {
    public:
        /** Constructs AlbumNode object.
         *
         * @param session Pointer to a current Session object.
         * @param rootNode Pointer to a root node.
         * (Profile Node on behalf of which all actions with this node will be performed).
         */
        AlbumNode( Session* session, ProfileNode* rootNode );
        
        virtual ~AlbumNode();
        
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
        
        /** Gets Can Upload flag.
         *
         * @return Returns true if viewer can upload photos.
         */
        bool getCanUpload() const { return _canUpload; };
        
        /** Sets Can Upload flag.
         *
         * @param canUpload Set to true if viewer can upload photos.
         */
        void setCanUpload( bool canUpload ) { _canUpload = canUpload; };
        
        /** Gets images count.
         *
         * @return Images count.
         */
        int getCount() const { return _count; };
        
        /** Sets images count.
         *
         * @param count Images count.
         */
        void setCount( int count ) { _count = count; };
        
        /** Gets cover photo.
         *
         * @return
         */
        std::string getCoverPhoto() const { return _coverPhoto; };
        
        /** Sets cover photo.
         *
         * @param coverPhoto Cover photo.
         */
        void setCoverPhoto( std::string coverPhoto ) { _coverPhoto = coverPhoto; };
        
        /** Gets created time.
         *
         * @return Created time.
         */
        std::string getCreatedTime() const { return _createdTime; };
        
        /** Sets created time
         *
         * @param createdTime Created time.
         */
        void setCreatedTime( std::string createdTime ) { _createdTime = createdTime; };
        
        /** Gets description.
         *
         * @return Description.
         */
        std::string getDescription() const { return _description; };
        
        /** Sets description.
         *
         * @param description Description.
         */
        void setDescription( std::string description ) { _description = description; };
        
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
        
        /** Gets link.
         *
         * @return Link.
         */
        std::string getLink() const { return _link; };
        
        /** Sets link.
         *
         * @param link Link
         */
        void setLink( std::string link ) { _link = link; };
        
        /** Gets location.
         *
         * @return Location.
         */
        std::string getLocation() const { return _location; };
        
        /** Sets location.
         *
         * @param locaion Location.
         */
        void setLocation( std::string location ) { _location = location; };
        
        /** Gets name.
         *
         * @return Name.
         */
        std::string getName() const { return _name; };
        
        /** Sets name.
         *
         * @param name Name.
         */
        void setName( std::string name ) { _name = name; };
        
        /** Gets place.
         *
         * @return Pointer to a Page Node.
         */
        PageNode*   getPlace() { return &_place; };
        
        /** Sets place.
         *
         * @param place Pointer to a Page Node.
         */
        void setPlace( PageNode* place ) { _place = *place; };
        
        /** Gets privacy settings.
         *
         * @return Rrivacy settings string.
         */
        std::string getPrivacy() const { return _privacy; };
        
        /** Sets privacy.
         *
         * @param privacy Privacy settings string.
         */
        void setPrivacy( std::string privacy ) { _privacy = privacy; };
        
        /** Gets album type.
         *
         * @return Album type.
         */
        std::string getType() const { return _type; };
        
        /** Sets album type.
         *
         * @param type Album type.
         */
        void setType( std::string type ) { _type = type; };
        
        /** Gets updated time.
         *
         * @return Updated time.
         */
        std::string getUpdatedTime() const { return _updatedTime; };
        
        /** Sets updated time.
         *
         * @param updated Updated time.
         */
        void setUpdatedTime( std::string updatedTime ) { _updatedTime = updatedTime; };
        
        // edges
        
        /** Requests a Photos Edge of this album.
         * Received Edge data is saved in _photosEdge class member.
         *
         * @param initFromGraphApi Flag, that if set to false will result in
         * returning a pointer to a local copy of Photos Edge without
         * sending a request to a Facebook server.
         *
         * @return pointer to a local copy of Photos Edge.
         */
        PhotosEdge* getPhotosEdge( bool initFromGraphApi = true );
        
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
        
        /** Contents of "can_upload" field.
         */
        bool _canUpload;
        
        /** Contents of "count" field.
         */
        int _count;
        
        /** Contents of "cover_photo" field.
         */
        std::string _coverPhoto;
        
        /** Contents of "created_time" field.
         */
        std::string _createdTime;
        
        /** Contents of "description" field.
         */
        std::string _description;
        
        /** Pointer to an author Node.
         */
        Node* _from;
        
        /** Contents of "link" field.
         */
        std::string _link;
        
        /** Contents of "location" field.
         */
        std::string _location;
        
        /** Contents of "name" field.
         */
        std::string _name;
        
        /** Contents of "place" field.
         */
        PageNode _place;
        
        /** Contents of "privacy" field.
         */
        std::string _privacy;
        
        /** Contents of "type" field.
         */
        std::string _type;
        
        /** Contents of "updated_time" field.
         */
        std::string _updatedTime;
        
        // edges
        
        /** Contents of "photos" edge.
         */
        PhotosEdge _photosEdge;
        
        // additional info
        
        /** Node Id of an author Node (taken from "from" field).
         */
        std::string _fromNodeId;
        
        /** Name of an author (taken from "from" field).
         */
        std::string _fromName;
    };
}


#endif // __FL4CPP_ALBUM_NODE__
