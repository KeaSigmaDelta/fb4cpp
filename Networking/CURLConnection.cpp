// CURLConnection.cpp
//
// See header file for details
//
// Copyright (C) 2013 by Kea Sigma Delta Limited, all rights reserved

#include "EventEmitterImpl.h"
#include "Exception.h"
#include "Networking/CURLConnection.h"
#include "Networking/CURLConnectionListener.h"

#include <boost/scoped_ptr.hpp>

#if defined(WIN32) || defined(WIN64)
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wldap32.lib")
#endif

#ifndef _
// ##### FIXME! ##### We actually do want these strings to be translated
#define _(x) x
#endif

using namespace Networking;

// ----- SUPPORT CLASSES -----

/** A class to handle global initialisation and cleanup.
 */
class CURLGlobal
{
public:
	/** Ensures that CURL is initialised.
	 * NOTE: Cleanup is handled automatically, so don't worry about that.
	 *
	 * @return CURLCode CURLE_OK if successful; otherwise it returns an error code
	 */
	static CURLcode ensureCURLReady();

private:
	CURLGlobal()
	{
		retCode = curl_global_init(CURL_GLOBAL_ALL);
	}

public:
	~CURLGlobal()
	{
		curl_global_cleanup();
	}

private:
	CURLcode retCode;

	/** The one and only CURLGlobal object.
	 */
	static boost::scoped_ptr<CURLGlobal> curlGlobal;
};

CURLcode CURLGlobal::ensureCURLReady()
{
	if(!curlGlobal)
	{
		// Not initialised yet
		curlGlobal.reset(new CURLGlobal());
	}

	CURLcode retCode = curlGlobal->retCode;

	if(retCode != CURLE_OK)
	{
		// Failed, so let's cleanup
		curlGlobal.reset();
	}

	return retCode;
}

boost::scoped_ptr<CURLGlobal> CURLGlobal::curlGlobal;


// ----- Support Functions -----


/** The CURL write hook for when data is received.
 */
extern "C" size_t receiveData(void *buffer, size_t size, size_t nitems, void *userp)
{
    CURLConnection *conn = (CURLConnection*) userp;
    
    return conn->receiveDataFromServer(buffer, size, nitems);
}

/** The CURL write hook for when header data is received.
 */
extern "C" size_t receiveHeaderData(void *buffer, size_t size, size_t nitems, void *userp)
{
    CURLConnection *conn = (CURLConnection*) userp;
    
    return conn->receiveHeaderDataFromServer(buffer, size, nitems);
}


/** The CURL read hook for sending data to the server.
 */
extern "C" size_t sendData(char *buffer, size_t size, size_t nitems, void *userp)
{
    CURLConnection *conn = (CURLConnection*) userp;
    
    return conn->sendDataToServer(buffer, size, nitems);
}

/** The CURL progress hook.
 * NOTE: This is the CURLOPT_XFERINFOFUNCTION variant
 */
extern "C" int progressCallback(void *userp, curl_off_t dlTotal, curl_off_t dlNow, curl_off_t ulTotal, curl_off_t ulNow)
{
	CURLConnection *conn = (CURLConnection*) userp;

	return conn->progressUpdate(dlTotal, dlNow, ulTotal, ulNow);
}

int debugCurlCallback( CURL *curl, curl_infotype infotype, char *data, size_t dataSize, void *userp )
{
    if ( infotype == CURLINFO_HEADER_OUT )
    {
        std::string headerString = std::string( data, dataSize );
        std::cout << headerString;
#ifdef _MSC_VER
		OutputDebugStringA( headerString.c_str() );
#endif
    }
    if ( infotype == CURLINFO_DATA_OUT )
    {
        std::string dataString = std::string( data, dataSize );
        std::cout << dataString;
#ifdef _MSC_VER
		OutputDebugStringA( dataString.c_str() );
#endif
    }
    
    return 0;
}

// ----- CLASS METHODS -----

