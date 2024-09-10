//---------------------------------------------------------------------------
#pragma hdrstop

#include <System.DateUtils.hpp>
#include "HelperFunctions.h"
#include "JobEndForm.h"
#include "NormalOvenChamber.h"
#include "NormalOvenSystemManager.h"
#include "Database.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "EllipsLabel"
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "BaseGrid"
#pragma link "AdvGlassButton"
#pragma link "AdvUtil"
#pragma resource "*.dfm"
TFmJobEnd *FmJobEnd;
//---------------------------------------------------------------------------
__fastcall TFmJobEnd::TFmJobEnd(TComponent* Owner, int chamberNo)
	: TForm(Owner)
{
	Chamber = GetChamber(chamberNo);
	btnOk->Caption = GetResStr("Common", "Confirm");

	Caption = UpperCase(Application->Title);

	Left = (1024-Width)/2 + chamberNo*150;
	Top = (768-Height)/2 + chamberNo*150;

	JobInfoGrid->Cells[0][0] = GetResStr("JobEnd", "Recipe");
	JobInfoGrid->Cells[0][1] = GetResStr("JobEnd", "JobTemp");
	JobInfoGrid->Cells[0][2] = GetResStr("JobEnd", "JobTime");
	JobInfoGrid->Cells[0][3] = GetResStr("JobEnd", "UserId");
	JobInfoGrid->Cells[0][4] = GetResStr("JobEnd", "StartTime");
	JobInfoGrid->Cells[0][5] = GetResStr("JobEnd", "EndTime");

	JobInfoGrid->ColWidths[0] = 130;
	JobInfoGrid->ColWidths[1] = 350;
}
//---------------------------------------------------------------------------
void __fastcall TFmJobEnd::FormShow(TObject *Sender)
{
	if( Chamber == nullptr )
		return;

	AnsiString str;

	JobInfoGrid->Cells[1][0] = Chamber->Recipe.RecipeName;
	str.sprintf("%.1f", Chamber->Recipe.TempPtn.MaxTemp);
	JobInfoGrid->Cells[1][1] = str;
	JobInfoGrid->Cells[1][2] = IntToStr(Chamber->Recipe.TempPtn.TotalTime);
	JobInfoGrid->Cells[1][3] = GetManager()->UserID;
	JobInfoGrid->Cells[1][4] = Chamber->Recipe.StartTime.FormatString("yyyy-mm-dd hh:nn:ss");
	JobInfoGrid->Cells[1][5] = EndTime.FormatString("yyyy-mm-dd hh:nn:ss");

	if (GetManager()->UseTowerLampControl)
	{
		int mode = GetManager()->TowerLampOption->GetTwrVal(TWR_BUZ, TWR_LOTEND);
		if (mode == BUZZ_OFF)
		{
			Chamber->BuzzerOff();
		}
		else
		{
			Chamber->BuzzerOn(BUZZER_SIREN);
		}
	}
	else
	{
		Chamber->BuzzerOn(BUZZER_SIREN);
	}

	if (GetManager()->JobEndTimer > 0)
	{
		BuzzerOffTime->Interval = GetManager()->JobEndTimer * 1000;
		BuzzerOffTime->Enabled = true;
	}
	else
	{
		BuzzerOffTime->Enabled = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmJobEnd::btnOkClick(TObject *Sender)
{
	String str;
	str = "JobEnd Close Button Click";
	GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFmJobEnd::FormClose(TObject *Sender, TCloseAction &Action)
{
	if( Chamber == nullptr )
		return;

	BuzzerOffTime->Enabled = false;
	Chamber->BuzzerOff();
}
//---------------------------------------------------------------------------
void __fastcall TFmJobEnd::BuzzerOffTimeTimer(TObject *Sender)
{
	if( Chamber == nullptr )
		return;

	BuzzerOffTime->Enabled = false;
	Chamber->BuzzerOff();
}
//---------------------------------------------------------------------------
void __fastcall TFmJobEnd::UpdateInfo(TDateTime end, int endType, bool remote)
{
	EndTime = end;

	String sChamber;

	sChamber.sprintf(L"[CHAMBER %c] ", Chamber->ChamberID+'A');

	if( endType == NORMAL_STOP )
		sChamber += GetResStr("JobEnd", "CureDone");
	else
	if( endType == FORCED_STOP )
		sChamber += GetResStr("JobEnd", "ForcedDone");
	else
	if( endType == LOWPRESSURE_STOP )
		sChamber += GetResStr("JobEnd", "LowPressureDone");
	else
	if( endType == EMERGENCY_STOP )
		sChamber += GetResStr("JobEnd", "EmergencyDone");
	else
	if( endType == MDCOOLFAIL_STOP )
		sChamber += GetResStr("JobEnd", "MDCoolAlarmDone");
	else
	if( endType == O2FAIL_STOP )
		sChamber += GetResStr("JobEnd", "O2AlarmDone");

	if( remote )
		sChamber += " [Remote]";

	GetOvenDB()->SaveLog(sChamber, MESSAGES, Chamber->ChamberID);

    lblChamber->Caption = sChamber;
	lblChamber->Font->Color = clPurple;
}
