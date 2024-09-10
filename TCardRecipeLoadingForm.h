//---------------------------------------------------------------------------

#ifndef TCardRecipeLoadingFormH
#define TCardRecipeLoadingFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvGlassButton.hpp"
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "BaseGrid.hpp"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include "AdvUtil.hpp"
#include <vector>
//---------------------------------------------------------------------------
class TFmTCardRecipeLoading : public TForm
{
__published:	// IDE-managed Components
	TAdvStringGrid *TCardInputGrid;
	TLabeledEdit *InputTCardEdit;
	TAdvGlassButton *btnOk;
	TAdvGlassButton *btnCancel;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TCardInputGridCanEditCell(TObject *Sender, int ARow, int ACol, bool &CanEdit);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall InputTCardEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

private:	// User declarations
	std::vector<String> TCardInputList;

public:		// User declarations
	int Result;
	String LastTCardID;

	__fastcall TFmTCardRecipeLoading(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFmTCardRecipeLoading *FmTCardRecipeLoading;
//---------------------------------------------------------------------------
#endif