CURLConnection::CURLConnection()
{
	// Important! Call this *BEFORE* doing anything else
	init();

	// Ensure that CURL is initialised 
	CURLcode retCode = CURLGlobal::ensureCURLReady();
	if(retCode != CURLE_OK)
	{
		// Something went really wrong
		throw Exception(_("Could not initialise CURL"), curl_easy_strerror(retCode));
	}

	// Create the CURL handle
	curlHandle = curl_easy_init();

	if(!curlHandle)
	{
		throw Exception(_("Couldn't create a nework connection"));
	}

	retCode = setDefaultOptions();
	if(retCode != CURLE_OK)
	{
		// Something went wrong
		curl_easy_cleanup(curlHandle);
		curlHandle = NULL;
		throw Exception(_("Could not set up network connection"), curl_easy_strerror(retCode));
	}

	// Start the worker thread
	workerThread = boost::thread(&CURLConnection::workerThreadFunc, this);
}

CURLConnection::CURLConnection(const CURLConnection& conn)
{
	// Important! Call this *BEFORE* doing anything else
	init();

	// Ensure that CURL is initialised 
	CURLcode retCode = CURLGlobal::ensureCURLReady();
	if(retCode != CURLE_OK)
	{
		// Something went really wrong
		throw Exception(_("Could not initialise CURL"), curl_easy_strerror(retCode));
	}

	// Create the CURL handle
	curlHandle = curl_easy_duphandle(conn.curlHandle);

	if(!curlHandle)
	{
		throw Exception(_("Couldn't clone a nework connection"));
	}
	
	retCode = setDefaultOptions();
	if(retCode != CURLE_OK)
	{
		// Something went wrong
		curl_easy_cleanup(curlHandle);
		curlHandle = NULL;
		throw Exception(_("Could not set up network connection"), curl_easy_strerror(retCode));
	}

	// Start the worker thread
	workerThread = boost::thread(&CURLConnection::workerThreadFunc, this);
}

CURLConnection::~CURLConnection()
{
	if(workerThread.joinable())
	{
		// Signal the thread to stop
		{
			// Accessing variables in a thread-safe manner
			boost::unique_lock<boost::mutex> mutexLock(mutex);

			// Time to quit...
			workerReq = CURLCTR_QUIT;
		}
		workerSignal.notify_all();

		// Wait for it to be done
		workerThread.join();
	}
	if(mimeHeader)
	{
		curl_slist_free_all(mimeHeader);
		mimeHeader = NULL;
	}
	if(curlHandle)
	{
		curl_easy_cleanup(curlHandle);
		curlHandle = NULL;
	}
}

CURLcode CURLConnection::setOption(CURLoption option, long value)
{
	return curl_easy_setopt(curlHandle, option, value);
}

CURLcode CURLConnection::setOption(CURLoption option, unsigned long value)
{
	return curl_easy_setopt(curlHandle, option, value);
}

/*
CURLcode CURLConnection::setOption(CURLoption option, curl_off_t value)
{
	return curl_easy_setopt(curlHandle, option, value);
}
*/
 
CURLcode CURLConnection::setOption(CURLoption option, char* value)
{
	return curl_easy_setopt(curlHandle, option, value);
}

CURLcode CURLConnection::setOption(CURLoption option, struct curl_httppost* value)
{
	return curl_easy_setopt(curlHandle, option, value);
}

CURLcode CURLConnection::setOption(CURLoption option, struct curl_slist* value)
{
	return curl_easy_setopt(curlHandle, option, value);
}

CURLcode CURLConnection::setOption(CURLoption option, void* value)
{
	return curl_easy_setopt(curlHandle, option, value);
}

bool CURLConnection::perform()
{
	{
		// Accessing variables in a thread-safe manner
		boost::unique_lock<boost::mutex> mutexLock(mutex);

		// Check if there's an existing operation
		if(busy)
		{
			// Can't start a new transfer while an existing one is busy
			return false;
		}
	
		// Signal the worker thread to start
		workerReq = CURLCTR_PROCESS;
		busy = true;

		// Reset the progress counters
		downloadTotal = 0;
		downloadNow = 0;
		uploadTotal = 0;
		uploadNow = 0;

		// Clear the receive buffer
		resetReceiveBuffer();
	}

	// Wake the worker thread
	workerSignal.notify_one();

	return true;
}

long CURLConnection::getProtocolResponseCode() const
{
	return protocolResponseCode;
}

bool CURLConnection::cancel()
{
	{
		// Accessing variables in a thread-safe manner
		boost::unique_lock<boost::mutex> mutexLock(mutex);

		// Check if there's an existing operation
		if(!busy || workerReq == CURLCTR_CANCEL)
		{
			// Can't cancel a transfer when none exists
			return false;
		}
	
		// Signal the worker thread to abort the transfer
		workerReq = CURLCTR_CANCEL;
		busy = true;

		// Reset the progress counters
		downloadTotal = 0;
		downloadNow = 0;
		uploadTotal = 0;
		uploadNow = 0;
	}

	// Wake the worker thread
	workerSignal.notify_one();

	return true;
}

