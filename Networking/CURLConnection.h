// CURLConnection.h

#ifndef __CURLCONNECTION_H__
#define __CURLCONNECTION_H__

#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include <curl/curl.h>
#include <sstream>

#include "EventEmitter.h"
#include "Types.h"


// ----- PRIVATE DATATYPES AND DEFINITIONS -----
namespace Networking
{
	typedef enum CURLConnThreadRequest_e
	{
		CURLCTR_NOTHING = 0, // Request nothing (the default state)
		CURLCTR_PROCESS, // Request that processing start
		CURLCTR_CANCEL, // Request that the current operation be cancelled
		CURLCTR_QUIT, // Request that the thread quits
	} CURLConnThreadRequest;
};


namespace Networking
{
	/** A support datatype for CURLConnection.
	 */
	//enum CURLConnThreadRequest_e;
	//typedef enum CURLConnThreadRequest_e CURLConnThreadRequest;

	class CURLConnectionListener;

	/** Base class for CURL network connections.
	 *
	 * To use, create a CURLConnection, set the relevant options/settings, and then call perform() to
	 * make the request/transfer happen. You can use callback hooks to perform tasks such as monitoring
	 * progress and handling errors. 
	 *
	 * This is a relatively low-level class. It is recommended that you use more specific classes such 
	 * as HTTPRequest, wherever possible.
	 *
	 * @author Hans de Ruiter
	 * 
	 * @copyright 2013 by Kea Sigma Delta Limited, all rights reserved (See license.txt for 
	 * licensing information)
	 *
	 * @version 0.1 -- 2013/11/15 - Initial version
	 */
	class CURLConnection : public EventEmitter<CURLConnection, CURLConnectionListener>
	{
	public:
		/** Constructs a CURLConnection.
		 */
		CURLConnection();

		/** Copy constructor.
		 * This creates a duplicate connection.
		 */
		CURLConnection(const CURLConnection& conn);

		virtual ~CURLConnection();

		/** Sets a CURL option.
		 * 
		 * NOTE: This is a low-level method. Where possible, we recomment using task-specific methods
		 *
		 * @param option the option's tag (e.g., CURLOPT_VERBOSE)
		 * @param value the value to set it to
		 *
		 * @return CURLcode an error code if this failed, or CURLE_OK if successful
		 */
		CURLcode setOption(CURLoption option, long value);

		/** Sets a CURL option.
		 * 
		 * NOTE: This is a low-level method. Where possible, we recomment using task-specific methods
		 *
		 * @param option the option's tag (e.g., CURLOPT_VERBOSE)
		 * @param value the value to set it to
		 *
		 * @return CURLcode an error code if this failed, or CURLE_OK if successful
		 */
		CURLcode setOption(CURLoption option, unsigned long value);

		/** Sets a CURL option.
		 * 
		 * NOTE: This is a low-level method. Where possible, we recomment using task-specific methods
		 *
		 * @param option the option's tag (e.g., CURLOPT_VERBOSE)
		 * @param value the value to set it to
		 *
		 * @return CURLcode an error code if this failed, or CURLE_OK if successful
		 */
		//CURLcode setOption(CURLoption option, curl_off_t value);

		/** Sets a CURL option.
		 * 
		 * NOTE: This is a low-level method. Where possible, we recomment using task-specific methods
		 *
		 * @param option the option's tag (e.g., CURLOPT_VERBOSE)
		 * @param value the value to set it to
		 *
		 * @return CURLcode an error code if this failed, or CURLE_OK if successful
		 */
		CURLcode setOption(CURLoption option, char* value);

		/** Sets a CURL option.
		 * 
		 * NOTE: This is a low-level method. Where possible, we recomment using task-specific methods
		 *
		 * @param option the option's tag (e.g., CURLOPT_VERBOSE)
		 * @param value the value to set it to
		 *
		 * @return CURLcode an error code if this failed, or CURLE_OK if successful
		 */
		CURLcode setOption(CURLoption option, struct curl_httppost* value);

		/** Sets a CURL option.
		 * 
		 * NOTE: This is a low-level method. Where possible, we recomment using task-specific methods
		 *
		 * @param option the option's tag (e.g., CURLOPT_VERBOSE)
		 * @param value the value to set it to
		 *
		 * @return CURLcode an error code if this failed, or CURLE_OK if successful
		 */
		CURLcode setOption(CURLoption option, struct curl_slist* value);

		/** Sets a CURL option.
		 * 
		 * NOTE: This is a low-level method. Where possible, we recomment using task-specific methods
		 *
		 * @param option the option's tag (e.g., CURLOPT_VERBOSE)
		 * @param value the value to set it to
		 *
		 * @return CURLcode an error code if this failed, or CURLE_OK if successful
		 */
		CURLcode setOption(CURLoption option, void* value);

		/** Performs whatever request/transfer needs to happen.
		 * 
		 * IMPORTANT: This will fail if an existing transfer is active. Use abort() to cancel an 
		 * existing transfer.
		 *
		 * @return bool true if the transfer started, and false if not (e.g., an existing
		 * transfer is busy)
		 */
		bool perform(); 

