#ifndef __FL_WX_APP__
#define __FL_WX_APP__

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

class FLWxApp: public wxApp
{
public:
    virtual bool OnInit();
};

#endif // __FL_WX_APP__
