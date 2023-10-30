#pragma once
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

static void UpdateTimeDB(int hour, sql::PreparedStatement*& pstmt, sql::Connection*& con, int EnteredSellQuantity, std::string EnteredSellID)
{
	switch (hour)
	{
	case 0:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `0` = `0` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 1:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `1` = `1` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 2:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `2` = `2` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 3:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `3` = `3` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 4:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `4` = `4` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 5:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `5` = `5` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 6:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `6` = `6` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 7:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `7` = `7` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 8:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `8` = `8` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 9:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `9` = `9` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 10:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `10` = `10` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 11:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `11` = `11` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 12:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `12` = `12` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 13:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `13` = `13` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 14:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `14` = `14` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 15:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `15` = `15` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 16:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `16` = `16` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 17:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `17` = `17` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 18:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `18` = `18` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 19:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `19` = `19` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 20:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `20` = `20` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 21:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `21` = `21` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 22:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `22` = `22` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	case 23:
		pstmt = con->prepareStatement("UPDATE `itemdatabase`.`timedata` SET `23` = `23` + ? WHERE(`ItemID` = ?)");
		pstmt->setInt(1, EnteredSellQuantity);
		pstmt->setString(2, EnteredSellID);
		pstmt->execute();
		break;
	}
}