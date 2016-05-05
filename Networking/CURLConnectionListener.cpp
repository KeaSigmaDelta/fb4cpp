// CURLConnectionListener.cpp
//
// See header file for details
//
// Copyright (C) 2013 by Kea Sigma Delta Limited, all rights reserved

#include "Networking/CURLConnectionListener.h"

// NOTE: This *must* be included after the other header files
#include "EventListenerImpl.h"

using namespace Networking;

CURLConnectionListener::CURLConnectionListener()
{
	// Nothing to do here
}

CURLConnectionListener::~CURLConnectionListener()
{
	// Nothing to do here
}

bool CURLConnectionListener::networkTransferProgress(CURLConnection* source, uint64 downloadTotal, uint64 downloadNow, uint64 uploadTotal, uint64 uploadNow)
{
	return true;
}

void CURLConnectionListener::networkTransferCompleted(CURLConnection* source, CURLcode retCode)
{
	// Nothing to do here
}
