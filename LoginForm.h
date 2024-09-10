//---------------------------------------------------------------------------

#ifndef uLogInH
#define uLogInH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AdvEdit.hpp"
#include "AdvGlassButton.hpp"
#include "AdvSmoothTouchKeyBoard.hpp"
//---------------------------------------------------------------------------
class TFmLogin : public TForm
{
__published:	// IDE-managed Components
    TAdvEdit *edtId;
    TAdvEdit *edtPassword;
    TAdvGlassButton *btnOk;
    TAdvGlassButton *btnCancel;
	TLabel *PwHint;
	TLabel *LevelMode;
    void __fastcall btnOkClick(TObject *Sender);
    void __fastcall edtIdKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall edtPasswordKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall edtIdEnter(TObject *Sender);
	void __fastcall edtIdExit(TObject *Sender);

private:	// User declarations
	String SuperKey;
	signed char m_Ret;
	const int Level;
 	void __fastcall AlignComps();
	void __fastcall Localize();

public:		// User declarations
	__fastcall TFmLogin(TComponent* Owner, const int nLevel);
};

bool __fastcall ShowLogin(const int nLevel, const bool newLogin);
//---------------------------------------------------------------------------
//extern PACKAGE TFmLogin *FmLogin;
//---------------------------------------------------------------------------
#endif
