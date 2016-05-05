#ifndef __FL4CPP_SESSION__
#define __FL4CPP_SESSION__

#include "UserNode.h"

#include <string>
#include <map>

#include <boost/thread.hpp>

namespace fl4cpp
{
    /** Delegate that Session uses to inform the main program that
     * provided Facebook User Access Token has expired and program should
     * redo Login Sequence again in order to get new User Access Token.
     */
    class SessionObserver
    {
    public:
        virtual ~SessionObserver() {};
        
        /** Callback that occurs when current Facebook User Access Token has expired.
         */
        virtual void onAccessTokenExpired() = 0;
    };

    class UserNode;
    
    /** Core class that is used for initiating Facebook Login sequence
     * and retrieving User Node of a logged in user.
     * Before retrieving a User Node - Session should be provided with
     * a User Access Token that is achieved during login sequence.
     * All network-related methods of this class and all other fl4cpp clases
     * work in a blocking manner, so it is better to run them in a thread other than UI.
     * 
     * How to use:
     * 1) Set App Id in Settings singleton
     * 2) Call getLoginPage with required permissions list.
     * More on permissions here: https://developers.facebook.com/docs/facebook-login/permissions/v2.0
     * 3) Open received web page data in a browser.
     * 4) Wait for user to accept requested permissions and
     *  complete Login sequence.
     * 5) Upon completion - facebook will send you a User Access Token.
     * 6) Call setAccessToken with that token as an argument.
     * 7) Call getUserNode to get the User Node of a current user.
     *
     * For a better understanding of the Facebook Login Sequence flow
     * read the following doc: https://developers.facebook.com/docs/facebook-login/manually-build-a-login-flow/v2.0
	 */
    class Session
    {
    public:
        /** Constructs Session object.
         */
        Session();
        
        ~Session();
        
        /** Gets current User Access Token.
         *
		 * @return User Access Token.
		 */
        std::string getAccessToken() const { return _accessToken; };
        
        /** Sets User Access Token and requests access tokens for all
         * Facebook Pages which current user has administrator access for.
         *
         * @param accessToken User Access Token.
         * @param expiresIn time in seconds before this token will expire (provided by Facebook in pair with Access Token).
         */
        void setAccessToken( std::string accessToken, unsigned int expiresIn );
        
        /** Gets a Page Access Token for a Facebook Page by its Node Id.
         *
         * @param pageNodeId Node Id of the desired page.
         *
         * @return Access Token string or empty string if user has no admin rights for this page.
         */
        std::string getPageAccessToken( std::string pageNodeId );
        
        /** Requests access tokens for all Facebook Pages
         * which current user has administrator access for.
         *
         * Called automatically on setAccessToken call.
         */
        void updatePageAccessTokens();
        
        /** Registers a SessionObserver object, that will be notified
         * when current User Access Token expires.
         *
         * @param observer pointer to a SessionObserver object.
         */
        void registerObserver( SessionObserver *observer );
        
        /** Unregisters a SessionObserver object.
         *
         * @param pointer to a SessionObserver object.
         */
        void unregisterObserver( SessionObserver *observer );
        
        /** Requests a Login Page data from Facebook with the list of permissions
         * required for operations that main program wants to execute.
         * Received data is affected by the User Agent setting (see Settings singleton),
         * so make sure that your browser support all features of a given User Agent.
         *
         * @param receivedData Login Page html data.
         * @param permissions pointer to a list of desired permissions.
         */
        bool getLoginPage( std::string* receivedData, std::vector< std::string >* permissions );

        /** Requests a Node of a logged in user.
         * Received Node data is saved in _currentUserNode class member.
         * 
         * @param initFromGraphApi flag, that if set to false will result in
         * returning a pointer to the local copy of User Node without
         * sending a request to a Facebook server.
         *
         * @return pointer to a local copy of User Node.
         */
        UserNode* getCurrentUserNode( bool initFromGraphApi = true );
        
        /** Invalidates current User Access Token and all its Page Accesss Tokens.
         */
        void logout();
        
    private:
        /** Thread function that notifies all observers about current 
         * User Access Token expiration after.
         *
         * @param timeToWait delay before observers shpuld be notified.
         */
        void observersNotificationThreadFunc( unsigned int timeToWait );
        
    private:
        /** User Access Token string.
         */
        std::string _accessToken;
        
        /** List of the Page Access Tokens which current user has administrator access for.
         */
        std::map< std::string, std::string >  _pageAccessTokens;
        #define PAGE_ACCESS_TOKENS_ITERATOR std::map< std::string, std::string >::iterator
        
        /** List of the observers waiting for token expiration callback.
         */
        std::map< SessionObserver*, SessionObserver* > _sessionObservers;
        #define SESSION_OBSERVERS_ITERATOR std::map< SessionObserver*, SessionObserver* >::iterator
        
        /** Local copy of a User Node.
         */
        UserNode _currentUserNode;
        
        /** Mutex for observers notification threads.
         */
        boost::mutex                _threadFuncMutex;
        
        /** Condition variable that is used to terminate observer notification threads
         * without notifying observers.
         */
        boost::condition_variable   _notificationThreadsShouldQuit;
    };
}

#endif // __FL4CPP_SESSION__
