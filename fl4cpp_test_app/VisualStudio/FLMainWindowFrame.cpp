#include "FLMainWindowFrame.h"

#include <sstream>
#include <iostream>
#include <fstream>

#include "Networking/wxCURLConnectionEvent.h"
#include "Networking/wxCURLConnectionEventPipe.h"
#include "Networking/httpRequest.h"

#include "Uri-parser/UriParser.hpp"

#include "json/json.h"

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

using namespace Networking;

FLMainWindowFrame::FLMainWindowFrame( const wxString& title, const wxPoint& pos, const wxSize& size )
: wxFrame( NULL, wxID_ANY, title, pos, size ),
_access_token( "" ),
_expires_in( 0 )
{

    // crap controls
    wxMenu* menuFile = new wxMenu;
    menuFile->Append( wxID_EXIT, "Super exit", "Exit hint" );
    Connect( wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FLMainWindowFrame::onExit ) );
    
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append( FLMainWindowIDs::ID_ABOUT_MAC, "About", "About hint" );
    Connect( FLMainWindowIDs::ID_ABOUT_MAC, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( FLMainWindowFrame::onAbout ) );
    
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "File" );
    menuBar->Append( menuHelp, "Help" );
    
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText( "Status string" );
    
    // facebook controls
    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, 2, 0, 0);
    //gridSizer->SetFlexibleDirection( wxBOTH );
    //gridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    this->SetSizer(gridSizer);
    
    // auth box
    wxStaticBoxSizer* boxAuth = new wxStaticBoxSizer( new wxStaticBox(this, wxID_ANY, _("Auth")), wxVERTICAL );
    gridSizer->Add( boxAuth, 1, wxALL|wxEXPAND, 5 );
    
    // app id box
    wxBoxSizer* boxAppId = new wxBoxSizer(wxHORIZONTAL);
    boxAuth->Add( boxAppId, 1, wxALL|wxEXPAND, 5 );
    
    // permissions
    wxStaticText* appIdText = new wxStaticText( this, -1, wxT( "AppID:" ), wxDefaultPosition );
    boxAppId->Add( appIdText, 0, wxALL, 5 );
    wxTextCtrl* appIdTextControl = new wxTextCtrl( this,
                                                     FLMainWindowIDs::ID_APPID_TEXT,
                                                     "1513872972174341",
                                                     wxDefaultPosition,
                                                     wxSize( 600, 25 ) );
    boxAppId->Add( appIdTextControl, 0, wxALL, 5 );
    
    // permissions box
    wxBoxSizer* boxPermissions = new wxBoxSizer(wxHORIZONTAL);
    boxAuth->Add( boxPermissions, 1, wxALL|wxEXPAND, 5 );
    
    // permissions
    wxStaticText* permissionsText = new wxStaticText( this, -1, wxT( "permissions:" ), wxDefaultPosition );
    boxPermissions->Add( permissionsText, 0, wxALL, 5 );
    wxTextCtrl* permissionsTextCtrl = new wxTextCtrl( this,
                                                     FLMainWindowIDs::ID_PERMISSIONS_TEXT,
                                                     "publish_actions,manage_pages,read_stream,user_photos,read_stream,user_birthday",
                                                     wxDefaultPosition,
                                                     wxSize( 600, 25 ) );
    boxPermissions->Add( permissionsTextCtrl, 0, wxALL, 5 );
    
    // auth buttons
    wxBoxSizer* boxAuthActions = new wxBoxSizer(wxHORIZONTAL);
    boxAuth->Add( boxAuthActions, 1, wxALL|wxEXPAND, 5 );
    
    wxButton* loginButton = new wxButton( this, FLMainWindowIDs::ID_LOGIN_BUTTON, wxT( "Login" ), wxDefaultPosition );
    boxAuthActions->Add( loginButton, 0, wxALL, 5 );
    Connect( FLMainWindowIDs::ID_LOGIN_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FLMainWindowFrame::onLoginButton ) );
    
    wxButton* logoutButton = new wxButton( this, FLMainWindowIDs::ID_LOGOUT_BUTTON, wxT( "Logout" ), wxDefaultPosition );
    boxAuthActions->Add( logoutButton, 0, wxALL, 5 );
    Connect( FLMainWindowIDs::ID_LOGOUT_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FLMainWindowFrame::onLogoutButton ) );
    
    // access token
    wxBoxSizer* boxAccessToken = new wxBoxSizer(wxHORIZONTAL);
    boxAuth->Add( boxAccessToken, 1, wxALL|wxEXPAND, 5 );
    
    wxStaticText* accessTokenText = new wxStaticText( this, -1, wxT( "access_token:" ), wxDefaultPosition );
    wxStaticText* accessTokenValueText = new wxStaticText( this, FLMainWindowIDs::ID_ACCESS_TOKEN_TEXT, wxT( "0" ), wxDefaultPosition );
    boxAccessToken->Add( accessTokenText, 0, wxALL, 5 );
    boxAccessToken->Add( accessTokenValueText, 0, wxALL, 5 );
    
    
    
    
    // user info box
    wxStaticBoxSizer* userInfoBox = new wxStaticBoxSizer( new wxStaticBox(this, wxID_ANY, _("User")), wxVERTICAL );
    gridSizer->Add( userInfoBox, 1, wxALL|wxEXPAND, 5 );

    wxButton* getUserButton = new wxButton( this, FLMainWindowIDs::ID_GET_USER_BUTTON, wxT( "Get User /me" ), wxDefaultPosition );
    userInfoBox->Add( getUserButton );
    Connect( FLMainWindowIDs::ID_GET_USER_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FLMainWindowFrame::onGetUserButton ) );
    
    // get user id box
    wxBoxSizer* userIdBox = new wxBoxSizer(wxHORIZONTAL);
    userInfoBox->Add( userIdBox, 1, wxALL|wxEXPAND, 5 );
    
    wxStaticText* userIdText = new wxStaticText( this, -1, wxT( "user Id:" ), wxDefaultPosition );
    wxStaticText* userIdValueText = new wxStaticText( this, FLMainWindowIDs::ID_USER_ID_TEXT, wxT( "0" ), wxDefaultPosition, wxSize( 600, 25 ) );
    userIdBox->Add( userIdText );
    userIdBox->Add( userIdValueText );
    
    userInfoBox->Add( 0, 20 );
    
    wxButton* getPictureButton = new wxButton( this, FLMainWindowIDs::ID_GET_PICTURE_BUTTON, wxT( "Get Picture" ), wxDefaultPosition );
    userInfoBox->Add( getPictureButton );
    Connect( FLMainWindowIDs::ID_GET_PICTURE_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FLMainWindowFrame::onGetPictureButton ) );
    
    // get picture box
    wxBoxSizer* pictureUrlBox = new wxBoxSizer(wxHORIZONTAL);
    userInfoBox->Add( pictureUrlBox, 1, wxALL|wxEXPAND, 5 );
    
    wxStaticText* pictureUrlText = new wxStaticText( this, -1, wxT( "picture url:" ), wxDefaultPosition );
    wxTextCtrl* pictureUrlTextCtrl = new wxTextCtrl( this, FLMainWindowIDs::ID_PICTURE_URL_TEXT, "", wxDefaultPosition, wxSize( 600, 25 ) );
    pictureUrlBox->Add( pictureUrlText );
    pictureUrlBox->Add( pictureUrlTextCtrl );
    
    
    // web view for login
    wxStaticBoxSizer* boxWebView = new wxStaticBoxSizer( new wxStaticBox(this, wxID_ANY, _("WebView")), wxVERTICAL );
    gridSizer->Add( boxWebView, 1, wxALL|wxEXPAND , 5 );
    
    wxWebView* authWebView = wxWebView::New( this, FLMainWindowIDs::ID_WEBVIEW, wxT( "" ), wxDefaultPosition, wxSize(700, 600 ) );
    boxWebView->Add( authWebView );
    Connect( FLMainWindowIDs::ID_WEBVIEW, wxEVT_WEBVIEW_NAVIGATED, wxWebViewEventHandler( FLMainWindowFrame::onWebViewEvent ) );
    


    // actions
    wxStaticBoxSizer* actionsBox = new wxStaticBoxSizer( new wxStaticBox(this, wxID_ANY, _("Actions")), wxVERTICAL );
    gridSizer->Add( actionsBox, 0, wxALL|wxEXPAND, 5 );
    
    // accounts info box
    wxStaticBoxSizer* accountsInfoBox = new wxStaticBoxSizer(  new wxStaticBox(this, wxID_ANY, _("Accounts (pages)")), wxVERTICAL );
    actionsBox->Add( accountsInfoBox, 0,  wxALL|wxEXPAND, 5 );
    
    wxButton* getAccountsButton = new wxButton( this, FLMainWindowIDs::ID_GET_ACCOUNTS_BUTTON, wxT( "Get Accounts" ), wxDefaultPosition );
    accountsInfoBox->Add( getAccountsButton );
    Connect( FLMainWindowIDs::ID_GET_ACCOUNTS_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FLMainWindowFrame::onGetAccountsButton ) );
   
    // pages ids box
    wxBoxSizer* pagesIdBox = new wxBoxSizer(wxHORIZONTAL);
    accountsInfoBox->Add( pagesIdBox, 1, wxSHAPED, 5 );
    
    wxStaticText* pageIdsText = new wxStaticText( this, -1, wxT( "pages Ids:" ), wxDefaultPosition );
    wxTextCtrl* pageIdsValueText = new wxTextCtrl( this, FLMainWindowIDs::ID_PAGES_IDS_TEXT, wxT( "0" ), wxDefaultPosition, wxSize( 600, 25 ) );
    pagesIdBox->Add( pageIdsText );
    pagesIdBox->Add( pageIdsValueText );

    
    accountsInfoBox->Add( 0, 20 );
    
    // page id box
    wxBoxSizer* pageIdBox = new wxBoxSizer(wxHORIZONTAL);
    accountsInfoBox->Add( pageIdBox, 1, wxSHAPED, 5 );
    
    wxStaticText* pageIdText = new wxStaticText( this, -1, wxT( "page Id:" ), wxDefaultPosition );
    wxTextCtrl* pageIdTextCtrl = new wxTextCtrl( this, FLMainWindowIDs::ID_PAGE_ID_TEXT, wxT( "" ), wxDefaultPosition, wxSize( 300, 25 ) );
    pageIdBox->Add( pageIdText );
    pageIdBox->Add( pageIdTextCtrl );
    
    // album name box
    wxBoxSizer* albumNameForPageBox = new wxBoxSizer(wxHORIZONTAL);
    accountsInfoBox->Add( albumNameForPageBox, 1, wxSHAPED, 5 );
    
    wxStaticText* albumNameForPageText = new wxStaticText( this, -1, wxT( "album name:" ), wxDefaultPosition );
    wxTextCtrl* albumNameForPageTextCtrl = new wxTextCtrl( this, FLMainWindowIDs::ID_ALBUM_NAME_FOR_PAGE_TEXT, wxT( "test album" ), wxDefaultPosition, wxSize( 300, 25 ) );
    albumNameForPageBox->Add( albumNameForPageText );
    albumNameForPageBox->Add( albumNameForPageTextCtrl );
    
    // create album button
    wxButton* createAlbumOnPageButton = new wxButton( this, FLMainWindowIDs::ID_CREATE_ALBUM_ON_PAGE_BUTTON, wxT( "Create album" ), wxDefaultPosition );
    accountsInfoBox->Add( createAlbumOnPageButton );
    Connect( FLMainWindowIDs::ID_CREATE_ALBUM_ON_PAGE_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FLMainWindowFrame::onCreateAlbumOnPageButton ) );
    
    
    
    // statuses info
    wxStaticBoxSizer* statusesBox = new wxStaticBoxSizer( new wxStaticBox(this, wxID_ANY, _("Statuses")), wxVERTICAL );
    actionsBox->Add( statusesBox, 0,  wxALL|wxEXPAND, 5 );
    
    wxButton* getStatusesButton = new wxButton( this, FLMainWindowIDs::ID_GET_STATUSES_BUTTON, wxT( "Get Statuses" ), wxDefaultPosition );
    statusesBox->Add( getStatusesButton );
    Connect( FLMainWindowIDs::ID_GET_STATUSES_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FLMainWindowFrame::onGetStatusesButton ) );
    
    wxBoxSizer* statusesIdsBox = new wxBoxSizer(wxHORIZONTAL);
    statusesBox->Add( statusesIdsBox, 1, wxSHAPED, 5 );
    
    wxStaticText* statusesIdsText = new wxStaticText( this, -1, wxT( "statuses Ids:" ), wxDefaultPosition );
    wxStaticText* statusesIdsValueText = new wxStaticText( this, FLMainWindowIDs::ID_STATUSES_IDS_TEXT, wxT( "0" ), wxDefaultPosition );
    statusesIdsBox->Add( statusesIdsText );
    statusesIdsBox->Add( statusesIdsValueText );
    
    statusesBox->Add( 0, 20 );
    
    // post status
    wxBoxSizer* postStatusBox = new wxBoxSizer(wxHORIZONTAL);
    statusesBox->Add( postStatusBox, 1, wxSHAPED, 5 );
    
    wxButton* postStatusButton = new wxButton( this, FLMainWindowIDs::ID_POST_STATUS_BUTTON, wxT( "Post Status" ), wxDefaultPosition );
    postStatusBox->Add( postStatusButton );
    Connect( FLMainWindowIDs::ID_POST_STATUS_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FLMainWindowFrame::onPostStatusButton ) );
    
    wxTextCtrl* statusTextCtrl = new wxTextCtrl( this, FLMainWindowIDs::ID_STATUS_TEXT, "status text", wxDefaultPosition, wxSize( 300, 25 ) );
    postStatusBox->Add( statusTextCtrl );
    
    
    
    // get albums info
    wxStaticBoxSizer* albumsBox = new wxStaticBoxSizer( new wxStaticBox(this, wxID_ANY, _("Albums")), wxVERTICAL );
    actionsBox->Add( albumsBox, 0, wxALL|wxEXPAND, 5 );
    
    wxButton* getAlbumsButton = new wxButton( this, FLMainWindowIDs::ID_GET_ALBUMS_BUTTON, wxT( "Get Albums" ), wxDefaultPosition );
    albumsBox->Add( getAlbumsButton );
    Connect( FLMainWindowIDs::ID_GET_ALBUMS_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FLMainWindowFrame::onGetAlbumsButton ) );
    
    wxBoxSizer* albumsIdsBox = new wxBoxSizer(wxHORIZONTAL);
    albumsBox->Add( albumsIdsBox, 1, wxSHAPED, 5 );
    
    wxStaticText* albumsIdsText = new wxStaticText( this, -1, wxT( "albums Ids:" ), wxDefaultPosition );
    wxTextCtrl* albumsIdsTextCtrl = new wxTextCtrl( this, FLMainWindowIDs::ID_ALBUMS_IDS_TEXT, wxT( "0" ), wxDefaultPosition, wxSize( 600, 25 ) );
    albumsIdsBox->Add( albumsIdsText );
    albumsIdsBox->Add( albumsIdsTextCtrl );
    
    albumsBox->Add( 0, 20 );
    
    // create album
    wxBoxSizer* createAlbumBox = new wxBoxSizer(wxHORIZONTAL);
    albumsBox->Add( createAlbumBox, 1, wxSHAPED, 5 );
    
    wxButton* createAlbumButton = new wxButton( this, FLMainWindowIDs::ID_CREATE_ALBUM_BUTTON, wxT( "Create Album" ), wxDefaultPosition );
    createAlbumBox->Add( createAlbumButton );
    Connect( FLMainWindowIDs::ID_CREATE_ALBUM_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FLMainWindowFrame::onCreateAlbum ) );
    
    wxTextCtrl* albumNameTextCtrl = new wxTextCtrl( this, FLMainWindowIDs::ID_ALBUM_NAME_TEXT, "album name", wxDefaultPosition, wxSize( 300, 25 ) );
    createAlbumBox->Add( albumNameTextCtrl );
    
    // photos box
    wxStaticBoxSizer* photosBox = new wxStaticBoxSizer( new wxStaticBox(this, wxID_ANY, _("Photos")), wxVERTICAL );
    actionsBox->Add( photosBox, 0, wxALL|wxEXPAND, 5 );
    
    // get photos box
    
    // album id box
    wxBoxSizer* albumIdBox = new wxBoxSizer(wxHORIZONTAL);
    photosBox->Add( albumIdBox, 1, wxSHAPED, 5 );
    
    wxStaticText* albumIdText = new wxStaticText( this, -1, wxT( "album Id:" ), wxDefaultPosition );
    wxTextCtrl* albumIdTextCtrl = new wxTextCtrl( this, FLMainWindowIDs::ID_ALBUM_ID_TEXT, "", wxDefaultPosition, wxSize( 300, 25 ) );
    albumIdBox->Add( albumIdText );
    albumIdBox->Add( albumIdTextCtrl );
    
    photosBox->Add( 0, 20 );
    
    // get photos button
    wxButton* getPhotosButton = new wxButton( this, FLMainWindowIDs::ID_GET_PHOTOS_BUTTON, wxT( "Get photos" ), wxDefaultPosition );
    photosBox->Add( getPhotosButton );
    Connect( FLMainWindowIDs::ID_GET_PHOTOS_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FLMainWindowFrame::onGetPhotosButton ) );

    wxBoxSizer* photosIdsBox = new wxBoxSizer(wxHORIZONTAL);
    photosBox->Add( photosIdsBox, 1, wxSHAPED, 5 );
    
    wxStaticText* photosIdsText = new wxStaticText( this, -1, wxT( "photos Ids:" ), wxDefaultPosition );
    wxStaticText* photosIdsValueText = new wxStaticText( this, FLMainWindowIDs::ID_PHOTOS_IDS_TEXT, wxT( "0" ), wxDefaultPosition, wxSize( 600, 25 ) );
    photosIdsBox->Add( photosIdsText );
    photosIdsBox->Add( photosIdsValueText );
    
    photosBox->Add( 0, 20 );
    
    // post photo with url box
    wxBoxSizer* imageUrlBox = new wxBoxSizer(wxHORIZONTAL);
    photosBox->Add( imageUrlBox, 1, wxSHAPED, 5 );
    
    wxStaticText* imageUrlText = new wxStaticText( this, -1, wxT( "image url:" ), wxDefaultPosition );
    wxTextCtrl* imageUrlTextCtrl = new wxTextCtrl( this, FLMainWindowIDs::ID_IMAGE_URL_TEXT, "", wxDefaultPosition, wxSize( 300, 25 ) );
    imageUrlBox->Add( imageUrlText );
    imageUrlBox->Add( imageUrlTextCtrl );
    
    wxButton* postImageUrlButton = new wxButton( this, FLMainWindowIDs::ID_POST_PHOTO_URL_BUTTON, wxT( "Post image with url" ), wxDefaultPosition );
    photosBox->Add( postImageUrlButton );
    Connect( FLMainWindowIDs::ID_POST_PHOTO_URL_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FLMainWindowFrame::onPostPhotoUrl ) );
    
    photosBox->Add( 0, 20 );
    
    // post photo with data box
    wxBoxSizer* fileNameBox = new wxBoxSizer(wxHORIZONTAL);
    photosBox->Add( fileNameBox, 1, wxSHAPED, 5 );
    
    wxStaticText* fileNameText = new wxStaticText( this, -1, wxT( "image file:" ), wxDefaultPosition );
    wxTextCtrl* fileNameTextCtrl = new wxTextCtrl( this, FLMainWindowIDs::ID_IMAGE_FILE_TEXT, "png or jpg", wxDefaultPosition, wxSize( 300, 25 ) );
    fileNameBox->Add( fileNameText );
    fileNameBox->Add( fileNameTextCtrl );
    
    wxButton* openDialogButton = new wxButton( this, FLMainWindowIDs::ID_OPEN_FILE_BUTTON, wxT( "..." ), wxDefaultPosition, wxSize( 40, 25 ) );
    Connect( FLMainWindowIDs::ID_OPEN_FILE_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FLMainWindowFrame::onOpenFileButton ) );
    fileNameBox->Add( openDialogButton );
    
    
    wxButton* postImageDataButtpn =  new wxButton( this, FLMainWindowIDs::ID_POST_PHOTO_DATA_BUTTON, wxT( "Post image with data" ), wxDefaultPosition );
    Connect( FLMainWindowIDs::ID_POST_PHOTO_DATA_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FLMainWindowFrame::onPostPhotoData ) );
    photosBox->Add( postImageDataButtpn );
    
 
    // fl4cpp init
	
    _flSession = new fl4cpp::Session();
    _flSession->registerObserver( this );
    fl4cpp::Settings::shared()->setAppId( std::string( appIdTextControl->GetLineText( 0 ).c_str() ) );
