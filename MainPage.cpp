#include "MainPage.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(MainPage);

bool MainPage::OnInit()
{
	MainFrame* mainFrame = new MainFrame("Smart Inventory Trend Analysis System");
	mainFrame->SetClientSize(1000, 600);
	mainFrame->Center();
	mainFrame->Show();

	return true;
}