#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

private:
	void CreateOptions();

	wxPanel* MainPanel;
	wxStaticText* headingText;
	wxPanel* InteractiveArea;
	wxPanel* NavigationBar;
	wxTextCtrl* textInput;
	wxButton* createButton;
	wxPanel* panel2;
};

