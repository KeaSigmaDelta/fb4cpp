// HTTPRequest.h

#ifndef __HTTPREQUEST_H__
#define __HTTPREQUEST_H__

#include "Networking/CURLConnection.h"

#define HTTP_CODE_ERROR_BADREQUEST 400
#define HTTP_CODE_ERROR_UNAUTHORISED 401
#define HTTP_CODE_ERROR_NOTFOUND 404
#define HTTP_CODE_ERROR_TIMEOUT 408
#define HTTP_CODE_ERROR_INTERNALSERVERERROR 500

namespace Networking
{
	/** Performs HTTP requests.
	 *
	 * @author Hans de Ruiter
	 * 
	 * @copyright 2013 by Kea Sigma Delta Limited, all rights reserved (See license.txt for 
	 * licensing information)
	 *
	 * @version 0.1 -- 2013/11/15 - Initial version
	 */
	class HTTPRequest : public CURLConnection
	{
	public:
		/** Constructs an HTTPRequest object.
		 */
		HTTPRequest();

		virtual ~HTTPRequest();

		/** Set the user agent.
		 *
		 * @param userAgent the user agent string
		 */
		void setUserAgent(const std::string& userAgent);

		// ##### FIXME! ##### headers? MIME Type?

		// ##### FIXME! ##### Login details?

		/** Initiates an HTTP get request.
		 * This function will return immediately, but the transfer will take more time. 
		 * You can install a CURLConnectionListener, and get progress/completion events to
		 * monitor progress. Once the transfer is done, call getReceivedBuffer() to get the
		 * result.
		 * 
		 * @param url the URL to get
		 *
		 * @return bool true if the transfer started, and false if not (e.g., an existing
		 * transfer is busy)
		 */
		bool performHTTPGet(const std::string& url);
		
		/** Initiates an HTTP head request.
		 * This function will return immediately, but the transfer will take more time. 
		 * You can install a CURLConnectionListener, and get progress/completion events to
		 * monitor progress. Once the transfer is done, call getReceivedHeaderData() to get 
		 * the result.
		 * 
		 * @param url the URL to get
		 *
		 * @return bool true if the transfer started, and false if not (e.g., an existing
		 * transfer is busy)
		 */
		bool performHTTPHead(const std::string& url);

		/** Initiates an HTTP put request.
		 * This function will return immediately, but the transfer will take more time. 
		 * You can install a CURLConnectionListener, and get progress/completion events to
		 * monitor progress. Once the transfer is done, call getReceivedBuffer() to get the
		 * result.
		 * 
		 * @param url the URL to get
		 * @param data the data to put
		 * @param mimeType the data's MIME type (e.g., "application/json"). Leave blank if 
		 * you don't want to specify this.
		 *
		 * @return bool true if the transfer started, and false if not (e.g., an existing
		 * transfer is busy)
		 */
		bool performHTTPPut(const std::string& url, const std::string& data, const std::string& mimeType = "");

		/** Initiates an HTTP put request.
		 * This function will return immediately, but the transfer will take more time. 
		 * You can install a CURLConnectionListener, and get progress/completion events to
		 * monitor progress. Once the transfer is done, call getReceivedBuffer() to get the
		 * result.
		 * 
		 * @param url the URL to get
		 * @param dataBuffer the data to put
		 * @param dataSize the size of the data buffer in bytes
		 * @param mimeType the data's MIME type (e.g., "application/json"). Leave blank if 
		 * you don't want to specify this.
		 * @param inPlaceBuffer set to true to use the data buffer directly. When set to false, the buffer 
		 * will be copied to an internal buffer. IMPORTANT: When set to true, you should wait until the transfer
		 * is complete before modifying or reusing the buffer
		 *
		 * @return bool true if the transfer started, and false if not (e.g., an existing
		 * transfer is busy)
		 */
		bool performHTTPPut(const std::string& url, void* dataBuffer, uint64 dataSize, 
			const std::string& mimeType = "", bool inPlaceBuffer = false);

		/** Clears any existing form in preparation for building a new one using addFormField().
		 *
		 * @return bool true if the successful, and false if not (e.g., an existing
		 * transfer is busy)
		 */
		bool resetForm();

		/** Appends a new field to the form.
		 *
		 * @param name the form field's name
		 * @param contents the field's contents
		 * @param errCode pointer to a variable where the CURL form add return code will be written. Set to 
		 * NULL to ignore. NOTE: Will be CURL_FORMADD_OK if this method fails because a transfer is busy
		 *
		 * @return bool true if the successful, and false if not (e.g., an existing
		 * transfer is busy, or some other error occurred)
		 */
		bool addFormField(const std::string& name, const std::string& contents, CURLFORMcode* errCode = NULL);

