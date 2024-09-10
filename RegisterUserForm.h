//---------------------------------------------------------------------------

#ifndef RegisterUserFormH
#define RegisterUserFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvCombo.hpp"
#include "AdvEdit.hpp"
#include "AdvGlassButton.hpp"
#include "AdvGroupBox.hpp"
#include "AdvOfficeButtons.hpp"
#include "AdvVerticalPolyList.hpp"
#include "CustomItemsContainer.hpp"
#include "GDIPCustomItem.hpp"
#include "GDIPExpandableImageSectionItem.hpp"
#include "GDIPHTMLItem.hpp"
#include "GDIPImageSectionItem.hpp"
#include "GDIPImageTextItem.hpp"
#include "GDIPSectionItem.hpp"
#include "GDIPTextItem.hpp"
#include <Vcl.DBGrids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Data.DB.hpp>
#include "AdvSmoothTouchKeyBoard.hpp"
#include <memory>
//---------------------------------------------------------------------------
class TFmRegisterUser : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnlRight;
	TAdvGlassButton *btnAdd;
	TAdvGlassButton *btnEdit;
	TAdvGlassButton *btnDelete;
	TAdvEdit *edtUserId;
	TAdvEdit *edtUserName;
	TAdvEdit *edtPassword;
	TAdvGlassButton *btnOk;
	TAdvGlassButton *btnCancel;
	TDBGrid *UserGrid;
	TAdvOfficeRadioGroup *rdgLevel;
	TAdvVerticalPolyList *polyHowTo;
	TExpandableImageSectionItem *expSecItem0;
	THTMLItem *HTMLItem0_HowToAddUser;
	TExpandableImageSectionItem *expSecItem1;
	THTMLItem *HTMLItem1_HowToEditUser;
	TExpandableImageSectionItem *expSecItem2;
	THTMLItem *HTMLItem2_HowToDelUser;
	TDataSource *SrcUser;
	TAdvGlassButton *BtnClose;
	TAdvSmoothTouchKeyBoard *Keyboard;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall UserGridCellClick(TColumn *Column);
	void __fastcall btnAddClick(TObject *Sender);
	void __fastcall btnEditClick(TObject *Sender);
	void __fastcall btnDeleteClick(TObject *Sender);
	void __fastcall edtUserIdKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall edtUserNameKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall edtPasswordKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall BtnCloseClick(TObject *Sender);
	void __fastcall edtUserIdEnter(TObject *Sender);
	void __fastcall edtUserIdExit(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:	// User declarations
	bool m_bEditMode, m_bAddMode;

	void __fastcall Localize();
	void __fastcall FillHowTo();
	void __fastcall FillUserGrid();

	void __fastcall EnableCtrls();
	void __fastcall EmptySelInfo();
	void __fastcall UpdateSelInfo();

public:		// User declarations
	__fastcall TFmRegisterUser(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFmRegisterUser *FmRegisterUser;
//---------------------------------------------------------------------------
#endif
