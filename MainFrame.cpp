#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	CreateOptions();
}

void MainFrame::CreateOptions()
{
	wxFont headingFont(wxFontInfo(wxSize(0, 36)).Bold());
	MainPanel = new wxPanel(this);
	MainPanel->SetBackgroundColour(*wxRED);

	headingText = new wxStaticText(MainPanel, wxID_ANY, "Stock Management Page",
		wxPoint(0, 0), wxSize(1000, 100), wxALIGN_CENTER_HORIZONTAL);
	headingText->SetBackgroundColour(*wxBLUE);
	headingText->SetFont(headingFont);

	InteractiveArea = new wxPanel(MainPanel, wxID_ANY, wxPoint(0, 100), wxSize(1000, 500));
	InteractiveArea->SetBackgroundColour(*wxGREEN);

	NavigationBar = new wxPanel(InteractiveArea, wxID_ANY, wxPoint(0, 0), wxSize(150, 500));
	NavigationBar->SetBackgroundColour(*wxYELLOW);

	panel2 = new wxPanel(InteractiveArea, wxID_ANY, wxPoint(500, 100), wxSize(450, 450));
	panel2->SetBackgroundColour(*wxWHITE);

	textInput = new wxTextCtrl(InteractiveArea, wxID_ANY, "", wxPoint(250, 100), wxSize(200, 20));

	createButton = new wxButton(InteractiveArea, wxID_ANY, "", wxPoint(325, 150), wxSize(50, 50));
	createButton->SetBackgroundColour(*wxBLUE);
}