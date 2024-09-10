//---------------------------------------------------------------------------

#ifndef SystemSettingFrameH
#define SystemSettingFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvCombo.hpp"
#include "AdvEdit.hpp"
#include "AdvGlassButton.hpp"
#include "AdvGrid.hpp"
#include "AdvGroupBox.hpp"
#include "AdvObj.hpp"
#include "AdvOfficeButtons.hpp"
#include "AdvPanel.hpp"
#include "AdvSelectors.hpp"
#include "AdvSmoothProgressBar.hpp"
#include "AdvSmoothSlider.hpp"
#include "AdvSmoothTabPager.hpp"
#include "AdvSmoothToggleButton.hpp"
#include "BaseGrid.hpp"
#include "ToolPanels.hpp"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class TFrmSystemSetting : public TFrame
{
__published:	// IDE-managed Components
	TAdvPanel *SystemSettingPanel;
	TAdvComboBox *MainTempControllerPortComboBox;
	TAdvComboBox *TempLimitPortComboBox;
	TAdvComboBox *LoggerPortComboBox;
	TAdvGlassButton *BtnSystemSave;
	TAdvGlassButton *BtnSystemCancel;
	TAdvPanel *AdvPanel2;
	TAdvComboBox *DIOPortComboBox;
	TAdvComboBox *DamperMotorPortComboBox;
	TAdvComboBox *DPPortComboBox;
	TAdvPanel *MainTempCtrlPanel;
	TAdvComboBox *ChamberAO2PortComboBox;
	TAdvComboBox *ChamberBO2PortComboBox;
	TAdvComboBox *ChamberCountComboBox;
	TAdvPanel *AdvPanel3;
	TAdvComboBox *LanguageComboBox;
	TAdvComboBox *DiffPressureTypeComboBox;
	TAdvComboBox *UseO2AnalyzerComboBox;
	TAdvComboBox *DamperControlTypeComboBox;
	TAdvPanel *AdvPanel1;
	TAdvComboBox *AutomationKindComboBox;
	TCheckBox *UseAutomationCheckBox;
	TAdvComboBox *UseMagazineDetectorComboBox;
	TAdvComboBox *UseWaterFlowDetectorComboBox;
	TAdvComboBox *UseAirPressureDetectorComboBox;
	TAdvComboBox *UseElectricPanelDoorDetectorComboBox;
	TAdvEdit *OFilmMES_DeviceID;
	TAdvEdit *OFilmMES_IP;
	TAdvEdit *OFilmMES_PORT;
	TAdvEdit *OFilmMES_ID;
	TAdvEdit *OFilmMES_PW;
	TAdvEdit *OFilmMES_FOLDER;
	TAdvPanel *AdvPanel4;
	TCheckBox *UseScreenSaverCheckBox;
	TAdvEdit *ScreenSaverCheckTime;
	TAdvComboBox *UseN2FlowDetectorComboBox;
	TAdvComboBox *DIOTypeComboBox;
	TAdvComboBox *UseInverterComboBox;
	TAdvComboBox *InverterPortComboBox;
	TAdvComboBox *UseN2FlowControlComboBox;
	TAdvComboBox *ChamberCO2PortComboBox;
	TAdvComboBox *ChamberDO2PortComboBox;
	TAdvComboBox *LoggerCountComboBox;
	TAdvComboBox *UseN2PressureDetectorComboBox;
	TAdvComboBox *UseHeaterOverloadDetectorComboBox;
	TAdvComboBox *UseDiffPressureDetectorComboBox;
	TAdvComboBox *UseSSROverTempDetectorComboBox;
	TAdvComboBox *UseChamberDoorTempInterlockComboBox;
	TAdvComboBox *UseGroundAlarmDetectorComboBox;
	TAdvComboBox *UseBlowMotorLowCurrentDetectorComboBox;
	TAdvComboBox *UseDoorLockCylinderComboBox;
	TAdvGlassButton *TowerLampSettingButton;
	TAdvComboBox *UseTowerLampControlComboBox;
	TAdvEdit *JobEndTimerEdit;
	TAdvEdit *DoorLockTempEdit;
	TAdvComboBox *UseDoorLockTempControlComboBox;
	TAdvComboBox *UsePasswordWhenIdleCoolingComboBox;
	TAdvComboBox *UseLoggerDataSaveComboBox;
	TAdvComboBox *ChamberAHanaPortComboBox;
	TAdvComboBox *ChamberBHanaPortComboBox;
	TAdvComboBox *ChamberCHanaPortComboBox;
	TAdvComboBox *ChamberDHanaPortComboBox;
	TAdvEdit *WaterCoolingExuastTimeEdit;
	TAdvComboBox *UseLocalAlarmComboBox;
	TAdvComboBox *DraBgSPComboBox;
	TAdvEdit *CureStartAlarmTimeEdit;
	TAdvEdit *ModeChangeTimerEdit;
	TCheckBox *UseCureEnableFromLocalCheckBox;
	TAdvComboBox *RfidReaderAPortComboBox;
	TCheckBox *RunRemoteCheckBox;
	TAdvPanel *EtcPanel;
	TCheckBox *UseCsvDataSaveCheckBox;
	TAdvEdit *MachineIdEdit;
	void __fastcall BtnSystemSaveClick(TObject *Sender);
	void __fastcall BtnSystemCancelClick(TObject *Sender);
	void __fastcall OnEditEnter(TObject *Sender);
	void __fastcall AutomationKindComboBoxChange(TObject *Sender);
	void __fastcall UseAutomationCheckBoxClick(TObject *Sender);
	void __fastcall UseScreenSaverCheckBoxClick(TObject *Sender);
	void __fastcall TowerLampSettingButtonClick(TObject *Sender);
	void __fastcall UseCureEnableFromLocalCheckBoxMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall UseCsvDataSaveCheckBoxClick(TObject *Sender);

private:	// User declarations
	void __fastcall InitSystemSetting();
	void __fastcall UpdateSystemSetting();
	void __fastcall ShowKeyboard(bool isNemeric);

public:		// User declarations
	__fastcall TFrmSystemSetting(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmSystemSetting *FrmSystemSetting;
//---------------------------------------------------------------------------
#endif
