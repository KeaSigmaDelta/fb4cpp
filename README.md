C++ Library for the Facebook Graph API
======================================

This is a C++ library for accessing the Facebook graph API. It doesn't implement
the complete API, but is enough to download/upload images to Facebook photo albums.


License
-------

Distributed under a BSD style license. See License.txt for details.


Getting Started
---------------

The easiest way to get started is as follows:
1. Install the dependencies (see below)
2. Have a look at the test application in fl4cpp_test_app
3. Look at the documentation in the documentation directory 
   (Documentation/html/index.html). Start with the Session class
   (Documentation/html/classfl4cpp_1_1_session.html)


Dependencies
------------

* The BOOST C++ library:  http://www.boost.org/
* LibCURL: https://curl.haxx.se/libcurl/
* WxWidgets 3+ (for fl4cpp_test_app): http://wxwidgets.org/


To-Do/Wishlist
--------------

* Implement more of the Facebook Graph API
* Replace BOOST with equvalent C++11 classes
* Add asynchronous operation. Right now networking is performed synchronously, 
  blocking the caller. We don't want an application's GUI becoming non-responsive
  every time that a network operation is in progress. So, asynchronous operation
  is preferred
