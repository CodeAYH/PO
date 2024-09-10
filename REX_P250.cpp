//---------------------------------------------------------------------------
#include <System.AnsiStrings.hpp>
#include <System.DateUtils.hpp>

#pragma hdrstop

#include "REX_P250.h"
#include "HelperFunctions.h"
#include "SerialPortManager.h"
#include "Chamber.h"
#include "JobForm.h"
#include "NormalOvenChamber.h"
#include "NormalOvenSystemManager.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

bool IsValidInt(const AnsiString data)
{
	if( data == "" )
		return false;

	for(int i=1; i<=data.Length(); i++)
	{
		char c = data[i];

		if( !(c >= '0' && c <= '9') )
			return false;
	}
	return true;
}

bool IsValidTempData(const AnsiString data)
{
	if( data == "" )
		return false;

	int len = data.Length();
	if (len != 6)
	{
		return false;
	}

	for(int i=1; i<=len; i++)
	{
		char c = data[i];

		if( !(c >= '0' && c <= '9') )
			return false;
	}
	return true;
}

bool IsValidTimeData(const AnsiString data)
{
	if( data == "" )
		return false;

	int len = data.Length();
	if (len != 8)
	{
		return false;
	}

	if (data[3] != '.' || data[6] != '.')
	{
		return false;
	}

	AnsiString str = StringReplace(data, ".", "", TReplaceFlags() << rfReplaceAll);

	for(int i=1; i <= str.Length(); i++)
	{
		char c = str[i];

		if( !(c >= '0' && c <= '9') )
			return false;
	}
	return true;
}

REX_P250::REX_P250(BaseChamber *chamber, PORTINFO portInfo)
	: SerialDevice(chamber, portInfo)
{
	ChamberNo = chamber->ChamberID;
	RxBuffer = "";
	RxTempBuffer = "";
	RxData = "";
	RxTempData = "";
	SegCount = 0;
	FSelectingCmd = false;
	CureStrtCmd = false;
	AckCount = 0;
	CommErrBaseTime = Now();

	String iniPath = GetDataFolder() + REX_P250INI;
	boost::shared_ptr<TIniFile> Test(new TIniFile(iniPath));
	const String sec = "TEST";

	unsigned char bcc;
	AnsiString cmd, pre, str, hex, tmp;

	pre.sprintf("%c00%c", EOT, STX);
	cmd.sprintf("PN01PE03%c", ETX);
	bcc = CalcBCC(cmd);
	str.sprintf("%s%s%c", pre.c_str(), cmd.c_str(), bcc);
	hex = "";
	for(int i=1; i<=str.Length(); i++)
	{
		tmp.sprintf("%02X", str[i]);
		hex += tmp;
	}
	if (Test->ValueExists(sec, cmd))
		Test->DeleteKey(sec, cmd);
	Test->WriteString(sec, cmd, hex);

	cmd.sprintf("PN01SN01LE00175%c", ETX);
	bcc = CalcBCC(cmd);
	str.sprintf("%s%s%c", pre.c_str(), cmd.c_str(), bcc);
	hex = "";
	for(int i=1; i<=str.Length(); i++)
	{
		tmp.sprintf("%02X", str[i]);
		hex += tmp;
	}
	if (Test->ValueExists(sec, cmd))
		Test->DeleteKey(sec, cmd);
	Test->WriteString(sec, cmd, hex);

	cmd.sprintf("PN01SN01TM00.30.00%c", ETX);
	bcc = CalcBCC(cmd);
	str.sprintf("%s%s%c", pre.c_str(), cmd.c_str(), bcc);
	hex = "";
	for(int i=1; i<=str.Length(); i++)
	{
		tmp.sprintf("%02X", str[i]);
		hex += tmp;
	}
	if (Test->ValueExists(sec, cmd))
		Test->DeleteKey(sec, cmd);
	Test->WriteString(sec, cmd, hex);

	cmd.sprintf("PN01SN02LE00175%c", ETX);
	bcc = CalcBCC(cmd);
	str.sprintf("%s%s%c", pre.c_str(), cmd.c_str(), bcc);
	hex = "";
	for(int i=1; i<=str.Length(); i++)
	{
		tmp.sprintf("%02X", str[i]);
		hex += tmp;
	}
	if (Test->ValueExists(sec, cmd))
		Test->DeleteKey(sec, cmd);
	Test->WriteString(sec, cmd, hex);

	cmd.sprintf("PN01SN02TM00.30.00%c", ETX);
	bcc = CalcBCC(cmd);
	str.sprintf("%s%s%c", pre.c_str(), cmd.c_str(), bcc);
	hex = "";
	for(int i=1; i<=str.Length(); i++)
	{
		tmp.sprintf("%02X", str[i]);
		hex += tmp;
	}
	if (Test->ValueExists(sec, cmd))
		Test->DeleteKey(sec, cmd);
	Test->WriteString(sec, cmd, hex);

	cmd.sprintf("PN01SN03LE00000%c", ETX);
	bcc = CalcBCC(cmd);
	str.sprintf("%s%s%c", pre.c_str(), cmd.c_str(), bcc);
	hex = "";
	for(int i=1; i<=str.Length(); i++)
	{
		tmp.sprintf("%02X", str[i]);
		hex += tmp;
	}
	if (Test->ValueExists(sec, cmd))
		Test->DeleteKey(sec, cmd);
	Test->WriteString(sec, cmd, hex);

	cmd.sprintf("PN01SN03TM00.30.00%c", ETX);
	bcc = CalcBCC(cmd);
	str.sprintf("%s%s%c", pre.c_str(), cmd.c_str(), bcc);
	hex = "";
	for(int i=1; i<=str.Length(); i++)
	{
		tmp.sprintf("%02X", str[i]);
		hex += tmp;
	}
	if (Test->ValueExists(sec, cmd))
		Test->DeleteKey(sec, cmd);
	Test->WriteString(sec, cmd, hex);

	cmd.sprintf("XM01%c", ETX);
	bcc = CalcBCC(cmd);
	str.sprintf("%s%s%c", pre.c_str(), cmd.c_str(), bcc);
	hex = "";
	for(int i=1; i<=str.Length(); i++)
	{
		tmp.sprintf("%02X", str[i]);
		hex += tmp;
	}
	if (Test->ValueExists(sec, cmd))
		Test->DeleteKey(sec, cmd);
	Test->WriteString(sec, cmd, hex);

	cmd.sprintf("XM00%c", ETX);
	bcc = CalcBCC(cmd);
	str.sprintf("%s%s%c", pre.c_str(), cmd.c_str(), bcc);
	hex = "";
	for(int i=1; i<=str.Length(); i++)
	{
		tmp.sprintf("%02X", str[i]);
		hex += tmp;
	}
	if (Test->ValueExists(sec, cmd))
		Test->DeleteKey(sec, cmd);
	Test->WriteString(sec, cmd, hex);
}

