// Exception.cpp
//
// See header file for details
//
// Copyright (C) 2009 by Hans de Ruiter, all rights reserved

#include "Exception.h"

#ifndef _
// ##### FIXME! ##### We actually do want these strings to be translated
#define _(x) x
#endif

Exception::Exception(const char message[], const char *debugInfo) : 
		message(message), debugInfo((debugInfo ? debugInfo : "")), exceptionType(ET_UNKNOWNERROR)
{
}

Exception::Exception(const std::string& message, const std::string& debugInfo) :
		message(message), debugInfo(debugInfo), exceptionType(ET_UNKNOWNERROR)
{
}

Exception::Exception(ExceptionType exceptionType, const char *debugInfo) :
		message(etToMessage(exceptionType)), debugInfo((debugInfo ? debugInfo : "")), exceptionType(exceptionType)
{
}

Exception::Exception(ExceptionType exceptionType, const std::string& debugInfo) :
		message(etToMessage(exceptionType)), debugInfo(debugInfo), exceptionType(exceptionType)
{
}

Exception::~Exception()
{
}

const std::string& Exception::getMessage() const
{
	return message;
}

bool Exception::haveDebugInfo() const 
{
	if(debugInfo.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const std::string& Exception::getDebugInfo() const
{
	if(!haveDebugInfo())
	{
		// Just repeat the message
		return message;
	}
	else
	{
		return debugInfo;
	}
}

ExceptionType Exception::getExceptionType() const
{
	return exceptionType;
}

const char* Exception::etToMessage(ExceptionType exceptionType)
{
	switch(exceptionType)
	{
	case ET_OUTOFMEMORY:
		return _("Out of memory");
	case ET_NULLPOINTERERROR:
		return _("NULL pointer exception. Please send a bug report to this application's developer.");
	case ET_INVALIDINPUTPARAMETERS:
		return _(" A function was called with invalid input parameters. "
				 "Please send a bug report to this application's developer.");
	case ET_UNKNOWNERROR:
	default:
		return _("An unknown error occurred");
	};
}

