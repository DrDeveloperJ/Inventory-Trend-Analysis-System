#include "pch.h"
#include "CppUnitTest.h"

#include "../MainFrame.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "../stdafx.h"
#include <cstdlib>
#include <wx/string.h>

#include <curl/curl.h>

#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/prepared_statement.h>

//AN ENVIRONMENT VARIABLE IS USED SO THE UNIT TEST CODE CAN BE PUSHED TO GITHUB
static std::string GlobalSQLPassword = std::getenv("InvTrendPASSWORD"); //PASSWORD IS STORED IN ENVIRONMENT VARIABLE FOR PRIVACY REASONS

std::array<string, 2> GrabLatLong()
{
	//Grabs the User's Latitude and Longitude using a HTTP request to the ipapi API
	//-----------------------------------------------------------------------------------------------------
	std::string UserLatitude;
	std::string UserLongitude;
	
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
		if (ipapiResult != CURLE_OK) {
			std::string error_message = "Connection to ipapi failed.";
			throw std::runtime_error(error_message);
		}

		nlohmann::json j = nlohmann::json::parse(response);
		UserLatitude = j["latitude"].dump();
		UserLongitude = j["longitude"].dump();

		std::array<string, 2> LatLong;
		LatLong[0] = UserLatitude;
		LatLong[1] = UserLongitude;

		curl_easy_cleanup(curl);

		return LatLong;
	};
	//-----------------------------------------------------------------------------------------------------
}

bool CheckDatabase(std::string CheckID, int CheckQuantity, std::string CheckItem)
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
		std::string ResultID = result->getString("ItemID").c_str();
		int ResultInt = result->getInt("ItemQuantity");
		std::string ResultItem = result->getString("ItemName").c_str();
		if (ResultID == CheckID && ResultInt == CheckQuantity && ResultItem == CheckItem)
		{
			IsSame = true;

			delete pstmt;
			delete con;
			delete result;

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

int CheckWeatherDatabase(std::string CheckID, std::string Weather)
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

	pstmt = con->prepareStatement("SELECT * FROM weatherdata WHERE ItemID = ?");
	pstmt->setString(1, CheckID);
	result = pstmt->executeQuery();

	while (result->next())
	{
		std::string ResultID = result->getString("ItemID").c_str();
		int ResultInt = result->getInt(Weather);
		if (ResultID == CheckID)
		{

			delete pstmt;
			delete con;
			delete result;

			return ResultInt;
			break;
		}
	}
}

int CheckTimeDatabase(std::string CheckID, int hour)
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

	pstmt = con->prepareStatement("SELECT * FROM timedata WHERE ItemID = ?");
	pstmt->setString(1, CheckID);
	result = pstmt->executeQuery();

	while (result->next())
	{
		std::string ResultID = result->getString("ItemID").c_str();
		int ResultInt = result->getInt(std::to_string(hour));
		if (ResultID == CheckID)
		{

			delete pstmt;
			delete con;
			delete result;

			return ResultInt;
			break;
		}
	}
}

void QuickEditDB(std::string ChangeState, std::string TestIDInput, int TestQuantityInput, std::string TestItemInput)
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

		pstmt = con->prepareStatement("INSERT INTO weatherdata(ItemID, Clouds, Clear, Atmosphere, Snow, Rain, Drizzle, Thunderstorm) VALUES(?, 0, 0, 0, 0, 0, 0, 0)");
		pstmt->setString(1, TestIDInput);
		pstmt->executeQuery();

		pstmt = con->prepareStatement("INSERT INTO timedata(ItemID) VALUES(?)");
		pstmt->setString(1, TestIDInput);
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

		pstmt = con->prepareStatement("DELETE FROM weatherdata WHERE ItemID = ?");
		pstmt->setString(1, TestIDInput);
		pstmt->executeQuery();

		pstmt = con->prepareStatement("DELETE FROM timedata WHERE ItemID = ?");
		pstmt->setString(1, TestIDInput);
		pstmt->executeQuery();

		pstmt = con->prepareStatement("DELETE FROM adminlogs WHERE Username = ?");
		pstmt->setString(1, "UnitTest");
		pstmt->executeQuery();
	}

	delete pstmt;
	delete con;
}

