//---------------------------------------------------------------------------

#ifndef COWELLValidationFormH
#define COWELLValidationFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvCombo.hpp"
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "AdvUtil.hpp"
#include "BaseGrid.hpp"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include "AdvGlowButton.hpp"
#include "EllipsLabel.hpp"
#include <memory>
#include <map>
#include "Defines.h"
//---------------------------------------------------------------------------
class NormalOvenChamber;

class TCowellValidationFrm : public TForm
{
__published:	// IDE-managed Components
	TAdvStringGrid *ListGrid;
	TAdvGlowButton *RequestValidationButton;
	TEllipsLabel *lblChamber;
	void __fastcall ListGridCanEditCell(TObject *Sender, int ARow, int ACol, bool &CanEdit);
	void __fastcall ListGridEditCellDone(TObject *Sender, int ACol, int ARow);
	void __fastcall ListGridGetCellColor(TObject *Sender, int ARow, int ACol, TGridDrawState AState,
          TBrush *ABrush, TFont *AFont);
	void __fastcall ListGridGetEditorType(TObject *Sender, int ACol, int ARow, TEditorType &AEditor);
	void __fastcall ListGridResize(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall RequestValidationButtonClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);



private:	// User declarations
	NormalOvenChamber *chamber;
	int MaxRow;
	int CurrentRow;
	bool IgnoreCellDone;
	std::map<String, String> ProcessList;
	void __fastcall AlignInGridComponent(TControl* Comp, TRect& Rect);
	bool __fastcall IsItemExist(String lotId);
	void __fastcall InitGrids();
	void __fastcall Localize();
	void __fastcall SetLotGridFocus();
	void __fastcall RefreshProcessList();

public:		// User declarations
	__fastcall TCowellValidationFrm(TComponent* Owner, int ChamberID);
};
//---------------------------------------------------------------------------
//extern PACKAGE TCowellValidationFrm *CowellValidationFrm;
void __fastcall ShowValidationForm(int chamberNo);
//---------------------------------------------------------------------------
#endif
