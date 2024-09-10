//---------------------------------------------------------------------------

#ifndef TCardChamberRecipeFormH
#define TCardChamberRecipeFormH
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

class TFmTCardChamberRecipe : public TForm
{
__published:	// IDE-managed Components
	TAdvStringGrid *TCardListGrid;
	TLabeledEdit *InputTCardEdit;
	TAdvGlassButton *btnCancel;
	TAdvComboBox *RecipeListComboBox;
	TAdvPanel *ChamberPanel;
	TAdvGlassButton *AddDeviceButton;
	TAdvGlassButton *DeleteDeviceButton;
	TAdvGlassButton *SaveDeviceListButton;
	TAdvPanel *AdvPanel1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TCardListGridCanEditCell(TObject *Sender, int ARow, int ACol, bool &CanEdit);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall InputTCardEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RecipeListComboBoxSelect(TObject *Sender);
	void __fastcall AddDeviceButtonClick(TObject *Sender);
	void __fastcall DeleteDeviceButtonClick(TObject *Sender);
	void __fastcall SaveDeviceListButtonClick(TObject *Sender);

private:	// User declarations
	std::vector<String> TCardInputList;
	NormalOvenChamber *Chamber;
	TStringList *DeviceList;
	std::map<String, TStringList*> *RecipeDeviceList;
	void __fastcall InitRecipeListComboBox();
	void __fastcall ClearRecipeDeviceList();
	void __fastcall UpdateDeviceListGrid();
	bool __fastcall LoadDeviceList();
	bool __fastcall SaveDeviceList();
public:		// User declarations
	int Result;
	String LastTCardList;
	String RecipeName;

	__fastcall TFmTCardChamberRecipe(TComponent* Owner, int chamberNo);
	__fastcall ~TFmTCardChamberRecipe();

	bool __fastcall LoadDeviceListAll();
};
//---------------------------------------------------------------------------
typedef boost::shared_ptr<TFmTCardChamberRecipe> TCardChamberRecipePtr;
extern TCardChamberRecipePtr TCardRecipeDlg[MAX_CHAMBER_COUNT];
//---------------------------------------------------------------------------
#endif
