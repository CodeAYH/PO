//---------------------------------------------------------------------------
#include <System.SysUtils.hpp>
//#include <memory>
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <map>
#include "AdvSmoothMessageDialog.hpp"
// 20181012 - Test - jhlim
#include <time.h>
#include <shellapi.h>
#include <tlhelp32.h>
#pragma hdrstop

 #include "HelperFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

std::shared_ptr<TStringList> MessageStr(new TStringList);

String LangTag = ENG_TAG;
String LangStr[LANG_COUNT] =
{
	"KOR",
	"ENG",
	"CHN",
};

String GetProgramTitle()
{
	return UpperCase(Application->Title);
}

String GetRootFolder()
{
	DWORD Drives = GetLogicalDrives();
	bool bDExist = (Drives >> 3) & 0x00000001;
	bool bDNotCD = (GetDriveType(L"D:\\") != DRIVE_CDROM);

	String folder;
	if( bDExist && bDNotCD )
		folder = "D:\\" + GetProgramTitle() + "\\";
	else
		folder = GetCurrentDir() + "\\";

	return folder;
}

String GetDataFolder()
{
	return GetRootFolder() + "DATA\\";
}

String GetLogFolder()
{
	return GetRootFolder() + "LOG\\";
}

String GetReportFolder()
{
	return GetRootFolder() + "REPORT\\";
}

String GetResFolder()
{
	return GetCurrentDir() + "\\RES\\";
}

String GetRemotePatternFolder()
{
	return GetRootFolder() + "REMOTE_PATTERN\\";
}

String GetFTPDataFolder()
{
	return GetDataFolder() + "FTP\\";
}

// 20191216 - SCK MES - jhlim
String GetSCKMESWorkFileFolder()
{
	return GetRootFolder() + "WORK\\";
}

String GetSCKMESWorkDoneFileFolder()
{
	return GetRootFolder() + "WORK_DONE\\";
}

String GetProductFolder()
{
	return GetRootFolder() + "PRODUCT\\";
}

bool IsGemRecipeExist(const String recipe)
{
	String filename = GetRemotePatternFolder();
	filename += recipe;
	filename += ".ptn";

	return FileExists(filename);
}

bool DeleteAllFile(String folder)
{
	bool ret = true;

	String Lfilename;
	WIN32_FIND_DATA filedata;

	HANDLE filehandle = FindFirstFile((folder + "*.*").c_str(), &filedata);
	if( filehandle != INVALID_HANDLE_VALUE )
	{
		do
		{
			Lfilename = folder + filedata.cFileName;
			ret = DeleteFile(Lfilename);
		} while(FindNextFile(filehandle, &filedata));
		FindClose(filehandle);
	}
	return ret;
}

StringListPtr LanguageSectionList;

void LoadLanguageStr()
{
	LanguageSectionList.reset(new TStringList);

	String iniFile = GetDataFolder() + LANGUAGEINI;
	std::shared_ptr<TIniFile> LangFile(new TIniFile(iniFile));

	LangFile->ReadSections(LanguageSectionList.get());
	for(int i=0; i<LanguageSectionList->Count; i++)
	{
		std::map<String, String> *KeyValueList = new std::map<String, String>;
		TStringList *tmpKeyList = new TStringList;

		String sec = LanguageSectionList->Strings[i];

		LangFile->ReadSection(sec, tmpKeyList);
		for(int j=0; j<tmpKeyList->Count; j++)
		{
			String key = tmpKeyList->Strings[j];
			(*KeyValueList)[key] = LangFile->ReadString(sec, key, key);
		}
		LanguageSectionList->Objects[i] = (TObject*)KeyValueList;
		delete tmpKeyList;
	}
}

void DestroyLanguageStr()
{
	if( LanguageSectionList == nullptr )
		return;

    for(int i=0; i<LanguageSectionList->Count; i++)
	{
		std::map<String, String> *KeyValueList;

		KeyValueList = (std::map<String, String> *)LanguageSectionList->Objects[i];
		if( KeyValueList )
			delete KeyValueList;
	}
}

String GetResStr(const String sec, const String key)
{
	if( LanguageSectionList == nullptr )
		return key;

	String lanKey, value = key;
	int idx;

	lanKey.sprintf(L"%s%s", key.c_str(), LangTag.c_str());
	idx = LanguageSectionList->IndexOf(sec);
	if( idx >= 0 )
	{
		std::map<String, String> *KeyValueList;

		KeyValueList = (std::map<String, String> *)LanguageSectionList->Objects[idx];
		if( KeyValueList )
			value = (*KeyValueList)[lanKey];
	}

	return value;
}
/*
String __fastcall
GetResStr(const String sSection, const String sKey)
{
	String iniFile = GetDataFolder() + LANGUAGEINI;
	boost::shared_ptr<TIniFile> LangFile(new TIniFile(iniFile));
	String res = sKey;
	String key;

	key.sprintf(L"%s%s", sKey, LangTag);
	if( LangFile->SectionExists(sSection) )
		res = LangFile->ReadString(sSection, key, sKey);

	return res;
} */

String GetIOName(const String key)
{
/*	String iniFile = GetDataFolder() + LANGUAGEINI;
	boost::shared_ptr<TIniFile> LangFile(new TIniFile(iniFile));
	String res = "";
	String section = "IO";

	if( LangFile->SectionExists(section) )
		res = LangFile->ReadString(section, key, "");

	return res;*/

	if( LanguageSectionList == nullptr )
		return key;

	String value = key;
	String sec = "IO";
	int idx;

	idx = LanguageSectionList->IndexOf(sec);
	if( idx >= 0 )
	{
		std::map<String, String> *KeyValueList;

		KeyValueList = (std::map<String, String> *)LanguageSectionList->Objects[idx];
		if( KeyValueList )
			value = (*KeyValueList)[key];
	}

	return value;
}

#define EPSILON 0.0001  //정확도.
// 실수형 비교 자체는 정확하지 않기 때문에 정확도 비교를 한다.

bool double_compare(double a, double b)
{
	return fabs(a-b) < EPSILON; // 앞서 정의한 0.0001 이하는 비교하지 않습니다.
}

// 16진수 문자열이 맞는지 검사한다.
bool IsValidHex(AnsiString hex)
{
	if( hex == "" )
		return false;

	for(int i=1; i<=hex.Length(); i++)
	{
		char c = hex[i];

		if( !((c >= '0' && c <= '9') ||
			(c >= 'A' && c <= 'F') ) )//||
//			(c >= 'a' && c <= 'f')) )
			return false;
	}
	return true;
}

// 16진수를 10진수로 바꾼다.
// 4바이트 이며, hex 값은 0x 를제외한 값.
// 부호 있는 값으로 변환한다.
int HexToInt(AnsiString hex)
{
	int _nTmp;

	if( hex == "" )
		return 0;

	hex = "0x" + hex;
	_nTmp = StrToInt(hex); // 10진수로 바꿈

	if (_nTmp & 0x8000)  //음수 처리
		_nTmp = _nTmp - 0x10000;

	return _nTmp;
}

void SetMessageStr(String str)
{
	if( MessageStr == nullptr )
		//MessageStr = boost::shared_ptr<TStringList>(new TStringList);
		MessageStr.reset(new TStringList);

	String msg = StringReplace(str, ">", "<br>", TReplaceFlags() << rfReplaceAll);
/*	int pos = str.Pos(">");
	if( pos )
	{
		MessageStr->Add(str.SubString(1, pos-1));
		MessageStr->Add(str.SubString(pos+1, str.Length()-pos));
	}
	else
		MessageStr->Add(str); */
	MessageStr->Add(msg);
}

