//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AdapterForm.h"
#include "NormalOvenSystemManager.h"
#include "JobForm.h"
#include "TempGraphFrame.h"
#include "Database.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvOfficePager"
#pragma link "AdvOfficePagerStylers"
#pragma resource "*.dfm"
TFmAdapter *FmAdapter;
//---------------------------------------------------------------------------
__fastcall TFmAdapter::TFmAdapter(TComponent* Owner)
	: TForm(Owner)
{
	Page[0] = Page1;
	Page[1] = Page2;
	Page[2] = Page3;
	Page[3] = Page4;
	Page[4] = Page5;
	Page[5] = Page6;
}
//---------------------------------------------------------------------------
void __fastcall TFmAdapter::FormClose(TObject *Sender, TCloseAction &Action)
{
	for(int i=0; i<6; i++)
	{
		if( Page[i] )
		{
			TFrame *frame = (TFrame*)Page[i]->FindChildControl(Page[i]->Caption);
			if( frame )
				delete frame;
			if( FFrame[i] )
			{
				delete FFrame[i];
				FFrame[i] = nullptr;
			}
			ChamberPager->RemoveAdvPage(Page[i]);
		}
	}

	String str;

	switch( AdapterType )
	{
	case AD_IO:
		str = "IO Test UI Closed.";
		break;
	case AD_SETTING:
		str = "Setting UI Closed.";
		break;
	case AD_GRAPH:
		str = "Graph UI Closed.";
		break;
	}

	GetOvenDB()->SaveLog(str, MESSAGES, -1);

	FmJob->Show();
}
//---------------------------------------------------------------------------
TFrame* __fastcall TFmAdapter::GetFrame(int idx)
{
	if( idx < 0 ||
		idx >= 6 )
		return nullptr;

	return FFrame[idx];
}
//---------------------------------------------------------------------------
void __fastcall TFmAdapter::SetFrame(int idx, TFrame *frame)
{
	if( idx < 0 ||
		idx >= 6 )
		return;

	if( frame )
	{
		FFrame[idx] = frame;
		FFrame[idx]->Parent = Page[idx];
		Page[idx]->Caption = frame->Hint;
		ChamberPager->AddAdvPage(Page[idx]);

		if( idx ==0 )
			ChamberPager->ActivePage = Page[idx];
	}
	else
	{
		FFrame[idx] = nullptr;
		Page[idx]->Caption = "";
		ChamberPager->RemoveAdvPage(Page[idx]);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmAdapter::Init(int type)
{
	for(int i=0; i<6; i++)
	{
		ChamberPager->RemoveAdvPage(Page[i]);
		delete FFrame[i];
		FFrame[i] = nullptr;
	}
	AdapterType = type;
}
//---------------------------------------------------------------------------
void __fastcall TFmAdapter::FormShow(TObject *Sender)
{
	String str;

	switch( AdapterType )
	{
	case AD_IO:
		str = "IO Test UI Executed.";
		break;
	case AD_SETTING:
		str = "Setting UI Executed.";
		break;
	case AD_GRAPH:
		str = "Graph UI Executed.";
		break;
	}

	GetOvenDB()->SaveLog(str, MESSAGES, -1);
}
//---------------------------------------------------------------------------

void __fastcall TFmAdapter::ChamberPagerChanging(TObject *Sender, int FromPage, int ToPage,
          bool &AllowChange)
{
	if( AdapterType == AD_GRAPH )
		PageNo = ToPage;
}
//---------------------------------------------------------------------------

void __fastcall TFmAdapter::ChamberPagerChange(TObject *Sender)
{
	if( FFrame[PageNo] == nullptr )
		return;

//	if( AdapterType == AD_GRAPH )
//		((TFrmTempGraph*)FFrame[PageNo])->ShowList();
}
//---------------------------------------------------------------------------

