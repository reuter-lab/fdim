/*
 * Original Author: Martin Reuter
 * Copyright 2014 Martin Reuter
 * 
 * Terms and conditions for use, reproduction, distribution and contribution
 * are found in the file 'LICENSE'
 *
 */

#ifndef _FDIMFRAME_H
#define _FDIMFRAME_H

#include <wx/wx.h>
#include "BitmapFractal.h"
#include "Line.h"

class BitmapView;
class LineView;

class CFdimFrame : public wxFrame
{
public:
  CFdimFrame();

private:
  wxString idir;

  BitmapFractal  bitmapFractal;
  Line           line;
  BitmapView    *bitmapView;
  LineView      *lineView;
  
  void OnFileOBitmap(wxCommandEvent& event);
  void OnFileExit(wxCommandEvent& event);
  void OnCalc(wxCommandEvent& event);
  void OnLineViewChanged(wxCommandEvent& event);
  void GetInfo(wxCommandEvent& event);
  DECLARE_EVENT_TABLE()
};
#endif
