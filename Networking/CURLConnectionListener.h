// CURLConnectionListener.h

#ifndef __CURLCONNECTIONLISTENER_H__
#define __CURLCONNECTIONLISTENER_H__

#include <curl/curl.h>

#include "EventListener.h"
#include "Types.h"

namespace Networking
{
	class CURLConnection;
	
	/** Base class for all objects that need to receive events from CURLConnection.
	 *
	 * WARNING: The event notification methods are usually called from a separate thread instead 
	 * of the main one. Make sure that your code is thread-safe. Or, send a signal back to the main thread.
	 *
	 *
	 * @author Hans de Ruiter
	 * 
	 * @copyright 2013 by Kea Sigma Delta Limited, all rights reserved (See license.txt for 
	 * licensing information)
	 *
	 * @version 0.1 -- 2013/11/25 - Initial version
	 */
	class CURLConnectionListener : public EventListener<CURLConnection, CURLConnectionListener>
	{
	public:
		CURLConnectionListener();

		virtual ~CURLConnectionListener();

		/** Called periodically during a transfer operation to give a progress update.
		 * This can be called many times a second while a transfer is active, or down to about once a second during slow periods.
		 *
		 * Unknown/unused argument values will be set to zero (like if you only download data, the upload size will remain 0). Many times the 
		 * callback will be called one or more times first, before it knows the data sizes so a program must be made to handle that.
		 *
		 * IMPORTANT: This method is called from a separate thread instead of the main one. Make sure that your code is
		 * thread-safe. Or, send a signal back to the main thread for further processing.
		 *
		 * @param downloadTotal the total number of bytes to be downloaded
		 * @param downloadNow the total number of bytes that have been downloaded so far
		 * @param uploadTotal the total number of bytes to be downloaded
		 * @param uploadNow the total number of bytes that have been downloaded so far
		 *
		 * @return bool 
		 */
		virtual bool networkTransferProgress(Networking::CURLConnection* source, uint64 downloadTotal, uint64 downloadNow, uint64 uploadTotal, uint64 uploadNow);

		/** Called when a transfer operation completes (successfully or unsuccessfully).
		 * IMPORTANT
		 *
		 * @param pointer to the source
		 * @param retCode the return code indicating if the transfer was successful, or if it ended with an error
		 */
		virtual void networkTransferCompleted(Networking::CURLConnection* source, CURLcode retCode);
	};
};

#endif
