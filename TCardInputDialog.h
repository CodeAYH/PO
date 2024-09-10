//----------------------------------------------------------------------------
#ifndef TCardInputDialogH
#define TCardInputDialogH
//----------------------------------------------------------------------------
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <StdCtrls.hpp>
#include <Controls.hpp>
#include <Forms.hpp>
#include <Graphics.hpp>
#include <Classes.hpp>
#include <SysUtils.hpp>
#include <Windows.hpp>
#include <System.hpp>
#include "AdvEdit.hpp"
#include "AdvGlassButton.hpp"
#include <boost/shared_ptr.hpp>
//----------------------------------------------------------------------------
class TTCardInputDlg : public TForm
{
__published:
	TAdvEdit *ChamberBacodeEdit;
	TLabel *NoticeLabel;
	TAdvGlassButton *CancelButton;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ChamberSelectEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);


private:
public:
	virtual __fastcall TTCardInputDlg(TComponent* AOwner);

	int Result;
};
//----------------------------------------------------------------------------
extern PACKAGE TTCardInputDlg *TCardInputDlg;
//----------------------------------------------------------------------------
#endif    
