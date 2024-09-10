//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ParameterSettingFrame.h"
#include "NormalOvenChamber.h"
#include "NormalOvenSystemManager.h"
#include "HelperFunctions.h"
#include "Defines.h"
#include "MainTempController.h"
#include "I7018.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvGlassButton"
#pragma link "AdvGrid"
#pragma link "AdvGroupBox"
#pragma link "AdvObj"
#pragma link "AdvOfficeButtons"
#pragma link "AdvPanel"
#pragma link "AdvSmoothProgressBar"
#pragma link "BaseGrid"
#pragma link "AdvUtil"
#pragma resource "*.dfm"
TFrmParameterSetting *FrmParameterSetting;
//---------------------------------------------------------------------------
__fastcall TFrmParameterSetting::TFrmParameterSetting(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
__fastcall TFrmParameterSetting::TFrmParameterSetting(TComponent* Owner, int chamberNo)
	: TFrame(Owner), ChamberNo(chamberNo)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmParameterSetting::BtnUP550SettingClick(TObject *Sender)
{
	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( chamber == nullptr )
		return;

	if( chamber->MainTempController )
	{
		UP550ProgressBar->Maximum = UP550_PARAM_COUNT-1;
		UP550ProgressBar->Position = 0;

		bool completed = true;
		for(int i=0; i<UP550_PARAM_COUNT; i++)
		{
			if( chamber->MainTempController->WriteParameter(i) )
				UP550ProgressBar->GoToValue((double)i);
			else
				completed = false;
		}

		if( completed == false )
			SetMessageStr(String(L"Failed!"));
		else
			SetMessageStr(String(L"Success!"));
		ShowMessageDlg();
		UP550ProgressBar->Position = 0;
	}
}
//---------------------------------------------------------------------------
