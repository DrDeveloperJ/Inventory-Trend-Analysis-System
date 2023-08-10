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

	ActiveArea = new wxPanel(InteractiveArea, wxID_ANY, wxPoint(150, 0), wxSize(850, 500));
	ActiveArea->SetBackgroundColour(*wxGREEN);

	TreeviewTable = new wxPanel(ActiveArea, wxID_ANY, wxPoint(425, 25), wxSize(400, 450));
	TreeviewTable->SetBackgroundColour(*wxWHITE);

	IDInput = new wxTextCtrl(ActiveArea, wxID_ANY, "", wxPoint(125, 100), wxSize(200, 20));
	ItemInput = new wxTextCtrl(ActiveArea, wxID_ANY, "", wxPoint(125, 125), wxSize(200, 20));
	QuantityInput = new wxTextCtrl(ActiveArea, wxID_ANY, "", wxPoint(125, 150), wxSize(200, 20));

	createButton = new wxButton(ActiveArea, wxID_ANY, "", wxPoint(325, 150), wxSize(50, 50));
	createButton->SetBackgroundColour(*wxBLUE);
}