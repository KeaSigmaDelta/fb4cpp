#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#ifdef __WXMAC__
    #include <ApplicationServices/ApplicationServices.h>
#endif

#include "FLWxApp.h"

#if _MSC_VER && !__INTEL_COMPILER
#include <Windows.h>
int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	int argc = 0;
	char* argv[ 1 ] = { 0 };
#else
int main( int argc, char *argv[] )
{
#endif

#ifdef __WXMAC__
	ProcessSerialNumber PSN;
	MacGetCurrentProcess(&PSN);
	TransformProcessType(&PSN,kProcessTransformToForegroundApplication);
#endif
    
	wxApp::SetInstance( new FLWxApp() );
	wxEntryStart( argc, argv );
	wxTheApp->OnInit();
    wxTheApp->OnRun();
    wxEntryCleanup();
}