		/** Return's the protocol's response code (e.g., the HTTP response code) for the last operation.
		 * Set to 0 if no response code was received.
		 */
		long getProtocolResponseCode() const;

		/** Cancels a request/transfer, if one is active.
		 *
		 * @return bool true if aborted; false if there was nothing to abort
		 */
		bool cancel();

		/** Returns true if the connection is busy (ie., a request/transfer is in progress).
		 */
		bool isBusy() const;

		/** Gets the current transfer progress.
		 *
		 * NOTE: The total transfer may not be known initially, so you should be able to handle this case.
		 *
		 * @param downloadTotal the total number of bytes to be downloaded will be written to this parameter
		 * (set to NULL to ignore)
		 * @param downloadNow the total number of bytes that has already been downloaded will be written to 
		 * this parameter (set to NULL to ignore)
		 * @param uploadTotal the total number of bytes to be uploaded will be written to this parameter
		 * (set to NULL to ignore)
		 * @param iploadNow the total number of bytes that has already been uploaded will be written to 
		 * this parameter (set to NULL to ignore)
		 *
		 * @return bool true if a transfer is in progress; false otherwise
		 */
		bool getProgress(uint64* downloadTotal, uint64* downloadNow, uint64* uploadTotal, uint64* uploadNow) const;

		/** Returns the return-code for the last completed request transfer.
		 * You should call isBusy() first, to make sure that the action completed.
		 */
		CURLcode getLastRetCode() const;

		/** Returns the error message (if any) for the last "completed" upload.
		 */
		virtual std::string getErrorMessage() const;

		/** Gets the received data. 
		 * This method should be called after a transfer is complete. Calling it during a transfer will 
		 * likely result in incomplete data. 
		 *
		 * @param std::string the received data
		 */
		std::string getReceivedData() const;

		/** Gets the received header data. 
		 * This method should be called after a transfer is complete. Calling it during a transfer will 
		 * likely result in incomplete data. 
		 *
		 * @param std::string the received data
		 */
		std::string getReceivedHeaderData() const;

		/** Resets this connection.
		 * This resets all settings/options back to their defaults.
		 */
		void reset();

		/** Assignment operator. 
		 * IMPORTANT: This is actually a copy operation that deletes the old connection.
		 */
		CURLConnection& operator=(const CURLConnection& conn);

		/** Enables SSL server authentication (which is enabled by default).
		 */
		void enableSSLAuth();
	
		/** Disables SSL server authentication (which is enabled by default).
		 */
		void disableSSLAuth();

		/** URL-encodes the given string.
		 */
		std::string urlEncode(const std::string& str);
		
		/** URL-decodes the given string.
		 */
		std::string urlDecode(const std::string& str);

		/** Adds an event listener to this object's listener list.
		 *
		 * IMPORTANT: The listener will receive progress update calls from a separate thread,
		 * so make sure that your code is thread-safe. You may wish to signal the main thread
		 * instead of performing actual work in the progress update method.
		 *
		 * @param listener pointer to the event listener
		 */
		virtual void addListener(CURLConnectionListener *listener);

		/** Removes the given update listener from this object.
		 * 
		 * @param updateListener the update listener to remove
		 *
		 * @return bool true if successful, false if not found
		 */
		virtual bool removeListener(CURLConnectionListener *listener);
	
		/** Callback hook for CURL to pass data received from the server to this
		 * object.
		 *
		 * @param buffer the buffer that the received 
		 * @param size the size of each element in the buffer
		 * @param nitems the number of elements in the buffer (the total size in bytes
		 * is size * nmemb)
		 *
		 * @return the number of bytes actually read
		 */
		size_t receiveDataFromServer(void *buffer, size_t size, size_t nitems);

		/** Callback hook for CURL to pass header data received from the server to this
		 * object.
		 *
		 * @param buffer the buffer that the received 
		 * @param size the size of each element in the buffer
		 * @param nitems the number of elements in the buffer (the total size in bytes
		 * is size * nmemb)
		 *
		 * @return the number of bytes actually read
		 */
		size_t receiveHeaderDataFromServer(void *buffer, size_t size, size_t nitems);
	
		/** Callback hook for CURL to get data to write to the server.
		 *
		 * @param bufptr pointer to the buffer
		 * @param size the size of each element in the buffer
		 * @param nitems the number of elements in the buffer (the total size in bytes
		 * is size * nitems
		 *
		 * @return the number of bytes actually written to the buffer
		 */
		size_t sendDataToServer(char *buffer, size_t size, size_t nitems);

		/** Progress callback hook for CURL.
		 *
		 * @param dlTotal the total to download in bytes
		 * @param dlNow the number of bytes downloaded so far
		 * @param ulTotal the total to upload in bytes
		 * @param ulNow the number of bytes uploaded so far
		 * 
		 * @return int non-zero if the transfer is to be aborted; 0 otherwise
		 */
		virtual int progressUpdate(curl_off_t dlTotal, curl_off_t dlNow, curl_off_t ulTotal, curl_off_t ulNow);

