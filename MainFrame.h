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
	wxPanel* ActiveArea;
	wxTextCtrl* IDInput;
	wxTextCtrl* ItemInput;
	wxTextCtrl* QuantityInput;
	wxButton* createButton;
	wxPanel* TreeviewTable;
};