__fastcall REX_P250::~REX_P250()
{

}

void REX_P250::DoCommunicate()
{
	if (!IsRxDone())
	{
	   return;
	}

	if (IsMyCommand() == false)
	{
		return;
	}

	if (IsPollingCommand())
	{
		Application->ProcessMessages();
		DoPollingSequence();
		return;
	}

	if (IsSelectingCommand())
	{
       	Application->ProcessMessages();
		if (IsSelectingParsing()== false)
		{
			InitRxValues();
			return;
		}
		return;
	}
}

bool REX_P250::IsRxDone()
{
	if( RxBuffer.IsEmpty() )
	{
		if (MilliSecondsBetween(CommErrBaseTime, Now()) >= 2000)
		{
			FCommFail = true;
        }
		return false;
	}

	try
	{
		if (IsPollingCommand() == false && IsSelectingCommand() == false)
		{
			return false;
		}
		else
		{
			CommErrBaseTime = Now();
			FCommFail = false;
			return true;
		}
	}
	catch(...)
	{
		return false;
	}

	return false;
}

bool REX_P250::IsPollingCommand()
{
	int len = RxBuffer.Length();
	if (len <= 0)
	{
		return false;
	}

	if (AnsiContainsStr(RxBuffer, AnsiString(EOT)) && AnsiContainsStr(RxBuffer, AnsiString(ENQ)))
	{
		int idx = RxBuffer.AnsiPos(EOT);
		int idxEnq = RxBuffer.AnsiPos(ENQ);
		int nLen = RxBuffer.Length();

		if (idx != 0)
		{
			RxData = RxBuffer.SubString(idx, idxEnq);
			ValidRxData = true;

			return true;
		}
		else
		{
			RxData = RxBuffer.SubString(idx, idxEnq);
			ValidRxData = true;
			return true;
		}
	}
	return false;
}

