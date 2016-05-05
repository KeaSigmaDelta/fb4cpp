#ifndef __FL4CPP_USER_NODE__
#define __FL4CPP_USER_NODE__

#include "AccountsEdge.h"
#include "StatusesEdge.h"
#include "FeedEdge.h"
#include "AlbumsEdge.h"
#include "ProfileNode.h"
#include "CommonObjects.h"
#include "PictureEdge.h"

#include <string>
#include <vector>

namespace Json
{
    class Value;
}

namespace fl4cpp
{
    class AccountsEdge;
    class StatusesEdge;
    class Session;
    class FeedEdge;
    
    /** User Node type string.
     */
    static const char kUserNodeType[] = "user";
    
    /** Represents User Node of the facebook Graph Api.
     * You can find detailed description of the fields and edges
     * of this node in the Graph Api documentation.
     * See: https://developers.facebook.com/docs/graph-api/reference/v2.0/user
     *
     * NOTE: Setter methods of the UserNode class are used for initialization
     * of the local copy, and has no affect on Graph Api copy.
     */
    class UserNode : public ProfileNode
    {
    public:
        /** Constructs UserNode object.
         *
         * @param session Pointer to a current Session object.
         */
        UserNode( Session* session );
        
        virtual ~UserNode() {};

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
        
        /** Gets age range.
         *
         * @return Age range.
         */
        AgeRange getAgeRange() const { return _ageRange; };
        
        /** Sets age range.
         *
         * @param ageRange Age range.
         */
        void setAgeRange( AgeRange ageRange ) { _ageRange = ageRange; };
        
        /** Gets birthday.
         *
         * @return Birthday.
         */
        std::string getBirthday() const { return _birthday; };
        
        /** Sets birthday.
         *
         * @param birthday Birthday.
         */
        void setBirthday( std::string birthday ) { _birthday = birthday; };
        
        /** Gets cover photo.
         *
         * NOTE: Seems deprecated. Use "picture" edge instead.
         *
         * @return Pointer to a CoverPhoto object.
         */
        CoverPhoto* getCover() { return &_cover; };
        
        /** Sets cover photo.
         *
         * @param cover Pointer to a CoverPhoto object
         */
        void setCover( CoverPhoto* cover ) { _cover = *cover; };
        
        /** Gets email.
         *
         * @return email.
         */
        std::string getEmail() const { return _email; };
        
        /** Sets email.
         *
         * @param email Email.
         */
        void setEmail( std::string email ) { _email = email; };
        
        /** Gets first name.
         *
         * @return First name.
         */
        std::string getFirstName() const { return _firstName; };
        
        /** Sets forst name.
         *
         * @param firstName First name.
         */
        void setFirstName( std::string firstName ) { _firstName = firstName; };
        
        /** Gets gender.
         *
         * @return Gender.
         */
        std::string getGender() const { return _gender; };
        
        /** Sets gender.
         *
         * @param gender Gender.
         */
        void setGender( std::string gender ) { _gender = gender; };
        
        /** Gets last name.
         *
         * @return Last name.
         */
        std::string getLastName() const { return _lastName; };
        
        /** Sets last name.
         *
         * @param lastName Last name.
         */
        void setLastName( std::string lastName ) { _lastName = lastName; };
        
        /** Gets link.
         *
         * @return link.
         */
        std::string getLink() const { return _link; };
        
        /** Sets link.
         *
         * @param link Link.
         */
        void setLink( std::string link ) { _link = link; };
        
        /** Gets locale.
         *
         * @return Locale.
         */
        std::string getLocale() const { return _locale; };
        
        /** Sets locale.
         *
         * @param locale Locale.
         */
        void setLocale( std::string locale ) { _locale = locale; };
        
        /** Gets location.
         *
         * @return Location.
         */
        std::string getLocation() const { return _location; };
        
        /** Sets location.
         *
         * @param location Location.
         */
        void setLocation( std::string location ) { _location = location; };
        
        /** Gets middle name.
         *
         * @return Middle name.
         */
        std::string getMiddleName() const { return _middleName; };
        
        /** Sets middle name.
         *
         * @param Middle name.
         */
        void setMiddleName( std::string middleName ) { _middleName = middleName; };
        
