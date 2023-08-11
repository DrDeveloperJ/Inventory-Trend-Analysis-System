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
	wxButton* StockManage;
	wxPanel* ActiveArea;

	wxPanel* CreateArea;
	wxStaticText* CreateHeading;
	wxTextCtrl* CreateIDInput;
	wxStaticText* CreateIDLabel;
	wxTextCtrl* CreateItemInput;
	wxStaticText* CreateItemLabel;
	wxTextCtrl* CreateQuantityInput;
	wxStaticText* CreateQuantityLabel;
	wxButton* createButton;

	wxPanel* DeleteArea;
	wxStaticText* DeleteHeading;
	wxTextCtrl* DeleteInput;
	wxStaticText* DeleteInputLabel;
	wxButton* DeleteButton;

	wxPanel* UpdateArea;
	wxStaticText* UpdateHeading;
	wxTextCtrl* UpdateIDInput;
	wxStaticText* UpdateIDLabel;
	wxTextCtrl* UpdateItemInput;
	wxStaticText* UpdateItemLabel;
	wxTextCtrl* UpdateQuantityInput;
	wxStaticText* UpdateQuantityLabel;
	wxButton* UpdateButton;

	wxPanel* TreeviewTable;
};

