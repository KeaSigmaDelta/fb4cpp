#ifndef __FL4CPP_PAGE_NODE__
#define __FL4CPP_PAGE_NODE__

#include "ProfileNode.h"
#include "AlbumsEdge.h"

#include <string>

namespace Json
{
    class Value;
}

namespace fl4cpp
{ 
    class Session;
    
    /** Page Node type string.
     */
    static const char kPageNodeType[] = "page";
    
    /** Represents Page Node of the facebook Graph Api.
     * You can find detailed description of the fields and edges
     * of this node in the Graph Api documentation.
     * See: https://developers.facebook.com/docs/graph-api/reference/v2.0/page
     *
     * NOTE: Setter methods of the PageNode class are used for initialization
     * of the local copy, and has no affect on Graph Api copy.
     */
    class PageNode : public ProfileNode
    {
    public:
        /** Constructs PageNode object.
         *
         * @param session Pointer to a current Session object.
         */
        PageNode( Session* session );
        
        virtual ~PageNode() {};
        
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
        // only core fields supported
        
        /** Gets category.
         *
         * @return Category.
         */
        std::string getCategory() const { return _category; };
        
        /** Sets category.
         *
         * @param category Category.
         */
        void setCategory( std::string category ) { _category = category; };
        
        /** Gets checkins.
         *
         * @return Number of checkins.
         */
        int getCheckins() const { return _checkins; };
        
        /** Sets checkins.
         *
         * @param checkins Number of checkins.
         */
        void setCheckins( int checkins ) { _checkins = checkins; };
        
        /** Gets description.
         *
         * @return Description.
         */
        std::string getDescription() const { return _description; };
        
        /** Sets description.
         *
         * @param Descripton..
         */
        void setDescription( std::string description ) { _description = description; };
        
        /** Gets likes.
         *
         * @return Number of likes.
         */
        int getLikes() const { return _likes; };
        
        /** Sets likes.
         *
         * @param likes Number of likes.
         */
        void setLikes( int likes ) { _likes = likes; };
        
        /** Gets link.
         *
         * @return Link.
         */
        std::string getLink() const { return _link; };
        
        /** Sets link.
         *
         * @param link Link.
         */
        void setLink( std::string link ) { _link = link; };
        
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
        
        /** Gets username.
         *
         * @return Username.
         */
        std::string getUsername() const { return _username; };
        
        /** Sets username.
         *
         * @param username Username.
         */
        void setUsername( std::string username ) { _username = username; };
        
        // edges
        
        /** Requests an Album Edge of this page.
         * Received Edge data is saved in _albumsEdge class member.
         *
         * @param initFromGraphApi Flag, that if set to false will result in
         * returning a pointer to a local copy of Photos Edge without
         * sending a request to a Facebook server.
         *
         * @return pointer to a local copy of Albums Edge.
         */
        AlbumsEdge* getAlbumsEdge( bool initFromGraphApi = true );
        
    private:
        // fields
        // nodeId resides in base Node class
        
        /** Contents of "category" field.
         */
        std::string _category;
        
        /** Contents of "checkins" field.
         */
        int _checkins;
        
        /** Contents of "description" field.
         */
        std::string _description;
        
        /** Contents of "likes" field.
         */
        int _likes;
        
        /** Contents of "link" field.
         */
        std::string _link;
        
        /** Contents of "name" field.
         */
        std::string _name;
        
        /** Contents of "username" field.
         */
        std::string _username;
        
        // edges
        
        /** Contents of "albums" edge.
         */
        AlbumsEdge  _albumsEdge;
    };
}

#endif // __FL4CPP_PAGE_NODE__
