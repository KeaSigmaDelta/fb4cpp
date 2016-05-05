#include "Session.h"
#include "SyncHTTPRequest.h"
#include "Settings.h"

#include "json/json.h"


#include <boost/date_time/posix_time/posix_time.hpp>

using namespace fl4cpp;

Session::Session() :
_accessToken( "" ),
_pageAccessTokens(),
_sessionObservers(),
_currentUserNode( this ),
_threadFuncMutex(),
_notificationThreadsShouldQuit()
{
}

Session::~Session()
{
}

void Session::setAccessToken( std::string accessToken, unsigned int expiresIn )
{
    assert( "Access token TTL should be equal or greater than zero." && ( expiresIn >= 0 ) );
    
    {
        boost::mutex::scoped_lock lock(_threadFuncMutex );

        _notificationThreadsShouldQuit.notify_all();
        
        if ( expiresIn > 0 )
        {
            boost::thread observerNotificationThread( &Session::observersNotificationThreadFunc, this, expiresIn );
            observerNotificationThread.detach();
        }
    }
    
    _accessToken = accessToken;
    _pageAccessTokens.clear();
    updatePageAccessTokens();
}

std::string Session::getPageAccessToken( std::string pageNodeId )
{
    if ( _pageAccessTokens.find( pageNodeId ) != _pageAccessTokens.end() )
    {
        return _pageAccessTokens[ pageNodeId ];
    }
    
    return "";
}

void Session::updatePageAccessTokens()
{
    std::string uri;
    uri.append( Settings::shared()->getGraphApiUrl() );
    uri.append( "/me" );
    uri.append( "/accounts" );
    uri.append( "?access_token=" + getAccessToken() );
    
    std::string receivedData;
    
    SyncHTTPRequest request;
    if ( request.performGet( uri, &receivedData ) )
    {
        Json::Value value;
        Json::Reader reader;
        
        if ( !reader.parse( receivedData, value, false ) )
        {
            std::cout << "Failed to parse json data.\n";
            return;
        }
        
        if ( value[ "data" ].isNull() )
        {
            std::cout << "No value for key \"data\".";
            return;
        }
            
        if ( !value[ "data" ].isArray() )
        {
            std::cout << "Value for key \"data\" is not an array.";
            return;
        }
            
        Json::Value dataArray = value[ "data" ];
        
        for ( unsigned int i = 0; i < dataArray.size(); i++ )
        {
            Json::Value pageObject = dataArray[ i ];
            
            if ( !pageObject[ "id" ].isNull() && !pageObject[ "access_token" ].isNull() && !pageObject[ "perms" ].isNull() )
            {
                _pageAccessTokens[ pageObject[ "id" ].asString() ] = pageObject[ "access_token" ].asString();
            }
        }
        
        return;
    }
    
    std::cout << "No data was received.\n";
    return;
}

void Session::registerObserver( SessionObserver *observer )
{
    _sessionObservers[ observer ] = observer;
}

void Session::unregisterObserver( SessionObserver *observer )
{
    if ( _sessionObservers.find( observer ) != _sessionObservers.end() )
    {
        _sessionObservers.erase( observer );
    }
}

bool Session::getLoginPage( std::string* receivedData, std::vector< std::string >* permissions )
{
    assert( "App Id should be provided first." && Settings::shared()->getAppId().size() );
        
    std::string uri;
    uri.append( "https://www.facebook.com/dialog/oauth?" );
    uri.append( "client_id=" + Settings::shared()->getAppId() );
    uri.append( "&redirect_uri=https://www.facebook.com/connect/login_success.html" );
    uri.append( "&response_type=token" );
    if ( permissions->size() )
    {
        uri.append( "&scope=" );
        
        for ( unsigned int i = 0; i < permissions->size(); i++ )
        {
            uri.append( permissions->at( i ) );
            if ( i != permissions->size() - 1 )
                uri.append( "," );
        }
        
    }
    
    SyncHTTPRequest request;
    return request.performGet( uri, receivedData );
}

UserNode* Session::getCurrentUserNode( bool initFromGraphApi /* = true */ )
{
    if ( initFromGraphApi )
    {
        if ( !_currentUserNode.initFromGraphApi( "me" ) )
        {
            std::cout << "UserNode::initFromGraphApi() failed.\n";
            return 0;
        }
    }
            
    return &_currentUserNode;
}

void Session::logout()
{
    std::string uri;
    uri.append( Settings::shared()->getGraphApiUrl() );
    uri.append( "/me/permissions" );
    uri.append( "?access_token=" + getAccessToken() );
    
    std::string receivedData;
    
    SyncHTTPRequest request;
    request.performDelete( uri, &receivedData );
}

void Session::observersNotificationThreadFunc( unsigned int timeToWait )
{
    boost::mutex::scoped_lock lock( _threadFuncMutex );
    
    std::cout << boost::this_thread::get_id() << " start \n";
    
    if( _notificationThreadsShouldQuit.timed_wait( lock, boost::posix_time::seconds( timeToWait ) ) )
    {
        // nothing to do, just quit
        std::cout << boost::this_thread::get_id() << " quit by cond \n";
    }
    else
    {
        _notificationThreadsShouldQuit.notify_all();
        
        for ( SESSION_OBSERVERS_ITERATOR it = _sessionObservers.begin(); it != _sessionObservers.end(); it++ )
        {
            it->second->onAccessTokenExpired();
        }
        
         std::cout << boost::this_thread::get_id() << " quit by time \n";
    }
}