#ifdef _MSC_VER
	fl4cpp::Settings::shared()->setUserAgent( "Mozilla/5.0 (Windows; U; MSIE 7.0; Windows NT 6.0; en-US)" );
#endif
    SetSizeHints(-1,-1);
    if ( GetSizer() ) {
        GetSizer()->Fit(this);
    }
    Centre(wxBOTH);

}

FLMainWindowFrame::~FLMainWindowFrame()
{
    _flSession->unregisterObserver( this );
	delete _flSession;
}

void FLMainWindowFrame::onExit( wxCommandEvent& event )
{
    Close( true );
}

void FLMainWindowFrame::onAbout( wxCommandEvent& event )
{
    wxMessageBox( "About", "About", wxOK | wxICON_INFORMATION );
}

void FLMainWindowFrame::onLoginButton( wxCommandEvent& event )
{
    std::string data;

    std::vector< std::string > permissions;

    wxTextCtrl* permsField = (wxTextCtrl*)FindWindow( FLMainWindowIDs::ID_PERMISSIONS_TEXT );
    std::string permsStr = std::string( permsField->GetLineText( 0 ).c_str() );
    
    boost::char_separator< char > sep(",");
    boost::tokenizer< boost::char_separator<char> > tokens( permsStr, sep );
    BOOST_FOREACH( std::string t, tokens )
    {
        permissions.push_back( t );
    }

    _flSession->getLoginPage( &data, &permissions );
    
    wxWebView* view = (wxWebView*)FindWindow( FLMainWindowIDs::ID_WEBVIEW );
    view->SetPage( wxString( data ), "https://www.facebook.com" );
    view->Update();
}

