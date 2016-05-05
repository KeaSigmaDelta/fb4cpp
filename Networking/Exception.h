// Exception.h

#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <string>


typedef enum ExceptionType_e
{
	ET_OUTOFMEMORY,
	ET_NULLPOINTERERROR,
	ET_INVALIDINPUTPARAMETERS,
	ET_UNKNOWNERROR
}ExceptionType;

/** Base class for all exceptions.
 * Generally exceptions should be kept for serious errors only.
 *
 * @author \Hans de Ruiter
 * 
 * @copyright 2009 by Hans de Ruiter, all rights reserved (See license.txt for 
 * licensing information)
 *
 * @version 0.1 -- 2009/03/24 - Initial version
 */
class Exception
{
public:
	/** Creates an Exception object with a specific error message.
	 * The exception type is set to ET_UNKNOWNERROR.
	 *
	 * @param message a message explaining the error
	 * @param debugInfo pointer to an optional debug info message
	 */
	Exception(const char message[], const char *debugInfo = NULL);

	/** Creates an Exception object with a specific error message.
	 * The exception type is set to ET_UNKNOWNERROR.
	 */
	Exception(const std::string& message, const std::string& debugInfo = std::string());

	/** Constructs a new Exception object with a standard error.
	 *
	 * @param exceptionType the exception type
	 * @param debugInfo pointer to an optional debug info message
	 */
	Exception(ExceptionType exceptionType, const char *debugInfo = NULL);

	/** Constructs a new Exception object with a standard error.
	 *
	 * @param exceptionType the exception type
	 * @param debugInfo a debug info message
	 */
	Exception(ExceptionType exceptionType, const std::string& debugInfo);

	virtual ~Exception();

	/** Returns the message
	 */
	virtual const std::string& getMessage() const;

	/** Returns true if there is additional debug information
	 */
	virtual bool haveDebugInfo() const;

	/** Returns the debug information string.
	 */
	virtual const std::string& getDebugInfo() const;

	/** Returns the exception type.
	 */
	ExceptionType getExceptionType() const;

protected:

	/** Converts an ExceptionType to a string that describes the
	 * error.
	 */
	static const char* etToMessage(ExceptionType exceptionType);

	/** The error message.
	 */
	std::string message;

	/** Optional additional debug information
	 */
	std::string debugInfo;

	ExceptionType exceptionType;
};

#endif

