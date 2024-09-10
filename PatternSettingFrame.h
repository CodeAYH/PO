//---------------------------------------------------------------------------

#ifndef PatternSettingFrameH
#define PatternSettingFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvCombo.hpp"
#include "AdvEdit.hpp"
#include "AdvGlassButton.hpp"
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "AdvPanel.hpp"
#include "AdvSmoothToggleButton.hpp"
#include "BaseGrid.hpp"
#include "ToolPanels.hpp"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include "AdvShape.hpp"

#include "Defines.h"
#include <VCLTee.Chart.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeeFunci.hpp>
#include "MSCommLib_OCX.h"
#include "VaClasses.hpp"
#include "VaComm.hpp"
#include <Vcl.OleCtrls.hpp>
#include "AdvLabel.hpp"
#include "AdvUtil.hpp"
//---------------------------------------------------------------------------
class TFrmPatternSetting : public TFrame
{
__published:	// IDE-managed Components
	TAdvPanel *EditorPanel;
	TAdvToolPanel *PatternEditor;
	TAdvEdit *TimeEdit;
	TAdvEdit *SPEdit;
	TAdvComboBox *EventTypeComboBox;
	TAdvEdit *OnEdit;
	TAdvComboBox *STCComboBox;
	TAdvEdit *OffEdit;
	TAdvEdit *SSPEdit;
	TAdvGlassButton *AddSegmentButton;
	TAdvGlassButton *AddEventButton;
	TAdvPanel *PatternRepositoryPanel;
	TAdvSmoothToggleButton *BtnPattern1;
	TAdvSmoothToggleButton *BtnPattern10;
	TAdvSmoothToggleButton *BtnPattern11;
	TAdvSmoothToggleButton *BtnPattern12;
	TAdvSmoothToggleButton *BtnPattern13;
	TAdvSmoothToggleButton *BtnPattern14;
	TAdvSmoothToggleButton *BtnPattern15;
	TAdvSmoothToggleButton *BtnPattern16;
	TAdvSmoothToggleButton *BtnPattern17;
	TAdvSmoothToggleButton *BtnPattern18;
	TAdvSmoothToggleButton *BtnPattern19;
	TAdvSmoothToggleButton *BtnPattern2;
	TAdvSmoothToggleButton *BtnPattern20;
	TAdvSmoothToggleButton *BtnPattern21;
	TAdvSmoothToggleButton *BtnPattern22;
	TAdvSmoothToggleButton *BtnPattern23;
	TAdvSmoothToggleButton *BtnPattern24;
	TAdvSmoothToggleButton *BtnPattern25;
	TAdvSmoothToggleButton *BtnPattern26;
	TAdvSmoothToggleButton *BtnPattern27;
	TAdvSmoothToggleButton *BtnPattern28;
	TAdvSmoothToggleButton *BtnPattern29;
	TAdvSmoothToggleButton *BtnPattern3;
	TAdvSmoothToggleButton *BtnPattern30;
	TAdvSmoothToggleButton *BtnPattern4;
	TAdvSmoothToggleButton *BtnPattern5;
	TAdvSmoothToggleButton *BtnPattern6;
	TAdvSmoothToggleButton *BtnPattern7;
	TAdvSmoothToggleButton *BtnPattern8;
	TAdvSmoothToggleButton *BtnPattern9;
	TAdvGlassButton *BtnPtnDelete;
	TAdvGlassButton *BtnPtnSave;
	TAdvSmoothToggleButton *AdvSmoothToggleButton1;
	TAdvSmoothToggleButton *AdvSmoothToggleButton2;
	TAdvSmoothToggleButton *AdvSmoothToggleButton3;
	TAdvPanel *RegisterRecipePanel;
	TAdvStringGrid *GrdRecipeEdit;
	TAdvGlassButton *BtnRecipeDelete;
	TAdvGlassButton *BtnRecipeAdd;
	TAdvEdit *RecipeNameEdit;
	TAdvComboBox *SelectPatternComboBox;
	TAdvStringGrid *EventInfoGrid;
	TAdvPanel *SegNoPanel;
	TAdvGlassButton *PrevSegButton;
	TAdvGlassButton *NextSegButton;
	TAdvStringGrid *SegInfoGrid;
	TAdvGlassButton *UpdateSegmentButton;
	TAdvGlassButton *DeleteSegmentButton;
	TAdvGlassButton *UpdateEventButton;
	TAdvGlassButton *DeleteEventButton;
	TAdvEdit *DamperEdit;
	TAdvGlassButton *UpdateSTCButton;
	TAdvEdit *InverterEdit;
	TCheckBox *UseLocalModeCheckBox;
	TAdvEdit *LocalTempEdit;
	TChart *TempGraph;
	void __fastcall BtnPattern1Click(TObject *Sender);
	void __fastcall BtnPtnSaveClick(TObject *Sender);
	void __fastcall BtnPtnDeleteClick(TObject *Sender);
	void __fastcall BtnRecipeAddClick(TObject *Sender);
	void __fastcall BtnRecipeDeleteClick(TObject *Sender);
	void __fastcall AddSegmentButtonClick(TObject *Sender);
	void __fastcall AddEventButtonClick(TObject *Sender);
	void __fastcall GrdRecipeEditClickCell(TObject *Sender, int ARow, int ACol);
	void __fastcall GrdRecipeEditDblClickCell(TObject *Sender, int ARow, int ACol);
	void __fastcall GrdRecipeEditDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall SelectPatternComboBoxEnter(TObject *Sender);
	void __fastcall OnEditEnter(TObject *Sender);
	void __fastcall OnEditExit(TObject *Sender);
	void __fastcall EventTypeComboBoxChange(TObject *Sender);
	void __fastcall EditorPanelClick(TObject *Sender);
	void __fastcall PrevSegButtonClick(TObject *Sender);
	void __fastcall NextSegButtonClick(TObject *Sender);
	void __fastcall SegInfoGridGetAlignment(TObject *Sender, int ARow, int ACol, TAlignment &HAlign,
          TVAlignment &VAlign);
	void __fastcall EventInfoGridGetAlignment(TObject *Sender, int ARow, int ACol, TAlignment &HAlign,
          TVAlignment &VAlign);
	void __fastcall EventInfoGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall EventInfoGridClickCell(TObject *Sender, int ARow, int ACol);
	void __fastcall UpdateSegmentButtonClick(TObject *Sender);
	void __fastcall DeleteSegmentButtonClick(TObject *Sender);
	void __fastcall UpdateEventButtonClick(TObject *Sender);
	void __fastcall DeleteEventButtonClick(TObject *Sender);
	void __fastcall TempGraphClick(TObject *Sender);
	void __fastcall UpdateSTCButtonClick(TObject *Sender);
	void __fastcall TempGraphBeforeDrawAxes(TObject *Sender);
	void __fastcall TempGraphBeforeDrawSeries(TObject *Sender);



private:	// User declarations
	int ChamberNo;
	int SelectedPtnNo;
	int CurrentSegNo;
	int CurrentEventIndex;
	PTN TempPtn;
	DAMPERINFO CurrentDamper;
	SEG CurrentSeg;
	double CurrentInverterFreq;

