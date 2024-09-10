//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <System.DateUtils.hpp>
#include "ChamberFrame.h"
#include "NormalOvenChamber.h"
#include "NormalOvenSystemManager.h"
#include "HelperFunctions.h"
#include "Defines.h"
#include "MainTempController.h"
#include "ST340.h"
#include "I7018.h"
#include "I7000DIOModule.h"
#include "DiffPressure.h"
#include "MoonsStepMotor.h"
#include "ZR5.h"
#include "JobEndForm.h"
#include "AlarmForm.h"
#include "LoginForm.h"
#include "ShowPtnForm.h"
#include "JobForm.h"
#include "GEMForm.h"
#include "VSOvenBoard.h"
#include "REX_P250.h"
#include "COWELLValidationForm.h"
#include "EzNetForm.h"
#include "JSCKValidationForm.h"
// 20180227 - FTP (OFILM MES) - jhlim
#include "FTPLotInfoForm.h"

#include "TCardRecipeLoadingForm.h"
#include "TCardChamberRecipeForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvCombo"
#pragma link "AdvGlassButton"
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "AdvPanel"
#pragma link "AdvSmoothProgressBar"
#pragma link "AdvSmoothStatusIndicator"
#pragma link "BaseGrid"
#pragma link "AdvSmoothButton"
#pragma link "AdvEdit"
#pragma link "AdvUtil"
#pragma link "AdvGlowButton"
#pragma resource "*.dfm"
TFrmChamber *FrmChamber;
//---------------------------------------------------------------------------
enum GRAPH_MODE
{ GM_NORMAL, GM_VALUE_LEGEND, GM_HIDE_LEGEND };

bool IsValidOperatorID(AnsiString id)
{
	if( id.IsEmpty() )
		return false;

	if( id.Length() != 8 )
	{
		return false;
	}

	for(int i=1; i<=id.Length(); i++)
	{
		char c = id[i];
		if( !(c >= '0' && c <= '9') )
		{
			return false;
		}
	}

	return true;
}

__fastcall TFrmChamber::TFrmChamber(TComponent* Owner, NormalOvenChamber *chamber)
	: TFrame(Owner), Chamber(chamber)
{
	AutoStartFailed = false;

	IsShowRecipe = true;
	IsIdleStarted = false;
	GraphMode = GM_NORMAL;
	CurrentMaxRow = 1;

	IsCsvSaveComplete = true;

    CureStarting = false;

	Init();
	InitLotInfoGrid();
	for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++)
	{
		TempLoggerData[i] = 0.0;
		Series[i] = nullptr;
	}
	SPBgSerie = nullptr;

	// 20180321 - FTP (OFILM MES) - jhlim
	if( GetManager()->UseAutomation &&
		GetManager()->AutomationKind == OFILM_MES )
	{
		// Init
		Chamber->FTPLotInfo.TotalLotCount = 0;
		for(int j=0; j<FTP_LOT_COUNT; j++)
		{
			Chamber->FTPLotInfo.LotID[j] = "";
			Chamber->FTPLotInfo.DeviceType[j] = "";
		}
	}
	else if( GetManager()->UseAutomation &&
		GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		Chamber->CowellLotInfo.TotalLotCount = 0;
		for(int j=0; j < LOT_COUNT; j++)
		{
			Chamber->CowellLotInfo.LotID[j] = "";
			Chamber->CowellLotInfo.DeviceType[j] = "";
		}
	}
	//2021-07-27 hcw
	if( GetManager()->UseAutomation && GetManager()->AutomationKind == HANA_CIM)
	{
		CureStartButton->Visible = false;
	}

//	if (GetManager()->UseSafetyReset == true)  //2023-04-17 restore
//	{
//		SafetyRelayResetButton->Visible = false;
//		SafetyRelayResetButton->Width = MachineStatusPanel->Width/2;
//		LocalButton->Width = MachineStatusPanel->Width/2;
//	}
//	else
//	{
//		SafetyRelayResetButton->Visible = false;
		LocalButton->Width = MachineStatusPanel->Width;
