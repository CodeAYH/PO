//---------------------------------------------------------------------------
#pragma hdrstop

#include "ShowPtnForm.h"
#include "NormalOvenChamber.h"
#include "NormalOvenSystemManager.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvGlassButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TFmShowPtn::TFmShowPtn(const int nChamber, TComponent* Owner)
	: FnChamber(nChamber), TForm(Owner)
{
	Caption = "Confirm Pattern Information...";
}
//---------------------------------------------------------------------------
void __fastcall TFmShowPtn::FormCreate(TObject *Sender)
{
	InitPtnChart();
}
//---------------------------------------------------------------------------
void __fastcall TFmShowPtn::InitPtnChart()
{
	NormalOvenChamber *chamber = GetChamber(FnChamber);
	if( chamber == nullptr )
		return;

	PtnChart->LeftAxis->Visible = false;

	PtnChart->BottomAxis->Automatic = true;
	PtnChart->BottomAxis->AutomaticMaximum = false;
	PtnChart->BottomAxis->AutomaticMinimum = false;
	PtnChart->BottomAxis->Minimum  = 0;
	PtnChart->BottomAxis->MinorTickCount = 0;
	PtnChart->BottomAxis->Grid->Visible = true;
	PtnChart->BottomAxis->Increment = 1;
	PtnChart->BottomAxis->Maximum = 0 + chamber->Recipe.TempPtn.Stc.SegCount;
	PtnChart->BottomAxis->Minimum = PtnChart->BottomAxis->Maximum - PtnChart->BottomAxis->Increment*chamber->Recipe.TempPtn.Stc.SegCount;

	PtnChart->Series[0]->Clear();
	PtnChart->Series[0]->AddXY(0, chamber->Recipe.TempPtn.Stc.Ssp);

	int cnt = chamber->Recipe.TempPtn.Stc.SegCount+1;
	for(int i=1; i<=cnt; i++)
	{
		if( chamber->Recipe.TempPtn.Seg[i-1].Time > 0)
		{
			String sTempTime;
			float fTemp = chamber->Recipe.TempPtn.Seg[i-1].Tsp;

			sTempTime.sprintf(L"%dm", chamber->Recipe.TempPtn.Seg[i-1].Time);
			if( fTemp > -1.0 )
				PtnChart->Series[0]->AddXY(i, fTemp, sTempTime);
			else
				break;
		}
		else
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmShowPtn::btnPtnOkClick(TObject *Sender)
{
	m_Ret = mrOk;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFmShowPtn::btnCancelClick(TObject *Sender)
{
	m_Ret = mrCancel;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFmShowPtn::FormClose(TObject *Sender, TCloseAction &Action)
{
	ModalResult = m_Ret;
}
//---------------------------------------------------------------------------

