#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	CreateOptions();
}

void MainFrame::CreateOptions()
{
	wxFont headingFont(wxFontInfo(wxSize(0, 36)).Bold());
	panel = new wxPanel(this);
	panel->SetBackgroundColour(*wxRED);
	headingText = new wxStaticText(panel, wxID_ANY, "Stock Management Page",
		wxPoint(0, 22), wxSize(1000, -1), wxALIGN_CENTER_HORIZONTAL);

	headingText->SetBackgroundColour(*wxBLUE);
	headingText->SetFont(headingFont);

	panel2 = new wxPanel(panel, wxID_ANY, wxPoint(500, 100), wxSize(450, 450));
	panel2->SetBackgroundColour(*wxWHITE);
	textInput = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(250, 100), wxSize(200, 20));
	createButton = new wxButton(panel, wxID_ANY, "", wxPoint(325, 150), wxSize(50, 50));
	createButton->SetBackgroundColour(*wxBLUE);
}