bool REX_P250::IsSelectingCommand()
{
	int len = RxBuffer.Length();
	if (len <= 0)
	{
		return false;
	}

	if (AnsiContainsStr(RxBuffer, AnsiString(EOT)) && AnsiContainsStr(RxBuffer, AnsiString(STX))
		&& AnsiContainsStr(RxBuffer, AnsiString(ETX) ))
	{
		int idx = RxBuffer.AnsiPos(ETX);
		int idxEot = RxBuffer.AnsiPos(EOT);
		int nLen = RxBuffer.Length();

		if (nLen > idx)
		{
			String str;
			str.sprintf(L"HANA OVEN CHM[%d]: IsSelectingCommand(): Include ETX(%d), EOT(%d):", ChamberNo, idx, idxEot);
			str += RxBuffer;
			HanaSaveLog(str);

			if (idxEot != 0)
			{
				RxData = RxBuffer.SubString(idxEot, idx+1);
				ValidRxData = true;
			}
			else
			{
				RxData = RxBuffer.SubString(idxEot, idx+1);
				ValidRxData = true;
            }

			FSelectingCmd = true;

			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

bool REX_P250::IsSelectingParsing()
{
	NormalOvenChamber *chamber;
	chamber = GetChamber(ChamberNo);

	if (chamber->Step == STEP_RUNNING)
		return false;

	String str, strNo;

	str.sprintf(L"HANA OVEN CHM[%d]: IsSelectingParsing(): Start:", ChamberNo);
	str += RxData;
	HanaSaveLog(str);

	int idx = RxData.AnsiPos(ETX);

	if (RxData[idx+1] == NULL)
	{
		str.sprintf(L"HANA OVEN CHM[%d]: IsSelectingParsing():Checksum[nullptr]:", ChamberNo);
		str += RxData;
		HanaSaveLog(str);
		//return false; //2022-05-16 Hana Oven doesn't exist checksum
		RxTempData = RxData.SubString(5, idx-4);
		str.sprintf(L"HANA OVEN CHM[%d]: IsSelectingParsing():Nothing Checksum[nullptr]:", ChamberNo);
		str += RxTempData;
		HanaSaveLog(str);
	}
	else
	{
		unsigned char bcc = RxData[idx+1];

		RxTempData = RxData.SubString(5, idx-4);

		str.sprintf(L"HANA OVEN CHM[%d]: IsSelectingParsing():Checksum Exist:", ChamberNo);
		str += RxTempData;
		str += bcc;
		HanaSaveLog(str);

		if (CheckBCC(RxTempData, bcc) == false)
		{
			SendNak();

			str.sprintf(L"HANA OVEN CHM[%d]: CheckBCC error:", ChamberNo);
			str += RxTempData;
			str += bcc;
			HanaSaveLog(str);

			return false;
		}
	}

	DoSelectingSequence();
	return true;
}

void REX_P250::GetMonitorCommands(TStringList *list)
{

}

void REX_P250::DecodeMonitor()
{

}

unsigned char REX_P250::CalcBCC(const AnsiString data)
{
	unsigned char bcc = data[1];
	int len = data.Length();
	for (int i = 2; i <= len; i++)
	{
		bcc = bcc ^ data[i];
	}
	return bcc;
}

bool REX_P250::CheckBCC(const AnsiString data, const unsigned char bcc)
{
	String str;
	unsigned char calcBcc = CalcBCC(data);

	str.sprintf(L"HANA OVEN CHM[%d]: CheckBCC :", ChamberNo);
	str +=  calcBcc;
	str +=  bcc;
	HanaSaveLog(str);

	return (bcc == calcBcc);
}

bool REX_P250::IsMyCommand()
{
	AnsiString str = RxData.SubString(2, 2);

	int addr = str.ToInt();


	if (addr != ChamberNo)
	{
		String str, strNo;
		strNo.sprintf(L"CHM[%d] addr[%d], Chamber[%d]", ChamberNo, addr, ChamberNo);
		str = L"HANA OVEN : IsMyCommand";
		str += strNo;
		HanaSaveLog(str);
		return false;
	}
	return true;
}

void REX_P250::SendAck()
{
	AnsiString data;
	data.sprintf("%c", ACK);

	AckCount += 1;
	TxData(data);
	TimeoutTimer->Enabled = false;
	String str;
	str.sprintf(L"HANA OVEN CHM[%d] : SendAak[%d]", ChamberNo, AckCount);
	HanaSaveLog(str);
}

void REX_P250::SendNak()
{
	AnsiString data;
	data.sprintf("%c", NAK);

	String str;
	str.sprintf(L"HANA OVEN CHM[%d] : SendNak", ChamberNo);
	HanaSaveLog(str);

	TxData(data);
	TimeoutTimer->Enabled = false;
}

AnsiString REX_P250::MakeTempDataString(const double tempData)
{
	// 온도 데이터를 7자리 아스키 숫자로 표현한다.
	// 포맷 : 00000.0
	AnsiString data;
	data.sprintf("%07.1f", tempData);
	return data;
}

AnsiString REX_P250::MakeTimeDataString(const int timeData)
{
	// 시간 데이터를 7자리 아스키 숫자로 표현한다.
	// timeData는 분 단위 시간 데이터이다.
	// 포맷 : 00.00.00 (시간.분.초)
	AnsiString data;
	int hour = timeData / 60;
	int min = timeData % 60;

	data.sprintf("%02d.%02d.00", hour, min);
	return data;
}

AnsiString REX_P250::MakeIntDataString(const int intData)
{
	// int 데이터를 7자리 아스키 숫자로 표현한다.
	// 포맷 : 0000000
	AnsiString data;

	data.sprintf("%06d", intData);
	return data;
}

void REX_P250::DoPollingSequence()
{
	AnsiString cmd = RxData.SubString(4, 2);
	AnsiString data;

	data += cmd;
	if (cmd == M1)	// Measured Value(PV)
	{
		data += MakeTempDataString(FHanaData.PV);
	}
	else if (cmd == TR)	// Segment Reamining Time
	{
	   //data += MakeTimeDataString(ReportData.SegmentRemainingTime);
		data += FHanaData.SegmentRemainingTime;
	}
	else if (cmd == HO)	// Hold State
	{
		data += MakeIntDataString(FHanaData.HoldState);
	}
	else if (cmd == EN)	// Pattern End State
	{
		data += MakeIntDataString(FHanaData.EndState);
	}
	else if (cmd == WT)	// Wait State Monitor
	{
		data += MakeIntDataString(FHanaData.WaitState);
	}
	else if (cmd == XM)	// Operation Mode transfer
	{
		data += MakeIntDataString(FHanaData.OperationMode);
	}
	else if (cmd == PS)	// Execution pattern selection
	{
		data += MakeIntDataString(FHanaData.CurrentPatternNo);
	}
	else if (cmd == SN)	// Segment Number
	{
		data += MakeIntDataString(FHanaData.CurrentSegNo);
	}
	else if (cmd == RA)	// 참조하는 프로토콜 문서에 없는 내용
	{
		// Local/Computer 모드라는데 확인되지 않음. Computer 모드 (= 1)라 가정하고 진행
		data += MakeIntDataString(1);
	}
	else if (cmd == S1)	// Set value (SV) monitor
	{
		data += MakeTempDataString(FHanaData.SV);
	}
	else if (cmd == B1)	// Burnout state monitor
	{
		data += MakeIntDataString(FHanaData.BurnoutState);
	}

	AnsiString str, bcc;

	str.sprintf("%c", ETX);
	data += str;
	str.sprintf("%c", CalcBCC(data));
	data += str;
	str.sprintf("%c%c", ENQ, STX);
	str += data;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	TxData(str);
	TimeoutTimer->Enabled = false;
}

void REX_P250::DoSelectingSequence()
{
	String str;
	bool error = false; //true

	str.sprintf(L"HANA OVEN CHM[%d]: DoSelectingSequence(): Start:", ChamberNo);
	str += RxTempData;
	HanaSaveLog(str);

	if ((AnsiContainsStr(RxTempData, PN)) && (AnsiContainsStr(RxTempData, PE)))
	{
		// PN01PE03 ==> 패턴 1번, 세그먼트 3개
		if (RxTempData.Length() == 9)
		{
			str.sprintf(L"HANA OVEN CHM[%d]: Recipe[PN/PE]:", ChamberNo);
			str += RxTempData;
			HanaSaveLog(str);
			CureStrtCmd = false;

			AnsiString strPtnNo, strSegCount;
			strPtnNo = RxTempData.SubString(3, 2);
			strSegCount = RxTempData.SubString(7, 2);

			if (strSegCount == "01")
			{
				str.sprintf(L"HANA OVEN CHM[%d]: Recipe[PE] = 1:", ChamberNo);
				str += RxTempData;
				HanaSaveLog(str);
			}

			FHanaData.OperationMode = 0;
			if (IsValidInt(strPtnNo) && IsValidInt(strSegCount))
			{
				SettingData.PatternNo = strPtnNo.ToInt();
				TempPtn.PtnNo = SettingData.PatternNo;
				TempPtn.Stc.SegCount = strSegCount.ToInt();
				error = false;

				str.sprintf(L"HANA OVEN CHM[%d]: IsValidInt", ChamberNo);
				HanaSaveLog(str);
			}
		}
		else
		{
			return;
		}
	}
	else if ((AnsiContainsStr(RxTempData, PN)) && (AnsiContainsStr(RxTempData, SN) && AnsiContainsStr(RxTempData, LE)))
	{
		// PN01SN01LE00125 ==> 패턴 1번, 세그먼트 1번, 온도 125도
		if (RxTempData.Length() == 17)
		{
			str.sprintf(L"HANA OVEN CHM[%d]: Recipe[PN/SN/LE]:", ChamberNo);
			str += RxTempData;
			HanaSaveLog(str);

			AnsiString strPtnNo, strSegNo, strTemp;
			strPtnNo = RxTempData.SubString(3, 2);
			strSegNo = RxTempData.SubString(7, 2);
			strTemp = RxTempData.SubString(11, 6);
			if (strTemp == "000000")
			{
				str.sprintf(L"HANA OVEN CHM[%d]: Recipe[LE] = 0:", ChamberNo);
				str += RxTempData;
				HanaSaveLog(str);
			}
			if (IsValidInt(strPtnNo) && IsValidInt(strSegNo)) //&& IsValidTempData(strTemp))
			{
				if (IsValidTempData(strTemp))
				{
					int segNo = strSegNo.ToInt();
					SettingData.PatternNo = strPtnNo.ToInt();
					TempPtn.PtnNo = SettingData.PatternNo;

					SettingData.Seg[segNo-1].SV = strTemp.ToInt();
					TempPtn.Seg[segNo-1].Tsp = SettingData.Seg[segNo-1].SV;

					SettingData.Seg[segNo-1].SVReceived = true;
					if (SettingData.Seg[segNo-1].TimeReceived == true)
					{
						SettingData.ReceivedSegCount++;
					}
					error = false;
				}
			 }
		  }
		  else
		  {
			return;
		  }
	}
	else if ((AnsiContainsStr(RxTempData, PN)) && (AnsiContainsStr(RxTempData, SN) && AnsiContainsStr(RxTempData, TM)))
	{
		// PN01SN01TM00.30.00 ==> 패턴 1번, 세그먼트 1번, 시간 30분
		if (RxTempData.Length() == 19)
		{
			str.sprintf(L"HANA OVEN CHM[%d]: Recipe[PN/SN/TM]:", ChamberNo);
			str += RxTempData;
			HanaSaveLog(str);

			AnsiString strPtnNo, strSegNo, strTime;
			strPtnNo = RxTempData.SubString(3, 2);
			strSegNo = RxTempData.SubString(7, 2);
			strTime = RxTempData.SubString(11, 8);
			if (strTime == "00.00.00") {
				str.sprintf(L"HANA OVEN CHM[%d]: Recipe[TM] = 0:", ChamberNo);
				str += RxTempData;
				HanaSaveLog(str);
			}

			if (IsValidInt(strPtnNo) && IsValidInt(strSegNo) && IsValidTimeData(strTime))
			{
				AnsiString str;
				int segNo = strSegNo.ToInt();
				SettingData.PatternNo = strPtnNo.ToInt();
				TempPtn.PtnNo = SettingData.PatternNo;

				AnsiString strHour, strMin;
				int hour, min;

				str = RxTempData.SubString(11, 8);

				if (IsValidTimeData(str))
				{

					strHour = str.SubString(1, 2);
					hour = strHour.ToInt();
					strMin = str.SubString(4, 2);
					min = strMin.ToInt();
					SettingData.Seg[segNo-1].Time = hour * 60 + min;
					TempPtn.Seg[segNo-1].Time = SettingData.Seg[segNo-1].Time;
					//TempPtn.TotalTime += TempPtn.Seg[segNo-1].Time;
					SettingData.Seg[segNo-1].TimeReceived = true;
					if (SettingData.Seg[segNo-1].SVReceived == true)
					{
						SettingData.ReceivedSegCount++;
					}

						error = false;
				}
			}
			if (TempPtn.Stc.SegCount == strSegNo)
			{
				UpdatePtnTimeTemp();

				PostMessage(Application->MainFormHandle, WM_USER+2, (WPARAM)ChamberNo, 0);
				error = false;
			}
		}
		else
		{
			return;
		}
	}

	if (AnsiContainsStr(RxTempData, XM))
	{
		if (RxTempData.SubString(1, 4) == "XM01")
		{
			if (CureStrtCmd == false)
			{
				str.sprintf(L"HANA OVEN CHM[%d]: Recipe[XM01]:", ChamberNo);
				str += RxTempData;
				HanaSaveLog(str);
				CureStrtCmd = true;

				PostMessage(Application->MainFormHandle, WM_USER+3, (WPARAM)ChamberNo, 0);

				error = false;
				CureStrtCmd = true;
			}
		}
		else if (RxTempData.SubString(1, 4) == "XM00")
		{
			str.sprintf(L"HANA OVEN CHM[%d]: Recipe[XM00]:", ChamberNo);
			str += RxData;
			HanaSaveLog(str);

			error = false;
			CureStrtCmd = false;
			InitRxValues();
			AckCount = 0;
		}
		else
		{
			return;
		}
	}

	if (error == true)
	{
		SendNak();
		CureStrtCmd = false;

	}
	else
	{
	  	SendAck();
		CureStrtCmd = false;
	}
	RxTempData = "";
	InitRxValues();
}

bool REX_P250::GetOperationEndState(int* EndState, int* OperationMode)
{
	if (FHanaData.EndState >= 0 && FHanaData.OperationMode >= 0)
	{
		*EndState =  FHanaData.EndState;
		*OperationMode = FHanaData.OperationMode;
		return true;
	}
	return false;
}

void REX_P250::UpdatePtnTimeTemp()
{
	int segCount = TempPtn.Stc.SegCount;
	int totalTime = 0;
	double maxTemp = 0.0;
	for(int i=0; i<segCount; i++)
	{
		totalTime += TempPtn.Seg[i].Time;
		if (maxTemp < TempPtn.Seg[i].Tsp)
		{
			maxTemp = TempPtn.Seg[i].Tsp;
		}
	}
	TempPtn.TotalTime = totalTime;
	TempPtn.MaxTemp = maxTemp;
}

void REX_P250::DoHanaSegNo(int SenNo)
{
	FHanaData.CurrentSegNo =  SenNo;

}

void REX_P250::DoHanaPatternNo(int PatternNo)
{
	FHanaData.CurrentPatternNo =  PatternNo;
}

void REX_P250::DoOperationMode(int OPMode)
{
	FHanaData.OperationMode =  OPMode;

 /*	String str;
	str.sprintf(L"HANA OVEN CHM[%d] : DoOperationMode[%d]", ChamberNo,
	FHanaData.OperationMode);
	SaveDebugLog(str);  */
}

void REX_P250::DoEndState(int EndState)
{
	FHanaData.EndState =  EndState;
}

void REX_P250::DoHanaPV(double PV)
{
	FHanaData.PV =  PV;
}

void REX_P250::DoHanaSV(double SV)
{
	FHanaData.SV =  SV;
}

void REX_P250::DoHanaRemainTime(String RemainTime)
{
	FHanaData.SegmentRemainingTime =  RemainTime;
}

bool REX_P250::GetHanaReceivePtn(PTN &ReceivePtn)
{
	if (TempPtn.Stc.SegCount != 0)
	{
       	ReceivePtn = TempPtn;
		return  true;
	}
	else
	{
		return false;
	}
}

void REX_P250::HanaSaveLog(String str)
{
	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if (chamber == nullptr)
		return;

	SaveDebugLog(str);
}

