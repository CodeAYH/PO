//---------------------------------------------------------------------------

#ifndef TowerSetFormH
#define TowerSetFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvGlassButton.hpp"
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "BaseGrid.hpp"
#include <Vcl.Grids.hpp>
#include <Vcl.ExtCtrls.hpp>

#include "boost/shared_ptr.hpp"
#include "AdvUtil.hpp"
#include "NormalOvenSystemManager.h"

class TFormTowerSet : public TForm
{
__published:	// IDE-managed Components
	TAdvGlassButton *OKButton;
	TAdvGlassButton *CancelButton;
	TBevel *Bevel1;
	TAdvStringGrid *TowerLampGrid;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall TowerLampGridCanClickCell(TObject *Sender, int ARow, int ACol, bool &Allow);
	void __fastcall TowerLampGridCanEditCell(TObject *Sender, int ARow, int ACol, bool &CanEdit);
	void __fastcall TowerLampGridGetAlignment(TObject *Sender, int ARow, int ACol, TAlignment &HAlign,
          Advobj::TVAlignment &VAlign);
	void __fastcall TowerLampGridGetCellColor(TObject *Sender, int ARow, int ACol, TGridDrawState AState,
          TBrush *ABrush, TFont *AFont);
	void __fastcall TowerLampGridGetEditorProp(TObject *Sender, int ACol, int ARow,
          TEditLink *AEditLink);
	void __fastcall TowerLampGridGetEditorType(TObject *Sender, int ACol, int ARow,
          TEditorType &AEditor);
	void __fastcall OKButtonClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);

private:	// User declarations
	void __fastcall Localize(void);
	void __fastcall InitTowerLampGrid(void);
	void __fastcall FillTowerSetInfo(void);
	bool __fastcall IsValueChangeed(void);

	boost::shared_ptr<TStringList>	FsLampList;
	boost::shared_ptr<TStringList>	FsBuzzList;

	CTowerLamp TempTwrVal;

	int __fastcall GetTowerLampIndex(const String sTowerLamp) const;
	int __fastcall GetBuzzerIndex(const String sBuzzer) const;
	void __fastcall SaveChangeLog(void);

public:		// User declarations
	__fastcall TFormTowerSet(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TFormTowerSet *FormTowerSet;
extern PACKAGE void ShowTowerLampSet(void);
//---------------------------------------------------------------------------
#endif