std::shared_ptr<TAdvSmoothMessageDialog> CreateMessageDlg(String caption)
{
	std::shared_ptr<TAdvSmoothMessageDialog> MessageDlg;

	if( MessageStr == nullptr ||
		MessageStr->Count <= 0 )
	{
		MessageDlg.reset();
		return MessageDlg;
	}

 //	MessageDlg = boost::shared_ptr<TAdvSmoothMessageDialog>(new TAdvSmoothMessageDialog(nullptr));
	MessageDlg.reset(new TAdvSmoothMessageDialog(nullptr));

	String str, msg;

	MessageDlg->Caption = caption;

	msg = "";
	for(int i=0; i<MessageStr->Count; i++)
		msg += MessageStr->Strings[i] + "<br>";
	str.sprintf(L"<P align=\"center\">%s</P>", msg.c_str());
	MessageDlg->HTMLText->Text = str;
	MessageDlg->HTMLText->Location = hlCenterCenter;
	MessageDlg->HTMLText->Font->Size = 12;
	MessageDlg->HTMLText->Font->Style = TFontStyles() << fsBold;
	MessageDlg->HTMLText->Top = 40;

	MessageDlg->CaptionFont->Size = 12;
	MessageDlg->CaptionFont->Style = TFontStyles() << fsBold;
	MessageDlg->CaptionFont->Color = clWhite;

	MessageDlg->ButtonFont->Size = 10;
	MessageDlg->ButtonFont->Style = TFontStyles() << fsBold;

	MessageDlg->CaptionFill->Color = clHighlight;
	MessageDlg->CaptionFill->ColorTo = clHighlight;
	MessageDlg->CaptionFill->Opacity = 255 * 0.8;

	MessageDlg->Fill->Color = clCream;
	MessageDlg->Fill->ColorTo = clCream;
	MessageDlg->Fill->Opacity = 255 * 0.8;
	MessageDlg->Fill->BorderColor = clBlack;
	MessageDlg->Fill->BorderOpacity = 255;

	MessageDlg->ButtonAreaFill->Color = clCream;
	MessageDlg->ButtonAreaFill->ColorTo = clCream;
	MessageDlg->ButtonAreaFill->Opacity = 255 * 0.8;

	MessageDlg->Position = poScreenCenter;
	MessageDlg->ButtonSpacing = 10;

	MessageDlg->Fill->Rounding = 10;
	MessageDlg->CaptionFill->Rounding = 10;
	MessageDlg->CaptionFill->RoundingType = rtTop;
	MessageDlg->ButtonAreaFill->Rounding = 10;
	MessageDlg->FormStyle = fsStayOnTop;

	return MessageDlg;
}

void ShowMessageDlg()
{
	ShowMessageDlg(GetProgramTitle());
}

void ShowMessageDlg(String caption)
{
	std::shared_ptr<TAdvSmoothMessageDialog> MessageDlg = CreateMessageDlg(caption);

	if( MessageDlg == nullptr )
		return;

	TAdvSmoothMessageDialogButton *button = MessageDlg->Buttons->Add();
	button->ButtonResult = mrOk;
	button->Color = clSkyBlue;
	button->ColorFocused = clSkyBlue;
	button->ColorDown = clCream;
	button->HoverColor = clCream;
	button->Caption = GetResStr("Common", "OK");

	MessageDlg->ExecuteDialog();
	MessageStr->Clear();
	MessageDlg.reset();
}

bool ShowQuestionDlg()
{
	return ShowQuestionDlg(GetProgramTitle());
}

bool ShowQuestionDlg(String caption)
{
	std::shared_ptr<TAdvSmoothMessageDialog> MessageDlg = CreateMessageDlg(caption);

	TAdvSmoothMessageDialogButton *button = MessageDlg->Buttons->Add();
	button->ButtonResult = mrYes;
	button->Color = clSkyBlue;
	button->ColorFocused = clSkyBlue;
	button->ColorDown = clCream;
	button->HoverColor = clCream;
	button->Caption = GetResStr("Common", "OK");

	button = MessageDlg->Buttons->Add();
	button->ButtonResult = mrNo;
	button->Color = clSkyBlue;
	button->ColorFocused = clSkyBlue;
	button->ColorDown = clCream;
	button->HoverColor = clCream;
	button->Caption = GetResStr("Common", "Cancel");

	int res = MessageDlg->ExecuteDialog();
	MessageStr->Clear();
	MessageDlg.reset();
	if( res == mrYes )
		return true;
	return false;
}

bool IsValidInt(AnsiString istr)
{
	if( istr == "" )
		return false;

	for(int i=1; i<=istr.Length(); i++)
	{
		char c = istr[i];

		if( !(c >= '0' && c <= '9') )
			return false;
	}
	return true;
}

// 십진수 실수형 문자열이 맞는지 검사한다.
// i-7018과 통신할 때 사용
bool IsValidFloat(AnsiString fstr)
{
	if( fstr == "" )
		return false;

	for(int i=1; i<=fstr.Length(); i++)
	{
		char c = fstr[i];

		if( !((c >= '0' && c <= '9') ||
			(c == '+' ) ||
			(c == '-' ) ||
			(c == '.' )) )
			return false;
	}
	return true;
}

int CheckSum(AnsiString strData)
{
	int len, i, sum;
	char c;

	len = strData.Length();

	sum = 0;
	for(i=1; i<=len; i++)
	{
		c = strData[i];
		sum += c;
	}

	sum = sum & 0xff;
	sum = 0x100 - sum;

	return sum;
}

AnsiString MakeCheckSum(AnsiString strData)
{
	int sum;
	AnsiString chksum, tmp;

	sum = CheckSum(strData);

	tmp.sprintf("%02X", sum);
	chksum = tmp.SubString(tmp.Length()-1, 2);
	return chksum;
}

unsigned int SetBit(unsigned int x, int n, bool on)
{
	if( on == false )
		return (unsigned int) ( x | (1 << n) );

	return (unsigned int) ( x & (~(1 << n)) );
}

// 지정한 정수에서, 몇번째 비트만 읽어서 반환하는 함수
int GetAbit(unsigned short x, int n)
{
  return (x & (1 << n)) >> n;
}

Graphics::TBitmap* CaptureControl(TControl *ctrl)
{
	Graphics::TBitmap *bmp = new Graphics::TBitmap;
	bmp->Width = ctrl->Width;
	bmp->Height = ctrl->Height;
	if( ctrl->InheritsFrom(__classid(TWinControl)) )
    {
		TWinControl *wCtrl = (TWinControl*)ctrl;
        bmp->Canvas->Lock();
		if( GetWindowLong(wCtrl->Handle, GWL_STYLE) & WS_BORDER )
			wCtrl->PaintTo(bmp->Canvas->Handle, 1, 1);
		else
			wCtrl->PaintTo(bmp->Canvas->Handle, 0, 0);
        bmp->Canvas->Unlock();
	}
    else
    {
		ctrl->Perform(WM_PAINT, (int)bmp->Canvas->Handle, 0);
	}

    return bmp;
}

