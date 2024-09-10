//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "COWELLValidationForm.h"
#include "HelperFunctions.h"
#include "GEMForm.h"
#include "NormalOvenChamber.h"
#include "NormalOvenSystemManager.h"
#include "LoginForm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvCombo"
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "AdvUtil"
#pragma link "BaseGrid"
#pragma link "AdvGlowButton"
#pragma link "EllipsLabel"
#pragma resource "*.dfm"
//TCowellValidationFrm *CowellValidationFrm;
//---------------------------------------------------------------------------
void __fastcall ShowValidationForm(int chamberNo)
{
 //	CowellValidationFrm->ShowModal();
	boost::shared_ptr<TCowellValidationFrm> TValidationDlg(new TCowellValidationFrm(nullptr, chamberNo));
	TValidationDlg->ShowModal();
}
//---------------------------------------------------------------------------
__fastcall TCowellValidationFrm::TCowellValidationFrm(TComponent* Owner, int ChamberID)
	: TForm(Owner)
{
	chamber = GetChamber(ChamberID);
	CurrentRow = 0;
	MaxRow = 1;
	IgnoreCellDone = false;
	InitGrids();
}
//---------------------------------------------------------------------------
void __fastcall TCowellValidationFrm::Localize()
{
	RequestValidationButton->Caption = GetResStr("COWELLGEM", "ReqValidation");
}
//---------------------------------------------------------------------------
void __fastcall TCowellValidationFrm::RefreshProcessList()
{

}
//-----------------------------------------------------------------
void __fastcall TCowellValidationFrm::InitGrids()
{
	ListGrid->Clear();
	ListGrid->RowCount = 11;
	ListGrid->ColCount = 2;
	ListGrid->FixedRows = 1;
	ListGrid->FixedCols = 1;
	ListGrid->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TCowellValidationFrm::ListGridCanEditCell(TObject *Sender, int ARow, int ACol,
          bool &CanEdit)
{
	if (ACol != 0 && ARow !=0)
	{
		if (ARow <= MaxRow)
		{
			CanEdit = true;
			CurrentRow = ARow;
		}
		else
		{
			CanEdit = false;
		}
	}
	else
	{
		CanEdit = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TCowellValidationFrm::ListGridEditCellDone(TObject *Sender, int ACol, int ARow)
{
	// 중복 실행 방지
//	if (IgnoreCellDone)
//	{
//		IgnoreCellDone = false;
//		return;
//	}

	if( ARow < 1 )
	{
		return;
	}
	if (ACol != 1)
	{
		return;
	}

	String str = ListGrid->Cells[ACol][ARow];
	if (IsItemExist(str))
	{
		IgnoreCellDone = true;
		SetMessageStr(GetResStr("Warning", "LotIdExist"));
		ShowMessageDlg();
		return;
	}

	IgnoreCellDone = true;
	SetLotGridFocus();
}
//---------------------------------------------------------------------------
void __fastcall TCowellValidationFrm::SetLotGridFocus()
{
	int lastIdx = 0;
	int col;

	int lotCount = ListGrid->RowCount-1;
	for (int i = 1; i <= lotCount; i++)
	{
		if (ListGrid->Cells[1][i].IsEmpty())
		{
			lastIdx = i;
			break;
		}
	}

	if (lastIdx == 0)	// 모든 칸 입력 완료
	{
		MaxRow = lotCount;
		CurrentRow = 0;
		ListGrid->Col = 1;
		ListGrid->Row = 1;
	}
	else
	{
		MaxRow = lastIdx;
		CurrentRow = MaxRow;
		ListGrid->Col = 1;
		ListGrid->Row = MaxRow;
	}
	ListGrid->SetFocus();
}
//---------------------------------------------------------------------------
bool __fastcall TCowellValidationFrm::IsItemExist(String lotId)
{
	if (lotId.IsEmpty())
	{
		return false;
	}

	String str;
	for (int i = 1; i < ListGrid->RowCount; i++)
	{
		str = ListGrid->Cells[1][i];
		if (str == lotId && CurrentRow != i)
		{
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------
void __fastcall TCowellValidationFrm::ListGridGetCellColor(TObject *Sender, int ARow, int ACol,
		  TGridDrawState AState, TBrush *ABrush, TFont *AFont)
{
	if( ARow != 0 &&
		ACol != 0 )
	{
		if( ARow == CurrentRow )
			ABrush->Color = clYellow;
		else
		if( ARow <= MaxRow )
			ABrush->Color = clWhite;
		else
			ABrush->Color = clInactiveCaption;
	}
}
//---------------------------------------------------------------------------
void __fastcall TCowellValidationFrm::ListGridGetEditorType(TObject *Sender, int ACol, int ARow,
		  TEditorType &AEditor)
{
	if (ACol == 1)
	{
		AEditor = edNormal;
	}
}
//---------------------------------------------------------------------------

void __fastcall TCowellValidationFrm::ListGridResize(TObject *Sender)
{
	ListGrid->DefaultColWidth = ListGrid->Width - ListGrid->FixedColWidth;

	ListGrid->ColWidths[0]	= 25;
	ListGrid->ColWidths[1]	= 300;
}
//---------------------------------------------------------------------------

void __fastcall TCowellValidationFrm::FormDestroy(TObject *Sender)
{
	ProcessList.clear();
}
//---------------------------------------------------------------------------

void __fastcall TCowellValidationFrm::FormShow(TObject *Sender)
{
	Localize();
	InitGrids();

	String str, sChamber;

	sChamber.sprintf(L"[CHAMBER %c] ", chamber->ChamberID+'A');
	lblChamber->Caption = sChamber;
	lblChamber->Font->Color = clPurple;

	ListGrid->Clear();
	for (int i = 1; i < ListGrid->RowCount; i++)
	{
		str.sprintf(L"%d", i);
		ListGrid->Cells[0][i] = str;
	}

	ListGrid->ColumnHeaders->Add("No.");
	ListGrid->ColumnHeaders->Add((GetResStr("COWELLGEM", "LOT ID")).c_str());

//	ProcessList.clear();
//
//	String sec = "LOTLIST";
//	String name, code;
//	String iniFile = GetDataFolder() + "CowellLotList.ini";
//	TIniFile *file = new TIniFile(iniFile);
//	TStringList *keyList = new TStringList();
//	file->ReadSection(sec, keyList);
//	for (int i = 0; i < keyList->Count; i++)
//	{
//		name = keyList->Strings[i];
//		code = file->ReadString(sec, name, "");
//		ProcessList[name] = code;
//	}
//	delete keyList;
//	delete file;
//
//	RefreshProcessList();

	RequestValidationButton->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TCowellValidationFrm::RequestValidationButtonClick(TObject *Sender)
{
	if (chamber == nullptr)
	{
		return;
	}

	SetMessageStr(GetResStr("COWELLGEM", "QestDoValidation"));
	if (ShowQuestionDlg() == false)
	{
		return;
	}

	RequestValidationButton->Enabled = false;

	chamber->CowellLotInfoClear();

	int lotCount = 0;
	String str, strLot;
	for (int i = 1; i < ListGrid->RowCount; i++)
	{
		str = ListGrid->Cells[1][i];
		if (str.IsEmpty() == false)
		{
			lotCount++;
		}
		else
		{
			break;
		}
	}

	if (lotCount <= 0)
	{
		SetMessageStr(GetResStr("COWELLGEM", "ZeroList"));
		ShowMessageDlg();
		RequestValidationButton->Enabled = true;
		return;
	}

	if (chamber->Recipe.RecipeName.IsEmpty())
	{
		SetMessageStr(GetResStr("Warning", "NoSelectedRecipe"));
		ShowMessageDlg();
		RequestValidationButton->Enabled = true;
		return;
	}

	for (int i = 0; i < lotCount; i++)
	{
		str.sprintf(L" %s", ListGrid->Cells[1][i+1].c_str());
		chamber->LotIDList->Add(str);
		strLot += str;
	}

	chamber->ValidationRequested = true;
	chamber->ValidationSuccess = false;
	GemDlg->SendJobValidation(chamber->ChamberID, strLot, chamber->Recipe.RecipeName);

	// 결과가 올 때까지 10초 대기
	int count = 0;
	while (true)
	{
		Sleep(100);
		Application->ProcessMessages();

		if (chamber->ValidationRequested == false)
		{
			break;
		}

		count++;
		if (count >= 100)
		{
			break;
		}
	}
	if (chamber->ValidationRequested == false)
	{
		if (chamber->ValidationSuccess)
		{
			RequestValidationButton->Enabled = true;
			Close();
		}
		else
		{
			SetMessageStr(GetResStr("COWELLGEM", "ValidationFail"));
			ShowMessageDlg();
			RequestValidationButton->Enabled = true;
            // kjh 2023-09-12 add
			if (GetManager()->UseCsvDataSave)
			{
				chamber->WriteProductData(Now(), GetManager()->CsvMachineId,
										GetManager()->UserID,
										strLot, true, chamber->ErrorDetail);
			}
		}
	}
	else
	{
		chamber->ValidationRequested = false;
		SetMessageStr(GetResStr("COWELLGEM", "ValidationTimeover"));
		ShowMessageDlg();
		RequestValidationButton->Enabled = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TCowellValidationFrm::FormClose(TObject *Sender, TCloseAction &Action)

{
	IgnoreCellDone = true;
}
//---------------------------------------------------------------------------

void __fastcall TCowellValidationFrm::AlignInGridComponent(TControl* Comp, TRect& Rect)
{
	Comp->Left = Rect.Left;
	Comp->Top = Rect.top;
	Comp->Width = Rect.Width();
	Comp->Height = Rect.Height();
	Comp->Parent = ListGrid;
}
