//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TCardRecipeLoadingForm.h"
#include "HelperFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvGlassButton"
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "BaseGrid"
#pragma link "AdvUtil"
#pragma resource "*.dfm"
TFmTCardRecipeLoading *FmTCardRecipeLoading;
//---------------------------------------------------------------------------
__fastcall TFmTCardRecipeLoading::TFmTCardRecipeLoading(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFmTCardRecipeLoading::FormShow(TObject *Sender)
{
	LastTCardID = "";
	TCardInputList.clear();

	TCardInputGrid->Clear();
	TCardInputGrid->RowCount = 2;
	TCardInputGrid->ColCount = 2;
	TCardInputGrid->FixedRows = 1;
	TCardInputGrid->FixedCols = 2;
	TCardInputGrid->ColWidths[0] = 25;
	TCardInputGrid->ColWidths[1] = 212;
	TCardInputGrid->Cells[0][0] = "No.";
	TCardInputGrid->Cells[1][0] = "T-Card ID";
	TCardInputGrid->RowCount = 1;

	InputTCardEdit->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFmTCardRecipeLoading::TCardInputGridCanEditCell(TObject *Sender,
		  int ARow, int ACol, bool &CanEdit)
{
	CanEdit = false;
}
//---------------------------------------------------------------------------

void __fastcall TFmTCardRecipeLoading::btnOkClick(TObject *Sender)
{
	Result = mrOk;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFmTCardRecipeLoading::btnCancelClick(TObject *Sender)
{
	Result = mrCancel;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFmTCardRecipeLoading::InputTCardEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift)
{
	if (Key == VK_RETURN)
	{
		String currID = InputTCardEdit->Text;
		if (currID.IsEmpty() == false)
		{
			if (LastTCardID.IsEmpty() || LastTCardID == currID)
			{
				int count, rows;

				LastTCardID = currID;
				TCardInputList.push_back(LastTCardID);
				count = TCardInputList.size();
				TCardInputGrid->RowCount = count + 1;
				rows = count;
				TCardInputGrid->Cells[0][rows] = count;
				TCardInputGrid->Cells[1][rows] = LastTCardID;
			}
			else
			{
				String str;
				SetMessageStr(GetResStr("Warning", "TCardMismatch"));
				SetMessageStr("");
				str = GetResStr("Common", "PrevInput") + " : " + LastTCardID;
				SetMessageStr(str);
				str = GetResStr("Common", "CurrentInput") + " : " + currID;
				SetMessageStr(str);
				ShowMessageDlg();
			}
			InputTCardEdit->Text = "";
		}
	}
}
//---------------------------------------------------------------------------