bool GetSerialList(TStringList *List)
{
	bool bRet;

	std::shared_ptr<TRegistry> reg(new TRegistry());
	std::shared_ptr<TStringList> Name(new TStringList());

	List->Clear();

    reg->RootKey = HKEY_LOCAL_MACHINE;
	reg->Access = KEY_READ;
    bRet = reg->OpenKey("HARDWARE\\DEVICEMAP\\SERIALCOMM", false);
	if (bRet == false)
		return false;

    reg->GetValueNames(Name.get());

    for (int i = 0; i < Name->Count; i++)
    {
        // \\device\\ 의 문자열을 "" 으로 변환
        TReplaceFlags flag = TReplaceFlags() << rfReplaceAll << rfIgnoreCase;
        String Value = StringReplace(Name->Strings[i], "\\device\\", "", flag);

        // TStringList 에서 values 사용이 가능하도록 변경 후 Add
        String Str;
		Str = reg->ReadString(Name->Strings[i]);
		List->Add(Str);
	   /*if(OnlyAvail)//유효한 시리얼포트만 리턴한다
        {
            if(ValidateComPort(Str.w_str()))
            {
				if(SubName)
					Str = Str.Format("%s=%s", ARRAYOFCONST((Str.Trim(), Value)));
				List->Add(Str);
            }
        }
		else
        {
			if(SubName)
				Str = Str.Format("%s=%s", ARRAYOFCONST((Str.Trim(), Value)));
			List->Add(Str);
        }*/
	}

    reg->CloseKey();
		//FList->CustomSort(ListSortIntegerProc);
	return true;
}

void WaitSecond(double sec)
{
	int cnt = 0;
	cnt = sec * 1000. / 100.;
	for(int i=0; i<cnt; i++)
	{
		Application->ProcessMessages();
		Sleep(100);
	}
}

// 20181011 - Gem Test - jhlim
#if 0
void WaitSecond(double sec)
{
	int cnt = 0;
	cnt = sec * 1000;
	clock_t start_clk = clock();
	while(true)
	{
		Application->ProcessMessages(); //중간중간 다른프로세스를 실행하기위해
		Sleep(1);
		if( (clock() - start_clk) > cnt) break;
	}
}
#endif

/****************************************************************************
2012-08-27
Description
	: Version information
*****************************************************************************/
String GetInformation(String AFilename, UnicodeString AQuery)
{
/*    ※ AQuery List
	- CompanyName
    - FileDescription
	- FileVersion
	- InternalName
    - LegalCopyright
    - LegalTrademarks
    - ProductName
    - ProductVersion
*/

	if( AQuery.IsEmpty() )
		return "";

	DWORD dwHandle;
	LPCVOID ptr;
    UINT uLength;

	DWORD dwVersionInfoSize = GetFileVersionInfoSize(AFilename.c_str(),
													 &dwHandle);
	if( dwVersionInfoSize == 0 )
		return "";

	LPVOID pFileInfo = (LPVOID) HeapAlloc(GetProcessHeap(),
										  HEAP_ZERO_MEMORY,
										  dwVersionInfoSize);

	GetFileVersionInfo(AFilename.c_str(),
					   dwHandle,
					   dwVersionInfoSize,
					   pFileInfo);
	VerQueryValue(pFileInfo,
				  TEXT("\\VarFileInfo\\Translation"),
				  (LPVOID*)&ptr,
				  &uLength);

    WORD *id = (WORD*) ptr;
	String szString =
		"\\StringFileInfo\\" + IntToHex(id[0], 4) +
		IntToHex(id[1], 4) + "\\" + AQuery;

	VerQueryValue(pFileInfo,
				  szString.c_str(),
				  (LPVOID*)&ptr,
				  &uLength);
	if( uLength == 0 )
		return "";

	String sOut = String((TCHAR *) ptr);
	HeapFree(GetProcessHeap(), 0, pFileInfo);

	return sOut;
}

bool LoadTempPtn(int chamberNo, int ptnNo, PTN &ptn, bool remote/*=false*/)
{
	ptn.Empty();

	String strFolder, strFile;
	strFolder.sprintf(L"%s%s%c\\",
					  GetRootFolder().c_str(),
					  L"PATTERN_CM_",
					  'A'+chamberNo);
	if( remote )
		strFile.sprintf(L"%sremote.ptn", strFolder.c_str());
	else
		strFile.sprintf(L"%s%d.ptn", strFolder.c_str(), ptnNo);
	if( FileExists(strFile) )
	{
		String sec = "PATTERN_T";
		String key;
		std::shared_ptr<TIniFile> ptnFile(new TIniFile(strFile));

		if( ptnFile->SectionExists(sec) == false )
			sec = "PATTERN";

		ptn.PtnNo = ptnNo;
		ptn.TotalTime = 0;
		ptn.MaxTemp = 0;
		ptn.Stc.SegCount = ptnFile->ReadInteger(sec, "SegCount", 0);
		ptn.Stc.Ssp = ptnFile->ReadFloat(sec, "Ssp", 0);
		ptn.Stc.Stc = ptnFile->ReadInteger(sec, "Stc", 0);
		ptn.Stc.Rcy = ptnFile->ReadInteger(sec, "Rcy", 0);
		ptn.Stc.Rst = ptnFile->ReadInteger(sec, "Rst", 0);
		ptn.Stc.Ren = ptnFile->ReadInteger(sec, "Ren", 0);

		ptn.VacSP = ptnFile->ReadInteger(sec, "VacSP", 500);
		ptn.VacHighLimit = ptnFile->ReadInteger(sec, "VacHighLimit", 1000);
		ptn.VacLowLimit = ptnFile->ReadInteger(sec, "VacLowLimit", 0);
		ptn.CuringVacuumLevel = ptnFile->ReadInteger(sec, "CuringVacuumLevel", 250);
		ptn.CoolingVacuumLevel = ptnFile->ReadInteger(sec, "CoolingVacuumLevel", 5000);
		ptn.UseN2Purge = ptnFile->ReadBool(sec, "UseN2Purge", false);

//		ptn.CureStartOption = ptnFile->ReadInteger(sec, "CureStartOpt", CS_AFTER_PURGE);
		ptn.O2AnalyzerPowerOff = ptnFile->ReadInteger(sec, "O2AnalyzerPower", OP_NONE);

        ptn.UseLocalMode = ptnFile->ReadBool(sec, "UseLocalMode", false);
        ptn.LocalTemp = ptnFile->ReadInteger(sec, "LocalTemp", 0);

		for(int i=0; i<5; i++)
		{
			key.sprintf(L"Wz[%d]", i);
			ptn.Stc.Wz[i] = ptnFile->ReadFloat(sec, key, 0);
			key.sprintf(L"WTm[%d]", i);
			ptn.Stc.WTm[i] = ptnFile->ReadInteger(sec, key, 0);
		}

		for(int i=0; i<ptn.Stc.SegCount; i++)
		{
			key.sprintf(L"Tsp[%d]", i);
			ptn.Seg[i].Tsp = ptnFile->ReadFloat(sec, key, 0);
			if( ptn.MaxTemp < ptn.Seg[i].Tsp )
				ptn.MaxTemp = ptn.Seg[i].Tsp;
			key.sprintf(L"Time[%d]", i);
			ptn.Seg[i].Time = ptnFile->ReadInteger(sec, key, 0);
			ptn.TotalTime += ptn.Seg[i].Time;
			key.sprintf(L"TmRt[%d]", i);
			ptn.Seg[i].TmRt = ptnFile->ReadInteger(sec, key, 0);
			key.sprintf(L"Pid[%d]", i);
			ptn.Seg[i].Pid = ptnFile->ReadInteger(sec, key, 0);
			key.sprintf(L"Jc[%d]", i);
			ptn.Seg[i].Jc = ptnFile->ReadInteger(sec, key, 0);

			for(int j=0; j<8; j++)
			{
				key.sprintf(L"Seg[%d].EvKind[%d]", i, j);
				ptn.Seg[i].EvKind[j] = ptnFile->ReadInteger(sec, key, 0);
				key.sprintf(L"Seg[%d].OnType[%d]", i, j);
				ptn.Seg[i].OnType[j] = ptnFile->ReadInteger(sec, key, 0);
				key.sprintf(L"Seg[%d].OffPe[%d]", i, j);
				ptn.Seg[i].OffPe[j] = ptnFile->ReadInteger(sec, key, 0);
			}

			key.sprintf(L"DamperIn[%d]", i);
			if( ptnFile->ValueExists(sec, key) )
				ptn.Damper[i].InPercent = ptnFile->ReadInteger(sec, key, 0);
			else
			{
				key.sprintf(L"InDegree[%d]", i);
				ptn.Damper[i].InPercent = ptnFile->ReadInteger(sec, key, 0);
			}

			key.sprintf(L"DamperOut[%d]", i);
			if( ptnFile->ValueExists(sec, key) )
				ptn.Damper[i].OutPercent = ptnFile->ReadInteger(sec, key, 0);
			else
			{
				key.sprintf(L"OutDegree[%d]", i);
				ptn.Damper[i].OutPercent = ptnFile->ReadInteger(sec, key, 0);
			}

			key.sprintf(L"InverterFreq[%d]", i);
			ptn.InverterFrequency[i] = ptnFile->ReadFloat(sec, key, 0);
		}
		ptn.IsLoaded = true;
		return true;
	}

	ptn.IsLoaded = false;
	return false;
}

