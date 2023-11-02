#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "stdafx.h"
#include <cstdlib>
#include <wx/string.h>
#include <nlohmann/json.hpp>
#include "UpdateTimeData.h"
#include <ctime>
#include <string>
#include <sstream>
#include <variant>

#include <curl/curl.h>

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
	inline static void SellButtonOnClick(string& EnteredSellID, int& EnteredSellQuantity, string& GlobalSQLPassword, string Latitude, string Longitude);
	inline static std::vector<std::variant<int, std::string>> AnalysisButtonOnClick(string& EnteredAnalysisID, string& GlobalSQLPassword);
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
	static wxListView* analysisBasicListView;

	wxPanel* SellArea;
	wxStaticText* SellHeading;
	wxTextCtrl* SellIDInput;
	wxStaticText* SellIDLabel;
	wxTextCtrl* SellQuantityInput;
	wxStaticText* SellQuantityLabel;
	wxButton* SellButton;

	wxPanel* AnalysisArea;
	wxStaticText* AnalysisHeading;
	wxTextCtrl* AnalysisIDInput;
	wxStaticText* AnalysisIDLabel;
	wxButton* AnalysisButton;

	wxButton* TimeAnalysisButton;
	wxButton* WeatherAnalysisButton;
	wxPanel* WeatherTrendsArea;
	wxPanel* TimeTrendsArea;

	wxStaticText* CloudsHeading;
	static wxStaticText* Clouds;
	wxStaticText* ClearHeading;
	static wxStaticText* Clear;
	wxStaticText* AtmosphereHeading;
	static wxStaticText* Atmosphere;
	wxStaticText* SnowHeading;
	static wxStaticText* Snow;
	wxStaticText* RainHeading;
	static wxStaticText* Rain;
	wxStaticText* ThunderstormHeading;
	static wxStaticText* Thunderstorm;
	wxStaticText* DrizzleHeading;
	static wxStaticText* Drizzle;

	static wxStaticText* WeatherCurrentItemHeading;

	wxStaticText* Time00Heading;
	static wxStaticText* Time00;
	wxStaticText* Time01Heading;
	static wxStaticText* Time01;
	wxStaticText* Time02Heading;
	static wxStaticText* Time02;
	wxStaticText* Time03Heading;
	static wxStaticText* Time03;
	wxStaticText* Time04Heading;
	static wxStaticText* Time04;
	wxStaticText* Time05Heading;
	static wxStaticText* Time05;
	wxStaticText* Time06Heading;
	static wxStaticText* Time06;
	wxStaticText* Time07Heading;
	static wxStaticText* Time07;
	wxStaticText* Time08Heading;
	static wxStaticText* Time08;
	wxStaticText* Time09Heading;
	static wxStaticText* Time09;
	wxStaticText* Time10Heading;
	static wxStaticText* Time10;
	wxStaticText* Time11Heading;
	static wxStaticText* Time11;
	wxStaticText* Time12Heading;
	static wxStaticText* Time12;
	wxStaticText* Time13Heading;
	static wxStaticText* Time13;
	wxStaticText* Time14Heading;
	static wxStaticText* Time14;
	wxStaticText* Time15Heading;
	static wxStaticText* Time15;
	wxStaticText* Time16Heading;
	static wxStaticText* Time16;
	wxStaticText* Time17Heading;
	static wxStaticText* Time17;
	wxStaticText* Time18Heading;
	static wxStaticText* Time18;
	wxStaticText* Time19Heading;
	static wxStaticText* Time19;
	wxStaticText* Time20Heading;
	static wxStaticText* Time20;
	wxStaticText* Time21Heading;
	static wxStaticText* Time21;
	wxStaticText* Time22Heading;
	static wxStaticText* Time22;
	wxStaticText* Time23Heading;
	static wxStaticText* Time23;

	static wxStaticText* TimeCurrentItemHeading;
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

		pstmt = con->prepareStatement("INSERT INTO weatherdata(ItemID) VALUES(?)");
		pstmt->setString(1, EnteredCreateID);
		pstmt->executeQuery();

		pstmt = con->prepareStatement("INSERT INTO timedata(ItemID) VALUES(?)");
		pstmt->setString(1, EnteredCreateID);
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

		pstmt = con->prepareStatement("DELETE FROM weatherdata WHERE ItemID = ?");
		pstmt->setString(1, EnteredDeleteID);
		pstmt->executeQuery();

		pstmt = con->prepareStatement("DELETE FROM timedata WHERE ItemID = ?");
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

