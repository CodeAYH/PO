//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("TowerSetForm.cpp", FormTowerSet);
USEFORM("SystemSettingFrame.cpp", FrmSystemSetting); /* TFrame: File Type */
USEFORM("ShowPtnForm.cpp", FmShowPtn);
USEFORM("ScreenSaver.cpp", ScreenSaver);
USEFORM("TCardChamberRecipeForm.cpp", FmTCardChamberRecipe);
USEFORM("TempGraphFrame.cpp", FrmTempGraph); /* TFrame: File Type */
USEFORM("TCardValidationForm.cpp", FmTCardValidation);
USEFORM("TCardRecipeLoadingForm.cpp", FmTCardRecipeLoading);
USEFORM("TCardInputDialog.cpp", TCardInputDlg);
USEFORM("EzNet\EzNetForm.cpp", FmEzNet);
USEFORM("BarunMES\BarunMES.cpp", FmBarunMES);
USEFORM("AlarmForm.cpp", FmAlarm);
USEFORM("AdapterForm.cpp", FmAdapter);
USEFORM("ChamberSettingFrame.cpp", FrmChamberSetting); /* TFrame: File Type */
USEFORM("ChamberFrame.cpp", FrmChamber); /* TFrame: File Type */
USEFORM("MainForm.cpp", FmMain);
USEFORM("LoginForm.cpp", FmLogin);
USEFORM("LogForm.cpp", FmLog);
USEFORM("JobForm.cpp", FmJob);
USEFORM("RegisterUserForm.cpp", FmRegisterUser);
USEFORM("PatternSettingFrame.cpp", FrmPatternSetting); /* TFrame: File Type */
USEFORM("ParameterSettingFrame.cpp", FrmParameterSetting); /* TFrame: File Type */
USEFORM("GEM\COWELLValidationForm.cpp", JsckGemValidationFrm);
USEFORM("FTPLotInfoForm.cpp", FTPLotInfoForm);
USEFORM("EzNet\JSCKValidationForm.cpp", JsckValidationFrm);
USEFORM("EzNet\JSCKEditProcessForm.cpp", JsckEditProcessFrm);
USEFORM("GEM\GEMForm.cpp", FmGem);
USEFORM("JobEndForm.cpp", FmJobEnd);
USEFORM("IOFrame.cpp", FrmIO); /* TFrame: File Type */
USEFORM("GEM\TerminalMsgForm.cpp", FmTerminalMsg);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		// 중복 실행 방지
		HANDLE hMutex = nullptr;
		hMutex = CreateMutex(nullptr, TRUE, L"VSPO");
		if (nullptr == hMutex)
		{
			throw Exception("Error: CreateMutex failed.!");
		}
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			MessageBox(nullptr, _T("The program is already running."), _T("VSPO"), MB_OK);
			CloseHandle( hMutex );
			return 0;
		}

		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TFmMain), &FmMain);
		Application->CreateForm(__classid(TFmLog), &FmLog);
		Application->CreateForm(__classid(TFmRegisterUser), &FmRegisterUser);
		Application->CreateForm(__classid(TFmJob), &FmJob);
		Application->CreateForm(__classid(TFmAdapter), &FmAdapter);
		Application->CreateForm(__classid(TScreenSaver), &ScreenSaver);
		Application->CreateForm(__classid(TFmTCardRecipeLoading), &FmTCardRecipeLoading);
		Application->CreateForm(__classid(TTCardInputDlg), &TCardInputDlg);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
