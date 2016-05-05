// wxCURLConnectionEvent.cpp
//
// See header file for details
//
// Copyright (C) 2013 by Kea Sigma Delta Limited, all rights reserved

#include "Networking/wxCURLConnectionEvent.h"

using namespace Networking;

wxDEFINE_EVENT(Networking::wxEVT_CURL_CONNECTION_PROGRESS, Networking::wxCURLConnectionEvent);
wxDEFINE_EVENT(Networking::wxEVT_CURL_CONNECTION_COMPLETE, Networking::wxCURLConnectionEvent);

wxCURLConnectionEvent::wxCURLConnectionEvent(wxEventType eventType, int id)
        		:  wxEvent(id, eventType) 
{ 
	// Nothing to do here
}
 
wxCURLConnectionEvent::wxCURLConnectionEvent(const wxCURLConnectionEvent& event)
        			:  wxEvent(event) 
{ 
	this->source = event.source;
	this->downloadTotal = event.downloadTotal;
	this->downloadNow = event.downloadNow;
	this->uploadTotal = event.uploadTotal;
	this->uploadNow = event.uploadNow;
	this->retCode = event.retCode;
}
 
wxEvent* wxCURLConnectionEvent::Clone() const 
{ 
	return new wxCURLConnectionEvent(*this); 
}

void wxCURLConnectionEvent::setSource(Networking::CURLConnection *source)
{
	this->source = source;
}

Networking::CURLConnection* wxCURLConnectionEvent::getSource() const
{
	return source;
}

void wxCURLConnectionEvent::setProgress(uint64 downloadTotal, uint64 downloadNow, uint64 uploadTotal, uint64 uploadNow)
{
	this->downloadTotal = downloadTotal;
	this->downloadNow = downloadNow;
	this->uploadTotal = uploadTotal;
	this->uploadNow = uploadNow;
}

void wxCURLConnectionEvent::getProgress(uint64* downloadTotal, uint64* downloadNow, uint64* uploadTotal, uint64* uploadNow) const
{
	if(downloadTotal) { *downloadTotal = this->downloadTotal; }
	if(downloadNow) { *downloadNow = this->downloadNow; }
	if(uploadTotal) { *uploadTotal = this->uploadTotal; }
	if(uploadNow) { *uploadNow = this->uploadNow; }
}

void wxCURLConnectionEvent::setRetCode(CURLcode retCode)
{
	this->retCode = retCode;
}

CURLcode wxCURLConnectionEvent::getRetCode() const
{
	return retCode;
}

std::vector< boost::shared_ptr<wxCURLConnectionEventPipe> > wxCURLConnectionEvent::eventPipes;
