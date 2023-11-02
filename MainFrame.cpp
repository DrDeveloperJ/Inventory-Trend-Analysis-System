#include "MainFrame.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "stdafx.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	CreateOptions();
}

static std::string GlobalSQLPassword;
static std::string UserLatitude;
static std::string UserLongitude;

wxListView* MainFrame::basicListView = nullptr;
wxListView* MainFrame::sellBasicListView = nullptr;
wxListView* MainFrame::analysisBasicListView = nullptr;

//Loops through the database and inserts all the current data into the treeview table
//-----------------------------------------------------------------------------------------------------
void StartTreeview(wxPanel*& TreeviewTable, wxListView*& basicListView)
{
	int YPos{ 0 };

	//Establishes a connection with the MySQL Database
	//-----------------------------------------------------------------------------------------------------
	const std::string server = "tcp://managestock.mysql.database.azure.com:3306";
	const std::string username = "rootConnect";
	const std::string password = GlobalSQLPassword;

	sql::Driver* driver;
	sql::Connection* con;
	sql::PreparedStatement* pstmt;
	sql::ResultSet* result;

	try
	{
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
	}
	catch (sql::SQLException e)
	{
		//cout << "Could not connect to server. Error message: " << e.what() << endl;
		system("pause");
		exit(1);
	}

	con->setSchema("itemdatabase");

	pstmt = con->prepareStatement("SELECT * FROM itemtable;");
	result = pstmt->executeQuery();
	//-----------------------------------------------------------------------------------------------------

	while (result->next())
	{
		sql::SQLString ItemName{ result->getString("ItemName") };
		wxString wxItemName{ wxString::FromUTF8(ItemName.c_str()) };
		//sql::SQLString ItemQuantity{ result->getInt("ItemQuantity") };
		wxString wxItemQuantity{ wxString::Format(wxT("%d"), result->getInt("ItemQuantity")) };
		sql::SQLString ItemID{ result->getString("ItemID") };
		wxString wxItemID{ wxString::FromUTF8(ItemID.c_str()) };

		basicListView->InsertItem(0, wxItemName);
		basicListView->SetItem(0, 1, wxItemQuantity);
		basicListView->SetItem(0, 2, wxItemID);
	};

	delete pstmt;
	delete con;
};
//-----------------------------------------------------------------------------------------------------