bool CURLConnection::isBusy() const
{
	boost::unique_lock<boost::mutex> mutexLock(const_cast<boost::mutex&>(mutex));

	return busy;
}

bool CURLConnection::getProgress(uint64* downloadTotal, uint64* downloadNow, uint64* uploadTotal, uint64* uploadNow) const
{
	boost::unique_lock<boost::mutex> mutexLock(const_cast<boost::mutex&>(mutex));

	if(downloadTotal) { *downloadTotal = this->downloadTotal; }
	if(downloadNow) { *downloadNow = this->downloadNow; }
	if(uploadTotal) { *uploadTotal = this->uploadTotal; }
	if(uploadNow) { *uploadNow = this->uploadNow; }

	return busy;
}

CURLcode CURLConnection::getLastRetCode() const
{
	boost::unique_lock<boost::mutex> mutexLock(const_cast<boost::mutex&>(mutex));

	return retCode;
}

std::string CURLConnection::getErrorMessage() const
{
	return curl_easy_strerror(getLastRetCode());
}

std::string CURLConnection::getReceivedData() const
{
	boost::unique_lock<boost::mutex> mutexLock(const_cast<boost::mutex&>(mutex));

	return receiveBuffer.str();
}

std::string CURLConnection::getReceivedHeaderData() const
{
	boost::unique_lock<boost::mutex> mutexLock(const_cast<boost::mutex&>(mutex));

	return headerBuffer.str();
}

void CURLConnection::reset()
{
	resetSendBuffer();
	resetHeaderBuffer();
	resetReceiveBuffer();
	curl_easy_reset(curlHandle);
}

CURLConnection& CURLConnection::operator=(const CURLConnection& conn)
{
	if(curlHandle)
	{
		curl_easy_cleanup(curlHandle);
	}
	curlHandle = curl_easy_duphandle(conn.curlHandle);

	if(!curlHandle)
	{
		throw Exception(_("Couldn't clone a nework connection"));
	}

	return *this;
}

void CURLConnection::enableSSLAuth()
{
    curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYPEER, 1);
}
	
void CURLConnection::disableSSLAuth()
{
    curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYPEER, 0);
}

std::string CURLConnection::urlEncode(const std::string& str)
{
	const char *encStr = curl_easy_escape(curlHandle, str.c_str(), 0);
	std::string encodedString = encStr;
	curl_free((void*)encStr);
	return encodedString;
}
		
std::string CURLConnection::urlDecode(const std::string& str)
{
	const char *decStr = curl_easy_unescape(curlHandle, str.c_str(), 0, NULL);
	std::string decodedString = decStr;
	curl_free((void*)decStr);
	return decodedString;
}

void CURLConnection::addListener(CURLConnectionListener *listener)
{
	// We only overrode this method so that we could provide class-specific documentation
	EventEmitter<CURLConnection, CURLConnectionListener>::addListener(listener);
}

bool CURLConnection::removeListener(CURLConnectionListener *listener)
{
	return EventEmitter<CURLConnection, CURLConnectionListener>::removeListener(listener);
}

size_t CURLConnection::receiveDataFromServer(void *buffer, size_t size, size_t nitems)
{
    int nBytes = size * nitems;

    receiveBuffer.write((const char*)buffer, nBytes);
    
	if(receiveBuffer.bad())
	{
	    receiveBuffer.clear();
	    return 0;
	}
    
    return nBytes;
}

size_t CURLConnection::receiveHeaderDataFromServer(void *buffer, size_t size, size_t nitems)
{
	int nBytes = size * nitems;
    headerBuffer.write((const char*)buffer, nBytes);
    
	if(headerBuffer.bad())
	{
	    headerBuffer.clear();
	    return 0;
	}
    
    return nBytes;
}

