#include "Settings.h"

using namespace fl4cpp;

const char kDefaultUserAgent[] = "Mozilla/5.0 (Windows NT 5.1; rv:31.0) Gecko/20100101 Firefox/31.0";
const char kDefaultGraphApiUrl[] = "https://graph.facebook.com/v2.0";

static boost::shared_ptr< Settings > settingsSharedInstance = 0;

Settings* Settings::shared()
{
    if ( settingsSharedInstance == 0 )
    {
        settingsSharedInstance = boost::shared_ptr< Settings >( new Settings() );
    }
    
    return settingsSharedInstance.get();
}

Settings::Settings() :
_userAgent( kDefaultUserAgent ),
_graphApiUrl( kDefaultGraphApiUrl ),
_appId( "" )
{
    
}

static boost::shared_ptr< PrivacySettings > privacySettingsSharedInstance = 0;

PrivacySettings* PrivacySettings::shared()
{
    if ( privacySettingsSharedInstance == 0 )
    {
        privacySettingsSharedInstance = boost::shared_ptr< PrivacySettings >( new PrivacySettings() );
    }
    
    return privacySettingsSharedInstance.get();
}

PrivacySettings::PrivacySettings() :
_settingStrings()
{
    _settingStrings[ PrivacySettingTypes::EVERYONE ] = "EVERYONE";
    _settingStrings[ PrivacySettingTypes::ALL_FRIENDS ] = "ALL_FRIENDS";
    _settingStrings[ PrivacySettingTypes::FRIENDS_OF_FRIENDS ] = "FRIENDS_OF_FRIENDS";
    //_settingStrings[ PrivacySettingTypes::CUSTOM ] = "CUSTOM";
    _settingStrings[ PrivacySettingTypes::SELF ] = "SELF";
    _settingStrings[ PrivacySettingTypes::DEFAULT ] = "";
}

std::string PrivacySettings::getPrivacySettingString( PrivacySettingTypes::types type )
{
    return _settingStrings[ type ];
}