	protected:
		/** Sets the default options/settings.
		 * This includes setting the default callbacks, etc.
		 *
		 * @return CURLcode an error code if this failed, or CURLE_OK if successful
		 */
		CURLcode setDefaultOptions();

		/** Initialise some variables.
		 * Should be called by the constructor BEFORE anything else.
		 */
		void init();

		/** Resets the receive buffer.
		 */
		void resetReceiveBuffer();

		/** Resets the receive buffer.
		 */
		void resetHeaderBuffer();

		/** Sets the send buffer.
		 *
		 * @param data the buffer. NOTE: This will be copied to an internal buffer. use the other
		 * setSendBuffer() method if you wish to use an in-place buffer to avoid this copy
		 * @param mimeType the data's MIME type (e.g., "application/json"). Leave blank if 
		 * you don't want to specify this.
		 *
		 * @return bool true if successful; false if an existing operation was taking place
		 */
		bool setSendBuffer(const std::string& data, const std::string& mimeType = "");

		/** Sets the send buffer.
		 *
		 * @param dataBuffer pointer to the data buffer
		 * @param dataSize the size of the data buffer in bytes
		 * @param inPlaceBuffer set to true to use the data buffer directly. When set to false, the buffer 
		 * will be copied to an internal buffer. IMPORTANT: When set to true, you should wait until the transfer
		 * is complete before modifying or reusing the buffer
		 * @param mimeType the data's MIME type (e.g., "application/json"). Leave blank if 
		 * you don't want to specify this.
		 *
		 * @return bool true if successful; false if an existing operation was taking place
		 */
		bool setSendBuffer(const void* dataBuffer, uint64 dataSize,
			const std::string& mimeType = "", bool inPlaceBuffer = false);

		/** Resets the send buffer.
		 */
		void resetSendBuffer();

		/** Set the upload data's MIME type.
		 *
		 * NOTES: 
		 * - Only call this when no transfer is active
		 * - This is only valid for one transfer. You will have to set it again for the next transfer
		 *
		 * @return bool true if successful; false if an existing operation was taking place
		 */
		bool setSendBufferMIMEType(const std::string& mimeType, unsigned long contentLength);

		/** Broadcasts the current transfer progress.
		 *
		 * @param downloadTotal the total number of bytes to be downloaded
		 * @param downloadNow the total number of bytes that have been downloaded so far
		 * @param uploadTotal the total number of bytes to be downloaded
		 * @param uploadNow the total number of bytes that have been downloaded so far
		 *
		 * @return bool true if the transfer should continue, and false if an abort has been requested
		 */
		virtual bool broadcastProgress(uint64 downloadTotal, uint64 downloadNow, uint64 uploadTotal, uint64 uploadNow);

		/** Broadcasts an operation completion event.
		 *
		 * @param retCode the operation's return code
		 */
		virtual void broadcastCompletionEvent(CURLcode retCode);

		/** The CURL handle.
		 */
		CURL* curlHandle;

		/** The current buffer of data to send to the server.
		 */
		std::stringstream sendBuffer;

		/** Pointer to a direct send buffer (i.e., an externally provided buffer).
		 */
		const void* directSendBuffer;

		/** The size of directSendBuffer.
		 */
		uint64 directSendBufferSize;

		/** The current position in directSendBuffer.
		 */
		uint64 directSendBufferPos;
	
		/** The current buffer for data received from the server.
		 */
		std::stringstream receiveBuffer; 

		/** The current buffer for header data received from the server.
		 */
		std::stringstream headerBuffer; 

		/** The MIME header.
		 */
		struct curl_slist *mimeHeader;

		/** Indicates if a transfer is in progress.
		 */
		bool busy;

		/** The total number of bytes to be downloaded.
		 * Only valid during a transfer.
		 */
		uint64 downloadTotal;

		/** The number of bytes that have been downloaded. 
		 * Only valid during a transfer.
		 */
		uint64 downloadNow;

		/** The total number of bytes to be uploaded.
		 * Only valid during a transfer.
		 */
		uint64 uploadTotal;

		/** The number of bytes that have been uploaded. 
		 * Only valid during a transfer.
		 */
		uint64 uploadNow;

		/** The return code for the last operation.
		 */
		CURLcode retCode;

		/** The protocol's return code for the last operation.
		 */
		long protocolResponseCode;

		/** The worker thread.
		 */
		boost::thread workerThread;

		/** The worker thread's function.
		 */
		static void workerThreadFunc(CURLConnection* conn);

		/** A signal for the worker thread.
		 */
        CURLConnThreadRequest_e workerReq;

		/** A mechanism to signal the worker thread.
		 */
		boost::condition_variable workerSignal;

		/** The mutex for managing data transfer between the main and worker threads.
		 */
		boost::mutex mutex;

	};
};

#endif