bool LoadRemoteTempPtn(String recipe, PTN &ptn)
{
	String strFolder, strFile;
	strFolder =  GetRemotePatternFolder();

	strFile.sprintf(L"%s%s.ptn", strFolder.c_str(), recipe.c_str());

	if( FileExists(strFile) )
	{
		String sec = "PATTERN_T";
		String key;
		std::shared_ptr<TIniFile> ptnFile(new TIniFile(strFile));

		if( ptnFile->SectionExists(sec) == false )
			sec = "PATTERN";

		ptn.PtnNo = 1;
		ptn.TotalTime = 0;
		ptn.MaxTemp = 0;
		ptn.Stc.SegCount = ptnFile->ReadInteger(sec, "SegCount", 0);
		ptn.Stc.Ssp = ptnFile->ReadFloat(sec, "Ssp", 0);
		ptn.Stc.Stc = ptnFile->ReadInteger(sec, "Stc", 0);
		ptn.Stc.Rcy = ptnFile->ReadInteger(sec, "Rcy", 0);
		ptn.Stc.Rst = ptnFile->ReadInteger(sec, "Rst", 0);
		ptn.Stc.Ren = ptnFile->ReadInteger(sec, "Ren", 0);

		ptn.VacSP = ptnFile->ReadInteger(sec, "VacSP", 500);
		ptn.VacHighLimit = ptnFile->ReadInteger(sec, "VacHighLimit", 1000);
		ptn.VacLowLimit = ptnFile->ReadInteger(sec, "VacLowLimit", 0);
		ptn.CuringVacuumLevel = ptnFile->ReadInteger(sec, "CuringVacuumLevel", 250);
		ptn.CoolingVacuumLevel = ptnFile->ReadInteger(sec, "CoolingVacuumLevel", 5000);
		ptn.UseN2Purge = ptnFile->ReadBool(sec, "UseN2Purge", false);

//		ptn.CureStartOption = ptnFile->ReadInteger(sec, "CureStartOpt", CS_AFTER_PURGE);
		ptn.O2AnalyzerPowerOff = ptnFile->ReadInteger(sec, "O2AnalyzerPower", OP_NONE);

        ptn.UseLocalMode = ptnFile->ReadBool(sec, "UseLocalMode", false);
        ptn.LocalTemp = ptnFile->ReadInteger(sec, "LocalTemp", 0);

		for(int i=0; i<5; i++)
		{
			key.sprintf(L"Wz[%d]", i);
			ptn.Stc.Wz[i] = ptnFile->ReadFloat(sec, key, 0);
			key.sprintf(L"WTm[%d]", i);
			ptn.Stc.WTm[i] = ptnFile->ReadInteger(sec, key, 0);
		}

		for(int i=0; i<ptn.Stc.SegCount; i++)
		{
			key.sprintf(L"Tsp[%d]", i);
			ptn.Seg[i].Tsp = ptnFile->ReadFloat(sec, key, 0);
			if( ptn.MaxTemp < ptn.Seg[i].Tsp )
				ptn.MaxTemp = ptn.Seg[i].Tsp;
			key.sprintf(L"Time[%d]", i);
			ptn.Seg[i].Time = ptnFile->ReadInteger(sec, key, 0);
			ptn.TotalTime += ptn.Seg[i].Time;
			key.sprintf(L"TmRt[%d]", i);
			ptn.Seg[i].TmRt = ptnFile->ReadInteger(sec, key, 0);
			key.sprintf(L"Pid[%d]", i);
			ptn.Seg[i].Pid = ptnFile->ReadInteger(sec, key, 0);
			key.sprintf(L"Jc[%d]", i);
			ptn.Seg[i].Jc = ptnFile->ReadInteger(sec, key, 0);

			for(int j=0; j<8; j++)
			{
				key.sprintf(L"Seg[%d].EvKind[%d]", i, j);
				ptn.Seg[i].EvKind[j] = ptnFile->ReadInteger(sec, key, 0);
				key.sprintf(L"Seg[%d].OnType[%d]", i, j);
				ptn.Seg[i].OnType[j] = ptnFile->ReadInteger(sec, key, 0);
				key.sprintf(L"Seg[%d].OffPe[%d]", i, j);
				ptn.Seg[i].OffPe[j] = ptnFile->ReadInteger(sec, key, 0);
			}

			key.sprintf(L"DamperIn[%d]", i);
			if( ptnFile->ValueExists(sec, key) )
				ptn.Damper[i].InPercent = ptnFile->ReadInteger(sec, key, 0);
			else
			{
				key.sprintf(L"InDegree[%d]", i);
				ptn.Damper[i].InPercent = ptnFile->ReadInteger(sec, key, 0);
			}

			key.sprintf(L"DamperOut[%d]", i);
			if( ptnFile->ValueExists(sec, key) )
				ptn.Damper[i].OutPercent = ptnFile->ReadInteger(sec, key, 0);
			else
			{
				key.sprintf(L"OutDegree[%d]", i);
				ptn.Damper[i].OutPercent = ptnFile->ReadInteger(sec, key, 0);
			}
		}
		ptn.IsLoaded = true;
		return true;
	}

	ptn.IsLoaded = false;
	return false;
}

