//---------------------------------------------------------------------------
#include <DateUtils.hpp>
#pragma hdrstop

#include "Database.h"
#include "HelperFunctions.h"
#include "SystemManager.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//TDB *DB;

//---------------------------------------------------------------------------
Database::Database()
{
	m_csLock = std::shared_ptr<TCriticalSection>(new TCriticalSection());
	NewLogAdded = false;

	ErrDB = new TADOConnection(nullptr);
	MainDB = new TADOConnection(nullptr);
	QryMain = new TADOQuery(nullptr);
	QryErr = new TADOQuery(nullptr);

	ErrDB->CommandTimeout = 30;
	ErrDB->ConnectionTimeout = 15;
	ErrDB->ConnectOptions = coConnectUnspecified;
	ErrDB->CursorLocation = clUseClient;
	ErrDB->IsolationLevel = ilCursorStability;
	ErrDB->KeepConnection = true;
	ErrDB->LoginPrompt = false;
	ErrDB->Mode = cmUnknown;

	MainDB->CommandTimeout = 30;
	MainDB->ConnectionTimeout = 15;
	MainDB->ConnectOptions = coConnectUnspecified;
	MainDB->CursorLocation = clUseClient;
	MainDB->IsolationLevel = ilCursorStability;
	MainDB->KeepConnection = true;
	MainDB->LoginPrompt = false;
	MainDB->Mode = cmUnknown;

	QryMain->Active = false;
	QryMain->AutoCalcFields = true;
	QryMain->CacheSize = 1;
	QryMain->CommandTimeout = 30;
	QryMain->Connection = MainDB;
	QryMain->CursorLocation = clUseClient;
	QryMain->CursorType = ctKeyset;
	QryMain->EnableBCD = true;
	QryMain->Filtered = false;
	QryMain->LockType = ltOptimistic;
	QryMain->MarshalOptions = moMarshalAll;
	QryMain->MaxRecords = 0;
	QryMain->ParamCheck = true;
	QryMain->Prepared = false;

	QryErr->Active = false;
	QryErr->AutoCalcFields = false;
	QryErr->CacheSize = 1;
	QryErr->CommandTimeout = 30;
	QryErr->Connection = ErrDB;
	QryErr->CursorLocation = clUseClient;
	QryErr->CursorType = ctKeyset;
	QryErr->EnableBCD = true;
	QryErr->Filtered = false;
	QryErr->LockType = ltOptimistic;
	QryErr->MarshalOptions = moMarshalAll;
	QryErr->MaxRecords = 0;
	QryErr->ParamCheck = true;
	QryErr->Prepared = false;
}
//---------------------------------------------------------------------------
Database::~Database()
{
	if( QryMain )
	{
		QryMain->Close();
		delete QryMain;
    }
	if( QryErr )
	{
		QryErr->Close();
		delete QryErr;
    }
	if( ErrDB )
	{
		ErrDB->Close();
		delete ErrDB;
    }
	if( MainDB )
	{
		MainDB->Close();
		delete MainDB;
    }
}

bool Database::ConnectErrDB()
{
	if( ErrDB == nullptr )
		return false;

	m_csLock->Acquire();

	bool ret = true;
	String sPath = GetDataFolder() + ERRDEFDB;

	if( FileExists(sPath) )
	{
		if( ErrDB->Connected )
			ErrDB->Close();
		ErrDB->LoginPrompt = false;

		String strConn  = "Provider=Microsoft.Jet.OLEDB.4.0;";
		strConn += "Data Source=" + sPath;// + "OvenDb.mdb;";
		strConn += ";Persist Security Info=False;";
		strConn += "Jet OLEDB:Database";//;Password=baik11";

		ErrDB->ConnectionString = strConn;

		try
		{
			ErrDB->Open();
			if( ErrDB->State == (TObjectStates() << stOpen) )
			{
				ErrDB->Connected = true;
			}
		}
		catch(Exception *e)
		{
			SetMessageStr(e->Message);
			ShowMessageDlg();
			ret = false;
		}
	}

	m_csLock->Release();
	return ret;
}

bool Database::AddUser(const DB_USER user)
{
	if( MainDB == nullptr )
		return false;

	if( MainDB->Connected == false )
		return false;

	m_csLock->Acquire();

	int ret = 0;
	String Field = " ([ID], [PW], [Grade], [Name] )";
	String Value = "'" + user.ID + "','";
	Value += user.PW + "'," + IntToStr(user.Grade);
	Value += ",'" + user.Name + "'";

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add("INSERT INTO UserTbl " + Field + " VALUES ("+Value+");");
	QryMain->Prepared = true;
	ret = QryMain->ExecSQL();
	m_csLock->Release();

	return (ret > 0 );
}

void Database::EditUser(const DB_USER user)
{
	if( MainDB == nullptr )
		return;

	if( MainDB->Connected == false )
		return;

 	m_csLock->Acquire();

	String Value = "SET ";
	Value += "[Grade] = " + IntToStr(user.Grade) + ",";
	Value += "[PW] = '" + user.PW + "',";
	Value += "[Name] = '" + user.Name + "'";
	String WHERE = " WHERE [ID]= '" + user.ID + "';";

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add("UPDATE UserTbl " + Value + WHERE);
	QryMain->Prepared = true;
	QryMain->ExecSQL();
	m_csLock->Release();
}

void Database::DeleteUser(const DB_USER user)
{
	if( MainDB == nullptr )
		return;

	if( MainDB->Connected == false )
		return;

	m_csLock->Acquire();

	String WHERE = " WHERE [ID]= '" + user.ID + "';";
	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add("DELETE from UserTbl " + WHERE);
	QryMain->Prepared = true;
	QryMain->ExecSQL();

	m_csLock->Release();
}

