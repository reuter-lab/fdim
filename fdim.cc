#include <wx/wx.h>
#include "FdimFrame.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

IMPLEMENT_APP(MyApp)
DECLARE_APP(MyApp)

bool MyApp::OnInit()
{
  wxInitAllImageHandlers();
  wxFrame *frame = new CFdimFrame();
  frame->Show(true);
  return true;
}
