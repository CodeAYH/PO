//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SCK_RMS.h"
#include "ChamberFrame.h"
#include "NormalOvenSystemManager.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall SckRmsThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall SckRmsThread::SckRmsThread(NormalOvenChamber *chamber)
	: TThread(false)
{
}
//---------------------------------------------------------------------------
__fastcall SckRmsThread::~SckRmsThread()
{

}
//---------------------------------------------------------------------------
void __fastcall SckRmsThread::Execute()
{
	while (!Terminated)
	{
		if( CheckWorkFolder() )
		{
			if( InRange(ChamberNo, 0, GetManager()->ChamberCount-1) )
			{
				PostMessage(Application->MainFormHandle, WM_SCK_RMS_MSG, (WPARAM)ChamberNo, VALID_FILE);
			}
		}
		else
		{
			PostMessage(Application->MainFormHandle, WM_SCK_RMS_MSG, (WPARAM)ChamberNo, INVALID_FILE);
        }
		WaitForSingleObject((void*)Handle, 100);
	}
}
//---------------------------------------------------------------------------
bool __fastcall SckRmsThread::CheckWorkFolder()
{
	bool bret = false;
	String strPath, strFile1;
	strPath = GetSCKMESWorkFileFolder();

	HANDLE hDir = CreateFileW(	strPath.c_str(),
								GENERIC_READ,
								FILE_SHARE_READ|FILE_SHARE_WRITE,
								0,
								OPEN_EXISTING,
								FILE_FLAG_BACKUP_SEMANTICS,
								nullptr);
	CONST DWORD cbBuffer = 1024*1024;
	BYTE* pBuffer = (PBYTE)malloc(cbBuffer);
	BOOL bWatchSubtree = FALSE;
	DWORD dwNotifyFilter = /*FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | */
		FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE |
		FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION;
	DWORD bytesReturned;

	FILE_NOTIFY_INFORMATION* pfni;
	BOOL fOk = ReadDirectoryChangesW(hDir, pBuffer, cbBuffer,
		bWatchSubtree, dwNotifyFilter, &bytesReturned, 0, 0);
	if(!fOk)
	{
		DWORD dwLastError = GetLastError();
		ChamberNo = -1;
		bret = false;
	}

	pfni = (FILE_NOTIFY_INFORMATION*)pBuffer;

	strFile1 = pfni->FileName;

	// 2021-12-15 파일 뒤에 챔버 이름 사용
	strFile1 = strFile1.SubString(strFile1.Length()-12,9);

	if( strFile1 == "CHAMBER_A")
	{
		ChamberNo = 0;
		bret = true;
    }
	else if( strFile1 == "CHAMBER_B")
	{
		ChamberNo = 1;
		bret = true;
	}
	else
	{
		ChamberNo = -1;
		bret = false;
	}

	return bret;
}
//---------------------------------------------------------------------------
String __fastcall SckRmsThread::GetSCKMESWorkFileFolder()
{
	DWORD Drives = GetLogicalDrives();
	bool bDExist = (Drives >> 3) & 0x00000001;
	bool bDNotCD = (GetDriveType(L"D:\\") != DRIVE_CDROM);

	String folder;
	if( bDExist && bDNotCD )
		folder = "D:\\" + UpperCase(Application->Title) + "\\";
	else
		folder = GetCurrentDir() + "\\";

	folder = folder + "WORK\\";
	return folder;
}
//---------------------------------------------------------------------------