bool Database::IsUserExist(const String userId)
{
	if( MainDB == nullptr )
		return false;

	if( MainDB->Connected == false )
		return false;

 	m_csLock->Acquire();

	bool ret = false;
	String WHERE = " WHERE [ID]= '" + userId  + "';";
	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add("SELECT * from UserTbl " + WHERE);
	QryMain->Open();

	if( QryMain->RecordCount > 0)
	{
		ret = true;
	}

	m_csLock->Release();
	return ret;
}

bool Database::IsCorrectLoginInfo(const AnsiString sId,
										const AnsiString sPass,
										const int grade)
{
	if( MainDB == nullptr )
		return false;

	if( MainDB->Connected == false )
		return false;

	m_csLock->Acquire();

	bool ret = false;
	String WHERE = " WHERE [ID] = '" + sId + "'";
	WHERE += " and [PW] = '" + sPass + "';";
	String Sql = "SELECT * from UserTbl" + WHERE;

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add(Sql);
	QryMain->Open();

	if( QryMain->RecordCount > 0)
	{
		QryMain->First();
		if(static_cast<int>(QryMain->FieldByName("Grade")->AsInteger) == grade)
		{
			ret = true;
		}
		else
		{
			SetMessageStr(GetResStr("Warning", "LowLevelId"));
			ShowMessageDlg();
			ret = false;
		}
	}
	else
	{
		SetMessageStr(GetResStr("Warning", "UnregedUserId"));
		ShowMessageDlg();
		ret = false;
	}

	m_csLock->Release();
	return ret;
}

bool Database::IsFieldExisted(String table, String field)
{
	if( MainDB == nullptr )
		return false;

	if( !MainDB->Connected )
		return false;

	m_csLock->Acquire();

	String query;
	bool ret = false;

	query.sprintf(L"select * from %s", table.c_str());

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add(query);
	QryMain->Open();

	TStringList *fieldList = new TStringList;
	QryMain->GetFieldNames(fieldList);
	if( fieldList->IndexOf(field) >= 0 )
		ret = true;

	delete fieldList;
	m_csLock->Release();

	return ret;
}

void Database::AddStringField(String table, String field, int length)
{
	if( MainDB == nullptr )
		return;

	if( MainDB->Connected == false )
		return;

	m_csLock->Acquire();

	String query;
	query.sprintf(L"alter table [%s] add [%s] varchar(%d)", table.c_str(), field.c_str(), length);

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add(query);
	QryMain->Prepared = true;
	QryMain->ExecSQL();
	m_csLock->Release();
}

void Database::ModifyStringFieldSize(String table, String field, int size)
{
	if( MainDB == nullptr )
		return;

	if( MainDB->Connected == false )
		return;

	m_csLock->Acquire();

	String query;
	query.sprintf(L"alter table [%s] alter column [%s] varchar(%d)", table.c_str(), field.c_str(), size);

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add(query);
	QryMain->Prepared = true;
	QryMain->ExecSQL();
	m_csLock->Release();
}

void Database::AddIntField(String table, String field)
{
	if( MainDB == nullptr )
		return;

	if( MainDB->Connected == false )
		return;

	m_csLock->Acquire();

	String query;
	query.sprintf(L"alter table [%s] add [%s] int", table.c_str(), field.c_str());

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add(query);
	QryMain->Prepared = true;
	QryMain->ExecSQL();
	m_csLock->Release();
}

QueryPtr Database::GetLogQuery(TDateTime start)
{
	QueryPtr logQuery;
	logQuery.reset(new TADOQuery(MainDB));

	if( MainDB == nullptr )
		return logQuery;

	if( MainDB->Connected == false )
		return logQuery;

	m_csLock->Acquire();

	String WHERE;
	WHERE.sprintf(L"WHERE LogTime>=%f ORDER BY LogTime ASC", (double)start);

	logQuery->Connection = MainDB;
	logQuery->Close();
	logQuery->SQL->Clear();
	logQuery->SQL->Add("SELECT * from LogTbl " + WHERE);
	logQuery->Open();

	int count = logQuery->RecordCount;

	m_csLock->Release();

	return logQuery;
}

bool Database::QueryUser()
{
	if( MainDB == nullptr )
		return false;

	if( MainDB->Connected == false )
		return false;

	m_csLock->Acquire();

	Query.reset(new TADOQuery(MainDB));
	Query->Connection = MainDB;
	Query->Close();
	Query->SQL->Clear();
	Query->SQL->Add("SELECT * from UserTbl");
	Query->Open();

	m_csLock->Release();

	return true;
}

bool Database::GetError(unsigned int code, DB_ERROR &error, int language)
{
	if( ErrDB == nullptr )
		return false;

	if( ErrDB->Connected == false )
		return false;

	if( QryErr == nullptr )
		return false;

	m_csLock->Acquire();

	bool ret = true;
	String WHERE;
	WHERE.sprintf(L" WHERE Code = %d", code);

	QryErr->Close();
	QryErr->SQL->Clear();
	QryErr->SQL->Add("SELECT * from ErrDefine " + WHERE);
	QryErr->Open();

	if( QryErr->RecordCount > 0 )
	{
		if( language == KOR )
		{
			error.Name = QryErr->FieldByName("Name")->AsString;
			error.Cause = QryErr->FieldByName("Cause")->AsString;
			error.Action = QryErr->FieldByName("Action")->AsString;
		}
		else
		if( language == CHN )
		{
			error.Name = QryErr->FieldByName("Name_C")->AsString;
			error.Cause = QryErr->FieldByName("Cause_C")->AsString;
			error.Action = QryErr->FieldByName("Action_C")->AsString;
		}
		else
		{
			error.Name = QryErr->FieldByName("Name_E")->AsString;
			error.Cause = QryErr->FieldByName("Cause_E")->AsString;
			error.Action = QryErr->FieldByName("Action_E")->AsString;
		}
	}
	else
		ret = false;

	m_csLock->Release();
	return ret;
}

