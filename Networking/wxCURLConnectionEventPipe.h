// wxCURLConnectionEventPipe.h

#ifndef __WXCURLCONNECTIONEVENTPIPE_H__
#define __WXCURLCONNECTIONEVENTPIPE_H__

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
#include "Networking/CURLConnectionListener.h"

namespace Networking
{
	class wxCURLConnectionEvent;

	/** Pipes events from a CURLConnection to wxWidgets in a thread-safe manner.
	 * Transforming to wxWidgets events makes it easier to update the GUI than using a 
	 * CURLConnectionListener directly (which is usually called in a separate thread).
	 *
	 * To use:
	 * - Create a wxCURLConnectionEventPipe that connects a CURLConnectionListener to a
	 * wxWidgets GUI object (e.g., a window needing updates)
	 * - Call Bind()/Connect() to connect your event handler method
	 *
	 * @author Hans de Ruiter
	 * 
	 * @copyright 2013 by Kea Sigma Delta Limited, all rights reserved (See license.txt for 
	 * licensing information)
	 */
	class wxCURLConnectionEventPipe : public CURLConnectionListener
	{
	public:
		/** Creates a new wxCURLConnectionEventPipe connecting a source CURLConnection to a target wxEvtHandler.
		 *
		 * @param source the source connection
		 * @param target the target wxWidgets event handler
		 * @param id the wxWidgets ID to use
		 */
		wxCURLConnectionEventPipe(CURLConnection *source, wxEvtHandler *target, int id = 0);

		virtual ~wxCURLConnectionEventPipe();

		virtual bool networkTransferProgress(Networking::CURLConnection* source, uint64 downloadTotal, uint64 downloadNow, uint64 uploadTotal, uint64 uploadNow);

		virtual void networkTransferCompleted(Networking::CURLConnection* source, CURLcode retCode);

	private:
		/** The target event handler.
		 */
		wxEvtHandler *target;

		/** The wxWidgets ID to use.
		 */
		int id;
	};
};

#endif
