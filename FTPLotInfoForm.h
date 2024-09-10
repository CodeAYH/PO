//---------------------------------------------------------------------------

#ifndef FTPLotInfoFormH
#define FTPLotInfoFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.TeeSpline.hpp>
#include "AdvGlassButton.hpp"
#include <VclTee.TeeGDIPlus.hpp>
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "BaseGrid.hpp"
#include <Vcl.Grids.hpp>
#include "AdvEdit.hpp"
#include "AdvUtil.hpp"

//---------------------------------------------------------------------------
class TFTPLotInfoForm : public TForm
{
__published:	// IDE-managed Components
	TAdvGlassButton *btnPtnOk;
	TAdvGlassButton *btnCancel;
	TAdvStringGrid *LotInfoGrid;
	TAdvEdit *UserIDEdit;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnPtnOkClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall LotInfoGridCanEditCell(TObject *Sender, int ARow, int ACol, bool &CanEdit);
	void __fastcall LotInfoGridEditCellDone(TObject *Sender, int ACol, int ARow);
	void __fastcall LotInfoGridGetAlignment(TObject *Sender, int ARow, int ACol, TAlignment &HAlign,
          TVAlignment &VAlign);
	void __fastcall LotInfoGridGetCellColor(TObject *Sender, int ARow, int ACol, TGridDrawState AState,
          TBrush *ABrush, TFont *AFont);
	void __fastcall LotInfoGridGetEditorType(TObject *Sender, int ACol, int ARow, TEditorType &AEditor);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall UserIDEditChange(TObject *Sender);



private:	// User declarations
	int m_Ret;
	int CurrentMaxRow;
	const int FnChamber;
	bool IsInsertLotInfoGrid;

	void __fastcall InitLotInfoGrid();
	void __fastcall SetLotGridFocus(int col, int row);
	bool __fastcall CheckValidLotInfo();
	bool __fastcall IsLotExist(String strValue, int row=-1);

public:		// User declarations
	__fastcall TFTPLotInfoForm (const int nChamber, TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TFTPLotInfoForm *FTPLotInfoForm;
//---------------------------------------------------------------------------
#endif