bool LoadPressPtn(int chamberNo, int ptnNo, PTN &ptn, bool remote/*=false*/)
{
	String strFolder, strFile;
	strFolder.sprintf(L"%s%s%c\\",
					  GetRootFolder().c_str(),
					  L"PATTERN_CM_",
					  'A'+chamberNo);
	if( remote )
		strFile.sprintf(L"%sremote.ptn", strFolder.c_str());
	else
		strFile.sprintf(L"%s%d.ptn", strFolder.c_str(), ptnNo);
	if( FileExists(strFile) )
	{
		String sec = "PATTERN_P";
		String key;
		std::shared_ptr<TIniFile> ptnFile(new TIniFile(strFile));

		ptn.TotalTime = 0;
		ptn.MaxTemp = 0;
		ptn.Stc.SegCount = ptnFile->ReadInteger(sec, "SegCount", 0);
		ptn.Stc.Ssp = ptnFile->ReadFloat(sec, "Ssp", 0);
		ptn.Stc.Stc = ptnFile->ReadInteger(sec, "Stc", 0);
		ptn.PressOption = ptnFile->ReadInteger(sec, "PressOpt", 0);
		ptn.DepressOption = ptnFile->ReadInteger(sec, "DepressOpt", 0);
		ptn.DepressLevel = ptnFile->ReadFloat(sec, "DepressLevel", 5);

		// Discolor 방지 임시 기능 용
		ptn.UsePreventOxydation = ptnFile->ReadBool(sec, "UsePreventOxydation", false);
		ptn.PreventOxydationStartPress = ptnFile->ReadFloat(sec, "PreventOxydationStartPress", 0.0);
		ptn.PreventOxydationTime = ptnFile->ReadInteger(sec, "PreventOxydationTime", 0);

		ptn.VacuumOption = ptnFile->ReadInteger(sec, "VacuumOpt", 0);
		ptn.VacuumTemp = ptnFile->ReadInteger(sec, "VacuumTemp", 90);
		ptn.VacuumRampupTime = ptnFile->ReadInteger(sec, "VacuumRampupTime", 30);
		ptn.VacuumHoldingTime = ptnFile->ReadInteger(sec, "VacuumHoldingTime", 30);

		for(int i=0; i<ptn.Stc.SegCount; i++)
		{
			key.sprintf(L"Tsp[%d]", i);
			ptn.Seg[i].Tsp = ptnFile->ReadFloat(sec, key, 0);
			if( ptn.MaxTemp < ptn.Seg[i].Tsp )
				ptn.MaxTemp = ptn.Seg[i].Tsp;

			key.sprintf(L"Time[%d]", i);
			ptn.Seg[i].Time = ptnFile->ReadInteger(sec, key, 0);
			ptn.TotalTime += ptn.Seg[i].Time;

			key.sprintf(L"UseFumeCollector[%d]", i);
			ptn.UseFumeCollector[i] = ptnFile->ReadBool(sec, key, false);
		}
		for(int j=0; j<2; j++)
		{
			key.sprintf(L"EvKind[%d]", j);
			ptn.Seg[j].EvKind[0] = ptnFile->ReadInteger(sec, key, 0);
			key.sprintf(L"OnType[%d]", j);
			ptn.Seg[j].OnType[0] = ptnFile->ReadInteger(sec, key, 0);
			key.sprintf(L"OffPe[%d]", j);
			ptn.Seg[j].OffPe[0] = ptnFile->ReadInteger(sec, key, 0);
		}
		return true;
	}

	return false;
}

bool SaveTempPtn(int chamberNo, PTN ptn, bool remote/*=false*/)
{
	String strFolder, strFile;
	strFolder.sprintf(L"%s%s%c\\",
					  GetRootFolder().c_str(),
					  L"PATTERN_CM_",
					  'A'+chamberNo);
	if( remote )
		strFile.sprintf(L"%sremote.ptn", strFolder.c_str());
	else
		strFile.sprintf(L"%s%d.ptn", strFolder.c_str(), ptn.PtnNo);

	if( FileExists(strFile) == false )
	{
		try
		{
			int hFile = FileCreate(strFile);
			FileClose(hFile);
		}
		catch(...)
		{
			return false;
		}
	}

	String sec = "PATTERN_T";
	String key, str;
	std::shared_ptr<TIniFile> ptnFile(new TIniFile(strFile));

	ptnFile->WriteInteger(sec, "SegCount", ptn.Stc.SegCount);
	str.sprintf(L"%.1f", ptn.Stc.Ssp);
	ptnFile->WriteString(sec, "Ssp", str);
	ptnFile->WriteInteger(sec, "Stc", ptn.Stc.Stc);
	ptnFile->WriteInteger(sec, "Rcy", ptn.Stc.Rcy);
	ptnFile->WriteInteger(sec, "Rst", ptn.Stc.Rst);
	ptnFile->WriteInteger(sec, "Ren", ptn.Stc.Ren);

	ptnFile->WriteInteger(sec, "VacSP", ptn.VacSP);
	ptnFile->WriteInteger(sec, "VacHighLimit", ptn.VacHighLimit);
	ptnFile->WriteInteger(sec, "CuringVacuumLevel", ptn.CuringVacuumLevel);
	ptnFile->WriteInteger(sec, "CoolingVacuumLevel", ptn.CoolingVacuumLevel);
	ptnFile->WriteInteger(sec, "VacLowLimit", ptn.VacLowLimit);
	ptnFile->WriteBool(sec, "UseN2Purge", ptn.UseN2Purge);

	ptnFile->WriteInteger(sec, "CureStartOpt", ptn.CureStartOption);
	ptnFile->WriteInteger(sec, "O2AnalyzerPower", ptn.O2AnalyzerPowerOff);

    ptnFile->WriteBool(sec, "UseLocalMode", ptn.UseLocalMode);
    ptnFile->WriteInteger(sec, "LocalTemp", ptn.LocalTemp);

	for(int i=0; i<5; i++)
	{
		key.sprintf(L"Wz[%d]", i);
		ptnFile->WriteInteger(sec, key, ptn.Stc.Wz[i]);
		key.sprintf(L"WTm[%d]", i);
		ptnFile->WriteInteger(sec, key, ptn.Stc.WTm[i]);
	}

	for(int i=0; i<ptn.Stc.SegCount; i++)
	{
		key.sprintf(L"Tsp[%d]", i);
		ptnFile->WriteInteger(sec, key, ptn.Seg[i].Tsp);
		key.sprintf(L"Time[%d]", i);
		ptnFile->WriteInteger(sec, key, ptn.Seg[i].Time);
		key.sprintf(L"TmRt[%d]", i);
		ptnFile->WriteInteger(sec, key, ptn.Seg[i].TmRt);
		key.sprintf(L"Pid[%d]", i);
		ptnFile->WriteInteger(sec, key, ptn.Seg[i].Pid);

        if (i == ptn.Stc.SegCount-1)
        {
        	ptn.Seg[i].Jc = ptn.UseLocalMode ? 2 : 0;
        }
		key.sprintf(L"Jc[%d]", i);
		ptnFile->WriteInteger(sec, key, ptn.Seg[i].Jc);

		for(int j=0; j<8; j++)
		{
			key.sprintf(L"Seg[%d].EvKind[%d]", i, j);
			ptnFile->WriteInteger(sec, key, ptn.Seg[i].EvKind[j]);
			key.sprintf(L"Seg[%d].OnType[%d]", i, j);
			ptnFile->WriteInteger(sec, key, ptn.Seg[i].OnType[j]);
			key.sprintf(L"Seg[%d].OffPe[%d]", i, j);
			ptnFile->WriteInteger(sec, key, ptn.Seg[i].OffPe[j]);
		}

		key.sprintf(L"DamperIn[%d]", i);
		ptnFile->WriteInteger(sec, key, ptn.Damper[i].InPercent);
		key.sprintf(L"DamperOut[%d]", i);
		ptnFile->WriteInteger(sec, key, ptn.Damper[i].OutPercent);

		key.sprintf(L"InverterFreq[%d]", i);
		ptnFile->WriteFloat(sec, key, ptn.InverterFrequency[i]);
	}

	return true;
}