static size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata)
{
	std::string* response = reinterpret_cast<std::string*>(userdata);
	response->append(ptr, size * nmemb);
	return size * nmemb;
}

inline void MainFrame::SellButtonOnClick(string& EnteredSellID, int& EnteredSellQuantity, string& GlobalSQLPassword, string Latitude, string Longitude)
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
		if (CheckID == EnteredSellID)
		{
			IDFound = true;

			if ((EnteredSellQuantity >= 0) && (IDFound == true))
			{

				pstmt = con->prepareStatement("UPDATE itemtable SET ItemQuantity = ItemQuantity - ? WHERE ItemID = ?");
				pstmt->setInt(1, EnteredSellQuantity);
				pstmt->setString(2, EnteredSellID);
				pstmt->executeQuery();

				//Sends a HTTP Request to OpenWeatherAPI to grab current weather when the sell button is pressed
				CURL* curl = curl_easy_init();

				std::string APIKey = std::getenv("OpenWeatherAPIKey"); //OpenWeather API KEY STORED IN ENVIRONMENT VARIABLE FOR SECURITY REASONS

				CURLcode OpenWeatherResult;
				std::string openweatherurl = "https://api.openweathermap.org/data/2.5/weather?lat=" + Latitude + "&lon=" + Longitude + "&appid=" + APIKey;


				if (curl)
				{
					curl_easy_setopt(curl, CURLOPT_URL, openweatherurl.c_str());

					std::string wresponse;

					curl_easy_setopt(curl, CURLOPT_WRITEDATA, &wresponse);
					curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

					OpenWeatherResult = curl_easy_perform(curl);

					//Parse the result for the Weather
					nlohmann::json root = nlohmann::json::parse(wresponse);
					std::string Weather = root["weather"][0]["main"].get<std::string>();

					curl_easy_cleanup(curl);

					//Comparison made to determine what the weather is and update the weather table accordingly
					if (Weather == "Clouds")
					{
						pstmt = con->prepareStatement("UPDATE weatherdata SET Clouds = Clouds + ? WHERE ItemID = ?");
						pstmt->setInt(1, EnteredSellQuantity);
						pstmt->setString(2, EnteredSellID);
						pstmt->execute();
					}
					else if (Weather == "Clear")
					{
						pstmt = con->prepareStatement("UPDATE weatherdata SET Clear = Clear + ? WHERE ItemID = ?");
						pstmt->setInt(1, EnteredSellQuantity);
						pstmt->setString(2, EnteredSellID);
						pstmt->execute();
					}
					else if (Weather == "Atmosphere")
					{
						pstmt = con->prepareStatement("UPDATE weatherdata SET Atmosphere = Atmosphere + ? WHERE ItemID = ?");
						pstmt->setInt(1, EnteredSellQuantity);
						pstmt->setString(2, EnteredSellID);
						pstmt->execute();
					}
					else if (Weather == "Snow")
					{
						pstmt = con->prepareStatement("UPDATE weatherdata SET Snow = Snow + ? WHERE ItemID = ?");
						pstmt->setInt(1, EnteredSellQuantity);
						pstmt->setString(2, EnteredSellID);
						pstmt->execute();
					}
					else if (Weather == "Rain")
					{
						pstmt = con->prepareStatement("UPDATE weatherdata SET Rain = Rain + ? WHERE ItemID = ?");
						pstmt->setInt(1, EnteredSellQuantity);
						pstmt->setString(2, EnteredSellID);
						pstmt->execute();
					}
					else if (Weather == "Drizzle")
					{
						pstmt = con->prepareStatement("UPDATE weatherdata SET Drizzle = Drizzle + ? WHERE ItemID = ?");
						pstmt->setInt(1, EnteredSellQuantity);
						pstmt->setString(2, EnteredSellID);
						pstmt->execute();
					}
					else if (Weather == "Thunderstorm")
					{
						pstmt = con->prepareStatement("UPDATE weatherdata SET Thunderstorm = Thunderstorm + ? WHERE ItemID = ?");
						pstmt->setInt(1, EnteredSellQuantity);
						pstmt->setString(2, EnteredSellID);
						pstmt->execute();
					}

					//Update the time database
					std::tm* timeinfo;
					std::time_t rawtime;
					std::time(&rawtime);
					timeinfo = std::localtime(&rawtime);
					int hour = timeinfo->tm_hour;

					UpdateTimeDB(hour, pstmt, con, EnteredSellQuantity, EnteredSellID);
				}
			}

			break;
		}
	}

	delete pstmt;
	delete con;
	delete result;
}
// ------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------


