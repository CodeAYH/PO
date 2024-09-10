//---------------------------------------------------------------------------

#ifndef ChamberFrameH
#define ChamberFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvCombo.hpp"
#include "AdvGlassButton.hpp"
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "AdvPanel.hpp"
#include "AdvSmoothProgressBar.hpp"
#include "AdvSmoothStatusIndicator.hpp"
#include "BaseGrid.hpp"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.TeeSpline.hpp>
#include "AdvSmoothButton.hpp"
#include "AdvEdit.hpp"
#include "AdvUtil.hpp"
#include "Database.h"
#include "JobEndForm.h"
#include "AdvGlowButton.hpp"
//---------------------------------------------------------------------------
class NormalOvenChamber;
class TFrmChamber : public TFrame
{
__published:	// IDE-managed Components
	TAdvPanel *ChamberPanel;
	TAdvPanel *CommStatusPanel;
	TAdvSmoothStatusIndicator *MainTempIndicator;
	TAdvSmoothStatusIndicator *TempLimitIndicator;
	TAdvSmoothStatusIndicator *O2AnalyzerIndicator;
	TAdvSmoothStatusIndicator *LoggerIndicator;
	TAdvSmoothStatusIndicator *DIOIndicator;
	TAdvSmoothStatusIndicator *DPIndicator;
	TAdvSmoothStatusIndicator *DamperMotorIndicator;
	TAdvSmoothProgressBar *ProgressBar;
	TAdvGlassButton *BuzzerOffButton;
	TAdvGlassButton *ShowAlarmButton;
	TAdvPanel *MachineStatusPanel;
	TLabeledEdit *O2AnalyzerStatus;
	TLabeledEdit *DoorStatus;
	TAdvGlassButton *CureStartButton;
	TAdvGlassButton *CureStopButton;
	TChart *TempGraph;
	TAdvPanel *ChamberStatusPanel;
	TAdvPanel *RemainTimePanel;
	TAdvPanel *ChamberTempPanel;
	TAdvPanel *DPPanel;
	TAdvPanel *RecipePanel;
	TAdvStringGrid *RecipeInfoGrid;
	TAdvComboBox *RecipeComboBox;
	TAdvGlassButton *SelectButton;
	TChart *PatternGraph;
	TLineSeries *LineSeries1;
	TTimer *Refresh;
	TTimer *GrabData;
	TTimer *AlarmCheck;
	TAdvPanel *LotInfoPanel;
	TAdvStringGrid *LotInfoGrid;
	TAdvGlassButton *LotInfoButton;
	TAdvSmoothButton *ChangeViewModeButton;
	TLabeledEdit *MagazineDetectorStatus;
	TLabeledEdit *OnlineStatus;
	TAdvEdit *UserIDEdit;
	TTimer *SwitchToLotInfoPanel;
	TAdvPanel *O2Panel;
	TAdvGlassButton *LocalButton;
	TTimer *CureStartAlarmTimer;
	TTimer *UpdateUI;
	TAdvPanel *CimStatusPanel;
	TGridPanel *CimStatusGridPanel;
	TAdvGlowButton *EzNetOfflineButton;
	TAdvGlowButton *EzNetLocalButton;
	TAdvGlowButton *EzNetRemoteButton;
	TPanel *OnlineStaticText;
	TPanel *EnabledStaticText;
	TPanel *ConnectedStaticText;
	void __fastcall RefreshTimer(TObject *Sender);
	void __fastcall GrabDataTimer(TObject *Sender);
	void __fastcall AlarmCheckTimer(TObject *Sender);
	void __fastcall TempGraphDblClick(TObject *Sender);
	void __fastcall ShowAlarmButtonClick(TObject *Sender);
	void __fastcall BuzzerOffButtonClick(TObject *Sender);
	void __fastcall SelectButtonClick(TObject *Sender);
	void __fastcall RecipeComboBoxChange(TObject *Sender);
	void __fastcall CureStartButtonClick(TObject *Sender);
	void __fastcall CureStopButtonClick(TObject *Sender);
	void __fastcall LotInfoButtonClick(TObject *Sender);
	void __fastcall LotInfoGridCanEditCell(TObject *Sender, int ARow, int ACol, bool &CanEdit);
	void __fastcall LotInfoGridGetEditorType(TObject *Sender, int ACol, int ARow, TEditorType &AEditor);
	void __fastcall LotInfoGridGetAlignment(TObject *Sender, int ARow, int ACol, TAlignment &HAlign,
          TVAlignment &VAlign);
	void __fastcall LotInfoGridEditCellDone(TObject *Sender, int ACol, int ARow);
	void __fastcall LotInfoGridGetCellColor(TObject *Sender, int ARow, int ACol, TGridDrawState AState,
          TBrush *ABrush, TFont *AFont);
	void __fastcall ChangeViewModeButtonClick(TObject *Sender);
	void __fastcall UserIDEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall SwitchToLotInfoPanelTimer(TObject *Sender);
	void __fastcall ChamberStatusPanelDblClick(TObject *Sender);
	void __fastcall LocalButtonClick(TObject *Sender);
	void __fastcall SafetyRelayResetButtonClick(TObject *Sender);
	void __fastcall CureStartAlarmTimerTimer(TObject *Sender);
	void __fastcall EzNetLocalButtonClick(TObject *Sender);
	void __fastcall EzNetRemoteButtonClick(TObject *Sender);
	void __fastcall EnabledStaticTextDblClick(TObject *Sender);
	void __fastcall UpdateUITimer(TObject *Sender);


private:	// User declarations
	NormalOvenChamber *Chamber;
	TPointSeries *Series[MAX_COLOR_COUNT];
	TLineSeries *SPBgSerie;