void OvenDatabase::CheckDBFile()
{
	int ret;
	try
	{
		String OrgDbName = GetCurrentDir() + "\\Data\\" + OVENDB;
		String TgtDbName = GetDataFolder() + OVENDB;

		if(OrgDbName.Compare(TgtDbName) != 0)
			ret = CopyFile(OrgDbName.c_str(), TgtDbName.c_str(), true);

		OrgDbName = GetCurrentDir() + "\\Data\\" + ERRDEFDB;
		TgtDbName = GetDataFolder() + ERRDEFDB;

		if(OrgDbName.Compare(TgtDbName) != 0)
			ret = CopyFile(OrgDbName.c_str(), TgtDbName.c_str(), true);
	}
	catch(Exception* e)
	{
		SetMessageStr(e->Message);
		ShowMessageDlg();
    }
}

OvenDatabase::OvenDatabase()
	: Database()
{
}

OvenDatabase::~OvenDatabase()
{
	m_csLock->Acquire();

	Query.reset();
	if( QryMain )
	{
		QryMain->Close();
		delete QryMain;
		QryMain = nullptr;
    }
	if( QryErr )
	{
		QryErr->Close();
		delete QryErr;
		QryErr = nullptr;
    }
	if( ErrDB )
	{
		ErrDB->Close();
		delete ErrDB;
		ErrDB = nullptr;
    }
	if( MainDB )
	{
		MainDB->Close();
		delete MainDB;
		MainDB = nullptr;
	}

	m_csLock->Release();
}

bool OvenDatabase::ConnectDB()
{
	if( MainDB == nullptr )
		return false;

	m_csLock->Acquire();

	bool ret = true;
	String sPath = GetDataFolder() + OVENDB;

	if( FileExists(sPath) )
	{
		if( MainDB->Connected )
			MainDB->Close();
		MainDB->LoginPrompt = false;

		String strConn  = "Provider=Microsoft.Jet.OLEDB.4.0;";
		strConn += "Data Source=" + sPath;// + "OvenDb.mdb;";
		strConn += ";Persist Security Info=False;";
		strConn += "Jet OLEDB:Database";//;Password=baik11";

		MainDB->ConnectionString = strConn;

		try
		{
			MainDB->Open();
			if( MainDB->State == (TObjectStates() << stOpen) )
			{
				MainDB->Connected = true;
			}
		}
		catch(Exception *e)
		{
			SetMessageStr(e->Message);
			ShowMessageDlg();

			ret = false;
		}
	}

	m_csLock->Release();
	return ret;
}

void OvenDatabase::AddRecipe(const OVEN_RECIPE dbRecipe)
{
	if( MainDB == nullptr )
		return;

	if( MainDB->Connected == false )
		return;

	m_csLock->Acquire();

	String Field = " ([ChamberNo], [Recipe], [PtnNo], [CureTemp], [CureTime] )";
	String Value = IntToStr(dbRecipe.ChamberNo) + ",'" + dbRecipe.Recipe + "',";
	Value += IntToStr(dbRecipe.PtnNo) + ",";
	Value += IntToStr(dbRecipe.CureTemp) + "," + IntToStr(dbRecipe.CureTime);

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add("INSERT INTO RecipeTbl " + Field + " VALUES ("+Value+");");
	QryMain->Prepared = true;
	QryMain->ExecSQL();
	m_csLock->Release();
}

void OvenDatabase::EditRecipe(const int nCmNo,
								const String sRecipe,
								const OVEN_RECIPE newRecipe)
{
	if( MainDB == nullptr )
		return;

	if( MainDB->Connected == false )
		return;

	m_csLock->Acquire();

	String Value = "SET [PtnNo] = " + IntToStr(newRecipe.PtnNo) + ",";
	Value += "[Recipe] = '" + newRecipe.Recipe + "',";
	Value += "[CureTemp] = " + IntToStr(newRecipe.CureTemp) + ",";
	Value += "[CureTime] = " + IntToStr(newRecipe.CureTime);
	String WHERE = " WHERE [ChamberNo]=" + IntToStr(nCmNo) + " and ";
	WHERE += "[Recipe]='" + sRecipe + "';";

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add("UPDATE RecipeTbl " + Value + WHERE);
	QryMain->Prepared = true;
	QryMain->ExecSQL();
	m_csLock->Release();
}

void OvenDatabase::DeleteRecipe(const OVEN_RECIPE dbRecipe)
{
	if( MainDB == nullptr )
		return;

	if( MainDB->Connected == false )
		return;

	m_csLock->Acquire();

	String WHERE = " WHERE [ChamberNo]=" + IntToStr(dbRecipe.ChamberNo) + " and ";
	WHERE += "[Recipe]='" + dbRecipe.Recipe + "';";

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add("DELETE from RecipeTbl " + WHERE);
	QryMain->Prepared = true;
	QryMain->ExecSQL();
	m_csLock->Release();
}

