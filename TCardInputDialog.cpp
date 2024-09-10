//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TCardInputDialog.h"
#include "TCardValidationForm.h"
#include "NormalOvenChamber.h"
#include "NormalOvenSystemManager.h"
#include "MainForm.h"
#include "HelperFunctions.h"
#include "Database.h"
//---------------------------------------------------------------------
#pragma link "AdvEdit"
#pragma link "AdvGlassButton"
#pragma resource "*.dfm"
TTCardInputDlg *TCardInputDlg;
//--------------------------------------------------------------------- 
__fastcall TTCardInputDlg::TTCardInputDlg(TComponent* AOwner)
	: TForm(AOwner)
{
	Result = mrCancel;
	//NoticeLabel->Caption = GetResStr("Warning", "RecipeSelectNotice");
}
//---------------------------------------------------------------------
void __fastcall TTCardInputDlg::FormShow(TObject *Sender)
{
	ChamberBacodeEdit->Text = "";
	ChamberBacodeEdit->SetFocus();

	int x, y;

	x = Left + (Width - FmMain->Keyboard->KeyboardWidth)/2;
	y = Top + Height;
	FmMain->Keyboard->KeyboardType = ktQWERTY;
	FmMain->Keyboard->ShowAtXY(x, y);
}
//---------------------------------------------------------------------------
void __fastcall TTCardInputDlg::CancelButtonClick(TObject *Sender)
{
	Result = mrCancel;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TTCardInputDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	FmMain->Keyboard->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TTCardInputDlg::ChamberSelectEditKeyDown(TObject *Sender, WORD &Key,
		  TShiftState Shift)
{
	if (Key == VK_RETURN)
	{
		String ChamberCode, str;
		ChamberCode = ChamberBacodeEdit->Text;

		NormalOvenChamber *chamber;
		for(int i=0; i<GetManager()->ChamberCount; i++)
		{
			chamber = GetChamber(i);

			if (ChamberCode == chamber->ChamberSelect)
			{
				CancelButtonClick(this);
				//TCardValidationDlg[chamber->ChamberID]->ShowModal();
				String str;
				str.sprintf(L"T-card Chamber Code [%s]", ChamberCode.c_str());
				GetOvenDB()->SaveLog(str, MESSAGES, i);
				boost::shared_ptr<TFmTCardValidation> TCardValidationDlg(new TFmTCardValidation(this, chamber->ChamberID));
				if( TCardValidationDlg->ShowModal() == mrCancel )
				{
					return;
				}
				return;
			}
		}

		ChamberBacodeEdit->Text = "";
		str = GetResStr("Warning", "NotExistedSelect") + " : " + ChamberCode;
		SetMessageStr(str);
		ShowMessageDlg();
	}
}
//---------------------------------------------------------------------------

