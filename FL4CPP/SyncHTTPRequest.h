#ifndef __FL4CPP_SYNC_HTTP_REQUEST__
#define __FL4CPP_SYNC_HTTP_REQUEST__

#include "Networking/CURLConnectionListener.h"

#include <boost/thread.hpp>

namespace boost
{
    class thread;
    class condition_variable;
    class mutex;
}

namespace Networking
{
    class HTTPRequest;
}

namespace fl4cpp
{
    /** Provides functionality to make blocking requests to a web server.
     * By default takes User Agent string from Settings singleton.
     */
    class SyncHTTPRequest : public Networking::CURLConnectionListener
    {
    public:
        /** Constructs SyncHTTPRequest object.
         */
        SyncHTTPRequest();
        virtual ~SyncHTTPRequest() {};
        
        /** Sets User Agent string.
         *
         * @param userAgent User Agent string.
         */
        void setUserAgent( std::string userAgent ) { _userAgent = userAgent; };
        
        /** Performs HTTP Get request.
         *
         * @param uri URI of the request.
         * @param receivedData Data received from a server.
         *
         * @return True if request was successfully completed, otherwise - false.
         */
        bool performGet( std::string uri, std::string* receivedData );
        
        /** Performs HTTP Post request.
         *
         * @param uri URI of the request.
         * @param dataToSend Data to send.
         * @param receivedData Data received from a server.
         *
         * @return True if request was successfully completed, otherwise - false.
         */
        bool performPost( std::string uri, std::string* dataToSend, std::string* receivedData );
        
        /** Performs HTTP Post request.
         *
         * @param uri URI of the request.
         * @param dataToSend Data to send.
         * @param dataSize Size of data to send.
         * @param receivedData Data recevied from a server.
         *
         * @return True if request was successfully completed, otherwise - false.
         */
        bool performPost( std::string uri, void* dataToSend, unsigned long dataSize, std::string* receivedData );
        
        /** Performs HTTP Post request with content of dataToSend argument transmitted as multipart/form-data.
         *
         * @param uri URI of the request.
         * @param dataFieldName Data name (form-data "name" field).
         * @param dataToSend Data to send.
         * @param dataSize Size of the data data to send.
         * @param mimeType MIME type of data.
         * @param receivedData Data recevied from a server.
         *
         * @return True if request was successfully completed, otherwise - false.
         */
        bool performPostFormWithData( std::string uri,
                                     std::string dataFieldName,
                                     void* dataToSend,
                                     unsigned long dataSize,
                                     std::string mimeType,
                                     std::string* receivedData );
        
        /** Performs HTTP Delete request.
         *
         * @param uri URI of the request.
         * @param receivedData Data received from a server.
         *
         * @return True if request was successfully completed, otherwise - false.
         */
        bool performDelete( std::string uri, std::string* reveicedData );
        
    protected:
        /** Callback method that is invoked periodically 
         * during a transfer operation to give a progress update.
         *
         * @param source pointer to the source.
         * @param downloadTotal the total number of bytes to be downloaded.
		 * @param downloadNow the total number of bytes that have been downloaded so far.
		 * @param uploadTotal the total number of bytes to be downloaded.
		 * @param uploadNow the total number of bytes that have been downloaded so far.
		 *
		 * @return Should return true to a caller in order to proceed with transaction.
         * Returning falce interrupts transaction.
		 */
        virtual bool networkTransferProgress( Networking::CURLConnection* source, uint64 downloadTotal, uint64 downloadNow, uint64 uploadTotal, uint64 uploadNow );
        
        /** Callback metod that is invoked when a transfer 
         * operation completes (successfully or unsuccessfully).
         *
         * @param source pointer to the source.
		 * @param retCode the return code indicating if the transfer was successful, or if it ended with an error.
         */
		virtual void networkTransferCompleted( Networking::CURLConnection* source, CURLcode retCode );
        
        /** Thread function that performs HTTP Get request.
         *
         * @param uri URI of the request.
         */
        void threadFuncGet( std::string uri );
        
        /** Thread function that performs basic HTTP Post request.
         *
         * @param uri URI of the request.
         * @param dataToSend Data to send.
         */
        void threadFuncPost( std::string uri, std::string* dataToSend );
        
        /** Thread function that performs basic HTTP Post request.
         *
         * @param uri URI of the request.
         * @param dataToSend Data to send.
         * @param dataSize Size of tha data to send.
         */
        void threadFuncPost2( std::string uri, void* dataToSend, unsigned long dataSize );
        
        /** Thread function that performs multipart/form-data HTTP Post request.
         *
         * @param uri URI of the request.
         * @param dataFieldName Data name (form-data "name" field).
         * @param dataToSend Data to send.
         * @param dataSize Size of the data to send.
         * @param mimeType MIME type of the data.
         */
        void threadFuncPostFormWithDate( std::string uri,
                                        std::string dataFieldName,
                                        void* dataToSend,
                                        unsigned long dataSize,
                                        std::string mimeType );
        
        /** Thread function that performs HTTP Delete request.
         * @param uri URI of the request.
         */
        void threadFuncDelete( std::string uri );

    protected:
        /** Buffer for received data.
         */
        std::string* _receivedData;
        
        /** User Agent string.
         */
        std::string _userAgent;
        
        /** Flag that signalize that a request wasn't completed or was completed with error.
         */
        bool _errorOccured;
        
        /** Mutex for HTTP request thread.
         */
        boost::mutex _threadFuncMutex;
        
        /** Condition variable that wakes waiting threads when HTTP request completes.
         */
        boost::condition_variable _transferCompletedCondition;
        
        /** Object that provides non-blocking HTTP requests functionality.
         */
        Networking::HTTPRequest* _httpRequest;
    };
}
#endif // __FL4CPP_SYNC_HTTP_REQUEST__