void OvenDatabase::DeleteRecipe(const int nCmNo, const String sRecipe)
{
	if( MainDB == nullptr )
		return;

	if( MainDB->Connected == false )
		return;

	m_csLock->Acquire();

	String WHERE = " WHERE [ChamberNo]=" + IntToStr(nCmNo) + " and ";
	WHERE += "[Recipe]='" + sRecipe + "';";

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add("DELETE from RecipeTbl " + WHERE);
	QryMain->Prepared = true;
	QryMain->ExecSQL();
	m_csLock->Release();
}

bool OvenDatabase::IsRecipeExist(const int nCmNo, const String sRecipe)
{
	if( MainDB == nullptr )
		return false;

	if( MainDB->Connected == false )
		return false;

	m_csLock->Acquire();

	bool ret = false;
	String WHERE = " WHERE [ChamberNo]= " + IntToStr(nCmNo);
	WHERE += " and [Recipe]= '" + sRecipe + "';";

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add("SELECT * from RecipeTbl " + WHERE);
	QryMain->Open();

	if( QryMain->RecordCount > 0)
	{
		ret = true;
	}

	m_csLock->Release();
	return ret;
}

bool OvenDatabase::IsRecipeExist(const int nCmNo, const int patNo)
{
	if( MainDB == nullptr )
		return false;

	if( MainDB->Connected == false )
		return false;

 	m_csLock->Acquire();

	bool ret = false;
	String WHERE = " WHERE [ChamberNo]= " + IntToStr(nCmNo);
	WHERE += " and [PtnNo]= " + IntToStr(patNo) + ";";

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add("SELECT * from RecipeTbl " + WHERE);
	QryMain->Open();

	if( QryMain->RecordCount > 0)
	{
		ret = true;
	}

	m_csLock->Release();
	return ret;
}

bool OvenDatabase::GetRecipe(const int chamberNo,
							   const String recipe,
							   OVEN_RECIPE &recipeInfo)
{
	if( MainDB == nullptr )
		return false;

	if( MainDB->Connected == false )
		return false;

	m_csLock->Acquire();

	bool ret = false;
	String WHERE = " WHERE [ChamberNo]= " + IntToStr(chamberNo);
	WHERE += " and [Recipe]= '" + recipe + "';";

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add("SELECT * from RecipeTbl " + WHERE);
	QryMain->Open();

	if( QryMain->RecordCount > 0)
	{
		recipeInfo.ChamberNo = QryMain->FieldByName("ChamberNo")->AsInteger;
		recipeInfo.Recipe = QryMain->FieldByName("Recipe")->AsString;
		recipeInfo.PtnNo = QryMain->FieldByName("PtnNo")->AsInteger;
		recipeInfo.CureTemp = QryMain->FieldByName("CureTemp")->AsInteger;
		recipeInfo.CureTime = QryMain->FieldByName("CureTime")->AsInteger;
		ret = true;
	}

	m_csLock->Release();
	return ret;
}

bool OvenDatabase::GetRecipeName(const int chamberNo, const int patNo, String &name)
{
	if( MainDB == nullptr )
		return false;

	if( MainDB->Connected == false )
		return false;

	m_csLock->Acquire();

	bool ret = false;
	String WHERE = " WHERE [ChamberNo]= " + IntToStr(chamberNo);
	WHERE += " and [PtnNo]= " + IntToStr(patNo) + ";";

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add("SELECT * from RecipeTbl " + WHERE);
	QryMain->Open();

	if( QryMain->RecordCount > 0)
	{
		name = QryMain->FieldByName("Recipe")->AsString;
		ret = true;
	}

	m_csLock->Release();
	return ret;
}

void OvenDatabase::UpdateRecipeWithPtnNo(const int chamberNo, const int patNo)
{
	if( MainDB == nullptr )
		return;

	if( MainDB->Connected == false )
		return;

	m_csLock->Acquire();

	Query.reset(new TADOQuery(MainDB));

	String WHERE = " WHERE [ChamberNo]= " + IntToStr(chamberNo);
	WHERE += " and [PtnNo]= " + IntToStr(patNo) + ";";


	Query->Connection = MainDB;
	Query->Close();
	Query->SQL->Clear();
	Query->SQL->Add("SELECT * from RecipeTbl " + WHERE);
	Query->Open();

	OVEN_RECIPE recipeInfo;
	PTN ptn;

	if( LoadTempPtn(chamberNo, patNo, ptn) )
	{
		for(int i=0; i<Query->RecordCount; i++)
		{
			recipeInfo.ChamberNo = chamberNo;
			recipeInfo.Recipe = Query->FieldByName("Recipe")->AsString;
			recipeInfo.PtnNo = patNo;
			recipeInfo.CureTemp = ptn.MaxTemp;
			recipeInfo.CureTime = ptn.TotalTime;
			EditRecipe(chamberNo, recipeInfo.Recipe, recipeInfo);
			Query->Next();
		}
	}

	m_csLock->Release();
}

void OvenDatabase::DeleteRecipeWithPtnNoAndRcpName(const int chamberNo, const int patNo, const String sRecipe)
{
	if( MainDB == nullptr )
		return;

	if( MainDB->Connected == false )
		return;

	m_csLock->Acquire();

	String WHERE = " WHERE [ChamberNo]= " + IntToStr(chamberNo);
	WHERE += " and [PtnNo]= " + IntToStr(patNo);
	WHERE += " and [Recipe]='" + sRecipe + "';";

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add("DELETE from RecipeTbl " + WHERE);
	QryMain->Prepared = true;
	QryMain->ExecSQL();
	m_csLock->Release();

	m_csLock->Release();
}

