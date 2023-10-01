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
	//stmt->execute("DROP TABLE IF EXISTS inventory");
	//cout << "Finished dropping table (if existed)" << endl;

	//stmt->execute("CREATE TABLE inventory (id serial PRIMARY KEY, name VARCHAR(50), quantity INTEGER);");
	//out << "Finished creating table" << endl;

	/*
	pstmt = con->prepareStatement("INSERT INTO itemtable(ItemName, ItemQuantity, ItemID) VALUES(?, ?, ?)");
	pstmt->setString(1, "banana");
	pstmt->setInt(2, 150);
	pstmt->setString(2, "EH819");
	pstmt->execute();
	cout << "One row inserted." << endl;


	pstmt->setString(1, "orange");
	pstmt->setInt(2, 154);
	pstmt->setString(2, "O201JK");
	pstmt->execute();
	cout << "One row inserted." << endl;
	*/

	//Creates the Main Page as "MainPanel" and prepares a font for the page heading
	wxFont headingFont(wxFontInfo(wxSize(0, 36)).Bold());
	MainPanel = new wxPanel(this);
	MainPanel->SetBackgroundColour(*wxRED);

	//Creates a heading
	headingText = new wxStaticText(MainPanel, wxID_ANY, "Stock Management Page",
		wxPoint(0, 0), wxSize(1000, 100), wxALIGN_CENTER_HORIZONTAL);
	headingText->SetBackgroundColour(*wxBLUE);
	headingText->SetFont(headingFont);

	/*This defines the rest of the page which will include the navigation bar on the left and the rest is what will
	be interacted with by the user (this will be called ActiveArea) */
	InteractiveArea = new wxPanel(MainPanel, wxID_ANY, wxPoint(0, 100), wxSize(1000, 500));
	InteractiveArea->SetBackgroundColour(*wxGREEN);

	//Navigation Bar for navigating between pages
	//-----------------------------------------------------------------------------------------------------
	NavigationBar = new wxPanel(InteractiveArea, wxID_ANY, wxPoint(0, 0), wxSize(150, 500));
	NavigationBar->SetBackgroundColour(*wxYELLOW);
	StockManage = new wxButton(NavigationBar, wxID_ANY, "Stock Manage", wxPoint(25, 120), wxSize(100, 75));
	StockManage->SetBackgroundColour(*wxBLUE);
	//-----------------------------------------------------------------------------------------------------

	static wxPanel* ActiveArea = new wxPanel(InteractiveArea, wxID_ANY, wxPoint(150, 0), wxSize(850, 500));
	ActiveArea->SetBackgroundColour(*wxGREEN);

	//This will display all the stock to the user in a user friendly Treeview Table
	static wxPanel* TreeviewTable = new wxPanel(ActiveArea, wxID_ANY, wxPoint(440, 25), wxSize(400, 450));
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
	CreateArea = new wxPanel(ActiveArea, wxID_ANY, wxPoint(125, 30), wxSize(200, 200));
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
	DeleteArea = new wxPanel(ActiveArea, wxID_ANY, wxPoint(20, 250), wxSize(200, 200));
	DeleteArea->SetBackgroundColour(*wxWHITE);
	DeleteHeading = new wxStaticText(DeleteArea, wxID_ANY, "Delete Item",
		wxPoint(50, 5), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);
	DeleteHeading->SetFont(SectionHeadingFont);

	DeleteInput = new wxTextCtrl(DeleteArea, wxID_ANY, "", wxPoint(5, 100), wxSize(190, 20));
	DeleteInput->SetBackgroundColour(*wxBLUE);
	DeleteInputLabel = new wxStaticText(DeleteArea, wxID_ANY, "Item ID",
		wxPoint(50, 80), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);

	DeleteButton = new wxButton(DeleteArea, wxID_ANY, "Delete", wxPoint(75, 140), wxSize(50, 25));
	DeleteButton->SetBackgroundColour(*wxBLUE);
	//-----------------------------------------------------------------------------------------------------

	//This will allow the user to update existing stock
	//-----------------------------------------------------------------------------------------------------
	UpdateArea = new wxPanel(ActiveArea, wxID_ANY, wxPoint(230, 250), wxSize(200, 200));
	UpdateArea->SetBackgroundColour(*wxWHITE);
	UpdateHeading = new wxStaticText(UpdateArea, wxID_ANY, "Update Item",
		wxPoint(50, 5), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);
	UpdateHeading->SetFont(SectionHeadingFont);

	UpdateIDInput = new wxTextCtrl(UpdateArea, wxID_ANY, "", wxPoint(5, 60), wxSize(190, 20));
	UpdateIDInput->SetBackgroundColour(*wxBLUE);
	UpdateIDLabel = new wxStaticText(UpdateArea, wxID_ANY, "Item ID",
		wxPoint(50, 40), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);

	UpdateItemInput = new wxTextCtrl(UpdateArea, wxID_ANY, "", wxPoint(5, 100), wxSize(190, 20));
	UpdateItemInput->SetBackgroundColour(*wxBLUE);
	UpdateItemLabel = new wxStaticText(UpdateArea, wxID_ANY, "Item Name",
		wxPoint(50, 80), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);

	UpdateQuantityInput = new wxTextCtrl(UpdateArea, wxID_ANY, "", wxPoint(5, 140), wxSize(190, 20));
	UpdateQuantityInput->SetBackgroundColour(*wxBLUE);
	UpdateQuantityLabel = new wxStaticText(UpdateArea, wxID_ANY, "Item Quantity",
		wxPoint(50, 120), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);

	UpdateButton = new wxButton(UpdateArea, wxID_ANY, "Update", wxPoint(75, 170), wxSize(50, 25));
	UpdateButton->SetBackgroundColour(*wxBLUE);
	//-----------------------------------------------------------------------------------------------------
}