//	}

	if( GetManager()->UseAutomation &&
		GetManager()->AutomationKind == SECS_GEM_SCK )
	{
		CimStatusPanel->Visible = true;
	}
	else
	{
		CimStatusPanel->Visible = false;
    }

	// 2021-12-01
	CureStartAlarmTimer->Enabled = false;
	EnableButtons();
}
//---------------------------------------------------------------------------
__fastcall TFrmChamber::~TFrmChamber()
{
	for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++)
	{
		if( Series[i] )
			delete Series[i];
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::Init()
{
	// 깜빡임 제거
	ChamberTempPanel->OptimizePaint = true;
	RemainTimePanel->OptimizePaint = true;
	DPPanel->OptimizePaint = true;
	O2Panel->OptimizePaint = true;
	ChamberStatusPanel->OptimizePaint = true;
	ChamberPanel->OptimizePaint = true;
	CommStatusPanel->DoubleBuffered = true;
	CommStatusPanel->ParentBackground = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::Localize()
{
	String str, prgName;

	if( GetManager()->ChamberCount == 4 )
	{
		if( GetManager()->UseAutomation == true &&
			GetManager()->AutomationKind == BARUN_MES &&
			Chamber->UseManualMode )
			str.sprintf(L"<P align=\"center\">CH. %c [M]</P>", Chamber->ChamberID + 'A');
		else
			str.sprintf(L"<P align=\"center\">CH. %c</P>", Chamber->ChamberID + 'A');
	}
	else
		str.sprintf(L"<P align=\"center\">CHAMBER %c</P>", Chamber->ChamberID + 'A');
	ChamberPanel->Caption->Text = str;

	CureStartButton->Caption = GetResStr("Form", "CureStart");
	CureStopButton->Caption = GetResStr("Form", "CureStop");
	SelectButton->Caption = GetResStr("Form", "Select");
	ShowAlarmButton->Caption = GetResStr("Form", "ShowAlarm");
	BuzzerOffButton->Caption = GetResStr("Form", "BuzzerOff");
	ChangeViewModeButton->Caption = GetResStr("Form", "ShowFull");

	str.sprintf(L"<P align=\"center\">%s</P>",
				GetResStr("Form", "RemainTime").c_str());
	RemainTimePanel->Caption->Text = str;

	str.sprintf(L"<P align=\"center\">%s</P>",
				GetResStr("Form", "ChamberTemp").c_str());
	ChamberTempPanel->Caption->Text = str;

	str.sprintf(L"<P align=\"center\">%s</P>",
				GetResStr("Form", "DP").c_str());
	DPPanel->Caption->Text = str;

	str.sprintf(L"<P align=\"center\">%s</P>",
				GetResStr("Form", "Recipe").c_str());
	RecipePanel->Caption->Text = str;

	str.sprintf(L"<P align=\"center\">%s</P>",
				GetResStr("Form", "ChamberStatus").c_str());
	ChamberStatusPanel->Caption->Text = str;

	RecipeInfoGrid->RowHeaders->Add(GetResStr("Recipe", "Recipe"));
	RecipeInfoGrid->RowHeaders->Add(GetResStr("Recipe", "PtnNo"));
	RecipeInfoGrid->RowHeaders->Add(GetResStr("Recipe", "Temp"));
	RecipeInfoGrid->RowHeaders->Add(GetResStr("Recipe", "Time"));
	RecipeInfoGrid->RowHeaders->Add(GetResStr("Recipe", "ID"));

    if (GetManager()->AutomationKind == SECS_GEM_SCK)
	{
		LotInfoButton->Caption = L"Validation";
	}
	else
	{
		LotInfoButton->Caption = L"Lot Info";
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::ArrangeFullMode()
{
	int height;
	int width;

	LotInfoGrid->ColWidths[0] = 25;
	LotInfoGrid->ColWidths[1] = 170;
//	LotInfoGrid->ColWidths[2] = 50;

	LotInfoButton->Width = MachineStatusPanel->Width;
	UserIDEdit->Width = MachineStatusPanel->Width - 85;
	LotInfoGrid->Width = LotInfoButton->Width-2;

	MachineStatusPanel->Left = ChamberPanel->Width - MachineStatusPanel->Width - 1;

	if( GetManager()->UseAutomation &&
		GetManager()->AutomationKind == SECS_GEM_SCK )
	{
		CimStatusPanel->Left =  MachineStatusPanel->Left;
		CimStatusPanel->Width = MachineStatusPanel->Width;
		CimStatusPanel->Top = ChamberPanel->Caption->Height - 2;
		CimStatusPanel->Height = 50;
		MachineStatusPanel->Top =  CimStatusPanel->Top + CimStatusPanel->Height + 1;
		MachineStatusPanel->Height = 80;
		PatternGraph->Top = MachineStatusPanel->Top + MachineStatusPanel->Height + 1;
		PatternGraph->Height = 120;
		LotInfoPanel->Height = 296;
		LotInfoGrid->Height = 269;
	}
	else
	{
		MachineStatusPanel->Top = ChamberPanel->Caption->Height - 2;
		MachineStatusPanel->Height = 114;
		PatternGraph->Top = MachineStatusPanel->Top + MachineStatusPanel->Height + 1;
		PatternGraph->Height = 149;
		LotInfoPanel->Height = 325;
		LotInfoGrid->Height = 298;
	}

	PatternGraph->Left = MachineStatusPanel->Left;

	RecipePanel->Left = MachineStatusPanel->Left;
	RecipePanel->Top = PatternGraph->Top + PatternGraph->Height + 1;
	LotInfoPanel->Left = MachineStatusPanel->Left;
	LotInfoPanel->Top = PatternGraph->Top;

	if( GetManager()->UseAutomation &&
		GetManager()->AutomationKind == BARUN_MES )
	{
		UserIDEdit->Visible = true;
		height = (Height - ChamberPanel->Caption->Height - MachineStatusPanel->Height - PatternGraph->Height - RecipePanel->Height - 1) / 5;
	}
	else
	{
		UserIDEdit->Visible = false;
		height = (Height - ChamberPanel->Caption->Height - MachineStatusPanel->Height - PatternGraph->Height - RecipePanel->Height - 1) / 4;
	}
	CureStartButton->Height = height - 1;

	if (GetManager()->UseAutomation && GetManager()->AutomationKind == HANA_CIM)
	{
		CureStopButton->Height = height + 60;
	}
	else
	{
		CureStopButton->Height = height - 1;
	}

	if( GetManager()->UseAutomation &&
		GetManager()->AutomationKind == SECS_GEM_SCK )
	{
		height = (Height - ChamberPanel->Caption->Height - CimStatusPanel->Height - MachineStatusPanel->Height - PatternGraph->Height - RecipePanel->Height - 1) / 4;
		CureStartButton->Height = height - 1;
		CureStopButton->Height = height - 1;
	}

	LotInfoButton->Height = height - 1;
	LocalButton->Height = height - 1;

	UserIDEdit->Height = height - 1;
//	SafetyRelayResetButton->Height = height - 1;   //2023-04-17 restore

	CureStartButton->Left = MachineStatusPanel->Left;
	CureStopButton->Left = MachineStatusPanel->Left;
	LotInfoButton->Left = MachineStatusPanel->Left;
	LocalButton->Left = MachineStatusPanel->Left;
//	SafetyRelayResetButton->Left = LocalButton->Left + MachineStatusPanel->Width/2;    //2023-04-17 restore
	UserIDEdit->Left = MachineStatusPanel->Left + 85;

	if( GetManager()->UseAutomation &&
		GetManager()->AutomationKind == BARUN_MES )
	{
		UserIDEdit->Top = RecipePanel->Top + RecipePanel->Height + 1;
		LotInfoButton->Top = UserIDEdit->Top + UserIDEdit->Height + 1;
	}
	else
		LotInfoButton->Top = RecipePanel->Top + RecipePanel->Height + 1;
	CureStartButton->Top = LotInfoButton->Top + LotInfoButton->Height + 1;

	if (GetManager()->UseAutomation && GetManager()->AutomationKind == HANA_CIM)
	{
		 CureStopButton->Top =  LotInfoButton->Top + LotInfoButton->Height + 1;
	}
	else
	{
		CureStopButton->Top = CureStartButton->Top + CureStartButton->Height + 1;
	}
	LocalButton->Top = CureStopButton->Top + CureStopButton->Height + 1;
//	SafetyRelayResetButton->Top = CureStopButton->Top + CureStopButton->Height + 1;   //2023-04-17 restore

	int divide = 3, left, top;
	if (GetManager()->DPType) divide++;
	if (GetManager()->UseO2Analyzer) divide++;

	width = (Width - MachineStatusPanel->Width - divide) / divide;
	ChamberStatusPanel->Width = width;
	RemainTimePanel->Width = width;
	if( GetManager()->DPType == false )
		ChamberTempPanel->Width = width;
	else
		ChamberTempPanel->Width = width;
	DPPanel->Width = width;
	O2Panel->Width = width;

	ChamberStatusPanel->Left = 1;
	ChamberStatusPanel->Top = ChamberPanel->Caption->Height - 2;
	RemainTimePanel->Left = ChamberStatusPanel->Left + ChamberStatusPanel->Width + 1;
	RemainTimePanel->Top = ChamberStatusPanel->Top;
	ChamberTempPanel->Left = RemainTimePanel->Left + RemainTimePanel->Width + 1;
	ChamberTempPanel->Top = RemainTimePanel->Top;

	TempGraph->Width = Width - MachineStatusPanel->Width - 3;
	TempGraph->Height = Height - ChamberPanel->Caption->Height - ChamberStatusPanel->Height;
	TempGraph->Left = ChamberStatusPanel->Left;
	TempGraph->Top = ChamberPanel->Caption->Height + ChamberStatusPanel->Height - 1;

	left = ChamberTempPanel->Left + ChamberTempPanel->Width + 1;
	top = ChamberTempPanel->Top;
	if (GetManager()->DPType)
	{
		DPPanel->Left = left;
		DPPanel->Top = top;

		if (GetManager()->UseO2Analyzer == false)
		{
			if (left + width > TempGraph->Left + TempGraph->Width)
			{
				DPPanel->Width -= left + width - (TempGraph->Left + TempGraph->Width);
			}
			else if (left + width < TempGraph->Left + TempGraph->Width)
			{
				DPPanel->Width += TempGraph->Left + TempGraph->Width - (left + width);
			}
		}

		left = DPPanel->Left + DPPanel->Width + 1;
	}

	if (GetManager()->UseO2Analyzer)
	{
		O2Panel->Left = left;
		O2Panel->Top = top;
		if (left + width > TempGraph->Left + TempGraph->Width)
		{
			O2Panel->Width -= left + width - (TempGraph->Left + TempGraph->Width);
		}
		else if (left + width < TempGraph->Left + TempGraph->Width)
		{
			O2Panel->Width += TempGraph->Left + TempGraph->Width - (left + width);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::AutoArrange()
{
	if( Chamber == nullptr )
		return;

	if (GetManager()->ChamberCount != 4)
	{
		DPPanel->Visible = GetManager()->DPType;
		O2Panel->Visible = GetManager()->UseO2Analyzer;
	}
	else
	{
		if (FmJob->ShowDetailMode)
		{
			DPPanel->Visible = GetManager()->DPType;
			O2Panel->Visible = GetManager()->UseO2Analyzer;
		}
		else
		{
			DPPanel->Visible = false;
			O2Panel->Visible = false;
		}
	}

	ChamberPanel->Left = 0;
	ChamberPanel->Top = 0;
	ChamberPanel->Width = Width;
	ChamberPanel->Height = Height;

	ArrangeCommIndicator();

	BuzzerOffButton->Left = Width - BuzzerOffButton->Width - 2;
	ShowAlarmButton->Left = BuzzerOffButton->Left - ShowAlarmButton->Width - 2;

	PatternGraph->Width = MachineStatusPanel->Width;
	RecipePanel->Width = MachineStatusPanel->Width;
	LotInfoPanel->Width = MachineStatusPanel->Width;
	CureStartButton->Width = MachineStatusPanel->Width;
	CureStopButton->Width = MachineStatusPanel->Width;

//	if (GetManager()->UseSafetyReset == true)   //2023-04-17 restore
//	{
//		if (Chamber->DIO[I_SAFETY_STATUS_ALARM_DETECTED]->IsOn())
//		{
//			LocalButton->Width = MachineStatusPanel->Width/2;
//		}
//	}
//	else
	if( GetManager()->UseAutomation &&
		(GetManager()->AutomationKind == SECS_GEM_SCK ||
		GetManager()->AutomationKind == SECS_GEM_COWELL))
	{
		LocalButton->Visible = false;
	}
	else
	{
		LocalButton->Visible = true;
	}

	LocalButton->Width = MachineStatusPanel->Width;

	int height;
	int width;
	int divide, left, top;
	int chamberCount = GetManager()->ChamberCount;
	switch( chamberCount )
	{
	case 1:
		ChangeViewModeButton->Visible = false;
		ArrangeFullMode();
		break;

	case 2:
		ChangeViewModeButton->Visible = false;
		LotInfoGrid->ColWidths[0] = 25;
		LotInfoGrid->ColWidths[1] = 170;
//		LotInfoGrid->ColWidths[2] = 50;

		if( GetManager()->UseAutomation &&
			GetManager()->AutomationKind == BARUN_MES )
			UserIDEdit->Visible = true;
		else
			UserIDEdit->Visible = false;

		LotInfoButton->Width = PatternGraph->Width;
		UserIDEdit->Width = LotInfoButton->Width - 85;
		LotInfoGrid->Width = LotInfoButton->Width-2;

		MachineStatusPanel->Left = ChamberPanel->Width - MachineStatusPanel->Width - 1;

		PatternGraph->Left = MachineStatusPanel->Left - MachineStatusPanel->Width - 1;
 		PatternGraph->Top = ChamberPanel->Caption->Height - 2;//MachineStatusPanel->Top;
		RecipePanel->Left = PatternGraph->Left;
		RecipePanel->Top = PatternGraph->Top + PatternGraph->Height + 1;
		LotInfoPanel->Left = PatternGraph->Left;
		LotInfoPanel->Top = PatternGraph->Top;

		if( GetManager()->UseAutomation &&
			GetManager()->AutomationKind == BARUN_MES )
			height = (Height - ChamberPanel->Caption->Height - MachineStatusPanel->Height) / 5;
		else
		{
			if( GetManager()->UseAutomation &&
				GetManager()->AutomationKind == SECS_GEM_SCK )
			{
				CimStatusPanel->Left =  MachineStatusPanel->Left;
				CimStatusPanel->Width = MachineStatusPanel->Width;
				CimStatusPanel->Top = ChamberPanel->Caption->Height - 2;
				CimStatusPanel->Height = 50;
				MachineStatusPanel->Top =  CimStatusPanel->Top + CimStatusPanel->Height + 1;
				MachineStatusPanel->Height = 80;
				height = (Height - CimStatusPanel->Height - ChamberPanel->Caption->Height - MachineStatusPanel->Height) / 4;
			}
			else
			{
				CimStatusPanel->Visible = false;
				MachineStatusPanel->Top = ChamberPanel->Caption->Height - 2;
				MachineStatusPanel->Height = 114;
				height = (Height - ChamberPanel->Caption->Height - MachineStatusPanel->Height) / 4;
			}
		}
		CureStartButton->Height = height - 1;
		CureStopButton->Height = height - 1;
		LotInfoButton->Height = height - 1;
		LocalButton->Height = height - 1;

		UserIDEdit->Height = height - 1;
//		SafetyRelayResetButton->Height = height - 1;   //2023-04-17 restore

		CureStartButton->Left = MachineStatusPanel->Left;
		CureStopButton->Left = MachineStatusPanel->Left;
		LocalButton->Left = MachineStatusPanel->Left;
		LotInfoButton->Left = MachineStatusPanel->Left;
		UserIDEdit->Left = MachineStatusPanel->Left + 85;
//		SafetyRelayResetButton->Left = LocalButton->Left + MachineStatusPanel->Width/2;    //2023-04-17 restore

		if( GetManager()->UseAutomation &&
			GetManager()->AutomationKind == BARUN_MES )
		{
			UserIDEdit->Top = MachineStatusPanel->Top + MachineStatusPanel->Height + 2;
			LotInfoButton->Top = UserIDEdit->Top + UserIDEdit->Height + 1;
		}
		else
			LotInfoButton->Top = MachineStatusPanel->Top + MachineStatusPanel->Height + 2;
		CureStartButton->Top = LotInfoButton->Top + LotInfoButton->Height + 1;
		CureStopButton->Top = CureStartButton->Top + CureStartButton->Height + 1;
		LocalButton->Top = CureStopButton->Top + CureStopButton->Height + 1;
//		SafetyRelayResetButton->Top = CureStopButton->Top + CureStopButton->Height + 1;   //2023-04-17 restore

		divide = 3;
		if (GetManager()->DPType) divide++;
		if (GetManager()->UseO2Analyzer) divide++;

		width = (Width - MachineStatusPanel->Width - RecipePanel->Width - divide) / divide;
		ChamberStatusPanel->Width = width;
		RemainTimePanel->Width = width;
/*		if( GetManager()->DPType )
			ChamberTempPanel->Width = width + 30;
		else
			ChamberTempPanel->Width = width - 1;
		DPPanel->Width = width - 30 - 3;
		O2Panel->Width = width - 30 - 3;*/

		ChamberTempPanel->Width = width;
		DPPanel->Width = width;
		O2Panel->Width = width;

		ChamberStatusPanel->Left = 1;
		ChamberStatusPanel->Top = ChamberPanel->Caption->Height - 2;
		RemainTimePanel->Left = ChamberStatusPanel->Left + ChamberStatusPanel->Width + 1;
		RemainTimePanel->Top = ChamberStatusPanel->Top;
		ChamberTempPanel->Left = RemainTimePanel->Left + RemainTimePanel->Width + 1;
		ChamberTempPanel->Top = RemainTimePanel->Top;

		left = ChamberTempPanel->Left + ChamberTempPanel->Width + 1;
		top = ChamberTempPanel->Top;

		if (GetManager()->DPType)
		{
			DPPanel->Left = left;
			DPPanel->Top = top;

			left = DPPanel->Left + DPPanel->Width + 1;
		}

		if (GetManager()->UseO2Analyzer)
		{
			O2Panel->Left = left;
			O2Panel->Top = top;
        }

		TempGraph->Width = Width - RecipePanel->Width - MachineStatusPanel->Width - 4;
		TempGraph->Height = Height - ChamberPanel->Caption->Height - ChamberStatusPanel->Height;
		TempGraph->Left = ChamberStatusPanel->Left;
		TempGraph->Top = ChamberPanel->Caption->Height + ChamberStatusPanel->Height - 1;
		break;

	case 4:
		ChangeViewModeButton->Visible = true;
		if( FmJob->ShowDetailMode )
		{
			CommStatusPanel->Visible = true;
			LotInfoPanel->Visible = true;
			PatternGraph->Visible = true;
			RecipePanel->Visible = true;
			MachineStatusPanel->Visible = true;
			LotInfoButton->Visible = true;
			CureStartButton->Visible = true;
			CureStopButton->Visible = true;

			if( GetManager()->UseAutomation &&
				GetManager()->AutomationKind == BARUN_MES )
				UserIDEdit->Visible = true;
			else
				UserIDEdit->Visible = false;

			if( GetManager()->UseAutomation &&
				GetManager()->AutomationKind == SECS_GEM_SCK )
			{
				CimStatusPanel->Visible = true;
			}
			else
				CimStatusPanel->Visible = false;

			ArrangeFullMode();

			ChangeViewModeButton->Caption = GetResStr("Form", "ShowAll");
			ChangeViewModeButton->Width = 70;
			ChangeViewModeButton->Height = CommStatusPanel->Height;
			ChangeViewModeButton->Left = CommStatusPanel->Left + CommStatusPanel->Width + 1;
			ChangeViewModeButton->Top = CommStatusPanel->Top;
		}
		else
		{
			CommStatusPanel->Visible = false;
			LotInfoPanel->Visible = false;
			PatternGraph->Visible = false;
			RecipePanel->Visible = false;
			MachineStatusPanel->Visible = false;
			LotInfoButton->Visible = false;
			UserIDEdit->Visible = false;
			CureStartButton->Visible = false;
			CureStopButton->Visible = false;
			LocalButton->Visible = false;
			DPPanel->Visible = false;
			O2Panel->Visible = false;
			CimStatusPanel->Visible = false;

			ChamberStatusPanel->Width = 145;
			ChamberStatusPanel->Height = 76;
			ChamberStatusPanel->Left = 1;
			ChamberStatusPanel->Top = ChamberPanel->Caption->Height - 2;
			RemainTimePanel->Width = 145;
			RemainTimePanel->Height = 76;
			RemainTimePanel->Left = ChamberStatusPanel->Left + ChamberStatusPanel->Width + 1;
			RemainTimePanel->Top = ChamberStatusPanel->Top;
			ChamberTempPanel->Width = 175;
			ChamberTempPanel->Height = 76;
			ChamberTempPanel->Left = RemainTimePanel->Left + RemainTimePanel->Width + 1;
			ChamberTempPanel->Top = RemainTimePanel->Top;

			ChangeViewModeButton->Caption = GetResStr("Form", "ShowFull");
			ChangeViewModeButton->Width = Width - ChamberStatusPanel->Width - RemainTimePanel->Width - ChamberTempPanel->Width;
			ChangeViewModeButton->Height = ChamberStatusPanel->Height;
			ChangeViewModeButton->Left = ChamberTempPanel->Left + ChamberTempPanel->Width + 1;
			ChangeViewModeButton->Top = ChamberTempPanel->Top;

			TempGraph->Width = Width;
			TempGraph->Height = Height - ChamberPanel->Caption->Height - ChamberStatusPanel->Height;
			TempGraph->Left = ChamberStatusPanel->Left;
			TempGraph->Top = ChamberPanel->Caption->Height + ChamberStatusPanel->Height - 1;
		}
		break;
	}
	ShowLotInfo();

	if( GetManager()->UseAutomation &&
		GetManager()->AutomationKind == FUJITSU_TCARD )
	{
		if (GetManager()->LoginLevel <= LV_OPERATOR)
		{
			CureStartButton->Enabled = false;
			LocalButton->Enabled = false;
		}
		else
		{
			CureStartButton->Enabled = true;
			LocalButton->Enabled = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::ArrangeCommIndicator()
{
#define INDICATOR_WIDTH	50
#define INDCATOR_NUM	7
	TAdvSmoothStatusIndicator *indicator[INDCATOR_NUM] = { nullptr, };

	int idx = 0;

	MainTempIndicator->Visible = true;
	indicator[idx++] = MainTempIndicator;
	if (Chamber->LimitControllerType != LIMIT_NONE)
	{
		TempLimitIndicator->Visible = true;
		indicator[idx++] = TempLimitIndicator;
	}
	else
	{
		TempLimitIndicator->Visible = false;
    }
	LoggerIndicator->Visible = true;
	indicator[idx++] = LoggerIndicator;
	DIOIndicator->Visible = true;
	indicator[idx++] = DIOIndicator;
	if( GetManager()->DamperControlType != CYLINDER_DAMPER )
	{
		DamperMotorIndicator->Visible = true;
		indicator[idx++] = DamperMotorIndicator;
	}
	if( GetManager()->DPType )
	{
		DPIndicator->Visible = true;
		indicator[idx++] = DPIndicator;
	}
	if( GetManager()->UseO2Analyzer )
	{
		O2AnalyzerIndicator->Visible = true;
		indicator[idx++] = O2AnalyzerIndicator;
	}

	CommStatusPanel->Width = INDICATOR_WIDTH*idx + 2;
	for(int i=0; i<idx; i++)
	{
		indicator[i]->Width = INDICATOR_WIDTH;
		if( i==0 )
			indicator[i]->Left = 1;
		else
			indicator[i]->Left = indicator[i-1]->Left + INDICATOR_WIDTH;
		indicator[i]->Top = (CommStatusPanel->Height - indicator[i]->Height)/2;
	}

	CommStatusPanel->Left = 1;
	CommStatusPanel->Top = 1;
	ProgressBar->Width = CommStatusPanel->Width - 4;
	ProgressBar->Height = CommStatusPanel->Height - 4;
	ProgressBar->Left = 2;
	ProgressBar->Top = 2;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::UpdateRecipeComboBox()
{
	if( Chamber == nullptr )
		return;

	String recipe;

	int sel=-1;
	RecipeComboBox->Clear();
	if( GetOvenDB()->QueryRecipe(Chamber->ChamberID) )
	{
		for(int j=0; j<GetOvenDB()->Query->RecordCount; j++)
		{
			recipe = GetOvenDB()->Query->FieldByName("Recipe")->AsString;
			if( recipe == Chamber->Recipe.RecipeName )
				sel = j;
			RecipeComboBox->Items->Add(recipe);
			GetOvenDB()->Query->Next();
		}
	}
	RecipeComboBox->ItemIndex = sel;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::FillUpInfo()
{
	if (Chamber == nullptr)
	{
		return;
    }

	OVEN_WORK work;
	LOTINFO lotInfo;
	String recipe;
	int chamberNo = Chamber->ChamberID;

	UpdateRecipeComboBox();	// 2022-01-17 위치 이동

	if (Chamber->LoadCurrentState())
	// 현재 작업 상태가 저장된 파일이 존재 한다는 것은
	// 작업이 진행 중이라는 의미
	{
//		UpdateRecipeComboBox();
		if (GetOvenDB()->GetWorkNotEnd(chamberNo, work, lotInfo))
		// 작업은 시작했지만 종료되지 않은 작업 로딩
		{
			Chamber->Recipe.StartTime = work.StartTime;
			Chamber->Recipe.RecipeName = work.Recipe;
			recipe = work.Recipe;

			if( Chamber->Recipe.IsGemRecipe || (GetManager()->AutomationKind == HANA_CIM
			&& GetManager()->UseAutomation == true))  //2021-04-15 HANA OVEN
				LoadRemoteTempPtn(recipe, Chamber->Recipe.TempPtn);
			else
				LoadTempPtn(chamberNo, work.PtnNo, Chamber->Recipe.TempPtn);
			Chamber->ApplyDataToController();

			OVEN_RECIPE recipeInfo;
			if( GetOvenDB()->GetRecipe(chamberNo, recipe, recipeInfo) )
				FillRecipeGrid(recipeInfo);
			FillLotInfoGrid(lotInfo);
			Chamber->MakeDataFilename();

			InitChart(work.StartTime, work.CureTime, work.CureTemp);
			LoadMainTempData(work.StartTime);
			LoadLoggerData(work.StartTime);
			if( GetManager()->UseO2Analyzer )
				LoadOxygenData(work.StartTime);
			InitPtnChart();

			for(int i=0; i<RecipeComboBox->Items->Count; i++)
			{
				if( RecipeComboBox->Items->Strings[i] == work.Recipe )
				{
					RecipeComboBox->ItemIndex = i;
					break;
				}
			}
		}
		else
		{
			if( RecipeComboBox->ItemIndex > -1 )
			// 작업은 종료되었지만 문을 열지 않아 완전종료가 안된 상태
			{
				if( GetOvenDB()->GetWorkEnd(chamberNo, work) )
				{
					Chamber->Recipe.StartTime = work.StartTime;
					Chamber->Recipe.RecipeName = work.Recipe;
					recipe = work.Recipe;

					if( Chamber->Recipe.IsGemRecipe )
						LoadRemoteTempPtn(recipe, Chamber->Recipe.TempPtn);
					else
						LoadTempPtn(Chamber->ChamberID, work.PtnNo, Chamber->Recipe.TempPtn);
					Chamber->ApplyDataToController();

					OVEN_RECIPE recipeInfo;
					if( GetOvenDB()->GetRecipe(chamberNo, recipe, recipeInfo) )
						FillRecipeGrid(recipeInfo);

					Chamber->MakeDataFilename();

					InitChart(work.StartTime, work.CureTime, work.CureTemp);
					LoadMainTempData(work.StartTime);
					LoadLoggerData(work.StartTime);
					if( GetManager()->UseO2Analyzer )
						LoadOxygenData(work.StartTime);
					InitPtnChart();
				}
				else
				{
					InitChart(Now(), 180, 500);
					RecipeComboBox->ItemIndex = -1;
				}
			}
			else
			{
				InitChart(Now(), 180, 500);
				RecipeComboBox->ItemIndex = -1;
			}
		}
	}
	else
	{
		//InitChart(Now(), 180, 500);
		RecipeComboBox->ItemIndex = -1;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::FillRecipeGrid(OVEN_RECIPE rec)
{
	RecipeInfoGrid->Cells[1][0] = rec.Recipe;
	RecipeInfoGrid->Cells[1][1] = rec.PtnNo;
	RecipeInfoGrid->Cells[1][2] = rec.CureTemp;
	RecipeInfoGrid->Cells[1][3] = rec.CureTime;
	RecipeInfoGrid->Cells[1][4] = GetManager()->UserID;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::FillRecipeGrid(PTN ptn, String name)
{
	RecipeInfoGrid->Cells[1][0] = name;
	RecipeInfoGrid->Cells[1][1] = ptn.PtnNo;
	RecipeInfoGrid->Cells[1][2] = ptn.MaxTemp;
	RecipeInfoGrid->Cells[1][3] = ptn.TotalTime;
	RecipeInfoGrid->Cells[1][4] = GetManager()->UserID;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::InitChart(TDateTime dtStart, int cureTime, int maxTemp)
{
	if( Chamber == nullptr )
		return;

	String str;
	int ch = 0;

	for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++)
	{
		if( Series[i] )
		{
			delete Series[i];
			Series[i] = nullptr;
		}
	}

	for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++)
	{
		if( Chamber->UseChannel[i] )
		{
			Series[ch] = new TPointSeries(TempGraph);
			Series[ch]->XValues->DateTime = true;
			Series[ch]->ParentChart = TempGraph;
			Series[ch]->HorizAxis = aBottomAxis;
			Series[ch]->VertAxis = aLeftAxis;
			Series[ch]->Pointer->Style = psCircle;
			Series[ch]->Pointer->Color = TColor(Colors[ch]);
			Series[ch]->Color = TColor(Colors[ch]);
			Series[ch]->Pointer->Size = 2;
			Series[ch]->Pointer->Frame->Visible = false;
			Series[ch]->Visible = true;
			str.sprintf(L"Ch. %d", ch+1);
			Series[ch]->LegendTitle = str;
			Series[ch]->Title = str;
			Series[ch]->Visible = true;
			ch++;
		}
	}

	if( GetManager()->UseO2Analyzer )
	{
		Series[ch] = new TPointSeries(TempGraph);
		Series[ch]->XValues->DateTime = true;
		Series[ch]->ParentChart = TempGraph;
		Series[ch]->HorizAxis = aBottomAxis;
		Series[ch]->VertAxis = aRightAxis;
		Series[ch]->Pointer->Style = psCircle;
		Series[ch]->Pointer->Color = TColor(Colors[MAX_COLOR_COUNT-2]);
		Series[ch]->Color = TColor(Colors[MAX_COLOR_COUNT-2]);
		Series[ch]->Pointer->Size = 2;
		Series[ch]->Pointer->Frame->Visible = false;
		Series[ch]->LegendTitle = "O2";
		Series[ch]->Title = "O2";
		Series[ch]->Visible = true;
		ch++;
	}

	Series[ch] = new TPointSeries(TempGraph);
	Series[ch]->XValues->DateTime = true;
	Series[ch]->ParentChart = TempGraph;
	Series[ch]->HorizAxis = aBottomAxis;
	Series[ch]->VertAxis = aLeftAxis;
	Series[ch]->Pointer->Style = psCircle;
	Series[ch]->Pointer->Color = TColor(Colors[MAX_COLOR_COUNT-1]);
	Series[ch]->Color = TColor(Colors[MAX_COLOR_COUNT-1]);
	Series[ch]->Pointer->Size = 2;
	Series[ch]->Pointer->Frame->Visible = false;
	Series[ch]->Visible = true;
	Series[ch]->LegendTitle = "Main Temp.";
	Series[ch]->Title = "Main Temp.";
	Series[ch]->Visible = true;

	double dInc = TDateTime(0, 1, 0, 0); 	// 단위 1분
	int step;

	if( cureTime<60 )
		step = 1;
	else
	if( cureTime<360 )
		step = 10;
	else
		step = 15;
	TempGraph->BottomAxis->DateTimeFormat = "hh:mm";
	TempGraph->BottomAxis->Automatic = false;
	TempGraph->BottomAxis->AutomaticMaximum = false;
	TempGraph->BottomAxis->AutomaticMinimum = false;
	TempGraph->BottomAxis->Increment = dInc*step;
	TempGraph->BottomAxis->SetMinMax(0, dInc*(cureTime+10));

	TempGraph->LeftAxis->Automatic = false;
	TempGraph->LeftAxis->AutomaticMaximum = false;
	TempGraph->LeftAxis->AutomaticMinimum = false;
	TempGraph->LeftAxis->Increment = 50;
	TempGraph->LeftAxis->SetMinMax(0, maxTemp + 70);

	if( GetManager()->UseO2Analyzer )
	{
		TempGraph->RightAxis->Automatic = false;
		TempGraph->RightAxis->AutomaticMaximum = false;
		TempGraph->RightAxis->AutomaticMinimum = false;
		TempGraph->RightAxis->LabelsAngle = 0;
		TempGraph->RightAxis->Increment = 10;
		TempGraph->RightAxis->Title->Caption = "O2 (ppm)";
		TempGraph->RightAxis->Logarithmic = false;
		TempGraph->RightAxis->LabelsSize = 40;
		TempGraph->RightAxis->LabelsSeparation = 10;
		TempGraph->RightAxis->SetMinMax(0, 200);
	}

	TempGraph->Legend->Visible = true;
	TempGraph->Legend->CheckBoxes = true;

	if (GetManager()->DrawBgSP)
	{
		if (SPBgSerie)
		{
			delete SPBgSerie;
		}
		SPBgSerie = new TLineSeries(TempGraph);
		SPBgSerie->Title = "SP";
		SPBgSerie->LegendTitle = "SP";
		SPBgSerie->XValues->DateTime = true;
		SPBgSerie->ParentChart = TempGraph;
		SPBgSerie->HorizAxis = aBottomAxis;
		SPBgSerie->VertAxis  = aLeftAxis;
		SPBgSerie->Pointer->Style = psCircle;
		SPBgSerie->Pointer->Color = clMaroon;
		SPBgSerie->Color = clMaroon;
		SPBgSerie->Pointer->Size  = 4;
		SPBgSerie->Pointer->Frame->Visible = false;
		SPBgSerie->Visible = true;

		int cnt = Chamber->Recipe.TempPtn.Stc.SegCount+1;
		int time = 0, accTime = 0;
		float temp = 0;
		TDateTime x = 0;

		SPBgSerie->AddXY(x, Chamber->Recipe.TempPtn.Stc.Ssp);
		for (int i = 1; i <= cnt; i++)
		{
			time = Chamber->Recipe.TempPtn.Seg[i-1].Time;
			temp = Chamber->Recipe.TempPtn.Seg[i-1].Tsp;
			if (time > 0 && temp > -1.0)
			{
				accTime += time;
				x = (double)accTime / 1440.0;
				SPBgSerie->AddXY(x, temp);
			}
			else
			{
				break;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::LoadMainTempData(TDateTime start)
{
	if( Chamber == nullptr )
		return;

	AnsiString file = Chamber->Recipe.MainTempFilename;

	if( FileExists(file) == false )
	{
		SetMessageStr(GetResStr("Warning", "NoFileSelectedDate"));
		ShowMessageDlg();
		return;
	}

	std::auto_ptr<TStringList> lineList(new TStringList());
	std::auto_ptr<TStringList> dataList(new TStringList());

	lineList->LoadFromFile(file);

	String str;
	TDateTime prev, date, time, xtime;
	int cnt = TempGraph->SeriesCount();
	int ch = cnt - 1;
	for(int i=0; i<lineList->Count; i++)
	{
		dataList->CommaText = lineList->Strings[i];

		if( dataList->Count != 4 )
			continue;
		else
		{
			str = dataList->Strings[0];
			date = StrToDate(start.DateString(), GetDateTimeFormat());
			time = StrToTime(str);

			// 날짜가 바뀌는지 체크
			if( i != 0 )
			{
				unsigned short hour, min, sec, mil;
				time.DecodeTime(&hour, &min, &sec, &mil);
				if( time.Val - prev.Val < 0.0 &&
					hour == 0 && min == 0 )
					date = IncDay(date);
			}
			xtime = date + time - start;
			if( Series[ch] )
				Series[ch]->AddXY(xtime, dataList->Strings[1].ToDouble());

			prev = time;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::LoadLoggerData(TDateTime start)
{
	if( Chamber == nullptr )
		return;

	AnsiString file = Chamber->Recipe.LoggerFilename;

	if( FileExists(file) == false )
	{
		SetMessageStr(GetResStr("Warning", "NoFileSelectedDate"));
		ShowMessageDlg();
		return;
	}

	std::auto_ptr<TStringList> lineList(new TStringList());
	std::auto_ptr<TStringList> dataList(new TStringList());

	lineList->LoadFromFile(file);

	String str;
	TDateTime prev, date, time, xtime;
	for(int i=0; i<lineList->Count; i++)
	{
		dataList->CommaText = lineList->Strings[i];

		if( dataList->Count != MAX_LOGGERPOINT_COUNT+1 )
			continue;
		else
		{
			str = dataList->Strings[0];
			date = StrToDate(start.DateString(), GetDateTimeFormat());
			time = StrToTime(str);

			// 날짜가 바뀌는지 체크
			if( i != 0 )
			{
				unsigned short hour, min, sec, mil;
				time.DecodeTime(&hour, &min, &sec, &mil);
				if( time.Val - prev.Val < 0.0 &&
					hour == 0 && min == 0 )
					date = IncDay(date);
			}
			xtime = date + time - start;

			int ch = 0;
			for(int j=1; j<=MAX_LOGGERPOINT_COUNT; j++)
			{
				if( dataList->Strings[j] != "---" &&
					Chamber->UseChannel[j-1] == true)
				{
					if( Series[ch] )
					{
						Series[ch]->AddXY(xtime, dataList->Strings[j].ToDouble());
						ch++;
					}
				}
			}

			prev = time;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::LoadOxygenData(TDateTime start)
{
	if( Chamber == nullptr )
		return;

	AnsiString file = Chamber->Recipe.O2Filename;

	if( FileExists(file) == false )
	{
		SetMessageStr(GetResStr("Warning", "NoFileSelectedDate"));
		ShowMessageDlg();
		return;
	}

	std::auto_ptr<TStringList> lineList(new TStringList());
	std::auto_ptr<TStringList> dataList(new TStringList());

	lineList->LoadFromFile(file);

	String str;
	TDateTime prev, date, time, xtime;
	int cnt = TempGraph->SeriesCount();
	int ch = cnt - 2;
	for(int i=0; i<lineList->Count; i++)
	{
		dataList->CommaText = lineList->Strings[i];

		if( dataList->Count != 2 )
			continue;
		else
		{
			str = dataList->Strings[0];
			date = StrToDate(start.DateString(), GetDateTimeFormat());
			time = StrToTime(str);

			// 날짜가 바뀌는지 체크
			if( i != 0 )
			{
				unsigned short hour, min, sec, mil;
				time.DecodeTime(&hour, &min, &sec, &mil);
				if( time.Val - prev.Val < 0.0 &&
					hour == 0 && min == 0 )
					date = IncDay(date);
			}
			xtime = date + time - start;
			if( Series[ch] )
				Series[ch]->AddXY(xtime, dataList->Strings[1].ToDouble());

			prev = time;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::InitPtnChart()
{
	if( Chamber == nullptr )
		return;

	PatternGraph->LeftAxis->Automatic = false;
	PatternGraph->LeftAxis->AutomaticMaximum = false;
	PatternGraph->LeftAxis->AutomaticMinimum = false;
	PatternGraph->LeftAxis->SetMinMax(0, Chamber->Recipe.TempPtn.MaxTemp * 1.2);
	double deltaY = PatternGraph->LeftAxis->Maximum - PatternGraph->LeftAxis->Minimum;
	PatternGraph->LeftAxis->Increment = (int)(deltaY/6);

	PatternGraph->BottomAxis->Automatic = true;
	PatternGraph->BottomAxis->AutomaticMaximum = false;
	PatternGraph->BottomAxis->AutomaticMinimum = false;
	PatternGraph->BottomAxis->MinorTickCount = 0;
	PatternGraph->BottomAxis->Grid->Visible = true;
	PatternGraph->BottomAxis->Increment = 1;
	PatternGraph->BottomAxis->SetMinMax(0, Chamber->Recipe.TempPtn.Stc.SegCount);

	PatternGraph->Series[0]->Clear();
	PatternGraph->Series[0]->AddXY(0, Chamber->Recipe.TempPtn.Stc.Ssp);

	int cnt = Chamber->Recipe.TempPtn.Stc.SegCount+1;
	for(int i=1; i<=cnt; i++)
	{
		if( Chamber->Recipe.TempPtn.Seg[i-1].Time > 0)
		{
			String time;
			float temp = Chamber->Recipe.TempPtn.Seg[i-1].Tsp;

			time.sprintf(L"%dm", Chamber->Recipe.TempPtn.Seg[i-1].Time);
			if( temp > -1.0 )
				PatternGraph->Series[0]->AddXY(i, temp, time);
			else
				break;
		}
		else
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::RefreshCommIndicator(bool blink)
{
	if( Chamber == nullptr )
		return;

	TColor color, clrSuccess = clNavy, clrFail = clRed;
	clrFail = blink ? clNavy : clRed;

	if( Chamber->MainTempController )
		color = Chamber->MainTempController->IsFail ? clrFail : clrSuccess;
	else
		color = clrFail;
	if( MainTempIndicator->Appearance->Fill->Color != color )
		MainTempIndicator->Appearance->Fill->Color = color;

	if (Chamber->LimitControllerType != LIMIT_NONE)
	{
		if( Chamber->ChamberTempLimitController )
			color = Chamber->ChamberTempLimitController->IsFail ? clrFail : clrSuccess;
		else
			color = clrFail;
		if( TempLimitIndicator->Appearance->Fill->Color != color )
			TempLimitIndicator->Appearance->Fill->Color = color;
	}

	if( Chamber->LoggerController[0] )
		color = Chamber->LoggerController[0]->IsFail ? clrFail : clrSuccess;
	else
		color = clrFail;
	if( LoggerIndicator->Appearance->Fill->Color != color )
		LoggerIndicator->Appearance->Fill->Color = color;

	if( Chamber->DPController )
		color = Chamber->DPController->IsFail ? clrFail : clrSuccess;
	else
		color = clrFail;
	if( DPIndicator->Appearance->Fill->Color != color )
		DPIndicator->Appearance->Fill->Color = color;

	if( Chamber->DamperMotor )
		color = Chamber->DamperMotor->IsFail ? clrFail : clrSuccess;
	else
		color = clrFail;
	if( DamperMotorIndicator->Appearance->Fill->Color != color )
		DamperMotorIndicator->Appearance->Fill->Color = color;

	if( Chamber->O2Analyzer )
		color = Chamber->O2Analyzer->IsFail ? clrFail : clrSuccess;
	else
		color = clrFail;
	if( O2AnalyzerIndicator->Appearance->Fill->Color != color )
		O2AnalyzerIndicator->Appearance->Fill->Color = color;

	if (GetManager()->DIOType == ICPDAS)
	{
		if( Chamber->ModuleDI &&
			Chamber->ModuleDO &&
			Chamber->ModuleDIO )
			color = Chamber->ModuleDI->IsFail || Chamber->ModuleDO->IsFail || Chamber->ModuleDIO->IsFail ? clrFail : clrSuccess;
		else
			color = clrFail;
	}
	else if (GetManager()->DIOType == OVEN_DIO)
	{
		if( Chamber->OvenBoard )
			color = Chamber->OvenBoard->IsFail ? clrFail : clrSuccess;
		else
			color = clrFail;
    }
	if( DIOIndicator->Appearance->Fill->Color != color )
		DIOIndicator->Appearance->Fill->Color = color;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::RefreshStatusIndicator()
{
	if( Chamber == nullptr )
		return;

	TColor clr;
	String str;

	if( Chamber->IsDoorClosed() )
	{
		clr = clNavy;
		str = "Door CLOSED";
	}
	else
	{
		clr = clRed;
		str = "Door OPENED";
	}

	if( GetManager()->UseAutomation )
	{
		if( GetManager()->IsUseGem() && GemDlg &&
			GemDlg->IsGemConnected() )
		{
			GemDlg->SendDoorEvent(Chamber->ChamberID);
		}
		else if (GetManager()->AutomationKind == SECS_GEM_SCK && EzNetDlg[Chamber->ChamberID] != nullptr &&
			EzNetDlg[Chamber->ChamberID]->IsGemConnected())
		{
 //			EzNetDlg[Chamber->ChamberID]->SendDoorEvent();   real-time 1 second interval
		}
	}

	if( DoorStatus->Color != clr )
		DoorStatus->Color = clr;
	if( DoorStatus->EditLabel->Caption != str )
		DoorStatus->EditLabel->Caption = str;

	if( GetManager()->UseO2Analyzer == true )
	{
		if( Chamber->O2Analyzer &&
			Chamber->O2Analyzer->IsValid )
		{
			clr = clYellow;
//			str.sprintf(L"O2 Analyzer READY [FR=%.1f l/h]", Chamber->O2Analyzer->FlowRate);
			str = "O2 Analyzer READY";
		}
		else
		{
			clr = clSilver;
			str = "O2 Analyzer NOT READY";
		}

		if( O2AnalyzerStatus->Visible != true )
			O2AnalyzerStatus->Visible = true;
		if( O2AnalyzerStatus->Color != clr )
			O2AnalyzerStatus->Color = clr;
		if( O2AnalyzerStatus->EditLabel->Caption != str )
			O2AnalyzerStatus->EditLabel->Caption = str;
	}
	else
		O2AnalyzerStatus->Visible = false;

	if( GetManager()->UseMagazineDetector )
	{
		if( Chamber->IsMagazineDetected() == true )
		{
			clr = clYellow;
			str = "Magazine Detected";
		}
		else
		{
			clr = clSilver;
			str = "Magazine Not Detected";
		}

		if( MagazineDetectorStatus->Visible != true )
			MagazineDetectorStatus->Visible = true;
		if( MagazineDetectorStatus->Color != clr )
			MagazineDetectorStatus->Color = clr;
		if( MagazineDetectorStatus->EditLabel->Caption != str )
			MagazineDetectorStatus->EditLabel->Caption = str;
	}
	else
		MagazineDetectorStatus->Visible = false;

	if( GetManager()->UseAutomation == true &&
		GetManager()->AutomationKind == BARUN_MES &&
		BarunMESDlg->CimParam.OnlineMode == true )
	{
		if( Chamber->BarunManager->Binded )
		{
			clr = clYellow;
			str = "ONLINE";
		}
		else
		{
			clr = clSilver;
			str = "OFFLINE";
		}

		if( OnlineStatus->Visible != true )
			OnlineStatus->Visible = true;
		if( OnlineStatus->Color != clr )
			OnlineStatus->Color = clr;
		if( OnlineStatus->EditLabel->Caption != str )
			OnlineStatus->EditLabel->Caption = str;
	}
	else
	{
		if( OnlineStatus->Visible != false )
			OnlineStatus->Visible = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::BlinkButtons(bool blink)
{
	if( Chamber == nullptr )
		return;

	TColor clr = blink ? clRed : clNavy;
	if( ShowAlarmButton->BackColor != clr )
	{
		ShowAlarmButton->BackColor = clr;
		ShowAlarmButton->ShineColor = clr;

		BuzzerOffButton->BackColor = clr;
		BuzzerOffButton->ShineColor = clr;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::BlinkControls(bool blink)
{
	BlinkButtons(blink);
	RefreshCommIndicator(blink);
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::RefreshTimer(TObject *Sender)
{
	if( Chamber == nullptr )
		return;

	Refresh->Enabled = false;
	RefreshData();
	UpdateButtonState();
	Chamber->IsBlink = !Chamber->IsBlink;

	String str;
	if (Chamber->UseIdleCooling && (Chamber->Step == STEP_DONE || Chamber->Step == STEP_IDLE))
    {
    	str = GetResStr("Form", "StopIdleCooling");
    }
	else
	{
		str = GetResStr("Form", "CureStop");
    }
    if (CureStopButton->Caption != str)
	{
    	CureStopButton->Caption = str;
	}

    if (GetManager()->IsShowLoginForm == true)
	{
		if (AlarmDlg[Chamber->ChamberID]->Showing)
		{
			AlarmDlg[Chamber->ChamberID]->Hide();
		}
	}
	else
	{
		if (JobEndDlg[Chamber->ChamberID]->Showing)
		{
			if (AlarmDlg[Chamber->ChamberID]->Showing)
			{
				AlarmDlg[Chamber->ChamberID]->BringToFront();
			}
			else
			{
				JobEndDlg[Chamber->ChamberID]->BringToFront();
			}
		}
	}

	Refresh->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::RefreshData()
{
	if (Chamber == nullptr)
	{
		String str = "Chamber nullptr";
		GetOvenDB()->SaveLog(str, MESSAGES);

		return;
	}
	if (Chamber->Step < STEP_IDLE)
	{
		return;
	}

	String str, tmp;

	if( Chamber->MainTempController == nullptr ||
		(Chamber->MainTempController && Chamber->MainTempController->IsFail) )
	{
		str = L"<P align=\"center\"><B>--</B></P>";
		if( ChamberTempPanel->Text != str )
			ChamberTempPanel->Text = str;

		str = L"NOT CONNECTED";
		if( ChamberStatusPanel->StatusBar->Text != str )
			ChamberStatusPanel->StatusBar->Text = str;
	}
	else
	{
		str.sprintf(L"<P align=\"center\"><B>%.1f</B></P>",
					Chamber->PV);
		if( ChamberTempPanel->Text != str )
			ChamberTempPanel->Text = str;

		str.sprintf(L"SP=%.1f  SEG=%d/%d  H=%.0f\%",
					Chamber->SP,
					Chamber->SegNo,
					Chamber->TotalSeg,
					Chamber->HeaterOut);
		if( ChamberTempPanel->StatusBar->Text != str )
			ChamberTempPanel->StatusBar->Text = str;

		int time=0, hour=0, min=0;

		time = Chamber->CalcRemainTime();


		if( Chamber->Step == STEP_DONE ||
			(Chamber->Step == STEP_COOLING &&
			 (Chamber->IsReset ||
			  Chamber->IsLocal)) )
		{
			hour = 0;
			min = 0;
		}
		else
		{
			hour = time / 60;
			min = time - hour*60;
		}
		str.sprintf(L"<P align=\"center\"><B>%02d:%02d</B></P>", hour, min);
		if( RemainTimePanel->Text != str )
			RemainTimePanel->Text = str;

		//2021-04-15 HANA OVEN
		if(GetManager()->AutomationKind == HANA_CIM && GetManager()->UseAutomation == true)
		{
			Chamber->HanOvenCim->DoHanaPV(Chamber->PV);
			Chamber->HanOvenCim->DoHanaSV(Chamber->SP);

			String sRet = "";
			sRet.sprintf(L"%02d.%02d.%02d", hour, min, 0);
			Chamber->HanOvenCim->DoHanaRemainTime(sRet);
		}

		str = L"MODE=";
		str += OpmodeStr[Chamber->OpMode];
		if( ChamberStatusPanel->StatusBar->Text != str )
			ChamberStatusPanel->StatusBar->Text = str;

		if( Chamber->Step != STEP_IDLE )
		{
			str = "IN=" + Chamber->Recipe.StartTime.FormatString("hh:nn");
			time = Chamber->Recipe.TempPtn.TotalTime;
			hour = time / 60;
			min = time - hour*60;
			TDateTime endTime = Chamber->Recipe.StartTime + TDateTime(hour, min, 0, 0);
			str += "  OUT=" + endTime.FormatString("hh:nn");
		}
		else
		{
			str = "";
		}
		if( RemainTimePanel->StatusBar->Text != str )
			RemainTimePanel->StatusBar->Text = str;
	}

	if( GetManager()->DPType )
	{
		if( Chamber->DPController == nullptr ||
			(Chamber->DPController && Chamber->DPController->IsFail) )
		{
			str = L"<P align=\"center\"><B>--</B></P>";
			if( DPPanel->Text != str )
				DPPanel->Text = str;

			str = L"NOT CONNECTED";
			if( DPPanel->StatusBar->Text != str )
				DPPanel->StatusBar->Text = str;
		}
		else
		{
			str.sprintf(L"<P align=\"center\"><B>%.1f</B></P>",
							Chamber->DPController->PV);
			if( DPPanel->Text != str )
				DPPanel->Text = str;

			str = "";
			if( DPPanel->StatusBar->Text != str )
				DPPanel->StatusBar->Text = str;
		}
	}
	else
	{
		str = "";
		if( DPPanel->Text != str )
			DPPanel->Text = str;

		if( DPPanel->StatusBar->Text != str )
			DPPanel->StatusBar->Text = str;
	}

	if( GetManager()->UseO2Analyzer )
	{
		if( Chamber->O2Analyzer == nullptr ||
			(Chamber->O2Analyzer && Chamber->O2Analyzer->IsFail) )
		{
			str = L"<P align=\"center\"><B>--</B></P>";
			if( O2Panel->Text != str )
				O2Panel->Text = str;

			str = L"NOT CONNECTED";
			if( O2Panel->StatusBar->Text != str )
				O2Panel->StatusBar->Text = str;
		}
		else
		{
			str.sprintf(L"<P align=\"center\"><B>%.0f</B></P>",
							Chamber->O2Analyzer->O2Density);
			if( O2Panel->Text != str )
				O2Panel->Text = str;

			str = "";
			if( O2Panel->StatusBar->Text != str )
				O2Panel->StatusBar->Text = str;
		}
	}
	else
	{
		str = "";
		if( O2Panel->Text != str )
			O2Panel->Text = str;

		if( O2Panel->StatusBar->Text != str )
			O2Panel->StatusBar->Text = str;
	}

	tmp = StatusStr[Chamber->Step];
	str.sprintf(L"<P align=\"center\"><B>%s</B></P>", tmp.c_str());
	if( ChamberStatusPanel->Text != str )
		ChamberStatusPanel->Text = str;

	if( Chamber->GetAlarmCount() > 0 )
	{
		if( ShowAlarmButton->Visible == false )
		{
			ShowAlarmButton->Visible = true;
			BuzzerOffButton->Visible = true;
		}
	}
	else
	{
		if( ShowAlarmButton->Visible == true )
		{
			ShowAlarmButton->Visible = false;
			BuzzerOffButton->Visible = false;
		}
	}
	//JSCK 2023-03-10
	if( Chamber->Step == STEP_RUNNING || Chamber->Step == STEP_COOLING )
	{
		if (EzNetDlg[Chamber->ChamberID] != nullptr)
		{
			EzNetDlg[Chamber->ChamberID]->SetProcessState(STATE_EXECUTING);
		}
		IsCsvSaveComplete = false;
	}

	if (GetManager()->UseAutomation && GetManager()->IsUseGem() &&
		GemDlg && GemDlg->IsGemConnected())
	{
		int chamberNumber = Chamber->ChamberID;
		if (Chamber->Step == STEP_IDLE)
			GemDlg->SetProcessState(chamberNumber, STATE_IDLE);
		else if (Chamber->Step > STEP_IDLE || Chamber->Step < STEP_DONE ||
				 Chamber->Step == STEP_LOCAL)
			GemDlg->SetProcessState(chamberNumber, STATE_EXECUTING);
		else if (Chamber->Step == STEP_DONE)
			GemDlg->SetProcessState(chamberNumber, STATE_DONE);
	}

	if( GetManager()->LoginLevel > 0 &&
		(Chamber->Step == STEP_DONE || Chamber->Step == STEP_LOCAL) &&
		Chamber->Recipe.JobEnd == false )
	{
		OVEN_WORK work;
		work.StartTime = Chamber->Recipe.StartTime;
		work.Recipe = Chamber->Recipe.RecipeName;
		work.PtnNo = Chamber->Recipe.TempPtn.PtnNo;
		work.CureTemp = Chamber->Recipe.TempPtn.MaxTemp;
		work.CureTime = Chamber->Recipe.TempPtn.TotalTime;
		work.Remote = false;
		work.UserID = GetManager()->UserID;
		work.ChamberNo = Chamber->ChamberID;
		work.EndTime = Now().FormatString("yyyy-mm-dd hh:nn:ss");
		GetOvenDB()->SaveWorkEnd(work, Chamber->Recipe.StopType);

		if (GetManager()->AutomationKind == SECS_GEM_COWELL)
		{
			String date, time;
			date = work.EndTime.DateTimeString();
			str = "Cure End Time " + date;
			GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);
			SaveCSVCureDate(work.EndTime);
			str = "CSV Save Success";   //COWELL LOG
			GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);
			IsCsvSaveComplete = true;
			Chamber->WriteJobStartEndData(work.EndTime, false);
			str = "Job Start End Save Success";   //COWELL LOG
			GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);
		}

		ShowJobEnd(Now().FormatString("yyyy-mm-dd hh:nn:ss"), Chamber->Recipe.StopType);

		InitLotInfoGrid();

		Chamber->Recipe.JobEnd = true;

		if (Chamber->RecipeMaintain == false)  //2022-07-12
		{
			Chamber->Recipe.RecipeName = "";	// 2022-01-05 Add 큐어종료시 레시피네임 삭제
		}

		if( GetManager()->UseAutomation )
		{
			if( GetManager()->AutomationKind == BARUN_MES &&
				Chamber->BarunManager &&
				Chamber->UseManualMode == false &&
				BarunMESDlg->CimParam.OnlineMode == true )
			{
				BARUN_RESULT res;
				String str;

				if( Chamber->Recipe.StopType == NORMAL_STOP )
				{
					bool success = false;
					bool tryFail = true;
					for(int i=0; i<3; i++)
					{
						res = Chamber->BarunManager->SendLotEnd(UserIDEdit->Text);
						if( res.Result )
						{
							success = true;
							break;
						}
						else
						{
							if( res.ErrorCode != ERRCODE_NO_RESPONSE &&
								res.ErrorCode != ERRCODE_NOT_CONNECTED &&
								res.ErrorCode != ERRCODE_MES_ERROR )
							{
								tryFail = false;
								break;
							}
							else
								Sleep(300);
						}
					}

					if( success )
					{
						str = L"LOT END가 정상적으로 처리되었습니다.";
					}
					else
					{
						if( tryFail )
						{
							str = L"서버로부터 LOT END 응답을 받지 못하였습니다.\r\n수동으로 LOT 처리를 진행 하십시오.";
						}
						else
						{
							str.sprintf(L"에러 코드가 수신되었습니다.\r\n에러코드 : %d\r\n에러내용 : %s",
										res.ErrorCode,
										res.Error.c_str());
						}
					}
					SetMessageStr(str);
					ShowMessageDlg();
				}
			}
			else
			if( GetManager()->IsUseGem() &&
				GemDlg && GemDlg->IsGemConnected() )
			{
				if( Chamber->Recipe.StopType == NORMAL_STOP )
				{
					GemDlg->SendJobEndEvent(Chamber->ChamberID,
											Chamber->Recipe.lotInfo.ToString(),
											Chamber->Recipe.RecipeName);
				}
				else
				if( Chamber->Recipe.StopType == FORCED_STOP )
				{
					GemDlg->SendJobCancelEvent(Chamber->ChamberID,
											Chamber->Recipe.lotInfo.ToString(),
											Chamber->Recipe.RecipeName);
				}

				GemDlg->SetProcessState(Chamber->ChamberID, STATE_DONE);

				if( GetManager()->AutomationKind == SECS_GEM_COWELL )
				{
					Chamber->CowellLotInfoClear();
				}

				str = "GEM Job End Send Success";   //COWELL LOG
				GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);
			}
			// 20191217 - SCK MES - jhlim
			else
			if( GetManager()->AutomationKind == SCK_MES )
			{
				if( SaveCureDoneInfo_MESLotInfo(Chamber->Recipe.StopType) == false )
				{
					str.sprintf(L"[MES] Failed to save cure done information file.");
					SetMessageStr(str);
					ShowMessageDlg();
				}
			}
//			else  //2023-02-08 COWELL Add
//			if( GetManager()->AutomationKind == SECS_GEM_COWELL )
//			{
//				Chamber->CowellLotInfoClear();
//			}
			else //2023-03-09 JSCK Add
			if ( GetManager()->AutomationKind == SECS_GEM_SCK && EzNetDlg[Chamber->ChamberID] != nullptr )
			{
				if( Chamber->Recipe.StopType == NORMAL_STOP )
				{
					String startTime = work.StartTime.FormatString("yyyy-mm-dd hh:nn:ss");
					String endTime = work.EndTime.FormatString("yyyy-mm-dd hh:nn:ss");

					EzNetDlg[Chamber->ChamberID]->SendJobComplete(1/*Chamber->MesMode*/, Chamber->Recipe.RecipeName,
							/*Chamber->DeviceIDList*/Chamber->MgzDeviceIDList, startTime, endTime);
				}
				else
				if( Chamber->Recipe.StopType == FORCED_STOP )
				{
					EzNetDlg[Chamber->ChamberID]->SendJobCancel(Chamber->MesMode, Chamber->Recipe.RecipeName,
						Chamber->MgzDeviceIDList, GetManager()->UserID);
				}
				Chamber->JsckPvDataList.clear();
				Chamber->JsckGemParamClear();
				Chamber->MgzDeviceIDList->Clear();
				EzNetDlg[Chamber->ChamberID]->SetProcessState(STATE_DONE);

				str = "JSCK GEM Job End Send Success";
				GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);
			}
		}
	}

	RefreshStatusIndicator();
 //	RefreshSafetyButton();  //2023-04-17 restore

	if (Chamber->DeviceRemoved)
	{
		Chamber->DeviceRemoved = false;
		if (GetManager()->AutomationKind == SECS_GEM_COWELL)
		{
			UserIDEdit->Text = "";
			if (this->Showing && UserIDEdit->Visible)
			{
				UserIDEdit->SetFocus();
			}
			for(int i=1; i<=LOT_COUNT; i++)
			{
				LotInfoGrid->Cells[1][i] = "";
            }

			if (Chamber->RecipeMaintain == false)
			{
				for(int i=0; i<=5; i++)
				{
					RecipeInfoGrid->Cells[1][i] = "";
                }
				PatternGraph->Series[0]->Clear();
			}
			String strFile;
			strFile.sprintf(L"%s%s%c\\remote.ptn",
							GetRootFolder().c_str(),
							L"PATTERN_CM_",
							'A' + Chamber->ChamberID);

			if( FileExists(strFile) )
				DeleteFile(strFile);
		}

		CurrentMaxRow = 1;

		if( GetManager()->IsUseGem() &&
			GemDlg && GemDlg->IsGemConnected() )
		{
			GemDlg->SetProcessState(Chamber->ChamberID, STATE_IDLE);
		}
		else if (GetManager()->UseAutomation == true &&
			GetManager()->AutomationKind == SECS_GEM_SCK && EzNetDlg[Chamber->ChamberID] != nullptr &&
			EzNetDlg[Chamber->ChamberID]->IsGemConnected())
		{
			EzNetDlg[Chamber->ChamberID]->SetProcessState(STATE_IDLE);
		}

        if (GetManager()->AutomationKind == SECS_GEM_COWELL)
		{
			str = "GEM Process State Change Success";   //COWELL LOG
			GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);
		}

		// 20180321 - FTP (OFILM MES) - jhlim
		if( GetManager()->UseAutomation == true &&
			GetManager()->AutomationKind == OFILM_MES )
		{
			// Init
			Chamber->FTPLotInfo.TotalLotCount = 0;
			for(int j=0; j<FTP_LOT_COUNT; j++)
			{
				Chamber->FTPLotInfo.LotID[j] = "";
				Chamber->FTPLotInfo.DeviceType[j] = "";
			}
		}
		else if( GetManager()->UseAutomation &&
			GetManager()->AutomationKind == SECS_GEM_COWELL)
		{
			Chamber->CowellLotInfo.TotalLotCount = 0;
			for(int j=0; j < LOT_COUNT; j++)
			{
				Chamber->CowellLotInfo.LotID[j] = "";
				Chamber->CowellLotInfo.DeviceType[j] = "";
			}
			str = "Cowell Lot Info Clear";   //COWELL LOG
			GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);
		}
	}

	if( ProgressBar->Visible )
		ProgressBar->Position = Chamber->ProgressValue;
	else
		ProgressBar->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::SaveCSVCureDate(TDateTime end)
{
	if (GetManager()->UseCsvDataSave && IsCsvSaveComplete == false)
	{
		String str, strLot;
		for (int i = 0; i < LotInfoGrid->RowCount; i++)
		{
			str.sprintf(L" %s", LotInfoGrid->Cells[1][i+1].c_str());
			strLot += str;
		}

        end.FormatString("yyyy-mm-dd hh:nn:ss");
		Chamber->WriteProductData(end, GetManager()->CsvMachineId, GetManager()->UserID,
									strLot, false);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::UpdateButtonState()
{
	if( Chamber == nullptr )
		return;

	if( !GetManager()->UseMagazineDetector )
	{
		if (GetManager()->UseAutomation == true &&
			GetManager()->IsUseGem() && GemDlg &&
			GemDlg->IsGemConnected() &&
			GemDlg->GemState[Chamber->ChamberID].nControlState == CONTROL_ONLINE_REMOTE)
		{
			if (GetManager()->AutomationKind == SECS_GEM_COWELL)
			{
				if (ProgressBar->Visible)
				{
					if( CureStartButton->Enabled != false )
						CureStartButton->Enabled = false;
				}
				else
				{
					if( CureStartButton->Enabled != true )
						CureStartButton->Enabled = true;
				}
			}
			else if (GetManager()->AutomationKind == SECS_GEM_SCK)
			{
				if (Chamber->ValidationSuccess && GetManager()->AutoCureStartAfterValidation == false)
				{
					if( CureStartButton->Enabled != true )
						CureStartButton->Enabled = true;
				}
				else
				{
					if( CureStartButton->Enabled != false )
						CureStartButton->Enabled = false;
				}
			}
			else
			{
				if( CureStartButton->Enabled != false )
					CureStartButton->Enabled = false;
			}
		}
		else
		{
			if( Chamber->Step == STEP_IDLE )
			{
				if (ProgressBar->Visible)
				{
					if( CureStartButton->Enabled != false )
						CureStartButton->Enabled = false;
				}
				else
				{
					if( CureStartButton->Enabled != true )
						CureStartButton->Enabled = true;
				}
			}
			else
			{
				if( CureStartButton->Enabled != true )
					CureStartButton->Enabled = true;
            }
		}
	}

	if (GetManager()->AutomationKind == SECS_GEM_SCK &&
		GetManager()->UseAutomation == true &&
		EzNetDlg[Chamber->ChamberID] != nullptr &&
		(EzNetDlg[Chamber->ChamberID]->IsOnlineLocal() ||
		EzNetDlg[Chamber->ChamberID]->IsOnlineRemote()))
	{
		if (Chamber->ValidationSuccess && GetManager()->AutoCureStartAfterValidation == false)
		{
			if( CureStartButton->Enabled != true )
				CureStartButton->Enabled = true;
		}
		else
		{
			if( CureStartButton->Enabled != false )
				CureStartButton->Enabled = false;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::ShowJobEnd(TDateTime end, int endType, bool remote/*=false*/)
{
	if( Chamber == nullptr )
	{
		String str;
		str = "ShowJobEnd Fail Chamber nullptr";
		GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);
		return;
	}

	JobEndDlg[Chamber->ChamberID]->UpdateInfo(end, endType, remote);

	JobEndDlg[Chamber->ChamberID]->Show();
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::ShowAlarm()
{
	if( Chamber == nullptr )
		return;

	if( Chamber->GetAlarmCount() > 0 &&
		Chamber->NewAlarm )
	{
		if( AlarmDlg[Chamber->ChamberID]->Showing == false )
			AlarmDlg[Chamber->ChamberID]->Show();
		Chamber->NewAlarm = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::GrabChamberData()
{
	if( Chamber == nullptr )
		return;

	TDateTime xtime, time = Now().FormatString("yyyy-mm-dd hh:nn:ss");

	if( Chamber->Step != STEP_IDLE )
	{
		if( Chamber->Step == STEP_DONE )
			IsIdleStarted = false;

		xtime = time - Chamber->Recipe.StartTime;
	}
	else
	{
		if( IsIdleStarted == false )
		{
			IsIdleStarted = true;
			IdleStartTime = Now().FormatString("yyyy-mm-dd hh:nn:ss");
			InitChart(IdleStartTime, 180, 200);
		}
		xtime = time - IdleStartTime;
	}

	if( TempGraph->Zoomed == false )
	{
		double dInc = TDateTime(0, 1, 0, 0);
		TDateTime maxTime = TDateTime(TempGraph->BottomAxis->Maximum - 10*dInc);
		if( xtime > maxTime )
		{
			double min = TempGraph->BottomAxis->Minimum;
			double max = TempGraph->BottomAxis->Maximum;
			double diff = (double)xtime - (double)maxTime;
			min += diff;
			max += diff;

			TempGraph->BottomAxis->SetMinMax(min, max);
		}
	}

	int ch = 0;
	double data;
	for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++)
	{
		data = Chamber->LoggerData[i];
		if( Chamber->UseChannel[i] )
		{
			if( data <= 0.0 )
				data = TempLoggerData[i];
			if( Series[ch] )
				Series[ch]->AddXY(xtime, data);
			TempLoggerData[i] = data;
			ch++;
		}
	}

	if( GetManager()->UseO2Analyzer &&
		Chamber->O2Analyzer &&
		Series[ch] )
	{
		Series[ch]->AddXY(xtime, Chamber->O2Analyzer->O2Density);
		ch++;
	}

	if( Chamber->MainTempController &&
		Series[ch] )
		Series[ch]->AddXY(xtime, Chamber->PV);

	if( Chamber->Step > STEP_IDLE &&
		Chamber->Step < STEP_DONE )
	{
		Chamber->WriteMainTempData(time);
		Chamber->WriteLoggerData(time);
		Chamber->WriteLimitData(time);
		if (GetManager()->UseO2Analyzer)
		{
			Chamber->WriteOxygenData(time);
		}
	}

	if( Chamber->Step > STEP_IDLE &&
		Chamber->Step <= STEP_DONE )
		Chamber->SaveCurrentState();

	if (GetManager()->UseAutomation &&
		GetManager()->IsUseGem() &&
		GemDlg && GemDlg->IsGemConnected())
	{
		GemDlg->UpdateSVidPara(Chamber->ChamberID);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::GrabDataTimer(TObject *Sender)
{
	GrabData->Enabled = false;
	GrabChamberData();
	GrabData->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamber::AlarmCheckTimer(TObject *Sender)
{
	if( Chamber == nullptr )
		return;

	AlarmCheck->Enabled = false;

	if( GetManager()->UseMagazineDetector )
	{
		if( Chamber->IsAbleAutoStart == true &&
			Chamber->Step == STEP_IDLE &&
			Chamber->IsDoorClosed() == true &&
			Chamber->IsMagazineDetected() == true &&
			Chamber->InitCompleted == true )
		{
			if( AutoStartFailed == false )
			{
				MessageShowed = false;
				EnableButtons(false);
				if( CureStart() == false )
				{
					ShowMessageDlg();
				}
				EnableButtons();
			}
			else
			{
				if( MessageShowed == false )
				{
					MessageShowed = true;
					SetMessageStr(GetResStr("Warning", "DoManualStart"));
					ShowMessageDlg();
				}
			}
		}
	}

	// 2021-12-01 KJH 큐어 시작 알람
	if (GetManager()->CureStartAlarmTime > 0)
	{
		if (Chamber->Step == STEP_IDLE &&
			Chamber->IsDoorClosed() &&
			Chamber->AlarmStatus[ERR_CURE_NOT_STARTED] == false)
		{
			if (CureStartAlarmTimer->Enabled == false)
			{
				CureStartAlarmTimer->Interval = GetManager()->CureStartAlarmTime * 1000;
				CureStartAlarmTimer->Enabled = true;
			}
		}
	}

	ShowAlarm();
	AlarmCheck->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::CureStartAlarmTimerTimer(TObject *Sender)
{
	if( Chamber == nullptr )
		return;

	CureStartAlarmTimer->Enabled = false;
	if (Chamber->Step == STEP_IDLE &&
		GetManager()->IsShowValidationForm == false &&
		CureStarting == false)
	{
		if (Chamber->IsDoorClosed() &&	// 2021-12-22 타이머 체크시 문열려 있으면 안뜨도록
			Chamber->AlarmStatus[ERR_CURE_NOT_STARTED] == false)
		{
			Chamber->AlarmStatus[ERR_CURE_NOT_STARTED] = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::TempGraphDblClick(TObject *Sender)
{
	GraphMode++;
	if( GraphMode > GM_HIDE_LEGEND )
		GraphMode = GM_NORMAL;

	if( GraphMode == GM_NORMAL )
	{
		TempGraph->Legend->Visible = true;
		TempGraph->Legend->LegendStyle = lsAuto;
	}
	else
	if( GraphMode == GM_VALUE_LEGEND )
	{
		TempGraph->Legend->Visible = true;
		TempGraph->Legend->LegendStyle = lsLastValues;
	}
	else
	if( GraphMode == GM_HIDE_LEGEND )
	{
		TempGraph->Legend->Visible = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamber::ShowAlarmButtonClick(TObject *Sender)
{
	if( Chamber == nullptr )
	{
		String str;
		str.sprintf(L"Chamber is nullptr");
		GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);
		return;
	}

	Chamber->NewAlarm = true;
	ShowAlarm();

	DoAutoFocus();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamber::BuzzerOffButtonClick(TObject *Sender)
{
	if( Chamber == nullptr )
	{
		String str;
		str.sprintf(L"Chamber is nullptr");
		GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);
		return;
	}

	Chamber->BuzzerOff();
	//Chamber->DoBuzzerAlarm = false;

	DoAutoFocus();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamber::SelectButtonClick(TObject *Sender)
{
	if( Chamber == nullptr )
		return;

	if( Chamber->Step != STEP_IDLE )
	{
		if( Chamber->Step == STEP_LOCAL )
		{
			SetMessageStr(GetResStr("Question", "QuestSelectRecipeInLocalMode"));
			if( !ShowQuestionDlg() )
				return;
		}
		else
		{
			SetMessageStr(GetResStr("Warning", "CuringJob"));
			ShowMessageDlg();
			return;
		}
	}

	if (GetManager()->UseAutomation && GetManager()->AutomationKind == TCARD_RECIPE_LOADING)
	{
		FmTCardRecipeLoading->ShowModal();
		if (FmTCardRecipeLoading->Result == mrOk)
		{
			if (SelectRecipe(FmTCardRecipeLoading->LastTCardID) == false)
			{
				SetMessageStr(GetResStr("Warning", "NotExistedRecipe"));
				ShowMessageDlg();
			}
			else
			{
                UpdateRecipeComboBox();
            }
		}
	}
	else
	{
		if( GetManager()->LoginLevel < LV_ENGINEER )
		{
        	if (Chamber->AllowRecipeSelection == false)
            {
                if( ShowLogin(LV_ENGINEER, false) == false )
                {
                	RecipeComboBox->Enabled = false;
                    return;
                }
            }
		}
		RecipeComboBox->Enabled = true;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TFrmChamber::SelectRecipe(String recipe)
{
	if( Chamber == nullptr )
		return false;

	OVEN_RECIPE recipeInfo;

	if( GetOvenDB()->GetRecipe(Chamber->ChamberID, recipe, recipeInfo) )
	{
		Chamber->Recipe.RecipeName = recipeInfo.Recipe;
		Chamber->Recipe.IsGemRecipe = false;
		LoadTempPtn(Chamber->ChamberID, recipeInfo.PtnNo, Chamber->Recipe.TempPtn);

		String rec;
        int nItemCount = RecipeComboBox->Items->Count;
        for(int i=0; i<nItemCount; i++)
		{
            rec = RecipeComboBox->Items->Strings[i];
            if(rec == recipe)
			{
                RecipeComboBox->ItemIndex = i;
            }
		}

		FillRecipeGrid(recipeInfo);
		InitPtnChart();

		Chamber->ApplyDataToController();

		if (GetManager()->UseAutomation == true &&
			GetManager()->IsUseGem() &&
			GemDlg && GemDlg->IsGemConnected())
		{
			GemDlg->SendPPSelectedEvent(Chamber->ChamberID, recipeInfo.PtnNo, recipe);
		}

		String str;
		str.sprintf(L"The recipe [%s] is selected.", recipe.c_str());
		GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);

		return true;
	}

	return false;
}
//---------------------------------------------------------------------------
bool __fastcall TFrmChamber::SelectRemoteRecipe(String recipe)
{
	if( Chamber == nullptr )
		return false;

	if( IsGemRecipeExist(recipe) )
	{
		Chamber->Recipe.RecipeName = recipe;
		Chamber->Recipe.IsGemRecipe = true;

		LoadRemoteTempPtn(recipe, Chamber->Recipe.TempPtn);

		String rec;
		RecipeComboBox->Clear();
		RecipeComboBox->Items->Add(recipe);
		RecipeComboBox->Text = recipe;
		RecipeComboBox->ItemIndex = 0;

		FillRecipeGrid(Chamber->Recipe.TempPtn, recipe);
		InitPtnChart();

		Chamber->ApplyDataToController();

		if (GetManager()->UseAutomation == true &&
			GetManager()->IsUseGem() &&
			GemDlg && GemDlg->IsGemConnected())
		{
			GemDlg->SendPPSelectedEvent(Chamber->ChamberID, Chamber->Recipe.TempPtn.PtnNo, recipe);
		}

/*		String str;
		str.sprintf(L"The recipe [%s] is selected.", recipe);
		GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID); */
		return true;
	}

	return false;
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamber::RecipeComboBoxChange(TObject *Sender)
{
	RecipeComboBox->Enabled = false;

	String rec = RecipeComboBox->Items->Strings[RecipeComboBox->ItemIndex];
	SelectRecipe(rec);

	InitChart(Chamber->Recipe.StartTime,
			  Chamber->Recipe.TempPtn.TotalTime,
			  Chamber->Recipe.TempPtn.MaxTemp);
	if (GetManager()->UseAutomation == true &&
		GetManager()->AutomationKind == SECS_GEM_SCK)
	{
		Chamber->ValidationSuccess = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::EnableButtons(bool enable/*=true*/)
{
	if( GetManager()->UseAutomation == true &&
		(GetManager()->AutomationKind == SCK_MES ||
		GetManager()->AutomationKind == SECS_GEM_SCK) ||
		GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		CureStartButton->Enabled = false;
        LocalButton->Enabled = false;
	}
	else if (GetManager()->UseAutomation == true &&
		GetManager()->IsUseGem() && GemDlg &&
		GemDlg->GemState[Chamber->ChamberID].nControlState == CONTROL_ONLINE_REMOTE)
	{
		CureStartButton->Enabled = false;
	}
	else if (GetManager()->UseAutomation == true &&
		GetManager()->IsUseGem() && EzNetDlg[Chamber->ChamberID] &&
		(EzNetDlg[Chamber->ChamberID]->IsOnlineLocal() ||
		EzNetDlg[Chamber->ChamberID]->IsOnlineRemote()))
	{
		CureStartButton->Enabled = false;
	}
	else
	{
		CureStartButton->Enabled = enable;
	}
	CureStopButton->Enabled = enable;
//	LocalButton->Enabled = enable;
}
//---------------------------------------------------------------------------
bool __fastcall TFrmChamber::CureStart(bool byRemote/*=false*/)
{
	if( Chamber == nullptr )
		return false;

	AutoStartFailed = true;
	CureStarting = true;

	String str;
	int chamberNo = Chamber->ChamberID;

	if (Chamber->O2Analyzer && Chamber->O2Analyzer->IsValid == false)
	{
		SetMessageStr(GetResStr("Warning", "O2AnalyzerNotReady"));
		CureStarting = false;
		return false;
	}

	if (Chamber->Step == STEP_DONE)
	{
		SetMessageStr(GetResStr("Warning", "ExistProduct"));
		CureStarting = false;
		return false;
	}

	if (Chamber->Step != STEP_IDLE && Chamber->Step != STEP_LOCAL)
	{
		SetMessageStr(GetResStr("Warning", "CuringJob"));
		CureStarting = false;
		return false;
	}

	if (Chamber->Step == STEP_LOCAL && GetManager()->UseCureStartEnableFromLocalState)
	{
		Chamber->DoCureStop(LOCALMODE_STOP);
    }

	if( (Chamber->Recipe.RecipeName.IsEmpty() || RecipeComboBox->ItemIndex < 0) )
	{
		SetMessageStr(GetResStr("Warning", "NoSelectedRecipe"));
		CureStarting = false;
		return false;
	}

	if( GetManager()->LoginLevel < LV_DEVELOPER &&
		Chamber->GetAlarmCount() > 0 )
	{
		SetMessageStr(GetResStr("Warning", "CantRunWhileAlarm"));
		CureStarting = false;
		return false;
	}

	if( GetManager()->UseMagazineDetector )
	{
		if( Chamber->IsMagazineDetected() == false )
		{
			SetMessageStr(GetResStr("Question", "MangazineNotDetected"));
			if( ShowQuestionDlg() == false )
			{
				CureStarting = false;
				return false;
			}
		}
	}

	// 20180426 - FTP (OFILM MES) - jhlim
	LOTINFO lotInfo;
//	if( CheckLotInfo(lotInfo) == false )
//		return false;
//	Chamber->Recipe.lotInfo = lotInfo;
	if( GetManager()->UseAutomation == true &&
		GetManager()->AutomationKind == OFILM_MES )
	{
		if(Chamber->FTPLotInfo.TotalLotCount > 0)
		{
			lotInfo = Chamber->FTPLotInfo;
		}
	}
	else if( GetManager()->UseAutomation &&
		GetManager()->AutomationKind == SECS_GEM_COWELL)
	{
		if(Chamber->CowellLotInfo.TotalLotCount > 0)
		{
			lotInfo = Chamber->CowellLotInfo;
		}
	}
	else
	{
		if (GetManager()->AutomationKind != HANA_CIM)  //2021-04-13 HANA OVEN
		{
			if( CheckLotInfo(lotInfo) == false )
			{
				CureStarting = false;
				return false;
			}
		}
	}
	Chamber->Recipe.lotInfo = lotInfo;

	if( Chamber->CannotStartWithoutLotInfo == true )
	{
		if( lotInfo.TotalLotCount <= 0 )
		{
			SetMessageStr(GetResStr("Warning", "InputLotInfo"));
			CureStarting = false;
			return false;
		}
	}

	if( Chamber->IsDoorClosed() == false )
	{
		SetMessageStr(GetResStr("Warning", "DoorOpened"));
		CureStarting = false;
		return false;
	}

	if( Chamber->Step == STEP_LOCAL && byRemote == false)
	{
		SetMessageStr(GetResStr("Question", "QuestCureStartInLocalMode"));
		if( !ShowQuestionDlg() )
		{
			CureStarting = false;
			return false;
		}
	}

	String userId = UserIDEdit->Text;
	if( GetManager()->UseAutomation )
	{
		if( GetManager()->AutomationKind == BARUN_MES &&
			Chamber->UseManualMode == false &&
			BarunMESDlg->CimParam.OnlineMode == true &&
			Chamber->BarunManager )
		{
			if (GetManager()->AutomationKind != HANA_CIM)  //2021-04-13 HANA OVEN
			{
				if( IsValidOperatorID(userId) == false )
				{
					SetMessageStr(L"사번을 정확히 입력 하십시오.");
					CureStarting = false;
					return false;
				}
			}
		}
	}

	if( byRemote == false &&
		GetManager()->UseAutomation == false )
	{
		if( GetManager()->UseMagazineDetector == false ||
			Chamber->IsAbleAutoStart == false )
		{
			boost::shared_ptr<TFmShowPtn> ShowPtn(new TFmShowPtn(chamberNo, this) );
			if( ShowPtn->ShowModal() == mrCancel )
			{
				CureStarting = false;
				return false;
			}
		}
	}
	else
	{
		if( GetManager()->UseAutomation )
		{
			if( GetManager()->AutomationKind != BARUN_MES  &&
				!(GetManager()->AutomationKind == HANA_CIM && byRemote == true)) //2021-07-22 hcw
			{
				OVEN_RECIPE recipeInfo;
				recipeInfo.Recipe = Chamber->Recipe.RecipeName;
				recipeInfo.PtnNo = Chamber->Recipe.TempPtn.PtnNo;
				recipeInfo.CureTemp = Chamber->Recipe.TempPtn.MaxTemp;
				recipeInfo.CureTime = Chamber->Recipe.TempPtn.TotalTime;
				recipeInfo.ChamberNo = Chamber->ChamberID;

				FillRecipeGrid(recipeInfo);
				InitPtnChart();
			}
		}
	}

	ProgressBar->Position = 0;
	ProgressBar->Maximum = Chamber->Recipe.TempPtn.Stc.SegCount + 3;
	ProgressBar->Visible = true;

	bool ret = false;

	SaveSerialLog(Name, "Cure Starting...", FALSE);

	AnsiString failMsg;
	if( Chamber->DoCureStart(failMsg, byRemote) )
	{
		if( GetManager()->UseAutomation )
		{
			if( GetManager()->AutomationKind == BARUN_MES &&
				Chamber->UseManualMode == false &&
				BarunMESDlg->CimParam.OnlineMode == true &&
				Chamber->BarunManager )
			{
				BARUN_RESULT res;
				String str;

				bool success = false;
				bool tryFail = true;
				for(int i=0; i<3; i++)
				{
					res = Chamber->BarunManager->SendLotStart(userId);
					if( res.Result )
					{
						success = true;
						break;
					}
					else
					{
						if( res.ErrorCode != ERRCODE_NO_RESPONSE &&
							res.ErrorCode != ERRCODE_NOT_CONNECTED &&
							res.ErrorCode != ERRCODE_MES_ERROR )
						{
							tryFail = false;
							break;
						}
						else
							Sleep(300);
					}
				}

				if( success == false )
				{
					if( tryFail )
					{
						str = L"서버로부터 LOT START 응답을 받지 못하였습니다.\r\n수동으로 LOT 처리를 진행 하십시오.";
					}
					else
					{
						str.sprintf(L"에러 코드가 수신되었습니다.\r\n에러코드 : %d\r\n에러내용 : %s",
									res.ErrorCode,
									res.Error.c_str());
					}
					SetMessageStr(str);
					ShowMessageDlg();
				}
			}
		}

		OVEN_WORK work;
		work.StartTime = Chamber->Recipe.StartTime;
		work.Recipe = Chamber->Recipe.RecipeName;
		work.PtnNo = Chamber->Recipe.TempPtn.PtnNo;
		work.CureTemp = Chamber->Recipe.TempPtn.MaxTemp;
		work.CureTime = Chamber->Recipe.TempPtn.TotalTime;
		work.Remote = byRemote;
		if( byRemote )
			work.UserID = "REMOTE";
		else
			work.UserID = GetManager()->UserID;
		work.ChamberNo = chamberNo;
		work.Filename.sprintf(L"%c_", 'A' + chamberNo);
		work.Filename += work.StartTime.FormatString("yyyy_mm_dd_hh_nn_ss");
		GetOvenDB()->SaveWork(work, lotInfo);

		InitChart(Chamber->Recipe.StartTime,
				  Chamber->Recipe.TempPtn.TotalTime,
				  Chamber->Recipe.TempPtn.MaxTemp);

		if (GetManager()->UseAutomation == true &&
			GetManager()->IsUseGem() &&
			GemDlg && GemDlg->IsGemConnected())
		{
			GemDlg->SendJobStartedEvent(Chamber->ChamberID, Chamber->Recipe.TempPtn.PtnNo, lotInfo.ToString(), Chamber->Recipe.RecipeName);
		}

		str = "Cure Started.";
		GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);

		if (GetManager()->AutomationKind == SECS_GEM_COWELL)
		{
			Chamber->WriteJobStartEndData(Chamber->Recipe.StartTime, true);
		}

		if( GetManager()->UseAutomation == true &&
			GetManager()->AutomationKind == FUJITSU_TCARD)
		{
			if(Chamber->Recipe.RecipeName == "")
			{
			   str = "[Cure Stop] Button Click.";
			   SetMessageStr(str);
			   ShowMessageDlg();
			}

			FujisuTCard_RecipeComboInit(false);
		}
		ret = true;
	}
	else
	{
		if (GetManager()->UseAutomation == true &&
			GetManager()->IsUseGem() &&
			GemDlg && GemDlg->IsGemConnected())
		{
			GemDlg->SendJobStartFailEvent(Chamber->ChamberID, lotInfo.ToString(), Chamber->Recipe.RecipeName);
		}
		else if (GetManager()->UseAutomation == true &&
			GetManager()->AutomationKind == SECS_GEM_SCK && EzNetDlg[Chamber->ChamberID] != nullptr &&
			EzNetDlg[Chamber->ChamberID]->IsGemConnected())
		{
			EzNetDlg[Chamber->ChamberID]->SendJobStartFail(Chamber->MesMode,
					Chamber->Recipe.RecipeName, Chamber->DeviceIDList);
        }

		str = "Cure Start Failed.";
		GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);

		SetMessageStr(GetResStr("Message", "CureStartFail"));
		SetMessageStr("\r\n");
		SetMessageStr(failMsg);
		SetMessageStr("\r\n");
		SetMessageStr(GetResStr("Message", "Retry"));

		SaveDebugLog(failMsg);
		ret = false;
	}

	ProgressBar->Visible = false;
	ProgressBar->Position = 0;

	if( ret == true )
		AutoStartFailed = false;
	else
		AutoStartFailed = true;

	CureStarting = false;
	return ret;
}
//---------------------------------------------------------------------------
bool __fastcall TFrmChamber::CureStop(bool byRemote/*=false*/)
{
	if( Chamber == nullptr )
		return false;

	bool ret = false;
	String str;

	if( byRemote == false )
	{
		if( ShowLogin(LV_SUPERVISOR, false) == false )
			return false;
	}

	if( Chamber->Step > STEP_IDLE &&
		Chamber->Step < STEP_DONE )
	{
		if( Chamber->DoCureStop(FORCED_STOP) )
		{
			if( GetManager()->UseAutomation )
			{
				if( GetManager()->AutomationKind == BARUN_MES &&
					Chamber->UseManualMode == false &&
					BarunMESDlg->CimParam.OnlineMode == true &&
					Chamber->BarunManager )
				{
					BARUN_RESULT res;

					res = Chamber->BarunManager->SendLotScanCancel(UserIDEdit->Text);
					if( res.Result == false )
					{
						str.sprintf(L"에러 코드가 수신되었습니다.\r\n에러코드 : %d\r\n에러내용 : %s",
									res.ErrorCode,
									res.Error.c_str());
						SetMessageStr(str);
						ShowMessageDlg();
					}
				}
				else
				if (GetManager()->IsUseGem() &&
					GemDlg && GemDlg->IsGemConnected() )
				{
					GemDlg->SendJobCancelEvent(Chamber->ChamberID, Chamber->Recipe.lotInfo.ToString(), Chamber->Recipe.RecipeName);
				}
				else
				if ( GetManager()->IsUseGem() &&
					GetManager()->AutomationKind == SECS_GEM_SCK && EzNetDlg[Chamber->ChamberID] != nullptr)
				{
					EzNetDlg[Chamber->ChamberID]->SendJobCancelEvent("", "", Chamber->Recipe.RecipeName);
				}
			}

			str = "Cure Stopped.";
			ret = true;
		}
		else
		{
			if (GetManager()->UseAutomation == true &&
				GetManager()->IsUseGem() &&
				GemDlg && GemDlg->IsGemConnected())
			{
				GemDlg->SendJobCancelFailEvent(Chamber->ChamberID, Chamber->Recipe.lotInfo.ToString(), Chamber->Recipe.RecipeName);
			}
			else if (GetManager()->UseAutomation == true &&
			GetManager()->AutomationKind == SECS_GEM_SCK && EzNetDlg[Chamber->ChamberID] != nullptr &&
			EzNetDlg[Chamber->ChamberID]->IsGemConnected())
			{
				EzNetDlg[Chamber->ChamberID]->SendJobCancelFail(Chamber->MesMode, Chamber->Recipe.RecipeName,
						Chamber->DeviceIDList, GetManager()->UserID);
			}

			str = "Cure Stop Failed.";
			ret = false;

			SetMessageStr(GetResStr("Message", "CureStopFail"));
			SetMessageStr("\r\n");
			SetMessageStr(GetResStr("Message", "Retry"));
			ShowMessageDlg();
		}
		GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);
	}
	return ret;
}
//---------------------------------------------------------------------------
bool __fastcall TFrmChamber::CheckAndSendLotIDToGEMHost()
{
	LOTINFO lotInfo;
	if( CheckLotInfo(lotInfo) == false )
		return false;

	Chamber->Recipe.lotInfo = lotInfo;

	if( GemDlg == nullptr )
		return false;

	String str;
	str = lotInfo.ToString();
	GemDlg->SendBarcodeInfoEvent(Chamber->ChamberID, str);

	GemDlg->SetProcessState(Chamber->ChamberID, STATE_SETUP);
	return true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::CureStartButtonClick(TObject *Sender)
{
	String str;
	str = "Cure Start Button Clicked.";
	GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);

	SaveSerialLog(Name, str, FALSE);
	// 20180322 - FTP (OFILM MES) - jhlim
	if( GetManager()->UseAutomation == true &&
		GetManager()->AutomationKind == OFILM_MES )
	{
		if(Chamber->FTPLotInfo.TotalLotCount == 0)
		{
			// Inser Lot Info Alarm
			str.sprintf(L"Please enter Lotinfo.");
			SetMessageStr(str);
			ShowMessageDlg();

			return;
		}
	}

	// 20191216 - SCK MES - jhlim
	/*if( GetManager()->UseAutomation == true &&
		GetManager()->AutomationKind == SCK_MES )
	{
		// Recipe / LotID Check Function
		if( DoSCKMESProc() == false )
		{
			ShowMessageDlg();
			return;
		}
	}*/		// 2021-12-01

	EnableButtons(false);

	if (GetManager()->UseAutomation == true && GetManager()->AutomationKind == SECS_GEM_SCK &&
		EzNetDlg[Chamber->ChamberID] != nullptr && EzNetDlg[Chamber->ChamberID]->IsGemConnected())
	{
		EzNetDlg[Chamber->ChamberID]->SendJobStart(Chamber->MesMode, Chamber->Recipe.RecipeName,
							Chamber->DeviceIDList);
	}
	else if (GetManager()->UseAutomation == true &&
		GetManager()->IsUseGem() &&
		GemDlg && GemDlg->IsGemConnected() )
	{
		if( GemDlg->GemState[Chamber->ChamberID].nControlState == CONTROL_ONLINE_REMOTE &&
			GetManager()->AutomationKind != SECS_GEM_COWELL)
		{
			if( CheckAndSendLotIDToGEMHost() == false )
				EnableButtons();
		}
		else
		{
			if (GemDlg->GemState[Chamber->ChamberID].nControlState == CONTROL_EQUIPMENT_OFFLINE &&
			GetManager()->AutomationKind == SECS_GEM_COWELL)
			{
				SetMessageStr(GetResStr("Question", "OfflineCureStart"));
				if (ShowQuestionDlg() == false)
				{
					EnableButtons();
					DoAutoFocus();
					return;
				}
			}

			if( CureStart() == false )
				ShowMessageDlg();
		}
	}
	else
	{
		bool byRemote = false;
		if( GetManager()->UseAutomation )
		{
			if( GetManager()->AutomationKind == BARUN_MES &&
				Chamber->UseManualMode == false &&
				BarunMESDlg->CimParam.OnlineMode )
				byRemote = true;
        }
		if( CureStart(byRemote) == false )
			ShowMessageDlg();
	}

	EnableButtons();
	DoAutoFocus();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamber::CureStopButtonClick(TObject *Sender)
{
	String str;

	str = "Cure Stop Button Clicked.";
	GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);
	if (Chamber->Step == STEP_LOCAL)
	{
		if (ShowLogin(LV_SUPERVISOR, false) == false)
		{
			return;
		}
		if (Chamber->MainTempController->Reset())
		{
			str = "LOCAL mode stopped.";
		}
		else
		{
			str = "LOCAL mode stop failed.";
		}
		GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);
		SetMessageStr(str);
		ShowMessageDlg();
	}
	else if (Chamber->UseIdleCooling && (Chamber->Step == STEP_DONE || Chamber->Step == STEP_IDLE))
    {
		// IDLE COOLING STOP
        str = GetResStr("Question", "QuestStopIdleCooling");
		SetMessageStr(str);
    	if (ShowQuestionDlg())
		{
			if ( GetManager()->UsePasswordWhenIdleCoolingStop )		// 2021-03-05 ADD Use Password When IdelCooling
			{
				if (ShowLogin(LV_ENGINEER, false) == false)
				{
					return;
				}
			}

			Chamber->DoIdleCoolingDisable();

            if (Chamber->IdleCoolingStarted == false)
            {
            	str = GetResStr("Message", "IdleCoolingStooped");
            }
            else
            {
            	str = GetResStr("Message", "IdleCoolingNotStooped");
            }

        	SetMessageStr(str);
            ShowMessageDlg();
        }
    }
    else
    {
		if( Chamber->Step == STEP_IDLE )
        {
            if( GetManager()->UseAutomation )
			{
				if( GetManager()->AutomationKind == BARUN_MES &&
                    Chamber->UseManualMode == false &&
                    BarunMESDlg->CimParam.OnlineMode == true &&
                    Chamber->BarunManager  )
                {
                    BARUN_RESULT res;

                    res = Chamber->BarunManager->SendLotScanCancel(UserIDEdit->Text);
                    if( res.Result == false )
                    {
						str.sprintf(L"에러 코드가 수신되었습니다.\r\n에러코드 : %d\r\n에러내용 : %s",
                                    res.ErrorCode,
									res.Error.c_str());
                        SetMessageStr(str);
                        ShowMessageDlg();
                    }
                    else
                    {
                        str = L"LOT 정보가 취소 되었습니다.";
                        SetMessageStr(str);
                        ShowMessageDlg();
                    }
                    for(int i=1; i<CurrentMaxRow; i++)
						LotInfoGrid->Cells[1][i] = "";
                    CurrentMaxRow = 1;
                }
            }
        }
        else
        {
			EnableButtons(false);
			if (CureStop())
            {
                // 20180321 - FTP (OFILM MES) - jhlim
				if( GetManager()->UseAutomation == true &&
					GetManager()->AutomationKind == OFILM_MES )
				{
                    // Init
					Chamber->FTPLotInfo.TotalLotCount = 0;
                    for(int j=0; j<FTP_LOT_COUNT; j++)
                    {
						Chamber->FTPLotInfo.LotID[j] = "";
						Chamber->FTPLotInfo.DeviceType[j] = "";
					}
				}

				if( GetManager()->UseAutomation == true &&
					GetManager()->AutomationKind == HANA_CIM )
				{
					Chamber->HanaStopStatus();
				}
            }
            EnableButtons();
        }
	}

	DoAutoFocus();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamber::LotInfoButtonClick(TObject *Sender)
{
	String str = "Lot ID Button Clicked.";
	GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);

	// 20180321 - FTP (OFILM MES) - jhlim
//	if( GetManager()->UseAutomation == true &&
//		GetManager()->AutomationKind == OFILM_MES )
//	{
//		boost::shared_ptr<TFTPLotInfoForm> FTPLotInfoForm(new TFTPLotInfoForm(Chamber->ChamberID, this) );
//		if( FTPLotInfoForm->ShowModal() == mrCancel )
//		{
//			return;
//		}
//	}
//	else
//	{
	GetManager()->LotInfoClickChamber = Chamber->ChamberID;

	if( GetManager()->UseAutomation == true)
	{
		if (GetManager()->AutomationKind == OFILM_MES)
		{
			boost::shared_ptr<TFTPLotInfoForm> FTPLotInfoForm(new TFTPLotInfoForm(Chamber->ChamberID, this) );
			if( FTPLotInfoForm->ShowModal() == mrCancel )
			{
				return;
			}
		}
		else if( GetManager()->AutomationKind == BARUN_MES &&
			Chamber->UseManualMode == false )
		{
			AnsiString user = UserIDEdit->Text;
			if( IsValidOperatorID(user) == false )
			{
				str = L"사번을 정확히 입력 하십시오.";
				SetMessageStr(str);
				ShowMessageDlg();
				UserIDEdit->Text = "";
				if( this->Showing &&
					UserIDEdit->Visible )
					UserIDEdit->SetFocus();
				return;
			}

			Chamber->OperatorID = user;
		}
		/////////////////////////////////////////
		else if (GetManager()->AutomationKind == SECS_GEM_COWELL)
		{
			if (Chamber->Step == STEP_IDLE )
			{
				if (RecipePanel->Visible == false)
				{
					if (Chamber->RecipeMaintain == false)
					{
						RecipeComboBox->ItemIndex = -1;
						InitLotInfoGrid();
					}
					IsShowRecipe = true;
					ShowLotInfo();

					DoAutoFocus();
					return;
				}
			}


			if (Chamber->Recipe.RecipeName.IsEmpty())
			{
				SetMessageStr(GetResStr("Warning", "NoSelectedRecipe"));
				ShowMessageDlg();
				return;
			}
			if (GemDlg->IsGemConnected() == false || GemDlg->IsOnlineRemote() == false)
			{
				SetMessageStr(GetResStr("Warning", "GemNotConnected"));
				ShowMessageDlg();
				return;
			}
	//				if (Chamber->ValidationSuccess == false)
	//				{
			ShowValidationForm(Chamber->ChamberID);
	//				}
		}
		/////////////////////////////////////////
		else if (GetManager()->AutomationKind == SECS_GEM_SCK)
		{
			if (Chamber->Recipe.RecipeName.IsEmpty())
			{
				SetMessageStr(GetResStr("Warning", "NoSelectedRecipe"));
				ShowMessageDlg();
				return;
			}
			if (EzNetDlg[Chamber->ChamberID] == nullptr ||
				EzNetDlg[Chamber->ChamberID]->IsGemConnected() == false ||
				EzNetDlg[Chamber->ChamberID]->IsOnlineRemote() == false)
			{
				SetMessageStr(GetResStr("Warning", "GemNotConnected"));
				ShowMessageDlg();
				return;
			}
			if (Chamber->ValidationSuccess == false)
			{
				/*선택된 챔버 이외 다른 챔버의 도어가 열려있는 경우 알람*/
				for (int i = 0; i < GetManager()->ChamberCount; i++)
				{
					if (i != Chamber->ChamberID)
					{
						NormalOvenChamber *chamber = GetChamber(i);
						if (chamber != nullptr)
						{
							if (chamber->IsDoorClosed() == false)
							{
								str.sprintf(L"[CHAMBER %c] ", i + 'A');
								str += GetResStr("Warning", "DoorOpened");
								SetMessageStr(str);
								ShowMessageDlg();
								return;
							}
						}
					}
				}
				ShowJSCKValidationForm(Chamber->ChamberID);
				return;
			}
		}
		/////////////////////////////////////////
		IsShowRecipe = !IsShowRecipe;
		ShowLotInfo();
		DoAutoFocus();
	}
	else
	{
		  IsShowRecipe = !IsShowRecipe;
		  ShowLotInfo();
		  DoAutoFocus();
	}
//	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::SetLotGridFocus(int col, int row)
{
	try
	{
		if( this->Showing &&
			LotInfoGrid->Visible &&
			LotInfoGrid->Enabled )
		{
			LotInfoGrid->Col = col;
			LotInfoGrid->Row = row;
			LotInfoGrid->SetFocus();
		}
	}
	catch(...)
	{
		return;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::ShowLotInfo()
{
	if( GetManager()->ChamberCount == 4 &&
		FmJob->ShowDetailMode == false )
	{
		RecipePanel->Visible = false;
		LotInfoPanel->Visible = false;
		return;
	}

	if( IsShowRecipe )
	{
		RecipePanel->Visible = true;
		LotInfoPanel->Visible = false;

		LOTINFO lotInfo;
		CheckLotInfo(lotInfo);
	}
	else
	{
		if (GetManager()->UseAutomation)
		{
			if (Chamber == nullptr)
				return;

			String strValue;

			if (GetManager()->AutomationKind == SECS_GEM_COWELL)
			{

				for (int i = 0; i < LotInfoGrid->RowCount; i++)
				{
					LotInfoGrid->Cells[1][i + 1] = "";
				}

				if ( Chamber->ValidationSuccess  == true )
				{
					for (int i = 0; i < Chamber->LotIDList->Count; i++)
					{
						strValue = Chamber->LotIDList->Strings[i];
						LotInfoGrid->Cells[1][i + 1] = strValue;
						Chamber->CowellLotInfo.LotID[i] = strValue;  //hcw
						if (strValue != "")
						{
							Chamber->CowellLotInfo.TotalLotCount++;
                        }
					}
				}
				else
				{
					Chamber->CowellLotInfoClear();
				}
			}

			if (GetManager()->AutomationKind == SECS_GEM_SCK)
			{
                for (int i = 0; i < LotInfoGrid->RowCount; i++)
				{
					LotInfoGrid->Cells[1][i + 1] = "";
				}
				for (int i = 0; i < Chamber->DeviceIDList->Count; i++)
				{
					strValue = Chamber->DeviceIDList->Strings[i];
					LotInfoGrid->Cells[1][i + 1] = strValue;
				}
			}
			else
			{
                for (int i = 0; i < Chamber->LotIDList->Count; i++)
				{
					LotInfoGrid->Cells[1][i + 1] = Chamber->LotIDList->Strings[i];
                }
			}
		}

		LotInfoPanel->Visible = true;
		RecipePanel->Visible = false;
		SetLotGridFocus(1, CurrentMaxRow);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::InitLotInfoGrid()
{
	LotInfoGrid->Clear();

	LotInfoGrid->ColWidths[0] = 25;
	LotInfoGrid->ColWidths[1] = 170;
//	LotInfoGrid->ColWidths[2] = 50;

	LotInfoGrid->Cells[0][0] = "No.";
	LotInfoGrid->Cells[1][0] = "Lot ID";
//	LotInfoGrid->Cells[2][0] = "Q'ty";
	for(int i=0; i<LOT_COUNT; i++)
	{
		LotInfoGrid->Cells[0][i+1] = IntToStr(i+1);
	}

	CurrentMaxRow = 1;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::FillLotInfoGrid(LOTINFO lotInfo)
{
	for(int i=0; i<lotInfo.TotalLotCount; i++)
	{
		LotInfoGrid->Cells[1][i+1] = lotInfo.LotID[i];
//		LotInfoGrid->Cells[2][i+1] = IntToStr(lotInfo.LotQuantity[i]);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::LotInfoGridCanEditCell(TObject *Sender, int ARow, int ACol,
          bool &CanEdit)
{
	if( Chamber == nullptr )
		return;

	if (Chamber->ChamberID != GetManager()->LotInfoClickChamber)
	{
		CanEdit = false;
		return;
	}

	if( Chamber->Step != STEP_IDLE )
	{
		CanEdit = false;
		return;
	}

	if (GetManager()->AutomationKind == SECS_GEM_COWELL &&
		GemDlg->IsGemConnected() == true && GemDlg->IsOnlineRemote() == true)
	{
		CanEdit = false;
		return;
	}

	if( ARow != 0 &&
		ACol != 0 )
	{
		if( ARow < CurrentMaxRow )
			CanEdit = true;
		else
		if( ARow == CurrentMaxRow )
		{
			if( ACol == 1)
				CanEdit = true;
			else
			{
				// Lot ID 란이 비어 있으면 비활성
				if( LotInfoGrid->Cells[1][ARow].IsEmpty() )
					CanEdit = false;
				else
					CanEdit = true;
			}
		}
		else
            CanEdit = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamber::LotInfoGridGetEditorType(TObject *Sender, int ACol, int ARow,
          TEditorType &AEditor)
{
	if( ARow != 0 )
	{
		if( ACol == 1 )
			AEditor = edNormal;
		else
		if( ACol == 2 )
			AEditor = edPositiveNumeric;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamber::LotInfoGridGetAlignment(TObject *Sender, int ARow, int ACol,
          TAlignment &HAlign, TVAlignment &VAlign)
{
	HAlign = taCenter;
	VAlign = Advobj::vtaCenter;
}
//---------------------------------------------------------------------------

AnsiString gLotID = "";
int gACol, gARow;
void __fastcall TFrmChamber::LotInfoGridEditCellDone(TObject *Sender, int ACol, int ARow)
{
	String str;

	// 20180713 - 문자 공백 지우기 - jhlim
	TReplaceFlags Flags = TReplaceFlags() << rfReplaceAll;

	// LOT ID 입력
	if( ARow >= 1 &&
		ACol == 1 )
	{
		AnsiString lotId = LotInfoGrid->Cells[ACol][ARow];

		// 20180713 - 문자 공백 지우기 - jhlim
		lotId = StringReplace(lotId, " ", "", Flags);
		LotInfoGrid->Cells[ACol][ARow] = lotId;

		if( lotId.IsEmpty() )
		{
			SetLotGridFocus(1, CurrentMaxRow);
			return;
		}

		if( GetManager()->UseAutomation )
		{
			if( GetManager()->AutomationKind == BARUN_MES &&
				Chamber->UseManualMode == false &&
				BarunMESDlg->CimParam.OnlineMode == true &&
				Chamber->BarunManager  )
			{
				BARUN_RESULT res;

				if( IsLotExist(lotId, ARow) )
				{
					str.sprintf(L"중복된 LOT입니다.");
					SetMessageStr(str);
					ShowMessageDlg();
					LotInfoGrid->Cells[ACol][ARow] = "";
					return;
				}

				for(int i=0; i<GetManager()->ChamberCount; i++)
				{
					if( Chamber->ChamberID != i &&
						FmJob->ChamberFrame[i]->IsLotExist(lotId) )
					{
						str.sprintf(L"타 챔버에서 사용 중인 중복된 LOT입니다.");
						SetMessageStr(str);
						ShowMessageDlg();
						LotInfoGrid->Cells[ACol][ARow] = "";
						return;
					}
				}

				res = Chamber->BarunManager->SendLotScan(UserIDEdit->Text, lotId);
				if( res.Result == false )
				{
					if( res.ErrorCode == ERRCODE_WRONG_RECIPE )
					{
						str.sprintf(L"수신된 레시피가 이전에 수신된 레시피와 다릅니다.\r\n현재 LOT만 삭제하시려면 [%s]을\r\n전체 LOT를 삭제 하시려면 [%s]를 눌러 주십시오.",
									GetResStr("Common", "OK").c_str(),
									GetResStr("Common", "Cancel").c_str());
						SetMessageStr(str);
						if( ShowQuestionDlg() )
						{
							res = Chamber->BarunManager->SendLotScanDelete(UserIDEdit->Text, lotId);
							if( res.Result == false )
							{
								str.sprintf(L"에러 코드가 수신되었습니다.\r\n에러코드 : %d\r\n에러내용 : %s",
											res.ErrorCode,
											res.Error.c_str());
								SetMessageStr(str);
								ShowMessageDlg();
							}
							LotInfoGrid->Cells[ACol][ARow] = "";
						}
						else
						{
							res = Chamber->BarunManager->SendLotScanCancel(UserIDEdit->Text);
							if( res.Result == false )
							{
								str.sprintf(L"에러 코드가 수신되었습니다.\r\n에러코드 : %d\r\n에러내용 : %s",
											res.ErrorCode,
											res.Error.c_str());
								SetMessageStr(str);
								ShowMessageDlg();
							}
							for(int i=1; i<CurrentMaxRow; i++)
								LotInfoGrid->Cells[ACol][i] = "";
							CurrentMaxRow = 1;
							Chamber->Recipe.RecipeName = "";
						}
					}
					else
					{
						str.sprintf(L"에러 코드가 수신되었습니다.\r\n에러코드 : %d\r\n에러내용 : %s",
									res.ErrorCode,
									res.Error.c_str());
						SetMessageStr(str);
						ShowMessageDlg();
						LotInfoGrid->Cells[ACol][ARow] = "";
					}
				}
				else
				{
					if( CurrentMaxRow == ARow )
						CurrentMaxRow = ARow + 1;

					Chamber->Recipe.RecipeName = res.ReceivedRecipeName;
					LoadTempPtn(Chamber->ChamberID, 1, Chamber->Recipe.TempPtn, true);
					FillRecipeGrid(Chamber->Recipe.TempPtn, res.ReceivedRecipeName);
					InitPtnChart();

					Chamber->ApplyDataToController();
				}
			}
			else
			{
				if( CurrentMaxRow == ARow )
					CurrentMaxRow = ARow + 1;
			}
		}
		else
		{
			if( CurrentMaxRow == ARow )
				CurrentMaxRow = ARow + 1;
		}
		SetLotGridFocus(1, CurrentMaxRow);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::LotInfoGridGetCellColor(TObject *Sender, int ARow, int ACol,
          TGridDrawState AState, TBrush *ABrush, TFont *AFont)
{
	if( Chamber == nullptr )
		return;

	if( Chamber->Step != STEP_IDLE )
	{
		ABrush->Color = clInactiveCaption;
		return;
	}

	if( ARow != 0 &&
		ACol != 0 )
	{
		if( ARow < CurrentMaxRow )
			ABrush->Color = clWhite;
		else
		if( ARow == CurrentMaxRow )
		{
			if( ACol == 1)
				ABrush->Color = clWhite;
			else
			{
				// Lot ID 란이 비어 있으면 비활성
				if( LotInfoGrid->Cells[1][ARow].IsEmpty() )
					ABrush->Color = clInactiveCaption;
				else
					ABrush->Color = clWhite;
			}
		}
		else
			ABrush->Color = clInactiveCaption;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TFrmChamber::CheckLotInfo(LOTINFO &lotInfo)
{
	bool isPrevLotIdEmpty = false;
	bool isPrevLotCountEmpty = false;
	int idCount = 0;
//	int countCount = 0;

	lotInfo.TotalLotCount = 0;
	for(int i=0; i<LOT_COUNT; i++)
	{
		lotInfo.LotID[i] = "";
		lotInfo.LotQuantity[i] = 0;
	}

	// 열 중간에 빈 라인이 있는지 검사
	for(int i=1; i<=LOT_COUNT; i++)
	{
		// Lot ID 체크
		if( isPrevLotIdEmpty == true &&
			LotInfoGrid->Cells[1][i].IsEmpty() == false )
		{
			SetMessageStr(GetResStr("Warning", "EmptyItemExist"));
			ShowMessageDlg();
			return false;
		}

		if( LotInfoGrid->Cells[1][i].IsEmpty() )
			isPrevLotIdEmpty = true;
		else
		{
			isPrevLotIdEmpty = false;
			idCount++;
		}

		// Lot Count 체크
		if( isPrevLotCountEmpty == true &&
			LotInfoGrid->Cells[2][i].IsEmpty() == false )
		{
			SetMessageStr(GetResStr("Warning", "EmptyItemExist"));
			ShowMessageDlg();
			return false;
		}
/*		if( LotInfoGrid->Cells[2][i].IsEmpty() )
			isPrevLotCountEmpty = true;
		else
		{
			isPrevLotCountEmpty = false;
			countCount++;
		}
		if( isPrevLotCountEmpty == false &&
			LotInfoGrid->Cells[2][i].ToIntDef(0) == 0 )
		{
			SetMessageStr(GetResStr("Warning", "MustNotZeroCount"));
			ShowMessageDlg();
			return false;
		} */
	}

/*	if( idCount != countCount )
	{
		SetMessageStr(GetResStr("Warning", "NotSameCount"));
		ShowMessageDlg();
		return false;
	}*/

	if( idCount == 0 )//&&
//		countCount == 0 )
		return true;

	lotInfo.TotalLotCount = idCount;
	for(int i=1; i<=idCount; i++)
	{
		lotInfo.LotID[i-1] = LotInfoGrid->Cells[1][i];
//		lotInfo.LotQuantity[i-1] = LotInfoGrid->Cells[2][i].ToInt();
	}

	return true;
}

void __fastcall TFrmChamber::ChangeViewModeButtonClick(TObject *Sender)
{
	FmJob->CurrentChamber = Chamber->ChamberID;
	FmJob->ShowDetailMode = !FmJob->ShowDetailMode;
	FmJob->ArrangeChamber();

	DoAutoFocus();
}
//---------------------------------------------------------------------------
bool __fastcall TFrmChamber::IsLotExist(String lotid, int row/*=-1"*/)
{
	String lot;

	for(int i=1; i<=LOT_COUNT; i++)
	{
		if( row > 0 &&
			row == i )
			continue;

		lot = LotInfoGrid->Cells[1][i];
		if( lot == lotid )
			return true;
	}

	return false;
}

void __fastcall TFrmChamber::UserIDEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if( Key == 0x0D )
	{
		SwitchToLotInfoPanel->Enabled = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamber::SwitchToLotInfoPanelTimer(TObject *Sender)
{
	SwitchToLotInfoPanel->Enabled = false;
	if( IsValidOperatorID(UserIDEdit->Text) )
	{
		Chamber->OperatorID = UserIDEdit->Text;
		IsShowRecipe = false;
		ShowLotInfo();
	}
	else
	{
		String str;
		str = L"사번을 정확히 입력 하십시오.";
		SetMessageStr(str);
		ShowMessageDlg();
		UserIDEdit->Text = "";
		if( this->Showing &&
			UserIDEdit->Visible )
			UserIDEdit->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::DoAutoFocus()
{
	if( GetManager()->UseAutomation )
	{
		if( GetManager()->AutomationKind == BARUN_MES &&
			Chamber->UseManualMode == false &&
			BarunMESDlg->CimParam.OnlineMode == true &&
			Chamber->BarunManager  )
		{
			if( IsShowRecipe )
			{
				if( this->Showing &&
					UserIDEdit->Visible )
					UserIDEdit->SetFocus();
			}
			else
				SetLotGridFocus(1, CurrentMaxRow);
		}
	}
}
//---------------------------------------------------------------------------

// 20180209 - FTP (OFILM MES) - jhlim
bool __fastcall TFrmChamber::WriteTempData(AnsiString sFileName, AnsiString sTime)
{
	if( Chamber == nullptr )
		return false;

	FILE * pFile;
	pFile = fopen(sFileName.c_str(), "a+t");
	if(pFile == nullptr)
		return false;

	AnsiString data, LotInfo;

	if( Chamber->Step > STEP_IDLE
		&& Chamber->Step < STEP_DONE )
	{
		LotInfo = "";
		LotInfo = Chamber->FTPLotInfo.ToString_FTP();
		if(LotInfo != "")
		{
			data.sprintf("%s,%.1f,%s\n"
				, sTime.c_str()
				, Chamber->PV
				, LotInfo.c_str());
		}
		else
		{
			data.sprintf("%s,%.1f, , ,\n"
				, sTime.c_str()
				, Chamber->PV);
		}
	}
	else
	{
		data.sprintf("%s,%.1f, , ,\n"
			, sTime.c_str()
			, Chamber->PV);
	}

	std::fwrite(data.c_str(), data.Length(), 1, pFile);
	fclose(pFile);

	return true;
}
//---------------------------------------------------------------------------
// 20191216 - SCK MES - jhlim
bool __fastcall TFrmChamber::DoSCKMESProc()
{
	// 제목 : yyyymmddhhmiss_LOTID
	// 내용 : LOTID/공정코드/레서피명/장비명

	bool bReturn = false;
	if( Chamber == nullptr )
		return bReturn;

	String str;
	// MES -> Oven Lot Information File Check
	if( Chamber->CheckExistFile_MESLotInfo() == true )
	{
		// Lot Information File Read
		if (Chamber->ReadFile_MESLotInfo() == true)
		{
			// Recipe Valid Check
			if( CheckRecipe_MESLotInfo() == true )
			{
				// Recipe Select
				SelectRecipe_MESLotInfo(Chamber->stSckMesInfo.RecipeName);
				// LotID Input
				InputLotID_MESLotInfo(Chamber->stSckMesInfo.LotID);
			}
			else
			{
				// Show Alarm MSG
				str.sprintf(L"[MES] Cure Start Fail.\n\rRecipeInfo not exist.");
				SetMessageStr(str);
				return bReturn;
			}
		}
		else
		{
			// Show Alarm MSG
			str.sprintf(L"[MES] Cure Start Fail.\n\rInvalid Lot Info.");
			SetMessageStr(str);
			return bReturn;
		}
	}
	else
	{
		// Show Alarm MSG
		str.sprintf(L"[MES] Cure Start Fail.\n\rLotInfo file does not exist.");
		SetMessageStr(str);
		return bReturn;
	}

	bReturn = true;
	return bReturn;
}

bool __fastcall TFrmChamber::CheckRecipe_MESLotInfo()
{
	bool bReturn = false;
	if( Chamber == nullptr )
		return bReturn;

	if(Chamber->stSckMesInfo.RecipeName != "")
	{
		String recipe;
		int nItemCount = RecipeComboBox->Items->Count;
		for(int i=0; i<nItemCount; i++)
		{
			recipe = RecipeComboBox->Items->Strings[i];
			if(recipe == Chamber->stSckMesInfo.RecipeName)
			{
				bReturn = true;
				break;
			}
		}
	}
	return bReturn;
}

void __fastcall TFrmChamber::SelectRecipe_MESLotInfo(String recipe)
{
	if( recipe == "" )
		return;

	RecipeComboBox->Enabled = false;

	String rec;
	int nItemCount = RecipeComboBox->Items->Count;
	for(int i=0; i<nItemCount; i++)
	{
		rec = RecipeComboBox->Items->Strings[i];
		if(rec == recipe)
		{
			RecipeComboBox->ItemIndex = i;
			SelectRecipe(rec);
			break;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::InputLotID_MESLotInfo(String LotID)
{
	if( LotID == "" )
		return;

	InitLotInfoGrid();

	LOTINFO lotInfo;
	lotInfo.TotalLotCount = 1;
	lotInfo.LotID[0] = LotID;
	lotInfo.LotQuantity[0] = 0;

	FillLotInfoGrid(lotInfo);
}
//---------------------------------------------------------------------------
bool __fastcall TFrmChamber::SaveCureDoneInfo_MESLotInfo(int stopType)
{
	bool bReturn = false;
	if( Chamber == nullptr )
		return bReturn;

	// File Path
	AnsiString strPath, strFileName, data;
	strFileName = FormatDateTime("yyyymmdd", Now());
	strPath = GetSCKMESWorkDoneFileFolder() + strFileName + ".txt";
    // File Open
	FILE * pFile;
	pFile = fopen(strPath.c_str(), "a+t");
	if(pFile == nullptr)
		return bReturn;

	// data
	data.sprintf("%S/%S/%S/%S/Chamber_%c/"
		, Chamber->stSckMesInfo.LotID.c_str(), Chamber->stSckMesInfo.ProductCode.c_str(), Chamber->stSckMesInfo.RecipeName.c_str()
		, Chamber->stSckMesInfo.EquipName.c_str(), 'A' + Chamber->ChamberID);
	// stop type
	if(stopType == NORMAL_STOP)
		data += "WORK_DONE\n";
	else
	if(stopType == FORCED_STOP)
		data += "FORCED_STOP\n";
	else
	if(stopType == LOWPRESSURE_STOP)
		data += "LOWPRESSURE_STOP\n";
	else
	if(stopType == EMERGENCY_STOP)
		data += "EMERGENCY_STOP\n";
	else
	if(stopType == MDCOOLFAIL_STOP)
		data += "MDCOOLFAIL_STOP\n";
	else
	if(stopType == O2FAIL_STOP)
		data += "O2FAIL_STOP\n";
	else
	if(stopType == VACUUMLEVEL_STOP)
		data += "VACUUMLEVEL_STOP\n";
	// File Write
	std::fwrite(data.c_str(), data.Length(), 1, pFile);
	fclose(pFile);
	// Data Initial
	Chamber->InitData_MESLotInfo();
	bReturn = true;

	return bReturn;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::ChamberStatusPanelDblClick(TObject *Sender)
{
	TempGraph->View3D = !TempGraph->View3D;
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamber::LocalButtonClick(TObject *Sender)
{
	String str;
	str = "LOCAL Button Clicked.";
	GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);

	if (Chamber->Step != STEP_IDLE)
	{
		str = GetResStr("Warning", "CuringJob");
		SetMessageStr(str);
		ShowMessageDlg();
		return;
	}

	if( Chamber->IsDoorClosed() == false )
	{
		str = GetResStr("Warning", "DoorOpened");
		SetMessageStr(str);
		ShowMessageDlg();
		return;
	}

	if (GetManager()->AutomationKind != SECS_GEM_SCK)
	{
		Chamber->DoNotChangeStep = true;
	}
	Chamber->Recipe.JobEnd = true;
	WaitSecond(2);
	Chamber->Step = STEP_LOCAL;
	Chamber->DoIdleCoolingDisable();
	if (Chamber->MainTempController->Local() == false)
	{
		str = GetResStr("Message", "Retry");
		SetMessageStr(str);
		ShowMessageDlg();
		return;
	}

	WaitSecond(2);
	Chamber->DoNotChangeStep = false;

	str = "LOCAL mode started.";
	SetMessageStr(str);
	ShowMessageDlg();
}
//----------------------------------------------------------
void __fastcall TFrmChamber::SafetyRelayResetButtonClick(TObject *Sender)
{
	if (Chamber == nullptr)
	{
		return;
	}

	String str = "Safety Reset Button Clicked.";
	GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);

	Chamber->ResetSafetyOn();
	Sleep(2000);
	Chamber->ResetSafetyRelay();
}
//---------------------------------------------------------

//void __fastcall TFrmChamber::RefreshSafetyButton()  //2023-04-17 restore
//{
//	if (Chamber == nullptr)
//	{
//		return;
//	}
//
//	if (GetManager()->UseSafetyReset == false)
//	{
//		return;
//	}
//
//	if (Chamber->DIO[I_SAFETY_STATUS_ALARM_DETECTED]->IsOff())
//	{
//		SafetyRelayResetButton->Visible = false;
//		LocalButton->Width = MachineStatusPanel->Width;
//		if (Chamber->DIO[O_SAFETY_RESET_ON]->IsOn())
//		{
//			Chamber->DIO[O_SAFETY_RESET_ON]->Off();
//		}
//		return;
//	}
//
//	if (Chamber->DIO[I_SAFETY_STATUS_ALARM_DETECTED]->IsOn())
//	{
//		if (SafetyRelayResetButton->Visible == false)
//		{
//			SafetyRelayResetButton->Visible = true;
//			LocalButton->Width = MachineStatusPanel->Width/2;
//			SafetyRelayResetButton->Width = MachineStatusPanel->Width/2;
//		}
//	}
//	else
//	{
//		if (SafetyRelayResetButton->Visible == true)
//		{
//			SafetyRelayResetButton->Visible = false;
//			LocalButton->Width = MachineStatusPanel->Width;
//			if (Chamber->DIO[O_SAFETY_RESET_ON]->IsOn())
//			{
//				Chamber->DIO[O_SAFETY_RESET_ON]->Off();
//			}
//		}
//	}
//	if (SafetyRelayResetButton->Visible == true)
//	{
//		TColor clr = Chamber->IsBlink ? clRed : clSkyBlue;
//		if (SafetyRelayResetButton->Appearance->Color != clr)
//		{
//			SafetyRelayResetButton->Appearance->Color = clr;
//			SafetyRelayResetButton->Appearance->ColorMirror = clr;
//		}
//	}
//}


bool __fastcall TFrmChamber::SelectReceiveHanaRecipe()
{
	if( Chamber == nullptr )
		return false;

	int ChamberNo = Chamber->ChamberID;
	int nSegCount;
	int nPtnNo;

	OVEN_RECIPE recipeInfo;

	Chamber->HanOvenCim->GetHanaReceivePtn(ptn);

	if( GetManager()->UseAutomation)
	{
	   if (GetManager()->AutomationKind == HANA_CIM && GetManager()->UseAutomation == true)
		{
			nPtnNo = ptn.PtnNo;
			nSegCount = ptn.Stc.SegCount;

			recipeInfo.ChamberNo = ChamberNo;
			recipeInfo.Recipe = L"HanaRecipe";
			recipeInfo.PtnNo = nPtnNo;
			recipeInfo.CureTemp = ptn.MaxTemp;
			recipeInfo.CureTime = ptn.TotalTime;

			String str;
			str.sprintf(L"HANA OVEN SelectReceiveHanaRecipe CHM[%d] : PTN NO[%d]", ChamberNo, recipeInfo.PtnNo);
			SaveDebugLog(str);

			HanaRecipeInfoUpdate(Chamber->ChamberID, recipeInfo.Recipe, ptn);

			FillRecipeGrid(recipeInfo);
			InitPtnChart();

			Chamber->ApplyDataToController();

			str.sprintf(L"The recipe [%s] is selected.", recipeInfo.Recipe.c_str());
			GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);
			return true;
		}
	}

	return false;
}

void __fastcall TFrmChamber::HanaRecipeInfoUpdate(int ChamberNo, String Recip, PTN ptn)
{
	Chamber->Recipe.RecipeName = Recip;
	Chamber->Recipe.TempPtn.PtnNo = ptn.PtnNo;
	Chamber->Recipe.TempPtn.MaxTemp = ptn.MaxTemp;
	Chamber->Recipe.TempPtn.TotalTime = ptn.TotalTime;
	Chamber->Recipe.TempPtn.Stc.SegCount = ptn.Stc.SegCount;
   //	Chamber->ChamberID = ChamberNo;
	Chamber->Recipe.IsGemRecipe = false;

	for (int i = 0; i < ptn.Stc.SegCount; i++)
	{
		Chamber->Recipe.TempPtn.Seg[i].Tsp =  ptn.Seg[i].Tsp;
		Chamber->Recipe.TempPtn.Seg[i].Time = ptn.Seg[i].Time;
		Chamber->Recipe.TempPtn.Seg[i].Pid = 1;

		if (i < (ptn.Stc.SegCount -1))
		{
			Chamber->Recipe.TempPtn.Seg[i].Jc = 0;
			Chamber->Recipe.TempPtn.Seg[i].EvKind[0] = 0;
			Chamber->Recipe.TempPtn.Seg[i].OnType[0] = 0;
			Chamber->Recipe.TempPtn.Seg[i].OffPe[0] = 0;
		}
		else
		{
			if (ptn.Seg[ptn.Stc.SegCount -1].Tsp < ptn.Seg[ptn.Stc.SegCount -2].Tsp)
			{
				Chamber->Recipe.TempPtn.Seg[i].Jc = 2; //2
				Chamber->Recipe.TempPtn.Seg[i].EvKind[0] = ET_COOLING;
				Chamber->Recipe.TempPtn.Seg[i].OnType[0] = 0;
				Chamber->Recipe.TempPtn.Seg[i].OffPe[0] = ptn.Seg[i].Time;
			}
			else
			{
				Chamber->Recipe.TempPtn.Seg[i].Jc = 0;
				Chamber->Recipe.TempPtn.Seg[i].EvKind[0] = 0;
				Chamber->Recipe.TempPtn.Seg[i].OnType[0] = 0;
				Chamber->Recipe.TempPtn.Seg[i].OffPe[0] = 0;

			}
		}
	}

	String str;
	str.sprintf(L"HANA OVEN HanaRecipeInfoUpdate CHM[%d] : SEG CNT[%d], JC[%d]", ChamberNo, ptn.Stc.SegCount,
	Chamber->Recipe.TempPtn.Seg[ptn.Stc.SegCount -1].Jc);
	SaveDebugLog(str);
}

bool __fastcall TFrmChamber::HanaReceiveCureStart()
{
	if( Chamber == nullptr )
		return false;

	if( GetManager()->UseAutomation && GetManager()->AutomationKind == HANA_CIM)
	{
		String str;
		bool byRemote;
		byRemote = true;
		LOTINFO lotInfo;

		if (Chamber->Step == STEP_DONE)
		{
			SetMessageStr(GetResStr("Warning", "ExistProduct"));
			return false;
		}

		if( Chamber->Step != STEP_IDLE && Chamber->Step != STEP_LOCAL)
		{
			SetMessageStr(GetResStr("Warning", "CuringJob"));
			return false;
		}

		if( (Chamber->Recipe.RecipeName.IsEmpty() ||
			 RecipeComboBox->ItemIndex < 0) &&
			 byRemote == false )
		{
			SetMessageStr(GetResStr("Warning", "NoSelectedRecipe"));
			return false;
		}

		if( GetManager()->LoginLevel < LV_DEVELOPER &&
			Chamber->GetAlarmCount() > 0 )
		{
			SetMessageStr(GetResStr("Warning", "CantRunWhileAlarm"));
			return false;
		}

		if( GetManager()->UseMagazineDetector )
		{
			if( Chamber->IsMagazineDetected() == false )
			{
				SetMessageStr(GetResStr("Question", "MangazineNotDetected"));
				if( ShowQuestionDlg() == false )
					return false;
			}
		}
		if( Chamber->IsDoorClosed() == false )
		{
			SetMessageStr(GetResStr("Warning", "DoorOpened"));
			return false;
		}

		str.sprintf(L"<P align=\"center\">RECIPE DOWNLOADING...%c</P>", Chamber->ChamberID + 'A');
		ChamberPanel->Caption->Text = str;

		ProgressBar->Position = 0;
		ProgressBar->Maximum = Chamber->Recipe.TempPtn.Stc.SegCount + 3;
		ProgressBar->Visible = false;

		AnsiString failMsg;
		if( Chamber->DoCureStart(failMsg, true) )
		{
			ProgressBar->Visible = false;
			ProgressBar->Position = 0;

			OVEN_WORK work;
			work.StartTime = Chamber->Recipe.StartTime;
			work.Recipe = Chamber->Recipe.RecipeName;
			work.PtnNo = Chamber->Recipe.TempPtn.PtnNo;
			work.CureTemp = Chamber->Recipe.TempPtn.MaxTemp;
			work.CureTime = Chamber->Recipe.TempPtn.TotalTime;
			work.Remote = byRemote;
			if( byRemote )
				work.UserID = "REMOTE";
			else
				work.UserID = GetManager()->UserID;
			work.ChamberNo = Chamber->ChamberID;
			work.Filename.sprintf(L"%c_", 'A' + work.ChamberNo);
			work.Filename += work.StartTime.FormatString("yyyy_mm_dd_hh_nn_ss");
			GetOvenDB()->SaveWork(work, lotInfo);

			InitChart(Chamber->Recipe.StartTime,
					  Chamber->Recipe.TempPtn.TotalTime,
					  Chamber->Recipe.TempPtn.MaxTemp);
		}
		else
		{
			EnableButtons();
			DoAutoFocus();
			AutoStartFailed = false;
			str.sprintf(L"<P align=\"center\">CH. %c</P>", Chamber->ChamberID + 'A');
			ChamberPanel->Caption->Text = str;
			return false;
		}
		EnableButtons();
		DoAutoFocus();
		AutoStartFailed = false;
		str.sprintf(L"<P align=\"center\">CH. %c</P>", Chamber->ChamberID + 'A');
		ChamberPanel->Caption->Text = str;
		return true;
	}
	return false;
}

 bool __fastcall TFrmChamber::SelectRecipe_FujitsuLotInfo(String recipe)
{
	if( recipe == "" )
		return false;

	RecipeComboBox->Enabled = false;

	String rec;
	int nItemCount = RecipeComboBox->Items->Count;
	for(int i=0; i<nItemCount; i++)
	{
		rec = RecipeComboBox->Items->Strings[i];
		if(rec == recipe)
		{
			RecipeComboBox->ItemIndex = i;
			if (SelectRecipe(rec)== false)
			{
				return false;
            }
			break;
		}
	}
	return true;
}

void __fastcall TFrmChamber::FujisuTCard_RecipeComboInit(bool everything)
{
	int sel=-1;

	if (everything == true)
	{
		RecipeComboBox->ItemIndex = sel;
		RecipeComboBox->Enabled = false;
		Chamber->Recipe.RecipeName = "";

		for(int i=1; i<=LOT_COUNT; i++)
			LotInfoGrid->Cells[1][i] = "";
		for(int i=0; i<=5; i++)
			RecipeInfoGrid->Cells[1][i] = "";
		PatternGraph->Series[0]->Clear();
	}
	else
	{
		RecipeComboBox->ItemIndex = sel;
		RecipeComboBox->Enabled = false;
		Chamber->Recipe.RecipeName = "";
	}
}

void __fastcall TFrmChamber::AutoModeButtonEnable(bool enable)
{
	if (enable == true)
	{
		CureStartButton->Enabled = true;
		LocalButton->Enabled = true;
	}
	else
	{
		CureStartButton->Enabled = false;
		LocalButton->Enabled = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::SCK_RMSStart()
{
	if( GetManager()->UseAutomation == true &&
		GetManager()->AutomationKind == SCK_MES )
	{
		if( Chamber->Step == STEP_IDLE )
		{
			if( Chamber->IsDoorClosed() )
			{
				// WORK폴더 작업파일과 WORKDONE폴더 작업내용 비교
				if( DoSCKMESProc() == false )
				{
					ShowMessageDlg();
				}
				else
				{
					CureStartButton->Click(); //CureStartButtonClick(Sender);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::UpdateCimStatus()
{
	String connectedStr, enabledStr, ctrlStateStr;

	if (GetManager()->AutomationKind == SECS_GEM_SCK)
	{
		GEMSTATE gemStatus;

		if (GetManager()->AutomationKind == SECS_GEM_SCK && EzNetDlg[Chamber->ChamberID] != nullptr &&
			EzNetDlg[Chamber->ChamberID] && EzNetDlg[Chamber->ChamberID]->IsGemConnected())
		{
			gemStatus = EzNetDlg[Chamber->ChamberID]->GemState;

			connectedStr = EzNetDlg[Chamber->ChamberID]->IsGemConnected() ? "ONLINE" : "OFFLINE";
			enabledStr = EzNetDlg[Chamber->ChamberID]->IsGemConnected() ? "RUNNING" : "STOPPED";
			EzNetOfflineButton->Enabled = true;

			switch(gemStatus.ControlState)
			{
			case CONTROL_EQUIPMENT_OFFLINE:
				ctrlStateStr = "OFFLINE";
				break;
			case CONTROL_ATTEMPT_ONLINE:
				ctrlStateStr = "ATTEMPT ONLINE";
				break;
			case CONTROL_HOST_OFFLINE:
				ctrlStateStr = "HOST OFFLINE";
				break;
			case CONTROL_ONLINE_LOCAL:
				ctrlStateStr = "ONLINE LOCAL";
				break;
			case CONTROL_ONLINE_REMOTE:
				ctrlStateStr = "ONLINE REMOTE";
				break;
			default:
				break;
			}

			NormalOvenChamber *chamber = GetChamber(Chamber->ChamberID);
			if (chamber == nullptr)
				return;

			if( chamber->Step == STEP_IDLE || chamber->Step == STEP_DONE )
			{
				EzNetLocalButton->Enabled = (gemStatus.ControlState == CONTROL_ONLINE_REMOTE) || (gemStatus.ControlState == CONTROL_EQUIPMENT_OFFLINE);
				EzNetRemoteButton->Enabled = (gemStatus.ControlState == CONTROL_ONLINE_LOCAL) || (gemStatus.ControlState == CONTROL_EQUIPMENT_OFFLINE);
			}
			else
			{
				EzNetLocalButton->Enabled = false;
				EzNetRemoteButton->Enabled = false;
			}

			bool remote = gemStatus.ControlState ? CONTROL_ONLINE_REMOTE : CONTROL_ONLINE_LOCAL;
			if(gemStatus.ControlState == CONTROL_ONLINE_LOCAL)
			{
				OnlineStaticText->Color = clLime;
				EnabledStaticText->Color = clWhite;
			}
			else if(gemStatus.ControlState == CONTROL_ONLINE_REMOTE)
			{
				OnlineStaticText->Color = clWhite;
				EnabledStaticText->Color = clLime;
			}
			else
			{
				OnlineStaticText->Color = clWhite;
				EnabledStaticText->Color = clWhite;
			}
		}
		else
		{
			connectedStr = "OFFLINE";
			enabledStr = "STOPPED";
			ctrlStateStr = "LOCAL";

			EzNetOfflineButton->Enabled = false;
			EzNetLocalButton->Enabled = false;
			EzNetRemoteButton->Enabled = false;

			OnlineStaticText->Color = clWhite;
			EnabledStaticText->Color = clWhite;
		}
	}
	else
	{
		connectedStr = "OFFLINE";
		enabledStr = "STOPPED";
		ctrlStateStr = "LOCAL";

		EzNetOfflineButton->Enabled = false;
		EzNetLocalButton->Enabled = false;
		EzNetRemoteButton->Enabled = false;
	}

	if (EzNetOfflineButton->Caption != connectedStr)
	{
		EzNetOfflineButton->Caption = connectedStr;
	}
	if (EnabledStaticText->Caption != enabledStr)
	{
		EnabledStaticText->Caption = enabledStr;
	}
	if (OnlineStaticText->Caption != ctrlStateStr)
	{
		OnlineStaticText->Caption = ctrlStateStr;
	}
	if (ConnectedStaticText->Caption != connectedStr)
	{
		ConnectedStaticText->Caption = connectedStr;
	}
}
//------------------------------------------------------------------
void __fastcall TFrmChamber::EzNetLocalButtonClick(TObject *Sender)
{
	if (EzNetDlg[Chamber->ChamberID] == nullptr)
	{
		return;
	}

	EzNetDlg[Chamber->ChamberID]->GoOnlineLocal();
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::EzNetRemoteButtonClick(TObject *Sender)
{
	if (EzNetDlg[Chamber->ChamberID] == nullptr)
	{
		return;
	}

	EzNetDlg[Chamber->ChamberID]->GoOnlineRemote();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamber::EnabledStaticTextDblClick(TObject *Sender)
{
	if (GetManager()->AutomationKind == SECS_GEM_SCK)
	{
		String gemCaption;
		HWND hwnd;

		gemCaption.sprintf(L"GEM [Chamber %c]", 'A' + Chamber->ChamberID);
		hwnd = FindWindow(nullptr, gemCaption.c_str());
		if (hwnd)
		{
			SendMessage(hwnd, WM_SHOW_GEM_FORM, 0, 0);
        }
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamber::UpdateUITimer(TObject *Sender)
{
	UpdateUI->Enabled = false;

	if (GetManager()->UseAutomation == true &&
		GetManager()->AutomationKind == SECS_GEM_SCK)
	{
        GEMSTATE gemStatus;
		gemStatus = EzNetDlg[Chamber->ChamberID]->GemState;

		if (Chamber->Step == STEP_IDLE &&
		(gemStatus.ControlState == CONTROL_EQUIPMENT_OFFLINE ||
		gemStatus.ControlState == CONTROL_HOST_OFFLINE) && CureStartButton->Enabled == false)
		{
			CureStartButton->Enabled = true;
		}

		UpdateCimStatus();

		UpdateUI->Enabled = true;

	}
}
//---------------------------------------------------------------------------
