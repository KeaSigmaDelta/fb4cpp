// HTTPRequest.cpp
//
// See header file for details
//
// Copyright (C) 2013 by Kea Sigma Delta Limited, all rights reserved

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "Exception.h"
#include "Networking/HTTPRequest.h"

#include <sstream>
#include <wx/platinfo.h>

using namespace Networking;
using namespace std;

HTTPRequest::HTTPRequest()
{
	formPost = NULL;
	formPostLastPtr = NULL;

	setDefaults();
}

HTTPRequest::~HTTPRequest()
{
	// Deallocate any form that might exist
	resetForm();
}

void HTTPRequest::setUserAgent(const std::string& userAgent)
{
	// NOTE: LibCURL doesn't create its own copy, so we store it ourselves
	this->userAgent = userAgent;
	setOption(CURLOPT_USERAGENT, (char*)userAgent.c_str());
}

void HTTPRequest::setDefaults()
{
	// Enable handling redirects
	setOption(CURLOPT_FOLLOWLOCATION, (long)1);

	// ##### FIXME! ##### What else?
}

bool HTTPRequest::performHTTPGet(const std::string& url)
{
	if(isBusy())
	{
		return false;
	}

	setOption(CURLOPT_HTTPHEADER, (void*)NULL);
	setOption(CURLOPT_CUSTOMREQUEST, (void*)NULL);
	setOption(CURLOPT_HTTPGET, (long)1);
	this->url = url;
	setOption(CURLOPT_URL, (char*)this->url.c_str());

	return perform();
}

bool HTTPRequest::performHTTPHead(const std::string& url)
{
	if(isBusy())
	{
		return false;
	}

	setOption(CURLOPT_HTTPHEADER, (void*)NULL);
	setOption(CURLOPT_CUSTOMREQUEST, (void*)NULL);
	setOption(CURLOPT_HTTPGET, (long)1);
	setOption(CURLOPT_NOBODY, (long)1);
	this->url = url;
	setOption(CURLOPT_URL, (char*)this->url.c_str());

	return perform();
}

bool HTTPRequest::performHTTPPut(const std::string& url, const std::string& data, const std::string& mimeType)
{
	if(isBusy())
	{
		return false;
	}

	// Set to PUT operation
	setOption(CURLOPT_CUSTOMREQUEST, (void*)NULL);
	setOption(CURLOPT_UPLOAD, (long)1);
	this->url = url;
	setOption(CURLOPT_URL, (char*)this->url.c_str());

	// Copy the data to the send buffer
	setSendBuffer(data, mimeType);

	// Perform
	return perform();
}

bool HTTPRequest::performHTTPPut(const std::string& url, void* dataBuffer, uint64 dataSize, 
			const std::string& mimeType, bool inPlaceBuffer)
{
	if(isBusy())
	{
		return false;
	}

	// Set to PUT operation
	setOption(CURLOPT_CUSTOMREQUEST, (void*)NULL);
	setOption(CURLOPT_UPLOAD, (long)1);
	this->url = url;
	setOption(CURLOPT_URL, (char*)this->url.c_str());

	// Copy the data to the send buffer
	setSendBuffer(dataBuffer, dataSize, mimeType, inPlaceBuffer);

	// Perform
	return perform();
}

bool HTTPRequest::resetForm()
{
	if(isBusy())
	{
		return false;
	}

	if(formPost)
	{
		curl_formfree(formPost);
		formPost = NULL;
		formPostLastPtr = NULL;
	}

	return true;
}

