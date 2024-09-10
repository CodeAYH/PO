//---------------------------------------------------------------------------

#include <vcl.h>
#include <DateUtils.hpp>
#pragma hdrstop

#include "GEMForm.h"
#include "JobForm.h"
#include "TerminalMsgForm.h"
#include "HelperFunctions.h"
#include "AdvOfficeStatusBar.hpp"
#include "NormalOvenChamber.h"
#include "NormalOvenSystemManager.h"
#include "Database.h"
#include "MainTempController.h"
#include "ST340.h"
#include "I7018.h"
#include "DiffPressure.h"
#include "ZR5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvPageControl"
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "BaseGrid"
#pragma link "AdvGlassButton"
#pragma link "AdvEdit"
#pragma link "AdvGroupBox"
#pragma link "AdvOfficeButtons"
#pragma link "AdvUtil"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFmGem::TFmGem(TComponent* Owner)
	: TForm(Owner)
{
	csLock = boost::shared_ptr<TCriticalSection>(new TCriticalSection());

	m_sPath = GetDataFolder();
	HsmsPara = HsmsParaPtr (new CHSMSPARA());
	Ecid = EcidPtr (new CECID_VAR());

	m_nS9F13TimerDly = 0;

	for(int i=0; i<4; i++)
	{
		GemState[i].bConnectedFg = false;
		GemState[i].bEnabled = false;
		GemState[i].nPrevProcessState = 0;
		GemState[i].nProcessState = 0;

		PrevDoorStatus[i] = DOOR_UNKOWN;	// 2021-11-02
	}

	m_sFileName = m_sPath + "EzGem.Ini";
	if( !FileExists(m_sFileName) )
	{
		try
		{
			int hFile = FileCreate(m_sFileName);
			FileClose(hFile);
		}
		catch(...)
		{
			throw (Exception("EzGem file creation fail"));
		}
	}

	String str;
	rdgSelCm->Items->Clear();
	for(int i=0; i<GetManager()->ChamberCount; i++)
	{
		str.sprintf(L"Chamber %c", i + 'A');
		rdgSelCm->Items->Add(str);
	}

	for(int i=0; i<100; i++)
		AlarmCatetory[i] = 0;

	TerminalDlg.reset();
}
//---------------------------------------------------------------------------

__fastcall TFmGem::~TFmGem(void)
{
	SaveGemPara();
}
//---------------------------------------------------------------------------