	TLineSeries *TempSeries;
	TAdvSmoothToggleButton *BtnPattern[30];
	String RecipeName;
	int SelectedRow;

	TDateTime AreaStartTime, AreaEndTime;
	double MaxTemp;

	void __fastcall Init();
	void __fastcall InitButtons();
	void __fastcall InitRecipeGrid();
	void __fastcall UpdateRecipeGrid();
	void __fastcall InitSegInfoGrid();
	void __fastcall InitEventInfoGrid();
	void __fastcall InitChart();
	void __fastcall InitSeries();
	void __fastcall ShowKeyboard(bool isNemeric);

	void __fastcall FillSegInfo();
	void __fastcall FillTempGraph(int segCount);
	void __fastcall FillCurSegTitle(int segCount);
	void __fastcall FillSegInfoGrid();
	void __fastcall FillEventInfoGrid();
	void __fastcall FillSegAdd();
	void __fastcall FillEventAdd();
	void __fastcall UpdateChart();

	void __fastcall UpdatePtnTimeTemp();
	void __fastcall ApplyPatternInfo();
	void __fastcall ApplySegmentInfo(bool isNewSeg);
	void __fastcall ApplyEventInfo();

	int __fastcall GetEventIndex(int eventKind);
	int __fastcall GetEventType(int eventIndex);
	int __fastcall GetEventCount(int segNo);
	int __fastcall GetSegNoByTime(int minutes);
    int __fastcall GetSegNoByPosX(int x);

	void __fastcall OnSelectAll(TMessage &Message);

	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(WM_USER+1, TMessage, OnSelectAll);
	END_MESSAGE_MAP(TFrame)

public:		// User declarations
	__fastcall TFrmPatternSetting(TComponent* Owner, int chamberNo);
	__fastcall ~TFrmPatternSetting();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmPatternSetting *FrmPatternSetting;
//---------------------------------------------------------------------------
#endif