void OvenDatabase::DeleteRecipeWithPtnNo(const int chamberNo, const int patNo)
{
	if( MainDB == nullptr )
		return;

	if( MainDB->Connected == false )
		return;

	m_csLock->Acquire();
	Query.reset(new TADOQuery(MainDB));

	String WHERE = " WHERE [ChamberNo]= " + IntToStr(chamberNo);
	WHERE += " and [PtnNo]= " + IntToStr(patNo) + ";";

	Query->Connection = MainDB;
	Query->Close();
	Query->SQL->Clear();
	Query->SQL->Add("SELECT * from RecipeTbl " + WHERE);
	Query->Open();

	OVEN_RECIPE recipeInfo;
	for(int i=0; i<Query->RecordCount; i++)
	{
		recipeInfo.ChamberNo = chamberNo;
		recipeInfo.Recipe = Query->FieldByName("Recipe")->AsString;
		DeleteRecipe(recipeInfo);
		Query->Next();
	}

	m_csLock->Release();
}

bool OvenDatabase::QueryLog(int condition, TDateTime start, TDateTime end, bool isAsc)
{
	if( MainDB == nullptr )
		return false;

	if( MainDB->Connected == false )
		return false;

	m_csLock->Acquire();
	Query.reset(new TADOQuery(MainDB));

	String WHERE;
	switch( condition )
	{
		case ALL_ALL_ALL:
			WHERE = "";
			break;
		case ALL_ALL_TIME:
			WHERE.sprintf(L"WHERE LogTime>=%f AND LogTime<=%f", (double)start, (double)end);
			break;
		case ALL_ALM_ALL:
			WHERE = L"WHERE Type=0";
			break;
		case ALL_ALM_TIME:
			WHERE.sprintf(L"WHERE Type=0 AND LogTime>=%f AND LogTime<=%f", (double)start, (double)end);
			break;
		case ALL_MSG_ALL:
			WHERE = "WHERE Type=1";
			break;
		case ALL_MSG_TIME:
			WHERE.sprintf(L"WHERE Type=1 AND LogTime>=%f AND LogTime<=%f", (double)start, (double)end);
			break;
		case A_ALL_ALL:
			WHERE = "WHERE Chamber=0";
			break;
		case A_ALL_TIME:
			WHERE.sprintf(L"WHERE Chamber=0 AND LogTime>=%f AND LogTime<=%f", (double)start, (double)end);
			break;
		case A_ALM_ALL:
			WHERE = "WHERE Chamber=0 AND Type=0";
			break;
		case A_ALM_TIME:
			WHERE.sprintf(L"WHERE Chamber=0 AND Type=0 AND LogTime>=%f AND LogTime<=%f", (double)start, (double)end);
			break;
		case A_MSG_ALL:
			WHERE = "WHERE Chamber=0 AND Type=1";
			break;
		case A_MSG_TIME:
			WHERE.sprintf(L"WHERE Chamber=0 AND Type=1 AND LogTime>=%f AND LogTime<=%f", (double)start, (double)end);
			break;
		case B_ALL_ALL:
			WHERE = "WHERE Chamber=1";
			break;
		case B_ALL_TIME:
			WHERE.sprintf(L"WHERE Chamber=1 AND LogTime>=%f AND LogTime<=%f", (double)start, (double)end);
			break;
		case B_ALM_ALL:
			WHERE = "WHERE Chamber=1 AND Type=0";
			break;
		case B_ALM_TIME:
			WHERE.sprintf(L"WHERE Chamber=1 AND Type=0 AND LogTime>=%f AND LogTime<=%f", (double)start, (double)end);
			break;
		case B_MSG_ALL:
			WHERE = "WHERE Chamber=1 AND Type=1";
			break;
		case B_MSG_TIME:
			WHERE.sprintf(L"WHERE Chamber=1 AND Type=1 AND LogTime>=%f AND LogTime<=%f", (double)start, (double)end);
			break;
	}

	WHERE += " ORDER BY LogTime";
    WHERE += isAsc ? " ASC" : " DESC";

	Query->Connection = MainDB;
	Query->Close();
	Query->SQL->Clear();
	Query->SQL->Add("SELECT * from LogTbl " + WHERE);
	Query->Open();

	m_csLock->Release();
	return true;
}

void OvenDatabase::SaveLog(const String msg,
				  const int type,
				  const int chamberNo/* = -1*/)
{
	if( MainDB == nullptr )
		return;

	if( MainDB->Connected == false )
		return;

	m_csLock->Acquire();

	String sSaveMsg;
	if(msg.Length() > 255)
		sSaveMsg = msg.SubString(1, 255);
	else
		sSaveMsg = msg;

	TDateTime dtSave = Now();
	String Field = " ([Chamber], [LogTime], [Contents], [Type])";
	String Value = IntToStr(chamberNo);
	Value +=  ",'";
	Value += dtSave.FormatString("yyyy-mm-dd hh:nn:ss") + "','";
	Value +=  sSaveMsg + "'," + IntToStr(type);

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add("INSERT INTO LogTbl " + Field + " VALUES ("+Value+");");
	QryMain->Prepared = true;
	QryMain->ExecSQL();
	NewLogAdded = true;

	m_csLock->Release();
}

