#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	CreateOptions();
}

void MainFrame::CreateOptions()
{
	wxFont headingFont(wxFontInfo(wxSize(0, 36)).Bold());
	panel = new wxPanel(this);
	headingText = new wxStaticText(panel, wxID_ANY, "Stock Management Page",
		wxPoint(0, 22), wxSize(1000, -1), wxALIGN_CENTER_HORIZONTAL);

	headingText->SetBackgroundColour(*wxBLUE);
	headingText->SetFont(headingFont);
}