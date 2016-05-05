// wxCURLConnectionEventPipe.cpp
//
// See header file for details
//
// Copyright (C) 2013 by Kea Sigma Delta Limited, all rights reserved

#include "Networking/wxCURLConnectionEvent.h"
#include "Networking/wxCURLConnectionEventPipe.h"

using namespace Networking;

wxCURLConnectionEventPipe::wxCURLConnectionEventPipe(CURLConnection *source, wxEvtHandler *target, int id)
{
	this->target = target;
	this->id = id;

	source->addListener(this);
}

wxCURLConnectionEventPipe::~wxCURLConnectionEventPipe()
{
	// Nothing to do here
}

bool wxCURLConnectionEventPipe::networkTransferProgress(
	Networking::CURLConnection* source, uint64 downloadTotal, uint64 downloadNow, uint64 uploadTotal, uint64 uploadNow)
{
	wxCURLConnectionEvent *event = new wxCURLConnectionEvent(wxEVT_CURL_CONNECTION_PROGRESS, id);
	event->setSource(source);
	event->setProgress(downloadTotal, downloadNow, uploadTotal, uploadNow);

	wxQueueEvent(target, event);

	return true; // The GUI code doesn't get to cancel the transfer, so always return true
}

void wxCURLConnectionEventPipe::networkTransferCompleted(Networking::CURLConnection* source, CURLcode retCode)
{
	wxCURLConnectionEvent *event = new wxCURLConnectionEvent(wxEVT_CURL_CONNECTION_COMPLETE, id);
	event->setSource(source);
	event->setRetCode(retCode);

	wxQueueEvent(target, event);
}