void OvenDatabase::SaveWork(OVEN_WORK work)
{
	if( MainDB == nullptr )
		return;

	if( MainDB->Connected == false )
		return;

	m_csLock->Acquire();

	String sql;
	String Field = " ([StartTime], [Recipe], [PtnNo], [CureTemp], [CureTime], ";
	Field += "[Remote], [UserID], [ChamberNo], [Filename])";
	String Value = "'" + work.StartTime.FormatString("yyyy-mm-dd hh:nn:ss") + "','";
	Value += work.Recipe + "',";
	Value += IntToStr(work.PtnNo) + ",";
	Value += IntToStr(work.CureTemp) + ",";
	Value += IntToStr(work.CureTime) + ",";
	Value += IntToStr(work.Remote) + ",'";
	Value += work.UserID + "',";
	Value += IntToStr(work.ChamberNo) + ",'";
	Value += work.Filename + "'";

	sql = "INSERT INTO WorkTbl " + Field + " VALUES ("+Value+");";

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add(sql);
	QryMain->Prepared = true;
	QryMain->ExecSQL();
	m_csLock->Release();
}

void OvenDatabase::SaveWork(OVEN_WORK work, String productCode)
{
	if( MainDB == nullptr )
		return;

	if( MainDB->Connected == false )
		return;

	m_csLock->Acquire();

	String sql;
	String Field = " ([StartTime], [Recipe], [PtnNo], [CureTemp], [CureTime], ";
	Field += "[Remote], [UserID], [ChamberNo], [Filename], [ProductCode])";

	String Value = "'" + work.StartTime.FormatString("yyyy-mm-dd hh:nn:ss") + "','";
	Value += work.Recipe + "',";
	Value += IntToStr(work.PtnNo) + ",";
	Value += IntToStr(work.CureTemp) + ",";
	Value += IntToStr(work.CureTime) + ",";
	Value += IntToStr(work.Remote) + ",'";
	Value += work.UserID + "',";
	Value += IntToStr(work.ChamberNo) + ",'";
	Value += work.Filename + "','";
	Value += productCode + "'";

	sql = "INSERT INTO WorkTbl " + Field + " VALUES ("+Value+");";

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add(sql);
	QryMain->Prepared = true;
	QryMain->ExecSQL();
	m_csLock->Release();
}

void OvenDatabase::SaveWork(OVEN_WORK work, LOTINFO lotInfo)
{
	if( MainDB == nullptr )
		return;

	if( MainDB->Connected == false )
		return;

	m_csLock->Acquire();

	String sql, str;
	String Field = " ([StartTime], [Recipe], [PtnNo], [CureTemp], [CureTime], ";
	Field += "[Remote], [UserID], [ChamberNo], [Filename], [TotalLotCount], [ProductCode]";
	for(int i=0; i<lotInfo.TotalLotCount; i++)
	{
		str.sprintf(L", [LotID_%d]", i);
		Field += str;

		if( lotInfo.UseQuantity )
		{
			str.sprintf(L", [LotCount_%d]", i);
			Field += str;
		}
	}
	Field += ")";

	String Value = "'" + work.StartTime.FormatString("yyyy-mm-dd hh:nn:ss") + "','";
	Value += work.Recipe + "',";
	Value += IntToStr(work.PtnNo) + ",";
	Value += IntToStr(work.CureTemp) + ",";
	Value += IntToStr(work.CureTime) + ",";
	Value += IntToStr(work.Remote) + ",'";
	Value += work.UserID + "',";
	Value += IntToStr(work.ChamberNo) + ",'";
	Value += work.Filename + "', ";
	Value += IntToStr(lotInfo.TotalLotCount);

	// 20180514 - ProductCode empty save - jhlim
//	if( lotInfo.LotID[0].IsEmpty() )
//		str = ", ' '";
//	else
//		str.sprintf(L", '%s'", lotInfo.LotID[0]);
//	Value += str;
		str = ", ' '";
	Value += str;

	for(int i=0; i<lotInfo.TotalLotCount; i++)
	{
		str.sprintf(L", '%s'", lotInfo.LotID[i].c_str());
		Value += str;

		if( lotInfo.UseQuantity )
		{
			str.sprintf(L", %d", lotInfo.LotQuantity[i]);
			Value += str;
		}
	}

	sql = "INSERT INTO WorkTbl " + Field + " VALUES (" + Value + ");";

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add(sql);
	QryMain->Prepared = true;
	QryMain->ExecSQL();
	m_csLock->Release();
}

void OvenDatabase::SaveWorkEnd(OVEN_WORK work, int endType)
{
	if( MainDB == nullptr )
		return;

	if( MainDB->Connected == false )
		return;

	m_csLock->Acquire();

	String sql;
	String Value = "SET [EndTime] = '";
	Value += work.EndTime.FormatString("yyyy-mm-dd hh:nn:ss") + "', ";
	Value += "[EndType] = " + IntToStr(endType);

	String WHERE = " WHERE [ChamberNo]=" + IntToStr(work.ChamberNo) + " and ";
	WHERE += "[Recipe]='" + work.Recipe + "' and ";
	WHERE += "[PtnNo]=" + IntToStr(work.PtnNo) + " and ";

	// 10초 전후
	String time;
	TDateTime startTime = work.StartTime;
	TDateTime beforeTime = IncSecond(startTime, -10);
	TDateTime afterTime = IncSecond(startTime, 10);

	time.sprintf(L"(StartTime>=%f and StartTime<=%f);", beforeTime, afterTime);
	WHERE += time;

	sql = "UPDATE WorkTbl " + Value + WHERE;

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add(sql);
	QryMain->Prepared = true;
	QryMain->ExecSQL();
	m_csLock->Release();
}

