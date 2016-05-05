#include "SyncHTTPRequest.h"
#include "Session.h"
#include "Settings.h"

#include "Networking/HTTPRequest.h"

using namespace fl4cpp;

SyncHTTPRequest::SyncHTTPRequest() :
_receivedData( 0 ),
_userAgent( Settings::shared()->getUserAgent() ),
_errorOccured( false ),
_threadFuncMutex(),
_transferCompletedCondition(),
_httpRequest( 0 )
{
    
}

bool SyncHTTPRequest::performGet( std::string uri, std::string* receivedData )
{
    _receivedData = receivedData;
    
    boost::thread thread( &SyncHTTPRequest::threadFuncGet, this, uri );
    thread.join();
    
    if ( _errorOccured )
        return false;
    
    return true;
}

bool SyncHTTPRequest::performPost( std::string uri, std::string* dataToSend, std::string* receivedData )
{
    _receivedData = receivedData;
    
    boost::thread thread( &SyncHTTPRequest::threadFuncPost, this, uri, dataToSend );
    thread.join();
    
    if ( _errorOccured )
        return false;
    
    return true;
}

bool SyncHTTPRequest::performPost( std::string uri, void* dataToSend, unsigned long dataSize, std::string* receivedData )
{
    _receivedData = receivedData;
    
    boost::thread thread( &SyncHTTPRequest::threadFuncPost2, this, uri, dataToSend, dataSize );
    thread.join();
    
    if ( _errorOccured )
        return false;
    
    return true;
}

bool SyncHTTPRequest::performPostFormWithData( std::string uri,
                                              std::string dataFieldName,
                                              void* dataToSend,
                                              unsigned long dataSize,
                                              std::string mimeType,
                                              std::string* receivedData )
{
    _receivedData = receivedData;
    
    boost::thread thread( &SyncHTTPRequest::threadFuncPostFormWithDate, this, uri, dataFieldName, dataToSend, dataSize, mimeType );
    thread.join();
    
    if ( _errorOccured )
        return false;
    
    return true;
}

bool SyncHTTPRequest::performDelete( std::string uri, std::string* receivedData )
{
    _receivedData = receivedData;
    
    boost::thread thread( &SyncHTTPRequest::threadFuncDelete, this, uri );
    thread.join();
    
    if ( _errorOccured )
        return false;
    
    return true;
}

bool SyncHTTPRequest::networkTransferProgress( Networking::CURLConnection* source, uint64 downloadTotal, uint64 downloadNow, uint64 uploadTotal, uint64 uploadNow )
{
    // nothing
    return true;
}

void SyncHTTPRequest::networkTransferCompleted( Networking::CURLConnection* source, CURLcode retCode )
{
    boost::mutex::scoped_lock lock(_threadFuncMutex );

    *_receivedData = source->getReceivedData();
    
    if ( source->getErrorMessage() != "No error" )
    {
        std::cout << "An error (" << source->getErrorMessage() << ") has occured.\n";
        _errorOccured = true;
    }
        
    _transferCompletedCondition.notify_one();
}

void SyncHTTPRequest::threadFuncGet( std::string uri )
{
    boost::mutex::scoped_lock lock( _threadFuncMutex );
    
    _httpRequest = new Networking::HTTPRequest();
    _httpRequest->addListener( this );
    _httpRequest->setUserAgent( _userAgent );
    _httpRequest->performHTTPGet( uri );
    
    _transferCompletedCondition.wait( lock );

    delete _httpRequest;
}

void SyncHTTPRequest::threadFuncPost( std::string uri, std::string* dataToSend )
{
    boost::mutex::scoped_lock lock( _threadFuncMutex );
    
    _httpRequest = new Networking::HTTPRequest();
    _httpRequest->addListener( this );
    _httpRequest->setUserAgent( _userAgent );
    _httpRequest->performHTTPPost( uri, *dataToSend );
    
    _transferCompletedCondition.wait( lock );
    
    delete _httpRequest;
}

void SyncHTTPRequest::threadFuncPost2( std::string uri, void* dataToSend, unsigned long dataSize )
{
    boost::mutex::scoped_lock lock( _threadFuncMutex );
    
    _httpRequest = new Networking::HTTPRequest();
    _httpRequest->addListener( this );
    _httpRequest->setUserAgent( _userAgent );
    _httpRequest->performHTTPPost( uri, dataToSend, dataSize, "", true );
    
    _transferCompletedCondition.wait( lock );
    
    delete _httpRequest;
}

void SyncHTTPRequest::threadFuncPostFormWithDate( std::string uri, std::string dataFieldName, void* dataToSend, unsigned long dataSize, std::string mimeType )
{
    boost::mutex::scoped_lock lock( _threadFuncMutex );
    
    _httpRequest = new Networking::HTTPRequest();
    _httpRequest->addListener( this );
    _httpRequest->setUserAgent( _userAgent );
    _httpRequest->addFormFileField( dataFieldName, dataToSend, dataSize, "filename_placeholder", mimeType );
    _httpRequest->performHTTPPostForm( uri );
    
    _transferCompletedCondition.wait( lock );
    
    delete _httpRequest;
}

void SyncHTTPRequest::threadFuncDelete( std::string uri )
{
    boost::mutex::scoped_lock lock( _threadFuncMutex );
    
    _httpRequest = new Networking::HTTPRequest();
    _httpRequest->addListener( this );
    _httpRequest->setUserAgent( _userAgent );
    _httpRequest->performHTTPDelete( uri );
    
    _transferCompletedCondition.wait( lock );
    
    delete _httpRequest;
}