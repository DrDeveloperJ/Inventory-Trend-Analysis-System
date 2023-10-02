#include "MainFrame.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "stdafx.h"
#include <cstdlib>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	CreateOptions();
}

wxListView* MainFrame::basicListView = nullptr;

//Loops through the database and inserts all the current data into the treeview table
//-----------------------------------------------------------------------------------------------------
void StartTreeview(wxPanel*& TreeviewTable, wxListView*& basicListView)
{
	int YPos{ 0 };

	//Establishes a connection with the MySQL Database
	//-----------------------------------------------------------------------------------------------------
	const std::string server = "tcp://127.0.0.1:3306";
	const std::string username = "root";
	const std::string password = "";

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

	//StockActive is the Stock Management page
	static wxPanel* StockActiveArea = new wxPanel(InteractiveArea, wxID_ANY, wxPoint(150, 0), wxSize(850, 500));
	StockActiveArea->SetBackgroundColour(*wxGREEN);
	//AnalysisActiveArea is the Analysis page
	static wxPanel* AnalysisActiveArea = new wxPanel(InteractiveArea, wxID_ANY, wxPoint(150, 0), wxSize(850, 500));
	AnalysisActiveArea->SetBackgroundColour(*wxGREEN);
	AnalysisActiveArea->Show(false);
	//StockActive area is the SellSystem page
	static wxPanel* SellActiveArea = new wxPanel(InteractiveArea, wxID_ANY, wxPoint(150, 0), wxSize(850, 500));
	SellActiveArea->SetBackgroundColour(*wxGREEN);
	SellActiveArea->Show(false);

	//Navigation Bar for navigating between pages
	//-----------------------------------------------------------------------------------------------------
	NavigationBar = new wxPanel(InteractiveArea, wxID_ANY, wxPoint(0, 0), wxSize(150, 500));
	NavigationBar->SetBackgroundColour(*wxYELLOW);

	AnalysisPage = new wxButton(NavigationBar, wxID_ANY, "Sell System", wxPoint(25, 20), wxSize(100, 75));
	AnalysisPage->SetBackgroundColour(*wxBLUE);
	AnalysisPage->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {
		StockActiveArea->Show(false);
		SellActiveArea->Show(false);
		AnalysisActiveArea->Show(true);
		headingText->SetLabel(wxT("Analysis Page"));
		headingText->Refresh();
		});
	StockManage = new wxButton(NavigationBar, wxID_ANY, "Stock Manage", wxPoint(25, 120), wxSize(100, 75));
	StockManage->SetBackgroundColour(*wxBLUE);
	StockManage->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {
		AnalysisActiveArea->Show(false);
		SellActiveArea->Show(false);
		StockActiveArea->Show(true);
		headingText->SetLabel(wxT("Stock Management Page"));
		headingText->Refresh();
		});
	SellSystem = new wxButton(NavigationBar, wxID_ANY, "Sell System", wxPoint(25, 220), wxSize(100, 75));
	SellSystem->SetBackgroundColour(*wxBLUE);
	SellSystem->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {
		AnalysisActiveArea->Show(false);
		StockActiveArea->Show(false);
		SellActiveArea->Show(true);
		headingText->SetLabel(wxT("Sell System"));
		headingText->Refresh();
		});
	//-----------------------------------------------------------------------------------------------------

	//This will display all the stock to the user in a user friendly Treeview Table
	static wxPanel* TreeviewTable = new wxPanel(StockActiveArea, wxID_ANY, wxPoint(440, 25), wxSize(400, 450));
	TreeviewTable->SetBackgroundColour(*wxWHITE);

	MainFrame:basicListView = new wxListView(TreeviewTable);

	basicListView->AppendColumn("ItemName");
	basicListView->AppendColumn("ItemQuantity");
	basicListView->AppendColumn("ItemID");
	basicListView->SetColumnWidth(0, 80);
	basicListView->SetColumnWidth(1, 80);
	basicListView->SetColumnWidth(2, 80);

	//Loops through the database and inserts all the current data into the treeview table
	StartTreeview(TreeviewTable, basicListView);

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
	createButton->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {

		//Establishes a connection with the MySQL Database
		//-----------------------------------------------------------------------------------------------------
		const std::string server = "tcp://127.0.0.1:3306";
		const std::string username = "root";
		const std::string password = "";

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
		//-----------------------------------------------------------------------------------------------------

		//Grab Values
		//---------------------------------------------------------------------
		wxString EnteredCreateID = CreateIDInput->GetValue();
		int EnteredCreateQuantity = wxAtoi(CreateQuantityInput->GetValue());
		wxString EnteredCreateItem = CreateItemInput->GetValue();
		//---------------------------------------------------------------------

		bool UniqueID = true;

		pstmt = con->prepareStatement("SELECT * FROM itemtable WHERE ItemID = ?");
		pstmt->setString(1, EnteredCreateID.ToStdString());
		result = pstmt->executeQuery();

		while (result->next())
		{
			string CheckID = result->getString(3).c_str();
			if (CheckID == EnteredCreateID)
			{
				UniqueID = false;
				break;
			}
		}

		if ((EnteredCreateQuantity >= 0) && (UniqueID == true))
		{
			pstmt = con->prepareStatement("INSERT INTO itemtable(ItemName, ItemQuantity, ItemID) VALUES(?,?,?)");
			pstmt->setString(1, EnteredCreateItem.ToStdString());
			pstmt->setInt(2, EnteredCreateQuantity);
			pstmt->setString(3, EnteredCreateID.ToStdString());
			pstmt->executeQuery();
		}

		delete pstmt;
		delete con;
		delete result;

		//Resets the Treeview Table to empty for refilling
		basicListView->DeleteAllItems();

		//Loops through the database and inserts all the current data into the treeview table
		StartTreeview(TreeviewTable, basicListView);
		});
	//-----------------------------------------------------------------------------------------------------

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
	DeleteButton->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {

		//Establishes a connection with the MySQL Database
		//-----------------------------------------------------------------------------------------------------
		const std::string server = "tcp://127.0.0.1:3306";
		const std::string username = "root";
		const std::string password = "";

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
		//-----------------------------------------------------------------------------------------------------

		//Grab Value
		//---------------------------------------------------------------------
		wxString EnteredDeleteID = DeleteInput->GetValue();
		//---------------------------------------------------------------------

		bool IDFound = false;

		pstmt = con->prepareStatement("SELECT * FROM itemtable WHERE ItemID = ?");
		pstmt->setString(1, EnteredDeleteID.ToStdString());
		result = pstmt->executeQuery();

		while (result->next())
		{
			string CheckID = result->getString(3).c_str();
			if (CheckID == EnteredDeleteID)
			{
				IDFound = true;
				break;
			}
		}

		if (IDFound == true)
		{
			pstmt = con->prepareStatement("DELETE FROM itemtable WHERE ItemID = ?");
			pstmt->setString(1, EnteredDeleteID.ToStdString());
			pstmt->executeQuery();
		}

		delete pstmt;
		delete con;
		delete result;

		//Resets the Treeview Table to empty for refilling
		basicListView->DeleteAllItems();

		//Loops through the database and inserts all the current data into the treeview table
		StartTreeview(TreeviewTable, basicListView);
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

		//Establishes a connection with the MySQL Database
		//-----------------------------------------------------------------------------------------------------
		const std::string server = "tcp://127.0.0.1:3306";
		const std::string username = "root";
		const std::string password = "";

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
		//-----------------------------------------------------------------------------------------------------

		//Grab Values
		//---------------------------------------------------------------------
		wxString EnteredUpdateID = UpdateIDInput->GetValue();
		int EnteredUpdateQuantity = wxAtoi(UpdateQuantityInput->GetValue());
		wxString EnteredUpdateItem = UpdateItemInput->GetValue();
		//---------------------------------------------------------------------

		bool IDFound = false;

		pstmt = con->prepareStatement("SELECT * FROM itemtable WHERE ItemID = ?");
		pstmt->setString(1, EnteredUpdateID.ToStdString());
		result = pstmt->executeQuery();

		while (result->next())
		{
			string CheckID = result->getString(3).c_str();
			if (CheckID == EnteredUpdateID)
			{
				IDFound = true;
				break;
			}
		}

		if ((EnteredUpdateQuantity >= 0) && (IDFound == true))
		{
			pstmt = con->prepareStatement("UPDATE itemtable SET ItemName = ?, ItemQuantity = ? WHERE ItemID = ?");
			pstmt->setString(1, EnteredUpdateItem.ToStdString());
			pstmt->setInt(2, EnteredUpdateQuantity);
			pstmt->setString(3, EnteredUpdateID.ToStdString());
			pstmt->executeQuery();
		}

		delete pstmt;
		delete con;
		delete result;

		//Resets the Treeview Table to empty for refilling
		basicListView->DeleteAllItems();

		//Loops through the database and inserts all the current data into the treeview table
		StartTreeview(TreeviewTable, basicListView);
		});
	//-----------------------------------------------------------------------------------------------------
}