bool SavePressPtn(int chamberNo, PTN ptn, bool remote/*=false*/)
{
	String strFolder, strFile;
	strFolder.sprintf(L"%s%s%c\\",
					  GetRootFolder().c_str(),
					  L"PATTERN_CM_",
					  'A'+chamberNo);
	if( remote )
		strFile.sprintf(L"%sremote.ptn", strFolder.c_str());
	else
		strFile.sprintf(L"%s%d.ptn", strFolder.c_str(), ptn.PtnNo);

	if( FileExists(strFile) == false )
	{
		try
		{
			int hFile = FileCreate(strFile);
			FileClose(hFile);
		}
		catch(...)
		{
			return false;
		}
	}

	String sec = "PATTERN_P";
	String key, str;
	std::shared_ptr<TIniFile> ptnFile(new TIniFile(strFile));

	ptnFile->WriteInteger(sec, "SegCount", ptn.Stc.SegCount);
	str.sprintf(L"%.1f", ptn.Stc.Ssp);
	ptnFile->WriteString(sec, "Ssp", str);
	ptnFile->WriteInteger(sec, "Stc", ptn.Stc.Stc);
	ptnFile->WriteInteger(sec, "PressOpt", ptn.PressOption);
	ptnFile->WriteInteger(sec, "DepressOpt", ptn.DepressOption);
	str.sprintf(L"%.1f", ptn.DepressLevel);
	ptnFile->WriteString(sec, "DepressLevel", str);

	// Discolor 방지 임시 기능 용
	ptnFile->WriteBool(sec, "UsePreventOxydation", ptn.UsePreventOxydation);
	str.sprintf(L"%.1f", ptn.PreventOxydationStartPress);
	ptnFile->WriteString(sec, "PreventOxydationStartPress", str);
	ptnFile->WriteInteger(sec, "PreventOxydationTime", ptn.PreventOxydationTime);

	ptnFile->WriteInteger(sec, "VacuumOpt", ptn.VacuumOption);
	ptnFile->WriteInteger(sec, "VacuumTemp", ptn.VacuumTemp);
	ptnFile->WriteInteger(sec, "VacuumRampupTime", ptn.VacuumRampupTime);
	ptnFile->WriteInteger(sec, "VacuumHoldingTime", ptn.VacuumHoldingTime);

	for(int i=0; i<ptn.Stc.SegCount; i++)
	{
		key.sprintf(L"Tsp[%d]", i);
		str.sprintf(L"%.1f", ptn.Seg[i].Tsp);
		ptnFile->WriteString(sec, key, str);
		key.sprintf(L"Time[%d]", i);
		ptnFile->WriteInteger(sec, key, ptn.Seg[i].Time);
		key.sprintf(L"UseFumeCollector[%d]", i);
		ptnFile->WriteBool(sec, key, ptn.UseFumeCollector[i]);
	}
	for(int j=0; j<2; j++)
	{
		key.sprintf(L"EvKind[%d]", j);
		ptnFile->WriteInteger(sec, key, ptn.Seg[j].EvKind[0]);
		key.sprintf(L"OnType[%d]", j);
		ptnFile->WriteInteger(sec, key, ptn.Seg[j].OnType[0]);
		key.sprintf(L"OffPe[%d]", j);
		ptnFile->WriteInteger(sec, key, ptn.Seg[j].OffPe[0]);
	}

	return true;
}

bool IsValidIPAddr(const String ip)
{
	std::shared_ptr<TStringList> slIp (new TStringList());
    slIp->Delimiter = '.';
	slIp->DelimitedText = ip;

	if( slIp->Count != 4 )
		return false;

	for(int i=0; i<slIp->Count; i++)
    {
        int nVal = slIp->Strings[i].ToInt();
		if( nVal > 255 || nVal < 0 )
			return false;
	}

	return true;
}

void GetIpAddressBytesIPv4(const String sIp, BYTE *byAddr)
{
	StringListPtr slIp (new TStringList() );
	slIp->Delimiter = '.';
	slIp->DelimitedText = sIp;

	if(slIp->Count != 4)
	{
		ShowMessage("Wrong Ip Address");
		return;
	}

	for(int i = 0; i < 4; ++i)
	{
		byAddr[i] = (BYTE)(slIp->Strings[i].ToIntDef(0) );
	}
}

String GetIpAddressStr(const DWORD dwAddress)
{
	String sRet;

	BYTE IPPart1 = FIRST_IPADDRESS((LPARAM)dwAddress);
	BYTE IPPart2 = SECOND_IPADDRESS((LPARAM)dwAddress);
	BYTE IPPart3 = THIRD_IPADDRESS((LPARAM)dwAddress);
	BYTE IPPart4 = FOURTH_IPADDRESS((LPARAM)dwAddress);

	sRet.sprintf(L"%d.%d.%d.%d", (int)IPPart1, (int)IPPart2, (int)IPPart3, (int)IPPart4);

	return sRet;
}

// 20181017 - Debug Log Save - jhlim
bool SaveDebugLog(String DebugString)
{
	if(DebugString == "")
		return false;

	AnsiString DebugLogPath, logFolder;
	TDateTime TodayTime = Now();

	logFolder = GetLogFolder() + TodayTime.FormatString("yyyy-mm-dd") + "\\";
	if (DirectoryExists(logFolder) == false)
	{
		CreateDir(logFolder);
	}

	DebugLogPath = logFolder + "Debug_" + TodayTime.FormatString("yyyy_mm_dd_hh") + ".txt";

	AnsiString DebugLog;
	DebugLog = (AnsiString)TodayTime.FormatString("yyyy:mm:dd:hh:nn:ss.zzz") + "->" + (AnsiString)DebugString + "\r\n";

	FILE * pFile;
	pFile = fopen(DebugLogPath.c_str(), "a+t");
	if( pFile == nullptr )
		return false;
	std::fwrite(DebugLog.c_str(), DebugLog.Length(), 1, pFile);
	fclose(pFile);

	return true;
}

// 20181116 - Communication Log Save - jhlim
bool SaveCommLog(String CommgString)
{
	if(CommgString == "")
		return false;

	AnsiString CommLogPath;
	TDateTime TodayTime = Now();
	CommLogPath = GetLogFolder() + "Comm_" + TodayTime.FormatString("yyyy_mm_dd_hh") + ".txt";

	AnsiString CommLog;
	CommLog = (AnsiString)TodayTime.FormatString("yyyy:mm:dd:hh:nn:ss") + "->" + (AnsiString)CommgString + "\r\n";

	FILE * pFile;
	pFile = fopen(CommLogPath.c_str(), "a+t");
	if( pFile == nullptr )
		return false;
	std::fwrite(CommLog.c_str(), CommLog.Length(), 1, pFile);
	fclose(pFile);

	return true;
}