void FLMainWindowFrame::onGetUserButton( wxCommandEvent& event )
{
    fl4cpp::UserNode* userNode = _flSession->getCurrentUserNode();
    
    wxStaticText* text = (wxStaticText*)FindWindow( FLMainWindowIDs::ID_USER_ID_TEXT );
    text->SetLabel( userNode->getNodeId() );
}

void FLMainWindowFrame::onGetAccountsButton( wxCommandEvent& event )
{
    fl4cpp::UserNode* userNode = _flSession->getCurrentUserNode();
    
    wxStaticText* text = (wxStaticText*)FindWindow( FLMainWindowIDs::ID_PAGES_IDS_TEXT );
    
    fl4cpp::AccountsEdge* accountsEdge = userNode->getAccountsEdge();
    
    std::vector< fl4cpp::PageNode* >* pageNodes = accountsEdge->getPageNodes();
    
    std::string label;
    for ( unsigned int i = 0; i < pageNodes->size(); i++ )
    {
        label.append( pageNodes->at( i )->getNodeId() + "," );
    }
    
    text->SetLabel( label );
}

void FLMainWindowFrame::onGetStatusesButton( wxCommandEvent &event )
{
    fl4cpp::UserNode* userNode = _flSession->getCurrentUserNode();
    
    wxStaticText* text = (wxStaticText*)FindWindow( FLMainWindowIDs::ID_STATUSES_IDS_TEXT );
    
    fl4cpp::StatusesEdge* statusesEdge = userNode->getStatusesEdge();
    
    std::vector< fl4cpp::StatusNode* >* statusNodes = statusesEdge->getStatusNodes();
    
    std::string label;
    for ( unsigned int i = 0; i < statusNodes->size(); i++ )
    {
        label.append( statusNodes->at( i )->getNodeId() + "," );
    }
        
    text->SetLabel( label );
    
    if ( !statusNodes->size() )
        return;
    
    fl4cpp::Node* authorNode = statusNodes->at( 0 )->getFrom();
}

