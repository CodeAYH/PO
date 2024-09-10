//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SystemSettingFrame.h"
#include "HelperFunctions.h"
#include "NormalOvenChamber.h"
#include "NormalOvenSystemManager.h"
#include "MainForm.h"
#include "TowerSetForm.h"
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
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TFrmSystemSetting *FrmSystemSetting;
//---------------------------------------------------------------------------
__fastcall TFrmSystemSetting::TFrmSystemSetting(TComponent* Owner)
	: TFrame(Owner)
{
	InitSystemSetting();
}
//---------------------------------------------------------------------------
void __fastcall TFrmSystemSetting::BtnSystemSaveClick(TObject *Sender)
{
	SetMessageStr(GetResStr("Question", "QuestSave"));
	if( ShowQuestionDlg() )
	{
		UpdateSystemSetting();
		GetManager()->SaveSystemParameters();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmSystemSetting::BtnSystemCancelClick(TObject *Sender)
{
	GetManager()->LoadSystemParameters();
	InitSystemSetting();
}
//---------------------------------------------------------------------------
void __fastcall TFrmSystemSetting::OnEditEnter(TObject *Sender)
{
	ShowKeyboard(false);
	PostMessage(Handle, WM_USER+1, (WPARAM)Sender, 0);
}
//---------------------------------------------------------------------------
void __fastcall TFrmSystemSetting::UpdateSystemSetting()
{
	GetManager()->Language = LanguageComboBox->ItemIndex;

	int pos = 4;
	int len;

	String portName, port;

	portName = MainTempControllerPortComboBox->Items->Strings[MainTempControllerPortComboBox->ItemIndex];
	len = portName.Length() - pos + 1;
	port = portName.SubString(pos, len);
	if( port.IsEmpty() == false )
		GetManager()->MainTempCtrlPort = port.ToInt();

	portName = TempLimitPortComboBox->Items->Strings[TempLimitPortComboBox->ItemIndex];
	len = portName.Length() - pos + 1;
	port = portName.SubString(pos, len);
	if( port.IsEmpty() == false )
		GetManager()->ChamberTempLimitCtrlPort = port.ToInt();

	portName = LoggerPortComboBox->Items->Strings[LoggerPortComboBox->ItemIndex];
	len = portName.Length() - pos + 1;
	port = portName.SubString(pos, len);
	for(int i=0; i<GetManager()->LoggerCount; i++)
	{
		if( port.IsEmpty() == false )
			GetManager()->LoggerPort = port.ToInt();
	}

	portName = DIOPortComboBox->Items->Strings[DIOPortComboBox->ItemIndex];
	len = portName.Length() - pos + 1;
	port = portName.SubString(pos, len);
	if( port.IsEmpty() == false )
		GetManager()->DIOCtrlPort = port.ToInt();

	GetManager()->DPType = DiffPressureTypeComboBox->ItemIndex == 1 ? true : false;
	GetManager()->UseO2Analyzer = UseO2AnalyzerComboBox->ItemIndex == 1 ? true : false;
	GetManager()->UseMagazineDetector = UseMagazineDetectorComboBox->ItemIndex == 1 ? true : false;
	GetManager()->UseWaterFlowDetector = UseWaterFlowDetectorComboBox->ItemIndex == 1 ? true : false;
	GetManager()->UseN2FlowDetector = UseN2FlowDetectorComboBox->ItemIndex == 1 ? true : false;
	GetManager()->UseN2FlowControl = UseN2FlowControlComboBox->ItemIndex == 1 ? true : false;
	GetManager()->UseAirPressureDetector = UseAirPressureDetectorComboBox->ItemIndex == 1 ? true : false;
	GetManager()->UseElectricPanelDoorDetector = UseElectricPanelDoorDetectorComboBox->ItemIndex == 1 ? true : false;
	GetManager()->DamperControlType = DamperControlTypeComboBox->ItemIndex;
	GetManager()->DIOType = DIOTypeComboBox->ItemIndex;
	GetManager()->UseInverter = UseInverterComboBox->ItemIndex;
    GetManager()->UseN2PressureDetector = UseN2PressureDetectorComboBox->ItemIndex == 1 ? true : false;
    GetManager()->UseHeaterOverloadDetector = UseHeaterOverloadDetectorComboBox->ItemIndex == 1 ? true : false;
	GetManager()->UseDiffPressureDetector = UseDiffPressureDetectorComboBox->ItemIndex == 1 ? true : false;
    GetManager()->UseSSROverTempDetector = UseSSROverTempDetectorComboBox->ItemIndex == 1 ? true : false;
    GetManager()->UseChamberDoorTempInterlock = UseChamberDoorTempInterlockComboBox->ItemIndex == 1 ? true : false;
	GetManager()->UseGroundAlarmDetector = UseGroundAlarmDetectorComboBox->ItemIndex == 1 ? true : false;
	GetManager()->UseBlowMotorLowCurrentDetector = UseBlowMotorLowCurrentDetectorComboBox->ItemIndex == 1 ? true : false;
	GetManager()->UseDoorLockCylinder = UseDoorLockCylinderComboBox->ItemIndex == 1 ? true : false;
	GetManager()->UseTowerLampControl = UseTowerLampControlComboBox->ItemIndex == 1 ? true : false;
	TowerLampSettingButton->Visible = GetManager()->UseTowerLampControl;
	// 2020-12-16 Add 챔버온도 도어락 사용유무
	GetManager()->UseDoorLockTempControl = UseDoorLockTempControlComboBox->ItemIndex == 1 ? true : false;
	// 2021-03-05 Add IdleCooling일때 패스워스 사용유무
	GetManager()->UsePasswordWhenIdleCoolingStop = UsePasswordWhenIdleCoolingComboBox->ItemIndex == 1 ? true : false;
	// 2021-03-15 ADD Logger 데이터 저장 사용유무
	GetManager()->UseLoggerDataSave = UseLoggerDataSaveComboBox->ItemIndex == 1 ? true : false;
	// 2021-09-09 Local Mode N2 Alarm Check
	GetManager()->UseLocalN2Alarm = UseLocalAlarmComboBox->ItemIndex == 1 ? true : false;
	// 2021-11-09 yjyoo, 메인화면 온도 그래프에 Set Point 그래프를 백그라운드로 그릴지 여부
	GetManager()->DrawBgSP = DraBgSPComboBox->ItemIndex == 1 ? true : false;
	// 2023-01-30 Change from flow meter to flow switch
 //	GetManager()->UseN2PurgeDetector = UseN2PurgeDetectorComboBox->ItemIndex == 1 ? true : false; //2023-04-17 restore
//	GetManager()->UseCDAFlowDetector = UseCDAFlowDetectorComboBox->ItemIndex == 1 ? true : false; //2023-04-17 restore
	// 2023-01-30 SiliconBox Add Option
//	GetManager()->UseSafetyReset = UseSafetyResetComboBox->ItemIndex == 1 ? true : false;   //2023-04-17 restore
	//로컬 상태에서 GEM Remote메세지로 큐어 시작 가능 사용유무
	GetManager()->UseCureStartEnableFromLocalState = UseCureEnableFromLocalCheckBox->Checked ? true : false;

	if( GetManager()->DPType )
	{
		DPPortComboBox->Visible = true;
		portName = DPPortComboBox->Items->Strings[DPPortComboBox->ItemIndex];
		len = portName.Length() - pos + 1;
		port = portName.SubString(pos, len);
		if( port.IsEmpty() == false )
			GetManager()->DPCtrlPort = port.ToInt();
	}

	portName = DamperMotorPortComboBox->Items->Strings[DamperMotorPortComboBox->ItemIndex];
	len = portName.Length() - pos + 1;
	port = portName.SubString(pos, len);
	if( port.IsEmpty() == false )
		GetManager()->DamperMotorCtrlPort = port.ToInt();

	portName = ChamberAO2PortComboBox->Items->Strings[ChamberAO2PortComboBox->ItemIndex];
	len = portName.Length() - pos + 1;
	port = portName.SubString(pos, len);
	if( port.IsEmpty() == false )
		GetManager()->O2AnalyzerAPort = port.ToInt();

	portName = ChamberBO2PortComboBox->Items->Strings[ChamberBO2PortComboBox->ItemIndex];
	len = portName.Length() - pos + 1;
	port = portName.SubString(pos, len);
	if( port.IsEmpty() == false )
		GetManager()->O2AnalyzerBPort = port.ToInt();

	portName = ChamberCO2PortComboBox->Items->Strings[ChamberCO2PortComboBox->ItemIndex];
	len = portName.Length() - pos + 1;
	port = portName.SubString(pos, len);
	if( port.IsEmpty() == false )
		GetManager()->O2AnalyzerCPort = port.ToInt();

	portName = ChamberDO2PortComboBox->Items->Strings[ChamberDO2PortComboBox->ItemIndex];
	len = portName.Length() - pos + 1;
	port = portName.SubString(pos, len);
	if( port.IsEmpty() == false )
		GetManager()->O2AnalyzerDPort = port.ToInt();

	portName = InverterPortComboBox->Items->Strings[InverterPortComboBox->ItemIndex];
	len = portName.Length() - pos + 1;
	port = portName.SubString(pos, len);
	if( port.IsEmpty() == false )
		GetManager()->InverterPort = port.ToInt();

	int idx = (int)pow(2.0, (double)ChamberCountComboBox->ItemIndex);
	GetManager()->ChamberCount = idx;
	GetManager()->LoggerCount = LoggerCountComboBox->ItemIndex + 1;

	GetManager()->UseAutomation = UseAutomationCheckBox->Checked;
	GetManager()->AutomationKind = AutomationKindComboBox->ItemIndex < 0 ? 0 : AutomationKindComboBox->ItemIndex;

	portName = RfidReaderAPortComboBox->Items->Strings[RfidReaderAPortComboBox->ItemIndex];
	len = portName.Length() - pos + 1;
	port = portName.SubString(pos, len);
	if( port.IsEmpty() == false )
		GetManager()->RfidReaderAPort = port.ToInt();

	// 20180209 - FTP (OFILM MES) - jhlim
	if(GetManager()->AutomationKind == OFILM_MES)
	{
		ftpConnectInfo.DeviceID 	= OFilmMES_DeviceID->Text;
		ftpConnectInfo.Ftp_IP 		= OFilmMES_IP->Text;
		port = OFilmMES_PORT->Text;
		if(port != "")
		ftpConnectInfo.Ftp_PORT 	= port.ToInt();
		ftpConnectInfo.Ftp_ID 		= OFilmMES_ID->Text;
		ftpConnectInfo.Ftp_PW 		= OFilmMES_PW->Text;
		ftpConnectInfo.Ftp_FOLDER 	= OFilmMES_FOLDER->Text;
	}

	// 20181029 - ScreenSaver (infineon) - jhlim
	GetManager()->UseScreenSaver = UseScreenSaverCheckBox->Checked;
	if(GetManager()->UseScreenSaver == true)
	{
		port = ScreenSaverCheckTime->Text;
		GetManager()->ScreenSaverTime = port.ToInt();
	}

	GetManager()->JobEndTimer = JobEndTimerEdit->Text.ToInt();

	// 2020-12-17 Add 챔버온도 도어락 저장.
	if(GetManager()->UseDoorLockTempControl)
	{
		if(DoorLockTempEdit->Text == "")
			DoorLockTempEdit->Text = 0;

		GetManager()->DoorLockTemp = DoorLockTempEdit->Text.ToInt();
	}

	// 2021-06-09 KJH 쿨링워터 배출시간 0일경우 작동안함
	GetManager()->CoolingVentExuastTime = WaterCoolingExuastTimeEdit->Text.ToInt();

	if(GetManager()->AutomationKind == HANA_CIM && UseAutomationCheckBox->Checked == true)
	{
		ChamberAHanaPortComboBox->Visible = true;
		portName = ChamberAHanaPortComboBox->Items->Strings[ChamberAHanaPortComboBox->ItemIndex];
		len = portName.Length() - pos + 1;
		port = portName.SubString(pos, len);
		if( port.IsEmpty() == false )
			GetManager()->REXP250APort = port.ToInt();

		ChamberBHanaPortComboBox->Visible = true;
		portName = ChamberBHanaPortComboBox->Items->Strings[ChamberBHanaPortComboBox->ItemIndex];
		len = portName.Length() - pos + 1;
		port = portName.SubString(pos, len);
		if( port.IsEmpty() == false )
			GetManager()->REXP250BPort = port.ToInt();

		ChamberCHanaPortComboBox->Visible = true;
		portName = ChamberCHanaPortComboBox->Items->Strings[ChamberCHanaPortComboBox->ItemIndex];
		len = portName.Length() - pos + 1;
		port = portName.SubString(pos, len);
		if( port.IsEmpty() == false )
			GetManager()->REXP250CPort = port.ToInt();

		ChamberDHanaPortComboBox->Visible = true;
		portName = ChamberDHanaPortComboBox->Items->Strings[ChamberDHanaPortComboBox->ItemIndex];
		len = portName.Length() - pos + 1;
		port = portName.SubString(pos, len);
		if( port.IsEmpty() == false )
			GetManager()->REXP250DPort = port.ToInt();
	}
	else
	{
		ChamberAHanaPortComboBox->Visible = false;
		ChamberBHanaPortComboBox->Visible = false;
		ChamberCHanaPortComboBox->Visible = false;
		ChamberDHanaPortComboBox->Visible = false;
	}

	if(GetManager()->AutomationKind == SECS_GEM_COWELL && UseAutomationCheckBox->Checked == true)
	{
		RunRemoteCheckBox->Visible = true;
		GetManager()->UseRemoteCheck = RunRemoteCheckBox->Checked;
	}
	else
	{
		RunRemoteCheckBox->Visible = false;
    }

	// 2021-12-01 KJH 큐어시작 알람
	GetManager()->CureStartAlarmTime = CureStartAlarmTimeEdit->Text.ToIntDef(0);

	if (ModeChangeTimerEdit->Visible)
	{
		GetManager()->AutoModeChangeTimer = ModeChangeTimerEdit->Text.ToIntDef(0);
	}

	GetManager()->UseCsvDataSave = UseCsvDataSaveCheckBox->Checked;
	if(UseCsvDataSaveCheckBox->Checked == true)
	{
		MachineIdEdit->Visible = true;
		GetManager()->CsvMachineId = MachineIdEdit->Text;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmSystemSetting::InitSystemSetting()
{
	MainTempCtrlPanel->Visible = GetManager()->LoginLevel == LV_DEVELOPER ? true : false;

	LanguageComboBox->Clear();
	for(int i=0; i<LANG_COUNT; i++)
		LanguageComboBox->Items->Add(LangStr[i]);

	LanguageComboBox->ItemIndex = GetManager()->Language;

	TStringList *list = new TStringList;

	MainTempControllerPortComboBox->Clear();
	TempLimitPortComboBox->Clear();
	LoggerPortComboBox->Clear();
	DIOPortComboBox->Clear();
	DamperMotorPortComboBox->Clear();
	DPPortComboBox->Clear();
	ChamberAO2PortComboBox->Clear();
	ChamberBO2PortComboBox->Clear();
	ChamberCO2PortComboBox->Clear();
	ChamberDO2PortComboBox->Clear();
	InverterPortComboBox->Clear();
	RfidReaderAPortComboBox->Clear();

	if(GetManager()->AutomationKind == HANA_CIM)
	{
		ChamberAHanaPortComboBox->Clear();		// 2021-04-14
		ChamberBHanaPortComboBox->Clear();		// 2021-04-14
		ChamberCHanaPortComboBox->Clear();		// 2021-04-14
		ChamberDHanaPortComboBox->Clear();		// 2021-04-14
	}

	DPPortComboBox->Visible = GetManager()->DPType;
	DamperMotorPortComboBox->Visible = (GetManager()->DamperControlType != CYLINDER_DAMPER) ? true : false;
	ChamberAO2PortComboBox->Visible = GetManager()->UseO2Analyzer;
	ChamberBO2PortComboBox->Visible = GetManager()->UseO2Analyzer;
	ChamberCO2PortComboBox->Visible = GetManager()->UseO2Analyzer;
	ChamberDO2PortComboBox->Visible = GetManager()->UseO2Analyzer;
	InverterPortComboBox->Visible = GetManager()->UseInverter;

	if( GetSerialList(list) )
	{
		for(int i=0; i<list->Count; i++)
		{
			MainTempControllerPortComboBox->Items->Add(list->Strings[i]);
			TempLimitPortComboBox->Items->Add(list->Strings[i]);
			LoggerPortComboBox->Items->Add(list->Strings[i]);
			DIOPortComboBox->Items->Add(list->Strings[i]);
			DamperMotorPortComboBox->Items->Add(list->Strings[i]);
			if( GetManager()->DPType )
				DPPortComboBox->Items->Add(list->Strings[i]);
			ChamberAO2PortComboBox->Items->Add(list->Strings[i]);
			ChamberBO2PortComboBox->Items->Add(list->Strings[i]);
			ChamberCO2PortComboBox->Items->Add(list->Strings[i]);
			ChamberDO2PortComboBox->Items->Add(list->Strings[i]);
			InverterPortComboBox->Items->Add(list->Strings[i]);

			ChamberAHanaPortComboBox->Items->Add(list->Strings[i]);		// 2021-04-14
			ChamberBHanaPortComboBox->Items->Add(list->Strings[i]);		// 2021-04-14
			ChamberCHanaPortComboBox->Items->Add(list->Strings[i]);		// 2021-04-14
			ChamberDHanaPortComboBox->Items->Add(list->Strings[i]);		// 2021-04-14

			RfidReaderAPortComboBox->Items->Add(list->Strings[i]);
		}
	}

	String str;
	str.sprintf(L"COM%d", GetManager()->MainTempCtrlPort);
	MainTempControllerPortComboBox->ItemIndex = MainTempControllerPortComboBox->Items->IndexOf(str);

	str.sprintf(L"COM%d", GetManager()->ChamberTempLimitCtrlPort);
	TempLimitPortComboBox->ItemIndex = TempLimitPortComboBox->Items->IndexOf(str);

	str.sprintf(L"COM%d", GetManager()->LoggerPort);
	LoggerPortComboBox->ItemIndex = LoggerPortComboBox->Items->IndexOf(str);

	str.sprintf(L"COM%d", GetManager()->DIOCtrlPort);
	DIOPortComboBox->ItemIndex = DIOPortComboBox->Items->IndexOf(str);

	if( GetManager()->DPType )
	{
		str.sprintf(L"COM%d", GetManager()->DPCtrlPort);
		DPPortComboBox->ItemIndex = DPPortComboBox->Items->IndexOf(str);
	}

	str.sprintf(L"COM%d", GetManager()->DamperMotorCtrlPort);
	DamperMotorPortComboBox->ItemIndex = DamperMotorPortComboBox->Items->IndexOf(str);

	str.sprintf(L"COM%d", GetManager()->O2AnalyzerAPort);
	ChamberAO2PortComboBox->ItemIndex = ChamberAO2PortComboBox->Items->IndexOf(str);

	str.sprintf(L"COM%d", GetManager()->O2AnalyzerBPort);
	ChamberBO2PortComboBox->ItemIndex = ChamberBO2PortComboBox->Items->IndexOf(str);

	str.sprintf(L"COM%d", GetManager()->O2AnalyzerCPort);
	ChamberCO2PortComboBox->ItemIndex = ChamberCO2PortComboBox->Items->IndexOf(str);

	str.sprintf(L"COM%d", GetManager()->O2AnalyzerDPort);
	ChamberDO2PortComboBox->ItemIndex = ChamberDO2PortComboBox->Items->IndexOf(str);

	str.sprintf(L"COM%d", GetManager()->InverterPort);
	InverterPortComboBox->ItemIndex = InverterPortComboBox->Items->IndexOf(str);

	if(GetManager()->AutomationKind == HANA_CIM)
	{
		str.sprintf(L"COM%d", GetManager()->REXP250APort);
		ChamberAHanaPortComboBox->ItemIndex = ChamberAHanaPortComboBox->Items->IndexOf(str);
		str.sprintf(L"COM%d", GetManager()->REXP250BPort);
		ChamberBHanaPortComboBox->ItemIndex = ChamberBHanaPortComboBox->Items->IndexOf(str);
		str.sprintf(L"COM%d", GetManager()->REXP250CPort);
		ChamberCHanaPortComboBox->ItemIndex = ChamberCHanaPortComboBox->Items->IndexOf(str);
		str.sprintf(L"COM%d", GetManager()->REXP250DPort);
		ChamberDHanaPortComboBox->ItemIndex = ChamberDHanaPortComboBox->Items->IndexOf(str);
	}

	if(GetManager()->AutomationKind == SECS_GEM_SCK)
	{
		str.sprintf(L"COM%d", GetManager()->RfidReaderAPort);
		RfidReaderAPortComboBox->ItemIndex = RfidReaderAPortComboBox->Items->IndexOf(str);
	}

	DiffPressureTypeComboBox->ItemIndex = GetManager()->DPType ? 1 : 0;
	UseO2AnalyzerComboBox->ItemIndex = GetManager()->UseO2Analyzer ? 1 : 0;
	DIOTypeComboBox->ItemIndex = GetManager()->DIOType;
	UseInverterComboBox->ItemIndex = GetManager()->UseInverter;

	UseMagazineDetectorComboBox->ItemIndex = GetManager()->UseMagazineDetector ? 1 : 0;
	UseWaterFlowDetectorComboBox->ItemIndex = GetManager()->UseWaterFlowDetector ? 1 : 0;
	UseN2FlowDetectorComboBox->ItemIndex = GetManager()->UseN2FlowDetector ? 1 : 0;
	UseN2FlowControlComboBox->ItemIndex = GetManager()->UseN2FlowControl ? 1 : 0;
	UseAirPressureDetectorComboBox->ItemIndex = GetManager()->UseAirPressureDetector ? 1 : 0;
	UseElectricPanelDoorDetectorComboBox->ItemIndex = GetManager()->UseElectricPanelDoorDetector ? 1 : 0;
    UseN2PressureDetectorComboBox->ItemIndex = GetManager()->UseN2PressureDetector ? 1 : 0;
    UseHeaterOverloadDetectorComboBox->ItemIndex = GetManager()->UseHeaterOverloadDetector ? 1 : 0;
    UseDiffPressureDetectorComboBox->ItemIndex = GetManager()->UseDiffPressureDetector ? 1 : 0;
    UseSSROverTempDetectorComboBox->ItemIndex = GetManager()->UseSSROverTempDetector ? 1 : 0;
	UseChamberDoorTempInterlockComboBox->ItemIndex = GetManager()->UseChamberDoorTempInterlock ? 1 : 0;
	UseGroundAlarmDetectorComboBox->ItemIndex = GetManager()->UseGroundAlarmDetector ? 1 : 0;
	UseBlowMotorLowCurrentDetectorComboBox->ItemIndex = GetManager()->UseBlowMotorLowCurrentDetector ? 1 : 0;
	UseDoorLockCylinderComboBox->ItemIndex = GetManager()->UseDoorLockCylinder ? 1 : 0;
	UseTowerLampControlComboBox->ItemIndex = GetManager()->UseTowerLampControl ? 1 : 0;
	// 2020-12-16 Add 챔버온도 도어락 사용유무
	UseDoorLockTempControlComboBox->ItemIndex = GetManager()->UseDoorLockTempControl ? 1 : 0;
	// 2021-03-05 Add IdleCooling일때 패스워스 사용유무
	UsePasswordWhenIdleCoolingComboBox->ItemIndex = GetManager()->UsePasswordWhenIdleCoolingStop ? 1 : 0;
	// 2021-03-15 ADD Logger 데이터 저장 사용유무
	UseLoggerDataSaveComboBox->ItemIndex = GetManager()->UseLoggerDataSave ? 1 : 0;
	//2021-09-09 Local Mode N2 Alarm Check
	UseLocalAlarmComboBox->ItemIndex = GetManager()->UseLocalN2Alarm ? 1 : 0;
	// 2021-11-09 yjyoo, 메인화면 온도 그래프에 Set Point 그래프를 백그라운드로 그릴지 여부
	DraBgSPComboBox->ItemIndex = GetManager()->DrawBgSP ? 1 : 0;
	// 2023-01-30 Change from flow meter to flow switch
//	UseN2PurgeDetectorComboBox->ItemIndex = GetManager()->UseN2PurgeDetector ? 1 : 0; //2023-04-17 restore
 //	UseCDAFlowDetectorComboBox->ItemIndex = GetManager()->UseCDAFlowDetector ? 1 : 0; //2023-04-17 restore
//	UseSafetyResetComboBox->ItemIndex = GetManager()->UseSafetyReset ? 1 : 0; //2023-04-17 restore
    //로컬 상태에서 GEM Remote메세지로 큐어 시작 가능 사용유무
	UseCureEnableFromLocalCheckBox->Checked = GetManager()->UseCureStartEnableFromLocalState ? true : false;

	str.sprintf(L"%d", GetManager()->ChamberCount);
	ChamberCountComboBox->ItemIndex = ChamberCountComboBox->Items->IndexOf(str);
	LoggerCountComboBox->ItemIndex = GetManager()->LoggerCount - 1;
	DamperControlTypeComboBox->ItemIndex = GetManager()->DamperControlType;

	UseAutomationCheckBox->Checked = GetManager()->UseAutomation;
	RunRemoteCheckBox->Checked = GetManager()->UseRemoteCheck;

	AutomationKindComboBox->Clear();
	for(int i=0; i<AUTOMATIONKIND_COUNT; i++)
		AutomationKindComboBox->Items->Add(AutomationKindStr[i]);
	AutomationKindComboBox->ItemIndex = GetManager()->AutomationKind;
	// 20180212 - FTP (OFILM MES) - jhlim
	//AutomationKindComboBox->Visible = GetManager()->LoginLevel == LV_DEVELOPER ? true : false;
	if (GetManager()->UseAutomation == true)
	{
		AutomationKindComboBox->Visible = true;
		if (GetManager()->AutomationKind == HANA_CIM)
		{
			ChamberAHanaPortComboBox->Visible = true;
			ChamberBHanaPortComboBox->Visible = true;
			ChamberCHanaPortComboBox->Visible = true;
			ChamberDHanaPortComboBox->Visible = true;
		}
		else
		{
			ChamberAHanaPortComboBox->Visible = false;
			ChamberBHanaPortComboBox->Visible = false;
			ChamberCHanaPortComboBox->Visible = false;
			ChamberDHanaPortComboBox->Visible = false;
        }
	}
	else
	{
		AutomationKindComboBox->Visible = false;

		ChamberAHanaPortComboBox->Visible = false;
		ChamberBHanaPortComboBox->Visible = false;
		ChamberCHanaPortComboBox->Visible = false;
		ChamberDHanaPortComboBox->Visible = false;
	}
	TowerLampSettingButton->Visible = GetManager()->UseTowerLampControl;

	// 20180209 - FTP (OFILM MES) - jhlim
	if(GetManager()->UseAutomation == true
		&& AutomationKindComboBox->Visible == true
		&& GetManager()->AutomationKind == OFILM_MES)
	{
		OFilmMES_DeviceID->Text = ftpConnectInfo.DeviceID;
		OFilmMES_IP->Text		= ftpConnectInfo.Ftp_IP;
		str.sprintf(L"%d", ftpConnectInfo.Ftp_PORT);
		OFilmMES_PORT->Text		= str;
		OFilmMES_ID->Text		= ftpConnectInfo.Ftp_ID;
		OFilmMES_PW->Text		= ftpConnectInfo.Ftp_PW;
		OFilmMES_FOLDER->Text	= ftpConnectInfo.Ftp_FOLDER;

		OFilmMES_DeviceID->Visible = true;
		OFilmMES_IP->Visible = true;
		OFilmMES_PORT->Visible = true;
		OFilmMES_ID->Visible = true;
		OFilmMES_PW->Visible = true;
		OFilmMES_FOLDER->Visible = true;
	}
	else
	{
		OFilmMES_DeviceID->Text = "";
		OFilmMES_IP->Text = "";
		OFilmMES_PORT->Text = "";
		OFilmMES_ID->Text = "";
		OFilmMES_PW->Text = "";
		OFilmMES_FOLDER->Text = "";

		OFilmMES_DeviceID->Visible = false;
		OFilmMES_IP->Visible = false;
		OFilmMES_PORT->Visible = false;
		OFilmMES_ID->Visible = false;
		OFilmMES_PW->Visible = false;
		OFilmMES_FOLDER->Visible = false;
	}

	// 20181029 - ScreenSaver (infineon) - jhlim
	UseScreenSaverCheckBox->Checked = GetManager()->UseScreenSaver;
	if(GetManager()->UseScreenSaver == true)
	{
		str.sprintf(L"%d", GetManager()->ScreenSaverTime);
		ScreenSaverCheckTime->Text = str;
		ScreenSaverCheckTime->Visible = true;
	}
	else
	{
		ScreenSaverCheckTime->Text = "";
		ScreenSaverCheckTime->Visible = false;
	}

	str.sprintf(L"%d", GetManager()->JobEndTimer);
	JobEndTimerEdit->Text = str;

	// 2020-12-16 Add 챔버온도 도어락
	if (GetManager()->UseDoorLockTempControl == true && GetManager()->LoginLevel >= LV_ENGINEER)
	{
		DoorLockTempEdit->Visible = true;
		str.sprintf(L"%d", GetManager()->DoorLockTemp );
		DoorLockTempEdit->Text = str;
	}
	else
	{
		DoorLockTempEdit->Visible = false;
	}

	//JSCK RFID Reader Port
	if (GetManager()->UseAutomation == true && GetManager()->AutomationKind == SECS_GEM_SCK)
	{
		RfidReaderAPortComboBox->Visible = GetManager()->ChamberCount >= 1 ? true : false;
	}
	else
	{
		RfidReaderAPortComboBox->Visible = false;
	}

	if(GetManager()->AutomationKind == SECS_GEM_COWELL && UseAutomationCheckBox->Checked == true)
	{
		RunRemoteCheckBox->Visible = true;
	}
	else
	{
		RunRemoteCheckBox->Visible = false;
    }

	// 2021-06-09 KJH 쿨링워터 배출시간
	WaterCoolingExuastTimeEdit->Text = GetManager()->CoolingVentExuastTime;

	// 2021-12-01 KJH 큐어시작 알람
	CureStartAlarmTimeEdit->Text = GetManager()->CureStartAlarmTime;

	ModeChangeTimerEdit->Text = GetManager()->AutoModeChangeTimer;

	// 2023-09-11 Cowell Csv File Save
	UseCsvDataSaveCheckBox->Checked = GetManager()->UseCsvDataSave;
	MachineIdEdit->Visible = GetManager()->UseCsvDataSave ? true : false;
	MachineIdEdit->Text = GetManager()->CsvMachineId;

	delete list;
}
//---------------------------------------------------------------------------
void __fastcall TFrmSystemSetting::ShowKeyboard(bool isNemeric)
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
//---------------------------------------------------------------------------
void __fastcall TFrmSystemSetting::AutomationKindComboBoxChange(TObject *Sender)
{
	bool bSelect = false;

	bSelect = (AutomationKindComboBox->ItemIndex == OFILM_MES && UseAutomationCheckBox->Checked);
	OFilmMES_DeviceID->Visible = bSelect;
	OFilmMES_IP->Visible = bSelect;
	OFilmMES_PORT->Visible = bSelect;
	OFilmMES_ID->Visible = bSelect;
	OFilmMES_PW->Visible = bSelect;
	OFilmMES_FOLDER->Visible = bSelect;

	bSelect = (AutomationKindComboBox->ItemIndex == HANA_CIM && UseAutomationCheckBox->Checked);
	ChamberAHanaPortComboBox->Visible = bSelect;
	ChamberBHanaPortComboBox->Visible = bSelect;
	ChamberCHanaPortComboBox->Visible = bSelect;
	ChamberDHanaPortComboBox->Visible = bSelect;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSystemSetting::UseAutomationCheckBoxClick(TObject *Sender)
{
	// Use Automation CheckBox
	bool bSelect = UseAutomationCheckBox->Checked;
	AutomationKindComboBox->Visible = bSelect;
	if(AutomationKindComboBox->Visible)
	{
		AutomationKindComboBox->ItemIndex = GetManager()->AutomationKind;
	}

	bSelect = (AutomationKindComboBox->ItemIndex == SECS_GEM && UseAutomationCheckBox->Checked);
	UseCureEnableFromLocalCheckBox->Visible = bSelect;

	bSelect = (AutomationKindComboBox->ItemIndex == OFILM_MES && UseAutomationCheckBox->Checked);
	OFilmMES_DeviceID->Visible = bSelect;
	OFilmMES_IP->Visible = bSelect;
	OFilmMES_PORT->Visible = bSelect;
	OFilmMES_ID->Visible = bSelect;
	OFilmMES_PW->Visible = bSelect;
	OFilmMES_FOLDER->Visible = bSelect;

	bSelect = (AutomationKindComboBox->ItemIndex == HANA_CIM && UseAutomationCheckBox->Checked);
	ChamberAHanaPortComboBox->Visible = bSelect;
	ChamberBHanaPortComboBox->Visible = bSelect;
	ChamberCHanaPortComboBox->Visible = bSelect;
	ChamberDHanaPortComboBox->Visible = bSelect;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSystemSetting::UseScreenSaverCheckBoxClick(TObject *Sender)
{
	// Use Screen Saver CheckBox
	bool bSelect = false;

	if(UseScreenSaverCheckBox->Checked == true)
	{
		bSelect = true;
	}
	else
	{
		bSelect = false;
	}

	ScreenSaverCheckTime->Visible = bSelect;
}
//---------------------------------------------------------------------------

void __fastcall TFrmSystemSetting::TowerLampSettingButtonClick(TObject *Sender)
{
	ShowTowerLampSet();
//	RunSwLamp->RefreshTowerSetVal();
}
//---------------------------------------------------------------------------
void __fastcall TFrmSystemSetting::UseCureEnableFromLocalCheckBoxMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y)
{
	SetMessageStr(GetResStr("Question", "QuestCureEnableFromLocal"));
	if( ShowQuestionDlg() )
	{
		UseCureEnableFromLocalCheckBox->Checked = true;
		return;
	}
	UseCureEnableFromLocalCheckBox->Checked = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmSystemSetting::UseCsvDataSaveCheckBoxClick(TObject *Sender)
{
	MachineIdEdit->Visible = UseCsvDataSaveCheckBox->Checked ? true : false;
}
//---------------------------------------------------------------------------

