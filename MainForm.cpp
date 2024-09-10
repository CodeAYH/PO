//---------------------------------------------------------------------------
#pragma hdrstop

#include "MainForm.h"
#include "NormalOvenSystemManager.h"
#include "HelperFunctions.h"
#include "LoginForm.h"
#include "Database.h"
#include "JobForm.h"
#include "AlarmForm.h"
#include "JobEndForm.h"
#include "GEMForm.h"
#include "BarunMES.h"
#include "TCardChamberRecipeForm.h"
#include "TCardValidationForm.h"
#include "EzNetForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvGlassButton"
#pragma link "AdvSmoothTouchKeyBoard"
#pragma resource "*.dfm"
TFmMain *FmMain;
//---------------------------------------------------------------------------

GemDlgPtr GemDlg;
BarunMESPtr BarunMESDlg;

__fastcall TFmMain::TFmMain(TComponent* Owner)
	: TForm(Owner)
{
	Caption = UpperCase(Application->Title) + " V" + GetInformation(Application->ExeName, "FileVersion");
	ClickCount = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFmMain::Localize()
{
	String str;
	str = GetResStr("Form", "Operator Login");
	BtnOperatorLogin->Caption = str;
	str = GetResStr("Form", "Engineer Login");
	BtnEngineerLogin->Caption = str;
	str = GetResStr("Form", "Exit");
	BtnExit->Caption = str;
}
//---------------------------------------------------------------------------
void __fastcall TFmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	SetMessageStr(GetResStr("Question", "Exit Program"));
	if( ShowQuestionDlg() )
	{
		if( GetManager()->LoginLevel <= LV_SUPERVISOR )
		{
			if( ShowLogin(LV_SUPERVISOR, false) )
				CanClose = true;
			else
				CanClose = false;
		}
		else
			CanClose = true;
	}
	else
	{
		CanClose = false;
	}

    if( CanClose )
	{
		String str;

		KillGEMProgram(L"GEM.exe");

		str = "Prgram Terminated.";
		GetOvenDB()->SaveLog(str, MESSAGES, -1);
	}

	// 20180212 - FTP (OFILM MES) - jhlim
	if(CanClose == true)
	{
		GetManager()->DeleteFTPDataFile();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFmMain::SetDateTimeFormat()
{
	FormatSettings.ShortDateFormat = "YYYY-MM-DD";
	FormatSettings.LongDateFormat = "YYYY-MM-DD";
	FormatSettings.ShortTimeFormat = "hh:mm:ss";
	FormatSettings.LongTimeFormat = "hh:mm:ss";
	FormatSettings.DateSeparator = '-';
}
//---------------------------------------------------------------------------
void __fastcall TFmMain::AppSettingChange(System::TObject* Sender, int Flag, const System::UnicodeString Section, int &Result)
{
	SetDateTimeFormat();
}
//---------------------------------------------------------------------------
void __fastcall TFmMain::FormCreate(TObject *Sender)
{
	// http://devquest.co.kr/impboard/impboard.dll?action=read&db=bcb_tip&no=885
	// 프로그램 시작시 Application->OnException 이벤트 핸들러를 걸어준다.
	Application->OnException = AppException;

	String title, str, tmp, tmp2;

	LogStartTime = Now();

	Application->OnSettingChange = AppSettingChange;
	SetDateTimeFormat();

	title = UpperCase(Application->Title);
	GetManager()->InitDirectory(title);

	LoadLanguageStr();

	GetOvenDB()->ConnectDB();
	GetOvenDB()->ConnectErrDB();

	str = "Program Started.";
	GetOvenDB()->SaveLog(str, MESSAGES);

	if( GetOvenDB()->IsFieldExisted("WorkTbl", "ProductCode") == false )
		GetOvenDB()->AddStringField("WorkTbl", "ProductCode", 100);
	if( GetOvenDB()->IsFieldExisted("WorkTbl", "TotalLotCount") == false )
		GetOvenDB()->AddIntField("WorkTbl", "TotalLotCount");
	for(int i=0; i<LOT_COUNT; i++)
	{
		str.sprintf(L"LotID_%d", i);
		if( GetOvenDB()->IsFieldExisted("WorkTbl", str) == false )
			GetOvenDB()->AddStringField("WorkTbl", str, 100);

		str.sprintf(L"LotCount_%d", i);
		if( GetOvenDB()->IsFieldExisted("WorkTbl", str) == false )
			GetOvenDB()->AddIntField("WorkTbl", str);
	}

	GetManager()->LoadSystemParameters();
	GetManager()->LoadTwrValue();
	GetManager()->CreateChamber();
	GetManager()->CreateSerialDevices();

	if( GetManager()->UseAutomation )
	{
		int kind = GetManager()->AutomationKind;
		if( kind == SCK_MES )
		{
			GetManager()->CreateSCKRMSThread();	//2021-12-15
		}
	}
	Localize();

	NormalOvenChamber *chamber;
	if( GetManager()->UseAutomation )
	{
		int kind = GetManager()->AutomationKind;
		if (GetManager()->IsUseGem())
		{
			GemDlg.reset(new TFmGem(nullptr));
		}
		else
		if( kind == BARUN_MES )
		{
			BarunMESDlg.reset(new TFmBarunMES(GetDataFolder(), nullptr));

			for(int i=0; i<GetManager()->ChamberCount; i++)
			{
				chamber = GetChamber(i);
				chamber->BarunManager->SetMachineName(chamber->MachineID);
				chamber->BarunManager->Connect();
			}
		}
		// 20191216 - SCK MES - jhlim
		else
		if( kind == SCK_MES )
		{
			// WORK / WORK_DONE Folder Check & Create
			chamber->MakeFolder_MESLotInfo();
		}
		else
		if( kind == SECS_GEM_SCK)
		{
			String ini = GetDataFolder() + "EzGem.Ini";
			const std::auto_ptr<TIniFile> GemFile(new TIniFile(ini));
			const String sec = "GENERAL";

			str = UpperCase(Application->Title);
			GemFile->WriteString(sec, "MODEL NAME", str);
			str = GetInformation(Application->ExeName, "ProductVersion");
			GemFile->WriteString(sec, "SW VERSION", str);

 			KillGEMProgram(L"GEM.exe");     //Add GEM 프로그램 실행전 Kill
			Sleep(500);

			for (int i = 0; i < GetManager()->ChamberCount; i++)
			{
				EzNetDlg[i].reset(new TFmEzNet(nullptr, GetChamber(i)));

				if (RunGEMProgram(L"GEM.exe", i) == false)
				{
					str.sprintf(L"[Chamber %c] ", 'A' + i);
					str += GetResStr("Warning", "GemRunFail");
					SetMessageStr(str);
					ShowMessageDlg();
				}
			}
		}
	}

	for(int i=0; i<GetManager()->ChamberCount; i++)
	{
		chamber = GetChamber(i);
		chamber->InitDIO();
		chamber->ClearAlarmAll();
		AlarmDlg[i].reset(new TFmAlarm(nullptr, i));
		JobEndDlg[i].reset(new TFmJobEnd(nullptr, i));

		chamber->RunWatchAlarm();
	}

	if( GetManager()->UseAutomation )
	{
		int kind = GetManager()->AutomationKind;
		if( kind == OFILM_MES )
		{
			GetManager()->RunWatchFtpConnect();
		}
    }

	BtnOperatorLogin->Left = (Width - BtnOperatorLogin->Width)/2;
	BtnEngineerLogin->Left = (Width - BtnEngineerLogin->Width)/2;
	BtnExit->Left = (Width - BtnExit->Width)/2;

	Keyboard->Hide();

	if (title.Pos("_"))
	{
		TReplaceFlags Flags = TReplaceFlags() << rfReplaceAll;
		title = StringReplace(title, "_", "-", Flags);
	}
	title += " Ver ";
	title += GetInformation(Application->ExeName, "ProductVersion");
	Caption = title;
}
//---------------------------------------------------------------------------
void __fastcall TFmMain::BtnOperatorLoginClick(TObject *Sender)
{
	if( ShowLogin(LV_OPERATOR, true) )
	{
		Hide();
		FmJob->Show();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmMain::BtnEngineerLoginClick(TObject *Sender)
{
	if( ShowLogin(LV_ENGINEER, true) )
	{
		Hide();
		FmJob->Show();

		if (GetManager()->AutoModeChangeTimer > 0) //300 Sec
		{
			EnableAutoModeChangeTimer(GetManager()->AutoModeChangeTimer);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmMain::BtnExitClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFmMain::FormClick(TObject *Sender)
{
	ClickCount++;
	ClickCancel->Enabled = true;

	if( ClickCount>5 )
	{
		if( ShowLogin(LV_DEVELOPER, true) )
		{
			Hide();
			FmJob->Show();
		}
    }
}
//---------------------------------------------------------------------------
void __fastcall TFmMain::FormShow(TObject *Sender)
{
	Localize();
}
//--------------------------------------------------------------------------
void __fastcall TFmMain::ClickCancelTimer(TObject *Sender)
{
	ClickCount = 0;
	ClickCancel->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TFmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	String str;
	str = "Program Terminated.";
	GetOvenDB()->SaveLog(str, MESSAGES);
	FmJob->Close();
	Sleep(1000);
}
//---------------------------------------------------------------------------
void __fastcall TFmMain::OnHanaReceivePtn(TMessage &Msg)
{
	int nCmIdx;

	nCmIdx = static_cast<int>(Msg.WParam);
 //	ChamberPtr SelChamber = GetChamber(nCmIdx);
	FmJob->ChamberFrame[nCmIdx]->SelectReceiveHanaRecipe();
}

void __fastcall TFmMain::OnHanaOvenStartStop(TMessage &Msg)
{
	int nCmIdx;
	String str;

	nCmIdx = static_cast<int>(Msg.WParam);

	NormalOvenChamber *chamber;
	chamber = GetChamber(nCmIdx);
	FmJob->ChamberFrame[nCmIdx]->HanaReceiveCureStart();

	str.sprintf(L"HANA OVEN CHM[%d]: OnHanaOvenStartStop()", nCmIdx);
	SaveDebugLog(str);
}
//---------------------------------------------------------------------------
void __fastcall TFmMain::OnSCK_RMSStart(TMessage &Msg)
{
	String str;
	if (Msg.LParam == INVALID_FILE)
	{
		str.sprintf(L"[MES]\n\rInvalid file name.\n\rPlease check the file name.");
		SetMessageStr(str);
		ShowMessageDlg();
		return;
	}
	else
	{
		if( GetManager()->UseAutomation == true &&
			GetManager()->AutomationKind == SCK_MES )
		{
			int nCmIdx;

			nCmIdx = static_cast<int>(Msg.WParam);

			NormalOvenChamber *chamber;
			chamber = GetChamber(nCmIdx);
			FmJob->ChamberFrame[nCmIdx]->SCK_RMSStart();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmMain::EnableAutoModeChangeTimer(int interval)
{
	AutoModeChange->Enabled = false;

	String str;
	if (interval > 0)
	{
		AutoModeChange->Enabled = true;
		AutoModeChange->Interval = interval * 1000;
		AutoModeChange->Tag = AutoModeChange->Interval;
	}
}
//2021-12-22 hcw.
void __fastcall TFmMain::AutoModeTimer(TObject *Sender)
{
	AutoModeChange->Enabled = false;

	if (GetManager()->AutoModeChangeTimer > 0)
	{
		if (GetManager()->LoginLevel  >= LV_ENGINEER)
		{
			GetManager()->LoginLevel = LV_OPERATOR;
			GetManager()->UserID = "Operator";
			String str;
			str = String(LevelStr[GetManager()->LoginLevel]);
			str.UpperCase();
			if( FmJob->LoginModePanel->Text != str )
				FmJob->LoginModePanel->Text = str;

			//2022-03-10 hcw.
			if( GetManager()->UseAutomation)
			{
				if(GetManager()->AutomationKind == FUJITSU_TCARD ||
				   GetManager()->AutomationKind == SECS_GEM_SCK)
				{
					for(int i=0; i<GetManager()->ChamberCount; i++)
					{
						FmJob->ChamberFrame[i]->AutoModeButtonEnable(false);
					}
				}
			}
		}
	}

	AutoModeChange->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFmMain::OnAlarmSet(TMessage &Msg)
{
	int chamberNo = Msg.WParam;
	int errorCode = Msg.LParam;
	DB_ERROR error;
	NormalOvenChamber *chamber = GetChamber(chamberNo);
	if (chamber == nullptr)
	{
		return;
	}

	if (GetOvenDB()->GetError(errorCode, error, GetManager()->Language))
	{
		GetOvenDB()->SaveLog(error.Name, ALARM, chamber->ChamberID);
	}

	if (GetManager()->UseAutomation)
	{
		if (GetManager()->IsUseGem() &&
			GemDlg)
		{
			GemDlg->SetAlarm(chamber->ChamberID, errorCode);
		}
		else
		if (GetManager()->AutomationKind == BARUN_MES &&
			BarunMESDlg &&
			chamber->UseManualMode == false &&
			BarunMESDlg->CimParam.OnlineMode == true &&
			chamber->BarunManager)
		{
			chamber->BarunManager->SendAlarm(chamber->OperatorID, errorCode);
		}
		else
		if (GetManager()->UseAutomation == true &&
			GetManager()->AutomationKind == SECS_GEM_SCK &&
			EzNetDlg[chamber->ChamberID]->IsGemConnected())
		{
			EzNetDlg[chamber->ChamberID]->SetAlarm(errorCode);
        }
	}
}
//---------------------------------------------------------------------------
// Exception ===============================================
void  __fastcall TFmMain::AppException(TObject* Sender,  Exception* E)
{
	// http://devquest.co.kr/impboard/impboard.dll?action=read&db=bcb_tip&no=885
	String strObj = L"" ;
	if(Sender)
	{
		if(Sender->InheritsFrom(__classid( TControl) ) )
		{
			TControl  * ctrl= (TControl *) Sender;
			while(ctrl)
			{
				strObj = ctrl->Name+ L" -> " + strObj;
				ctrl = ctrl->Parent;
			}
		}
		else if(Sender->InheritsFrom(__classid(TComponent) ) )
		{
			TComponent* Comp= (TComponent *)Sender;
			strObj = Comp->Name;
			if(Comp->Owner)
			{
				strObj = Comp->Owner->Name +  L" -> " +  strObj;
			}
		}
	}

	String strExpMsg = strObj +  L" Msg:"  + E->Message;

	// Log를 저장하거나.. SaveLog .
	// 필요에 따라 메세지를 뿌려준다.
	E->Message = strExpMsg;
	SaveDebugLog(strExpMsg);
	Application->ShowException(E) ;
}
//---------------------------------------------------------------------------

