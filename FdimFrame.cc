#include <wx/splitter.h>
#include <wx/filename.h>
#include "FdimFrame.h"
#include "BitmapView.h"
#include "LineView.h"
#include <wx/msgdlg.h>

enum {
   Menu_File_OBitmap = wxID_HIGHEST + 1,
   Menu_Calc_Capacity,
   Menu_Calc_Information,
   Menu_Calc_Correlation,
   Menu_Calc_Probability,
};

BEGIN_EVENT_TABLE(CFdimFrame, wxFrame)
	EVT_MENU(Menu_File_OBitmap,         CFdimFrame::OnFileOBitmap)
	EVT_MENU(wxID_EXIT,            CFdimFrame::OnFileExit)

	EVT_MENU(Menu_Calc_Capacity,        CFdimFrame::OnCalc)
	EVT_MENU(Menu_Calc_Information,     CFdimFrame::OnCalc)
	EVT_MENU(Menu_Calc_Correlation,     CFdimFrame::OnCalc)
	EVT_MENU(Menu_Calc_Probability,     CFdimFrame::OnCalc)
	EVT_MENU(wxID_ABOUT,                CFdimFrame::GetInfo)

	EVT_COMMAND(wxID_ANY, myEVT_LINEVIEW_CHANGED, CFdimFrame::OnLineViewChanged)
END_EVENT_TABLE()


CFdimFrame::CFdimFrame()
	: wxFrame(0, wxID_ANY, wxT("Fractal-Dimension"), wxPoint(50, 50), wxSize(850, 420))
{
	idir = wxGetCwd();
	
	// File Menu
	wxMenu* file_menu = new wxMenu;
	file_menu->Append(Menu_File_OBitmap,wxT("Open Image"));
	//file_menu->AppendSeparator();
	file_menu->Append(wxID_ABOUT,wxT("About"));
	file_menu->Append(wxID_EXIT,wxT("Exit"));

	// Calculate Menu
	wxMenu* calc_menu = new wxMenu;
	calc_menu->Append(Menu_Calc_Capacity,wxT("Capacity"));
	calc_menu->Append(Menu_Calc_Information,wxT("Information"));
	calc_menu->Append(Menu_Calc_Correlation,wxT("Correlation"));
	calc_menu->Append(Menu_Calc_Probability,wxT("Probability"));
	// InfoMenu
	//wxMenu* info_menu = new wxMenu;
	//info_menu->Append(Menu_Info,wxT("About"));

	
	// MenuBar
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(file_menu, wxT("File"));
	menuBar->Append(calc_menu, wxT("Calculate"));
	//menuBar->Append(info_menu, wxT("Info"));
	SetMenuBar(menuBar);

	// Statusbar
	CreateStatusBar(2);	
	SetStatusBarPane(-1); // disable help text
	GetStatusBar()->PushStatusText(wxT("FDIM GUI initialized"), 0);
		
	// Fuege Canvas hinzu	
	wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY);
 	bitmapView = new BitmapView(splitter,wxID_ANY);
	lineView   = new LineView(splitter,wxID_ANY);

	splitter->SplitVertically(bitmapView, lineView);
	splitter->SetMinimumPaneSize(20);
	splitter->SetSashGravity(0.5);
	splitter->SetSashPosition(0);
}


void CFdimFrame::OnFileOBitmap(wxCommandEvent& event)
{
	wxString filename = wxFileSelector(wxT("Open Image"),idir,wxT(""), wxT("bmp"),
									   wxT("Possible files (*.bmp;*.tiff;*.png;*.jpg;*.jpeg)|*.bmp;*.tiff;*.png;*.jpg;*.jpeg|Bitmap files (*.bmp)|*.bmp|PNG files (*.png)|*.png|TIFF files (*.tiff)|*.tiff|JPG files (*.jpg;*.jpeg)|*.jpg;*jpeg|All files (*.*)|*.*"),
									   wxFD_OPEN);
	if ( !filename.empty() ) {
		idir = wxPathOnly(filename);
		bitmapFractal.LoadFile(filename);
		bitmapFractal.convertToGrey();
		bitmapView->setImage(bitmapFractal);
		lineView->setLine(0);

		GetStatusBar()->PopStatusText(0);
		GetStatusBar()->PushStatusText(wxFileName(filename).GetName(), 0);
	}
}


void CFdimFrame::OnFileExit(wxCommandEvent& event)
{
	Close();
}


void CFdimFrame::OnCalc(wxCommandEvent& event)
{
	wxRect sel(0, 0, bitmapFractal.GetWidth(), bitmapFractal.GetHeight());
	if(bitmapView->isActiveSelection()) 
		sel = bitmapView->getSelection();
	
	switch(event.GetId()) {
	case Menu_Calc_Capacity:
		bitmapFractal.capacity(sel, line);
		break;
	case Menu_Calc_Information:
		bitmapFractal.information(sel, line);
		break;
	case Menu_Calc_Correlation:
		bitmapFractal.correlation(sel, line);
		break;
	case Menu_Calc_Probability:
		bitmapFractal.probability(sel, line);
		break;
	default: ;
	}

	lineView->setLine(&line);
}

void CFdimFrame::OnLineViewChanged(wxCommandEvent& event)
{
	wxString message;
	message << wxT("Slope: ") << line.slope() << wxT(" Correlation: ") <<  line.correlation();
	SetStatusText(message,1);
}


void   CFdimFrame::GetInfo(wxCommandEvent& event)
{
    wxMessageBox(wxT("FDim v1.1 (March 2012) \nVisit: http://reuter.mit.edu \n2007-2012 Copyright by Martin Reuter"), wxT("Fdim"),
                            wxICON_INFORMATION| wxOK, this);

}


