//---------------------------------------------------------------------------

#ifndef HelperFunctionsH
#define HelperFunctionsH
//---------------------------------------------------------------------------

#include <StdCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include "Defines.h"
#include <memory>

#define SWAP(a, b) { (a) ^= (b) ^= (a) ^= (b); }

String GetProgramTitle();
bool double_compare(double a, double b);
void SetMessageStr(String str);
void ShowMessageDlg();
void ShowMessageDlg(String caption);
bool ShowQuestionDlg();
bool ShowQuestionDlg(String caption);

bool RunGEMProgram(String programName, int chamberNo);
bool KillGEMProgram(String programName);

bool IsValidFloat(AnsiString fstr);
bool IsValidInt(AnsiString istr);
bool IsValidHex(AnsiString hex);
int HexToInt (AnsiString hex);
AnsiString MakeCheckSum(AnsiString strData);
int CheckSum(AnsiString strData);
unsigned int SetBit(unsigned int x, int n, bool on);
int GetAbit(unsigned short x, int n);

Graphics::TBitmap* CaptureControl(TControl *ctrl);
bool GetSerialList(TStringList *List);

void WaitSecond(double sec);
String GetInformation(String AFilename, UnicodeString AQuery);

String GetRootFolder();
String GetDataFolder();
String GetLogFolder();
String GetReportFolder();
String GetResFolder();
String GetRemotePatternFolder();
String GetFTPDataFolder();
// 20191216 - SCK MES - jhlim
String GetSCKMESWorkFileFolder();
String GetSCKMESWorkDoneFileFolder();

String GetProductFolder();

bool IsValidIPAddr(const String sIpVal);
bool IsGemRecipeExist(const String recipe);
bool DeleteAllFile(String folder);

typedef std::shared_ptr<TStringList> StringListPtr;
extern StringListPtr LanguageSectionList;

void LoadLanguageStr();
void DestroyLanguageStr();
String GetResStr(const String sSection, const String sKey);
String GetIOName(const String sKey);

int  GetSegCount(PTN ptn) { return ptn.Stc.SegCount; }
bool LoadTempPtn(int chamberNo, int ptnNo, PTN &ptn, bool remote=false);
bool LoadRemoteTempPtn(String recipe, PTN &ptn);
bool SaveTempPtn(int chamberNo, PTN ptn, bool remote=false);
bool LoadPressPtn(int chamberNo, int ptnNo, PTN &ptn, bool remote=false);
bool SavePressPtn(int chamberNo, PTN ptn, bool remote=false);
bool SaveHanaOvenPtn(int chamberNo, PTN ptn, bool remote=false); //2021-04-15 HANA OVEN
bool HasPVHighEvent(PTN ptn, int segNo, int &Index);
bool HasPVLowEvent(PTN ptn, int segNo, int &Index);
bool HasPVHighClear(PTN ptn, int segNo, int &Index);
bool HasPVLowClear(PTN ptn, int segNo, int &Index);

double GetPVHighOffset(PTN ptn, int segNo);
double GetPVLowOffset(PTN ptn, int segNo);
int GetPVHighOnType(PTN ptn, int segNo);
int GetPVLowOnType(PTN ptn, int segNo);

bool IsValidIPAddr(const String ip);
void GetIpAddressBytesIPv4(const String sIp, BYTE *byAddr);
String GetIpAddressStr(const DWORD dwAddress);

// 20181017 - Debug Log Save - jhlim
bool SaveDebugLog(String DebugString);
// 20181116 - Communication Log Save - jhlim
bool SaveCommLog(String CommgString);
// 20181207 - Serial Log - jhlim
bool SaveSerialLog(AnsiString devName, String SerialString, bool bSend);
void DeleteSerialLog(int nDeleteDay);
// 20181212 - Gem Log - jhlim
bool SaveGemLog(String GemString);
// 20191002 - Delete Log - jhlim
void DeleteLog(int nDeleteDay);

String GetEventName(int eventKind);

TFormatSettings GetDateTimeFormat();

#endif