bool HTTPRequest::addFormField(const std::string& name, const std::string& contents, CURLFORMcode* errCode)
{
	if(isBusy())
	{
		if(errCode)
		{
			// Error isn't related to the form add operation, so set the error code to "okay"
			*errCode = CURL_FORMADD_OK;
		}
		return false;
	}

	CURLFORMcode retVal = curl_formadd(&formPost, &formPostLastPtr,
               CURLFORM_COPYNAME, name.c_str(),
               CURLFORM_COPYCONTENTS, contents.c_str(),
               CURLFORM_END);

	if(errCode)
	{
		*errCode = retVal;
	}

	if(retVal == CURL_FORMADD_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool HTTPRequest::addFormFileField(const std::string& name, const std::string& filename, const std::string& fileUploadName, 
			const std::string& mimeType, CURLFORMcode* errCode)
{
	if(isBusy())
	{
		if(errCode)
		{
			// Error isn't related to the form add operation, so set the error code to "okay"
			*errCode = CURL_FORMADD_OK;
		}
		return false;
	}

	CURLFORMcode retVal;

	if(mimeType.size() < 1)
	{
		retVal = curl_formadd(&formPost, &formPostLastPtr,
				   CURLFORM_COPYNAME, name.c_str(),
				   CURLFORM_FILE, filename.c_str(),
				   (fileUploadName.size() > 0 ? CURLFORM_FILENAME : CURLFORM_END), fileUploadName.c_str(),
				   CURLFORM_END);
	}
	else
	{
		retVal = curl_formadd(&formPost, &formPostLastPtr,
				   CURLFORM_COPYNAME, name.c_str(),
				   CURLFORM_FILE, filename.c_str(),
				   (fileUploadName.size() > 0 ? CURLFORM_FILENAME : CURLFORM_END), fileUploadName.c_str(),
				   CURLFORM_CONTENTTYPE, mimeType.c_str(),
				   CURLFORM_END);
	}

	if(errCode)
	{
		*errCode = retVal;
	}

	if(retVal == CURL_FORMADD_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool HTTPRequest::addFormFileField(const std::string& name, const void* buffer, uint64 bufferSize, 
			const std::string& fileUploadName, const std::string& mimeType, CURLFORMcode* errCode)
{
	if(isBusy())
	{
		if(errCode)
		{
			// Error isn't related to the form add operation, so set the error code to "okay"
			*errCode = CURL_FORMADD_OK;
		}
		return false;
	}

	CURLFORMcode retVal;
	
	if(mimeType.size() < 1)
	{
		retVal = curl_formadd(&formPost, &formPostLastPtr,
				   CURLFORM_COPYNAME, name.c_str(),
				   CURLFORM_BUFFER, fileUploadName.c_str(),
				   CURLFORM_BUFFERPTR, buffer,
				   CURLFORM_BUFFERLENGTH, (long)bufferSize,
				   CURLFORM_END);
	}
	else
	{
		retVal = curl_formadd(&formPost, &formPostLastPtr,
				   CURLFORM_COPYNAME, name.c_str(),
				   CURLFORM_BUFFER, fileUploadName.c_str(),
				   CURLFORM_BUFFERPTR, buffer,
				   CURLFORM_BUFFERLENGTH, (long)bufferSize,
				   CURLFORM_CONTENTTYPE, mimeType.c_str(),
				   CURLFORM_END);
	}

	if(errCode)
	{
		*errCode = retVal;
	}

	if(retVal == CURL_FORMADD_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool HTTPRequest::performHTTPPostForm(const std::string& url)
{
	if(isBusy())
	{
		return false;
	}

	if(!formPost)
	{
		// Uh, there's nothing to post!
		return false;
	}

	// Set up the POST operation
	setOption(CURLOPT_CUSTOMREQUEST, (void*)NULL);
	setOption(CURLOPT_HTTPPOST, formPost);
	this->url = url;
	setOption(CURLOPT_URL, (char*)this->url.c_str());

	// Perform
	return perform();
}

bool HTTPRequest::performHTTPPost(const std::string& url, const std::string& data, const std::string& mimeType)
{
	if(isBusy())
	{
		return false;
	}

	// Set to PUT operation
	setOption(CURLOPT_CUSTOMREQUEST, (void*)NULL);
	setOption(CURLOPT_POST, (long)1);
	setOption(CURLOPT_POSTFIELDS, (void*)NULL); // We're using the send buffer for the POST data
	this->url = url;
	setOption(CURLOPT_URL, (char*)this->url.c_str());

	// Copy the data to the send buffer
	setSendBuffer(data, mimeType);

	// Perform
	return perform();
}

bool HTTPRequest::performHTTPPost(const std::string& url, void* dataBuffer, uint64 dataSize, 
			const std::string& mimeType, bool inPlaceBuffer)
{
	if(isBusy())
	{
		return false;
	}

	// Set to PUT operation
	setOption(CURLOPT_CUSTOMREQUEST, (void*)NULL);
	setOption(CURLOPT_POST, (long)1);
	setOption(CURLOPT_POSTFIELDS, (void*)NULL); // We're using the send buffer for the POST data
	this->url = url;
	setOption(CURLOPT_URL, (char*)this->url.c_str());

	// Copy the data to the send buffer
	setSendBuffer(dataBuffer, dataSize, mimeType, inPlaceBuffer);

	// Perform
	return perform();
}

bool HTTPRequest::performHTTPDelete(const std::string& url)
{
	if(isBusy())
	{
		return false;
	}

	setOption(CURLOPT_HTTPHEADER, (void*)NULL);
	setOption(CURLOPT_CUSTOMREQUEST, "DELETE");
	setOption(CURLOPT_HTTPGET, (long)1);
	this->url = url;
	setOption(CURLOPT_URL, (char*)this->url.c_str());

	// Perform
	return perform();
}

std::string HTTPRequest::getErrorMessage() const
{
	if(getLastRetCode() == CURLE_HTTP_RETURNED_ERROR)
	{
		// Deliver an error message that's appropriate
		switch(protocolResponseCode)
		{
		case HTTP_CODE_ERROR_BADREQUEST:
			return _("Server returned 400 Error: Bad Request.").ToStdString();
		case HTTP_CODE_ERROR_UNAUTHORISED:
			return _("Server returned 401 Error: Unauthorised.").ToStdString();
		case HTTP_CODE_ERROR_NOTFOUND:
			return _("Server returned 404 Error: Resource Not Found.").ToStdString();
		case HTTP_CODE_ERROR_TIMEOUT:
			return _("Server returned 408 Error: Bad Request.").ToStdString();
		case HTTP_CODE_ERROR_INTERNALSERVERERROR:
			return _("Server returned 500 Error: Internal Server Error.").ToStdString();
		default:
			ostringstream errorMessage;
			errorMessage << _("Server returned ").ToStdString() << protocolResponseCode << ".";
			return errorMessage.str();
		}
	}

	return CURLConnection::getErrorMessage();
}

std::string HTTPRequest::generateUserAgent(const std::string &appName, const std::string &appVersion)
{
	ostringstream osStringStream;
	const wxPlatformInfo& platformInfo = wxPlatformInfo::Get();
	wxOperatingSystemId osID = platformInfo.GetOperatingSystemId();
	if(osID & wxOS_MAC_OS) { osStringStream << "Macintosh; Mac OS"; }
	else if(osID & wxOS_MAC_OSX_DARWIN) { osStringStream << "Macintosh; Mac OS X"; }
	else if(osID & wxOS_WINDOWS_9X) { osStringStream << "Windows"; }
 	else if(osID & wxOS_WINDOWS_NT) { osStringStream << "Windows NT"; }
	else if(osID & wxOS_WINDOWS_MICRO) { osStringStream << "Windows"; }
	else if(osID & wxOS_WINDOWS_CE) { osStringStream << "Windows CE"; }
	else if(osID & wxOS_UNIX_LINUX) { osStringStream << "Linux"; }
	else if(osID & wxOS_UNIX_FREEBSD) { osStringStream << "FreeBSD"; }
	else if(osID & wxOS_UNIX_OPENBSD) { osStringStream << "OpenBSD"; }
	else if(osID & wxOS_UNIX_NETBSD) { osStringStream << "NetBSD"; }
	else if(osID & wxOS_UNIX_SOLARIS) { osStringStream << "Solaris"; }
	else if(osID & wxOS_UNIX_AIX) { osStringStream << "AIX"; }
	else if(osID & wxOS_UNIX_HPUX) { osStringStream << "HP/UX"; }
	else if(osID & wxOS_UNIX) { osStringStream << "UNIX"; }
	else if(osID & wxOS_DOS) { osStringStream << "DOS"; }
	else if(osID & wxOS_OS2) { osStringStream << "OS/2"; }
	else { 
#if defined(_DEBUG) || defined(DEBUG)
		throw Exception(ET_UNKNOWNERROR, "Unknown OS ID");
#endif
		osStringStream << "Unknown"; 
	}

	osStringStream << " " << platformInfo.GetOSMajorVersion() << "." << platformInfo.GetOSMinorVersion();

	string userAgent = appName + string("/") + appVersion + " (" + osStringStream.str() + ")";

	return userAgent;
}