void DeleteFromSignupLoginDB(std::string Username)
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

	pstmt = con->prepareStatement("DELETE FROM userlogin WHERE Username = ?");
	pstmt->setString(1, Username);
	pstmt->executeQuery();

	delete pstmt;
	delete con;
}

void AddToSignupLoginDB(std::string Username, std::string Password)
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

	pstmt = con->prepareStatement("INSERT INTO userlogin(Username, Password) VALUES(?,?)");
	pstmt->setString(1, Username);
	pstmt->setString(2, Password);
	pstmt->executeQuery();

	delete pstmt;
	delete con;
}

bool CheckSignupLoginDB(std::string Username, std::string Password)
{
	//Establishes a connection with the MySQL Database
	//-----------------------------------------------------------------------------------------------------
	const std::string server = "tcp://managestock.mysql.database.azure.com:3306";
	const std::string username = "rootConnect";
	const std::string password = GlobalSQLPassword;

	sql::Driver* driver;
	sql::Connection* con;
	sql::PreparedStatement* pstmt = nullptr;
	sql::ResultSet* result = nullptr;

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

	pstmt = con->prepareStatement("SELECT * FROM userlogin WHERE Username = ?");
	pstmt->setString(1, Username);
	result = pstmt->executeQuery();

	while (result->next())
	{
		std::string FoundUser = result->getString("Username").c_str();
		std::string FoundPass = result->getString("Password").c_str();
		if (FoundPass == Password)
		{

			delete pstmt;
			delete con;
			delete result;

			return true;
		}
	}

	delete pstmt;
	delete con;
	delete result;
	return false;
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
			
			std::string TestCreateIDInput = "T3S701";
			int TestCreateQuantityInput = 8;
			std::string TestCreateItemInput = "UnitTest0001";

			MainFrame::CreateButtonOnClick(TestCreateIDInput, TestCreateQuantityInput, TestCreateItemInput, GlobalSQLPassword);

			bool CheckData = CheckDatabase(TestCreateIDInput, TestCreateQuantityInput, TestCreateItemInput);
			QuickEditDB("DELETE", TestCreateIDInput, TestCreateQuantityInput, TestCreateItemInput);
			Assert::AreEqual(true, CheckData); //If CheckData is true then the test will pass
		}

		TEST_METHOD(StockManagementUpdateFunctionWorks)
		{

			std::string TestUpdateIDInput = "T3S701";
			int TestUpdateQuantityInput = 8;
			std::string TestUpdateItemInput = "UnitTest0001";
			QuickEditDB("CREATE", TestUpdateIDInput, TestUpdateQuantityInput, TestUpdateItemInput);

			int ChangeTestUpdateQuantityInput = 5;
			std::string ChangeTestUpdateItemInput = "UnitTest0002";

			MainFrame::UpdateButtonOnClick(TestUpdateIDInput, ChangeTestUpdateQuantityInput, ChangeTestUpdateItemInput, GlobalSQLPassword);

			bool CheckData = CheckDatabase(TestUpdateIDInput, ChangeTestUpdateQuantityInput, ChangeTestUpdateItemInput);
			QuickEditDB("DELETE", TestUpdateIDInput, TestUpdateQuantityInput, TestUpdateItemInput);
			Assert::AreEqual(true, CheckData); //If CheckData is true then the test will pass
		}

		TEST_METHOD(StockManagementDeleteFunctionWorks)
		{

			std::string TestDeleteIDInput = "T3S701";
			int TestDeleteQuantityInput = 8;
			std::string TestDeleteItemInput = "UnitTest0001";

			QuickEditDB("CREATE", TestDeleteIDInput, TestDeleteQuantityInput, TestDeleteItemInput);

			MainFrame::DeleteButtonOnClick(TestDeleteIDInput, GlobalSQLPassword);

			bool CheckData = CheckDatabase(TestDeleteIDInput, TestDeleteQuantityInput, TestDeleteItemInput);
			Assert::AreEqual(false, CheckData); //If CheckData is false then the test will pass

		}

		TEST_METHOD(SellSystemFunctionWorks)
		{
			std::array<string, 2> LatLong = GrabLatLong();
			std::string UserLatitude = LatLong[0];
			std::string UserLongitude = LatLong[1];
			
			std::string TestSellIDInput = "T3S701";
			int TestSellQuantityInput = 8;
			std::string TestSellItemInput = "UnitTest0001";

			int ToSell = 2;
			int NewQuantity = 6;

			QuickEditDB("CREATE", TestSellIDInput, TestSellQuantityInput, TestSellItemInput);

			MainFrame::SellButtonOnClick(TestSellIDInput, ToSell, GlobalSQLPassword, UserLatitude, UserLongitude, "UnitTest");

			bool CheckData = CheckDatabase(TestSellIDInput, NewQuantity, TestSellItemInput);
			QuickEditDB("DELETE", TestSellIDInput, TestSellQuantityInput, TestSellItemInput);
			Assert::AreEqual(true, CheckData); //If CheckData is true then the test will pass

		}

		TEST_METHOD(SellSystemModifiesWeatherLogsCorrectly)
		{
			std::array<string, 2> LatLong = GrabLatLong();
			std::string UserLatitude = LatLong[0];
			std::string UserLongitude = LatLong[1];

			std::string TestSellIDInput = "T3S701";
			int TestSellQuantityInput = 8;
			std::string TestSellItemInput = "UnitTest0001";

			QuickEditDB("CREATE", TestSellIDInput, TestSellQuantityInput, TestSellItemInput);

			//Grabs the Weather using a HTTP request to the OpenWeather API
			//-----------------------------------------------------------------------------------------------------
			CURL* ecurl = curl_easy_init();

			std::string APIKey = std::getenv("OpenWeatherAPIKey"); //OpenWeather API KEY STORED IN ENVIRONMENT VARIABLE FOR SECURITY REASONS

			CURLcode OpenWeatherResult;
			std::string openweatherurl = "https://api.openweathermap.org/data/2.5/weather?lat=" + UserLatitude + "&lon=" + UserLongitude + "&appid=" + APIKey;

			if (ecurl)
			{
				curl_easy_setopt(ecurl, CURLOPT_URL, openweatherurl.c_str());

				std::string wresponse;

				curl_easy_setopt(ecurl, CURLOPT_WRITEDATA, &wresponse);
				curl_easy_setopt(ecurl, CURLOPT_WRITEFUNCTION, write_callback);

				OpenWeatherResult = curl_easy_perform(ecurl);

				nlohmann::json root = nlohmann::json::parse(wresponse);
				std::string Weather = root["weather"][0]["main"].get<std::string>();

				int PreWeather = CheckWeatherDatabase(TestSellIDInput, Weather);

				int ToSell = 2;

				MainFrame::SellButtonOnClick(TestSellIDInput, ToSell, GlobalSQLPassword, UserLatitude, UserLongitude, "UnitTest");

				int WeatherShouldBe = PreWeather + ToSell;

				int CheckData = CheckWeatherDatabase(TestSellIDInput, Weather);

				QuickEditDB("DELETE", TestSellIDInput, TestSellQuantityInput, TestSellItemInput);
				Assert::AreEqual(WeatherShouldBe, CheckData); //If CheckData is true then the test will pass

				curl_easy_cleanup(ecurl);
			}
			//-----------------------------------------------------------------------------------------------------
		}

		TEST_METHOD(SellSystemModifiesTimeLogsCorrectly)
		{
			std::array<string, 2> LatLong = GrabLatLong();
			std::string UserLatitude = LatLong[0];
			std::string UserLongitude = LatLong[1];

			std::string TestSellIDInput = "T3S701";
			int TestSellQuantityInput = 8;
			std::string TestSellItemInput = "UnitTest0001";

			QuickEditDB("CREATE", TestSellIDInput, TestSellQuantityInput, TestSellItemInput);

			std::tm* timeinfo;
			std::time_t rawtime;
			std::time(&rawtime);
			timeinfo = std::localtime(&rawtime);
			int hour = timeinfo->tm_hour;

			int PreTime = CheckTimeDatabase(TestSellIDInput, hour);

			int ToSell = 2;

			MainFrame::SellButtonOnClick(TestSellIDInput, ToSell, GlobalSQLPassword, UserLatitude, UserLongitude, "UnitTest");

			int TimeShouldBe = PreTime + ToSell;

			int CheckData = CheckTimeDatabase(TestSellIDInput, hour);

			QuickEditDB("DELETE", TestSellIDInput, TestSellQuantityInput, TestSellItemInput);
			Assert::AreEqual(TimeShouldBe, CheckData); //If CheckData is true then the test will pass
			//-----------------------------------------------------------------------------------------------------
		}

		TEST_METHOD(AnalysisValidationReturnsCorrectly)
		{
			bool valid = false;

			std::string TestAnalyseIDInput = "T3S701";
			int TestAnalyseQuantityInput = 8;
			std::string TestAnalyseItemInput = "UnitTest0001";

			QuickEditDB("CREATE", TestAnalyseIDInput, TestAnalyseQuantityInput, TestAnalyseItemInput);

			std::vector<std::variant<int, std::string>> ComparisonVector;
			std::vector<std::variant<int, std::string>> AnalysisValidateReturn = MainFrame::AnalysisButtonOnClick(TestAnalyseIDInput, GlobalSQLPassword);

			if (ComparisonVector != AnalysisValidateReturn)
			{
				valid = true;
			}

			QuickEditDB("DELETE", TestAnalyseIDInput, TestAnalyseQuantityInput, TestAnalyseItemInput);
			Assert::AreEqual(true, valid); //They should not be equal as it is in the database
		}

		TEST_METHOD(AnalysisValidationPreventAccessingEmptyData)
		{
			bool valid = false;

			std::string TestAnalyseIDInput = "ti10nuws";

			std::vector<std::variant<int, std::string>> ComparisonVector;
			std::vector<std::variant<int, std::string>> AnalysisValidateReturn = MainFrame::AnalysisButtonOnClick(TestAnalyseIDInput, GlobalSQLPassword);

			if (ComparisonVector == AnalysisValidateReturn)
			{
				valid = true;
			}

			Assert::AreEqual(true, valid); //It should be equal as it is not in the database
		}

		TEST_METHOD(SignupWorksCorrectly)
		{
			std::string EnteredSignupUsername = "T3S701";
			std::string EnteredSignupPassword = "UnitTest0001";
			std::string PasswordShouldBe = "XqlwWhvw0001";

			MainFrame::SignupButtonOnClick(EnteredSignupUsername, EnteredSignupPassword, GlobalSQLPassword);

			bool valid = CheckSignupLoginDB(EnteredSignupUsername, PasswordShouldBe);

			DeleteFromSignupLoginDB(EnteredSignupUsername);

			Assert::AreEqual(true, valid);
		}

		TEST_METHOD(LoginWorksCorrectly)
		{
			std::string EnteredSignupUsername = "T3S701";
			std::string EnteredSignupPassword = "UnitTest0001";
			std::string PasswordEncrypted = "XqlwWhvw0001";

			AddToSignupLoginDB(EnteredSignupUsername, PasswordEncrypted);

			bool LoginValid = MainFrame::LoginButtonOnClick(EnteredSignupUsername, EnteredSignupPassword, GlobalSQLPassword);

			DeleteFromSignupLoginDB(EnteredSignupUsername);

			Assert::AreEqual(true, LoginValid);
		}
	};
}

