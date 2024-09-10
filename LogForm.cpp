//---------------------------------------------------------------------------
#pragma hdrstop

#include "LogForm.h"
#include "JobForm.h"
#include "Database.h"
#include "HelperFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvDateTimePicker"
#pragma link "AdvGlassButton"
#pragma link "AdvGrid"
#pragma link "AdvGroupBox"
#pragma link "AdvObj"
#pragma link "AdvOfficeButtons"
#pragma link "AdvPanel"
#pragma link "BaseGrid"
#pragma link "DBAdvGrid"
#pragma link "AdvUtil"
#pragma resource "*.dfm"
TFmLog *FmLog;
//---------------------------------------------------------------------------
__fastcall TFmLog::TFmLog(TComponent* Owner)
	: TForm(Owner)
{
	Caption = UpperCase(Application->Title);
}
//---------------------------------------------------------------------------

void __fastcall TFmLog::Localize()
{
	SelectLog->Items->Clear();
	SelectLog->Items->Add(GetResStr("Report", "AllLog"));
	SelectLog->Items->Add(GetResStr("Report", "Alarm"));
	SelectLog->Items->Add(GetResStr("Report", "Message"));

	AllTime->Caption = GetResStr("Report", "AllLog");
	BtnClose->Caption = GetResStr("Common", "Close");
}

void __fastcall TFmLog::InitGrid()
{
	String str;

	LogGrid->Clear();

	str = GetResStr("Report", "Time");
	LogGrid->ColumnHeaders->Add(str);
	str = GetResStr("Report", "Contents");
	LogGrid->ColumnHeaders->Add(str);
	str = GetResStr("Report", "Chamber");
	LogGrid->ColumnHeaders->Add(str);
	str = GetResStr("Report", "Type");
	LogGrid->ColumnHeaders->Add(str);

	LogGrid->ColWidths[0] = 180;
	LogGrid->ColWidths[1] = 1000;
	LogGrid->ColWidths[2] = 60;
	LogGrid->ColWidths[3] = 60;

//	LogGrid->ScrollBars = ssBoth;
}
//---------------------------------------------------------------------------
void __fastcall TFmLog::FormShow(TObject *Sender)
{
	Localize();
	InitGrid();

	dtPickerStart->Date = Now().CurrentDate();
	dtPickerEnd->Date = Now().CurrentDate();
	SelectLog->ItemIndex = 0;
	AllTime->Checked = false;

	SearchLog();

	String str;
	str = "Log UI Executed.";
	GetOvenDB()->SaveLog(str, MESSAGES);
}
//---------------------------------------------------------------------------
void __fastcall TFmLog::BtnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFmLog::SearchLog()
{
	int selLog, selChamber, selTime, cond;

	selLog = SelectLog->ItemIndex * 2;
	selChamber = 0 * 6;
	selTime = AllTime->Checked==true ? 0:1;

	cond = selLog + selChamber + selTime;
	TDateTime start = StrToDate(dtPickerStart->Date.DateString(), GetDateTimeFormat());
	TDateTime end = StrToDate(TDateTime(dtPickerEnd->Date+1).DateString(), GetDateTimeFormat());
	if( GetOvenDB()->QueryLog(cond, start, end) )
	{
		LogGrid->RowCount = GetOvenDB()->Query->RecordCount + 1;
		for(int i=0; i<GetOvenDB()->Query->RecordCount; i++)
		{
			LogGrid->Cells[0][i+1] = GetOvenDB()->Query->FieldByName("LogTime")->AsString;
			LogGrid->Cells[1][i+1] = GetOvenDB()->Query->FieldByName("Contents")->AsString;

			int tmp = GetOvenDB()->Query->FieldByName("Chamber")->AsInteger;
			if( tmp==0 )
				LogGrid->Cells[2][i+1] = "A";
			else if( tmp==1 )
				LogGrid->Cells[2][i+1] = "B";
			else if( tmp==2 )
				LogGrid->Cells[2][i+1] = "C";
			else if( tmp==3 )
				LogGrid->Cells[2][i+1] = "D";
			else
				LogGrid->Cells[2][i+1] = "";
			tmp = GetOvenDB()->Query->FieldByName("Type")->AsInteger;
			if( tmp==ALARM )
				LogGrid->Cells[3][i+1] = "ALARM";
			else if( tmp==MESSAGES )
				LogGrid->Cells[3][i+1] = "MESSAGE";
			GetOvenDB()->Query->Next();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmLog::AllTimeClick(TObject *Sender)
{
	dtPickerStart->Enabled = AllTime->Checked==true ? false:true;
	dtPickerEnd->Enabled = AllTime->Checked==true ? false:true;
	SearchLog();
}
//---------------------------------------------------------------------------
void __fastcall TFmLog::SelectLogClick(TObject *Sender)
{
	SearchLog();
}
//---------------------------------------------------------------------------
void __fastcall TFmLog::FormClose(TObject *Sender, TCloseAction &Action)
{
	String str;
	str = "Log UI Closed.";
	GetOvenDB()->SaveLog(str, MESSAGES);

	FmJob->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFmLog::SaveToCSVButtonClick(TObject *Sender)
{
	if( SaveTextFileDialog->Execute() )
	{
		LogGrid->SaveToCSV(SaveTextFileDialog->FileName);
	}
}
//---------------------------------------------------------------------------

