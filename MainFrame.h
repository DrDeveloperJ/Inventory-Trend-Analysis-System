#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>

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
	wxButton* SellSystem;
	wxButton* AnalysisPage;
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
	//void CreateButtonClicked(wxCommandEvent& evt);

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
	static wxListView* basicListView;
	static wxListView* sellBasicListView;

	wxPanel* SellArea;
	wxStaticText* SellHeading;
	wxTextCtrl* SellIDInput;
	wxStaticText* SellIDLabel;
	wxTextCtrl* SellQuantityInput;
	wxStaticText* SellQuantityLabel;
	wxButton* SellButton;
};