void OvenDatabase::SaveWorkEndPressure(OVEN_WORK work, int endType)
{
	if( MainDB == nullptr )
		return;

	if( MainDB->Connected == false )
		return;

	m_csLock->Acquire();

	String sql;
	String Value = "SET [EndTime] = '";
	Value += work.EndTime.FormatString("yyyy-mm-dd hh:nn:ss") + "', ";
	Value += "[EndType] = " + IntToStr(endType);
	Value += ", [PressingTime] = " + IntToStr(work.PressingTime);
	Value += ", [DepressingTime] = " + IntToStr(work.DepressingTime);

	String WHERE = " WHERE [ChamberNo]=" + IntToStr(work.ChamberNo) + " and ";
	WHERE += "[Recipe]='" + work.Recipe + "' and ";
	WHERE += "[PtnNo]=" + IntToStr(work.PtnNo) + " and ";

	// 10초 전후
	String time;
	TDateTime startTime = work.StartTime;
	TDateTime beforeTime = IncSecond(startTime, -10);
	TDateTime afterTime = IncSecond(startTime, 10);

	time.sprintf(L"(StartTime>=%f and StartTime<=%f);", beforeTime, afterTime);
	WHERE += time;

	sql = "UPDATE WorkTbl " + Value + WHERE;

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add(sql);
	QryMain->Prepared = true;
	QryMain->ExecSQL();
	m_csLock->Release();
}

bool OvenDatabase::GetWorkNotEnd(int chamberNo, OVEN_WORK &work)
{
	if( MainDB == nullptr )
		return false;

	if( MainDB->Connected == false )
		return false;

	m_csLock->Acquire();

	bool ret = true;
	String WHERE, sChamber;
	WHERE.sprintf(L" WHERE ChamberNo= %d ORDER BY StartTime DESC", chamberNo);

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add("SELECT * from WorkTbl " + WHERE);
	QryMain->Open();

	if( QryMain->RecordCount > 0 )
	{
		work.StartTime = QryMain->FieldByName("StartTime")->AsDateTime;
		work.EndTime = QryMain->FieldByName("EndTime")->AsDateTime;

		if( work.StartTime < work.EndTime )
			ret = false;

		work.Recipe = QryMain->FieldByName("Recipe")->AsString;
		work.PtnNo = QryMain->FieldByName("PtnNo")->AsInteger;
		work.CureTemp = QryMain->FieldByName("CureTemp")->AsInteger;
		work.CureTime = QryMain->FieldByName("CureTime")->AsInteger;
		work.Remote = QryMain->FieldByName("Remote")->AsInteger;
		work.UserID = QryMain->FieldByName("UserID")->AsString;
		work.ChamberNo = QryMain->FieldByName("ChamberNo")->AsInteger;
		work.Filename = QryMain->FieldByName("Filename")->AsString;
	}
	else
		ret = false;

	m_csLock->Release();
	return ret;
}

bool OvenDatabase::GetWorkNotEnd(int chamberNo, OVEN_WORK &work, LOTINFO &lotInfo)
{
	if( MainDB == nullptr )
		return false;

	if( MainDB->Connected == false )
		return false;

	m_csLock->Acquire();

 	bool ret = true;
	String WHERE, sChamber;
	WHERE.sprintf(L" WHERE ChamberNo= %d ORDER BY StartTime DESC", chamberNo);

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add("SELECT * from WorkTbl " + WHERE);
	QryMain->Open();

	if( QryMain->RecordCount > 0 )
	{
		work.StartTime = QryMain->FieldByName("StartTime")->AsDateTime;
		work.EndTime = QryMain->FieldByName("EndTime")->AsDateTime;

		if( work.StartTime < work.EndTime )
			ret = false;

		work.Recipe = QryMain->FieldByName("Recipe")->AsString;
		work.PtnNo = QryMain->FieldByName("PtnNo")->AsInteger;
		work.CureTemp = QryMain->FieldByName("CureTemp")->AsInteger;
		work.CureTime = QryMain->FieldByName("CureTime")->AsInteger;
		work.Remote = QryMain->FieldByName("Remote")->AsInteger;
		work.UserID = QryMain->FieldByName("UserID")->AsString;
		work.ChamberNo = QryMain->FieldByName("ChamberNo")->AsInteger;
		work.Filename = QryMain->FieldByName("Filename")->AsString;

		for(int i=0; i<LOT_COUNT; i++)
		{
			lotInfo.LotID[i] = "";
			lotInfo.LotQuantity[i] = 0;
		}

		String str;

		lotInfo.TotalLotCount = QryMain->FieldByName("TotalLotCount")->AsInteger;
		for(int i=0; i<lotInfo.TotalLotCount; i++)
		{
			str.sprintf(L"LotID_%d", i);
			lotInfo.LotID[i] = QryMain->FieldByName(str)->AsString;
			str.sprintf(L"LotCount_%d", i);
			lotInfo.LotQuantity[i] = QryMain->FieldByName(str)->AsInteger;
		}
	}
	else
		ret = false;

	m_csLock->Release();
	return ret;
}

