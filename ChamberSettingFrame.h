//---------------------------------------------------------------------------

#ifndef ChamberSettingFrameH
#define ChamberSettingFrameH
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
#include "Defines.h"
#include "AdvUtil.hpp"
#include <Vcl.Graphics.hpp>
//---------------------------------------------------------------------------
class TFrmChamberSetting : public TFrame
{
__published:	// IDE-managed Components
	TAdvSmoothTabPager *SettingTabPager;
	TAdvSmoothTabPage *PatternSetting;
	TAdvSmoothTabPage *ChamberSetting;
	TAdvPanel *ChamberSettingPanel;
	TAdvGlassButton *BtnChamberSave;
	TAdvGlassButton *BtnChamberCancel;
	TAdvPanel *TempLimitPanel;
	TAdvPanel *AddFunctionPanel;
	TAdvEdit *OffsetHighEdit;
	TAdvEdit *TempLimitBiasEdit;
	TTimer *Refresh;
	TAdvPanel *WorkPanel;
	TAdvPanel *OxygenPanel;
	TAdvEdit *O2DensityLimitEdit;
	TAdvEdit *O2DensityAlarmEnableTimeEdit;
	TCheckBox *CureStopOnO2DensityAlarmCheckBox;
	TCheckBox *CannotStartWithoutLotInfoCheckBox;
	TCheckBox *AutoStartCheckBox;
	TCheckBox *O2AnalyzeUseRunLimitCheckBox;
	TAdvEdit *O2DensityAlarmRunLimitTime;
	TAdvEdit *O2DensityRunLimitEdit;
	TAdvSmoothTabPage *FactorySetting;
	TAdvPanel *FactorySettingPanels;
	TAdvGlassButton *BtnChamberFactorySave;
	TAdvGlassButton *BtnChamberFactoryCancel;
	TAdvPanel *BarunMesPanel;
	TAdvEdit *MachineIDEdit;
	TCheckBox *UseManualModeCheckBox;
	TAdvPanel *FactorySettingPanel;
	TAdvComboBox *MainTempCtrlTypeComboBox;
	TAdvPanel *DamperMotorPanel;
	TAdvGlassButton *CloseButton;
	TAdvGlassButton *HomeButton;
	TAdvGlassButton *OpenButton;
	TAdvGlassButton *EnableButton;
	TAdvGlassButton *DisableButton;
	TAdvPanel *MoonsMotorWarningPanel;
	TLabeledEdit *CCWLimitIndicator;
	TLabeledEdit *CWLimitIndicator;
	TLabeledEdit *UnderVoltageIndicator;
	TLabeledEdit *CommErrorIndicator;
	TLabeledEdit *BadFlashIndicator;
	TLabeledEdit *NoMoveIndicator;
	TLabeledEdit *BlankQSegmentIndicator;
	TAdvPanel *MoonsMotorAlarmPanel;
	TLabeledEdit *OverTempIndicator;
	TLabeledEdit *InternalVoltageIndicator;
	TLabeledEdit *OverVoltageIndicator;
	TLabeledEdit *OverCurrentIndicator;
	TLabeledEdit *BadEncoderIndicator;
	TLabeledEdit *OpenWindingIndicator;
	TAdvGlassButton *AlarmClearButton;
	TAdvPanel *MainTempControllerPanel;
	TAdvStringGrid *GrdUP550Instruction;
	TAdvGlassButton *BtnUP550Setting;
	TAdvSmoothProgressBar *UP550ProgressBar;
	TAdvPanel *TempLimitCtrlPanel;
	TAdvComboBox *CbxTempLimitCtrlType;
	TAdvPanel *EziMotorStatusPanel;
	TLabeledEdit *ErrorAllIndicator;
	TLabeledEdit *HWPlusLimitIndicator;
	TLabeledEdit *HWMinusLimitIndicator;
	TLabeledEdit *SWPlusLimitIndicator;
	TLabeledEdit *SWMinusLimitIndicator;
	TLabeledEdit *ErrStepAlarmIndicator;
	TLabeledEdit *ErrOverCurrentIndicator;
	TLabeledEdit *ErrOverSpeedIndicator;
	TLabeledEdit *ErrSpeedIndicator;
	TLabeledEdit *ErrOverHeatIndicator;
	TLabeledEdit *ErrRevPowerIndicator;
	TLabeledEdit *ErrMotorPowerIndicator;
	TLabeledEdit *ErrLowPowerIndicator;
	TLabeledEdit *EmgStopIndicator;
	TLabeledEdit *LowStopIndicator;
	TLabeledEdit *OrgReturningIndicator;
	TLabeledEdit *AlarmResetIndicator;
	TLabeledEdit *PTStoppedIndicator;
	TLabeledEdit *OrgSensorIndicator;
	TLabeledEdit *ZPulseIndicator;
	TLabeledEdit *OrgRetOKIndicator;
	TLabeledEdit *MotionDirIndicator;
	TLabeledEdit *MotioningIndicator;
	TLabeledEdit *MotionPasueIndicator;
	TLabeledEdit *MotionAccelIndicator;
	TLabeledEdit *MotionDecelIndicator;
	TAdvPanel *IdleCoolingPanel;
	TAdvEdit *IdleCoolingTempEdit;
	TAdvEdit *IdleCoolingOffsetEdit;
	TCheckBox *UseIdleCoolingCheckBox;
	TAdvEdit *LargeN2ValveOpenO2DensityEdit;
	TCheckBox *AllowRecipeSelectionCheckBox;
	TCheckBox *GotoIdleStepCheckBox;
	TAdvEdit *LoggerDeviationEdit;
	TAdvGroupBox *TCardGroupBox;
	TAdvEdit *ChamberSelectEdit;
	TAdvEdit *StartCureEdit;
	TCheckBox *RecipeMaintainCheckBox;
	TAdvEdit *DoorOpenAlarmEdit;
	TAdvSmoothTabPage *GemPage;
	TAdvPanel *GemPagePanel;
	TAdvGlassButton *btnBcr;
	TAdvGlassButton *btnJobCancel;
	TAdvGlassButton *btnJobEnd;
	TAdvGlassButton *btnJobStart;
	TAdvGlassButton *btnPPSelect;
	TAdvGlassButton *btnRecipeChange;
	TAdvGlassButton *btnSetAlarm;
	TAdvEdit *edtAlarmNo;
	TAdvEdit *edtBcr;
	TAdvEdit *edtRecipe;
	TAdvGlassButton *GemLocalButton;
	TAdvGlassButton *GemOfflineButton;
	TAdvGlassButton *GemRemoteButton;
	TAdvOfficeRadioGroup *rdgSelState;
	TAdvGlassButton *StartGemButton;
	TAdvStringGrid *StatusGrid;
	TImage *NotConnectedImage;
	TImage *ConnectedImage;
	TAdvGlassButton *StopGemButton;
	TTimer *GEMRefresh;
	TAdvGlassButton *btnJobValidation;
	TAdvEdit *WorkCompletionEdit;
	TCheckBox *UseDoorOpenDetectedWhenIdle;
	TAdvEdit *DoorOpenDetectedInterval;
	void __fastcall BtnChamberSaveClick(TObject *Sender);
	void __fastcall BtnChamberCancelClick(TObject *Sender);
	void __fastcall BtnUP550SettingClick(TObject *Sender);
	void __fastcall HomeButtonClick(TObject *Sender);
	void __fastcall OpenButtonClick(TObject *Sender);
	void __fastcall CloseButtonClick(TObject *Sender);
	void __fastcall RefreshTimer(TObject *Sender);
	void __fastcall AlarmClearButtonClick(TObject *Sender);
	void __fastcall EnableButtonClick(TObject *Sender);
	void __fastcall DisableButtonClick(TObject *Sender);
	void __fastcall OnEditEnter(TObject *Sender);
	void __fastcall OnEditExit(TObject *Sender);
	void __fastcall GEMRefreshTimer(TObject *Sender);
	void __fastcall StartGemButtonClick(TObject *Sender);
	void __fastcall StopGemButtonClick(TObject *Sender);
	void __fastcall GemOfflineButtonClick(TObject *Sender);
	void __fastcall GemLocalButtonClick(TObject *Sender);
	void __fastcall GemRemoteButtonClick(TObject *Sender);
	void __fastcall btnJobStartClick(TObject *Sender);
	void __fastcall btnJobEndClick(TObject *Sender);
	void __fastcall btnJobCancelClick(TObject *Sender);
	void __fastcall btnBcrClick(TObject *Sender);
	void __fastcall btnSetAlarmClick(TObject *Sender);
	void __fastcall btnPPSelectClick(TObject *Sender);
	void __fastcall btnRecipeChangeClick(TObject *Sender);
	void __fastcall rdgSelStateClick(TObject *Sender);
	void __fastcall btnJobValidationClick(TObject *Sender);

private:	// User declarations
	int ChamberNo;