void FLMainWindowFrame::onPostStatusButton( wxCommandEvent& event )
{
    fl4cpp::UserNode* userNode = _flSession->getCurrentUserNode();
    
    wxTextCtrl* text = (wxTextCtrl*)FindWindow( FLMainWindowIDs::ID_STATUS_TEXT );
    
    fl4cpp::FeedEdge* feedEdge = userNode->getFeedEdge();
    
    feedEdge->publish( std::string( text->GetLineText( 0 ).c_str() ), fl4cpp::PrivacySettingTypes::SELF );
}

void FLMainWindowFrame::onGetAlbumsButton( wxCommandEvent &event )
{
    fl4cpp::UserNode* userNode = _flSession->getCurrentUserNode();
    
    wxTextCtrl* text = (wxTextCtrl*)FindWindow( FLMainWindowIDs::ID_ALBUMS_IDS_TEXT );
    
    fl4cpp::AlbumsEdge* albumsEdge = userNode->getAlbumsEdge();
    
    std::vector< fl4cpp::AlbumNode* >* albumNodes = albumsEdge->getAlbumNodes();
    
    std::string label;
    for ( unsigned int i = 0; i < albumNodes->size(); i++ )
    {
        label.append( albumNodes->at( i )->getNodeId() + "," );
    }
    
    text->SetLabel( label );

    if ( !albumNodes->size() )
        return;
    
    fl4cpp::Node* authorNode = albumNodes->at( 0 )->getFrom();
}