bool OvenDatabase::GetWorkEnd(int chamberNo, OVEN_WORK &work)
{
	if( MainDB == nullptr )
		return false;

	if( MainDB->Connected == false )
		return false;

	m_csLock->Acquire();

	bool ret = true;
	String WHERE, sChamber;
	WHERE.sprintf(L" WHERE ChamberNo= %d ORDER BY StartTime DESC", chamberNo);

	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add("SELECT * from WorkTbl " + WHERE);
	QryMain->Open();

	if( QryMain->RecordCount > 0 )
	{
		work.StartTime = QryMain->FieldByName("StartTime")->AsDateTime;
		work.EndTime = QryMain->FieldByName("EndTime")->AsDateTime;
		work.Recipe = QryMain->FieldByName("Recipe")->AsString;
		work.PtnNo = QryMain->FieldByName("PtnNo")->AsInteger;
		work.CureTemp = QryMain->FieldByName("CureTemp")->AsInteger;
		work.CureTime = QryMain->FieldByName("CureTime")->AsInteger;
		work.Remote = QryMain->FieldByName("Remote")->AsInteger;
		work.UserID = QryMain->FieldByName("UserID")->AsString;
		work.ChamberNo = QryMain->FieldByName("ChamberNo")->AsInteger;
		work.Filename = QryMain->FieldByName("Filename")->AsString;
		if( work.StartTime < work.EndTime )
			ret = true;
	}
	else
		ret = false;

	m_csLock->Release();
	return ret;
}

bool OvenDatabase::QueryWork(int chamberNo, TDateTime start, TDateTime end)
{
	if( MainDB == nullptr )
		return false;

	if( MainDB->Connected == false )
		return false;

	m_csLock->Acquire();
	Query.reset(new TADOQuery(MainDB));

	String WHERE;
	WHERE.sprintf(L" WHERE (StartTime >= %f and StartTime < %f) and ChamberNo = %d", start, end, chamberNo);

	Query->Connection = MainDB;
	Query->Close();
	Query->SQL->Clear();
	Query->SQL->Add("SELECT * from WorkTbl " + WHERE);
	Query->Open();

	m_csLock->Release();
	return true;
}

bool OvenDatabase::IsDataFileExist(int chamberNo, TDateTime date)
{
	if( MainDB == nullptr )
		return false;

	if( MainDB->Connected == false )
		return false;

	m_csLock->Acquire();

	bool ret = false;
	String WHERE, filename;
	filename.sprintf(L"%c_", 'A' + chamberNo);
	filename += date.FormatString("yyyy_mm_dd");
	WHERE.sprintf(L" WHERE Filename like '%s%%' and ChamberNo = %d", filename.c_str(), chamberNo);

	QryMain->Connection = MainDB;
	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add("SELECT * from WorkTbl " + WHERE);
	QryMain->Open();

	if( QryMain->RecordCount > 0 )
		ret = true;

	m_csLock->Release();
	return ret;
}

int OvenDatabase::GetDataFileCount(int chamberNo, TDateTime date)
{
	int cnt = 0;
	if( MainDB == nullptr )
		return cnt;

	if( MainDB->Connected == false )
		return cnt;

	m_csLock->Acquire();

	String WHERE, filename;
	filename.sprintf(L"%c_", 'A' + chamberNo);
	filename += date.FormatString("yyyy_mm_dd");
	WHERE.sprintf(L" WHERE Filename like '%s%%' and ChamberNo = %d", filename.c_str(), chamberNo);

	QryMain->Connection = MainDB;
	QryMain->Close();
	QryMain->SQL->Clear();
	QryMain->SQL->Add("SELECT * from WorkTbl " + WHERE);
	QryMain->Open();

	cnt = QryMain->RecordCount;
	m_csLock->Release();
	return cnt;
}

bool OvenDatabase::QueryProductCode(int chamberNo, String productCode)
{
	if( MainDB == nullptr )
		return false;

	if( MainDB->Connected == false )
		return false;

	m_csLock->Acquire();
	Query.reset(new TADOQuery(MainDB));

	String WHERE, query, str, pattern;

	pattern.sprintf(L"%%%s%%", productCode.c_str());
//	WHERE.sprintf(L" WHERE ([ProductCode] = '%s'", productCode);
	WHERE.sprintf(L" WHERE ([ProductCode] LIKE '%s'", pattern.c_str());

	if( IsFieldExisted("WorkTbl", "LotID_0") )
	{
		for(int i=0; i<LOT_COUNT; i++)
		{
//			str.sprintf(L" or [LotID_%d] = '%s'", i, productCode);
			str.sprintf(L" or [LotID_%d] LIKE '%s'", i, pattern.c_str());
			WHERE += str;
		}
	}
	str.sprintf(L") and [ChamberNo] = %d", chamberNo);

	WHERE += str;
	query = "SELECT * from WorkTbl" + WHERE;

	Query->Connection = MainDB;
	Query->Close();
	Query->SQL->Clear();
	Query->SQL->Add(query);
	Query->Open();

	m_csLock->Release();
	return true;
}

bool OvenDatabase::QueryRecipe(int chamberNo)
{
	if( MainDB == nullptr )
		return false;

	if( MainDB->Connected == false )
		return false;

	m_csLock->Acquire();

	Query.reset(new TADOQuery(MainDB));

	String WHERE, sChamber;
	WHERE.sprintf(L" WHERE ChamberNo=%d", chamberNo);

	Query->Connection = MainDB;
	Query->Close();
	Query->SQL->Clear();
	Query->SQL->Add("SELECT * from RecipeTbl " + WHERE);
	Query->Open();

	m_csLock->Release();
	return true;
}

bool OvenDatabase::GetRecipeList(int chamber, TStringList *list)
{
	if( QueryRecipe(chamber) )
	{
		if( Query->RecordCount==0 )
			return false;

		String str;
		for(int j=0; j<Query->RecordCount; j++)
		{
			str = Query->FieldByName("Recipe")->AsString;
			list->Add(str);
			Query->Next();
		}
		return true;
	}
	return false;
}

void OvenDatabase::SaveButtonClickLog(String buttonName, int chamberNo)
{
	String str;
	str.sprintf(L"%s button clicked.", buttonName.c_str());
	SaveLog(str, MESSAGES, chamberNo);
}

