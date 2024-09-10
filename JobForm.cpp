//---------------------------------------------------------------------------
#pragma hdrstop

#include "JobForm.h"
#include "NormalOvenChamber.h"
#include "NormalOvenSystemManager.h"
#include "ST340.h"
#include "I7018.h"
#include "DiffPressure.h"
#include "MainTempController.h"

#include "HelperFunctions.h"
#include "MainForm.h"
#include "LoginForm.h"
#include "AlarmForm.h"
#include "JobEndForm.h"
#include "IOFrame.h"
#include "LogForm.h"
#include "RegisterUserForm.h"
#include "TempGraphFrame.h"
#include "ShowPtnForm.h"
#include "AdapterForm.h"
#include "SystemSettingFrame.h"
#include "ChamberSettingFrame.h"
#include "GEMForm.h"
#include "BarunMES.h"
#include "TCardChamberRecipeForm.h"
#include "TCardInputDialog.h"
#include "TCardValidationForm.h"
// 20180227 - FTP (OFILM MES) - jhlim
#include "FTPLotInfoForm.h"
// 20181023 - ScreenSaver (infineon) - jhlim
#include "ScreenSaver.h"
#include "EzNetForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvCombo"
#pragma link "AdvGlassButton"
#pragma link "AdvGrid"
#pragma link "AdvMemo"
#pragma link "AdvMenus"
#pragma link "AdvObj"
#pragma link "AdvOfficeStatusBar"
#pragma link "AdvOfficeStatusBarStylers"
#pragma link "AdvPanel"
#pragma link "AdvSmoothProgressBar"
#pragma link "AdvSmoothStatusIndicator"
#pragma link "AdvStickyPopupMenu"
#pragma link "BaseGrid"
#pragma resource "*.dfm"
TFmJob *FmJob;
AlarmPtr AlarmDlg[MAX_CHAMBER_COUNT];
JobEndPtr JobEndDlg[MAX_CHAMBER_COUNT];
TCardChamberRecipePtr TCardRecipeDlg[MAX_CHAMBER_COUNT];
//TCardValidationPtr TCardValidationDlg[MAX_CHAMBER_COUNT];
//---------------------------------------------------------------------------
__fastcall TFmJob::TFmJob(TComponent* Owner)
	: TForm(Owner)
{
	Color = clBlack;
	ShowDetailMode = false;
	CurrentChamber = CHAMBER_A;

	for(int i=0; i<MAX_CHAMBER_COUNT; i++)
		ChamberFrame[i].reset();

	// 20181022 - ScreenSaver (infineon) - jhlim
	nScreenSaverCnt = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFmJob::FormCreate(TObject *Sender)
{
	int chamberCount = GetManager()->ChamberCount;

	for(int i=0; i<chamberCount; i++)
	{
		ChamberFrame[i] = ChamberFramePtr(new TFrmChamber(nullptr, GetChamber(i)));
		ChamberFrame[i]->Parent = this;
		ChamberFrame[i]->FillUpInfo();
		Sleep(500);
	}

	// 20180209 - FTP (OFILM MES) - jhlim
	if( GetManager()->UseAutomation )
	{
		if( GetManager()->AutomationKind == OFILM_MES )
		{
			// 타이머 활성화
			OFilmMESDataSave->Enabled = true;
			OFilmMESSendFile->Enabled = true;

//			// Chamber LotInfo Button 비활성화
//			for(int i=0; i<chamberCount; i++)
//			{
//				ChamberFrame[i]->LotInfoButton->Visible = false;
//			}
		}

		if (GetManager()->AutomationKind == SECS_GEM_COWELL)
		{
			OPIDEdit->Visible = true;
            OPIDEdit->Text = "";
			OPIDChangeButton->Visible = true;
		}
		else
		{
			OPIDEdit->Visible = false;
			OPIDChangeButton->Visible = false;
		}
	}

	// 20181029 - ScreenSaver (infineon) - jhlim
	if( GetManager()->UseScreenSaver == true )
	{
        ScreenSaverTimer->Enabled = true;
	}
	else
	{
		ScreenSaverTimer->Enabled = false;
	}

	if( GetManager()->UseAutomation &&     //2021-09-29 hcw.
		GetManager()->AutomationKind == FUJITSU_TCARD )
	{
		BtnTCard->Visible = true;
	}
	else
	{
		BtnTCard->Visible = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFmJob::ArrangeChamber()
{
	int chamberCount = GetManager()->ChamberCount;
	int totalWidth = 1024;
	int totalHeight = Height - CaptionPanel->Height - AutomationStatusBar->Height;

	if( chamberCount == 4 )
	{
		for(int i=0; i<chamberCount; i++)
		{
			if( ChamberFrame[i] == nullptr )
				break;
			ChamberFrame[i]->Visible = false;
		}
	}

	switch( chamberCount )
	{
	case 1:
		ChamberFrame[0]->Width = totalWidth;
		ChamberFrame[0]->Height = totalHeight;
		ChamberFrame[0]->Left = 0;
		ChamberFrame[0]->Top = CaptionPanel->Height;
		break;

	case 2:
		for(int i=0; i<chamberCount; i++)
		{
			if( ChamberFrame[i] == nullptr )
				break;
			ChamberFrame[i]->Width = totalWidth;
			ChamberFrame[i]->Height = totalHeight/2;
			ChamberFrame[i]->Left = 0;
			ChamberFrame[i]->Top = i * ChamberFrame[i]->Height + CaptionPanel->Height;
		}
		break;

	case 4:
		for(int i=0; i<chamberCount; i++)
		{
			if( ChamberFrame[i] == nullptr )
				break;

			if( ShowDetailMode )
			{
				if( i == CurrentChamber )
				{
					ChamberFrame[i]->Width = totalWidth;
					ChamberFrame[i]->Height = totalHeight;
					ChamberFrame[i]->Left = 0;
					ChamberFrame[i]->Top = CaptionPanel->Height;
//					ChamberFrame[i]->Visible = true;
				}
//				else
//					ChamberFrame[i]->Visible = false;
			}
			else
			{
				ChamberFrame[i]->Width = totalWidth/2;
				ChamberFrame[i]->Height = totalHeight/2;
				ChamberFrame[i]->Left = ChamberFrame[i]->Width * (i%2);
				ChamberFrame[i]->Top = ChamberFrame[i]->Height * (i/2) + CaptionPanel->Height;
//				ChamberFrame[i]->Visible = true;
			}
		}
		break;
	}

	for(int i=0; i<chamberCount; i++)
	{
		if( ChamberFrame[i] == nullptr )
			break;

		ChamberFrame[i]->Localize();
		ChamberFrame[i]->AutoArrange();
		ChamberFrame[i]->ShowLotInfo();
//		ChamberFrame[i]->FillUpInfo();
		ChamberFrame[i]->UpdateRecipeComboBox();
		GetChamber(i)->InitCompleted = true;
	}

	if( chamberCount == 4 )
	{
		if( ShowDetailMode )
		{
			ChamberFrame[CurrentChamber]->Visible = true;
		}
		else
		{
			for(int i=0; i<chamberCount; i++)
			{
				if( ChamberFrame[i] == nullptr )
					break;
				ChamberFrame[i]->Visible = true;
			}
		}
	}

	for(int i=0; i<chamberCount; i++)
	{
		if( ChamberFrame[i] == nullptr )
			break;
		if( ChamberFrame[i]->Visible &&
			ChamberFrame[i]->UserIDEdit->Visible )
			ChamberFrame[i]->UserIDEdit->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmJob::FormShow(TObject *Sender)
{
	ArrangeChamber();
	Localize();

	NormalOvenChamber *chamber;
	for(int i=0; i<GetManager()->ChamberCount; i++)
	{
		chamber = GetChamber(i);

		if (GetManager()->UseAutomation &&
			GetManager()->IsUseGem() &&
			GemDlg && GemDlg->IsGemConnected())
		{
	 //		NormalOvenChamber *chamber;
	 //		for(int i=0; i<GetManager()->ChamberCount; i++)
	 //		{
	 //			chamber = GetChamber(i);
				if( chamber->Step == STEP_IDLE )
					GemDlg->SetProcessState(i, STATE_IDLE);
				else
				if( chamber->Step == STEP_RUNNING ||
					chamber->Step == STEP_COOLING )
					GemDlg->SetProcessState(i, STATE_EXECUTING);
				else
				if( chamber->Step == STEP_DONE )
					GemDlg->SetProcessState(i, STATE_DONE);
	 //		}
		}
		else if (GetManager()->UseAutomation == true &&
			GetManager()->AutomationKind == SECS_GEM_SCK && EzNetDlg[i] != nullptr &&
			EzNetDlg[i]->IsGemConnected())
		{
			if( chamber->Step == STEP_IDLE )
				EzNetDlg[i]->SetProcessState(STATE_IDLE);
			else
			if( chamber->Step == STEP_RUNNING ||
				chamber->Step == STEP_COOLING )
				EzNetDlg[i]->SetProcessState(STATE_EXECUTING);
			else
			if( chamber->Step == STEP_DONE )
				EzNetDlg[i]->SetProcessState(STATE_DONE);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFmJob::CaptionPanelMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	ReleaseCapture();
	Perform(WM_SYSCOMMAND, 0xF012, 0 );
}
//---------------------------------------------------------------------------
void __fastcall TFmJob::Localize()
{
	String str, prgName;

	int level = GetManager()->LoginLevel;

	if( level == LV_OPERATOR )
		CaptionPanel->Caption->Color = clHighlight;
	else
	if( level == LV_ENGINEER )
		CaptionPanel->Caption->Color = clRed;
	else
	if( level == LV_DEVELOPER )
		CaptionPanel->Caption->Color = clFuchsia;

	prgName = GetManager()->ProgramName;
	if( prgName.Pos("_") )
	{
		TReplaceFlags Flags = TReplaceFlags() << rfReplaceAll;
		prgName = StringReplace(prgName, "_", "-", Flags);
	}
	prgName += " Ver ";
	prgName += GetInformation(Application->ExeName, "ProductVersion");
	str.sprintf(L"<P align=\"center\"><FONT color=\"#FFFFFF\">%s</FONT></P>", prgName.c_str());
	CaptionPanel->Text = str;
	MenuButton->Caption = GetResStr("Menu", "Menu");

	str = String(LevelStr[GetManager()->LoginLevel]);
	str.UpperCase();
	if( LoginModePanel->Text != str )
		LoginModePanel->Text = str;
}
//---------------------------------------------------------------------------
bool __fastcall TFmJob::IsChamberStarting()
{
	bool isStarting = false;
	for (int i = 0; i < GetManager()->ChamberCount; i++)
	{
		if (ChamberFrame[i]->ProgressBar->Visible)
		{
			isStarting = true;
		}
	}

	return isStarting;
}
//---------------------------------------------------------------------------
void __fastcall TFmJob::MenuButtonClick(TObject *Sender)
{
	if (IsChamberStarting())
	{
		return;
	}

	TStickyMenuItem *item;
	PopupMenu->MenuItems->Clear();
	int level = GetManager()->LoginLevel;

	if( level == LV_OPERATOR )
	{
		item = PopupMenu->MenuItems->Add();
		item->Caption = GetResStr("Menu", "Log");
		item->OnClick = OnLog;

		item = PopupMenu->MenuItems->Add();
		item->Caption = GetResStr("Menu", "Graph");
		item->OnClick = OnGraph;

		item = PopupMenu->MenuItems->Add();
		item->Caption = GetResStr("Menu", "Logout");
		item->OnClick = OnLogout;

		item = PopupMenu->MenuItems->Add();
		item->Caption = GetResStr("Menu", "Exit");
		item->OnClick = OnExit;
	}
	else
	if( level == LV_ENGINEER )
	{
		item = PopupMenu->MenuItems->Add();
		item->Caption = GetResStr("Menu", "Log");
		item->OnClick = OnLog;

		item = PopupMenu->MenuItems->Add();
		item->Caption = GetResStr("Menu", "Graph");
		item->OnClick = OnGraph;

		item = PopupMenu->MenuItems->Add();
		item->Caption = GetResStr("Menu", "User");
		item->OnClick = OnRegisterUser;

		item = PopupMenu->MenuItems->Add();
		item->Caption = GetResStr("Menu", "Setting");
		item->OnClick = OnSetting;

		item = PopupMenu->MenuItems->Add();
		item->Caption = GetResStr("Menu", "IO Test");
		item->OnClick = OnIOTest;

		item = PopupMenu->MenuItems->Add();
		item->Caption = GetResStr("Menu", "Logout");
		item->OnClick = OnLogout;

		item = PopupMenu->MenuItems->Add();
		item->Caption = GetResStr("Menu", "Exit");
		item->OnClick = OnExit;
	}
	else
	if( level == LV_DEVELOPER )
	{
		item = PopupMenu->MenuItems->Add();
		item->Caption = GetResStr("Menu", "Log");
		item->OnClick = OnLog;

		item = PopupMenu->MenuItems->Add();
		item->Caption = GetResStr("Menu", "Graph");
		item->OnClick = OnGraph;

		item = PopupMenu->MenuItems->Add();
		item->Caption = GetResStr("Menu", "User");
		item->OnClick = OnRegisterUser;

		item = PopupMenu->MenuItems->Add();
		item->Caption = GetResStr("Menu", "Setting");
		item->OnClick = OnSetting;

		item = PopupMenu->MenuItems->Add();
		item->Caption = GetResStr("Menu", "IO Test");
		item->OnClick = OnIOTest;

		item = PopupMenu->MenuItems->Add();
		item->Caption = GetResStr("Menu", "Logout");
		item->OnClick = OnLogout;

        item = PopupMenu->MenuItems->Add();
		item->Caption = GetResStr("Menu", "Exit");
		item->OnClick = OnExit;
	}

	int x = MenuButton->ClientOrigin.x;
	int y = MenuButton->ClientOrigin.y + MenuButton->Height;
	PopupMenu->ShowMenu(x, y);
}
//---------------------------------------------------------------------------
void __fastcall TFmJob::OnLogout(TObject *Sender)
{
	PopupMenu->HideMenu();

	GetManager()->LoginLevel = LV_NONE;

	Hide();
	FmMain->Show();

	String sMsg = "Logout (ID : " + GetManager()->UserID +")";
	GetOvenDB()->SaveLog(sMsg, MESSAGES);
}
//---------------------------------------------------------------------------
void __fastcall TFmJob::OnLog(TObject *Sender)
{
	PopupMenu->HideMenu();

	Hide();
	FmLog->Show();
}
//---------------------------------------------------------------------------
void __fastcall TFmJob::OnIOTest(TObject *Sender)
{
	int cnt = GetManager()->ChamberCount;

	PopupMenu->HideMenu();
	Hide();
	FmAdapter->Init(AD_IO);
	for(int i=0; i<cnt; i++)
	{
		FmAdapter->Frame[i] = new TFrmIO(nullptr, i);
	}

	FmAdapter->Caption = "IO TEST";
	FmAdapter->Show();
}
//---------------------------------------------------------------------------
void __fastcall TFmJob::OnExit(TObject *Sender)
{
	PopupMenu->HideMenu();
	FmMain->Close();
}
//---------------------------------------------------------------------------
void __fastcall TFmJob::OnSetting(TObject *Sender)
{
	int cnt = GetManager()->ChamberCount;

	PopupMenu->HideMenu();
	Hide();
	FmAdapter->Init(AD_SETTING);
	for(int i=0; i<cnt; i++)
	{
		FmAdapter->Frame[i] = new TFrmChamberSetting(nullptr, i);
	}

	TFrame *frame = new TFrmSystemSetting(nullptr);
	frame->Hint = "System Settings";
	FmAdapter->Frame[cnt] = frame;

	FmAdapter->Caption = "SETTINGS";
	FmAdapter->Show();
}
//---------------------------------------------------------------------------
void __fastcall TFmJob::OnRegisterUser(TObject *Sender)
{
	if( GetManager()->LoginLevel < LV_SUPERVISOR )
	{
		if( ShowLogin(LV_SUPERVISOR, false) == false )
			return;
	}

	PopupMenu->HideMenu();
	Hide();
	FmRegisterUser->Show();
}
//---------------------------------------------------------------------------
void __fastcall TFmJob::OnGraph(TObject *Sender)
{
	int cnt = GetManager()->ChamberCount;

	PopupMenu->HideMenu();
	Hide();
	FmAdapter->Init(AD_GRAPH);
	for(int i=0; i<cnt; i++)
	{
		FmAdapter->Frame[i] = new TFrmTempGraph(nullptr, i);
	}

	((TFrmTempGraph*)FmAdapter->Frame[0])->CalendarSelectDate(nullptr, dmDay, Date());

	FmAdapter->Caption = "GRAPH";
	FmAdapter->Show();
}
//---------------------------------------------------------------------------
void __fastcall TFmJob::FormClose(TObject *Sender, TCloseAction &Action)
{
	int chamberCount = GetManager()->ChamberCount;
	for(int i=0; i<chamberCount; i++)
	{
		if( ChamberFrame[i] )
			ChamberFrame[i].reset();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmJob::UpdateAutomationStatus()
{
	if( GetManager()->UseAutomation )
	{
		int autoKind = GetManager()->AutomationKind;
		if( GemDlg && GetManager()->IsUseGem() )
		{
			String sConnected, sEnabled, sCtrlState;
			if( GemDlg->GemState[0].bConnectedFg )
				sConnected = "CONNECTED";
			else
				sConnected = "NOT CONNECTED";

			if( GemDlg->GemState[0].bEnabled )
				sEnabled = "ENABLED";
			else
				sEnabled = "DISABLED";

			switch(GemDlg->GemState[0].nControlState)
			{
			case CONTROL_EQUIPMENT_OFFLINE:
				sCtrlState = "OFFLINE";
				break;
			case CONTROL_ATTEMPT_ONLINE:
				sCtrlState = "ATTEMPT ONLINE";
				break;
			case CONTROL_HOST_OFFLINE:
				sCtrlState = "HOST OFFLINE";
				break;
			case CONTROL_ONLINE_LOCAL:
				sCtrlState = "ONLINE LOCAL";
				break;
			case CONTROL_ONLINE_REMOTE:
				sCtrlState = "ONLINE REMOTE";
				break;
			default:
				break;
			}

			AutomationStatusBar->Panels->Items[2]->Text = GemDlg->GetHostIP();
			AutomationStatusBar->Panels->Items[2]->AppearanceStyle = (GemDlg->GemState[0].bConnectedFg) ? psLight : psDark;

			AutomationStatusBar->Panels->Items[3]->Text = sCtrlState;
			AutomationStatusBar->Panels->Items[3]->AppearanceStyle =
				(GemDlg->GemState[0].nControlState == CONTROL_EQUIPMENT_OFFLINE
				|| GemDlg->GemState[0].nControlState == CONTROL_HOST_OFFLINE) ?
				psDark : psLight;

			AutomationStatusBar->Panels->Items[4]->Text = sConnected;
			AutomationStatusBar->Panels->Items[4]->AppearanceStyle = (GemDlg->GemState[0].bConnectedFg) ? psLight : psDark;

			AutomationStatusBar->Panels->Items[5]->Text = sEnabled;
			AutomationStatusBar->Panels->Items[5]->AppearanceStyle = (GemDlg->GemState[0].bEnabled ) ? psLight : psDark;
		}
		else
		if( autoKind == BARUN_MES )
		{
			AutomationStatusBar->Panels->Items[2]->Text = "";
			AutomationStatusBar->Panels->Items[2]->AppearanceStyle = psLight;

			AutomationStatusBar->Panels->Items[3]->Text = "";
			AutomationStatusBar->Panels->Items[3]->AppearanceStyle = psLight;

			AutomationStatusBar->Panels->Items[4]->Text = "";
			AutomationStatusBar->Panels->Items[4]->AppearanceStyle = psLight;

			AutomationStatusBar->Panels->Items[5]->Text = "";
			AutomationStatusBar->Panels->Items[5]->AppearanceStyle = psLight;
		}
	}
	else
	{
		AutomationStatusBar->Panels->Items[2]->Text = "";
		AutomationStatusBar->Panels->Items[2]->AppearanceStyle = psLight;

		AutomationStatusBar->Panels->Items[3]->Text = "";
		AutomationStatusBar->Panels->Items[3]->AppearanceStyle = psLight;

		AutomationStatusBar->Panels->Items[4]->Text = "";
		AutomationStatusBar->Panels->Items[4]->AppearanceStyle = psLight;

		AutomationStatusBar->Panels->Items[5]->Text = "";
		AutomationStatusBar->Panels->Items[5]->AppearanceStyle = psLight;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFmJob::UpdateAutomationBarTimer(TObject *Sender)
{
	UpdateAutomationBar->Enabled = false;
	UpdateAutomationStatus();
	UpdateAutomationBar->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFmJob::AutomationStatusBarClick(TObject *Sender)
{
	if( GetManager()->UseAutomation )
	{
		if( ShowLogin(LV_SUPERVISOR, false) == false )
			return;

		int kind = GetManager()->AutomationKind;
		if (GetManager()->IsUseGem())
		{
			if( GemDlg )
				GemDlg->Show();
		}
		else
		if( kind == BARUN_MES )
		{
			if( BarunMESDlg )
				BarunMESDlg->ShowModal();
		}
		else
		if( kind == SECS_GEM_SCK )
		{

		}
	}
}
//---------------------------------------------------------------------------
// 20180209 - FTP (OFILM MES) - jhlim
void __fastcall TFmJob::OFilmMESDataSaveTimer(TObject *Sender)
{
	OFilmMESDataSave->Enabled = false;

	// OFilm 챔버별 온도값 저장
	String sFileName, str;
	int chamberCount = GetManager()->ChamberCount;
	String sTime = FormatDateTime(L"yyyy/mm/dd hh:nn:ss.zzz", Now());
	for(int i=0; i<chamberCount; i++)
	{
		if( ChamberFrame[i] != nullptr )
		{
			sFileName.sprintf(L"%s%02d.TXT", GetDataFolder().c_str(), i+1);
			ChamberFrame[i]->WriteTempData(sFileName, sTime);
		}
	}

	OFilmMESDataSave->Enabled = true;
}
//---------------------------------------------------------------------------
// 20180209 - FTP (OFILM MES) - jhlim
void __fastcall TFmJob::OFilmMESSendFileTimer(TObject *Sender)
{
	OFilmMESDataSave->Enabled = false;
	OFilmMESSendFile->Enabled = false;

	GetManager()->CheckFtpConnect = true;
	// OFilm FTP File Sending
//	GetManager()->CreateFTPConnection();

	// Data File Delete
//	GetManager()->DeleteFTPDataFile();	// 20180212 - FTP (OFILM MES) - jhlim

	OFilmMESSendFile->Enabled = true;
	OFilmMESDataSave->Enabled = true;
}
//---------------------------------------------------------------------------
// 20181023 - ScreenSaver (infineon) - jhlim
void __fastcall TFmJob::ScreenSaverTimerTimer(TObject *Sender)
{
	ScreenSaverTimer->Enabled = false;

	if(nScreenSaverCnt > GetManager()->ScreenSaverTime)
	{
		nScreenSaverCnt = 0;
		ScreenSaver->Show();
	}
	else
	{
		nScreenSaverCnt += 1;
		ScreenSaverTimer->Enabled = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmJob::FormKeyPress(TObject *Sender, System::WideChar &Key)
{
	// 20181023 - ScreenSaver (infineon) - jhlim
	nScreenSaverCnt = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFmJob::FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)

{
	// 20181023 - ScreenSaver (infineon) - jhlim
	nScreenSaverCnt = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFmJob::FormMouseActivate(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y, int HitTest, TMouseActivate &MouseActivate)
{
	// 20181023 - ScreenSaver (infineon) - jhlim
	nScreenSaverCnt = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFmJob::BtnTCardClick(TObject *Sender)
{
	TCardInputDlg->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TFmJob::OPIDChangeButtonClick(TObject *Sender)
{
	if (GetManager()->UseAutomation)
	{
		if (GemDlg && GetManager()->IsUseGem() && GemDlg->IsGemConnected() &&
			GetManager()->AutomationKind == SECS_GEM_COWELL)
		{
			GetManager()->OperatorID = OPIDEdit->Text;
			GemDlg->SendOPIDStatusEvent(GetManager()->OperatorID);
		}
	}
}
//---------------------------------------------------------------------------
