//---------------------------------------------------------------------------
#pragma hdrstop

#include "AlarmForm.h"
#include "Database.h"
#include "HelperFunctions.h"
#include "NormalOvenChamber.h"
#include "NormalOvenSystemManager.h"
#include "JobForm.h"
#include "GEMForm.h"
#include "EzNetForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvGlassButton"
#pragma resource "*.dfm"

enum { DIR_START, DIR_PREV, DIR_NEXT };

//---------------------------------------------------------------------------
__fastcall TFmAlarm::TFmAlarm(TComponent* Owner, int chamberNo)
    : TForm(Owner)
{
	Chamber = GetChamber(chamberNo);
	ChamberNo = chamberNo;
	AlignComps();
	Localize();

	int count = GetManager()->ChamberCount;

	Index = 0;
	Left = (1024-Width)/count + chamberNo*50;
	Top = (768-Height)/count + chamberNo*50;

	Caption = UpperCase(Application->Title);

	DoBuzzerOn = false;
}
//---------------------------------------------------------------------------
void __fastcall TFmAlarm::AlignComps()
{
	this->BorderStyle = Forms::bsSingle;
    TBorderIcons tempBI = BorderIcons;
    tempBI >> biMaximize >> biMinimize >> biHelp;
	BorderIcons = tempBI;
}
//---------------------------------------------------------------------------
void __fastcall TFmAlarm::Localize()
{
	pnlCause->Caption   = GetResStr("Common", "Cause");
	pnlAction->Caption  = GetResStr("Common", "Action");

    stCause->Caption  = "";
    stAction->Caption = "";
	btnAllClear->Caption = GetResStr("Common", "AllClear");
	btnClear->Caption = GetResStr("Common", "Clear");
	btnShowPrevError->Caption = GetResStr("Common", "Prev");
	btnShowNextError->Caption = GetResStr("Common", "Next");
	btnBuzzerOff->Caption = GetResStr("Form", "BuzzerOff");
	btnHide->Caption = GetResStr("Common", "Hide");
}
//---------------------------------------------------------------------------
void __fastcall TFmAlarm::BlinkTimer(TObject *Sender)
{
	if( Chamber == nullptr )
		return;

	Blink->Enabled = false;

	if( Color != clRed )
		Color = clRed;
	else
		Color = clYellow;

	if( Chamber->GetAlarmCount() > 0 )
	{
		if( Chamber->IsBuzzerOn() )
			Chamber->BuzzerOffSwitchOn(true);
		else
			Chamber->BuzzerOffSwitchOn(false);
	}
	else
	{
		Chamber->BuzzerOffSwitchOn(false);
	}

	if( DoBuzzerOn )
	{
		if (GetManager()->UseTowerLampControl)
		{
			int mode = GetManager()->TowerLampOption->GetTwrVal(TWR_BUZ, TWR_ERR);
			if (mode == BUZZ_OFF)
			{
				Chamber->BuzzerOff();
			}
			else
			{
				Chamber->BuzzerOn(BUZZER_SOUND);
			}
		}
		else
		{
			Chamber->BuzzerOn(BUZZER_SOUND);
		}

		DoBuzzerOn = false;
	}

	Chamber->DoTowerLampControl(Chamber->IsBlink);

	Blink->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFmAlarm::btnClearClick(TObject *Sender)
{
	int code = Chamber->GetAlarm(Index);

    String str;
	str = "Alarm Clear Button Clicked.";
	GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);

	if( GetManager()->UseAutomation &&
		GetManager()->IsUseGem() &&
		GemDlg && GemDlg->IsGemConnected() )
	{
		GemDlg->ClearAlarm(Chamber->ChamberID, code);
	}
	else
	if (GetManager()->UseAutomation == true &&
		GetManager()->AutomationKind == SECS_GEM_SCK && EzNetDlg[ChamberNo] != nullptr &&
		EzNetDlg[Chamber->ChamberID]->IsGemConnected())
	{
		EzNetDlg[Chamber->ChamberID]->ClearAlarm(code);
	}

	Chamber->ClearAlarm(code);
	UpdateInfo(0);

	if( Chamber->GetAlarmCount() == 0 )
		Close();
}
//---------------------------------------------------------------------------
void __fastcall TFmAlarm::btnAllClearClick(TObject *Sender)
{
    String str;
	str = "Alarm All Clear Button Clicked.";
	GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);

	if( GetManager()->UseAutomation &&
		GetManager()->IsUseGem() &&
		GemDlg && GemDlg->IsGemConnected() )
	{
		GemDlg->ClearAlarmAll(Chamber->ChamberID);
	}
	else
	if (GetManager()->UseAutomation == true &&
		GetManager()->AutomationKind == SECS_GEM_SCK && EzNetDlg[ChamberNo] != nullptr &&
		EzNetDlg[Chamber->ChamberID]->IsGemConnected())
	{
		EzNetDlg[Chamber->ChamberID]->ClearAlarmAll();
	}

	Chamber->ClearAlarmAll();
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFmAlarm::FormShow(TObject *Sender)
{
	if (GetManager()->UseLocalN2Alarm)  //2021-09-09 AMD China Local Mode N2 Option Check
	{
		if (GetManager()->LoginLevel <= LV_OPERATOR)
		{
			btnHide->Visible = false;
		}
		else
		{
			btnHide->Visible = true;
		}
	}
	else
	{
		btnHide->Visible = true;
	}

	UpdateInfo(Index);
	DoBuzzerOn = true;
}
//---------------------------------------------------------------------------
void __fastcall TFmAlarm::UpdateInfo(int idx)
{
	unsigned int alarmCode;

	alarmCode = Chamber->GetAlarm(idx);
	Index = idx;

	String sTmp, sAction;
	pnlErrNo->Caption = alarmCode;

	DB_ERROR error;
	if( GetOvenDB()->GetError(alarmCode, error, GetManager()->Language) )
	{
		sTmp.sprintf(L"[CHAMBER %c] %s", 'A'+Chamber->ChamberID, error.Name.c_str());
		pnlErrName->Caption = sTmp;
		stCause->Caption = error.Cause;
		sAction = error.Action;
	}

	if( sAction.Pos(">") )
	{
		TReplaceFlags Flags = TReplaceFlags() << rfReplaceAll;
		sAction = StringReplace(sAction, ">", "\n", Flags);
	}
	stAction->Caption = sAction;
}
//---------------------------------------------------------------------------
void __fastcall TFmAlarm::btnBuzzerOffClick(TObject *Sender)
{
    String str;
	str = "Buzzer Off Button Clicked.";
	GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);

	Chamber->BuzzerOff();
}
//---------------------------------------------------------------------------
void __fastcall TFmAlarm::btnHideClick(TObject *Sender)
{
	String str;
	str = "Alarm Hide Button Clicked.";
	GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFmAlarm::btnShowPrevErrorClick(TObject *Sender)
{
    String str;
	str = "Alarm Prev Button Clicked.";
	GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);

	if( Index > 0 )
		Index--;
	UpdateInfo(Index);
}
//---------------------------------------------------------------------------
void __fastcall TFmAlarm::btnShowNextErrorClick(TObject *Sender)
{
    String str;
	str = "Alarm Next Button Clicked.";
	GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);

	if( Index < Chamber->GetAlarmCount() - 1 )
		Index++;
	UpdateInfo(Index);
}
//---------------------------------------------------------------------------
void __fastcall TFmAlarm::UpdateTimer(TObject *Sender)
{
	Update->Enabled = false;

    ViewAllClearButton();

	int cnt = Chamber->GetAlarmCount();
	if( Index == 0 )
		btnShowPrevError->Visible = false;
	else
		btnShowPrevError->Visible = true;
	if( Index < cnt-1 )
		btnShowNextError->Visible = true;
	else
		btnShowNextError->Visible = false;

	String sTmp;
	sTmp.sprintf(L"%d/%d", Index+1, cnt);
	pnlCount->Caption = sTmp;

	Update->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFmAlarm::FormClose(TObject *Sender, TCloseAction &Action)
{
	Chamber->BuzzerOff();
}
//---------------------------------------------------------------------------
void __fastcall TFmAlarm::ViewAllClearButton()
{
	bool visible = true;
	if (GetManager()->UseAutomation == true &&
		GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		visible = false;
	}

	if (btnAllClear->Visible != visible)
	{
		btnAllClear->Visible = visible;
	}
}
//---------------------------------------------------------------------------

