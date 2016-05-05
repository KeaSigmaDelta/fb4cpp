// wxCURLConnectionEvent.h

#ifndef __WXCURLCONNECTIONEVENT_H__
#define __WXCURLCONNECTIONEVENT_H__

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <boost/shared_ptr.hpp>
#include <vector>

#include "Networking/CURLConnection.h"

#define wxCURLConnectionEventHandler(func) wxEVENT_HANDLER_CAST(Networking::wxCURLConnectionEventFunction, func)

namespace Networking
{
	class wxCURLConnectionEvent;
	class wxCURLConnectionEventPipe;
	wxDECLARE_EVENT(wxEVT_CURL_CONNECTION_PROGRESS, Networking::wxCURLConnectionEvent);
	wxDECLARE_EVENT(wxEVT_CURL_CONNECTION_COMPLETE, Networking::wxCURLConnectionEvent);

	typedef void (wxEvtHandler::*wxCURLConnectionEventFunction)(Networking::wxCURLConnectionEvent &);

	/** A wxWidgets event class for CURLConnection progress and complete events.
	 * This class provides a thread-safe means to transfer CURLConnection events to wxWidgets GUI code. 
	 * To use this class, see wxCURLConnectionEventPipe.
	 *
	 * There are two event types:
	 * - wxEVT_CURL_CONNECTION_PROGRESS - a progress update; use getProgress() to obtain the progress data
	 * - wxEVT_CURL_CONNECTION_COMPLETE - the operation is complete; use getRetCode() to check if it completed
	 * successfully, or if there was an error
	 *
	 * @author Hans de Ruiter
	 * 
	 * @copyright 2013 by Kea Sigma Delta Limited, all rights reserved (See license.txt for 
	 * licensing information)
	 */
	class wxCURLConnectionEvent: public wxEvent
	{
	public:
		wxCURLConnectionEvent(wxEventType eventType = wxEVT_CURL_CONNECTION_PROGRESS, int id = 0);
 
		// You *must* copy here the data to be transported
		wxCURLConnectionEvent(const wxCURLConnectionEvent& event);
 
		// Required for sending with wxPostEvent()
		wxEvent* Clone() const;

		/** Set the source CURLConnection.
		 */
		void setSource(Networking::CURLConnection *source);

		/** Gets the source CURLConnection.
		 */
		Networking::CURLConnection* getSource() const;

		/** Sets the current transfer progress.
		 *
		 * NOTE: The total transfer may not be known initially, so you should be able to handle this case.
		 *
		 * @param downloadTotal the total number of bytes to be downloaded 
		 * @param downloadNow the total number of bytes that has already been downloaded
		 * @param uploadTotal the total number of bytes to be uploaded 
		 * @param uploadNow the total number of bytes that has already been uploaded
		 */
		void setProgress(uint64 downloadTotal, uint64 downloadNow, uint64 uploadTotal, uint64 uploadNow);

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
		 * @param uploadNow the total number of bytes that has already been uploaded will be written to 
		 * this parameter (set to NULL to ignore)
		 */
		void getProgress(uint64* downloadTotal, uint64* downloadNow, uint64* uploadTotal, uint64* uploadNow) const;

		/** Sets the return code.
		 */
		void setRetCode(CURLcode retCode);

		/** Gets the return code
		 */
		CURLcode getRetCode() const;

	private:
		/** The source connection.
		 */
		Networking::CURLConnection* source;

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

		/** The return code.
		 *
		 * NOTE: Only valid if the event type is wxEVT_CURL_CONNECTION_COMPLETE.
		 */
		CURLcode retCode;

		/** Vector of event pipes that transfer events from CURLConnection to wxWidgets.
		 */
		static std::vector< boost::shared_ptr<wxCURLConnectionEventPipe> > eventPipes;
	};
};

#endif