//Analysis Page Function Definitions
// ------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------

inline std::vector<std::variant<int, std::string>> MainFrame::AnalysisButtonOnClick(string& EnteredAnalysisID, string& GlobalSQLPassword)
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
	sql::ResultSet* weatherresult = nullptr;
	sql::ResultSet* timeresult = nullptr;

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
	pstmt = con->prepareStatement("SELECT * FROM itemtable WHERE ItemID = ?");
	pstmt->setString(1, EnteredAnalysisID);
	result = pstmt->executeQuery();

	std::vector<std::variant<int, std::string>> ReturnedAnalysisData;

	while (result->next())
	{
		string CheckID = result->getString(3).c_str();
		string ItemName = result->getString("ItemName").c_str();
		if (CheckID == EnteredAnalysisID)
		{
			std::vector<std::variant<int, std::string>> ReturnedAnalysisData;
			ReturnedAnalysisData.push_back(ItemName);

			pstmt = con->prepareStatement("SELECT * FROM weatherdata WHERE ItemID = ?");
			pstmt->setString(1, EnteredAnalysisID);
			weatherresult = pstmt->executeQuery();

			while (weatherresult->next())
			{
				ReturnedAnalysisData.push_back(weatherresult->getInt("Clouds"));
				ReturnedAnalysisData.push_back(weatherresult->getInt("Clear"));
				ReturnedAnalysisData.push_back(weatherresult->getInt("Atmosphere"));
				ReturnedAnalysisData.push_back(weatherresult->getInt("Snow"));
				ReturnedAnalysisData.push_back(weatherresult->getInt("Rain"));
				ReturnedAnalysisData.push_back(weatherresult->getInt("Drizzle"));
				ReturnedAnalysisData.push_back(weatherresult->getInt("Thunderstorm"));
			}

			pstmt = con->prepareStatement("SELECT * FROM timedata WHERE ItemID = ?");
			pstmt->setString(1, EnteredAnalysisID);
			timeresult = pstmt->executeQuery();

			//Grabs each number of items sold for each hour and appends them one by one
			while (timeresult->next())
			{
				for (int iter = 2; iter < 26; iter++)
				{
					ReturnedAnalysisData.push_back(timeresult->getInt(iter));
				}				
			}

			delete pstmt;
			delete con;
			delete result;
			delete weatherresult;
			delete timeresult;
			return ReturnedAnalysisData;
		}
	}

	delete pstmt;
	delete con;
	delete result;
	delete weatherresult;
	return ReturnedAnalysisData;
}

// ------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------