// 20181207 - Serial Log - jhlim
bool SaveSerialLog(AnsiString devName, String SerialString, bool bSend)
{
	if(SerialString == "")
		return false;

	AnsiString SerailLogPath, logFolder;
	TDateTime TodayTime = Now();

	logFolder = GetLogFolder() + TodayTime.FormatString("yyyy-mm-dd") + "\\";
	if (DirectoryExists(logFolder) == false)
	{
		CreateDir(logFolder);
	}

	AnsiString strController;
	strController = "[" + devName + "]";

	AnsiString strSend;
	if(bSend == true)
		strSend = "[SEND]->";
	else
		strSend = "[RECV]->";

	SerailLogPath = logFolder + "Serial_" + strController + "_" + TodayTime.FormatString("yyyy_mm_dd_hh") + ".txt";

	AnsiString SerialLog;
	SerialLog = (AnsiString)TodayTime.FormatString("yyyy:mm:dd:hh:nn:ss.zzz") + " " + strController + strSend + (AnsiString)SerialString + "\r\n";

	FILE * pFile;
	pFile = fopen(SerailLogPath.c_str(), "a+t");
	if( pFile == nullptr )
		return false;
	std::fwrite(SerialLog.c_str(), SerialLog.Length(), 1, pFile);
	fclose(pFile);

	return true;
}

// 20190304 - Delete Serial Log - jhlim
void DeleteSerialLog(int nDeleteDay)
{
	if(nDeleteDay <= 0)
		return;

	int nPos = 0;
	AnsiString strFileName = "";
	AnsiString strFoler = GetLogFolder();
	AnsiString path = "";

	TDateTime TodayTime = Now();
	TodayTime = TodayTime - (nDeleteDay + 1);
	ReplaceTime(TodayTime, EncodeTime(0, 0, 0, 0));

	strFileName.sprintf("Serial_%s.txt", TodayTime.FormatString("yyyy_mm_dd_hh").c_str());

	TSearchRec sr;
	if( FindFirst(strFoler + "*.*", faArchive, sr) == 0)
	{
		do
		{
			nPos = AnsiPos("Serial_", sr.Name.c_str());
			if( nPos > 0 )
			{
				path.sprintf("%s%S", strFoler.c_str(), sr.Name.c_str());

				if( CompareStr(strFileName, sr.Name.c_str()) > 0 )
				{
				DeleteFile(path);
				}
			}
		} while( FindNext(sr) == 0 );
	}
	FindClose(sr);
}

// 20181212 - Gem Log - jhlim
bool SaveGemLog(String GemString)
{
	if(GemString == "")
		return false;

	AnsiString GemLogPath;
	TDateTime TodayTime = Now();
	GemLogPath = GetLogFolder() + "Gem_" + TodayTime.FormatString("yyyy_mm_dd_hh") + ".txt";

	AnsiString GemLog;
	GemLog = (AnsiString)TodayTime.FormatString("yyyy:mm:dd:hh:nn:ss") + "->" + (AnsiString)GemString + "\r\n";

	FILE * pFile;
	pFile = fopen(GemLogPath.c_str(), "a+t");
	if( pFile == nullptr )
		return false;
	std::fwrite(GemLog.c_str(), GemLog.Length(), 1, pFile);
	fclose(pFile);

	return true;
}

// 20191002 - Delete Log - jhlim
void DeleteLog(int nDeleteDay)
{
	if(nDeleteDay <= 0)
		return;

	int nPos = 0;
	AnsiString strFileName = "";
	AnsiString strFoler = GetLogFolder();
	AnsiString strCompare = "";
	AnsiString strCompare1 = "";
	AnsiString path = "";

	TDateTime TodayTime = Now();
	TodayTime = TodayTime - (nDeleteDay + 1);
	ReplaceTime(TodayTime, EncodeTime(0, 0, 0, 0));

	strFileName.sprintf("%s.txt", TodayTime.FormatString("yyyy_mm_dd_hh").c_str());

	TSearchRec sr;
	if( FindFirst(strFoler + "*.*", faArchive, sr) == 0)
	{
		do
		{
			// 날짜와 파일확장자명 비교하여 로그 삭제
			strCompare = sr.Name.c_str();
			strCompare1 = strCompare.SubString(strCompare.Length() - (strFileName.Length() - 1), strCompare.Length());
			if( CompareStr(strFileName, strCompare1) > 0 )
			{
				path.sprintf("%s%S", strFoler.c_str(), sr.Name.c_str());
				DeleteFile(path);
			}
		} while( FindNext(sr) == 0 );
	}
	FindClose(sr);
}

