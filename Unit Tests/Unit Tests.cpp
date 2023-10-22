#include "pch.h"
#include "CppUnitTest.h"

#include "../MainFrame.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "../stdafx.h"
#include <cstdlib>
#include <wx/string.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

//AN ENVIRONMENT VARIABLE IS USED SO THE UNIT TEST CODE CAN BE PUSHED TO GITHUB
static std::string GlobalSQLPassword = std::getenv("InvTrendPASSWORD"); //PASSWORD IS STORED IN ENVIRONMENT VARIABLE FOR PRIVACY REASONS

bool CheckDatabase(string CheckID, int CheckQuantity, string CheckItem)
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

	bool IsSame = false;

	pstmt = con->prepareStatement("SELECT * FROM itemtable WHERE ItemID = ? AND ItemQuantity = ? AND ItemName = ?");
	pstmt->setString(1, CheckID);
	pstmt->setInt(2, CheckQuantity);
	pstmt->setString(3, CheckItem);
	result = pstmt->executeQuery();

	while (result->next())
	{
		string ResultID = result->getString("ItemID").c_str();
		int ResultInt = result->getInt("ItemQuantity");
		string ResultItem = result->getString("ItemName").c_str();
		if (ResultID == CheckID && ResultInt == CheckQuantity && ResultItem == CheckItem)
		{
			IsSame = true;
			return true;
			break;
		}
	}

	delete pstmt;
	delete con;
	delete result;

	if (IsSame == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void QuickEditDB(string ChangeState, string TestIDInput, int TestQuantityInput, string TestItemInput)
{
	//Establishes a connection with the MySQL Database
	//-----------------------------------------------------------------------------------------------------
	const std::string server = "tcp://managestock.mysql.database.azure.com:3306";
	const std::string username = "rootConnect";
	const std::string password = GlobalSQLPassword;

	sql::Driver* driver;
	sql::Connection* con;
	sql::PreparedStatement* pstmt = nullptr;

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

	if (ChangeState == "CREATE")
	{
		pstmt = con->prepareStatement("INSERT INTO itemtable(ItemName, ItemQuantity, ItemID) VALUES(?,?,?)");
		pstmt->setString(1, TestItemInput);
		pstmt->setInt(2, TestQuantityInput);
		pstmt->setString(3, TestIDInput);
		pstmt->executeQuery();
	}
	if (ChangeState == "UPDATE")
	{
		pstmt = con->prepareStatement("UPDATE itemtable SET ItemName = ?, ItemQuantity = ? WHERE ItemID = ?");
		pstmt->setString(1, TestItemInput);
		pstmt->setInt(2, TestQuantityInput);
		pstmt->setString(3, TestIDInput);
		pstmt->executeQuery();
	}
	if (ChangeState == "DELETE")
	{
		pstmt = con->prepareStatement("DELETE FROM itemtable WHERE ItemID = ?");
		pstmt->setString(1, TestIDInput);
		pstmt->executeQuery();
	}

	delete pstmt;
	delete con;
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	public:

		TEST_METHOD(EnvironmentVariablePasswordCheck) //TESTS THAT THE ENVIRONMENT VARIABLE FOR THE PASSWORD IS WORKING
		{
			bool CheckPass = true;
			char* CheckedPass = std::getenv("InvTrendPASSWORD");

			if (static_cast<void*>(CheckedPass) == nullptr) {
				CheckPass = false;
			}

			Assert::AreEqual(true, CheckPass);
		}

		TEST_METHOD(StockManagementCreateFunctionWorks)
		{
			
			string TestCreateIDInput = "T3S701";
			int TestCreateQuantityInput = 8;
			string TestCreateItemInput = "UnitTest0001";

			MainFrame::CreateButtonOnClick(TestCreateIDInput, TestCreateQuantityInput, TestCreateItemInput, GlobalSQLPassword);

			bool CheckData = CheckDatabase(TestCreateIDInput, TestCreateQuantityInput, TestCreateItemInput);
			Assert::AreEqual(true, CheckData); //If CheckData is true then the test will pass
			
			QuickEditDB("DELETE", TestCreateIDInput, TestCreateQuantityInput, TestCreateItemInput);
		}

		TEST_METHOD(StockManagementUpdateFunctionWorks)
		{

			string TestUpdateIDInput = "T3S701";
			int TestUpdateQuantityInput = 8;
			string TestUpdateItemInput = "UnitTest0001";
			QuickEditDB("CREATE", TestUpdateIDInput, TestUpdateQuantityInput, TestUpdateItemInput);

			int ChangeTestUpdateQuantityInput = 5;
			string ChangeTestUpdateItemInput = "UnitTest0002";

			MainFrame::UpdateButtonOnClick(TestUpdateIDInput, ChangeTestUpdateQuantityInput, ChangeTestUpdateItemInput, GlobalSQLPassword);

			bool CheckData = CheckDatabase(TestUpdateIDInput, ChangeTestUpdateQuantityInput, ChangeTestUpdateItemInput);
			Assert::AreEqual(true, CheckData); //If CheckData is true then the test will pass

			QuickEditDB("DELETE", TestUpdateIDInput, TestUpdateQuantityInput, TestUpdateItemInput);
		}

		TEST_METHOD(StockManagementDeleteFunctionWorks)
		{

			string TestDeleteIDInput = "T3S701";
			int TestDeleteQuantityInput = 8;
			string TestDeleteItemInput = "UnitTest0001";

			QuickEditDB("CREATE", TestDeleteIDInput, TestDeleteQuantityInput, TestDeleteItemInput);

			MainFrame::DeleteButtonOnClick(TestDeleteIDInput, GlobalSQLPassword);

			bool CheckData = CheckDatabase(TestDeleteIDInput, TestDeleteQuantityInput, TestDeleteItemInput);
			Assert::AreEqual(false, CheckData); //If CheckData is false then the test will pass

		}

		TEST_METHOD(SellSystemFunctionWorks)
		{

			string TestSellIDInput = "T3S701";
			int TestSellQuantityInput = 8;
			string TestSellItemInput = "UnitTest0001";

			int ToSell = 2;
			int NewQuantity = 6;

			QuickEditDB("CREATE", TestSellIDInput, TestSellQuantityInput, TestSellItemInput);

			MainFrame::SellButtonOnClick(TestSellIDInput, ToSell, GlobalSQLPassword);

			bool CheckData = CheckDatabase(TestSellIDInput, NewQuantity, TestSellItemInput);
			Assert::AreEqual(true, CheckData); //If CheckData is true then the test will pass

			QuickEditDB("DELETE", TestSellIDInput, TestSellQuantityInput, TestSellItemInput);
		}
	};
}