size_t CURLConnection::sendDataToServer(char *buffer, size_t size, size_t nitems)
{
    size_t nBytes = size * nitems;

	if(directSendBuffer)
	{
		// Pulling data directly from an external buffer
		if(directSendBufferPos > directSendBufferSize) { directSendBufferPos = directSendBufferSize; }
		if(nBytes + directSendBufferPos > directSendBufferSize)
		{
			nBytes = (size_t)(directSendBufferSize - directSendBufferPos);
		}

		memcpy(buffer, ((char*)directSendBuffer) + directSendBufferPos, nBytes);

		directSendBufferPos += nBytes;
	}
	else
	{
		// Reading from the internal send buffer
		sendBuffer.read(buffer, nBytes);
		nBytes = (int)sendBuffer.gcount();
	
		// We don't care about the status bits, but they do need to be cleared
		sendBuffer.clear();
	}
    
    return nBytes;
}

int CURLConnection::progressUpdate(curl_off_t dlTotal, curl_off_t dlNow, curl_off_t ulTotal, curl_off_t ulNow)
{
	// Update the progress fields
	{
		boost::unique_lock<boost::mutex> mutexLock(mutex);
		downloadTotal = (uint64)dlTotal;
		downloadNow = (uint64)dlNow;
		uploadTotal = (uint64)ulTotal;
		uploadNow = (uint64)ulNow;
	}

	// Broadcast the progress to listeners
	bool keepGoing = broadcastProgress((uint64)dlTotal, (uint64)dlNow, (uint64)ulTotal, (uint64)ulNow);
	if(workerReq == CURLCTR_CANCEL)
	{
		// Abort the operation
		keepGoing = false;
	}

	if(keepGoing)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

CURLcode CURLConnection::setDefaultOptions()
{
	// Do NOT swap the followingwrite and read functions around
    // What is a write function from libCURL's point-of-view is part 
    // of a data receive operation, and libCURL's read function gets
    // data from this class to send to the server
	curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, receiveData);
	curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void*)this);
	curl_easy_setopt(curlHandle, CURLOPT_READFUNCTION, sendData);
	curl_easy_setopt(curlHandle, CURLOPT_READDATA, (void*)this);

    //curl_easy_setopt(curlHandle, CURLOPT_DEBUGFUNCTION, debugCurlCallback);
	//curl_easy_setopt(curlHandle, CURLOPT_DEBUGDATA, (void*)this);
    //curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, 1L );

	curl_easy_setopt(curlHandle, CURLOPT_XFERINFOFUNCTION, progressCallback);
	curl_easy_setopt(curlHandle, CURLOPT_XFERINFODATA, (void*)this);
	curl_easy_setopt(curlHandle, CURLOPT_NOPROGRESS, 0); // Enabling progress updates

	return CURLE_OK;
}

void CURLConnection::init()
{
	// Set some fields to NULL first for safety in case of exceptions
	curlHandle = NULL; 
	mimeHeader = NULL;

	retCode = CURLE_OK;
	workerReq = CURLCTR_NOTHING;
	
	busy = false;
	downloadTotal = 0;
	downloadNow = 0;
	uploadTotal = 0;
	uploadNow = 0;
	protocolResponseCode = 0;

	resetSendBuffer();
	resetHeaderBuffer();
	resetReceiveBuffer();
}

void CURLConnection::resetReceiveBuffer()
{
    receiveBuffer.str("");
    receiveBuffer.clear();
}

void CURLConnection::resetHeaderBuffer()
{
	headerBuffer.str("");
    headerBuffer.clear();
}

bool CURLConnection::setSendBuffer(const std::string& data, const std::string& mimeType)
{
	if(busy)
	{
		return false;
	}

	resetSendBuffer();
	sendBuffer.write(data.c_str(), data.size());
	curl_off_t size = (curl_off_t)data.size();

	setOption(CURLOPT_INFILESIZE_LARGE, (unsigned long)size);
	setOption(CURLOPT_POSTFIELDSIZE_LARGE, (unsigned long)size); // Just in case this is for a POST operation

	setSendBufferMIMEType(mimeType, (unsigned long)size);

	return true;
}

bool CURLConnection::setSendBuffer(const void* dataBuffer, uint64 dataSize,
			const std::string& mimeType, bool inPlaceBuffer)
{
	if(busy)
	{
		return false;
	}

	if(inPlaceBuffer)
	{
		directSendBuffer = dataBuffer;
		directSendBufferSize = dataSize;
		directSendBufferPos = 0;
	}
	else
	{
		resetSendBuffer();
		sendBuffer.write((const char*)dataBuffer, dataSize); 
		
	}
	curl_off_t putSize = (curl_off_t)dataSize;
	setOption(CURLOPT_INFILESIZE_LARGE, (unsigned long)putSize);
	setOption(CURLOPT_POSTFIELDSIZE_LARGE, (unsigned long)putSize); // Just in case this is for a POST operation

	setSendBufferMIMEType(mimeType, (unsigned long)putSize);

	return true;
}
	