		/** Appends a new file upload to the form.
		 *
		 * @param name the form field's name
		 * @param filename the file name/path for the file to upload
		 * @param fileUploadName the filename to tell the server (leave empty to use the file's actual name)
		 * @param mimeType the file's MIME type (leave empty to use the default)
		 * @param errCode pointer to a variable where the CURL form add return code will be written. Set to 
		 * NULL to ignore. NOTE: Will be CURL_FORMADD_OK if this method fails because a transfer is busy
		 *
		 * @return bool true if the successful, and false if not (e.g., an existing
		 * transfer is busy, or some other error occurred)
		 */
		bool addFormFileField(const std::string& name, const std::string& filename, const std::string& fileUploadName = "", 
			const std::string& mimeType = "", CURLFORMcode* errCode = NULL);

		/** Appends a new file upload to the form, using an in-memory buffer as the file's contents instead.
		 *
		 * @param name the form field's name
		 * @param buffer pointer to the buffer containing the file's data. IMPORTANT: Do *NOT* free or reuse this 
		 * buffer until after the HTTP post operation is complete
		 * @param bufferSize the size of the buffer in bytes
		 * @param fileUploadName the filename to tell the server (leave empty to use the file's actual name)
		 * @param mimeType the file's MIME type (leave empty to use the default)
		 * @param errCode pointer to a variable where the CURL form add return code will be written. Set to 
		 * NULL to ignore. NOTE: Will be CURL_FORMADD_OK if this method fails because a transfer is busy
		 *
		 * @return bool true if the successful, and false if not (e.g., an existing
		 * transfer is busy, or some other error occurred)
		 */
		bool addFormFileField(const std::string& name, const void* buffer, uint64 bufferSize, 
			const std::string& fileUploadName = "", const std::string& mimeType = "", CURLFORMcode* errCode = NULL);

		/** Initiates an HTTP post request.
		 * This version posts a form built using resetForm() and addFormField(), addFormFileField().
		 * 
		 * This function will return immediately, but the transfer will take more time. 
		 * You can install a CURLConnectionListener, and get progress/completion events to
		 * monitor progress. Once the transfer is done, call getReceivedBuffer() to get the
		 * result.
		 * 
		 * @param url the URL to get
		 *
		 * @return bool true if the transfer started, and false if not (e.g., an existing
		 * transfer is busy)
		 */
		bool performHTTPPostForm(const std::string& url);

		/** Initiates an HTTP post request.
		 * This function will return immediately, but the transfer will take more time. 
		 * You can install a CURLConnectionListener, and get progress/completion events to
		 * monitor progress. Once the transfer is done, call getReceivedBuffer() to get the
		 * result.
		 * 
		 * @param url the URL to get
		 * @param data the data to post
		 * @param mimeType the data's MIME type (e.g., "application/json"). Leave blank if 
		 * you don't want to specify this.
		 *
		 * @return bool true if the transfer started, and false if not (e.g., an existing
		 * transfer is busy)
		 */
		bool performHTTPPost(const std::string& url, const std::string& data, const std::string& mimeType = "");

		/** Initiates an HTTP post request.
		 * This function will return immediately, but the transfer will take more time. 
		 * You can install a CURLConnectionListener, and get progress/completion events to
		 * monitor progress. Once the transfer is done, call getReceivedBuffer() to get the
		 * result.
		 * 
		 * @param url the URL to get
		 * @param dataBuffer the data to post
		 * @param dataSize the size of the data buffer in bytes
		 * @param mimeType the data's MIME type (e.g., "application/json"). Leave blank if 
		 * you don't want to specify this.
		 * @param inPlaceBuffer set to true to use the data buffer directly. When set to false, the buffer 
		 * will be copied to an internal buffer. IMPORTANT: When set to true, you should wait until the transfer
		 * is complete before modifying or reusing the buffer
		 *
		 * @return bool true if the transfer started, and false if not (e.g., an existing
		 * transfer is busy)
		 */
		bool performHTTPPost(const std::string& url, void* dataBuffer, uint64 dataSize, 
			const std::string& mimeType = "", bool inPlaceBuffer = false);
		
		/** Initiates an HTTP delete request.
		 * This function will return immediately, but the transfer will take more time. 
		 * You can install a CURLConnectionListener, and get progress/completion events to
		 * monitor progress. Once the transfer is done, call getReceivedBuffer() to get the
		 * result.
		 * 
		 * @param url the URL to get
		 *
		 * @return bool true if the transfer started, and false if not (e.g., an existing
		 * transfer is busy)
		 */
		bool performHTTPDelete(const std::string& url);

		virtual std::string getErrorMessage() const;
		
		/** Generates an HTTP user-agent string, including basic OS details.
		 * 
		 * @param appName the application's name (NOTE: no spaces)
		 * @param appVersion the application's version string
		 *
		 * @return std::string the user-agent string
		 */
		static std::string generateUserAgent(const std::string &appName, const std::string &appVersion);


	protected:
		/** Sets the defaults.
		 */
		void setDefaults();

		/** The user agent string.
		 */
		std::string userAgent;

		/** Local copy of the URL.
		 */
		std::string url;

		/** Pointer to the form post (if one exists).
		 */
		struct curl_httppost* formPost;

		/** Pointer to the last element in the form post.
		 */
		struct curl_httppost* formPostLastPtr;
	};
};

#endif