////////////////////////////////////////////////////////////2021-04-15HANA OVEN
bool SaveHanaOvenPtn(int chamberNo, PTN ptn, bool remote/*=false*/)
{
	String strFolder, strFile, recipeName;
	strFolder.sprintf(L"%s%s\\",
					  GetRootFolder().c_str(),
					  L"REMOTE_PATTERN");
	if( remote )
		strFile.sprintf(L"%sremote.ptn", strFolder.c_str());
	else
		strFile.sprintf(L"%s%d.ptn", strFolder.c_str(), ptn.PtnNo);

	if( FileExists(strFile) == false )
	{
		try
		{
			int hFile = FileCreate(strFile);
			FileClose(hFile);
		}
		catch(...)
		{
			return false;
		}
	}

	String sec = "PATTERN_T";
	String key, str;
	std::shared_ptr<TIniFile> ptnFile(new TIniFile(strFile));

	ptnFile->WriteInteger(sec, "SegCount", ptn.Stc.SegCount);
	str.sprintf(L"%.1f", ptn.Stc.Ssp);
	ptnFile->WriteString(sec, "Ssp", str);
	ptnFile->WriteInteger(sec, "Stc", ptn.Stc.Stc);
	ptnFile->WriteInteger(sec, "Rcy", ptn.Stc.Rcy);
	ptnFile->WriteInteger(sec, "Rst", ptn.Stc.Rst);
	ptnFile->WriteInteger(sec, "Ren", ptn.Stc.Ren);

	ptnFile->WriteInteger(sec, "VacSP", ptn.VacSP);
	ptnFile->WriteInteger(sec, "VacHighLimit", ptn.VacHighLimit);
	ptnFile->WriteInteger(sec, "CuringVacuumLevel", ptn.CuringVacuumLevel);
	ptnFile->WriteInteger(sec, "CoolingVacuumLevel", ptn.CoolingVacuumLevel);
	ptnFile->WriteInteger(sec, "VacLowLimit", ptn.VacLowLimit);
	ptnFile->WriteBool(sec, "UseN2Purge", ptn.UseN2Purge);

	ptnFile->WriteInteger(sec, "CureStartOpt", ptn.CureStartOption);
	ptnFile->WriteInteger(sec, "O2AnalyzerPower", ptn.O2AnalyzerPowerOff);

    ptnFile->WriteBool(sec, "UseLocalMode", ptn.UseLocalMode);
    ptnFile->WriteInteger(sec, "LocalTemp", ptn.LocalTemp);

	for(int i=0; i<5; i++)
	{
		key.sprintf(L"Wz[%d]", i);
		ptnFile->WriteInteger(sec, key, ptn.Stc.Wz[i]);
		key.sprintf(L"WTm[%d]", i);
		ptnFile->WriteInteger(sec, key, ptn.Stc.WTm[i]);
	}

	for(int i=0; i<ptn.Stc.SegCount; i++)
	{
		key.sprintf(L"Tsp[%d]", i);
		ptnFile->WriteInteger(sec, key, ptn.Seg[i].Tsp);
		key.sprintf(L"Time[%d]", i);
		ptnFile->WriteInteger(sec, key, ptn.Seg[i].Time);
		key.sprintf(L"TmRt[%d]", i);
		ptnFile->WriteInteger(sec, key, ptn.Seg[i].TmRt);
		key.sprintf(L"Pid[%d]", i);
		ptnFile->WriteInteger(sec, key, ptn.Seg[i].Pid);

		if (i == ptn.Stc.SegCount-1)
		{
			ptn.Seg[i].Jc = ptn.UseLocalMode ? 2 : 0;
		}
		key.sprintf(L"Jc[%d]", i);
		ptnFile->WriteInteger(sec, key, ptn.Seg[i].Jc);

		for(int j=0; j<8; j++)
		{
			key.sprintf(L"Seg[%d].EvKind[%d]", i, j);
			ptnFile->WriteInteger(sec, key, ptn.Seg[i].EvKind[j]);
			key.sprintf(L"Seg[%d].OnType[%d]", i, j);
			ptnFile->WriteInteger(sec, key, ptn.Seg[i].OnType[j]);
			key.sprintf(L"Seg[%d].OffPe[%d]", i, j);
			ptnFile->WriteInteger(sec, key, ptn.Seg[i].OffPe[j]);
		}

		key.sprintf(L"DamperIn[%d]", i);
		ptnFile->WriteInteger(sec, key, ptn.Damper[i].InPercent);
		key.sprintf(L"DamperOut[%d]", i);
		ptnFile->WriteInteger(sec, key, ptn.Damper[i].OutPercent);

		key.sprintf(L"InverterFreq[%d]", i);
		ptnFile->WriteFloat(sec, key, ptn.InverterFrequency[i]);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////

String GetEventName(int eventKind)
{
	String name;
	for (int i = 0; i < MAX_EVENT_COUNT; i++)
	{
		if (EventList[i].EventNo == eventKind)
		{
			name = EventList[i].Name;
			break;
		}
	}
	return name;
}

bool HasPVHighEvent(PTN ptn, int segNo, int &Index)
{
	int segCount = GetSegCount(ptn);
	if (segNo < 1 || segNo > segCount)
	{
		return false;
	}

	int segIdx = segNo - 1;
	for (int i = 0; i < EVENT_COUNT; i++)
	{
		if (ptn.Seg[segIdx].EvKind[i] == ET_PVHIGH)
		{
			Index = i;
			return true;
		}
	}
	return false;
}

bool HasPVHighClear(PTN ptn, int segNo, int &Index)
{
	int segCount = GetSegCount(ptn);
	if (segNo < 1 || segNo > segCount)
	{
		return false;
	}

	int segIdx = segNo - 1;
	for (int i = 0; i < EVENT_COUNT; i++)
	{
		if (ptn.Seg[segIdx].EvKind[i] == ET_PVHIGH_CLEAR)
		{
			Index = i;
			return true;
		}
	}
	return false;
}

bool HasPVLowEvent(PTN ptn, int segNo, int &Index)
{
	int segCount = GetSegCount(ptn);
	if (segNo < 1 || segNo > segCount)
	{
		return false;
	}

	int segIdx = segNo - 1;
    for (int i = 0; i < EVENT_COUNT; i++)
	{
		if (ptn.Seg[segIdx].EvKind[i] == ET_PVLOW)
		{
			Index = i;
			return true;
		}
	}
	return false;
}

bool HasPVLowClear(PTN ptn, int segNo, int &Index)
{
	int segCount = GetSegCount(ptn);
	if (segNo < 1 || segNo > segCount)
	{
		return false;
	}

	int segIdx = segNo - 1;
	for (int i = 0; i < EVENT_COUNT; i++)
	{
		if (ptn.Seg[segIdx].EvKind[i] == ET_PVLOW_CLEAR)
		{
			Index = i;
			return true;
		}
	}
	return false;
}

double GetPVHighOffset(PTN ptn, int segNo)
{
	int Index;

	if (HasPVHighEvent(ptn, segNo, Index) == false)
	{
		return 0.0;
	}

	int segIdx = segNo - 1;
	for (int i = 0; i < EVENT_COUNT; i++)
	{
		if (ptn.Seg[segIdx].EvKind[i] == ET_PVHIGH)
		{
			return ptn.Seg[segIdx].OffPe[i];
		}
	}
	return 0.0;
}

double GetPVLowOffset(PTN ptn, int segNo)
{
	int Index;

	if (HasPVLowEvent(ptn, segNo, Index) == false)
	{
		return 0.0;
	}

	int segIdx = segNo - 1;
	for (int i = 0; i < EVENT_COUNT; i++)
	{
		if (ptn.Seg[segIdx].EvKind[i] == ET_PVLOW)
		{
			return ptn.Seg[segIdx].OffPe[i];
		}
	}
	return 0.0;
}

int GetPVHighOnType(PTN ptn, int segNo)
{
	int Index;

	if (HasPVHighEvent(ptn, segNo, Index) == false)
	{
		return 0;
	}

	int segIdx = segNo - 1;
	for (int i = 0; i < EVENT_COUNT; i++)
	{
		if (ptn.Seg[segIdx].EvKind[i] == ET_PVHIGH)
		{
			return ptn.Seg[segIdx].OnType[i];
		}
	}
	return 0;
}

int GetPVLowOnType(PTN ptn, int segNo)
{
	int Index;

	if (HasPVLowEvent(ptn, segNo, Index) == false)
	{
		return 0;
	}

	int segIdx = segNo - 1;
	for (int i = 0; i < EVENT_COUNT; i++)
	{
		if (ptn.Seg[segIdx].EvKind[i] == ET_PVLOW)
		{
			return ptn.Seg[segIdx].OnType[i];
		}
	}
	return 0;
}

bool RunGEMProgram(String programName, int chamberNo)
{
	String gemCaption;
	HINSTANCE hinst = nullptr;
	HWND hwnd = nullptr;

	gemCaption.sprintf(L"GEM [Chamber %c]", 'A' + chamberNo);
	hwnd = FindWindow(nullptr, gemCaption.c_str());
	if (hwnd == nullptr)
	{
		String chamber = IntToStr(chamberNo);
		hinst = ShellExecute(nullptr, nullptr, programName.w_str(), chamber.w_str(), nullptr, SW_HIDE);
	}
	else
	{
		hinst = (HINSTANCE)32;
    }

	return ((int)hinst > 31);
}

bool KillGEMProgram(String programName)
{
	HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	DWORD error = GetLastError();
	HANDLE handle;
	DWORD exitCode = 0;

	PROCESSENTRY32W entry = { 0 };
	entry.dwSize = sizeof(entry);
	Process32First(snapShot, &entry);

	while (true)
	{
		String tmp = entry.szExeFile;
		if (tmp == programName)
		{
			handle = OpenProcess(PROCESS_ALL_ACCESS, 0, entry.th32ProcessID);
			if (GetExitCodeProcess(handle, &exitCode))
			{
				TerminateProcess(handle, exitCode);
			}
		}
		if (Process32Next(snapShot, &entry) == FALSE)
		{
			break;
		}
	}
	return false;
}

TFormatSettings GetDateTimeFormat()
{
	TFormatSettings DefaultFormat;
	DefaultFormat.ShortDateFormat=L"YYYY-MM-DD";
	DefaultFormat.LongDateFormat=L"YYYY-MM-DD";
	DefaultFormat.ShortTimeFormat=L"hh:nn:ss";
	DefaultFormat.LongTimeFormat=L"hh:nn:ss";
	DefaultFormat.DateSeparator='-';
	DefaultFormat.TimeSeparator = ':';

	return DefaultFormat;
}