	// 로거용 컨트롤들(동적 생성)
	// 로거를 최대 2개 사용한다고 가정
	TAdvPanel *LoggerPanel[MAX_LOGGER_COUNT];
	TAdvEdit *LoggerEdit[MAX_LOGGER_COUNT][MAX_POINT_COUNT];
	TCheckBox *LoggerUseCheckBox[MAX_LOGGER_COUNT][MAX_POINT_COUNT];

	void __fastcall Init();
	void __fastcall InitButtons();
	void __fastcall InitCtrls();
	void __fastcall DestroyLoggerCtrls();
	void __fastcall InitChamberSetting();
	void __fastcall InitRecipeGrid();
	void __fastcall UpdateRecipeGrid();
	void __fastcall UpdateChamberSetting();
	void __fastcall ShowPtnInfo(PTN tempPtn);
	void __fastcall ConvertToPtn(int ptnNo);
	void __fastcall ShowKeyboard(bool isNemeric);

	void __fastcall UpdateMoonsMotorStatus(DWORD status);
	void __fastcall UpdateEziMotorStatus(DWORD status);

	void __fastcall OnUseCheckBoxClick(TObject *Sender);
	void __fastcall OnOffsetEditEnter(TObject *Sender);
 	void __fastcall OnSelectAll(TMessage &Message);
	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(WM_USER+1, TMessage, OnSelectAll);
	END_MESSAGE_MAP(TFrame)

public:		// User declarations
	__fastcall TFrmChamberSetting(TComponent* Owner);
	__fastcall TFrmChamberSetting(TComponent* Owner, int chamberNo);
	__fastcall ~TFrmChamberSetting();
};
//---------------------------------------------------------------------------
//extern PACKAGE TFrmChamberSetting *FrmChamberSetting;
//---------------------------------------------------------------------------
#endif
