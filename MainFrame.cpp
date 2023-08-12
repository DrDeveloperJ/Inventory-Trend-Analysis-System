#include "MainFrame.h"
#include <mysql.h>
#pragma comment(lib, "\libmysql.lib")

struct connection_details {
	const char* server, * user, * password, * database;
};

MYSQL* mysql_connection_setup(struct connection_details mysql_details)
{
	MYSQL* connection = mysql_init(NULL);

	if (!mysql_real_connect(connection, mysql_details.server, mysql_details.user,
		mysql_details.password, mysql_details.database, 0, NULL, 0))
	{
		std::cout << "Connection Error: " << mysql_error(connection) << "/n";
		exit(1);
	}

	return connection;
}

MYSQL_RES* mysql_execute_query(MYSQL *connection, const char* sql_query)
{
	if (mysql_query(connection, sql_query))
	{
		std::cout << "MySQL Query Error :" << mysql_error(connection) << "/n";
		exit(1);
	}

	return mysql_use_result(connection);
}

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	CreateOptions();
}

void MainFrame::CreateOptions()
{
	MYSQL* SQLConnection;
	MYSQL_RES* SQLResults;
	MYSQL_ROW row;

	struct connection_details mysqlD;
	mysqlD.server = "localhost";
	mysqlD.user = "";
	mysqlD.password = "Admin475!";
	mysqlD.database = "";

	SQLConnection = mysql_connection_setup(mysqlD);
	SQLResults = mysql_execute_query(SQLConnection, "select * from itemtable");

	while ((row = mysql_fetch_row(SQLResults)) != NULL)
	{
		std::cout << row[0];
	}

	mysql_free_result(SQLResults);
	mysql_close(SQLConnection);

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

	ActiveArea = new wxPanel(InteractiveArea, wxID_ANY, wxPoint(150, 0), wxSize(850, 500));
	ActiveArea->SetBackgroundColour(*wxGREEN);

	//This will display all the stock to the user in a user friendly Treeview Table
	TreeviewTable = new wxPanel(ActiveArea, wxID_ANY, wxPoint(440, 25), wxSize(400, 450));
	TreeviewTable->SetBackgroundColour(*wxWHITE);

	//This will allow the user to create a new item to add to the stock
	//-----------------------------------------------------------------------------------------------------
	wxFont SectionHeadingFont(wxFontInfo(wxSize(0, 20)).Bold());
	CreateArea = new wxPanel(ActiveArea, wxID_ANY, wxPoint(125, 30), wxSize(200, 200));
	CreateArea->SetBackgroundColour(*wxWHITE);
	CreateHeading = new wxStaticText(CreateArea, wxID_ANY, "Create Item",
		wxPoint(50, 5), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);
	CreateHeading->SetFont(SectionHeadingFont);

	CreateIDInput = new wxTextCtrl(CreateArea, wxID_ANY, "", wxPoint(5, 60), wxSize(190, 20));
	CreateIDInput->SetBackgroundColour(*wxBLUE);
	CreateIDLabel = new wxStaticText(CreateArea, wxID_ANY, "Item ID",
		wxPoint(50, 40), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);

	CreateItemInput = new wxTextCtrl(CreateArea, wxID_ANY, "", wxPoint(5, 100), wxSize(190, 20));
	CreateItemInput->SetBackgroundColour(*wxBLUE);
	CreateItemLabel = new wxStaticText(CreateArea, wxID_ANY, "Item Name",
		wxPoint(50, 80), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);

	CreateQuantityInput = new wxTextCtrl(CreateArea, wxID_ANY, "", wxPoint(5, 140), wxSize(190, 20));
	CreateQuantityInput->SetBackgroundColour(*wxBLUE);
	CreateQuantityLabel = new wxStaticText(CreateArea, wxID_ANY, "Item Quantity",
		wxPoint(50, 120), wxSize(100, 20), wxALIGN_CENTER_HORIZONTAL);

	createButton = new wxButton(CreateArea, wxID_ANY, "Create", wxPoint(75, 170), wxSize(50, 25));
	createButton->SetBackgroundColour(*wxBLUE);
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