void FLMainWindowFrame::onCreateAlbum( wxCommandEvent& event )
{
    fl4cpp::UserNode* userNode = _flSession->getCurrentUserNode();
    
    wxTextCtrl* text = (wxTextCtrl*)FindWindow( FLMainWindowIDs::ID_ALBUM_NAME_TEXT );
    
    fl4cpp::AlbumsEdge* albumsEdge = userNode->getAlbumsEdge();
    
    albumsEdge->publish( std::string( text->GetLineText( 0 ).c_str() ), "test description", fl4cpp::PrivacySettingTypes::SELF );
}

void FLMainWindowFrame::onPostPhotoUrl( wxCommandEvent &event )
{
    fl4cpp::UserNode* userNode = _flSession->getCurrentUserNode();
    
    wxTextCtrl* albumIdTextField = (wxTextCtrl*)FindWindow( FLMainWindowIDs::ID_ALBUM_ID_TEXT );
    wxTextCtrl* imageUrlTextField = (wxTextCtrl*)FindWindow( FLMainWindowIDs::ID_IMAGE_URL_TEXT );
    
    std::string targetAlbumId( albumIdTextField->GetLineText( 0 ).c_str() );
    std::string imageUrlText( imageUrlTextField->GetLineText( 0 ).c_str() );
    
    fl4cpp::AlbumsEdge* albumsEdge = userNode->getAlbumsEdge();
    
    std::vector< fl4cpp::AlbumNode* >* albumNodes = albumsEdge->getAlbumNodes();
    
    for ( unsigned int i = 0; i < albumNodes->size(); i++ )
    {
        if ( targetAlbumId == albumNodes->at( i )->getNodeId() )
        {
            fl4cpp::PhotosEdge* photosEdge = albumNodes->at( i )->getPhotosEdge();
            
            photosEdge->publishUrl( imageUrlText, "test message", false );
            
            break;
        }
    }
}