void __fastcall TFmGem::FormCreate(TObject *Sender)
{
	AlignComps();

	for(int i=0; i<4; i++)
	{
		GemState[i].nControlState = CONTROL_EQUIPMENT_OFFLINE;
		GemState[i].nPrevControlState = CONTROL_EQUIPMENT_OFFLINE;
	}

	LoadGemPara();
	FillGemPara();
	UpdateECidPara();
	UdpateGemState();

	Init();
//	btnGemStartClick(Sender);
//	SendEventReport(CEID_PROCSTATE_CHANGE);

	pcGem->ActivePage = Setup;
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::Init()
{
	EzGem->DeviceID	= static_cast<short>(GemParaGrid->Cells[1][0].ToIntDef(0));
	EzGem->IP       = GetBstr(GemParaGrid->Cells[1][1]);
	EzGem->Port     = static_cast<short>(GemParaGrid->Cells[1][2].ToIntDef(5000));

	int nMode;
	if(GemParaGrid->GetRadioIdx(1, 10, nMode))
		EzGem->PassiveMode = (nMode == 0) ? true : false;

	String str;
	str = UpperCase(Application->Title);
	EzGem->SetModelName(GetBstr(str).c_bstr());
	str = GetInformation(Application->ExeName, "ProductVersion");
	EzGem->SetSoftwareRev(GetBstr(str).c_bstr());
	Ecid->nTimeFormat = 16;

	// Add Remote command
	EzGem->AddRemoteCommand(GetBstr("PP-SELECT").c_bstr());
	EzGem->AddRemoteCommand(GetBstr("START").c_bstr());
	EzGem->AddRemoteCommand(GetBstr("STOP").c_bstr());
	EzGem->AddRemoteCommand(GetBstr("REMOTE").c_bstr());
	EzGem->AddRemoteCommand(GetBstr("LOCAL").c_bstr());
	EzGem->AddRemoteCommand(GetBstr("LOTID_CHECK_RESULT").c_bstr());

	///////CEID////////////////
	AddCEIDs();
	//////SVID////////
	AddSVIDs();
	//////ALARM//////
	AddALIDs();
	/////////////////
	AddECIDs();

	AddRPTIDs();

	EzGem->EnableCommunication();
	//FORMAT.SML에 등록된 구조 적용
	String sFile;
	sFile.sprintf(L"%sFORMAT.SML", GetDataFolder().c_str());
	EzGem->SetFormatFile(GetBstr(sFile).c_bstr());
	EzGem->SetFormatCheck(true);
	//-----------------------------------------------
	sFile.sprintf(L"%sVS_GEM.LOG", GetLogFolder().c_str());
	EzGem->SetLogFile(GetBstr(sFile).c_bstr(), true );
	EzGem->SetFormatCodeALID(ITEM_UINT4B);	// U2=52, I2=32, U4=54, I4=34, U1=51, I1=31
	EzGem->SetFormatCodeSVID(ITEM_UINT4B);
	EzGem->SetFormatCodeDATAID(ITEM_UINT4B);
	EzGem->SetFormatCodeCEID(ITEM_UINT4B);
	EzGem->SetFormatCodeECID(ITEM_UINT4B);
	//   Ezgem->SetFormatCodeRPTID();
	EzGem->SetFormatCodeTRACEID(ITEM_UINT4B);

//	for(int i=0; i<GetManager()->ChamberCount; i++)
//		GemState[i].bEnabled = true;

	int nRet = EzGem->Start();
	if(nRet < 0)
	{
		SetMessageStr(GetErrorCode(nRet));
		ShowMessageDlg();
		return;
	}

	nRet = EzGem->GetRuntimeState();
	if(nRet == 0)
	{
		SetMessageStr("There is no SoftKey.");
		ShowMessageDlg();
	}

	GetOvenDB()->SaveLog("GEM is Online Requested.", MESSAGES);

	if( Ecid->nDefaultControlState == CONTROL_ONLINE_LOCAL )
	{
		EzGem->GoOnlineLocal();
		EnableAutoReply();
	}
	else
	if( Ecid->nDefaultControlState == CONTROL_ONLINE_REMOTE )
	{
		EzGem->GoOnlineRemote();
		DisableAutoReply();
	}
	else
	if( Ecid->nDefaultControlState == CONTROL_EQUIPMENT_OFFLINE )
	{
		EzGem->GoOffline();
		EnableAutoReply();
	}

	UpdateECidPara();
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::AlignComps()
{
	Width  = 600;
	Height = 400;
    Position = poOwnerFormCenter;

	this->BorderStyle = Forms::bsSingle;
	this->FormStyle = fsStayOnTop;
    TBorderIcons tempBI = BorderIcons;
    tempBI >> biMaximize >> biMinimize >> biHelp;
    BorderIcons = tempBI;
	Font->Name = "Tahoma";
	Caption = "GEM";

	EzGem->Visible = false;

	pcGem->Pages[1]->Visible = false;

	GemParaGrid->Options >> goRangeSelect;
	GemParaGrid->Options << goEditing;
	GemParaGrid->ShowSelection = false;
	GemParaGrid->ColWidths[0] = 120;
	GemParaGrid->ColWidths[1] = 160;
	GemParaGrid->RowHeights[GemParaGrid->RowCount-1] = 35;

	std::auto_ptr<TStringList> sMode (new TStringList());
	sMode->Add("Passive");
	sMode->Add("Active");
	GemParaGrid->CreateRadio(1, 11, 1, -1)->AddStrings(sMode.get());

	GemParaGrid->MergeCells(0, 12, 2, 1);
	GemParaGrid->AddButton(0, 12, 278, 32, "Save && Apply", Advgrid::haCenter, Advgrid::vaCenter);

	StatusGrid->ColWidths[0] = 122;
	StatusGrid->ColWidths[1] = 138;
	StatusGrid->Options >> goRangeSelect;
	StatusGrid->ShowSelection = false;
	StatusGrid->MergeCells(0, 0, 1, 3);
	StatusGrid->ShowSelection = false;
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::LoadGemPara()
{
	const std::auto_ptr<TIniFile> GemFile(new TIniFile(m_sFileName));
	const String sSection = "GEM PARAMETER";

	if( GemFile->SectionExists(sSection) == false )
	{
		HsmsPara->nDeviceId	= 0;
		HsmsPara->nPort		= 5000;
		HsmsPara->nLnkTest	= 10;
		HsmsPara->nRetry	= 3;
		HsmsPara->nT3		= 5;
		HsmsPara->nT5		= 5;
		HsmsPara->nT7		= 5;
		HsmsPara->nT8		= 5;
		HsmsPara->nCTOut	= 5;
		HsmsPara->bPassiveMode = true;
		HsmsPara->sHostIP 	= "127.0.0.1";

		Ecid->nDefaultControlState = CONTROL_EQUIPMENT_OFFLINE;
	}
	else
	{
		try
		{
			HsmsPara->nDeviceId	= GemFile->ReadInteger(sSection, "DEVICE ID", 0);
			HsmsPara->nPort		= GemFile->ReadInteger(sSection, "PORT NO", 5000);
			HsmsPara->nLnkTest	= GemFile->ReadInteger(sSection, "LINK TEST", 10);
			HsmsPara->nRetry	= GemFile->ReadInteger(sSection, "RETRY", 3);
			HsmsPara->nT3		= GemFile->ReadInteger(sSection, "T3", 5);
			HsmsPara->nT5		= GemFile->ReadInteger(sSection, "T5", 5);
			HsmsPara->nT6		= GemFile->ReadInteger(sSection, "T6", 5);
			HsmsPara->nT7		= GemFile->ReadInteger(sSection, "T7", 5);
			HsmsPara->nT8		= GemFile->ReadInteger(sSection, "T8", 5);
			HsmsPara->nCTOut	= GemFile->ReadInteger(sSection, "CONVERSATION TIMEOUT", 5);
			HsmsPara->bPassiveMode = GemFile->ReadBool(sSection, "PASSIVE", false);
			HsmsPara->sHostIP 	= GemFile->ReadString(sSection, "HOST IP", "127.0.0.1");

			String sDefaultCtrlState = GemFile->ReadString(sSection, "DefaultControlState", "Offline");
			if( sDefaultCtrlState == "Offline" )
				Ecid->nDefaultControlState = CONTROL_EQUIPMENT_OFFLINE;
			else
			if( sDefaultCtrlState == "Local" )
				Ecid->nDefaultControlState = CONTROL_ONLINE_LOCAL;
			else
			if( sDefaultCtrlState == "Remote" )
				Ecid->nDefaultControlState = CONTROL_ONLINE_REMOTE;
		}
		catch (Exception* e)
		{
			SetMessageStr(e->Message);
			ShowMessageDlg();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::FillGemPara()
{
	GemParaGrid->Cells[1][0] = IntToStr(HsmsPara->nDeviceId);
	GemParaGrid->Cells[1][1] = HsmsPara->sHostIP;
	GemParaGrid->Cells[1][2] = IntToStr(HsmsPara->nPort);
	GemParaGrid->Cells[1][3] = IntToStr(HsmsPara->nLnkTest);
	GemParaGrid->Cells[1][4] = IntToStr(HsmsPara->nRetry);
	GemParaGrid->Cells[1][5] = IntToStr(HsmsPara->nT3);
	GemParaGrid->Cells[1][6] = IntToStr(HsmsPara->nT5);
	GemParaGrid->Cells[1][7] = IntToStr(HsmsPara->nT6);
	GemParaGrid->Cells[1][8] = IntToStr(HsmsPara->nT7);
	GemParaGrid->Cells[1][9] = IntToStr(HsmsPara->nT8);
	GemParaGrid->Cells[1][10] = IntToStr(HsmsPara->nCTOut);
	if(HsmsPara->bPassiveMode)
		GemParaGrid->SetRadioIdx(1, 11, 0);
	else
		GemParaGrid->SetRadioIdx(1, 11, 1);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::SaveGemPara()
{
	const std::auto_ptr<TIniFile> GemFile(new TIniFile(m_sFileName));
	const String sSection = "GEM PARAMETER";

	if(IsValidPara())
	{
		HsmsPara->nDeviceId = GemParaGrid->Cells[1][0].ToIntDef(0);
		HsmsPara->sHostIP  	= GemParaGrid->Cells[1][1];
		HsmsPara->nPort	   	= GemParaGrid->Cells[1][2].ToIntDef(5000);
		HsmsPara->nLnkTest	= GemParaGrid->Cells[1][3].ToIntDef(10);
		HsmsPara->nRetry	= GemParaGrid->Cells[1][4].ToIntDef(3);
		HsmsPara->nT3		= GemParaGrid->Cells[1][5].ToIntDef(5);
		HsmsPara->nT5		= GemParaGrid->Cells[1][6].ToIntDef(5);
		HsmsPara->nT6		= GemParaGrid->Cells[1][7].ToIntDef(5);
		HsmsPara->nT7		= GemParaGrid->Cells[1][8].ToIntDef(5);
		HsmsPara->nT8		= GemParaGrid->Cells[1][9].ToIntDef(5);
		HsmsPara->nCTOut	= GemParaGrid->Cells[1][10].ToIntDef(5);
		int nMode;
		if(GemParaGrid->GetRadioIdx(1, 11, nMode))
			HsmsPara->bPassiveMode = (nMode == 0) ? true : false;
		try
		{
			GemFile->WriteInteger(sSection, "DEVICE ID", HsmsPara->nDeviceId);
			GemFile->WriteInteger(sSection, "PORT NO", HsmsPara->nPort);
			GemFile->WriteInteger(sSection, "LINK TEST", HsmsPara->nLnkTest);
			GemFile->WriteInteger(sSection, "RETRY", HsmsPara->nRetry);
			GemFile->WriteInteger(sSection, "T3", HsmsPara->nT3);
			GemFile->WriteInteger(sSection, "T5", HsmsPara->nT5);
			GemFile->WriteInteger(sSection, "T6", HsmsPara->nT6);
			GemFile->WriteInteger(sSection, "T7", HsmsPara->nT7);
			GemFile->WriteInteger(sSection, "T8", HsmsPara->nT8);
			GemFile->WriteInteger(sSection, "CONVERSATION TIMEOUT", HsmsPara->nCTOut);
			GemFile->WriteString(sSection, "HOST IP", HsmsPara->sHostIP);
			GemFile->WriteBool(sSection, "PASSIVE", HsmsPara->bPassiveMode);

			if (GemState[0].nControlState == CONTROL_ONLINE_LOCAL)
				GemFile->WriteString(sSection, "DefaultControlState", "Local");
			else
			if (GemState[0].nControlState == CONTROL_ONLINE_REMOTE)
				GemFile->WriteString(sSection, "DefaultControlState", "Remote");
			else
				GemFile->WriteString(sSection, "DefaultControlState", "Offline");
		}
		catch (Exception* e)
		{
			SetMessageStr(e->Message);
			ShowMessageDlg();
		}
	}
}
//---------------------------------------------------------------------------
bool __fastcall TFmGem::IsValidPara()
{
	for(int i=0; i<GemParaGrid->RowCount-2; i++)
	{
		if( GemParaGrid->Cells[1][i].Trim() == "" )
			return false;
	}

	if( !IsValidIPAddr(GemParaGrid->Cells[1][1]) )
		return false;

	return true;
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::GemParaGridGetEditorType(TObject *Sender, int ACol, int ARow,
		  TEditorType &AEditor)
{
	switch (ARow)
	{
	case 1:
		AEditor = edNormal;
		break;
	case 11:
		AEditor = edButton;
		break;
	default:
		AEditor = edPositiveNumeric;
		break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::GemParaGridGetAlignment(TObject *Sender, int ARow, int ACol,
		  TAlignment &HAlign, Advobj::TVAlignment &VAlign)
{
	HAlign = static_cast<TAlignment>(haCenter);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::GemParaGridGetEditMask(TObject *Sender, int ACol, int ARow,
          UnicodeString &Value)
{
	if(ARow == 1)
	{
		Value = "000.000.000.000";
    }
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::GemParaGridEditingDone(TObject *Sender)
{
	String sIp = GemParaGrid->Cells[1][1];
	GemParaGrid->Cells[1][1] = StringReplace(sIp, " ", "", TReplaceFlags() << rfReplaceAll);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::GemParaGridButtonClick(TObject *Sender, int ACol, int ARow)
{
	Hide();

	SetMessageStr(GetResStr("Question", "QuestSave"));
	if( !ShowQuestionDlg() )
	{
		Show();
		return;
	}

	SaveGemPara();
	Show();
}
//---------------------------------------------------------------------------

void __fastcall TFmGem::StatusGridCanClickCell(TObject *Sender, int ARow, int ACol,
          bool &Allow)
{
	Allow = false;
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::StatusGridGetAlignment(TObject *Sender, int ARow, int ACol,
          TAlignment &HAlign, Advobj::TVAlignment &VAlign)
{
	HAlign = static_cast<TAlignment>(haCenter);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::UdpateGemState()
{
	String sConnected, sEnabled;
	if( GemState[0].bConnectedFg )
	{
		ConnectedImage->Visible = true;
		NotConnectedImage->Visible = false;
		sConnected = "CONNECTED";
	}
	else
	{
		ConnectedImage->Visible = false;
		NotConnectedImage->Visible = true;
		sConnected = "NOT CONNECTED";
	}

	if( GemState[0].bEnabled )
		sEnabled = "ENABLED";
	else
		sEnabled = "DISABLED";

	StatusGrid->Cells[1][0] = sConnected;
	StatusGrid->Cells[1][1] = sEnabled;

	String sCtrlState = "";

	switch (GemState[0].nControlState)
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
	StatusGrid->Cells[1][2] = sCtrlState;

	btnGemStart->Enabled = !GemState[0].bConnectedFg;
	btnGemStop->Enabled	 = GemState[0].bConnectedFg;
	btnGemOffline->Enabled = GemState[0].bConnectedFg && (GemState[0].nControlState != CONTROL_EQUIPMENT_OFFLINE);
	btnGemLocal->Enabled   = GemState[0].bConnectedFg && (GemState[0].nControlState != CONTROL_ONLINE_LOCAL);
	btnGemRemote->Enabled  = GemState[0].bConnectedFg && (GemState[0].nControlState != CONTROL_ONLINE_REMOTE);
}
//---------------------------------------------------------------------------

// *****************************************************************************
// EZGEM MESSAGE HANDLER START *************************************************
// *****************************************************************************
void __fastcall TFmGem::EzGemConnected(TObject *Sender)
{
	for(int i=0; i<GetManager()->ChamberCount; i++)
	{
		GemState[i].bConnectedFg = true;
		GemState[i].bEnabled = true;
	}
	UdpateGemState();

	String str;
	str = "GEM is Connected.";
	GetOvenDB()->SaveLog(str, MESSAGES);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::EzGemDisconnected(TObject *Sender)
{
	for(int i=0; i<GetManager()->ChamberCount; i++)
	{
		GemState[i].bConnectedFg = false;
		GemState[i].bEstablish = false;
		GemState[i].bEnabled = false;
	}
	GemState[0].nControlState = CONTROL_EQUIPMENT_OFFLINE;
	UdpateGemState();

	String str;
	str = "GEM is Disconnected.";
	GetOvenDB()->SaveLog(str, MESSAGES);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::EzGemEstablishCommRequest(TObject *Sender, long lMsgId)
{
	String str;
	str.sprintf(L"GEM EstablishCommRequest(Message ID: %ld).", lMsgId);
	GetOvenDB()->SaveLog(str, MESSAGES);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::EzGemCommEstablished(TObject *Sender)
{
	for(int i=0; i<GetManager()->ChamberCount; i++)
		GemState[i].bEstablish = true;
	UdpateGemState();

	String str;
	str = "GEM is CommEstablished(Communicating)";
	GetOvenDB()->SaveLog(str, MESSAGES);
}
//---------------------------------------------------------------------------
// S1F15 Handler
void __fastcall TFmGem::EzGemOfflineRequest(TObject *Sender, long lMsgId)
{
	String str;
	str.sprintf(L"GEM OfflineRequest(Message ID: %ld).", lMsgId);
	GetOvenDB()->SaveLog(str, MESSAGES);

	for(int i=0; i<GetManager()->ChamberCount; i++)
	{
		GemState[i].nPrevControlState = GemState[i].nControlState;
		GemState[i].nControlState = CONTROL_HOST_OFFLINE;

//		EzGem->SetCurrentStatusValue(SVID_CONTROLSTATE, GetBstr(IntToStr(GemState[i].nControlState)).c_bstr());
//		EzGem->SetCurrentStatusValue(SVID_PREV_CONTROLSTATE, GetBstr(IntToStr(GemState[i].nPrevControlState)).c_bstr());
		SetSVID(SVID_CONTROLSTATE, IntToStr(GemState[i].nControlState));
		SetSVID(SVID_PREV_CONTROLSTATE, IntToStr(GemState[i].nPrevControlState));

		SendEventReport(CEID_OFFLINE);
	}
//	EzGem->GoOffline();

	EnableAutoReply();
	UdpateGemState();

	str.sprintf(L"CONNECTED\nOFFLINE\nIP:%s\nPort:%d", EzGem->IP.c_bstr(), EzGem->Port);
	GetOvenDB()->SaveLog(str, MESSAGES);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::EzGemOnlineLocal(TObject *Sender)
{
	for(int i=0; i<GetManager()->ChamberCount; i++)
	{
		GemState[i].nPrevControlState = GemState[i].nControlState;
		GemState[i].nControlState = CONTROL_ONLINE_LOCAL;

//		EzGem->SetCurrentStatusValue(SVID_CONTROLSTATE, GetBstr(IntToStr(GemState[i].nControlState)).c_bstr());
//		EzGem->SetCurrentStatusValue(SVID_PREV_CONTROLSTATE, GetBstr(IntToStr(GemState[i].nPrevControlState)).c_bstr());
		SetSVID(SVID_CONTROLSTATE, IntToStr(GemState[i].nControlState));
		SetSVID(SVID_PREV_CONTROLSTATE, IntToStr(GemState[i].nPrevControlState));

		SendEventReport(CEID_LOCAL);
	}
//	EzGem->GoOnlineLocal();

	UdpateGemState();
	EnableAutoReply();

	String str;
	str = "GEM is LOCAL.";
	GetOvenDB()->SaveLog(str, MESSAGES);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::EzGemOnlineRemote(TObject *Sender)
{
	for(int i=0; i<GetManager()->ChamberCount; i++)
	{
		GemState[i].nPrevControlState = GemState[i].nControlState;
		GemState[i].nControlState = CONTROL_ONLINE_REMOTE;

//		EzGem->SetCurrentStatusValue(SVID_CONTROLSTATE, GetBstr(IntToStr(GemState[i].nControlState)).c_bstr());
//		EzGem->SetCurrentStatusValue(SVID_PREV_CONTROLSTATE, GetBstr(IntToStr(GemState[i].nPrevControlState)).c_bstr());
		SetSVID(SVID_CONTROLSTATE, IntToStr(GemState[i].nControlState));
		SetSVID(SVID_PREV_CONTROLSTATE, IntToStr(GemState[i].nPrevControlState));

		SendEventReport(CEID_REMOTE);
	}
//	EzGem->GoOnlineRemote();

	UdpateGemState();
	DisableAutoReply();

	String str;
	str = "GEM is REMOTE.";
	GetOvenDB()->SaveLog(str, MESSAGES);
}
//---------------------------------------------------------------------------
// S1F17 Handler
void __fastcall TFmGem::EzGemOnlineRequest(TObject *Sender, long lMsgId)
{
	String str;
	str.sprintf(L"GEM OnlineRequest(Message ID: %ld).", lMsgId);
	GetOvenDB()->SaveLog(str, MESSAGES);

	if( GemState[0].nControlState == CONTROL_HOST_OFFLINE ||
		GemState[0].nControlState == CONTROL_EQUIPMENT_OFFLINE)
	{
		EzGem->AcceptOnlineRequest(lMsgId);
		if( GemState[0].nPrevControlState == CONTROL_ONLINE_REMOTE )
			EzGem->GoOnlineRemote();
		else
		{
			if( GemState[0].nPrevControlState == CONTROL_ONLINE_LOCAL )
				EzGem->GoOnlineLocal();
        }
	}
	else
	{
		EzGem->DenyOnlineRequest(lMsgId);
    }
}
//---------------------------------------------------------------------------
/**********************************
   Recipe삭제 요청
**********************************/
void __fastcall TFmGem::EzGemNewEquipConst(TObject *Sender, long lMsgId, short nCount)
// S2F15
{
	String str;
	str.sprintf(L"GEM requested NewEquipConst(Message ID: %ld, Count:%d)", lMsgId, nCount);
	GetOvenDB()->SaveLog(str, MESSAGES);

	for(short i=0; i< nCount; i++)
	{
		long ECID = EzGem->GetNewEquipConst(lMsgId, i);
		WideString sValue = EzGem->GetNewEquipConstValue(lMsgId, i);

		switch (ECID)
		{
		case ECID_PORT :
			HsmsPara->nPort = StrToInt(sValue);
			break;
		case ECID_DEVICEID:
			HsmsPara->nDeviceId = StrToInt(sValue);
			break;
		case ECID_T3:
			HsmsPara->nT3 = StrToInt(sValue);
			break;
		case ECID_T5:
			HsmsPara->nT5 = StrToInt(sValue);
			break;
		case ECID_T6:
			HsmsPara->nT6 = StrToInt(sValue);
			break;
		case ECID_T7:
			HsmsPara->nT7 = StrToInt(sValue);
			break;
		case ECID_T8:
			HsmsPara->nT8 = StrToInt(sValue);
			break;
		case ECID_LINKTEST:
			HsmsPara->nLnkTest = StrToInt(sValue);
			break;
		case ECID_RETRY:
			HsmsPara->nRetry = StrToInt(sValue);
			break;
		case ECID_DEFAULT_COMM_STATE:
			Ecid->nDefaultCommState = StrToInt(sValue);
			break;
		case ECID_DEFAULT_CONTROL_STATE:
			Ecid->nDefaultControlState = StrToInt(sValue);
			break;
		case ECID_ESTABLISH_TIMEOUT:
			Ecid->nCommReqeustTimeout = StrToInt(sValue);
			break;
		case ECID_MDLN:
			Ecid->sModelName = sValue;
			break;
		case ECID_SOFTREV:
			Ecid->sSoftRev = sValue;
			break;
		case ECID_MAXSPOOL_TRANSMIT:
			Ecid->SPOOLINFO.nMaxSpoolTransmit = StrToInt(sValue);
			break;
		case ECID_SPOOL_OVERWRITE:
			Ecid->SPOOLINFO.bSpoolOverwrite   = (StrToInt(sValue)==1);
			break;
		case ECID_SPOOLING_MODE:
			Ecid->SPOOLINFO.bSpoolEnabled     = (StrToInt(sValue)==1);
			break;
		case ECID_MAX_SPOOL_SIZE:
			Ecid->SPOOLINFO.nMaxSpoolSize     = StrToInt(sValue);
			break;
		case ECID_TIME_FORMAT:
			if( sValue == "0" )
				Ecid->nTimeFormat = 14;
			else
			if( sValue == "1" )
				Ecid->nTimeFormat = 16;
			break;
			//case ECID_JOBINFO_OPCONFIRM:	m_bJobInfoConfirm = (StrToInt(strValue)==1);		break;
		default:
			break;
		}
	}
	EzGem->AcceptNewEquipConst(lMsgId);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::EzGemMsgRequested(TObject *Sender, long lMsgId)
{
	short nStream, nFunction, nWbit;
	long lLength;

	EzGem->GetMsgInfo(lMsgId, &nStream, &nFunction, &nWbit, &lLength);

	String str;
	str.sprintf(L"GEM is MsgRequested(Message ID: %ld, S%dF%dW%d, Length:%ld)", lMsgId, nStream, nFunction, nWbit, lLength);
	GetOvenDB()->SaveLog(str, MESSAGES);

//	if( nStream == 6 &&
//		nFunction == 15 &&
//		nWbit )
//	{
//		OnS6F15W(lMsgId);
//	}
//	else
	if( nStream == 7 &&
		nFunction == 1 &&
		nWbit )
	{
		//Process Program Load Inquire(PPI)
		//호스트가 Recipe를 내려주기 전 미리 해당 Recipe명을 알려 줌
		//해당 Recipe가 존재 하는지 판단하여 응답 하여야 함
		//이미 존재 한다면 Ack 를 0보다 큰값으로 보냄
		OnS7F1W(lMsgId);
	}
	else
	if( nStream == 7 &&
		nFunction == 3 &&
		nWbit )
	{
		//Process Program Send(PPS)
		//호스트가 Recipe를 내려줌 해당 Recipe는 RECIPE폴더
		//내부에 저장 됨
		OnS7F3W(lMsgId);
	}
	else
	if( nStream == 7 &&
		nFunction == 5 &&
		nWbit )
	{
		//Process Program Request(PPR)
		//호스트가 Recipe를 요청함 장비는 해당 Recipe의
		//존재 여부를 판단하여 응답 하여야 함.
		OnS7F5W(lMsgId);
	}
	else
	if( nStream == 7 &&
		nFunction == 17 &&
		nWbit )
	{
		//S7F17 : Delete Process Program Send(DPS)
		//Recipe삭제 요청
		OnS7F17W(lMsgId);
	}
	else
	if( nStream == 7 &&
		nFunction == 19 &&
		nWbit )
	{
		//S7F19 : Current EPPD Request
		//Host가 레시피 리스트 조회/리스트 송부 요청
		OnS7F19W(lMsgId);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::EzGemRemoteCommand(TObject *Sender, long lMsgId, OLECHAR *strCommand,
		  short nParamCount)
{	// S2F41
	short nAck = 0;
	int i = 0, chamberNo = -1, result = 0;
	String sMode, sRCMD, sName, sValue, sLOTID="", sPPID="", sCSTID="", sDetail="";
	AnsiString sLog;
	bool isValidSuccess = false;

	// nAck
	//0 = Acknowledge, command has been performed
	//1 = Command does not exists
	//2 = Cannot perform now
	//3 = At least one parameter is invalid
	//4 = Acknowledge, command will be performed with completion signalled later by an event
	//5 = Rejected, Already in the desired condition
	//6 = No such object exists

	//DenyRemotCommand
	//-1 = 2
	//-2 = 3
	//-3 = 5
	//-4 = 6
	//Other = 2
	//AcceptRemoteCommand
	//0 = 0
	//1 = 4
	//Other = 0

	sRCMD = strCommand;//.sprintf("%s", (String)strCommand);

	if( sRCMD == "REMOTE" )
	{
		btnGemRemoteClick(Sender);
		//수행이 가능한 경우 해당 명령을 실행 시키고 다음과 같이 응답
		EzGem->AcceptRemoteCommand(lMsgId, strCommand, nAck);
		return;
	}
	else
	if( sRCMD == "LOCAL" )
	{
		btnGemLocalClick(Sender);
		//수행이 가능한 경우 해당 명령을 실행 시키고 다음과 같이 응답
		EzGem->AcceptRemoteCommand(lMsgId, strCommand, nAck);
		return;
	}

	int maxChamberNo = GetManager()->ChamberCount - 1;

	for(i=0; i<nParamCount; i++)
	{
		sName 	= EzGem->GetRemoteCommandParamName(lMsgId, GetBstr(sRCMD).c_bstr(), i);
		sValue 	= EzGem->GetRemoteCommandParamValue(lMsgId, GetBstr(sRCMD).c_bstr(), i);

		if( sName == "LOTID" )
			sLOTID = sValue;
		else
		if( sName == "PPID" )
			sPPID = sValue;
		else
		if( sName == "PORTID" )
		{
			AnsiString tmp = sValue;
			if( IsValidInt(tmp) )
			{
				chamberNo = sValue.ToInt();  // Chamber Number!!!! A, B, C, D
			}
		}
		else
		if (sName == "RESULT") //COWELL Add HOST COMMAND LIST
		{
			result = sValue.ToIntDef(0);
			isValidSuccess = (result == 1) ? true : false;
		}
		else
		if (sName == "DETAIL") //COWELL Host Command Error Detail
		{
			sDetail = sValue;
        }
	}

	String str;
	str.sprintf(L"RCMD:%s Name:%s Value:%s Message ID: :%d Parameter Count:%d", sRCMD.c_str(), sName.c_str(), sValue.c_str(), lMsgId, nParamCount);
	GetOvenDB()->SaveLog(str, MESSAGES, chamberNo);

	if( InRange(chamberNo, 0, maxChamberNo) == false )
	{
		nAck = -2;//3;
		EzGem->DenyRemoteCommand(lMsgId, strCommand, nAck);
		return;
	}
	else
	if( GemState[chamberNo].nControlState != CONTROL_ONLINE_REMOTE )
	{
		nAck = -1;//2;
		//수행이 불가능 한 경우 명령 수행을 하지 않고 다음과 같이 응답
		EzGem->DenyRemoteCommand(lMsgId, strCommand, nAck);

		if( GemState->nControlState == CONTROL_ONLINE_LOCAL )
		{
			sMode = IntToStr(GemState->nControlState);
//			EzGem->SetCurrentStatusValue((long)SVID_CONTROLSTATE, GetBstr(sMode).c_bstr() );
			SetSVID(SVID_CONTROLSTATE, sMode);
			SendEventReport(CEID_LOCAL);
		}
		else
		if( GemState->nControlState == CONTROL_EQUIPMENT_OFFLINE )
		{
			sMode = IntToStr(GemState->nControlState);
//			EzGem->SetCurrentStatusValue((long)SVID_CONTROLSTATE, GetBstr(sMode).c_bstr() );
			SetSVID(SVID_CONTROLSTATE, sMode);
			SendEventReport(CEID_OFFLINE);
		}
		//수행이 불가능 한 경우 명령 수행을 하지 않고 다음과 같이 응답
		//EzGem->DenyRemoteCommand(lMsgId, strCommand,nAck);
		return;
	}

	/**********************************
	해당 명령에 대하여 장비 상태를 확인 후 적용 가능 여부를 판단하여 명령을 수행
	**********************************/
	if( sRCMD == "PP-SELECT" )
	{
		if (GetChamber(chamberNo)->Step == STEP_LOCAL &&
			GetManager()->UseCureStartEnableFromLocalState == true)
		{
			if( sPPID.IsEmpty() == false )
			{
				nAck = RemoteCommandPpselect(chamberNo, sPPID);
			}
			else
			{
				nAck = -2;//3 = At least one parameter is invalid
			}
		}
		else if( GetChamber(chamberNo)->Step != STEP_IDLE )
		{
			nAck = -1;//2;
		}
		else
		{
			if( sPPID.IsEmpty() == false )
			{
				nAck = RemoteCommandPpselect(chamberNo, sPPID);
			}
			else
			{
				nAck = -2;//3 = At least one parameter is invalid
			}
		}
	}
	else
	if( sRCMD == "START" )
	{
		if( InRange(chamberNo, 0, maxChamberNo) == false )
			nAck = -2;//3;
		else
		if( FmJob->ChamberFrame[chamberNo]->CureStart(true) == false )
		{
			nAck = -1;//2;
//			ShowMessageDlg();
		}
		else
		{
			SetProcessState(chamberNo, STATE_EXECUTING);
		}
	}
	else
	if( sRCMD == "STOP" )
	{
		if( InRange(chamberNo, 0, maxChamberNo) == false )
			nAck = -2;//3;
		else
		if( GetChamber(chamberNo)->Step == STEP_DONE )
			nAck = -3;//5;
		else
		{
			if( FmJob->ChamberFrame[chamberNo]->CureStop(true) == false )
				nAck = -1;//2 = Cannot perform now
		}
	}
	else
	if ( sRCMD == "LOTID_CHECK_RESULT" )
	{
		if (GetChamber(chamberNo)->ValidationRequested)
		{
			if(	GetChamber(chamberNo)->Step != STEP_IDLE )
			{
				nAck = -1;//2;
			}
			else
			{
				if (isValidSuccess)
				{
					if( sPPID.IsEmpty() == false )
					{
						if( GetOvenDB()->IsRecipeExist(chamberNo, sPPID) )
						{
							SetProcessState(chamberNo, STATE_READY);
							nAck = 0;
						}
						else
						{
							nAck = -2;//3;
						}
					}
					else
					{
						nAck = -2;//3 = At least one parameter is invalid
					}
				}
			}

			GetChamber(chamberNo)->ValidationRequested = false;
			GetChamber(chamberNo)->ValidationSuccess = isValidSuccess;
			GetChamber(chamberNo)->ErrorDetail = sDetail;
		}
	}
	else
		nAck = 1;

	if( nAck == 0 )
	{
		//수행이 가능한 경우 해당 명령을 실행 시키고 다음과 같이 응답
		EzGem->AcceptRemoteCommand(lMsgId, strCommand, nAck);

		if( sRCMD == "PP-SELECT" )
		{
			String str;
			str.sprintf(L"The recipe [%s] is selected.", sPPID.c_str());
			GetOvenDB()->SaveLog(str, MESSAGES, chamberNo);
		}
	}
	else
	{
		//수행이 불가능 한 경우 명령 수행을 하지 않고 다음과 같이 응답
		EzGem->DenyRemoteCommand(lMsgId, strCommand, nAck);

		// 응답 보내고 메시지 띄우도록 이동
        if( sRCMD == "START" )
		{
			ShowMessageDlg();
		}

		String str;
		str.sprintf(L"The remote command [%s] is denied.", strCommand);
		GetOvenDB()->SaveLog(str, MESSAGES, chamberNo);
	}
}
//---------------------------------------------------------------------------
short __fastcall TFmGem::RemoteCommandPpselect(int chamberNo, String strPPID)
{
	short nAck = 0;

	if (IsGemRecipeExist(strPPID))
	{
		if (FmJob->ChamberFrame[chamberNo]->SelectRemoteRecipe(strPPID) == false)
		{
			nAck = -2;//3;
		}
		else
		{
			SetProcessState(chamberNo, STATE_READY);
		}
	}
	else
	if (GetOvenDB()->IsRecipeExist(chamberNo, strPPID))
	{
		if (FmJob->ChamberFrame[chamberNo]->SelectRecipe(strPPID) == false)
		{
			nAck = -2;//3;
        }
		else
		{
			SetProcessState(chamberNo, STATE_READY);
		}
	}
	else
	{
	   nAck = -2;//3;
	}

	return nAck;
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::EzGemTerminalMessageMulti(TObject *Sender, long lMsgId, short nCode,
		  short nCount)
{
	if( nCount < 1 )
		return;

	boost::shared_ptr<TStringList> slTerminalMsg(new TStringList() );
	slTerminalMsg->Clear();
	for(int i=0; i<nCount; i++)
	{
		String sMsg = EzGem->GetTerminalMessageMulti(lMsgId, i);
		slTerminalMsg->Add(sMsg);

		String str;
		str.sprintf(L"GEM received TerminalMessageMulti:%s (Message ID: %d Code: %d Count: %d).", sMsg.c_str(), lMsgId, nCode, nCount);
		GetOvenDB()->SaveLog(str, MESSAGES);
	}

	EzGem->AcceptTerminalMessage(lMsgId);
	DisplayTerminalMsg(slTerminalMsg);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::EzGemTerminalMessageSingle(TObject *Sender, long lMsgId, OLECHAR *strCommand,
		  short nParamCount)
{
	EzGem->AcceptTerminalMessage(lMsgId);

	String sMsg = strCommand;

	boost::shared_ptr<TStringList> slTerminalMsg(new TStringList() );
	slTerminalMsg->Clear();
	slTerminalMsg->Add(sMsg);

	String str;
	str.sprintf(L"GEM received TerminalMessageSingle(Message ID: %d Command: %s Parameter Count: %d).", lMsgId, sMsg.c_str(), nParamCount);
	GetOvenDB()->SaveLog(str, MESSAGES);

	DisplayTerminalMsg(slTerminalMsg);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::DisplayTerminalMsg(const boost::shared_ptr<TStringList> slMsg)
{
	TerminalDlg.reset(new TFmTerminalMsg(slMsg, nullptr));
	TerminalDlg->Show();
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::EzGemDateTimeSetRequest(TObject *Sender, long lMsgId, OLECHAR *strNewDateTime)
{
	EzGem->AcceptAndSetNewDateTime(lMsgId, strNewDateTime);

	String str;
	str.sprintf(L"GEM is DateTimeSetRequest(Message ID: %ld, Data: %s)", lMsgId, String(strNewDateTime).c_str());
	GetOvenDB()->SaveLog(str, MESSAGES);
}
//---------------------------------------------------------------------------
// *****************************************************************************
// EZGEM MESSAGE HANDLER END ***************************************************
// *****************************************************************************

// *****************************************************************************
// GEM FUNCTION START        ***************************************************
// *****************************************************************************
WideString TFmGem::GetBstr(const String sVal) const
{
	return static_cast<WideString>(sVal);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::SetSVID(const long svid, const String sVal)
{
	csLock->Acquire();

	WideString wVal = sVal;
	EzGem->SetCurrentStatusValue(svid, wVal.c_bstr());

	csLock->Release();
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::AddSVIDs(void) // 1. Status Variable ID 연결
{
	String sFile = m_sPath + "SVID.TXT";
	if( FileExists(sFile) == false )
	{
		SetMessageStr("SVID Definition file does not exist.");
		ShowMessageDlg();
		return;
	}

	std::auto_ptr<TStringList> sLine (new TStringList());
	std::auto_ptr<TStringList> sData (new TStringList());
	sLine->LoadFromFile(sFile);

	for(int i=0; i<sLine->Count; i++)
	{
		sData->CommaText = sLine->Strings[i];

		if(sData->Count != 3)
			continue;
		else	// VARIANT_BOOL    __fastcall AddSVID(long SVID, BSTR strName, BSTR strDataFormat, BSTR strUnit);
		{
			String svid = sData->Strings[0];
			String name = sData->Strings[1];
			String format = sData->Strings[2];
			EzGem->AddSVID((long)(svid.ToIntDef(0) ), GetBstr(name).c_bstr(), GetBstr(format).c_bstr(), GetBstr("").c_bstr() );
		}
	}
	
	EzGem->CloseMsg(SVID_RECIPE_PARAM);	// 리스트형 SVID 초기화
	EzGem->OpenListItem(SVID_RECIPE_PARAM);
	EzGem->CloseListItem(SVID_RECIPE_PARAM);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::AddRPTIDs(void)
{
	String sFile;

	if (GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		sFile = m_sPath + "RPTID_COWELL.TXT";
	}
	else
	{
		sFile = m_sPath + "RPTID.TXT";
    }
	if( FileExists(sFile) == false )
	{
		SetMessageStr("RPTID Definition file does not exist.");
		ShowMessageDlg();
		return;
	}

	std::auto_ptr<TStringList> sLine (new TStringList());
	std::auto_ptr<TStringList> sData (new TStringList());
	sLine->LoadFromFile(sFile);

	for(int i=0; i<sLine->Count; i++)
	{
		sData->CommaText = sLine->Strings[i];

		if( sData->Count > 2 )
		{
			int len = sData->Count - 2;
			long *svid = new long[len];
			long rptid, ceid;

			rptid = sData->Strings[0].ToInt();
			ceid = sData->Strings[1].ToInt();
			for(int j=0; j<len; j++)
				svid[j] = sData->Strings[j+2].ToInt();

			if( EzGem->SetReport(rptid, svid, len) > 0 )
				EzGem->SetEventReport(ceid, &rptid, 1);

			delete [] svid;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::AddCEIDs(void) // 2. Collective Event ID 연결
{
	String sFile = m_sPath + "CEID.TXT";
	if( FileExists(sFile) == false )
	{
		SetMessageStr("CEID Definition file does not exist.");
		ShowMessageDlg();
		return;
	}

	std::auto_ptr<TStringList> sLine (new TStringList());
	std::auto_ptr<TStringList> sData (new TStringList());
	sLine->LoadFromFile(sFile);

	for(int i=0; i<sLine->Count; i++)
	{
		String str = sLine->Strings[i];
		sData->CommaText = sLine->Strings[i];

		if( sData->Count < 2 )
			continue;
		else	// VARIANT_BOOL    __fastcall AddCEID(long CEID, BSTR strName, BSTR strDescription);
		{
			String sErrContents = "";
			for(int j=1; j<sData->Count; j++)
			{
				sErrContents = sErrContents + sData->Strings[j];
				sErrContents += " ";
			}
			EzGem->AddCEID((long)(sData->Strings[0].ToIntDef(0) ), GetBstr(sErrContents).c_bstr(), GetBstr("").c_bstr() );
        }
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::AddALIDs(void)	// 3. Alarm ID 연결
{
	String sFile = m_sPath + "ALID.TXT";
	if( FileExists(sFile) == false )
	{
		SetMessageStr("ALID Definition file does not exist.");
		ShowMessageDlg();
		return;
	}

	std::auto_ptr<TStringList> sLine (new TStringList());
	std::auto_ptr<TStringList> sData (new TStringList());
	sLine->LoadFromFile(sFile);

	for(int i=0; i<sLine->Count; i++)
	{
		sData->Delimiter = '\t';
		sData->DelimitedText = sLine->Strings[i];

		if( sData->Count < 3 )
			continue;
		else	// VARIANT_BOOL    __fastcall AddAlarmID(long ALID, BSTR strName, BSTR strDescription);
		{
			String sErrContents = "";
			for(int j=2; j<sData->Count; j++)
			{
				sErrContents = sErrContents + sData->Strings[j];
				sErrContents += " ";
			}

			long alid = (long)(sData->Strings[0].ToInt());
			AlarmCatetory[i] = sData->Strings[1].ToInt();

			for (int k = 0; k < GetManager()->ChamberCount; k++)
			{
				EzGem->AddAlarmID(alid + (k*1000), GetBstr(sErrContents).c_bstr(), GetBstr(sErrContents).c_bstr());
			}
        }
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::AddECIDs(void)	// 4. Equipment Constant ID 연결
{
	// ECID는 파일로 관리 하지 않음
	WideString wsVal;

	EzGem->AddECID(ECID_PORT, GetBstr("PortNumber").c_bstr(), GetBstr("").c_bstr(), GetBstr("U2").c_bstr());
	EzGem->SetEquipConstRange(ECID_PORT, GetBstr("2000").c_bstr(), GetBstr("10000").c_bstr());
	EzGem->SetEquipConstValue(ECID_PORT, GetBstr(IntToStr(HsmsPara->nPort)).c_bstr());

	EzGem->AddECID(ECID_DEVICEID,  GetBstr("DeviceID").c_bstr(),  GetBstr("").c_bstr(),  GetBstr("U1").c_bstr());
	EzGem->SetEquipConstRange(ECID_DEVICEID, GetBstr("0").c_bstr(), GetBstr("255").c_bstr());
	EzGem->SetEquipConstValue(ECID_DEVICEID, GetBstr(IntToStr(HsmsPara->nDeviceId)).c_bstr());

	EzGem->AddECID(ECID_T3, GetBstr("T3 Timeout").c_bstr(), GetBstr("second").c_bstr(), GetBstr("U1").c_bstr());
	EzGem->SetEquipConstRange(ECID_T3, GetBstr("1").c_bstr(), GetBstr("255").c_bstr());
	EzGem->SetEquipConstValue(ECID_T3, GetBstr(IntToStr(HsmsPara->nT3)).c_bstr());

	EzGem->AddECID(ECID_T5, GetBstr("T5 Timeout").c_bstr(), GetBstr("second").c_bstr(), GetBstr("U1").c_bstr());
	EzGem->SetEquipConstRange(ECID_T5, GetBstr("1").c_bstr(), GetBstr("255").c_bstr());
	EzGem->SetEquipConstValue(ECID_T5, GetBstr(IntToStr(HsmsPara->nT5)).c_bstr());

	EzGem->AddECID(ECID_T6,	GetBstr("T6 Timeout").c_bstr(), GetBstr("second").c_bstr(), GetBstr("U1").c_bstr());
	EzGem->SetEquipConstRange(ECID_T6, GetBstr("1").c_bstr(), GetBstr("255").c_bstr());
	EzGem->SetEquipConstValue(ECID_T6, GetBstr(IntToStr(HsmsPara->nT6)).c_bstr());

	EzGem->AddECID(ECID_T7, GetBstr("T7 Timeout").c_bstr(), GetBstr("second").c_bstr(), GetBstr("U1").c_bstr());
	EzGem->SetEquipConstRange(ECID_T7, GetBstr("1").c_bstr(), GetBstr("255").c_bstr());
	EzGem->SetEquipConstValue(ECID_T7, GetBstr(IntToStr(HsmsPara->nT7)).c_bstr());

	EzGem->AddECID(ECID_T8, GetBstr("T8 Timeout").c_bstr(), GetBstr("second").c_bstr(), GetBstr("U1").c_bstr());
	EzGem->SetEquipConstRange(ECID_T8, GetBstr("1").c_bstr(), GetBstr("255").c_bstr());
	EzGem->SetEquipConstValue(ECID_T8, GetBstr(IntToStr(HsmsPara->nT8)).c_bstr());

	EzGem->AddECID(ECID_LINKTEST, GetBstr("LinkTestInterVal").c_bstr(), GetBstr("second").c_bstr(), GetBstr("U1").c_bstr());
	EzGem->SetEquipConstRange(ECID_LINKTEST, GetBstr("1").c_bstr(), GetBstr("255").c_bstr());
	EzGem->SetEquipConstValue(ECID_LINKTEST, GetBstr(IntToStr(HsmsPara->nLnkTest)).c_bstr());

	EzGem->AddECID(ECID_RETRY, GetBstr("RetryLimit").c_bstr(), GetBstr("").c_bstr(), GetBstr("U1").c_bstr());
	EzGem->SetEquipConstRange(ECID_RETRY, GetBstr("1").c_bstr(), GetBstr("255").c_bstr());
	EzGem->SetEquipConstValue(ECID_RETRY, GetBstr(IntToStr(HsmsPara->nRetry)).c_bstr());

	EzGem->AddECID(ECID_DEFAULT_COMM_STATE, GetBstr("DefaultCommState").c_bstr(), GetBstr("").c_bstr(), GetBstr("U1").c_bstr());
	EzGem->SetEquipConstRange(ECID_DEFAULT_COMM_STATE, GetBstr("1").c_bstr(), GetBstr("5").c_bstr());
	EzGem->SetEquipConstValue(ECID_DEFAULT_COMM_STATE, GetBstr(IntToStr(static_cast<int>(Ecid->nDefaultCommState))).c_bstr());

	EzGem->AddECID(ECID_DEFAULT_CONTROL_STATE, GetBstr("DefaultControlState").c_bstr(), GetBstr("").c_bstr(), GetBstr("U1").c_bstr());
	EzGem->SetEquipConstRange(ECID_DEFAULT_CONTROL_STATE, GetBstr("1").c_bstr(), GetBstr("3").c_bstr() );
	EzGem->SetEquipConstValue(ECID_DEFAULT_CONTROL_STATE, GetBstr(IntToStr(Ecid->nDefaultControlState)).c_bstr());

	EzGem->AddECID(ECID_ESTABLISH_TIMEOUT, GetBstr("EstablishCommTimeOut").c_bstr(), GetBstr("").c_bstr(), GetBstr("U1").c_bstr());
	EzGem->SetEquipConstRange(ECID_ESTABLISH_TIMEOUT, GetBstr("1").c_bstr(), GetBstr("60").c_bstr());
	EzGem->SetEquipConstValue(ECID_ESTABLISH_TIMEOUT, GetBstr(IntToStr(static_cast<int>(Ecid->nCommReqeustTimeout))).c_bstr());

	EzGem->AddECID(ECID_MDLN, GetBstr("MDLN").c_bstr(), GetBstr("").c_bstr(), GetBstr("A").c_bstr());
	EzGem->SetEquipConstValue(ECID_MDLN, GetBstr(Ecid->sModelName).c_bstr());

	EzGem->AddECID(ECID_SOFTREV, GetBstr("SOFTREV").c_bstr(), GetBstr("").c_bstr(), GetBstr("A").c_bstr());
	EzGem->SetEquipConstValue(ECID_SOFTREV, GetBstr(Ecid->sSoftRev).c_bstr());

	EzGem->AddECID(ECID_MAXSPOOL_TRANSMIT, GetBstr("MaxSpoolTransmit").c_bstr(), GetBstr("").c_bstr(), GetBstr("U2").c_bstr());
	EzGem->SetEquipConstRange(ECID_MAXSPOOL_TRANSMIT, GetBstr("1").c_bstr(), GetBstr("8196").c_bstr() );
	EzGem->SetEquipConstValue(ECID_MAXSPOOL_TRANSMIT, GetBstr(IntToStr(static_cast<int>(Ecid->SPOOLINFO.nMaxSpoolTransmit))).c_bstr());

	EzGem->AddECID(ECID_SPOOL_OVERWRITE, GetBstr("SpoolOverwrite").c_bstr(), GetBstr("").c_bstr(), GetBstr("U1").c_bstr());
	EzGem->SetEquipConstRange(ECID_SPOOL_OVERWRITE, GetBstr("0").c_bstr(), GetBstr("1").c_bstr() );
	EzGem->SetEquipConstValue(ECID_SPOOL_OVERWRITE, GetBstr(IntToStr(static_cast<int>(Ecid->SPOOLINFO.bSpoolOverwrite))).c_bstr());

	EzGem->AddECID(ECID_SPOOLING_MODE, GetBstr("SpoolingMode").c_bstr(), GetBstr("").c_bstr(), GetBstr("U1").c_bstr());
	EzGem->SetEquipConstRange(ECID_SPOOLING_MODE, GetBstr("0").c_bstr(), GetBstr("1").c_bstr() );
	EzGem->SetEquipConstValue(ECID_SPOOLING_MODE, GetBstr(IntToStr(static_cast<int>(Ecid->SPOOLINFO.bSpoolEnabled))).c_bstr());

	EzGem->AddECID(ECID_MAX_SPOOL_SIZE,	GetBstr("MaxSpoolCount").c_bstr(), GetBstr("").c_bstr(), GetBstr("U2").c_bstr());
	EzGem->SetEquipConstRange(ECID_MAX_SPOOL_SIZE, GetBstr("1").c_bstr(), GetBstr("8196").c_bstr() );
	EzGem->SetEquipConstValue(ECID_MAX_SPOOL_SIZE, GetBstr(IntToStr(static_cast<int>(Ecid->SPOOLINFO.nMaxSpoolSize))).c_bstr());

	EzGem->AddECID(ECID_TIME_FORMAT, GetBstr("TimeFormat").c_bstr(), GetBstr("").c_bstr(), GetBstr("U1").c_bstr());
	EzGem->SetEquipConstRange(ECID_TIME_FORMAT, GetBstr("12").c_bstr(), GetBstr("16").c_bstr());
	if( Ecid->nTimeFormat == 14 )
		EzGem->SetEquipConstValue(ECID_TIME_FORMAT, GetBstr("0").c_bstr());
	else
	if( Ecid->nTimeFormat == 16 )
		EzGem->SetEquipConstValue(ECID_TIME_FORMAT, GetBstr("1").c_bstr());
}
//---------------------------------------------------------------------------
bool __fastcall TFmGem::GetChamberNoAndRecipeNameFromPPID(String ppid, int &chamberNo, String &recipe)
{
	String strChamber, strRecipe;

	if( ppid.IsEmpty() )
		return false;

	int pos = ppid.Pos("/");
	if( pos <= 0 )
		return false;

	strChamber = ppid.SubString(1, pos-1);
	strRecipe = ppid.SubString(pos+1, ppid.Length()-pos);
	if( strChamber.Length() != 1 )
		return false;

	chamberNo = strChamber[0] - 'A';
	if( InRange(chamberNo, 0, GetManager()->ChamberCount-1) == false )
		return false;

	recipe = strRecipe;
	return true;
}

//void __fastcall TFmGem::OnS6F15W(long lMsgId)
//{
//	long CEID = 0;
//
//	EzGem->GetU4Item(lMsgId, &CEID);
//
//	long RPTID = CEID + 3000;
//	long DATAID = 0;
//	int chamberNo = 0;
//
//	chamberNo = CEID - CEID_CURRENT_CURE_DATA_A;
//
//	long replyMsgId, lvalue;
//	short int svalue;
//	String strvalue;
//	NormalOvenChamber *chamber = GetChamber(chamberNo);
//
//	replyMsgId = EzGem->CreateReplyMsg(lMsgId);
//
//	EzGem->AddListItem(replyMsgId, 3);
//		EzGem->AddU4Item(replyMsgId, &DATAID, 1);
//		EzGem->AddU4Item(replyMsgId, &CEID, 1);
//
//		EzGem->AddListItem(replyMsgId, GetManager()->ChamberCount);
//			EzGem->AddListItem(replyMsgId, 2);
//				EzGem->AddU4Item(replyMsgId, &RPTID, 1);
//
//				EzGem->OpenListItem(replyMsgId);
//
//					// TotalSegCount
//					if( chamber->IsRun ||
//						(chamber->Step >= STEP_RUNNING && chamber->Step < STEP_DONE) )
//						svalue = chamber->Recipe.TempPtn.Stc.SegCount;
//					else
//						svalue = 0;
//					EzGem->AddU1Item(replyMsgId, &svalue, 1);
//
//					// CurrentSegNo
//					if( chamber->IsRun ||
//						(chamber->Step >= STEP_RUNNING && chamber->Step < STEP_DONE) )
//						svalue = chamber->SegNo;
//					else
//						svalue = 0;
//					EzGem->AddU1Item(replyMsgId, &svalue, 1);
//
//					// TotalCureTime
//					if( chamber->IsRun ||
//						(chamber->Step >= STEP_RUNNING && chamber->Step < STEP_DONE) )
//						svalue = chamber->Recipe.TempPtn.TotalTime;
//					else
//						lvalue = 0;
//					EzGem->AddU2Item(replyMsgId, &lvalue, 1);
//
//					// SetTemp
//					if( chamber->IsRun ||
//						(chamber->Step >= STEP_RUNNING && chamber->Step < STEP_DONE) )
//						strvalue.sprintf(L"%.1f", chamber->SP);
//					else
//						strvalue = "0.0";
//					EzGem->AddAsciiItem(replyMsgId, GetBstr(strvalue).c_bstr(), strvalue.Length());
//
//					// SetTime
//					if( chamber->IsRun ||
//						(chamber->Step >= STEP_RUNNING && chamber->Step < STEP_DONE) )
//					{
//						int idx = chamber->SegNo - 1;
//						if( idx < 0 )
//							idx = 0;
//						strvalue.sprintf(L"%d", chamber->Recipe.TempPtn.Seg[idx].Time);
//					}
//					else
//						strvalue = "0.0";
//					EzGem->AddAsciiItem(replyMsgId, GetBstr(strvalue).c_bstr(), strvalue.Length());
//
//					// MainTemp
//					strvalue.sprintf(L"%.1f", chamber->PV);
//					EzGem->AddAsciiItem(replyMsgId, GetBstr(strvalue).c_bstr(), strvalue.Length());
//
//					if( GetManager()->DPType )
//					{
//						// DiffPressure
//						strvalue.sprintf(L"%.1f", chamber->DPController->PV);
//						EzGem->AddAsciiItem(replyMsgId, GetBstr(strvalue).c_bstr(), strvalue.Length());
//					}
//
//					if( GetManager()->UseO2Analyzer )
//					{
//						// O2Density
//						strvalue.sprintf(L"%.1f", chamber->O2Analyzer->O2Density);
//						EzGem->AddAsciiItem(replyMsgId, GetBstr(strvalue).c_bstr(), strvalue.Length());
//					}
//
//					// PointTemp1 ~ PointTemp5
//					for(int j=0; j<MAX_LOGGERPOINT_COUNT; j++)
//					{
//                    	if (chamber->UseChannel[j])
//                        {
//                            strvalue.sprintf(L"%.1f", chamber->LoggerData[j]);
//                            EzGem->AddAsciiItem(replyMsgId, GetBstr(strvalue).c_bstr(), strvalue.Length());
//                        }
//					}
//				EzGem->CloseListItem(replyMsgId);
//
//	EzGem->SendMsg(replyMsgId);
//}



/**********************************************
Process Program Load Inquire(PPI)
호스트가 Recipe를 내려주기 전 미리 해당 Recipe명을 알려 줌
해당 Recipe가 존재 하는지 판단하여 응답 하여야 함
이미 존재 한다면 Ack 를 0보다 큰값으로 보냄
**********************************************/
void __fastcall TFmGem::OnS7F1W(long lMsgId)
{
	WideString sPPID;
	String recipeName;
	double length = 0;

	EzGem->GetListItemOpen(lMsgId);
		EzGem->GetAsciiItem(lMsgId, &sPPID);
		EzGem->GetU4Item(lMsgId, &length);
	EzGem->GetListItemClose(lMsgId);

	// strPPID이름을 가지고 존재하는지 허용할 수 있는 이름인지 확인 후
	// 아래의 응답을 보냅니다.
	// nAck
	// 0x00 = OK
	// 0x01 = Already Have
	// 0x02 = No Space
	// 0x03 = Invalid PORTID
	// 0x04 = Machine Running

	String str;
	str.sprintf(L"Host PPID inquire as %s.", sPPID.c_bstr());
	GetOvenDB()->SaveLog(str, MESSAGES);

	short nAck = 0x00;

	/*******
	Recipe 가 이미 존재 하는지 판단하여 Ack Code를 분리 함
	1 : Already Exist
	********/
	if( IsGemRecipeExist(sPPID) )
		nAck = 1;

	//S7F1에 대한 응답메세지인 S7F2생성.
	lMsgId = EzGem->CreateReplyMsg(lMsgId);
	EzGem->AddBinaryItem(lMsgId, &nAck , 1);

	EzGem->SendMsg(lMsgId);
}
//---------------------------------------------------------------------------

/**********************************************
//Process Program Send(PPS)
호스트가 Recipe를 내려줌 해당 Recipe는 RECIPE폴더
내부에 저장 됨 <사용안함>
**********************************************/
void __fastcall TFmGem::OnS7F3W(long lMsgId)
{
	int i;
	WideString sPPID = "";
	short nAck = 0x00;
	String str, recipeName, ptnFile;

	// nAck
	// 0 = Accepted
	// 1 = Machine Running
	// 2 = Invalid PPBODY

	EzGem->GetListItemOpen(lMsgId);
		EzGem->GetAsciiItem(lMsgId, &sPPID);

	recipeName = sPPID;

	str.sprintf(L"Host Sended New Recipe as %s.", recipeName.c_str());
	GetOvenDB()->SaveLog(str, MESSAGES, -1);

	ptnFile.sprintf(L"%s%s.ptn", GetRemotePatternFolder().c_str(), recipeName.c_str());

	if( IsGemRecipeExist(recipeName) )
	{
		if( FileExists(ptnFile) )
		{
			DeleteFile(ptnFile);
			str.sprintf(L"Host Sended Old Recipe %s is deleted.", recipeName.c_str());
			GetOvenDB()->SaveLog(str, MESSAGES);
		}
	}

	//create file
	long ret = EzGem->GetFileBinaryItem(lMsgId, GetBstr(ptnFile).c_bstr());
	if( ret >= 0 )
	{
		str.sprintf(L"Host Sended New Recipe %s is saved.", recipeName.c_str());
		GetOvenDB()->SaveLog(str, MESSAGES);
	}
	else
	{
		nAck = 2;
		str.sprintf(L"Host Sended New Recipe %s is NOT saved.", recipeName.c_str());
		GetOvenDB()->SaveLog(str, MESSAGES);
	}

	EzGem->GetListItemClose(lMsgId);

	lMsgId = EzGem->CreateReplyMsg(lMsgId);
	EzGem->AddBinaryItem(lMsgId, &nAck , 1);
	EzGem->SendMsg(lMsgId);
}
//---------------------------------------------------------------------------

/**********************************************
//Process Program Request(PPR)
호스트가 Recipe를 요청함 장비는 해당 Recipe의
존재 여부를 판단하여 응답 하여야 함.
**********************************************/
void __fastcall TFmGem::OnS7F5W(long lMsgId)
{
	WideString sPPID = "";
	String sFilePath = "", recipeName;

	bool bRecipeExist = false;
	short nAck = 0x00;

	EzGem->GetAsciiItem(lMsgId, &sPPID);

	recipeName = sPPID;
	///////////////////////
	//해당 폴더 내부에 Recipe가 존재 하는지 여부를 판단하여
	//bRecipeExist 변수에 값을 셋팅
	///////////////////////
	if( IsGemRecipeExist(recipeName) )
	{
		bRecipeExist = true;
		sFilePath.sprintf(L"%s%s.ptn", GetRemotePatternFolder().c_str(), recipeName.c_str());
	}
	else
	{
		for(int i=0; i<GetManager()->ChamberCount; i++)
		{
			if( GetOvenDB()->IsRecipeExist(i, recipeName) )
			{
				bRecipeExist = true;
				OVEN_RECIPE recipe;
				GetOvenDB()->GetRecipe(i, recipeName, recipe);

				sFilePath.sprintf(L"%sPATTERN_CM_%c\\%d.ptn", GetRootFolder().c_str(), 'A' + i, recipe.PtnNo);
				break;
			}
			else
				bRecipeExist = false;
		}
	}

	lMsgId = EzGem->CreateReplyMsg(lMsgId);

	EzGem->OpenListItem(lMsgId);

	if( bRecipeExist )
	{
		//요청한 Recipe가 존재하므로 S7F6을 통해 Recipe를 전송
		EzGem->AddAsciiItem(lMsgId, GetBstr(sPPID).c_bstr(), sPPID.Length());
		long ret = EzGem->AddFileBinaryItem(lMsgId, GetBstr(sFilePath).c_bstr());

		String str;
		str.sprintf(L"Sended the Recipe [%s].", sPPID.c_bstr());
		GetOvenDB()->SaveLog(str, MESSAGES);
	}
	else
	{
		String str;
		str.sprintf(L"Could not Send the Recipe [%s].", sPPID.c_bstr());
		GetOvenDB()->SaveLog(str, MESSAGES);

		// report that there is no file.
		// Zero-length list
	}
	EzGem->CloseListItem(lMsgId);
	EzGem->SendMsg(lMsgId);
}
//---------------------------------------------------------------------------

/**********************************
//Delete Process Program Send(DPS)
   Recipe삭제 요청
**********************************/
void __fastcall TFmGem::OnS7F17W(long lMsgId)
{
	short nCount, i;

	WideString sPPID="";
	String str, ptnFile;

	short nAck = 0x00;//Acknowledge를 설정 하기 위한 변수
	long lReplyMsgID; //S7F17W에 대한 응답 메세지

	// nAck
	// 0: Accepted
	// 1: Length Error
	// 2: PPID not found

	boost::shared_ptr<TStringList> sPPIDList (new TStringList());
	sPPIDList->Clear();

	//메시지를 열어서 아이템 확인
//	EzGem->GetListItemOpen(lMsgId);

	nCount = EzGem->GetListItemOpen(lMsgId); //삭제할 갯수 := nCount

	if( nCount == 0 )	// 모두 지움.
	{
		ptnFile = GetRemotePatternFolder();
		DeleteAllFile(ptnFile);

		TStringList *list = new TStringList();
		for(int i=0; i<GetManager()->ChamberCount; i++)
		{
			if( GetOvenDB()->GetRecipeList( i, list) )
			{
				for(int j=0; j<list->Count; j++)
				{
					String recipeName = list->Strings[j];
					OVEN_RECIPE recipe;

					GetOvenDB()->GetRecipe(i, recipeName, recipe);

					ptnFile.sprintf(L"%sPATTERN_CM_%c\\%d.ptn", GetRootFolder().c_str(), 'A' + i, recipe.PtnNo);
					if( FileExists(ptnFile) )
						DeleteFile(ptnFile);

					GetOvenDB()->DeleteRecipe(i, recipeName);
				}
			}
		}
		delete list;

		str = "All recipe is deleted by host.";
		GetOvenDB()->SaveLog(str, MESSAGES);
	}
	else
	if( nCount > 0)
	{
		for(i=0; i<nCount; i++)
		{
			EzGem->GetAsciiItem(lMsgId, &sPPID);

			ptnFile.sprintf(L"%s%s.ptn", GetRemotePatternFolder().c_str(), sPPID.c_bstr());
			if( FileExists(ptnFile) )
				DeleteFile(ptnFile);

			for(int j=0; j<GetManager()->ChamberCount; j++)
			{
				if( GetOvenDB()->IsRecipeExist(j, sPPID) )
				{
					OVEN_RECIPE recipe;
					GetOvenDB()->GetRecipe(j, sPPID, recipe);

					ptnFile.sprintf(L"%sPATTERN_CM_%c\\%d.ptn", GetRootFolder().c_str(), 'A' + j, recipe.PtnNo);
					if( FileExists(ptnFile) )
						DeleteFile(ptnFile);

					GetOvenDB()->DeleteRecipe(j, sPPID);

					str.sprintf(L"Host Sended Recipe %s is deleted.", sPPID.c_bstr());
					GetOvenDB()->SaveLog(str, MESSAGES, j);
				}
//				else
//				{
//					nAck = 2;
//					break;
//				}
			}
		}
	}
	else
		nAck = 1;	// Length error

	for(int i=0; i<GetManager()->ChamberCount; i++)
	{
		FmJob->ChamberFrame[i]->UpdateRecipeComboBox();
	}
	EzGem->GetListItemClose(lMsgId);

	//	삭제에 관한 응답을 보내는 부분
	lReplyMsgID = EzGem->CreateReplyMsg(lMsgId);  // S7F18 메세지 생성
	EzGem->AddBinaryItem(lReplyMsgID, &nAck, 1);	// ACK값 메세지에 첨부
	EzGem->SendMsg(lReplyMsgID);
}
//---------------------------------------------------------------------------

/********************************************
// Current EPPD Request (RER)
Host 레시피 리스트 조회/리스트 송부 요청
********************************************/
void __fastcall TFmGem::OnS7F19W(long lMsgId)
{
	long lReplyMsgID;

	EzGem->GetListItemOpen(lMsgId);
	EzGem->GetListItemClose(lMsgId);

	String str;
	lReplyMsgID = EzGem->CreateReplyMsg(lMsgId);
	EzGem->OpenListItem(lReplyMsgID);

	int idx;
	TStringList *sPPIDList = new TStringList;
	if (GetManager()->AutomationKind == SECS_GEM ||
		GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		// 각 챔버의 레시피 리스트 조회
		// 같은 이름은 제외한다.
		for(int i=0; i<GetManager()->ChamberCount; i++)
		{
			TStringList *recList = new TStringList;
			GetOvenDB()->GetRecipeList(i, recList);

			for(int j=0; j<recList->Count; j++)
			{
				str = recList->Strings[j];
				if( sPPIDList->IndexOf(str) < 0 )
					sPPIDList->Add(str);
			}
			delete recList;
		}
	}
	else if (GetManager()->AutomationKind == SECS_GEM_AMD)
	{
		// 레시피이름 앞에 챔버구분자를 붙여서 보낸다.
		for (int i=0; i<GetManager()->ChamberCount; i++)
		{
			TStringList *recList = new TStringList;
			GetOvenDB()->GetRecipeList(i, recList);

			for(int j=0; j<recList->Count; j++)
			{
				str = recList->Strings[j];

				String recipe;
				recipe.sprintf(L"%c_%s", 'A' + i, str.c_str());
				sPPIDList->Add(recipe);
			}
			delete recList;
		}
	}

	// REMOTE_PATTERN 폴더 내의 레시피 조회
	WIN32_FIND_DATA fileData;
	HANDLE fileHandle = FindFirstFile((GetRemotePatternFolder() + "*.*").c_str(), &fileData);
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		AnsiString file, filename, ext;
		do
		{
			file = fileData.cFileName;
			ext = ExtractFileExt(file);
			ext = LowerCase(ext);
			if (ext == ".ptn")
			{
				int pos = file.Pos(ext);
				filename = file.SubString(1, pos-1);
				if (sPPIDList->Find(filename, idx) == false)
				{
					sPPIDList->Add(filename);
				}
			}
		} while(FindNextFile(fileHandle, &fileData));
		FindClose(fileHandle);
	}

	EzGem->AddListItem(lMsgId, sPPIDList->Count);
	for(int i=0; i<sPPIDList->Count; i++)
	{
		str = sPPIDList->Strings[i];
		EzGem->AddAsciiItem(lReplyMsgID, GetBstr(str).c_bstr(), str.Length());
	}
	delete sPPIDList;

	EzGem->CloseListItem(lReplyMsgID);
	EzGem->SendMsg(lReplyMsgID);

	str = "Send all the list of recipes.";
	GetOvenDB()->SaveLog(str, MESSAGES);
}
//---------------------------------------------------------------------------

void __fastcall TFmGem::SendS9F13(const String strMEXP, const String strEDID)
{
	//S9F13 함수 구현 이하 내용은 그대로 추가 해서 사용 하면 됨
	long lMsgId = EzGem->CreateMsg(9,13,0);

	EzGem->OpenListItem(lMsgId);
	EzGem->AddAsciiItem(lMsgId,GetBstr(strMEXP).c_bstr(), strMEXP.Length());
	EzGem->AddAsciiItem(lMsgId,GetBstr(strEDID).c_bstr(), strEDID.Length());
	EzGem->CloseListItem(lMsgId);

	EzGem->SendMsg(lMsgId);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::UpdateSVidPara(const int chamberNo)
{
	NormalOvenChamber *chamber = GetChamber(chamberNo);
	if( chamber == nullptr )
		return;

	String sSvidVal;

	SetSVID(SVID_USERID, GetManager()->UserID);
	SetSVID(SVID_OPERATORID, GetManager()->OperatorID);

	SetSVID(SVID_TOTAL_SEG_COUNT_A + chamberNo*100, IntToStr(chamber->Recipe.TempPtn.Stc.SegCount));
	SetSVID(SVID_CURRENT_SEG_NO_A + chamberNo*100, IntToStr(chamber->SegNo));
	SetSVID(SVID_TOTAL_CURE_TIME_A + chamberNo*100, IntToStr(chamber->Recipe.TempPtn.TotalTime));

	sSvidVal.sprintf(L"%.1f", chamber->SP);
	SetSVID(SVID_CURRENT_SP_A + chamberNo*100, sSvidVal);

	sSvidVal = IntToStr(chamber->Recipe.TempPtn.Seg[chamber->SegNo-1].Time);
	SetSVID(SVID_CURRENT_SET_TIME_A + chamberNo*100, sSvidVal);

	sSvidVal.sprintf(L"%.1f", chamber->PV);
	SetSVID(SVID_MAIN_TEMP_A + chamberNo*100, sSvidVal);

	if( GetManager()->DPType == true &&
		chamber->DPController )
	{
		sSvidVal.sprintf(L"%.1f", chamber->DPController->PV);
		SetSVID(SVID_DIFF_PRESSURE_A + chamberNo*100, sSvidVal);
	}

	if( chamber->O2Analyzer )
	{
		sSvidVal.sprintf(L"%.1f", chamber->O2Analyzer->O2Density);
		SetSVID(SVID_O2_DENSITY_A + chamberNo*100, sSvidVal);
	}

	for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++)
	{
    	if (chamber->UseChannel[i])
        {
            sSvidVal.sprintf(L"%.1f", chamber->LoggerData[i]);
            SetSVID(SVID_LOGGER1_A + chamberNo*100 + i, sSvidVal);
        }
	}

	SetSVID(SVID_PPID_A + chamberNo, chamber->Recipe.RecipeName);	// 2022-03-07 jhkim

//	SendEventReport(CEID_CURRENT_CURE_DATA_A + chamberNo);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::UpdateECidPara(void)
{
	EzGem->SetEquipConstValue(ECID_PORT, 	GetBstr(IntToStr(HsmsPara->nPort)).c_bstr() );
	EzGem->SetEquipConstValue(ECID_DEVICEID,GetBstr(IntToStr(HsmsPara->nDeviceId)).c_bstr() );
	EzGem->SetEquipConstValue(ECID_T3,		GetBstr(IntToStr(HsmsPara->nT3)).c_bstr() );
	EzGem->SetEquipConstValue(ECID_T5,		GetBstr(IntToStr(HsmsPara->nT5)).c_bstr() );
	EzGem->SetEquipConstValue(ECID_T6,		GetBstr(IntToStr(HsmsPara->nT6)).c_bstr() );
	EzGem->SetEquipConstValue(ECID_T7,		GetBstr(IntToStr(HsmsPara->nT7)).c_bstr() );
	EzGem->SetEquipConstValue(ECID_T8,		GetBstr(IntToStr(HsmsPara->nT8)).c_bstr() );
	EzGem->SetEquipConstValue(ECID_LINKTEST,GetBstr(IntToStr(HsmsPara->nLnkTest)).c_bstr() );
	EzGem->SetEquipConstValue(ECID_RETRY,	GetBstr(HsmsPara->nRetry).c_bstr() );

	EzGem->SetEquipConstValue(ECID_DEFAULT_COMM_STATE, 		GetBstr(IntToStr(static_cast<int>(Ecid->nDefaultCommState))).c_bstr() );
	EzGem->SetEquipConstValue(ECID_DEFAULT_CONTROL_STATE, 	GetBstr(IntToStr(Ecid->nDefaultControlState)).c_bstr() );
	EzGem->SetEquipConstValue(ECID_ESTABLISH_TIMEOUT, 		GetBstr(IntToStr(static_cast<int>(Ecid->nCommReqeustTimeout))).c_bstr() );

	EzGem->SetEquipConstValue(ECID_MAXSPOOL_TRANSMIT, 	GetBstr(IntToStr(static_cast<int>(Ecid->SPOOLINFO.nMaxSpoolTransmit))).c_bstr() );
	EzGem->SetEquipConstValue(ECID_SPOOL_OVERWRITE, 	GetBstr(IntToStr(static_cast<int>(Ecid->SPOOLINFO.bSpoolOverwrite))).c_bstr() );
	EzGem->SetEquipConstValue(ECID_SPOOLING_MODE, 		GetBstr(IntToStr(static_cast<int>(Ecid->SPOOLINFO.bSpoolEnabled))).c_bstr() );
	EzGem->SetEquipConstValue(ECID_MAX_SPOOL_SIZE, 		GetBstr(IntToStr(static_cast<int>(Ecid->SPOOLINFO.nMaxSpoolSize))).c_bstr() );

	EzGem->SetEquipConstValue(ECID_MDLN, 				GetBstr(Ecid->sModelName).c_bstr() );
	EzGem->SetEquipConstValue(ECID_SOFTREV, 			GetBstr(Ecid->sSoftRev).c_bstr() );

	if( Ecid->nTimeFormat == 14 )
		EzGem->SetEquipConstValue(ECID_TIME_FORMAT,	GetBstr("0").c_bstr() );
	else
	if( Ecid->nTimeFormat == 16 )
		EzGem->SetEquipConstValue(ECID_TIME_FORMAT, GetBstr("1").c_bstr() );
}
//---------------------------------------------------------------------------

String __fastcall TFmGem::GetErrorCode(const short ret)
{
	String sRet = "";
	int lang = GetManager()->Language;

	switch (ret)
	{
	case LICENSE_PROBLEM:
		sRet = (lang== KOR)? ("라이센스(License)문제발생") : ("License Problem");
		break;
	case CONFIG_FILE_NOTFOUND:
		sRet = (lang == KOR)? ("환경설정 파일(Configuration File)이 존재하지 않음") : ("No Configuration File");
		break;
	case FILE_NOT_FOUND:
		sRet = (lang == KOR)? ("파일이 존재하지 않음") : ("There is no File");
		break;
	case FILE_IO_FAILURE:
		sRet = (lang == KOR)? ("파일 입출력(I/O) 실패") : ("File Input/Output Failure");
		break;
	case FILE_CREATION_ERROR:
		sRet = (lang == KOR)? ("파일 생성 실패") : ("Failure of file creation");
		break;
	case NO_ITEM:
		sRet = (lang == KOR)? ("아이템(Item)이 존재하지 않음") : ("There is no item");
		break;
	case TYPE_MISMATCH:
		sRet = (lang == KOR)? ("ITEM의 타입이 맞지 않음") : ("Type of Item is not matched");
		break;
	case SIZE_MISMATCH:
		sRet = (lang == KOR)? ("ITEM의 크기/개수가 맞지 않음") : ("Size and count of item is not matched");
		break;
	case ITEM_I1_OVERFLOW:
		sRet = (lang == KOR)? ("I1(1Byte Integer) Overflow 발생") : ("I1(1Byte Integer) Overflow happened");
		break;
	case ITEM_I2_OVERFLOW:
		sRet = (lang == KOR)? ("I2(2Byte Integer) Overflow 발생") : ("I2(2Byte Integer) Overflow happened");
		break;
	case ITEM_I4_OVERFLOW:
		sRet = (lang == KOR)? ("I4(4Byte Integer) Overflow 발생") : ("I4(4Byte Integer) Overflow happened");
		break;
	case ITEM_U1_OVERFLOW:
		sRet = (lang == KOR)? ("U1(1Byte Unsigned Integer) Overflow 발생") : ("U1(1Byte Unsigned Integer) Overflow happened");
		break;
	case ITEM_U2_OVERFLOW:
		sRet = (lang == KOR)? ("U2(2Byte Unsigned Integer) Overflow 발생") : ("U2(2Byte Unsigned Integer) Overflow happened");
		break;
	case ITEM_U4_OVERFLOW:
		sRet = (lang == KOR)? ("U4(4Byte Unsigned Integer) Overflow 발생") : ("U4(4Byte Unsigned Integer) Overflow happened");
		break;
	case INVALID_MSGID:
		sRet = (lang == KOR)? ("유효하지않은 Message ID") : ("Invalid Message ID");
		break;
	case INVALID_STREAM:
		sRet = (lang == KOR)? ("유효하지않은 Stream번호") : ("Invalid Stream Number");
		break;
	case INVALID_FUNCTION:
		sRet = (lang == KOR)? ("유효하지않은 Function 번호") : ("Invalid Function Number");
		break;
	case DUPLICATE_SYSTEMBYTE:
		sRet = (lang == KOR)? ("시스템바이트의 중복") : ("System Byte duplicate");
		break;
	case UNDEFINED_STRUCTURE:
		sRet = (lang == KOR)? ("정의되지 않 은Message Structure를 수신") : ("Received the not defined Message Structure");
		break;
	case NOT_CONNECTED:
		sRet = (lang == KOR)? ("아직 연결이 안된 상태임") : ("Not Connected yet");
		break;
	case LOW_LEVEL_ERROR:
		sRet = (lang == KOR)? ("Operating System 상의  Error") : ("Error of Operating System");
		break;
	case NOT_SUPPORTED:
		sRet = (lang == KOR)? ("지원되지 않는기능") : ("Not supported function");
		break;
	case ALREADY_CONNECTED:
		sRet = (lang == KOR)? ("이미 연결되어 있음") : ("Already connected");
		break;
	case ALREADY_STATRED:
		sRet = (lang == KOR)? ("이미 구동된 상태임") : ("Already drived");
		break;
	case THREAD_NULL:
		sRet = (lang == KOR)? ("각종 쓰레드(THREAD) 에러") : ("Various Thread error");
		break;
	case CREATE_EVENT_FAIL:
		sRet = (lang == KOR)? ("이벤트(EVENT)발생 에러") : ("Event happened error");
		break;
	case SERIAL_OPEN_FAIL:
		sRet = (lang == KOR)? ("시리얼 포트 구동 에러") : ("Serial Port activating error");
		break;
	case SERIAL_SETUP_FAIL:
		sRet = (lang == KOR)? ("시리얼 포트 초기화 에러") : ("Serial Port initializing error");
		break;
	case TIMER_CREATE_FAIL:
		sRet = (lang == KOR)? ("타이머 생성 실패") : ("Timer Creating error");
		break;
	case NOT_STARTED_YET:
		sRet = (lang == KOR)? ("프로그램이 구동되지 않았음") : ("Program not activated");
		break;
	case SOCKET_INUSE:
		sRet = (lang == KOR)? ("소켓(Socket)이 이미 사용중임") : ("Socket is used already");
		break;
	case SOCKET_STARTERROR:
		sRet = (lang == KOR)? ("소켓 구동 에러") : ("Socket Activating error");
		break;
	case SOCKET_INVALID:
		sRet = (lang == KOR)? ("소켓이 유효하지 않음") : ("Invalid Socket");
		break;
	case SOCKET_WINDOWERROR:
		sRet = (lang == KOR)? ("메시지 통신을 위한 Worker Window 에러") : ("Worker Window error for communitating message");
		break;
	case SOCKET_LOCALNAME:
		sRet = (lang == KOR)? ("Host Name을 알아오는 과정에서 에러발생") : ("Error happened during getting Host name");
		break;
	case SOCKET_CONNECTERROR:
		sRet = (lang == KOR)? ("소켓접속 에러") : ("Socket connecting error");
		break;
	case SOCKET_SETEVENT:
		sRet = (lang == KOR)? ("소켓 이벤트 설정 에러") : ("Socket Event Selecting error");
		break;
	case SOCKET_RESOLVE:
		sRet = (lang == KOR)? ("소켓함수 Resolve()수행 중 에러") : ("Resolve() executing error of Socket functions");
		break;
	default:
		break;
	}

	return sRet;
}
//---------------------------------------------------------------------------

void  __fastcall TFmGem::Save_Para(const AnsiString filepath)
{
//   int rcnt,i;
//   rcnt = tMRTmp.StepCnt;
//   //1. rename file to new
//   for (i=0; i<rcnt; i++) {
//      tMRTmp.Mr[i].RfPwr  = GetIntValid(SY.RfMin,     SY.RfMax,      tMRTmp.Mr[i].RfPwr );
//      tMRTmp.Mr[i].CnTime = GetIntValid(SY.MinClean,  SY.MaxClean,   tMRTmp.Mr[i].CnTime);
//	  tMRTmp.Mr[i].Gas1   = GetIntValid(SY.MfcLow[0], SY.MfcHigh[0], tMRTmp.Mr[i].Gas1  );
//      tMRTmp.Mr[i].Gas2   = GetIntValid(SY.MfcLow[1], SY.MfcHigh[1], tMRTmp.Mr[i].Gas2  );
//      tMRTmp.Mr[i].Gas3   = GetIntValid(SY.MfcLow[2], SY.MfcHigh[2], tMRTmp.Mr[i].Gas3  );
//
//	  FmMn->S7F23_RWIniFile(1, filepath, i, 0);//1:write, 2:erase
//   }
//   for (i=rcnt; i<10; i++) {
//	  FmMn->S7F23_RWIniFile(2, filepath, i, 0);//1:write, 2:erase
//   }
}
//---------------------------------------------------------------------------

void __fastcall TFmGem::SetProcessState(const int chamberNo, const int nProcessState)
{
	WideString sVal;

	if( GemState[chamberNo].nProcessState == nProcessState )       //2003 : Equipment State    SVID_PROC_STATE
		return;

	GemState[chamberNo].nPrevProcessState = GemState[chamberNo].nProcessState;
	GemState[chamberNo].nProcessState = nProcessState;

	sVal.sprintf(L"%d", GemState[chamberNo].nPrevProcessState);
	SetSVID(SVID_PREV_PROCSTATE_A + chamberNo*10, sVal.c_bstr()); //EzGem->GetCurrentStatusValue(SVID_PROC_STATE));       //2004 : PrevEquipment State
	sVal.sprintf(L"%d", nProcessState);
	SetSVID(SVID_PROC_STATE_A + chamberNo*10, sVal.c_bstr());

//	if( EzGem->GetCurrentStatusValue(SVID_PREV_PROCSTATE) != EzGem->GetCurrentStatusValue(SVID_PROC_STATE))
	{
		SendEventReport(CEID_PROCSTATE_CHANGE_A + chamberNo);    //Equipment State Change
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::SetAlarm(const int chamberNo, const int errCode)
{
	long lALID=0;
	//short nALCD = 0x86;
	short nALCD = 0x80, ALCate;
	//Bit 8:1 Set => Alarm Set
	//      0 Set => Alarm Clear
	//Bit 1~7 = Alarm Category
	//1=Personal Safety
	//2=Equipment Safety
	//3=Parameter Control Warning
	//4=Parameter Control Error
	//5=Inrecoverable Error
	//6=Equipment Status Warning
	//7=Attension Flag
	//8=Data Integrity
	/*
	switch (ALCD) {
      case 1: ALCate = 1; break;
      case 2: ALCate = 2; break;
      case 3: ALCate = 4; break;
      case 4: ALCate = 8; break;
	  case 5: ALCate = 16; break;
	  case 6: ALCate = 32; break;
      case 7: ALCate = 64; break;
      default : ALCate = 0;break;
	}
	PERSONAL_SAFETY
	EQUIP_SAFETY
	PARA_CONTROL_WARNING
	PARA_CONTROL_ERROR
	IRRECOVERABLE ERROR
	EQUIP_STATUS_WARNING
	ATTENTION_FLAG
	*/
	nALCD += AlarmCatetory[errCode];
	//lALID = ErrToAlmCodeConvert(errCode);
	if (GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		lALID = errCode + chamberNo*1000;
	}
	else
	{
		lALID = errCode;
	}

	// S5F1
	csLock->Acquire();
	EzGem->SendAlarmReport(lALID, nALCD);
	csLock->Release();

	SetSVID(SVID_PORTID, IntToStr(chamberNo));
    if (GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		SetSVID(SVID_ALID, FloatToStr(lALID));
	}
	else
	{
		SetSVID(SVID_ALID, IntToStr(errCode));
	}
	SendEventReport(CEID_ALARM_SET);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::ClearAlarm(const int chamberNo, const int errCode)
{
	long lALID = 0;
	//short nALCD = 0x06;
	short nALCD = 0x00;
    // 20190124 - Alarm Clear Modify - jhlim
	//nALCD += AlarmCatetory[errCode];
	//lALID = ErrToAlmCodeConvert(errCode);
	if (GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		lALID = errCode + chamberNo*1000;
	}
	else
	{
		lALID = errCode;
	}

	// S5F1
	csLock->Acquire();
	EzGem->SendAlarmReport(lALID, nALCD);
	csLock->Release();

	SetSVID(SVID_PORTID, IntToStr(chamberNo));
    if (GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		SetSVID(SVID_ALID, FloatToStr(lALID));
	}
	else
	{
		SetSVID(SVID_ALID, IntToStr(errCode));
	}
	SendEventReport(CEID_ALARM_CLEAR);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::SendEventReport(const long CEID)
{
//	if (IsGemConnected() == false)
//	{
//		return;
//	}
	csLock->Acquire();
	if (EzGem->SendEventReport(CEID) < 0)
	{
		String str;
		int ceid = CEID;
		str.sprintf(L"S6F11 Send Fail %d", ceid);
		GetOvenDB()->SaveLog(str, MESSAGES);
    }
	csLock->Release();
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::SendTerminalMsg(const String Str)
{
	short TID = 0;  // 터미널번호 (그냥 무시... 사용할 일 없음)
	long lMsgID, len;
	len = Str.Length();

	lMsgID = EzGem->CreateMsg(10,1,0);    // Stream=10, Funtion=1, Reply = 0 ,   (Reply는 옵션이므로  1로 해도 됩니다, 1로 하면 응답을 요청하는 경우임)
	EzGem->OpenListItem(lMsgID);
	EzGem->AddBinaryItem(lMsgID, &TID, 1);

	EzGem->AddAsciiItem(lMsgID, GetBstr(Str).c_bstr(), len);
	EzGem->CloseListItem(lMsgID);
	EzGem->SendMsg(lMsgID);
}
//---------------------------------------------------------------------------
// *****************************************************************************
// GEM FUNCTION END          ***************************************************
// *****************************************************************************

void __fastcall TFmGem::btnGemStartClick(TObject *Sender)
{
	EzGem->DeviceID	= static_cast<short>(GemParaGrid->Cells[1][0].ToIntDef(0));
	EzGem->IP       = GetBstr(GemParaGrid->Cells[1][1]);
	EzGem->Port     = static_cast<short>(GemParaGrid->Cells[1][2].ToIntDef(5000));

	int nMode;
	if(GemParaGrid->GetRadioIdx(1, 10, nMode))
		EzGem->PassiveMode = (nMode == 0) ? true : false;

	String str;
	str = UpperCase(Application->Title);
	EzGem->SetModelName(GetBstr(str).c_bstr());
	str = GetInformation(Application->ExeName, "ProductVersion");
	EzGem->SetSoftwareRev(GetBstr(str).c_bstr());
	Ecid->nTimeFormat = 16;   //14

	// Add Remote command
	EzGem->AddRemoteCommand(GetBstr("PP-SELECT").c_bstr());
	EzGem->AddRemoteCommand(GetBstr("START").c_bstr());
	EzGem->AddRemoteCommand(GetBstr("STOP").c_bstr());
	EzGem->AddRemoteCommand(GetBstr("REMOTE").c_bstr());
	EzGem->AddRemoteCommand(GetBstr("LOCAL").c_bstr());

	///////CEID////////////////
	AddCEIDs();
	//////SVID////////
	AddSVIDs();
	//////ALARM//////
	AddALIDs();
	/////////////////
	AddECIDs();

	AddRPTIDs();

	// Recipe Management
	DisableAutoReply();

	EzGem->EnableCommunication();
	//FORMAT.SML에 등록된 구조 적용
	String sFile;
	sFile.sprintf(L"%sFORMAT.SML", GetDataFolder().c_str());
	EzGem->SetFormatFile(GetBstr(sFile).c_bstr());
	EzGem->SetFormatCheck(true);
	//-----------------------------------------------
	sFile.sprintf(L"%sVS_GEM.LOG", GetLogFolder().c_str());
	EzGem->SetLogFile(GetBstr(sFile).c_bstr(), true );
	EzGem->SetFormatCodeALID(ITEM_UINT4B);	// U2=52, I2=32, U4=54, I4=34, U1=51, I1=31
	EzGem->SetFormatCodeSVID(ITEM_UINT4B);
	EzGem->SetFormatCodeDATAID(ITEM_UINT4B);
	EzGem->SetFormatCodeCEID(ITEM_UINT4B);
	EzGem->SetFormatCodeECID(ITEM_UINT4B);
	//   Ezgem->SetFormatCodeRPTID();
	EzGem->SetFormatCodeTRACEID(ITEM_UINT4B);

//	for(int i=0; i<GetManager()->ChamberCount; i++)
//		GemState[i].bEnabled = true;

	int nRet = EzGem->Start();
	if(nRet < 0)
	{
//		SetMessageStr(GetErrorCode(nRet));
//		ShowMessageDlg();
		return;
	}

	GetOvenDB()->SaveLog("GEM is Online Requested.", MESSAGES);

	if( Ecid->nDefaultControlState == CONTROL_ONLINE_LOCAL )
		EzGem->GoOnlineLocal();
	else
	if( Ecid->nDefaultControlState == CONTROL_ONLINE_REMOTE )
		EzGem->GoOnlineRemote();
	else
	if( Ecid->nDefaultControlState == CONTROL_EQUIPMENT_OFFLINE )
		EzGem->GoOffline();

	UpdateECidPara();
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::btnGemStopClick(TObject *Sender)
{
	String str;
	int nRet = EzGem->Stop();
	if( nRet < 0 )
	{
		SetMessageStr(GetErrorCode(nRet));
		ShowMessageDlg();
		return;
	}

	str = "GEM stop normally.";
	GetOvenDB()->SaveLog(str, MESSAGES);

	for(int i=0; i<GetManager()->ChamberCount; i++)
	{
		GemState[i].bConnectedFg = false;
		GemState[i].bEstablish = false;
	}
	btnGemOfflineClick(Sender);

	str = "GEM is stop.";
	GetOvenDB()->SaveLog(str, MESSAGES);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::btnGemLocalClick(TObject *Sender)
{
	EzGem->GoOnlineLocal();

	Ecid->nDefaultControlState = CONTROL_ONLINE_LOCAL;
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::btnGemOfflineClick(TObject *Sender)
{
	for(int i=0; i<GetManager()->ChamberCount; i++)
	{
		GemState[i].nPrevControlState = GemState[i].nControlState;
//		EzGem->SetCurrentStatusValue(SVID_PREV_CONTROLSTATE, GetBstr(IntToStr(GemState[i].nPrevControlState)).c_bstr() );
		SetSVID(SVID_PREV_CONTROLSTATE, IntToStr(GemState[i].nPrevControlState));

		GemState[i].nControlState = CONTROL_EQUIPMENT_OFFLINE;
//		EzGem->SetCurrentStatusValue(SVID_CONTROLSTATE, GetBstr(IntToStr(GemState[i].nControlState)).c_bstr() );
		SetSVID(SVID_CONTROLSTATE, IntToStr(GemState[i].nControlState));

		SendEventReport(CEID_OFFLINE);
	}
	EzGem->GoOffline();
	UdpateGemState();

	Ecid->nDefaultControlState = CONTROL_EQUIPMENT_OFFLINE;
	EnableAutoReply();

	String str;
	str = "GEM is Offline.";
	GetOvenDB()->SaveLog(str, MESSAGES);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::btnGemRemoteClick(TObject *Sender)
{
//	if(GemState->bConnectedFg)
//	{
//
//	}
	EzGem->GoOnlineRemote();

	Ecid->nDefaultControlState = CONTROL_ONLINE_REMOTE;
	DisableAutoReply();
}
//---------------------------------------------------------------------------

// *****************************************************************************
// FUNCTION TEST START       ***************************************************
// *****************************************************************************
void __fastcall TFmGem::edtTermMsgKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if(Key == VK_RETURN)
     	SendTerminalMsg(edtTermMsg->Text);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::S9F13TimerTimer(TObject *Sender)
{
/*	if(GemState->nControlState != CONTROL_ONLINE_REMOTE)
		return;

	m_nS9F13TimerDly++;
	if(m_nS9F13TimerDly > HsmsPara->nCTOut)
	{
		//CT발생 시 다음과 같은 순서로 처리
		SendS9F13("S02F41","START");	//S9F13을 Host로 전송 전송시 내용을 담아야 함
										//Host에서 보내 달라고 하는데로 보내야 함 이는 협의를 통해서 결정
										//ex)START라고 표시된 부분이  LC<->START 와 같이 표현 될 수도 있음
		S9F13Timer->Enabled = false;    	//전송 후 Timer를 끔
	}

	//CT 구간이 Load Complete <-> START 구간이라면 Load Complete 후에 Timer를 살려 줍니다.
	//CT는 동시에 여러개가 발생 할 수 있으므로 CT가 정의 된 구간마다 Timer를 따로 할당하여 처리 해야 합니다.

	//해당 Timer는 START를 받으면 false 시켜 준다. 받지 못하면 timer 이벤트를 타게 되어 S9F13을 전송 합니다.
	 //START 부분은 //SWPark20110704 주석으로 검색 해 보세요.   */
}
//---------------------------------------------------------------------------

void __fastcall TFmGem::rdgSelStateClick(TObject *Sender)
{
	int nState;
	SetSVID(SVID_PORTID, IntToStr(rdgSelCm->ItemIndex) );

	switch (rdgSelState->ItemIndex)
	{
	case 0:
		nState = STATE_IDLE;
		break;
	case 1:
		nState = STATE_SETUP;
		break;
	case 2:
		nState = STATE_READY;
		break;
	case 3:
		nState = STATE_EXECUTING;
		break;
	default:
        break;
	}

	SetProcessState(rdgSelCm->ItemIndex, nState);
}
//---------------------------------------------------------------------------

void __fastcall TFmGem::btnSetAlarmClick(TObject *Sender)
{
	int AlmId = edtAlarmNo->Text.Trim().ToIntDef(0);
	SetAlarm(rdgSelCm->ItemIndex, AlmId);
}
//---------------------------------------------------------------------------

void __fastcall TFmGem::btnRecipeChangeClick(TObject *Sender)
{
	if( edtRecipe->Text.IsEmpty() )
		return;
	SendPPChangedEvent(rdgSelCm->ItemIndex, 1, edtRecipe->Text, PPCHANGESTATUS_CHANGED);

//	SetSVID(SVID_PORTID, IntToStr(rdgSelCm->ItemIndex) );
//	SetSVID(SVID_PPID_CHANGE_NAME, edtRecipe->Text);
//	SetSVID(SVID_PPID_CHANGE_STATUS, PPCHANGESTATUS_CHANGED);
//	SendEventReport(CEID_PP_CHANGED);
}
//---------------------------------------------------------------------------

void __fastcall TFmGem::btnJobStartClick(TObject *Sender)
{
	if( edtRecipe->Text.IsEmpty() )
		return;

	SendJobStartedEvent(rdgSelCm->ItemIndex, 1, L"LotID Test", edtRecipe->Text);
}
//---------------------------------------------------------------------------

void __fastcall TFmGem::btnJobEndClick(TObject *Sender)
{
	if( edtRecipe->Text.IsEmpty() )
		return;

	SetSVID(SVID_PORTID, IntToStr(rdgSelCm->ItemIndex) );

	if (GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		MakeLotIDList(L"LotID Test");
	}
	else
	{
		SetSVID(SVID_LOTID, L"LotID Test");
	}

	SetSVID(SVID_PPID, edtRecipe->Text);

	SendEventReport(CEID_JOB_END);
}
//---------------------------------------------------------------------------

void __fastcall TFmGem::btnJobCancelClick(TObject *Sender)
{
	if( edtRecipe->Text.IsEmpty() )
		return;

	SetSVID(SVID_PORTID, IntToStr(rdgSelCm->ItemIndex) );

	if (GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		MakeLotIDList(L"LotID Test");
	}
	else
	{
		SetSVID(SVID_LOTID, L"LotID Test");
	}

	SetSVID(SVID_PPID, edtRecipe->Text);
	SetSVID(SVID_USERID, L"UserID Test");

	SendEventReport(CEID_JOB_CANCEL);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::btnPPSelectClick(TObject *Sender)
{
	if( edtRecipe->Text.IsEmpty() )
		return;

	SetSVID(SVID_PORTID, IntToStr(rdgSelCm->ItemIndex) );
	SetSVID(SVID_PPID, edtRecipe->Text);
	SendEventReport(CEID_PP_SELECTED);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::ClearAlarmAll(const int chamberNo)
{
	NormalOvenChamber *chamber = nullptr;

	chamber = GetChamber(chamberNo);
	if( chamber == nullptr )
		return;

	for(int i=0; i<MAX_ALARM_COUNT; i++)
	{
		if( chamber->AlarmStatus[i] )
		{
			ClearAlarm(chamberNo, i);
		}
	}
}
void __fastcall TFmGem::btnBcrClick(TObject *Sender)
{
	if( edtBcr->Text.IsEmpty() == false )
		SendBarcodeInfoEvent(rdgSelCm->ItemIndex, edtBcr->Text);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::SendJobEndEvent(const int chamberNo, const String lotid, const String ppid)
{
	SetSVID(SVID_PORTID, IntToStr(chamberNo));

 	if (GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		MakeLotIDList(lotid);
		SetSVID(SVID_USERID, GetManager()->UserID);
	}
	else
	{
		SetSVID(SVID_LOTID, lotid);
	}

	SetSVID(SVID_PPID, ppid);

	// S6F11
	SendEventReport(CEID_JOB_END);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::SendJobStartedEvent(const int chamberNo, int ptnNo, const String lotid, const String ppid)
{
	SetSVID(SVID_PORTID, IntToStr(chamberNo));

	if (GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		MakeLotIDList(lotid);
	}
	else
	{
		SetSVID(SVID_LOTID, lotid);
	}

	SetSVID(SVID_PPID, ppid);

	if (GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		MakeRecipeParamSvid(chamberNo, ptnNo, ppid);
		SetSVID(SVID_USERID, GetManager()->UserID);
	}

	SendEventReport(CEID_JOB_START);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::SendJobStartFailEvent(const int chamberNo, const String lotid, const String ppid)
{
	SetSVID(SVID_PORTID, IntToStr(chamberNo));

	if (GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		MakeLotIDList(lotid);
	}
	else
	{
		SetSVID(SVID_LOTID, lotid);
	}

	SetSVID(SVID_PPID, ppid);

	SendEventReport(CEID_JOB_START_FAIL);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::SendJobCancelEvent(const int chamberNo, const String lotid, const String ppid)
{
	SetSVID(SVID_PORTID, IntToStr(chamberNo));

	if (GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		MakeLotIDList(lotid);
	}
	else
	{
		SetSVID(SVID_LOTID, lotid);
	}

	SetSVID(SVID_PPID, ppid);

	SendEventReport(CEID_JOB_CANCEL);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::SendJobCancelFailEvent(const int chamberNo, const String lotid, const String ppid)
{
	SetSVID(SVID_PORTID, IntToStr(chamberNo));

	if (GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		MakeLotIDList(lotid);
	}
	else
	{
		SetSVID(SVID_LOTID, lotid);
	}

	SetSVID(SVID_PPID, ppid);

	SendEventReport(CEID_JOB_CANCEL_FAIL);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::SendJobValidation(const int chamberNo, const String lotid, const String ppid)
{
	SetSVID(SVID_PORTID, IntToStr(chamberNo));
	MakeLotIDList(lotid);
	SetSVID(SVID_PPID, ppid);
	SetSVID(SVID_USERID, GetManager()->UserID);

	SendEventReport(CEID_JOB_VALIDATION);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::SendBarcodeInfoEvent(const int chamberNo, const String barcode)
{
	SetSVID(SVID_PORTID, IntToStr(chamberNo));
 //	SetSVID(SVID_LOTID, barcode);
 	if (GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		MakeLotIDList(barcode);
	}
	else
	{
		SetSVID(SVID_LOTID, barcode);
	}

	SendEventReport(CEID_BCR_READ);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::SendPPSelectedEvent(const int chamberNo, int ptnNo, const String ppid)
{
	SetSVID(SVID_PORTID, IntToStr(chamberNo));
	SetSVID(SVID_PPID, ppid);
	if (GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		MakeRecipeParamSvid(chamberNo, ptnNo, ppid);
	}

	SendEventReport(CEID_PP_SELECTED);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::SendPPChangedEvent(const int chamberNo, int ptnNo, const String ppid, int changeStatus)
{
	SetSVID(SVID_PORTID, IntToStr(chamberNo));
	SetSVID(SVID_PPID_CHANGE_NAME, ppid);
	SetSVID(SVID_PPID_CHANGE_STATUS, IntToStr(changeStatus));
	if (GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		MakeRecipeParamSvid(chamberNo, ptnNo, ppid);
	}

	SendEventReport(CEID_PP_CHANGED);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::SendDoorStatusEvent(const int chamberNo, const int doorStatus)	// 2021-11-02
{
	SetSVID(SVID_DOOR_STATUS_A + chamberNo, IntToStr(doorStatus));
	SendEventReport(CEID_DOOR_STATUS_A + chamberNo);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::SendDoorEvent(const int chamberNo)
{
	NormalOvenChamber *chamber = GetChamber(chamberNo);

	if( chamber == nullptr )
		return;

	int doorStatus = DOOR_UNKOWN;

	if (chamber->IsDoorClosed())
	{
		doorStatus = DOOR_CLOSED;
	}
	else
	{
		doorStatus = DOOR_OPENED;
	}

	if( PrevDoorStatus[chamberNo] != doorStatus )
	{
		PrevDoorStatus[chamberNo] = doorStatus;
		SendDoorStatusEvent(chamberNo, doorStatus);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::SendOPIDStatusEvent(const String opid)
{
	SetSVID(SVID_OPERATORID, opid);
	SendEventReport(CEID_OP_ID_STATUS);
}
//---------------------------------------------------------------------------
bool __fastcall TFmGem::IsGemConnected()
{
	return GemState[0].bConnectedFg;
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::MakeRecipeParamSvid(int chamberNo, int ptnNo, String ppid)
{
	PTN ptn;
	LoadTempPtn(chamberNo, ptnNo, ptn);

	if (ptn.IsLoaded == false)
	{
		return;
	}

	String str;
	long svid = SVID_RECIPE_PARAM;
	int segCount = ptn.Stc.SegCount;
	int eventCount;
	
	EzGem->CloseMsg(svid);
	EzGem->OpenListItem(svid);
	{
		// 챔버 번호
		str.sprintf(L"%d", chamberNo);
		EzGem->AddAsciiItem(svid, GetBstr(str).c_bstr(), str.Length());	
		// 레시피 이름	
		str = ppid;
		EzGem->AddAsciiItem(svid, GetBstr(str).c_bstr(), str.Length());	
		// 세그먼트 갯수
		str.sprintf(L"%d", segCount);
		EzGem->AddAsciiItem(svid, GetBstr(str).c_bstr(), str.Length());		
		
		// 세그먼트 리스트
		for (int i = 0; i < segCount; i++)
		{
			EzGem->OpenListItem(svid);
			{
				// 세그먼트 시간
				str.sprintf(L"%d", ptn.Seg[i].Time);
				EzGem->AddAsciiItem(svid, GetBstr(str).c_bstr(), str.Length());	
				// 목표 온도
				str.sprintf(L"%d", (int)ptn.Seg[i].Tsp);
				EzGem->AddAsciiItem(svid, GetBstr(str).c_bstr(), str.Length());	

				eventCount = 0;
				for (int j = 0; j < 8; j++) 
				{
					if (ptn.Seg[i].EvKind[j] != 0)
					{	
						eventCount++;
					}
				}
				// 이벤트 갯수
				str.sprintf(L"%d", eventCount);
				EzGem->AddAsciiItem(svid, GetBstr(str).c_bstr(), str.Length());		
								
				// 이벤트 리스트
				for (int j = 0; j < eventCount; j++) 
				{
					EzGem->OpenListItem(svid);
					{
						// 이벤트 이름
						int eventKind = ptn.Seg[i].EvKind[j];
						if (eventKind == ET_PVHIGH)
						{
							if (ptn.Seg[i].OnType[j] == 0)
							{
								eventKind =  ET_PVHIGH_CLEAR;
							}
							else if (ptn.Seg[i].OnType[j] == 3)
							{
								eventKind =  ET_PVHIGH;
							}
						}
						else if (eventKind == ET_PVLOW)
						{
							if (ptn.Seg[i].OnType[j] == 0)
							{
								eventKind =  ET_PVLOW_CLEAR;
							}
							else if (ptn.Seg[i].OnType[j] == 4)
							{
								eventKind =  ET_PVLOW;
							}
						}

						str = GetEventName(eventKind);
						EzGem->AddAsciiItem(svid, GetBstr(str).c_bstr(), str.Length());

						if (eventKind == ET_PVHIGH || eventKind == ET_PVLOW)
						{
							// 이벤트 파라미터 1
							str.sprintf(L"%d", ptn.Seg[i].OffPe[j]);
							EzGem->AddAsciiItem(svid, GetBstr(str).c_bstr(), str.Length());
                        }
						else if (eventKind == ET_PURGE || eventKind == ET_COOLING)
						{
							// 이벤트 파라미터 1
							str.sprintf(L"%d", ptn.Seg[i].OnType[j]);
							EzGem->AddAsciiItem(svid, GetBstr(str).c_bstr(), str.Length());
							// 이벤트 파라미터 2
							str.sprintf(L"%d", ptn.Seg[i].OffPe[j]);
							EzGem->AddAsciiItem(svid, GetBstr(str).c_bstr(), str.Length());
						}
					}
					EzGem->CloseListItem(svid);
				}
			}
			EzGem->CloseListItem(svid);			
		}
	}
	EzGem->CloseListItem(svid);
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::MakeLotIDList(String lotid)
{
	String strValue;
	long svid;
	TStringList *LotIDList = new TStringList;
	LotIDList->CommaText = lotid;

	svid = SVID_LOT_ID_LIST;
	EzGem->CloseMsg(svid);
	EzGem->OpenListItem(svid);

	for (int i = 0; i < LotIDList->Count; i++)
	{
		EzGem->OpenListItem(svid);
		strValue = LotIDList->Strings[i];
		EzGem->AddAsciiItem(svid, GetBstr(strValue).c_bstr(), strValue.Length());
		EzGem->CloseListItem(svid);
	}

	EzGem->CloseListItem(svid);
}
//--------------------------------------------------------------------------
void __fastcall TFmGem::FormShow(TObject *Sender)
{
	FillGemPara();
	UpdateECidPara();
	UdpateGemState();
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::EnableAutoReply()
{
	EzGem->EnableAutoReply(7, 1);	// S7,F1 : Process Program Load Inquire(PPI)
	EzGem->EnableAutoReply(7, 3);	// S7,F3 : Process Program Send(PPS)
	EzGem->EnableAutoReply(7, 5);	// S7,F5 : Process Program Request(PPR)
	EzGem->EnableAutoReply(7, 17);	// S7,F17 : Delete Process Program Send(DPS)
	EzGem->EnableAutoReply(7, 19);	// S7,F19 : Current EPPD Request(RER)
}
//---------------------------------------------------------------------------
void __fastcall TFmGem::DisableAutoReply()
{
	EzGem->DisableAutoReply(7, 1);	// S7,F1 : Process Program Load Inquire(PPI)
	EzGem->DisableAutoReply(7, 3);	// S7,F3 : Process Program Send(PPS)
	EzGem->DisableAutoReply(7, 5);	// S7,F5 : Process Program Request(PPR)
	EzGem->DisableAutoReply(7, 17);	// S7,F17 : Delete Process Program Send(DPS)
	EzGem->DisableAutoReply(7, 19);	// S7,F19 : Current EPPD Request(RER)
}
//---------------------------------------------------------------------------

