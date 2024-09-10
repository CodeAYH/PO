//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ChamberSettingFrame.h"
#include "HelperFunctions.h"
#include "Database.h"
#include "NormalOvenChamber.h"
#include "NormalOvenSystemManager.h"
#include "MainForm.h"
#include "MainTempController.h"
#include "MoonsStepMotor.h"
#include "GEMForm.h"
#include "EzNetForm.h"
#include "PatternSettingFrame.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvCombo"
#pragma link "AdvEdit"
#pragma link "AdvGlassButton"
#pragma link "AdvGrid"
#pragma link "AdvGroupBox"
#pragma link "AdvObj"
#pragma link "AdvOfficeButtons"
#pragma link "AdvPanel"
#pragma link "AdvSelectors"
#pragma link "AdvSmoothProgressBar"
#pragma link "AdvSmoothSlider"
#pragma link "AdvSmoothTabPager"
#pragma link "AdvSmoothToggleButton"
#pragma link "BaseGrid"
#pragma link "ToolPanels"
#pragma link "AdvUtil"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFrmChamberSetting::TFrmChamberSetting(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
__fastcall TFrmChamberSetting::TFrmChamberSetting(TComponent* Owner, int chamberNo)
	: TFrame(Owner), ChamberNo(chamberNo)
{
	Init();
	InitCtrls();
	InitChamberSetting();

	String str;
	str.sprintf(L"Chamber %c", chamberNo+'A');
	Hint = str;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamberSetting::Init()
{
	SettingTabPager->RemoveAdvSmoothTabPage(ChamberSetting);
	SettingTabPager->RemoveAdvSmoothTabPage(FactorySetting);
	SettingTabPager->RemoveAdvSmoothTabPage(GemPage);

	TFrmPatternSetting *frame = new TFrmPatternSetting(PatternSetting, ChamberNo);
	frame->Parent = PatternSetting;
	SettingTabPager->AddAdvSmoothTabPage(PatternSetting);
	SettingTabPager->AddAdvSmoothTabPage(ChamberSetting);

	if (GetManager()->UseAutomation == true &&
		GetManager()->AutomationKind == SECS_GEM_SCK)
	{
		SettingTabPager->AddAdvSmoothTabPage(GemPage);
    }

	// 20191021 - Factory Setting Add - jhlim
	if( GetManager()->LoginLevel == LV_DEVELOPER )
	{
		SettingTabPager->AddAdvSmoothTabPage(FactorySetting);
        // Barun MES
		if( GetManager()->UseAutomation )
		{
			if( GetManager()->AutomationKind == BARUN_MES )
				BarunMesPanel->Visible = true;
			else
				BarunMesPanel->Visible = false;
		}
	}
	else
	{
		SettingTabPager->RemoveAdvSmoothTabPage(FactorySetting);
	}

	if( GetManager()->UseAutomation &&     //2021-10-12 hcw.
		GetManager()->AutomationKind == FUJITSU_TCARD )
	{
		TCardGroupBox->Visible = true;
		AddFunctionPanel->Height = 51;
	}
	else
	{
		TCardGroupBox->Visible = false;
		AddFunctionPanel->Height = 78;
	}

	if (GetManager()->UseGroundAlarmDetector)
	{
        UseDoorOpenDetectedWhenIdle->Visible = false;
    }
	SettingTabPager->ActivePage = PatternSetting;

	StatusGrid->ColWidths[0] = 122;
	StatusGrid->ColWidths[1] = 138;
	StatusGrid->Options >> goRangeSelect;
	StatusGrid->ShowSelection = false;
	StatusGrid->MergeCells(0, 0, 1, 3);
	StatusGrid->ShowSelection = false;
}
//---------------------------------------------------------------------------
__fastcall TFrmChamberSetting::~TFrmChamberSetting()
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamberSetting::ShowKeyboard(bool isNemeric)
{
	int x, y;

	if( isNemeric )
	{
		FmMain->Keyboard->KeyboardHeight = 162;
		FmMain->Keyboard->KeyboardWidth = 122;
		FmMain->Keyboard->KeyboardType = ktCELLPHONE;
		x = (1024 - FmMain->Keyboard->KeyboardWidth) / 2;
		x += FmMain->Left;
		FmMain->Keyboard->ShowAtXY(x, 200+FmMain->Top);
	}
	else
	{
		FmMain->Keyboard->KeyboardHeight = 202;
		FmMain->Keyboard->KeyboardWidth = 602;
		FmMain->Keyboard->KeyboardType = ktQWERTY;
		x = (1024 - FmMain->Keyboard->KeyboardWidth) / 2;
		x += FmMain->Left;
		y = 400+FmMain->Top;
		FmMain->Keyboard->ShowAtXY(x, y);
	}
}

void __fastcall TFrmChamberSetting::InitCtrls()
{
	String str;
	int loggerCount = GetManager()->LoggerCount;

	for(int i=0; i<loggerCount; i++)
	{
		for(int j=0; j<MAX_POINT_COUNT; j++)
		{
			LoggerEdit[i][j] = nullptr;
		}
	}

	int top;
	for(int i=0; i<loggerCount; i++)
	{
		int cnt = 8;
		LoggerPanel[i] = new TAdvPanel(ChamberSettingPanel);
		LoggerPanel[i]->Parent = ChamberSettingPanel;
		LoggerPanel[i]->Width = IdleCoolingPanel->Width;
		LoggerPanel[i]->Height = (cnt + 1) * 21;//(cnt + 1) * 22 + 3;
		LoggerPanel[i]->Left = IdleCoolingPanel->Left;
	 //	LoggerPanel[i]->Top = IdleCoolingPanel->Top + IdleCoolingPanel->Height + (LoggerPanel[i]->Height * i);
		LoggerPanel[i]->Top = AddFunctionPanel->Top + AddFunctionPanel->Height + (LoggerPanel[i]->Height * i);
		top = LoggerPanel[i]->Top + LoggerPanel[i]->Height;
		LoggerPanel[i]->Caption->Visible = true;
		LoggerPanel[i]->Caption->Color = clSkyBlue;
		if( loggerCount == 1 )
			str = "Temp. Logger";
		else
			str.sprintf(L"Temp. Logger %d", i+1);
		LoggerPanel[i]->Caption->Text = str;
		LoggerPanel[i]->Caption->Height = 21;  //22
		LoggerPanel[i]->Caption->Font->Size = 12;
		LoggerPanel[i]->Caption->Font->Style = TFontStyles() << fsBold;
		LoggerPanel[i]->Visible = true;

		for(int j=0; j<cnt; j++)
		{
			// Use CheckBox
			LoggerUseCheckBox[i][j] = new TCheckBox(LoggerPanel[i]);
			LoggerUseCheckBox[i][j]->Parent = LoggerPanel[i];
			LoggerUseCheckBox[i][j]->Width = (LoggerPanel[i]->Width / 4);
			LoggerUseCheckBox[i][j]->Height = 21;  //22
			LoggerUseCheckBox[i][j]->Left = (LoggerUseCheckBox[i][j]->Width / 5);
			LoggerUseCheckBox[i][j]->Top = LoggerPanel[i]->Caption->Height + (j * LoggerUseCheckBox[i][j]->Height);
			str.sprintf(L"Use Ch. %d", j+1);
			LoggerUseCheckBox[i][j]->Caption = str;
			LoggerUseCheckBox[i][j]->Font->Size = 8;
			LoggerUseCheckBox[i][j]->Font->Style = TFontStyles() << fsBold;
			LoggerUseCheckBox[i][j]->Visible = true;
			// Offset Edit
			LoggerEdit[i][j] = new TAdvEdit(LoggerPanel[i]);
			LoggerEdit[i][j]->Parent = LoggerPanel[i];
			LoggerEdit[i][j]->Font->Size = 10;
			LoggerEdit[i][j]->Font->Style = TFontStyles() << fsBold;
			LoggerEdit[i][j]->LabelFont->Size = 8;
			LoggerEdit[i][j]->LabelFont->Style = TFontStyles() << fsBold;
			LoggerEdit[i][j]->LabelAlwaysEnabled = true;
			LoggerEdit[i][j]->AutoSize = false;
			str.sprintf(L"Ch. %d Offset", j+1);
			LoggerEdit[i][j]->LabelCaption = str;
			LoggerEdit[i][j]->LabelPosition = Advedit::lpLeftCenter;
			LoggerEdit[i][j]->Tag = i*10 + j;
			LoggerEdit[i][j]->OnClick = OnOffsetEditEnter;
			LoggerEdit[i][j]->Visible = true;
			LoggerEdit[i][j]->Width = (LoggerPanel[i]->Width / 4);
			LoggerEdit[i][j]->Height = 21;     //22
			LoggerEdit[i][j]->Left = LoggerUseCheckBox[i][j]->Width + (LoggerUseCheckBox[i][j]->Left * 6);
			LoggerEdit[i][j]->Top = LoggerUseCheckBox[i][j]->Top;
		}
	}

	if (GetManager()->DamperControlType == MOONS_STEP_MOTOR_DAMPER)
	{
		MoonsMotorWarningPanel->Visible = true;
		MoonsMotorAlarmPanel->Visible = true;
		EziMotorStatusPanel->Visible = false;
	}
	else if (GetManager()->DamperControlType == EZI_STEP_MOTOR_DAMPER)
	{
		MoonsMotorWarningPanel->Visible = false;
		MoonsMotorAlarmPanel->Visible = false;
		EziMotorStatusPanel->Visible = true;

		EziMotorStatusPanel->Left = MoonsMotorWarningPanel->Left;
		EziMotorStatusPanel->Top = MoonsMotorWarningPanel->Top;
	}
	else
	{
		MoonsMotorWarningPanel->Visible = false;
		MoonsMotorAlarmPanel->Visible = false;
		EziMotorStatusPanel->Visible = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamberSetting::DestroyLoggerCtrls()
{
	for(int i=0; i<GetManager()->LoggerCount; i++)
	{
		for(int j=0; j<MAX_POINT_COUNT; j++)
		{
		//	if( LoggerCheckBox[i][j] )
		//		delete LoggerCheckBox[i][j];
			if( LoggerEdit[i][j] )
				delete LoggerEdit[i][j];
		}
		delete LoggerPanel[i];
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamberSetting::InitChamberSetting()
{
	String str;
	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( chamber == nullptr )
		return;

	MainTempCtrlTypeComboBox->ItemIndex = chamber->MainTempControllerType;

	CannotStartWithoutLotInfoCheckBox->Checked = chamber->CannotStartWithoutLotInfo;
	AllowRecipeSelectionCheckBox->Checked = chamber->AllowRecipeSelection;
	GotoIdleStepCheckBox->Checked = chamber->GotoIdleInLocal;
	RecipeMaintainCheckBox->Checked = chamber->RecipeMaintain; //2022-07-12
	if( GetManager()->UseMagazineDetector == false )
	{
		AutoStartCheckBox->Visible = false;
	}
	else
	{
		AutoStartCheckBox->Visible = true;
		AutoStartCheckBox->Checked = chamber->IsAbleAutoStart;
	}

	if (GetManager()->UseGroundAlarmDetector)
	{
		UseDoorOpenDetectedWhenIdle->Visible = false;
        DoorOpenDetectedInterval->Visible = false;
	}
	else
	{
		UseDoorOpenDetectedWhenIdle->Visible = true;
		DoorOpenDetectedInterval->Visible = true;
		UseDoorOpenDetectedWhenIdle->Checked = chamber->DoorOpenDetectedWhenIdle;
		if (chamber->DoorOpenDetectedInterval > 10)
		{
			chamber->DoorOpenDetectedInterval = 10;;
			DoorOpenDetectedInterval->Text = chamber->DoorOpenDetectedInterval;
		}
		else if (chamber->DoorOpenDetectedInterval <= 2)
		{
			chamber->DoorOpenDetectedInterval = 2;
			DoorOpenDetectedInterval->Text = chamber->DoorOpenDetectedInterval;
		}
		else
		{
			DoorOpenDetectedInterval->Text = chamber->DoorOpenDetectedInterval;
		}
	}

	DoorOpenAlarmEdit->Text = chamber->DoorOpenConditionTemp;

	OffsetHighEdit->Text = chamber->ChamberTempLimitOffset;
	str.sprintf(L"%.1f", chamber->ChamberTempLimitBias);
	TempLimitBiasEdit->Text = str;
	//2021-08-09 hcw.
	LoggerDeviationEdit->Text = chamber->ChamberLoggerTempDeviation;
	//2021-09-29 Fujitsu
	ChamberSelectEdit->Text = chamber->ChamberSelect;
	StartCureEdit->Text = chamber->TCardStartCure;

	MachineIDEdit->Text = chamber->MachineID;
	UseManualModeCheckBox->Checked = chamber->UseManualMode;

	if (GetManager()->UseO2Analyzer)
	{
		OxygenPanel->Visible = true;
		O2DensityLimitEdit->Text = chamber->O2DensityLimit;
		O2DensityAlarmEnableTimeEdit->Text = chamber->O2DensityAlarmEnableTime;
		CureStopOnO2DensityAlarmCheckBox->Checked = chamber->CureStopOnO2Alarm;
		// 20180305 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
		O2AnalyzeUseRunLimitCheckBox->Checked = chamber->O2AnalyzerUseRunLimit;
		// 20180313 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
		O2DensityRunLimitEdit->Text = chamber->O2DensityRunLimit;
		O2DensityAlarmRunLimitTime->Text = chamber->O2DensityAlarmRunLimit;

		LargeN2ValveOpenO2DensityEdit->Text = chamber->LargeN2ValveOpenO2Density;
		LargeN2ValveOpenO2DensityEdit->Visible = GetManager()->UseN2FlowControl;
	}
	else
	{
		OxygenPanel->Visible = false;
	}

	if (GetManager()->DamperControlType == CYLINDER_DAMPER)
	{
		DamperMotorPanel->Visible = false;
	}
	else
	{
		DamperMotorPanel->Visible = true;
    }

	for(int i=0; i<GetManager()->LoggerCount; i++)
	{
		for(int j=0; j<MAX_POINT_COUNT; j++)
		{
			str.sprintf(L"%.1f", chamber->LoggerOffset[i*MAX_POINT_COUNT + j]);
			LoggerEdit[i][j]->Text = str;

			LoggerUseCheckBox[i][j]->Checked = chamber->UseChannel[i*MAX_POINT_COUNT + j];
		}
	}

	CbxTempLimitCtrlType->ItemIndex = chamber->LimitControllerType;

	str.sprintf(L"%.1f", chamber->IdleCoolingTemp);
	IdleCoolingTempEdit->Text = str;
	str.sprintf(L"%.1f", chamber->IdleCoolingOffset);
	IdleCoolingOffsetEdit->Text = str;
	UseIdleCoolingCheckBox->Checked = chamber->UseIdleCooling;

	WorkCompletionEdit->Text = chamber->WorkCompletionTemp;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamberSetting::UpdateChamberSetting()
{
	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( chamber == nullptr )
		return;

	chamber->MainTempControllerType = MainTempCtrlTypeComboBox->ItemIndex;

	chamber->CannotStartWithoutLotInfo = CannotStartWithoutLotInfoCheckBox->Checked;
	chamber->AllowRecipeSelection = AllowRecipeSelectionCheckBox->Checked;
    chamber->DoorOpenDetectedWhenIdle = UseDoorOpenDetectedWhenIdle->Checked;
	chamber->GotoIdleInLocal = GotoIdleStepCheckBox->Checked;
	chamber->RecipeMaintain = RecipeMaintainCheckBox->Checked; //2022-07-12
	if( GetManager()->UseMagazineDetector )
	{
		chamber->IsAbleAutoStart = AutoStartCheckBox->Checked;
	}
	chamber->DoorOpenConditionTemp = DoorOpenAlarmEdit->Text.ToDouble();

	if (DoorOpenDetectedInterval->Text.ToInt() > 10)
	{
		chamber->DoorOpenDetectedInterval = 10;
	}
	else if (DoorOpenDetectedInterval->Text.ToInt() <= 2)
	{
		chamber->DoorOpenDetectedInterval = 2;
	}
	else
	{
		chamber->DoorOpenDetectedInterval =  DoorOpenDetectedInterval->Text.ToInt();
	}

	chamber->ChamberTempLimitOffset = OffsetHighEdit->Text.ToDouble();
	chamber->ChamberTempLimitBias = TempLimitBiasEdit->Text.ToDouble();
	chamber->ChamberLoggerTempDeviation = LoggerDeviationEdit->Text.ToDouble();//2021-08-09 hcw
	chamber->ChamberSelect =   ChamberSelectEdit->Text; //2021-09-29 Fujitsu
	chamber->TCardStartCure =   StartCureEdit->Text;  //2021-09-29 Fujitsu

	chamber->MachineID = MachineIDEdit->Text;
	chamber->UseManualMode = UseManualModeCheckBox->Checked;

	chamber->O2DensityLimit = O2DensityLimitEdit->Text.ToInt();
	chamber->O2DensityAlarmEnableTime = O2DensityAlarmEnableTimeEdit->Text.ToInt();
	chamber->CureStopOnO2Alarm = CureStopOnO2DensityAlarmCheckBox->Checked;
	// 20180305 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
	chamber->O2AnalyzerUseRunLimit = O2AnalyzeUseRunLimitCheckBox->Checked;
	// 20180313 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
	chamber->O2DensityRunLimit = O2DensityRunLimitEdit->Text.ToInt();
	chamber->O2DensityAlarmRunLimit = O2DensityAlarmRunLimitTime->Text.ToInt();
	chamber->LargeN2ValveOpenO2Density = LargeN2ValveOpenO2DensityEdit->Text.ToInt();

    int count = 0;
	for(int i=0; i<GetManager()->LoggerCount; i++)
	{
		for(int j=0; j<MAX_POINT_COUNT; j++)
		{
			chamber->LoggerOffset[i*MAX_POINT_COUNT + j] = LoggerEdit[i][j]->Text.ToDouble();
			chamber->UseChannel[i*MAX_POINT_COUNT + j] = LoggerUseCheckBox[i][j]->Checked;
            if (chamber->UseChannel[i*MAX_POINT_COUNT + j] == true)
            {
                count++;
            }
		}
	}

	chamber->LimitControllerType = CbxTempLimitCtrlType->ItemIndex;

	chamber->IdleCoolingTemp = IdleCoolingTempEdit->Text.ToDouble();
	chamber->IdleCoolingOffset = IdleCoolingOffsetEdit->Text.ToDouble();
	chamber->UseIdleCooling = UseIdleCoolingCheckBox->Checked;
    //Add Option
	chamber->WorkCompletionTemp = WorkCompletionEdit->Text.ToDouble();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::BtnChamberSaveClick(TObject *Sender)
{
	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( chamber == nullptr )
		return;

	SetMessageStr(GetResStr("Question", "QuestSave"));
	if( ShowQuestionDlg() )
	{
		UpdateChamberSetting();
		chamber->SaveChamberParameters();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::BtnChamberCancelClick(TObject *Sender)
{
	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( chamber == nullptr )
		return;

	chamber->LoadChamberParameters();
	InitChamberSetting();
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamberSetting::OnUseCheckBoxClick(TObject *Sender)
{
	if( Sender == nullptr )
		return;

	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( chamber == nullptr )
		return;

	TCheckBox *chkBox = (TCheckBox*)Sender;
	int tag = (int)chkBox->Tag;
	int lgIdx = tag / 10;
	int chIdx = tag - lgIdx * 10;

	LoggerEdit[lgIdx][chIdx]->Enabled = chkBox->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamberSetting::OnOffsetEditEnter(TObject *Sender)
{
	ShowKeyboard(false);
	PostMessage(Handle, WM_USER+1, (WPARAM)Sender, 0);
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamberSetting::OnSelectAll(TMessage &Message)
{
	TAdvEdit *edit = (TAdvEdit*)Message.WParam;
	edit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamberSetting::BtnUP550SettingClick(TObject *Sender)
{
	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( chamber == nullptr )
		return;

	if( chamber->MainTempController )
	{
		int cnt = chamber->MainTempControllerType == TEMPCTRL_UP55A ? UP55A_PARAM_COUNT : UP550_PARAM_COUNT;
		UP550ProgressBar->Maximum = cnt-1;
		UP550ProgressBar->Position = 0;

		bool completed = true;
		for(int i=0; i<cnt; i++)
		{
			if( chamber->MainTempController->WriteParameter(i) )
				UP550ProgressBar->GoToValue((double)i);
			else
				completed = false;
		}

		if( completed == false )
			SetMessageStr(String(L"Failed!"));
		else
			SetMessageStr(String(L"Success!"));
		ShowMessageDlg();
		UP550ProgressBar->Position = 0;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamberSetting::HomeButtonClick(TObject *Sender)
{
	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( chamber == nullptr )
		return;

	chamber->DamperMotor->SearchHome();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::OpenButtonClick(TObject *Sender)
{
	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( chamber == nullptr )
		return;

	chamber->DamperMotor->Open();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::CloseButtonClick(TObject *Sender)
{
	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( chamber == nullptr )
		return;

	chamber->DamperMotor->Close();
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamberSetting::UpdateMoonsMotorStatus(DWORD status)
{
	TColor clr;
	int bit;
	unsigned short alarmCode = (unsigned short)status;

	// Alarm
	bit = GetAbit(alarmCode, 1);
	clr = bit == 1 ? clYellow : clSilver;
	if( CCWLimitIndicator->Color != clr )
		CCWLimitIndicator->Color = clr;

	bit = GetAbit(alarmCode, 2);
	clr = bit == 1 ? clYellow : clSilver;
	if( CWLimitIndicator->Color != clr )
		CWLimitIndicator->Color = clr;

	bit = GetAbit(alarmCode, 6);
	clr = bit == 1 ? clYellow : clSilver;
	if( UnderVoltageIndicator->Color != clr )
		UnderVoltageIndicator->Color = clr;

	bit = GetAbit(alarmCode, 10);
	clr = bit == 1 ? clYellow : clSilver;
	if( CommErrorIndicator->Color != clr )
		CommErrorIndicator->Color = clr;

	bit = GetAbit(alarmCode, 11);
	clr = bit == 1 ? clYellow : clSilver;
	if( BadFlashIndicator->Color != clr )
		BadFlashIndicator->Color = clr;

	bit = GetAbit(alarmCode, 12);
	clr = bit == 1 ? clYellow : clSilver;
	if( NoMoveIndicator->Color != clr )
		NoMoveIndicator->Color = clr;

	bit = GetAbit(alarmCode, 14);
	clr = bit == 1 ? clYellow : clSilver;
	if( BlankQSegmentIndicator->Color != clr )
		BlankQSegmentIndicator->Color = clr;

	// Error
	bit = GetAbit(alarmCode, 3);
	clr = bit == 1 ? clRed : clSilver;
	if( OverTempIndicator->Color != clr )
		OverTempIndicator->Color = clr;

	bit = GetAbit(alarmCode, 4);
	clr = bit == 1 ? clRed : clSilver;
	if( InternalVoltageIndicator->Color != clr )
		InternalVoltageIndicator->Color = clr;

	bit = GetAbit(alarmCode, 5);
	clr = bit == 1 ? clRed : clSilver;
	if( OverVoltageIndicator->Color != clr )
		OverVoltageIndicator->Color = clr;

	bit = GetAbit(alarmCode, 7);
	clr = bit == 1 ? clRed : clSilver;
	if( OverCurrentIndicator->Color != clr )
		OverCurrentIndicator->Color = clr;

	bit = GetAbit(alarmCode, 8);
	clr = bit == 1 ? clRed : clSilver;
	if( OpenWindingIndicator->Color != clr )
		OpenWindingIndicator->Color = clr;

	bit = GetAbit(alarmCode, 9);
	clr = bit == 1 ? clRed : clSilver;
	if( BadEncoderIndicator->Color != clr )
		BadEncoderIndicator->Color = clr;
}

void __fastcall TFrmChamberSetting::UpdateEziMotorStatus(DWORD dwStatus)
{
	DWORD dwRet;
	TColor clr;

	//FFLAG_ERRORALL 여러 에러중 하나 이상의 에러가 발생함. 0X00000001
	dwRet = dwStatus & 0x00000001;
	clr = dwRet ? clRed : clSilver;
	if( ErrorAllIndicator->Color != clr )
		ErrorAllIndicator->Color = clr;

	//FFLAG_HWPOSILMT +방향 리미트 센서가 ON 이 된경우 0X00000002
	dwRet = dwStatus & 0x00000002;
	clr = dwRet ? clRed : clSilver;
	if( HWPlusLimitIndicator->Color != clr )
		HWPlusLimitIndicator->Color = clr;

	//FFLAG_HWNEGALMT -방향 리미트 센서가 ON 이 된경우 0X00000004
	dwRet = dwStatus & 0x00000004;
	clr = dwRet ? clRed : clSilver;
	if( HWMinusLimitIndicator->Color != clr )
		HWMinusLimitIndicator->Color = clr;

	//FFLAG_SWPOGILMT +방향 프로그램 리미트를 초과한 경우 0X00000008
	dwRet = dwStatus & 0x00000008;
	clr = dwRet ? clRed : clSilver;
	if( SWPlusLimitIndicator->Color != clr )
		SWPlusLimitIndicator->Color = clr;

	//FFLAG_SWNEGALMT -방향 프로그램 리미트를 초과한 경우 0X00000010
	dwRet = dwStatus & 0x00000010;
	clr = dwRet ? clRed : clSilver;
	if( SWMinusLimitIndicator->Color != clr )
		SWMinusLimitIndicator->Color = clr;

	//FFLAG_ERRSTEPALARM Step Alarm(이하 8 가지)중 한가지 이상 발생. 0X00000080
	dwRet = dwStatus & 0x00000080;
	clr = dwRet ? clRed : clSilver;
	if( ErrStepAlarmIndicator->Color != clr )
		ErrStepAlarmIndicator->Color = clr;

	//FFLAG_ERROVERCURRENT 모터 구동소자에 과전류 이상 Alarm 발생. 0X00000100
	dwRet = dwStatus & 0x00000100;
	clr = dwRet ? clRed : clSilver;
	if( ErrOverCurrentIndicator->Color != clr )
		ErrOverCurrentIndicator->Color = clr;

	//FFLAG_ERROVERSPEED 모터의 속도가 3000[rpm]을 초과한 Alarm 발생. 0X00000200
	dwRet = dwStatus & 0x00000200;
	clr = dwRet ? clRed : clSilver;
	if( ErrOverSpeedIndicator->Color != clr )
		ErrOverSpeedIndicator->Color = clr;

	//FFLAG_ERRSPEED 모터가 펄스 입력에 정상적으로 추종하지 않는 Alarm 발생 0X00000400
	dwRet = dwStatus & 0x00000400;
	clr = dwRet ? clRed : clSilver;
	if( ErrSpeedIndicator->Color != clr )
		ErrSpeedIndicator->Color = clr;

	//FFLAG_ERROVERHEAT 드라이브의 내부온도가 55°C 를 초과하는 Alarm 발생. 0X00001000
	dwRet = dwStatus & 0x00001000;
	clr = dwRet ? clRed : clSilver;
	if( ErrOverHeatIndicator->Color != clr )
		ErrOverHeatIndicator->Color = clr;

	//FFLAG_ERRREVPWR 모터의 역기전력 전압이 70V 를 초과하는 Alarm 발생. 0X00002000
	dwRet = dwStatus & 0x00002000;
	clr = dwRet ? clRed : clSilver;
	if( ErrRevPowerIndicator->Color != clr )
		ErrRevPowerIndicator->Color = clr;

	//FFLAG_ERRMOTORPOWER 모터 전압 이상 Alarm 발생. 0X00004000
	dwRet = dwStatus & 0x00004000;
	clr = dwRet ? clRed : clSilver;
	if( ErrMotorPowerIndicator->Color != clr )
		ErrMotorPowerIndicator->Color = clr;

	//FFLAG_ERRLOWPOWER 드라이브 전압 이상 Alarm 발생. 0X00008000
	dwRet = dwStatus & 0x00008000;
	clr = dwRet ? clRed : clSilver;
	if( ErrLowPowerIndicator->Color != clr )
		ErrLowPowerIndicator->Color = clr;

	//FFLAG_EMGSTOP 모터가 비상 정지 상태임. 0X00010000
	dwRet = dwStatus & 0x00010000;
	clr = dwRet ? clRed : clSilver;
	if( EmgStopIndicator->Color != clr )
		EmgStopIndicator->Color = clr;

	//FFLAG_SLOWSTOP 모터가 일반 정지 상태임. 0X00020000
	dwRet = dwStatus & 0x00020000;
	clr = dwRet ? clRed : clSilver;
	if( LowStopIndicator->Color != clr )
		LowStopIndicator->Color = clr;

	//FFLAG_ORIGINRETURNING 원점복귀 운전중임. 0X00040000
	dwRet = dwStatus & 0x00040000;
	clr = dwRet ? clRed : clSilver;
	if( OrgReturningIndicator->Color != clr )
		OrgReturningIndicator->Color = clr;

	//FFLAG_ALARMRESET AlarmReset 명령이 실시된 상태임. 0X00200000
	dwRet = dwStatus & 0x00200000;
	clr = dwRet ? clRed : clSilver;
	if( AlarmResetIndicator->Color != clr )
		AlarmResetIndicator->Color = clr;

	//FFLAG_PTSTOPED 포지션테이블 운전이 종료된 상태임. 0X00400000
	dwRet = dwStatus & 0x00400000;
	clr = dwRet ? clRed : clSilver;
	if( PTStoppedIndicator->Color != clr )
		PTStoppedIndicator->Color = clr;

	//FFLAG_ORIGINSENSOR 원점센서가 ON 되어 있는 상태임. 0X00800000
	dwRet = dwStatus & 0x00800000;
	clr = dwRet ? clRed : clSilver;
	if( OrgSensorIndicator->Color != clr )
		OrgSensorIndicator->Color = clr;

	//FFLAG_ZPULSE 원점 복귀 운전중 z-pulse 방식의 동작인 경우. 0X01000000
	dwRet = dwStatus & 0x01000000;
	clr = dwRet ? clRed : clSilver;
	if( ZPulseIndicator->Color != clr )
		ZPulseIndicator->Color = clr;

	//FFLAG_ORIGINRETOK 원점복귀 운전이 완료된 상황임. 0X02000000
	dwRet = dwStatus & 0x02000000;
	clr = dwRet ? clRed : clSilver;
	if( OrgRetOKIndicator->Color != clr )
		OrgRetOKIndicator->Color = clr;

	//FFLAG_MOTIONDIR 모터의 운전 방향 (+방향:OFF, -방향:ON) 0X04000000
	dwRet = dwStatus & 0x04000000;
	clr = dwRet ? clRed : clSilver;
	if( MotionDirIndicator->Color != clr )
		MotionDirIndicator->Color = clr;

	//FFLAG_MOTIONING 모터가 현재 운전중임. 0X08000000
	dwRet = dwStatus & 0x08000000;
	clr = dwRet ? clRed : clSilver;
	if( MotioningIndicator->Color != clr )
		MotioningIndicator->Color = clr;

	//FFLAG_MOTIONPAUSE 운전중 Pause 명령으로 정지 상태임. 0X10000000
	dwRet = dwStatus & 0x10000000;
	clr = dwRet ? clRed : clSilver;
	if( MotionPasueIndicator->Color != clr )
		MotionPasueIndicator->Color = clr;

	//FFLAG_MOTIONACCEL 가속구간의 운전중임. 0X20000000
	dwRet = dwStatus & 0x20000000;
	clr = dwRet ? clRed : clSilver;
	if( MotionAccelIndicator->Color != clr )
		MotionAccelIndicator->Color = clr;

	//FFLAG_MOTIONDECEL 감속구간의 운전중임. 0X40000000
	dwRet = dwStatus & 0x40000000;
	clr = dwRet ? clRed : clSilver;
	if( MotionDecelIndicator->Color != clr )
		MotionDecelIndicator->Color = clr;
}

void __fastcall TFrmChamberSetting::RefreshTimer(TObject *Sender)
{
	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( chamber == nullptr )
		return;

	if( chamber->DamperMotor == nullptr )
		return;

	String str, enable, init;

	enable = chamber->DamperMotor->Enabled ? "Enabled" : "Disabled";
	init = chamber->DamperMotor->HomeInitialized ? "Initialized" : "Not Initialized";
	str.sprintf(L"<P align=\"left\">%s / %s<BR>Current Position : %d</P>", enable.c_str(), init.c_str(), chamber->DamperMotor->CurrentPos);
	if( str != DamperMotorPanel->Text )
		DamperMotorPanel->Text = str;

	DWORD alarmCode = chamber->DamperMotor->AlarmCode;

	Refresh->Enabled = false;
	if (GetManager()->DamperControlType == MOONS_STEP_MOTOR_DAMPER)
	{
		UpdateMoonsMotorStatus(alarmCode);
	}
	else if (GetManager()->DamperControlType == EZI_STEP_MOTOR_DAMPER)
	{
		UpdateEziMotorStatus(alarmCode);
	}

	Refresh->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::AlarmClearButtonClick(TObject *Sender)
{
	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( chamber == nullptr )
		return;

	chamber->DamperMotor->AlarmClear();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::EnableButtonClick(TObject *Sender)
{
	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( chamber == nullptr )
		return;

	chamber->DamperMotor->MotorEnable();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::DisableButtonClick(TObject *Sender)
{
	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( chamber == nullptr )
		return;

	chamber->DamperMotor->MotorEnable(false);
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::OnEditEnter(TObject *Sender)
{
	ShowKeyboard(false);
	PostMessage(Handle, WM_USER+1, (WPARAM)Sender, 0);
	((TAdvEdit*)Sender)->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::OnEditExit(TObject *Sender)
{
	FmMain->Keyboard->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::GEMRefreshTimer(TObject *Sender)
{
	if (EzNetDlg[ChamberNo] == nullptr)
	{
		return;
	}

	String connected, enabled;
	if (EzNetDlg[ChamberNo]->GemState.Connected)
	{
		ConnectedImage->Visible = true;
		NotConnectedImage->Visible = false;
		connected = "CONNECTED";
	}
	else
	{
		ConnectedImage->Visible = false;
		NotConnectedImage->Visible = true;
		connected = "NOT CONNECTED";
	}
	enabled = EzNetDlg[ChamberNo]->GemState.Enabled ? "ENABLED" : "DISABLED";

	StatusGrid->Cells[1][0] = connected;
	StatusGrid->Cells[1][1] = enabled;

	String ctrlState = "";

	switch (EzNetDlg[ChamberNo]->GemState.ControlState)
	{
	case CONTROL_EQUIPMENT_OFFLINE:
		ctrlState = "OFFLINE";
		break;
	case CONTROL_ATTEMPT_ONLINE:
		ctrlState = "ATTEMPT ONLINE";
		break;
	case CONTROL_HOST_OFFLINE:
		ctrlState = "HOST OFFLINE";
		break;
	case CONTROL_ONLINE_LOCAL:
		ctrlState = "ONLINE LOCAL";
		break;
	case CONTROL_ONLINE_REMOTE:
		ctrlState = "ONLINE REMOTE";
		break;
	default:
		break;
	}
	StatusGrid->Cells[1][2] = ctrlState;

	StartGemButton->Enabled = !EzNetDlg[ChamberNo]->GemState.Started;
	StopGemButton->Enabled	= EzNetDlg[ChamberNo]->GemState.Started;
	GemOfflineButton->Enabled = EzNetDlg[ChamberNo]->GemState.Connected && (EzNetDlg[ChamberNo]->GemState.ControlState != CONTROL_EQUIPMENT_OFFLINE);
	GemLocalButton->Enabled = EzNetDlg[ChamberNo]->GemState.Connected && (EzNetDlg[ChamberNo]->GemState.ControlState != CONTROL_ONLINE_LOCAL);
	GemRemoteButton->Enabled = EzNetDlg[ChamberNo]->GemState.Connected && (EzNetDlg[ChamberNo]->GemState.ControlState != CONTROL_ONLINE_REMOTE);
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::StartGemButtonClick(TObject *Sender)
{
	if (EzNetDlg[ChamberNo] == nullptr)
	{
		return;
	}

	EzNetDlg[ChamberNo]->StartGEM();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::StopGemButtonClick(TObject *Sender)
{
	if (EzNetDlg[ChamberNo] == nullptr)
	{
		return;
	}

	EzNetDlg[ChamberNo]->StopGEM();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::GemOfflineButtonClick(TObject *Sender)
{
	if (EzNetDlg[ChamberNo] == nullptr)
	{
		return;
	}

	EzNetDlg[ChamberNo]->GoOffline();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::GemLocalButtonClick(TObject *Sender)
{
	if (EzNetDlg[ChamberNo] == nullptr)
	{
		return;
	}

	EzNetDlg[ChamberNo]->GoOnlineLocal();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::GemRemoteButtonClick(TObject *Sender)
{
	if (EzNetDlg[ChamberNo] == nullptr)
	{
		return;
	}

	EzNetDlg[ChamberNo]->GoOnlineRemote();
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::btnJobStartClick(TObject *Sender)
{
	if (EzNetDlg[ChamberNo] == nullptr)
	{
		return;
	}

	long svid[4];
	String sVal[4];

	ZeroMemory(svid, 4);

	svid[0] = SVID_PORTID;
	sVal[0] = IntToStr(ChamberNo);
	svid[1] = SVID_LOTID;
	sVal[1] = L"Lot ID";
	svid[2] = SVID_EZNET_CSTID;
	sVal[2] = L"Cassette ID";
	svid[3] = SVID_PPID;
	sVal[3] = edtRecipe->Text;

	EzNetDlg[ChamberNo]->SetSVID(svid, sVal, 4);

	EzNetDlg[ChamberNo]->SendEventReport(CEID_JOB_START);
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::btnJobEndClick(TObject *Sender)
{
	if (EzNetDlg[ChamberNo] == nullptr)
	{
		return;
	}

	long svid[4];
	String sVal[4];

	ZeroMemory(svid, 4);

	svid[0] = SVID_PORTID;
	sVal[0] = IntToStr(ChamberNo);
	svid[1] = SVID_LOTID;
	sVal[1] = L"Lot ID";
	svid[2] = SVID_EZNET_CSTID;
	sVal[2] = L"Cassette id";
	svid[3] = SVID_PPID;
	sVal[3] = edtRecipe->Text;

	EzNetDlg[ChamberNo]->SetSVID(svid, sVal, 4);

	EzNetDlg[ChamberNo]->SendEventReport(CEID_JOB_END);
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamberSetting::btnJobCancelClick(TObject *Sender)
{
	if (EzNetDlg[ChamberNo] == nullptr)
	{
		return;
	}

	EzNetDlg[ChamberNo]->SendJobCancelEvent("Lot ID", L"Cassette ID", edtRecipe->Text);
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamberSetting::btnJobValidationClick(TObject *Sender)
{
	if (EzNetDlg[ChamberNo] == nullptr)
	{
		return;
	}

	NormalOvenChamber *Chamber = GetChamber(ChamberNo);
	Chamber->JsckGemParamClear();
	String str, str_1, str_2;
	Chamber->MesMode = 1;
	str_1 = "1111";
	str_2 = "@@@aaa1234";
	str.sprintf(L"%s,%s", str_1.c_str(), str_2.c_str());
	Chamber->DeviceIDList->Add(str);
	str_1 = "2222";
	str_2 = "!!!aaa5678";
	str.sprintf(L"%s,%s", str_1.c_str(), str_2.c_str());
	Chamber->DeviceIDList->Add(str);

	EzNetDlg[ChamberNo]->SendJobValidation(Chamber->MesMode,
		edtRecipe->Text, Chamber->DeviceIDList);
}
//---------------------------------------------------------------------------
void __fastcall TFrmChamberSetting::btnBcrClick(TObject *Sender)
{
	if (EzNetDlg[ChamberNo] == nullptr)
	{
		return;
	}

	if (edtBcr->Text.IsEmpty() == false)
	{
//		EzNetDlg[ChamberNo]->SendBarcodeInfoEvent(edtBcr->Text);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::btnSetAlarmClick(TObject *Sender)
{
	if (EzNetDlg[ChamberNo] == nullptr)
	{
		return;
	}

	int AlmId = edtAlarmNo->Text.Trim().ToIntDef(0);
	EzNetDlg[ChamberNo]->SetAlarm(AlmId);
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::btnPPSelectClick(TObject *Sender)
{
	if (EzNetDlg[ChamberNo] == nullptr)
	{
		return;
	}

	EzNetDlg[ChamberNo]->SendPPSelectedEvent(edtRecipe->Text);
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::btnRecipeChangeClick(TObject *Sender)
{
	if (EzNetDlg[ChamberNo] == nullptr)
	{
		return;
	}

    long svid[2];
	String sVal[2];

	ZeroMemory(svid, 2);

	svid[0] = SVID_PORTID;
	sVal[0] = IntToStr(ChamberNo);
	svid[1] = SVID_PPID_CHANGE_NAME;
	sVal[1] = edtRecipe->Text;

	EzNetDlg[ChamberNo]->SetSVID(svid, sVal, 2);

	EzNetDlg[ChamberNo]->SendEventReport(CEID_PP_CHANGED);
}
//---------------------------------------------------------------------------

void __fastcall TFrmChamberSetting::rdgSelStateClick(TObject *Sender)
{
	if (EzNetDlg[ChamberNo] == nullptr)
	{
		return;
	}

	int nState;
	long svid[1];
	String sVal[1];

	ZeroMemory(svid, 1);

	svid[0] = SVID_PORTID;
	sVal[0] = IntToStr(ChamberNo);
	EzNetDlg[ChamberNo]->SetSVID(svid, sVal, 1);

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
#ifdef _DEBUG
	EzNetDlg[ChamberNo]->SetProcessState(nState);
#endif
}
//---------------------------------------------------------------------------

