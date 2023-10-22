#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "stdafx.h"
#include <cstdlib>
#include <wx/string.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
	inline static void CreateButtonOnClick(string& EnteredCreateID, int& EnteredCreateQuantity, string& EnteredCreateItem, string& GlobalSQLPassword);
	inline static void DeleteButtonOnClick(string& EnteredDeleteID, string& GlobalSQLPassword);
	inline static void UpdateButtonOnClick(string& EnteredUpdateID, int& EnteredUpdateQuantity, string& EnteredUpdateItem, string& GlobalSQLPassword);
	inline static void SellButtonOnClick(string& EnteredSellID, int& EnteredSellQuantity, string& GlobalSQLPassword);

private:
	void CreateOptions();

	wxPanel* MainPanel;
	wxStaticText* headingText;
	wxPanel* InteractiveArea;
	wxPanel* OwnerUnlockArea;
	wxPanel* NavigationBar;
	wxButton* StockManage;
	wxButton* SellSystem;
	wxButton* AnalysisPage;
	wxPanel* ActiveArea;

	static wxTextCtrl* SQLPasswordInput;
	wxStaticText* SQLPasswordLabel;
	wxPanel* OwnerUnlockDetailsArea;
	wxStaticText* OwnerUnlockHeading;
	wxButton* OwnerUnlockButton;

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

// Stock Management Function Definitions
// ------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------
inline void MainFrame::CreateButtonOnClick(string& EnteredCreateID, int& EnteredCreateQuantity, string& EnteredCreateItem, string& GlobalSQLPassword)
{
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
	//-----------------------------------------------------------------------------------------------------

	bool UniqueID = true;

	pstmt = con->prepareStatement("SELECT * FROM itemtable");
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
		pstmt->setString(1, EnteredCreateItem);
		pstmt->setInt(2, EnteredCreateQuantity);
		pstmt->setString(3, EnteredCreateID);
		pstmt->executeQuery();
	}

	delete pstmt;
	delete con;
	delete result;
}


inline void MainFrame::DeleteButtonOnClick(string& EnteredDeleteID, string& GlobalSQLPassword)
{
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
	//-----------------------------------------------------------------------------------------------------

	bool IDFound = false;

	pstmt = con->prepareStatement("SELECT * FROM itemtable WHERE ItemID = ?");
	pstmt->setString(1, EnteredDeleteID);
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
		pstmt->setString(1, EnteredDeleteID);
		pstmt->executeQuery();
	}

	delete pstmt;
	delete con;
	delete result;
}



inline void MainFrame::UpdateButtonOnClick(string& EnteredUpdateID, int& EnteredUpdateQuantity, string& EnteredUpdateItem, string& GlobalSQLPassword)
{
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
	//-----------------------------------------------------------------------------------------------------

	bool IDFound = false;

	pstmt = con->prepareStatement("SELECT * FROM itemtable WHERE ItemID = ?");
	pstmt->setString(1, EnteredUpdateID);
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
		pstmt->setString(1, EnteredUpdateItem);
		pstmt->setInt(2, EnteredUpdateQuantity);
		pstmt->setString(3, EnteredUpdateID);
		pstmt->executeQuery();
	}

	delete pstmt;
	delete con;
	delete result;
}

// ------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------


//Sell System Function Definitions
// ------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------

inline void MainFrame::SellButtonOnClick(string& EnteredSellID, int& EnteredSellQuantity, string& GlobalSQLPassword)
{
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
	//-----------------------------------------------------------------------------------------------------

	bool IDFound = false;

	pstmt = con->prepareStatement("SELECT * FROM itemtable WHERE ItemID = ?");
	pstmt->setString(1, EnteredSellID);
	result = pstmt->executeQuery();

	while (result->next())
	{
		string CheckID = result->getString(3).c_str();
		int FoundQuantity = result->getInt("ItemQuantity");
		if (CheckID == EnteredSellID)
		{
			IDFound = true;
			int NewQuantity = FoundQuantity - EnteredSellQuantity;

			if ((EnteredSellQuantity >= 0) && (IDFound == true))
			{
				pstmt = con->prepareStatement("UPDATE itemtable SET ItemQuantity = ? WHERE ItemID = ?");
				pstmt->setInt(1, NewQuantity);
				pstmt->setString(2, EnteredSellID);
				pstmt->executeQuery();
			}

			break;
		}
	}

	/*
	if ((EnteredSellQuantity >= 0) && (IDFound == true))
	{
		pstmt = con->prepareStatement("UPDATE itemtable SET ItemQuantity = ? WHERE ItemID = ?");
		pstmt->setInt(2, NewQuantity);
		pstmt->setString(3, EnteredSellID);
		pstmt->executeQuery();
	}
	*/

	delete pstmt;
	delete con;
	delete result;
}
// ------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------
