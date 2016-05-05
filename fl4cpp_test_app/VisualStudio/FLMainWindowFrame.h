#ifndef __FL_MAIN_WINDOW_FRAME__
#define __FL_MAIN_WINDOW_FRAME__

#include "wx/wxprec.h"
#include "wx/webview.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "FL4CPP/FL4CPP.h"

class FLMainWindowFrame : public wxFrame, public fl4cpp::SessionObserver
{
public:
    FLMainWindowFrame( const wxString& title, const wxPoint& pos, const wxSize& size );
	virtual ~FLMainWindowFrame();

private:
    void onExit( wxCommandEvent& event );
    void onAbout( wxCommandEvent& event );
    
    void onLoginButton( wxCommandEvent& event );
    void onGetUserButton( wxCommandEvent& event );
    void onGetAccountsButton( wxCommandEvent& event );
    void onGetStatusesButton( wxCommandEvent& event );
    void onPostStatusButton( wxCommandEvent& event );
    void onGetAlbumsButton( wxCommandEvent& event );
    void onCreateAlbum( wxCommandEvent& event );
    void onPostPhotoUrl( wxCommandEvent& event );
    void onPostPhotoData( wxCommandEvent& event );
    void onOpenFileButton( wxCommandEvent& event );
    void onLogoutButton( wxCommandEvent& event );
    void onGetPhotosButton( wxCommandEvent& event );
    void onGetPictureButton( wxCommandEvent& event );
    void onCreateAlbumOnPageButton( wxCommandEvent& event );
    
    void onWebViewEvent( wxWebViewEvent& event );
    
private:
    virtual void onAccessTokenExpired();
    
private:
	std::string _access_token;
	unsigned int _expires_in;

    fl4cpp::Session* _flSession;
};

namespace FLMainWindowIDs
{
    enum IDs
    {
        // crap controls
        ID_HELLO_BUTTON = 1,
        ID_ABOUT_MAC,
        
        // facebook controls
        ID_LOGIN_BUTTON,
        ID_ACCESS_TOKEN_TEXT,
        
        ID_GET_USER_BUTTON,
        ID_GET_ACCOUNTS_BUTTON,
        ID_GET_STATUSES_BUTTON,
        ID_POST_STATUS_BUTTON,
        ID_GET_ALBUMS_BUTTON,
        ID_CREATE_ALBUM_BUTTON,
        ID_POST_PHOTO_URL_BUTTON,
        ID_POST_PHOTO_DATA_BUTTON,
        ID_OPEN_FILE_BUTTON,
        ID_LOGOUT_BUTTON,
        ID_GET_PHOTOS_BUTTON,
        ID_GET_PICTURE_BUTTON,
        ID_CREATE_ALBUM_ON_PAGE_BUTTON,
        
        ID_APPID_TEXT,
        ID_PERMISSIONS_TEXT,
        ID_USER_ID_TEXT,
        ID_PAGES_IDS_TEXT,
        ID_STATUSES_IDS_TEXT,
        ID_STATUS_TEXT,
        ID_ALBUMS_IDS_TEXT,
        ID_ALBUM_NAME_TEXT,
        ID_ALBUM_ID_TEXT,
        ID_IMAGE_URL_TEXT,
        ID_IMAGE_FILE_TEXT,
        ID_PHOTOS_IDS_TEXT,
        ID_PICTURE_URL_TEXT,
        ID_PAGE_ID_TEXT,
        ID_ALBUM_NAME_FOR_PAGE_TEXT,
        
        // web view for login
        ID_WEBVIEW,
        ID_LOGIN_REQUEST_COMPLETE_EVENT,
        ID_GET_USER_INFO_REQUEST_COMPLETE_EVENT,
    };
}

#endif // __FL_MAIN_WINDOW_FRAME__
