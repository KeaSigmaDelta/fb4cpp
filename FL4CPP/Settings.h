#ifndef __FL4CPP_SETTINGS__
#define __FL4CPP_SETTINGS__

#include <string>
#include <map>

#include <boost/shared_ptr.hpp>

namespace fl4cpp
{
    /** Contains string values which is used by fl4cpp library
	 * for interaction with facebook Graph API.
     *
     * App Id value should be set by the programmer before
     * using any Session class methods. 
     *
     * User Agent value depends on which browser engine
     * is used for a login sequence.
     *
     * Graph Api URL should keep its default value, unless
     * Facebook changes its api.
	 */
    class Settings
    {
    public:
        /** Provides a pointer to a Settings singleton.
         */
        static Settings* shared();
        
		~Settings() {};
        
        /** Gets current User Agent.
         *
		 * @return User Agent string.
		 */
        std::string getUserAgent() const { return _userAgent; };
        
        /** Sets current User Agent.
		 *
		 * @param userAgent User Agent string.
		 */
        void setUserAgent( std::string userAgent ) { _userAgent = userAgent; };
        
        /** Gets current Graph Api URL.
         *
		 * @return Graph Api URL string.
		 */
        std::string getGraphApiUrl() const { return _graphApiUrl; };
        
        /** Sets current Graph Api URL.
		 *
		 * @param graphApiUrl Graph Api URL string.
		 */
        void setGraphApiUrl( std::string graphApiUrl ) { _graphApiUrl = graphApiUrl; };
        
        /** Gets current App Id.
         *
		 * @return App Id string.
		 */
        std::string getAppId() const { return _appId; };
        
        /** Sets current App Id.
		 *
		 * @param appId App Id string.
		 */
        void setAppId( std::string appId ) { _appId = appId; };
        
    private:
        /** Constructs Sessings object.
         */
        Settings();
        
    private:
        /** Browser User Agent.
         */
        std::string _userAgent;
        
        /** Facebook Graph Api URL.
         */
        std::string _graphApiUrl;
        
        /** Facebook Application Id.
         */
        std::string _appId;
    };
    
    /** List of available privacy settings of a post/photo/etc.
     * 
     * Using DEFAULT value for posting leads to a post being published with
     * the privacy level granted to the app in the Login Dialog.
     */
    namespace PrivacySettingTypes
    {
        enum types
        {
            EVERYONE,
            ALL_FRIENDS,
            FRIENDS_OF_FRIENDS,
            //CUSTOM, ///< custom settings not supported yet
            SELF,
            DEFAULT
        };
    }
    
    /** Helper class that converts PrivacySettingTypes enum value
     * to a string value.
     */
    class PrivacySettings
    {
    public:
        /** Provides a pointer to a PrivacySettings singleton.
         */
        static PrivacySettings* shared();
        
        /** Converts PrivacySettingTypes enum value to a string value.
		 *
		 * @param type privacy level.
		 */
        std::string getPrivacySettingString( PrivacySettingTypes::types type );
        
    private:
        /** Constructs PrivacySettings object.
         */
        PrivacySettings();
        
    private:
        /** List of string values corresponding to PrivacySettingTypes enum.
         */
        std::map< PrivacySettingTypes::types, std::string > _settingStrings;
    };
    
}

#endif // __FL4CPP_SETTINGS__
