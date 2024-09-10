//---------------------------------------------------------------------------

#ifndef TempGraphFrameH
#define TempGraphFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvEdit.hpp"
#include "AdvGlassButton.hpp"
#include "AdvOfficeButtons.hpp"
#include "AdvSmoothCalendar.hpp"
#include <Data.DB.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <VCLTee.Chart.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "AdvPanel.hpp"
#include "BaseGrid.hpp"
#include "Defines.h"
#include "FolderDialog.hpp"
#include "AdvOfficeImage.hpp"
#include "AdvPicture.hpp"
#include "AdvUtil.hpp"
#include <VCLTee.Series.hpp>

//---------------------------------------------------------------------------
class NormalOvenChamber;
class TFrmTempGraph : public TFrame
{
__published:	// IDE-managed Components
	TChart *TempChart;
	TDBGrid *WorkGrid;
	TAdvSmoothCalendar *Calendar;
	TPanel *pnlVeiwVal;
	TAdvEdit *lblCh1;
	TAdvEdit *lblCh2;
	TAdvEdit *lblCh3;
	TAdvEdit *lblCh4;
	TAdvEdit *lblCh5;
	TAdvEdit *lblTime;
	TAdvEdit *lblPv;
	TAdvEdit *lblSp;
	TAdvEdit *lblOut;
	TAdvEdit *lblLimit;
	TAdvGlassButton *ProfileBackupButton;
	TDataSource *WorkSrc;
	TAdvEdit *lblO2;
	TAdvEdit *lblCh6;
	TAdvEdit *ProductCodeEdit;
	TAdvGlassButton *SearchProductCodeButton;
	TAdvEdit *lblCh7;
	TAdvEdit *lblCh8;
	TAdvPanel *LotInfoPanel;
	TAdvStringGrid *LotInfoGrid;
	TFolderDialog *FolderDialog;
	TTimer *GetDayInfoTimer;
	TAdvEdit *lblCh9;
	TAdvEdit *lblCh10;
	TAdvEdit *lblCh11;
	TAdvEdit *lblCh12;
	TAdvEdit *lblCh13;
	TAdvEdit *lblCh14;
	TAdvEdit *lblCh15;
	TAdvEdit *lblCh16;
	void __fastcall WorkGridCellClick(TColumn *Column);
	void __fastcall CalendarSelectDate(TObject *Sender, TAdvSmoothCalendarDateMode Mode,
          TDateTime Date);
	void __fastcall TempChartAllowScroll(TChartAxis *Sender, double &AMin, double &AMax,
          bool &AllowScroll);
	void __fastcall TempChartMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall edtTimeChange(TObject *Sender);
	void __fastcall ProfileBackupButtonClick(TObject *Sender);
	void __fastcall SearchProductCodeButtonClick(TObject *Sender);
	void __fastcall TempChartDblClick(TObject *Sender);
	void __fastcall LotInfoGridGetAlignment(TObject *Sender, int ARow, int ACol, TAlignment &HAlign,
          TVAlignment &VAlign);
	void __fastcall LotInfoGridGetCellColor(TObject *Sender, int ARow, int ACol, TGridDrawState AState,
          TBrush *ABrush, TFont *AFont);
	void __fastcall CalendarDateFill(TObject *Sender, TGDIPFill *AFill, TFont *AFont,
          TDateTime Date, TAdvSmoothCalendarDateKind DateKind);
	void __fastcall CalendarMonthChanged(TObject *Sender, int Month);
	void __fastcall CalendarDateStatus(TObject *Sender, TDateTime Date, UnicodeString &StatusMessage,
          TGDIPStatus *Fill, int &OffsetX, int &OffsetY);
	void __fastcall CalendarYearChanged(TObject *Sender, int Year);
	void __fastcall GetDayInfoTimerTimer(TObject *Sender);
	void __fastcall TempChartZoom(TObject *Sender);
	void __fastcall TempChartUndoZoom(TObject *Sender);
	void __fastcall TempChartAfterDraw(TObject *Sender);
	void __fastcall TempChartBeforeDrawSeries(TObject *Sender);

private:	// User declarations
	NormalOvenChamber *Chamber;
	TPointSeries *SrsMainTempPv, *SrsMainTempSp, *SrsMainTempOut;
	TPointSeries *SrsLimit, *SrsO2;
	TPointSeries *Srs_Ch[MAX_LOGGERPOINT_COUNT];
	TAdvEdit *Ch[MAX_LOGGERPOINT_COUNT];
	String SelectedFile;
	bool DisplayAbsTime;	// true : 절대시간, false : 상대시간 표시
	int DataFileCount[31];
	int CurrentMouseX;

	void __fastcall Init();
	void __fastcall InitGrid();
	void __fastcall InitChart(TDateTime dtStart,
							  int cureTime,
							  int maxTemp);
	void __fastcall InitSeries();
	void __fastcall LoadMainTempData(String filename, TDateTime start);
	void __fastcall LoadLoggerData(String filename, TDateTime start);
	void __fastcall LoadLimitData(String filename, TDateTime start);
	void __fastcall LoadOxygenData(String filename, TDateTime start);

	int __fastcall FindClosestXIndex(TPointSeries *serie, double xValue);
	bool __fastcall GetValue(TPointSeries* series, double &value);
	void __fastcall Localize();
	void __fastcall ArrangeLabels();

	void __fastcall FindData();
	void __fastcall InitLotInfoGrid(int chamberNo);
	void __fastcall FillLotInfoGrid(int chamberNo, LOTINFO lotInfo);
	void __fastcall CalcDataFileCount(int year, int month);

public:		// User declarations
	__fastcall TFrmTempGraph(TComponent* Owner);
	__fastcall TFrmTempGraph(TComponent* Owner, int chamberNo);
	__fastcall ~TFrmTempGraph();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmTempGraph *FrmTempGraph;
//---------------------------------------------------------------------------
#endif
