#ifndef __FL4CPP_COMMON_OBJECTS__
#define __FL4CPP_COMMON_OBJECTS__

#include <string>

namespace fl4cpp
{
    /** Represents an "age_range" object of a User Node.
     * Specifies age range that this person's age fits into.
     * See: https://developers.facebook.com/docs/graph-api/reference/v2.0/user
     */
    class AgeRange
    {
    public:
        /** Constructs AgeRange object.
         *
         * @param min lower bounds.
         * @param max upped bounds.
         */
        AgeRange( std::string min, std::string max );
        
        /** Gets age lower bounds.
         */
        std::string getMin() const { return _min; };
        
        /** Gets age upped bounds.
         */
        std::string getMax() const { return _max; };
        
    private:
        /** Age lower bounds.
         */
        std::string _min;
        
        /** Age upped bounds.
         */
        std::string _max;
        
    };
   
    /** Represents an "CoverPhoto" object of a User Node.
     * Holds information about the person's cover photo.
     * See: https://developers.facebook.com/docs/graph-api/reference/v2.0/user
     */
    class CoverPhoto
    {
    public:
        /** Constructs CoverPhoto object.
         *
         * @param nodeId Node Id of the Photo Node.
         * @param source Link to a picture.
         * @param offsetY The vertical offset in pixels of the photo from the bottom.
         */
        CoverPhoto( std::string nodeId, std::string source, int offsetY );
        
        /** Gets Id of a Photo Node.
         */
        std::string getNodeId() const { return _nodeId; };
        
        /** Gets Link to a picture.
         */
        std::string getSource() const { return _source; };
        
        /** Gets vertical offset.
         */
        int getOffsetY() const { return _offsetY; };
        
    private:
        /** Id of a Photo Node.
         */
        std::string _nodeId;
        
        /** Link to a picture.
         */
        std::string _source;
        
        /** The vertical offset in pixels of the photo from the bottom.
         */
        int _offsetY;
    };
   
    /** Represents an "ImageSource" object of a Photo Node.
     * Holds information about representation of the photo.
     * See: https://developers.facebook.com/docs/graph-api/reference/v2.0/photo
     */
    class ImageSource
    {
    public:
        /** Constructs ImageSource object.
         *
         * @param width Image width.
         * @param height Image height.
         * @param source Link to the image.
         */
        ImageSource( int width, int height, std::string source );
        
        /** Gets image width.
         */
        int getWidth() const { return _width; };
        
        /** Gets image height.
         */
        int getHeight() const { return _height; };
        
        /** Gets link to the image.
         */
        std::string getSource() const { return _source; };
        
    private:
        /** Image width.
         */
        int         _width;
        
        /** Image height.
         */
        int         _height;
        
        /** Link to the image.
         */
        std::string _source;
    };
   
    /** Represents "name_tags" object of a Photo Node.
     * Describes object tagged in the photo.
     * See: https://developers.facebook.com/docs/graph-api/reference/v2.0/photo
     */
    class NameTag
    {
    public:
        /** Constructs NameTag object.
         *
         * @param nodeId Id of a tagged profile.
         * @param name Name of a tagged profile.
         * @param createdTime Time when tag was added.
         * @param x X position of a tagged person on the photo.
         * @param y Y position of a tagged person on the photo.
         */
        NameTag( std::string nodeId, std::string name, std::string createdTime, int x, int y );
        
        /** Gets tagged profile Node Id.
         */
        std::string getNodeId() const { return _nodeId; };
        
        /** Gets tagged profile name.
         */
        std::string getName() const { return _name; };
        
        /** Gets time of tag creation.
         */
        std::string getCreatedTime() const { return _createdTime; };
        
        /** Gets X position of a tagged person on the photo.
         */
        int getX() const { return _x; };
        
        /** Gets Y position of a tagged person on the photo.
         */
        int getY() const { return _y; };
        
    private:
        /** Id of a tagged profile.
         */
        std::string _nodeId;
        
        /** Name of a tagged profile.
         */
        std::string _name;
        
        /** Time when tag was added.
         */
        std::string _createdTime;
        
        /** X position of a tagged person on the photo.
         */
        int _x;
        
        /** Y position of a tagged person on the photo.
         */
        int _y;
    };
}

#endif // __FL4CPP_COMMON_OBJECTS__
