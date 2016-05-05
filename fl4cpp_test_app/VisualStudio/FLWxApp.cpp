#include "FLWxApp.h"
#include "FLMainWindowFrame.h"

bool FLWxApp::OnInit()
{
 	FLMainWindowFrame *frame = new FLMainWindowFrame( "fl4cpp sample", wxPoint(150, 150), wxSize(-1, -1) );
    frame->Show( true );

    return true;
}