void FLMainWindowFrame::onPostPhotoData( wxCommandEvent &event )
{
    wxTextCtrl* albumIdTextField = (wxTextCtrl*)FindWindow( FLMainWindowIDs::ID_ALBUM_ID_TEXT );
    wxTextCtrl* filenameTextField = (wxTextCtrl*)FindWindow( FLMainWindowIDs::ID_IMAGE_FILE_TEXT );
    
    std::string targetAlbumId( albumIdTextField->GetLineText( 0 ).c_str() );
    std::string fileName( filenameTextField->GetLineText( 0 ).c_str() );

    std::ifstream imageFile;
    imageFile.open ( fileName, std::ios::binary | std::ios::ate );
    if ( !imageFile.good() )
    {
        std::cout << "Error reading image file.";
        imageFile.close();
        return;
    }
    
    unsigned long fileSize = imageFile.tellg();
    char* fileData = (char*)malloc( fileSize );
    
    imageFile.seekg( 0 );
    imageFile.read( fileData, fileSize );
    
    imageFile.close();
    
    fl4cpp::UserNode* userNode = _flSession->getCurrentUserNode();
    
    fl4cpp::AlbumsEdge* albumsEdge = userNode->getAlbumsEdge();
    
    std::vector< fl4cpp::AlbumNode* >* albumNodes = albumsEdge->getAlbumNodes();
    
    for ( unsigned int i = 0; i < albumNodes->size(); i++ )
    {
        if ( targetAlbumId == albumNodes->at( i )->getNodeId() )
        {
            fl4cpp::PhotosEdge* photosEdge = albumNodes->at( i )->getPhotosEdge();
            
            photosEdge->publishData( fileData, fileSize, "test description", false );
            
            break;
        }
    }

	free( fileData );
}