        /** Gets name.
         *
         * @return name.
         */
        std::string getName() const { return _name; };
        
        /** Sets name.
         *
         * @param name Name.
         */
        void setName( std::string name ) { _name = name; };
        
        /** Gets time zone.
         *
         * @return Time zone.
         */
        int getTimeZone() const { return _timeZone; };
        
        /** Sets time zone.
         *
         * @param timeZone Time zone.
         */
        void setTimeZone( int timeZone ) { _timeZone = timeZone; };
        
        // edges
        
        /** Requests an Accounts Edge of this user.
         * Received Edge data is saved in _accountsEdge class member.
         *
         * @param initFromGraphApi Flag, that if set to false will result in
         * returning a pointer to a local copy of Accounts Edge without
         * sending a request to a Facebook server.
         *
         * @return pointer to a local copy of Accounts Edge.
         */
        AccountsEdge* getAccountsEdge( bool initFromGraphApi = true );
        
        /** Requests an Albums Edge of this user.
         * Received Edge data is saved in _albumsEdge class member.
         *
         * @param initFromGraphApi Flag, that if set to false will result in
         * returning a pointer to a local copy of Albums Edge without
         * sending a request to a Facebook server.
         *
         * @return pointer to a local copy of Albums Edge.
         */
        AlbumsEdge* getAlbumsEdge( bool initFromGraphApi = true );
        
        /** Requests an Feed Edge of this user.
         * Received Edge data is saved in _feedEdge class member.
         *
         * @param initFromGraphApi Flag, that if set to false will result in
         * returning a pointer to a local copy of Feed Edge without
         * sending a request to a Facebook server.
         *
         * @return pointer to a local copy of Feed Edge.
         */
        FeedEdge* getFeedEdge( bool initFromGraphApi = true );
        
        /** Requests an Statuses Edge of this user.
         * Received Edge data is saved in _statusesEdge class member.
         *
         * @param initFromGraphApi Flag, that if set to false will result in
         * returning a pointer to a local copy of Statuses Edge without
         * sending a request to a Facebook server.
         *
         * @return pointer to a local copy of Statuses Edge.
         */
        StatusesEdge* getStatusesEdge( bool initFromGraphApi = true );
        
        /** Requests an Picture Edge of this user.
         * Received Edge data is saved in _pictureEdge class member.
         *
         * @param initFromGraphApi Flag, that if set to false will result in
         * returning a pointer to a local copy of Picture Edge without
         * sending a request to a Facebook server.
         *
         * @return pointer to a local copy of Picture Edge.
         */
        PictureEdge* getPictureEdge( bool initFromGraphApi = true );
        
    private:
        // fields
        // nodeId resides in base Node class
        
        /** Contents of "age_range" field.
         */
        AgeRange _ageRange;
        
        /** Contents of "birthday" field.
         */
        std::string _birthday;
        
        /** Contents of "cover" field.
         */
        CoverPhoto  _cover;
        
        /** Contents of "email" field.
         */
        std::string _email;
        
        /** Contents of "first_name" field.
         */
        std::string _firstName;
        
        /** Contents of "gender" field.
         */
        std::string _gender;
        
        /** Contents of "last_name" field.
         */
        std::string _lastName;
        
        /** Contents of "link" field.
         */
        std::string _link;
        
        /** Contents of "location" field.
         */
        std::string _location;
        
        /** Contents of "locale" field.
         */
        std::string _locale;
        
        /** Contents of "middle_name" field.
         */
        std::string _middleName;
        
        /** Contents of "name" field.
         */
        std::string _name;
        
        /** Contents of "time_zone" field.
         */
        int _timeZone;
        
        // edges
        
        /** Contents of "accounts" edge.
         */
        AccountsEdge _accountsEdge;
        
        /** Contents of "albums" edge.
         */
        AlbumsEdge _albumsEdge;
        
        /** Contents of "albums" edge.
         */
        FeedEdge _feedEdge;
        
        /** Contents of "statuses" edge.
         */
        StatusesEdge _statusesEdge;
        
        /** Contents of "pictures" edge.
         */
        PictureEdge _pictureEdge;
    };
}

#endif // __FL4CPP_USER_NODE__