	bool AutoStartFailed;
	bool MessageShowed;

	bool IsIdleStarted;
	bool IsShowRecipe;
	TDateTime IdleStartTime;
	double TempLoggerData[MAX_LOGGERPOINT_COUNT];
	int GraphMode;
	int CurrentMaxRow;

	bool IsCsvSaveComplete;

    bool CureStarting;

	void __fastcall Init();
	void __fastcall ArrangeCommIndicator();

	void __fastcall BlinkButtons(bool blink);
	void __fastcall RefreshCommIndicator(bool blink);
	void __fastcall RefreshStatusIndicator();
	void __fastcall RefreshData();
 //	void __fastcall RefreshSafetyButton();  //2023-04-17 restore

	void __fastcall FillRecipeGrid(OVEN_RECIPE rec);
	void __fastcall FillRecipeGrid(PTN ptn, String name);
	void __fastcall InitChart(TDateTime dtStart, int cureTime, int maxTemp);
	void __fastcall LoadMainTempData(TDateTime start);
	void __fastcall LoadLoggerData(TDateTime start);
	void __fastcall LoadOxygenData(TDateTime start);
	bool __fastcall CheckAndSendLotIDToGEMHost();

	void __fastcall GrabChamberData();
	void __fastcall ShowAlarm();
	void __fastcall ShowJobEnd(TDateTime end, int endType, bool remote=false);
	void __fastcall EnableButtons(bool enable=true);

	void __fastcall InitLotInfoGrid();
	void __fastcall FillLotInfoGrid(LOTINFO lotInfo);
	void __fastcall ArrangeFullMode();
	void __fastcall SetLotGridFocus(int col, int row);
	void __fastcall HanaRecipeInfoUpdate(int ChamberNo, String Recip, PTN ptn);//2021-04-14 HANA OVEN
	void __fastcall UpdateCimStatus();
	PTN ptn; //2021-08-09 hcw

	void __fastcall SaveCSVCureDate(TDateTime end);

public:		// User declarations
	__fastcall TFrmChamber(TComponent* Owner, NormalOvenChamber *chamber);
	__fastcall ~TFrmChamber();

	void __fastcall Localize();
	void __fastcall InitPtnChart();
	void __fastcall AutoArrange();
	void __fastcall FillUpInfo();
	void __fastcall UpdateRecipeComboBox();
	void __fastcall BlinkControls(bool blink);
	void __fastcall UpdateButtonState();
	void __fastcall ShowLotInfo();
	bool __fastcall CheckLotInfo(LOTINFO &lotInfo);
	bool __fastcall IsLotExist(String lotid, int row=-1);
	bool __fastcall SelectRecipe(String recipe);
	bool __fastcall SelectRemoteRecipe(String recipe);
  //bool __fastcall SelectReceiveHanaRecipe(PTN ptn);			// 2021-04-14 HANA OVEN  hcw
	bool __fastcall SelectReceiveHanaRecipe(); //2021-08-09 hcw
	bool __fastcall CureStart(bool byRemote=false);
	bool __fastcall CureStop(bool byRemote=false);
	void __fastcall DoAutoFocus();

	// 20180209 - FTP (OFILM MES) - jhlim
	bool __fastcall WriteTempData(AnsiString sFileName, AnsiString sTime);

	// 20191216 - SCK MES - jhlim
	bool __fastcall DoSCKMESProc();
	bool __fastcall	CheckRecipe_MESLotInfo();
	void __fastcall SelectRecipe_MESLotInfo(String recipe);
	void __fastcall InputLotID_MESLotInfo(String LotID);
	bool __fastcall SaveCureDoneInfo_MESLotInfo(int stopType);

	bool __fastcall HanaReceiveCureStart(); //2021-07-22 hcw.
	bool __fastcall SelectRecipe_FujitsuLotInfo(String recipe); //2021-10-05
	void __fastcall AutoModeButtonEnable(bool enable);
	void __fastcall SCK_RMSStart();
	void __fastcall FujisuTCard_RecipeComboInit(bool everything);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmChamber *FrmChamber;
typedef boost::shared_ptr<TFrmChamber> ChamberFramePtr;
//---------------------------------------------------------------------------
#endif
