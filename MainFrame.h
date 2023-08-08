#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

private:
	void CreateOptions();

	wxPanel* panel;
	wxStaticText* headingText;
	wxTextCtrl* textInput;
	wxButton* createButton;
	wxPanel* panel2;
};

