//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TowerSetForm.h"
#include "HelperFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvGlassButton"
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "BaseGrid"
#pragma resource "*.dfm"
#pragma link "AdvUtil"

//TFormTowerSet *FormTowerSet;
//---------------------------------------------------------------------------
__fastcall TFormTowerSet::TFormTowerSet(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormTowerSet::FormCreate(TObject *Sender)
{
	FsLampList = boost::shared_ptr<TStringList> (new TStringList());
	FsLampList->Clear();
	FsLampList->Add(GetResStr("TowerLamp", "TowerOff"));
	FsLampList->Add(GetResStr("TowerLamp", "TowerOn"));
	FsLampList->Add(GetResStr("TowerLamp", "TowerBlink"));

	FsBuzzList = boost::shared_ptr<TStringList> (new TStringList());
	FsBuzzList->Clear();
	FsBuzzList->Add(GetResStr("TowerLamp", "BuzzOff"));
	FsBuzzList->Add(GetResStr("TowerLamp", "BuzzAlarm"));

	Localize();
	TempTwrVal = *GetManager()->TowerLampOption;
	InitTowerLampGrid();
	FillTowerSetInfo();
}
//---------------------------------------------------------------------------
void __fastcall TFormTowerSet::Localize(void)
{
	TowerLampGrid->Cells[0][0] = GetResStr("TowerLamp", "Item");
	TowerLampGrid->Cells[1][0] = GetResStr("TowerLamp", "TowerRed");
	TowerLampGrid->Cells[2][0] = GetResStr("TowerLamp", "TowerYellow");
	TowerLampGrid->Cells[3][0] = GetResStr("TowerLamp", "TowerGreen");
	TowerLampGrid->Cells[4][0] = GetResStr("TowerLamp", "TowerBuzz");
	TowerLampGrid->Cells[0][1] = GetResStr("TowerLamp", "MachineRun");
	TowerLampGrid->Cells[0][2] = GetResStr("TowerLamp", "MachineStop");
	TowerLampGrid->Cells[0][3] = GetResStr("TowerLamp", "LotEnd");
	TowerLampGrid->Cells[0][4] = GetResStr("TowerLamp", "Error");

	OKButton->Caption 		= GetResStr("Common", "OK");
	CancelButton->Caption 	= GetResStr("Common", "Cancel");
}
//---------------------------------------------------------------------------
void __fastcall TFormTowerSet::InitTowerLampGrid(void)
{
	TowerLampGrid->ColCount = 5;
	TowerLampGrid->RowCount = 5;
	TowerLampGrid->DefaultColWidth = 100;
	TowerLampGrid->DefaultRowHeight = 30;
	TowerLampGrid->FixedColWidth = 150;

	TowerLampGrid->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFormTowerSet::FillTowerSetInfo(void)
{
	for(int i=0; i<TOWERLAMP_MODE; i++)
	{
		TowerLampGrid->Cells[TWR_RED+1][i+1] = FsLampList->Strings[TempTwrVal.GetTwrVal(TWR_RED, i)];
		TowerLampGrid->Cells[TWR_YEL+1][i+1] = FsLampList->Strings[TempTwrVal.GetTwrVal(TWR_YEL, i)];// nTwr_Ylw[i]];
		TowerLampGrid->Cells[TWR_GRN+1][i+1] = FsLampList->Strings[TempTwrVal.GetTwrVal(TWR_GRN, i)];// nTwr_Grn[i]];
		TowerLampGrid->Cells[TWR_BUZ+1][i+1] = FsLampList->Strings[TempTwrVal.GetTwrVal(TWR_BUZ, i)];// nTwr_Buz[i]];
	}
}
//---------------------------------------------------------------------------
bool __fastcall TFormTowerSet::IsValueChangeed(void)
{
	for(int i=0; i<TOWERLAMP_MODE; i++)
	{
		TempTwrVal.SetTwrVal(TWR_RED, i, GetTowerLampIndex(TowerLampGrid->Cells[TWR_RED+1][i+1]) );
		TempTwrVal.SetTwrVal(TWR_YEL, i, GetTowerLampIndex(TowerLampGrid->Cells[TWR_YEL+1][i+1]) );
		TempTwrVal.SetTwrVal(TWR_GRN, i, GetTowerLampIndex(TowerLampGrid->Cells[TWR_GRN+1][i+1]) );
		TempTwrVal.SetTwrVal(TWR_BUZ, i, GetBuzzerIndex(TowerLampGrid->Cells[TWR_BUZ+1][i+1]) );
	}

	return !(TempTwrVal == *GetManager()->TowerLampOption);
}
//---------------------------------------------------------------------------

int __fastcall TFormTowerSet::GetTowerLampIndex(const String sTowerLamp) const
{
	int nIdx = FsLampList->IndexOf(sTowerLamp);

	return nIdx;
}
//---------------------------------------------------------------------------
int __fastcall TFormTowerSet::GetBuzzerIndex(const String sBuzzer) const
{
	int nIdx = FsBuzzList->IndexOf(sBuzzer);

	return nIdx;
}
//---------------------------------------------------------------------------
void __fastcall TFormTowerSet::SaveChangeLog()
{
//	LOG_PRINTF("GUI", "Tower Lamp Setting Changed RED:%s->%s, YEL:%s->%s, GRN:%s->%s, BUZ:%s->%s",
//		SysOption->TowerLamp->GetTwrValStr(TWR_RED), TempTwrVal.GetTwrValStr(TWR_RED),
//		SysOption->TowerLamp->GetTwrValStr(TWR_YEL), TempTwrVal.GetTwrValStr(TWR_YEL),
//		SysOption->TowerLamp->GetTwrValStr(TWR_GRN), TempTwrVal.GetTwrValStr(TWR_GRN),
//		SysOption->TowerLamp->GetTwrValStr(TWR_BUZ), TempTwrVal.GetTwrValStr(TWR_BUZ) );
}
//---------------------------------------------------------------------------

/****************************************************************************
Description	: GRID EVENT HANDLER
*****************************************************************************/
void __fastcall TFormTowerSet::TowerLampGridCanClickCell(TObject *Sender, int ARow,
          int ACol, bool &Allow)
{
	Allow = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormTowerSet::TowerLampGridCanEditCell(TObject *Sender, int ARow,
          int ACol, bool &CanEdit)
{
	CanEdit = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormTowerSet::TowerLampGridGetAlignment(TObject *Sender, int ARow,
		  int ACol, TAlignment &HAlign, Advobj::TVAlignment &VAlign)
{
	HAlign = taCenter;
	VAlign = Advobj::vtaCenter;
}
//---------------------------------------------------------------------------
void __fastcall TFormTowerSet::TowerLampGridGetCellColor(TObject *Sender, int ARow,
          int ACol, TGridDrawState AState, TBrush *ABrush, TFont *AFont)
{
	if(AState.Contains(gdFixed) )
	{
		AFont->Size = 11;
		AFont->Style << fsBold;
		AFont->Name = "¸¼Àº °íµñ";
	}
	else
	{
		if(ACol == TWR_RED + 1)
		{
			AFont->Size = 10;
			AFont->Style << fsBold;
			AFont->Name = "¸¼Àº °íµñ";
			ABrush->Color = clRed;
			AFont->Color = clWhite;
		}
		else if(ACol == TWR_YEL + 1)
		{
			AFont->Size = 10;
			AFont->Style << fsBold;
			AFont->Name = "¸¼Àº °íµñ";
			ABrush->Color = clYellow;
			AFont->Color = clNavy;
		}
		else if(ACol == TWR_GRN + 1)
		{
			AFont->Size = 10;
			AFont->Style << fsBold;
			AFont->Name = "¸¼Àº °íµñ";
			ABrush->Color = clGreen;
			AFont->Color = clWhite;
		}
		else
		{
			AFont->Size = 10;
			AFont->Style << fsBold;
			AFont->Name = "¸¼Àº °íµñ";
			ABrush->Color = clNavy;
			AFont->Color = clWhite;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormTowerSet::TowerLampGridGetEditorProp(TObject *Sender, int ACol,
          int ARow, TEditLink *AEditLink)
{
	if(ACol < 4)
		TowerLampGrid->Combobox->Items->Assign(FsLampList.get());
	else
		TowerLampGrid->Combobox->Items->Assign(FsBuzzList.get());
}
//---------------------------------------------------------------------------
void __fastcall TFormTowerSet::TowerLampGridGetEditorType(TObject *Sender, int ACol,
          int ARow, TEditorType &AEditor)
{
	if(ARow > 0 && ACol > 0)
		AEditor = edComboList;
}
//---------------------------------------------------------------------------
void __fastcall TFormTowerSet::OKButtonClick(TObject *Sender)
{
	if (IsValueChangeed() == false)
	{
		Close();
		return;
	}

	SetMessageStr(GetResStr("Question", "QuestUpdate"));
	if (ShowQuestionDlg() == false)
	{
		return;
	}

	OKButton->Enabled = false;

	SaveChangeLog();

	GetManager()->TowerLampOption->FbChanging = true;
	*(GetManager()->TowerLampOption) = TempTwrVal;
	GetManager()->SaveTwrValue();
	GetManager()->TowerLampOption->FbChanging = false;
	OKButton->Enabled = true;

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormTowerSet::CancelButtonClick(TObject *Sender)
{
	CancelButton->Enabled = false;
	FillTowerSetInfo();
	CancelButton->Enabled = true;

	Close();
}
//---------------------------------------------------------------------------

void ShowTowerLampSet(void)
{
	boost::shared_ptr<TFormTowerSet> TowerSetDlg(new TFormTowerSet(nullptr) );
	TowerSetDlg->ShowModal();
}
//---------------------------------------------------------------------------