void CURLConnection::resetSendBuffer()
{
    sendBuffer.str("");
    sendBuffer.clear();
	
	directSendBuffer = NULL;
	directSendBufferSize = 0;
	directSendBufferPos = 0;
}

bool CURLConnection::setSendBufferMIMEType(const std::string& mimeType, unsigned long contentLength)
{
	if(busy)
	{
		return false;
	}

	/* commented out due to bug in libcurl
	if(mimeHeader)
	{
		// Clear if the header is different from last time. Otherwise, we reuse it
		if(mimeType.compare(mimeHeader->data) != 0)
		{
			curl_slist_free_all(mimeHeader);
			mimeHeader = NULL;
		}
	}
	*/

	curl_slist_free_all(mimeHeader);
	mimeHeader = NULL;

	//if(!mimeHeader && mimeType.length() > 0) commented out due to bug in libcurl
	//{
		std::string mimeHeaderStr = std::string("Content-Type: ") + mimeType;
		mimeHeader = curl_slist_append(mimeHeader, mimeHeaderStr.c_str());
	//}

	// for some reason automatic Content-Length detection not working properly in windows version of libcurl v7.37.0
	// so we have to set it manually
	std::stringstream contentLengthStr;
	contentLengthStr << contentLength;
	mimeHeaderStr = std::string("Content-Length: ") + contentLengthStr.str();
	mimeHeader = curl_slist_append(mimeHeader, mimeHeaderStr.c_str());
	
	// Finally, set the HTTP header
	setOption(CURLOPT_HTTPHEADER, mimeHeader);

	return true;
}

bool CURLConnection::broadcastProgress(uint64 downloadTotal, uint64 downloadNow, uint64 uploadTotal, uint64 uploadNow)
{
	bool keepGoing = true;
	EVENTEMITTER_EMPTYCHECK_RET(keepGoing)
	EVENTEMITTER_LOOPSTART(CURLConnectionListener)
		keepGoing = keepGoing && (*currListener)->networkTransferProgress(this, downloadTotal, downloadNow, uploadTotal, uploadNow);
	EVENTEMITTER_LOOPEND

	return keepGoing;
}

void CURLConnection::broadcastCompletionEvent(CURLcode retCode)
{
	EVENTEMITTER_EMPTYCHECK
	EVENTEMITTER_LOOPSTART(CURLConnectionListener)
		(*currListener)->networkTransferCompleted(this, retCode);
	EVENTEMITTER_LOOPEND
}


void CURLConnection::workerThreadFunc(CURLConnection* conn)
{
	CURLConnThreadRequest workerReq = CURLCTR_NOTHING;
	bool quit = false;
	boost::thread& thisThread = conn->workerThread;

	while(!quit)
	{
		// Wait for the main thread to signal that there's work for us
		{
			boost::unique_lock<boost::mutex> mutexLock(conn->mutex);
			conn->workerSignal.wait(mutexLock, [conn]{return (conn->workerReq != CURLCTR_NOTHING);});
			workerReq = conn->workerReq;
			
			// Reset the workerReq variable so that we don't retrigger the same action multiple times
			conn->workerReq = CURLCTR_NOTHING;
		}

		switch(workerReq)
		{
		case CURLCTR_PROCESS:
			{
				// Process
				CURLcode retCode = curl_easy_perform(conn->curlHandle);

				{
					// We're done
					boost::unique_lock<boost::mutex> mutexLock(conn->mutex);
					conn->busy = false;

					// Output the return codes
					conn->retCode = retCode;
					conn->protocolResponseCode = 0;
					curl_easy_getinfo(conn->curlHandle, CURLINFO_RESPONSE_CODE, &(conn->protocolResponseCode));

					// Clear the header
					conn->setOption(CURLOPT_HTTPHEADER, (void*)NULL);
				}

				// Emit completion event
				conn->broadcastCompletionEvent(retCode);

				break;
			}
		case CURLCTR_QUIT:
			// Time to go...
			quit = true;
			break;

		default:
			; // Do nothing
		}
	}
}