void FLMainWindowFrame::onOpenFileButton( wxCommandEvent &event )
{
    wxFileDialog* OpenDialog = new wxFileDialog( this,
                                                _("Choose a file to open"),
                                                wxEmptyString,
                                                wxEmptyString,
                                                _("Image files (*.png,*.jpg)|*.png;*.jpg"),
                                                  wxFD_OPEN,
                                                wxDefaultPosition );

    if (OpenDialog->ShowModal() == wxID_OK)
    {
        wxTextCtrl* text = (wxTextCtrl*)FindWindow( FLMainWindowIDs::ID_IMAGE_FILE_TEXT
                                                   );
        text->SetLabel( OpenDialog->GetPath() );
    }

    OpenDialog->Destroy();
}

void FLMainWindowFrame::onLogoutButton( wxCommandEvent &event )
{
    _flSession->logout();
}

void FLMainWindowFrame::onGetPhotosButton( wxCommandEvent &event )
{
    fl4cpp::UserNode* userNode = _flSession->getCurrentUserNode();
    
    wxTextCtrl* albumIdTextCtrl = (wxTextCtrl*)FindWindow( FLMainWindowIDs::ID_ALBUM_ID_TEXT );
    wxStaticText* photosIdsValuesText = (wxStaticText*)FindWindow( FLMainWindowIDs::ID_PHOTOS_IDS_TEXT );
    
    fl4cpp::AlbumsEdge* albumsEdge = userNode->getAlbumsEdge();
    
    std::vector< fl4cpp::AlbumNode* >* albumNodes = albumsEdge->getAlbumNodes();
    
    std::string targetAlbumId( albumIdTextCtrl->GetLineText( 0 ).c_str() );
    
    std::string label;
    for ( unsigned int i = 0; i < albumNodes->size(); i++ )
    {
        if ( targetAlbumId == albumNodes->at( i )->getNodeId() )
        {
            fl4cpp::PhotosEdge* photosEdge = albumNodes->at( i )->getPhotosEdge();
            
            std::vector< fl4cpp::PhotoNode* >* photoNodes = photosEdge->getPhotoNodes();
            
            for ( unsigned int j = 0; j < photoNodes->size(); j++ )
            {
                label.append( photoNodes->at( j )->getNodeId() + "," );
            }
                
            photosIdsValuesText->SetLabel( label );
            
            if ( !photoNodes->size() )
                break;
            
            fl4cpp::Node* authorNode = photoNodes->at( 0 )->getFrom();
        }
    }
}

