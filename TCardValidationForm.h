//---------------------------------------------------------------------------

#ifndef TCardValidationFormH
#define TCardValidationFormH
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
#include "AdvCombo.hpp"
#include "AdvPanel.hpp"
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include "Defines.h"
//---------------------------------------------------------------------------
class NormalOvenChamber;

class TFmTCardValidation : public TForm
{
__published:	// IDE-managed Components
	TAdvStringGrid *TCardListGrid;
	TLabeledEdit *InputTCardEdit;
	TAdvGlassButton *btnCancel;
	TAdvComboBox *RecipeListComboBox;
	TAdvPanel *ChamberPanel;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TCardListGridCanEditCell(TObject *Sender, int ARow, int ACol, bool &CanEdit);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall InputTCardEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RecipeListComboBoxSelect(TObject *Sender);


private:	// User declarations
	const int FnChamber;
	std::vector<String> TCardInputList;
	String TCardList[10][10];
	NormalOvenChamber *Chamber;
	TStringList *DeviceList[TCARD_COUNT];
	std::map<String, TStringList*> *RecipeDeviceList;
	void __fastcall InitRecipeListComboBox();
	void __fastcall ClearRecipeDeviceList();
	void __fastcall UpdateDeviceListGrid();
	bool __fastcall LoadDeviceList();
	bool __fastcall SaveDeviceList();
	bool __fastcall SelectRecipeExists(int RecipeID);

public:		// User declarations
	int Result;
	String LastTCardList;
	String RecipeName;

	__fastcall TFmTCardValidation(TComponent* Owner, int chamberNo);
	__fastcall ~TFmTCardValidation();

	bool __fastcall LoadDeviceListAll();

};
//---------------------------------------------------------------------------
typedef boost::shared_ptr<TFmTCardValidation> TCardValidationPtr;
//extern TCardValidationPtr TCardValidationDlg[MAX_CHAMBER_COUNT];
//---------------------------------------------------------------------------
#endif