void MainFrame::CreateOptions()
{

	//Creates the Main Page as "MainPanel" and prepares a font for the page heading
	wxFont headingFont(wxFontInfo(wxSize(0, 36)).Bold());
	MainPanel = new wxPanel(this);
	MainPanel->SetBackgroundColour(*wxRED);

	//Creates a heading
	static wxStaticText* headingText = new wxStaticText(MainPanel, wxID_ANY, "Stock Management Page",
		wxPoint(0, 0), wxSize(1000, 100), wxALIGN_CENTER_HORIZONTAL);
	headingText->SetBackgroundColour(*wxWHITE);
	headingText->SetFont(headingFont);

	/*This defines the rest of the page which will include the navigation bar on the left and the rest is what will
	be interacted with by the user (this will be called ActiveArea) */
	InteractiveArea = new wxPanel(MainPanel, wxID_ANY, wxPoint(0, 100), wxSize(1000, 500));
	InteractiveArea->SetBackgroundColour(*wxGREEN);

	static wxPanel* OwnerUnlockArea = new wxPanel(InteractiveArea, wxID_ANY, wxPoint(0, 0), wxSize(1000, 500));
	OwnerUnlockArea->SetBackgroundColour(*wxBLUE);

	//StockActive is the Stock Management page
	static wxPanel* StockActiveArea = new wxPanel(InteractiveArea, wxID_ANY, wxPoint(150, 0), wxSize(850, 500));
	StockActiveArea->SetBackgroundColour(*wxGREEN);
	StockActiveArea->Show(false);
	//AnalysisActiveArea is the First Analysis page
	static wxPanel* AnalysisActiveArea = new wxPanel(InteractiveArea, wxID_ANY, wxPoint(150, 0), wxSize(850, 500));
	AnalysisActiveArea->SetBackgroundColour(*wxGREEN);
	AnalysisActiveArea->Show(false);
	//AnalysisActiveArea is the Second Analysis page that is shown when the ItemID searched is validated
	static wxPanel* AnalysisChooseArea = new wxPanel(InteractiveArea, wxID_ANY, wxPoint(150, 0), wxSize(850, 500));
	AnalysisChooseArea->SetBackgroundColour(*wxGREEN);
	AnalysisChooseArea->Show(false);
	//WeatherAnalysisActiveArea is the Analysis page for Weather that is shown when the user chooses weather in AnalysisChooseArea
	static wxPanel* WeatherAnalysisActiveArea = new wxPanel(InteractiveArea, wxID_ANY, wxPoint(150, 0), wxSize(850, 500));
	WeatherAnalysisActiveArea->SetBackgroundColour(*wxGREEN);
	WeatherAnalysisActiveArea->Show(false);
	//TimeAnalysisActiveArea is the Analysis page for Weather that is shown when the user chooses weather in AnalysisChooseArea
	static wxPanel* TimeAnalysisActiveArea = new wxPanel(InteractiveArea, wxID_ANY, wxPoint(150, 0), wxSize(850, 500));
	TimeAnalysisActiveArea->SetBackgroundColour(*wxGREEN);
	TimeAnalysisActiveArea->Show(false);
	//StockActive area is the SellSystem page
	static wxPanel* SellActiveArea = new wxPanel(InteractiveArea, wxID_ANY, wxPoint(150, 0), wxSize(850, 500));
	SellActiveArea->SetBackgroundColour(*wxGREEN);
	SellActiveArea->Show(false);

	static wxPanel* SellTreeviewTable = new wxPanel(SellActiveArea, wxID_ANY, wxPoint(440, 25), wxSize(399, 450));
	SellTreeviewTable->SetBackgroundColour(*wxWHITE);
	sellBasicListView = new wxListView(SellTreeviewTable, wxID_ANY, wxDefaultPosition, wxSize(399, 450));

	static wxPanel* AnalysisTreeviewTable = new wxPanel(AnalysisActiveArea, wxID_ANY, wxPoint(440, 25), wxSize(399, 450));
	AnalysisTreeviewTable->SetBackgroundColour(*wxWHITE);
	analysisBasicListView = new wxListView(AnalysisTreeviewTable, wxID_ANY, wxDefaultPosition, wxSize(399, 450));

	//Navigation Bar for navigating between pages
	//-----------------------------------------------------------------------------------------------------
	NavigationBar = new wxPanel(InteractiveArea, wxID_ANY, wxPoint(0, 0), wxSize(150, 500));
	NavigationBar->SetBackgroundColour(*wxYELLOW);
	NavigationBar->Show(false);

	AnalysisPage = new wxButton(NavigationBar, wxID_ANY, "Analysis", wxPoint(25, 20), wxSize(100, 75));
	AnalysisPage->SetBackgroundColour(*wxBLUE);
	AnalysisPage->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {
		StockActiveArea->Show(false);
		SellActiveArea->Show(false);
		AnalysisActiveArea->Show(true);
		AnalysisChooseArea->Show(false);
		WeatherAnalysisActiveArea->Show(false);
		TimeAnalysisActiveArea->Show(false);
		headingText->SetLabel(wxT("Analysis Page"));
		headingText->Refresh();
		});
	StockManage = new wxButton(NavigationBar, wxID_ANY, "Stock Manage", wxPoint(25, 120), wxSize(100, 75));
	StockManage->SetBackgroundColour(*wxBLUE);
	StockManage->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {
		AnalysisActiveArea->Show(false);
		SellActiveArea->Show(false);
		StockActiveArea->Show(true);
		AnalysisChooseArea->Show(false);
		WeatherAnalysisActiveArea->Show(false);
		TimeAnalysisActiveArea->Show(false);
		headingText->SetLabel(wxT("Stock Management Page"));
		headingText->Refresh();
		});
	SellSystem = new wxButton(NavigationBar, wxID_ANY, "Sell System", wxPoint(25, 220), wxSize(100, 75));
	SellSystem->SetBackgroundColour(*wxBLUE);
	SellSystem->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {
		AnalysisActiveArea->Show(false);
		StockActiveArea->Show(false);
		SellActiveArea->Show(true);
		AnalysisChooseArea->Show(false);
		WeatherAnalysisActiveArea->Show(false);
		TimeAnalysisActiveArea->Show(false);
		headingText->SetLabel(wxT("Sell System"));
		headingText->Refresh();
		});
	//-----------------------------------------------------------------------------------------------------

	//Stock Management Page
	//--------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------

	//This will display all the stock to the user in a user friendly Treeview Table
	static wxPanel* TreeviewTable = new wxPanel(StockActiveArea, wxID_ANY, wxPoint(440, 25), wxSize(399, 450));
	TreeviewTable->SetBackgroundColour(*wxWHITE);

	basicListView = new wxListView(TreeviewTable, wxID_ANY, wxDefaultPosition, wxSize(399, 450));

	basicListView->AppendColumn("ItemName");
	basicListView->AppendColumn("ItemQuantity");
	basicListView->AppendColumn("ItemID");
	basicListView->SetColumnWidth(0, 133);
	basicListView->SetColumnWidth(1, 133);
	basicListView->SetColumnWidth(2, 133);

	//This will allow the user to create a new item to add to the stock
	//-----------------------------------------------------------------------------------------------------
	wxFont SectionHeadingFont(wxFontInfo(wxSize(0, 20)).Bold());
	CreateArea = new wxPanel(StockActiveArea, wxID_ANY, wxPoint(125, 30), wxSize(200, 200));
	CreateArea->SetBackgroundColour(*wxWHITE);
	CreateHeading = new wxStaticText(CreateArea, wxID_ANY, "Create Item",
		wxPoint(50, 5), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);
	CreateHeading->SetFont(SectionHeadingFont);

	static wxTextCtrl* CreateIDInput = new wxTextCtrl(CreateArea, wxID_ANY, "", wxPoint(5, 60), wxSize(190, 20));
	CreateIDInput->SetBackgroundColour(*wxBLUE);
	CreateIDLabel = new wxStaticText(CreateArea, wxID_ANY, "Item ID",
		wxPoint(50, 40), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);

	static wxTextCtrl* CreateItemInput = new wxTextCtrl(CreateArea, wxID_ANY, "", wxPoint(5, 100), wxSize(190, 20));
	CreateItemInput->SetBackgroundColour(*wxBLUE);
	CreateItemLabel = new wxStaticText(CreateArea, wxID_ANY, "Item Name",
		wxPoint(50, 80), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);

	static wxTextCtrl* CreateQuantityInput = new wxTextCtrl(CreateArea, wxID_ANY, "0", wxPoint(5, 140), wxSize(190, 20));
	CreateQuantityInput->SetBackgroundColour(*wxBLUE);
	CreateQuantityLabel = new wxStaticText(CreateArea, wxID_ANY, "Item Quantity",
		wxPoint(50, 120), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);

	createButton = new wxButton(CreateArea, wxID_ANY, "Create", wxPoint(75, 170), wxSize(50, 25));
	createButton->SetBackgroundColour(*wxBLUE);
	createButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {

		//Grab Values
	    //---------------------------------------------------------------------
		string EnteredCreateID = (CreateIDInput->GetValue()).ToStdString();
		int EnteredCreateQuantity = wxAtoi(CreateQuantityInput->GetValue());
		string EnteredCreateItem = (CreateItemInput->GetValue()).ToStdString();
		//---------------------------------------------------------------------

		CreateButtonOnClick(EnteredCreateID, EnteredCreateQuantity, EnteredCreateItem, GlobalSQLPassword);

		//Resets the Treeview Table to empty for refilling
		sellBasicListView->DeleteAllItems();
		basicListView->DeleteAllItems();
		analysisBasicListView->DeleteAllItems();

		//Loops through the database and inserts all the current data into the treeview table
		StartTreeview(SellTreeviewTable, sellBasicListView);
		StartTreeview(TreeviewTable, basicListView);
		StartTreeview(AnalysisTreeviewTable, analysisBasicListView);
		});

	//This will allow the user to delete stock
	//-----------------------------------------------------------------------------------------------------
	DeleteArea = new wxPanel(StockActiveArea, wxID_ANY, wxPoint(20, 250), wxSize(200, 200));
	DeleteArea->SetBackgroundColour(*wxWHITE);
	DeleteHeading = new wxStaticText(DeleteArea, wxID_ANY, "Delete Item",
		wxPoint(50, 5), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);
	DeleteHeading->SetFont(SectionHeadingFont);

	static wxTextCtrl* DeleteInput = new wxTextCtrl(DeleteArea, wxID_ANY, "", wxPoint(5, 100), wxSize(190, 20));
	DeleteInput->SetBackgroundColour(*wxBLUE);
	DeleteInputLabel = new wxStaticText(DeleteArea, wxID_ANY, "Item ID",
		wxPoint(50, 80), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);

	DeleteButton = new wxButton(DeleteArea, wxID_ANY, "Delete", wxPoint(75, 140), wxSize(50, 25));
	DeleteButton->SetBackgroundColour(*wxBLUE);
	DeleteButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {

		//Grab Value
		//---------------------------------------------------------------------
		string EnteredDeleteID = (DeleteInput->GetValue()).ToStdString();
		//---------------------------------------------------------------------

		DeleteButtonOnClick(EnteredDeleteID, GlobalSQLPassword);

		//Resets the Treeview Table to empty for refilling
		sellBasicListView->DeleteAllItems();
		basicListView->DeleteAllItems();
		analysisBasicListView->DeleteAllItems();

		//Loops through the database and inserts all the current data into the treeview table
		StartTreeview(SellTreeviewTable, sellBasicListView);
		StartTreeview(TreeviewTable, basicListView);
		StartTreeview(AnalysisTreeviewTable, analysisBasicListView);
		});
	//-----------------------------------------------------------------------------------------------------

	//This will allow the user to update existing stock
	//-----------------------------------------------------------------------------------------------------
	UpdateArea = new wxPanel(StockActiveArea, wxID_ANY, wxPoint(230, 250), wxSize(200, 200));
	UpdateArea->SetBackgroundColour(*wxWHITE);
	UpdateHeading = new wxStaticText(UpdateArea, wxID_ANY, "Update Item",
		wxPoint(50, 5), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);
	UpdateHeading->SetFont(SectionHeadingFont);

	static wxTextCtrl* UpdateIDInput = new wxTextCtrl(UpdateArea, wxID_ANY, "", wxPoint(5, 60), wxSize(190, 20));
	UpdateIDInput->SetBackgroundColour(*wxBLUE);
	UpdateIDLabel = new wxStaticText(UpdateArea, wxID_ANY, "Item ID",
		wxPoint(50, 40), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);

	static wxTextCtrl* UpdateItemInput = new wxTextCtrl(UpdateArea, wxID_ANY, "", wxPoint(5, 100), wxSize(190, 20));
	UpdateItemInput->SetBackgroundColour(*wxBLUE);
	UpdateItemLabel = new wxStaticText(UpdateArea, wxID_ANY, "Item Name",
		wxPoint(50, 80), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);

	static wxTextCtrl* UpdateQuantityInput = new wxTextCtrl(UpdateArea, wxID_ANY, "", wxPoint(5, 140), wxSize(190, 20));
	UpdateQuantityInput->SetBackgroundColour(*wxBLUE);
	UpdateQuantityLabel = new wxStaticText(UpdateArea, wxID_ANY, "Item Quantity",
		wxPoint(50, 120), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);

	UpdateButton = new wxButton(UpdateArea, wxID_ANY, "Update", wxPoint(75, 170), wxSize(50, 25));
	UpdateButton->SetBackgroundColour(*wxBLUE);
	UpdateButton->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {

		//Grab Values
		//---------------------------------------------------------------------
		string EnteredUpdateID = (UpdateIDInput->GetValue()).ToStdString();
		int EnteredUpdateQuantity = wxAtoi(UpdateQuantityInput->GetValue());
		string EnteredUpdateItem = (UpdateItemInput->GetValue()).ToStdString();
		//---------------------------------------------------------------------

		UpdateButtonOnClick(EnteredUpdateID, EnteredUpdateQuantity, EnteredUpdateItem, GlobalSQLPassword);

		//Resets the Treeview Table to empty for refilling
		sellBasicListView->DeleteAllItems();
		basicListView->DeleteAllItems();
		analysisBasicListView->DeleteAllItems();

		//Loops through the database and inserts all the current data into the treeview table
		StartTreeview(SellTreeviewTable, sellBasicListView);
		StartTreeview(TreeviewTable, basicListView);
		StartTreeview(AnalysisTreeviewTable, analysisBasicListView);
		});
	//-----------------------------------------------------------------------------------------------------


	//Sell System
	//--------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------

	//This will display all the stock to the user in a user friendly Treeview Table
	//-----------------------------------------------------------------------------------------------------

	sellBasicListView->AppendColumn("ItemName");
	sellBasicListView->AppendColumn("ItemQuantity");
	sellBasicListView->AppendColumn("ItemID");
	sellBasicListView->SetColumnWidth(0, 133);
	sellBasicListView->SetColumnWidth(1, 133);
	sellBasicListView->SetColumnWidth(2, 133);

	//This will allow the user to Sell stock
	//-----------------------------------------------------------------------------------------------------
	SellArea = new wxPanel(SellActiveArea, wxID_ANY, wxPoint(125, 120), wxSize(200, 200));
	SellArea->SetBackgroundColour(*wxWHITE);
	SellHeading = new wxStaticText(SellArea, wxID_ANY, "Sell Item",
		wxPoint(50, 5), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);
	SellHeading->SetFont(SectionHeadingFont);

	static wxTextCtrl* SellIDInput = new wxTextCtrl(SellArea, wxID_ANY, "", wxPoint(5, 60), wxSize(190, 20));
	SellIDInput->SetBackgroundColour(*wxBLUE);
	SellIDLabel = new wxStaticText(SellArea, wxID_ANY, "Item ID",
		wxPoint(50, 40), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);

	static wxTextCtrl* SellQuantityInput = new wxTextCtrl(SellArea, wxID_ANY, "", wxPoint(5, 140), wxSize(190, 20));
	SellQuantityInput->SetBackgroundColour(*wxBLUE);
	SellQuantityLabel = new wxStaticText(SellArea, wxID_ANY, "Item Quantity",
		wxPoint(50, 120), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);

	SellButton = new wxButton(SellArea, wxID_ANY, "Sell", wxPoint(75, 170), wxSize(50, 25));
	SellButton->SetBackgroundColour(*wxBLUE);
	SellButton->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {

		//Grab Values
		//---------------------------------------------------------------------
		std::string EnteredSellID = (SellIDInput->GetValue()).ToStdString();
		int EnteredSellQuantity = wxAtoi(SellQuantityInput->GetValue());
		//---------------------------------------------------------------------

		SellButtonOnClick(EnteredSellID, EnteredSellQuantity, GlobalSQLPassword, UserLatitude, UserLongitude);

		//Resets the Treeview Table to empty for refilling
		sellBasicListView->DeleteAllItems();
		basicListView->DeleteAllItems();
		analysisBasicListView->DeleteAllItems();

		//Loops through the database and inserts all the current data into the treeview table
		StartTreeview(SellTreeviewTable, sellBasicListView);
		StartTreeview(TreeviewTable, basicListView);
		StartTreeview(AnalysisTreeviewTable, analysisBasicListView);
		});
	//-----------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------



	//Analysis Page
	//--------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------

	//WeatherAnalysisActiveArea (This is where the User analyses the item for Weather Purchasing Trends)
	//-----------------------------------------------------------------------------------------------------
	wxFont WeatherDisplayFont = wxFont();
	WeatherDisplayFont.SetPixelSize(wxSize(0, 48));
	wxFont WeatherValueDisplayFont = wxFont();
	WeatherValueDisplayFont.SetPixelSize(wxSize(0, 38));

	static wxStaticText* WeatherCurrentItemHeading = new wxStaticText(WeatherAnalysisActiveArea, wxID_ANY, "Blueberries",
		wxPoint(340, 0), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);
	WeatherCurrentItemHeading->SetFont(WeatherValueDisplayFont);
	WeatherCurrentItemHeading->SetBackgroundColour(*wxWHITE);

	//display the numbers for each weather in here
	WeatherTrendsArea = new wxPanel(WeatherAnalysisActiveArea, wxID_ANY, wxPoint(50, 50), wxSize(750, 400));
	WeatherTrendsArea->SetBackgroundColour(*wxWHITE);

	CloudsHeading = new wxStaticText(WeatherTrendsArea, wxID_ANY, "Clouds",
		wxPoint(50, 20), wxSize(100, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Clouds = new wxStaticText(WeatherTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(50, 90), wxSize(100, 50), wxALIGN_CENTER_HORIZONTAL);
	CloudsHeading->SetFont(WeatherDisplayFont);
	Clouds->SetFont(WeatherValueDisplayFont);

	ClearHeading = new wxStaticText(WeatherTrendsArea, wxID_ANY, "Clear",
		wxPoint(240, 20), wxSize(100, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Clear = new wxStaticText(WeatherTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(240, 90), wxSize(100, 50), wxALIGN_CENTER_HORIZONTAL);
	ClearHeading->SetFont(WeatherDisplayFont);
	Clear->SetFont(WeatherValueDisplayFont);

	RainHeading = new wxStaticText(WeatherTrendsArea, wxID_ANY, "Rain",
		wxPoint(420, 20), wxSize(100, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Rain = new wxStaticText(WeatherTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(420, 90), wxSize(100, 50), wxALIGN_CENTER_HORIZONTAL);
	RainHeading->SetFont(WeatherDisplayFont);
	Rain->SetFont(WeatherValueDisplayFont);

	SnowHeading = new wxStaticText(WeatherTrendsArea, wxID_ANY, "Snow",
		wxPoint(580, 20), wxSize(100, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Snow = new wxStaticText(WeatherTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(580, 90), wxSize(100, 50), wxALIGN_CENTER_HORIZONTAL);
	SnowHeading->SetFont(WeatherDisplayFont);
	Snow->SetFont(WeatherValueDisplayFont);

	AtmosphereHeading = new wxStaticText(WeatherTrendsArea, wxID_ANY, "Atmosphere",
		wxPoint(80, 160), wxSize(100, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Atmosphere = new wxStaticText(WeatherTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(80, 230), wxSize(100, 50), wxALIGN_CENTER_HORIZONTAL);
	AtmosphereHeading->SetFont(WeatherDisplayFont);
	Atmosphere->SetFont(WeatherValueDisplayFont);

	ThunderstormHeading = new wxStaticText(WeatherTrendsArea, wxID_ANY, "Thunderstorm",
		wxPoint(400, 160), wxSize(100, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Thunderstorm = new wxStaticText(WeatherTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(400, 230), wxSize(100, 50), wxALIGN_CENTER_HORIZONTAL);
	ThunderstormHeading->SetFont(WeatherDisplayFont);
	Thunderstorm->SetFont(WeatherValueDisplayFont);

	DrizzleHeading = new wxStaticText(WeatherTrendsArea, wxID_ANY, "Drizzle",
		wxPoint(290, 280), wxSize(100, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Drizzle = new wxStaticText(WeatherTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(290, 350), wxSize(100, 50), wxALIGN_CENTER_HORIZONTAL);
	DrizzleHeading->SetFont(WeatherDisplayFont);
	Drizzle->SetFont(WeatherValueDisplayFont);
	//-----------------------------------------------------------------------------------------------------

	//TimeAnalysisActiveArea (This is where the User analyses the item for Time Purchasing Trends)
	//-----------------------------------------------------------------------------------------------------
	wxFont TimeDisplayFont = wxFont();
	TimeDisplayFont.SetPixelSize(wxSize(0, 28));
	wxFont TimeValueDisplayFont = wxFont();
	TimeValueDisplayFont.SetPixelSize(wxSize(0, 18));

	//display the numbers for each hour in here
	TimeTrendsArea = new wxPanel(TimeAnalysisActiveArea, wxID_ANY, wxPoint(50, 50), wxSize(750, 400));
	TimeTrendsArea->SetBackgroundColour(*wxWHITE);

	static wxStaticText* TimeCurrentItemHeading = new wxStaticText(TimeAnalysisActiveArea, wxID_ANY, "Blueberries",
		wxPoint(340, 0), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);
	TimeCurrentItemHeading->SetFont(WeatherValueDisplayFont);
	TimeCurrentItemHeading->SetBackgroundColour(*wxWHITE);

	Time00Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "00:00-00:59",
		wxPoint(20, 0), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time00 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(20, 30), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time00Heading->SetFont(TimeDisplayFont);
	Time00->SetFont(TimeValueDisplayFont);

	Time01Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "01:00-01:59",
		wxPoint(200, 0), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time01 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(200, 30), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time01Heading->SetFont(TimeDisplayFont);
	Time01->SetFont(TimeValueDisplayFont);

	Time02Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "02:00-02:59",
		wxPoint(380, 0), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time02 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(380, 30), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time02Heading->SetFont(TimeDisplayFont);
	Time02->SetFont(TimeValueDisplayFont);

	Time03Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "03:00-03:59",
		wxPoint(560, 0), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time03 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(560, 30), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time03Heading->SetFont(TimeDisplayFont);
	Time03->SetFont(TimeValueDisplayFont);

	Time04Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "04:00-04:59",
		wxPoint(20, 60), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time04 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(20, 90), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time04Heading->SetFont(TimeDisplayFont);
	Time04->SetFont(TimeValueDisplayFont);

	Time05Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "05:00-05:59",
		wxPoint(200, 60), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time05 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(200, 90), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time05Heading->SetFont(TimeDisplayFont);
	Time05->SetFont(TimeValueDisplayFont);

	Time06Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "06:00-06:59",
		wxPoint(380, 60), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time06 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(380, 90), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time06Heading->SetFont(TimeDisplayFont);
	Time06->SetFont(TimeValueDisplayFont);

	Time07Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "07:00-07:59",
		wxPoint(560, 60), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time07 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(560, 90), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time07Heading->SetFont(TimeDisplayFont);
	Time07->SetFont(TimeValueDisplayFont);

	Time08Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "08:00-08:59",
		wxPoint(20, 130), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time08 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(20, 160), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time08Heading->SetFont(TimeDisplayFont);
	Time08->SetFont(TimeValueDisplayFont);

	Time09Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "09:00-09:59",
		wxPoint(200, 130), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time09 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(200, 160), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time09Heading->SetFont(TimeDisplayFont);
	Time09->SetFont(TimeValueDisplayFont);

	Time10Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "10:00-10:59",
		wxPoint(380, 130), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time10 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(380, 160), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time10Heading->SetFont(TimeDisplayFont);
	Time10->SetFont(TimeValueDisplayFont);

	Time11Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "11:00-11:59",
		wxPoint(560, 130), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time11 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(560, 160), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time11Heading->SetFont(TimeDisplayFont);
	Time11->SetFont(TimeValueDisplayFont);

	Time12Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "12:00-12:59",
		wxPoint(20, 200), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time12 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(20, 230), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time12Heading->SetFont(TimeDisplayFont);
	Time12->SetFont(TimeValueDisplayFont);

	Time13Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "13:00-13:59",
		wxPoint(200, 200), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time13 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(200, 230), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time13Heading->SetFont(TimeDisplayFont);
	Time13->SetFont(TimeValueDisplayFont);

	Time14Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "14:00-14:59",
		wxPoint(380, 200), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time14 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(380, 230), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time14Heading->SetFont(TimeDisplayFont);
	Time14->SetFont(TimeValueDisplayFont);

	Time15Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "15:00-15:59",
		wxPoint(560, 200), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time15 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(560, 230), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time15Heading->SetFont(TimeDisplayFont);
	Time15->SetFont(TimeValueDisplayFont);

	Time16Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "16:00-16:59",
		wxPoint(20, 270), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time16 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(20, 300), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time16Heading->SetFont(TimeDisplayFont);
	Time16->SetFont(TimeValueDisplayFont);

	Time17Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "17:00-17:59",
		wxPoint(200, 270), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time17 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(200, 300), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time17Heading->SetFont(TimeDisplayFont);
	Time17->SetFont(TimeValueDisplayFont);

	Time18Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "18:00-18:59",
		wxPoint(380, 270), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time18 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(380, 300), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time18Heading->SetFont(TimeDisplayFont);
	Time18->SetFont(TimeValueDisplayFont);

	Time19Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "19:00-19:59",
		wxPoint(560, 270), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time19 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(560, 300), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time19Heading->SetFont(TimeDisplayFont);
	Time19->SetFont(TimeValueDisplayFont);

	Time20Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "20:00-20:59",
		wxPoint(20, 340), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time20 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(20, 370), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time20Heading->SetFont(TimeDisplayFont);
	Time20->SetFont(TimeValueDisplayFont);

	Time21Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "21:00-21:59",
		wxPoint(200, 340), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time21 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(200, 370), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time21Heading->SetFont(TimeDisplayFont);
	Time21->SetFont(TimeValueDisplayFont);

	Time22Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "22:00-22:59",
		wxPoint(380, 340), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time22 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(380, 370), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time22Heading->SetFont(TimeDisplayFont);
	Time22->SetFont(TimeValueDisplayFont);

	Time23Heading = new wxStaticText(TimeTrendsArea, wxID_ANY, "23:00-23:59",
		wxPoint(560, 340), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	static wxStaticText* Time23 = new wxStaticText(TimeTrendsArea, wxID_ANY, "0 Sold",
		wxPoint(560, 370), wxSize(54, 50), wxALIGN_CENTER_HORIZONTAL);
	Time23Heading->SetFont(TimeDisplayFont);
	Time23->SetFont(TimeValueDisplayFont);
	//-----------------------------------------------------------------------------------------------------

	//This will display all the stock to the user in a user friendly Treeview Table
	//-----------------------------------------------------------------------------------------------------
	analysisBasicListView->AppendColumn("ItemName");
	analysisBasicListView->AppendColumn("ItemQuantity");
	analysisBasicListView->AppendColumn("ItemID");
	analysisBasicListView->SetColumnWidth(0, 133);
	analysisBasicListView->SetColumnWidth(1, 133);
	analysisBasicListView->SetColumnWidth(2, 133);
	//-----------------------------------------------------------------------------------------------------

	//This will allow the user to Analyse stock trends
	//-----------------------------------------------------------------------------------------------------
	AnalysisArea = new wxPanel(AnalysisActiveArea, wxID_ANY, wxPoint(125, 120), wxSize(200, 200));
	AnalysisArea->SetBackgroundColour(*wxWHITE);
	AnalysisHeading = new wxStaticText(AnalysisArea, wxID_ANY, "Analyse Item",
		wxPoint(50, 5), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);
	AnalysisHeading->SetFont(SectionHeadingFont);

	static wxTextCtrl* AnalysisIDInput = new wxTextCtrl(AnalysisArea, wxID_ANY, "", wxPoint(5, 60), wxSize(190, 20));
	SellIDInput->SetBackgroundColour(*wxBLUE);
	AnalysisIDLabel = new wxStaticText(AnalysisArea, wxID_ANY, "Item ID",
		wxPoint(50, 40), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);

	AnalysisButton = new wxButton(AnalysisArea, wxID_ANY, "Analyse", wxPoint(75, 170), wxSize(50, 25));
	AnalysisButton->SetBackgroundColour(*wxBLUE);
	AnalysisButton->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {

		//Grab Values
		//---------------------------------------------------------------------
		std::string EnteredAnalysisID = (AnalysisIDInput->GetValue()).ToStdString();
		//---------------------------------------------------------------------

		std::vector<std::variant<int, std::string>> emptyvariant;
		std::vector<std::variant<int, std::string>> CheckAnalysis = AnalysisButtonOnClick(EnteredAnalysisID, GlobalSQLPassword);

		if (CheckAnalysis != emptyvariant)
		{
			AnalysisActiveArea->Show(false);
			AnalysisChooseArea->Show(true);

			//Assigns ItemName for each Analysis option
			WeatherCurrentItemHeading->SetLabel(*std::get_if<string>(&CheckAnalysis[0]));
			TimeCurrentItemHeading->SetLabel(*std::get_if<string>(&CheckAnalysis[0]));

			//Assigns all number of products sold each weather in Weather Analysis
			Clouds->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[1])));
			Clear->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[2])));
			Atmosphere->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[3])));
			Snow->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[4])));
			Rain->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[5])));
			Drizzle->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[6])));
			Thunderstorm->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[7])));

			//Assigns all number of products sold per each hour in Time Analysis
			Time00->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[8])));
			Time01->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[9])));
			Time02->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[10])));
			Time03->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[11])));
			Time04->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[12])));
			Time05->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[13])));
			Time06->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[14])));
			Time07->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[15])));
			Time08->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[16])));
			Time09->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[17])));
			Time10->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[18])));
			Time11->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[19])));
			Time12->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[20])));
			Time13->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[21])));
			Time14->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[22])));
			Time15->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[23])));
			Time16->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[24])));
			Time17->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[25])));
			Time18->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[26])));
			Time19->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[27])));
			Time20->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[28])));
			Time21->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[29])));
			Time22->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[30])));
			Time23->SetLabel(wxString::Format("%d Sold", *std::get_if<int>(&CheckAnalysis[31])));
		}
		});
	//-----------------------------------------------------------------------------------------------------

	//AnalysisChooseArea (This is where the user picks which metric they want to analyse the item on)
	//-----------------------------------------------------------------------------------------------------
	wxFont AnalysisButtonsFont = wxFont();
	AnalysisButtonsFont.SetPixelSize(wxSize(0, 24));

	WeatherAnalysisButton = new wxButton(AnalysisChooseArea, wxID_ANY, "Weather", wxPoint(200, 170), wxSize(150, 150));
	WeatherAnalysisButton->SetBackgroundColour(*wxBLUE);
	WeatherAnalysisButton->SetFont(AnalysisButtonsFont);
	WeatherAnalysisButton->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {
		AnalysisChooseArea->Show(false);
		WeatherAnalysisActiveArea->Show(true);
		});

	TimeAnalysisButton = new wxButton(AnalysisChooseArea, wxID_ANY, "Time", wxPoint(500, 170), wxSize(150, 150));
	TimeAnalysisButton->SetBackgroundColour(*wxBLUE);
	TimeAnalysisButton->SetFont(AnalysisButtonsFont);
	TimeAnalysisButton->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {
		AnalysisChooseArea->Show(false);
		TimeAnalysisActiveArea->Show(true);
		});
	//-----------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------


	//Owner Unlock Page
	//--------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------
	wxFont OwnerUnlockSectionHeadingFont(wxFontInfo(wxSize(0, 20)).Bold());
	OwnerUnlockDetailsArea = new wxPanel(OwnerUnlockArea, wxID_ANY, wxPoint(400, 125), wxSize(200, 200));
	OwnerUnlockDetailsArea->SetBackgroundColour(*wxWHITE);
	OwnerUnlockHeading = new wxStaticText(OwnerUnlockDetailsArea, wxID_ANY, "Shop Owner Unlock",
		wxPoint(5, 5), wxSize(100, 50), wxALIGN_CENTER_HORIZONTAL);
	OwnerUnlockHeading->SetFont(OwnerUnlockSectionHeadingFont);

	static wxTextCtrl* SQLPasswordInput = new wxTextCtrl(OwnerUnlockDetailsArea, wxID_ANY, "", wxPoint(5, 90), wxSize(190, 20));
	SQLPasswordInput->SetBackgroundColour(*wxBLUE);
	SQLPasswordLabel = new wxStaticText(OwnerUnlockDetailsArea, wxID_ANY, "SQL Password",
		wxPoint(50, 70), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);

	OwnerUnlockButton = new wxButton(OwnerUnlockDetailsArea, wxID_ANY, "Unlock", wxPoint(75, 170), wxSize(50, 25));
	OwnerUnlockButton->SetBackgroundColour(*wxBLUE);
	OwnerUnlockButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {

		//Grab Value
		//---------------------------------------------------------------------
		std::string SQLPassword = (SQLPasswordInput->GetValue()).ToStdString();
		//---------------------------------------------------------------------

		//Establishes a connection with the MySQL Database
		//-----------------------------------------------------------------------------------------------------
		const std::string newserver = "tcp://managestock.mysql.database.azure.com:3306";
		const std::string newusername = "rootConnect";
		const std::string newpassword = SQLPassword;
		GlobalSQLPassword = SQLPassword;

		sql::Driver* driver;
		sql::Connection* con = nullptr;

		try
		{
			driver = get_driver_instance();
			con = driver->connect(newserver, newusername, newpassword);
		}
		catch (sql::SQLException e)
		{
			//Could not connect to server.
			delete con;
			return;
		}

		delete con;
		//-----------------------------------------------------------------------------------------------------

		//Loops through the database and inserts all the current data into the treeview table
		StartTreeview(SellTreeviewTable, sellBasicListView);
		StartTreeview(TreeviewTable, basicListView);
		StartTreeview(AnalysisTreeviewTable, analysisBasicListView);

		//Unlocks the Program
		OwnerUnlockArea->Show(false);
		NavigationBar->Show(true);
		StockActiveArea->Show(true);

		//Grabs the User's Latitude and Longitude using a HTTP request to the ipapi API
		//-----------------------------------------------------------------------------------------------------
		CURL* curl = curl_easy_init();
		CURLcode ipapiResult;
		std::string ipapiApiKey = getenv("ipapiAPIKey");

		std::string ipapiurl = "https://ipapi.co/json/?key=" + ipapiApiKey;

		if (curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, ipapiurl.c_str());
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

			std::string response;

			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

			ipapiResult = curl_easy_perform(curl);
			if (ipapiResult != CURLE_OK)
			{
				std::string error_message = "Connection to ipapi failed.";
				throw std::runtime_error(error_message);
			}

			nlohmann::json j = nlohmann::json::parse(response);
			UserLatitude = j["latitude"].dump();
			UserLongitude = j["longitude"].dump();

			curl_easy_cleanup(curl);
		}
		//-----------------------------------------------------------------------------------------------------

		});

	//--------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------

}


