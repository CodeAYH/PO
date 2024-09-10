//---------------------------------------------------------------------------

#ifndef I7018H
#define I7018H
//---------------------------------------------------------------------------
#include "SerialDevice.h"

class I7018 : public SerialDevice
{
enum { READPV, READCJC, READOFFSET };	// READPV : 온도 정보 읽기, READCJC : CJC 정보 읽기, READOFFSET : CJC OFFSET 정보 읽기
private:
	double FCJC;
	int FLoggerIndex;
	int FLoggerOffset; // 20181218 - DVF Mode Logger Offset - mtcho

	char RxStx;
	int DecodeMode;

	void DoMonitor();
	void DecodeMonitor();
	bool IsRxDone();
	void GetMonitorCommands(TStringList *list);

	double GetPV(int idx);

	void DoWriteCJCSetting(PARAM *param);
	void DoWriteChannelSetting(PARAM *param);
	void DoWriteCommSetting(PARAM *param);
	void DoWriteOffsetSetting(PARAM *param);
	bool SaveLoggerLog(String SerialString);

public:
	I7018(BaseChamber *chamber, PORTINFO portInfo);
	__fastcall ~I7018();

	__property double CJC = { read = FCJC };
	__property int LoggerOffset = { read = FLoggerOffset }; // 20181218 - DVF Mode Logger Offset - mtcho
	__property int LoggerIndex = { read = FLoggerIndex, write = FLoggerIndex };

	bool WriteCJCSetting();
	bool WriteChannelSetting();
	bool WriteCommSetting();
	bool WriteOffsetSetting(int LoggerOffset);
};
#endif