void FLMainWindowFrame::onGetPictureButton( wxCommandEvent &event )
{
    fl4cpp::UserNode* userNode = _flSession->getCurrentUserNode();
    
    wxTextCtrl* imageUrlText = (wxTextCtrl*)FindWindow( FLMainWindowIDs::ID_PICTURE_URL_TEXT );

    fl4cpp::PictureEdge* pictureEdge = userNode->getPictureEdge();
    
    fl4cpp::PictureInfo pictureInfo( "", false, 0, 0 );
    
    if ( pictureEdge->readInfo( fl4cpp::PictureTypes::NORMAL, &pictureInfo ) )
    {
        imageUrlText->SetLabel( pictureInfo.getUrl() );
    }
}

void FLMainWindowFrame::onWebViewEvent( wxWebViewEvent& event )
{
    if ( event.GetEventType() == wxEVT_WEBVIEW_NAVIGATED )
    {
        std::string url = std::string( event.GetURL().c_str() );
        
        http::url parsed = http::ParseHttpUrl( url );
        
        if ( parsed.error_reason.size() )
        {
            // do something
        }
        
        if ( parsed.access_token.size() == 0 )
            return;
        
        _access_token = parsed.access_token;
        
        
        try
        {
            _expires_in = std::stoi( parsed.expires_in );
        }
        catch ( ... )
        {
            _expires_in = 1;
        }
        
        std::string tokenStr = _access_token;
        tokenStr.resize( 30 );
        tokenStr = tokenStr + "...";
        
        _flSession->setAccessToken( _access_token, _expires_in );
        
        wxStaticText* access_tokenText = (wxStaticText*)FindWindow( FLMainWindowIDs::ID_ACCESS_TOKEN_TEXT );
        access_tokenText->SetLabel( tokenStr );
        
    }
}

void FLMainWindowFrame::onCreateAlbumOnPageButton( wxCommandEvent &event )
{
    
    fl4cpp::UserNode* userNode = _flSession->getCurrentUserNode();
    
    wxTextCtrl* pageIdText = (wxTextCtrl*)FindWindow( FLMainWindowIDs::ID_PAGE_ID_TEXT );
    wxTextCtrl* albumNameText = (wxTextCtrl*)FindWindow( FLMainWindowIDs::ID_ALBUM_NAME_FOR_PAGE_TEXT );
    
    std::string pageId = std::string( pageIdText->GetLineText( 0 ).c_str() );
    std::string albumName = std::string( albumNameText->GetLineText( 0 ).c_str() );
    
    fl4cpp::AccountsEdge* accountsEdge = userNode->getAccountsEdge();
    
    std::vector< fl4cpp::PageNode* >* pageNodes = accountsEdge->getPageNodes();
    
    for ( unsigned int i = 0; i < pageNodes->size(); i++ )
    {
        if ( pageNodes->at( i )->getNodeId() == pageId )
        {
            
            fl4cpp::AlbumsEdge* albumsEdge = pageNodes->at( i )->getAlbumsEdge();
            
            albumsEdge->publish( albumName, "test album description", fl4cpp::PrivacySettingTypes::SELF );
        }
    }
}

void FLMainWindowFrame::onAccessTokenExpired()
{
    wxMessageBox( wxT("Access token expired! please relogin.") );
}