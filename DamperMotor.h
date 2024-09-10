//---------------------------------------------------------------------------

#ifndef DamperMotorH
#define DamperMotorH
//---------------------------------------------------------------------------
#include "SerialDevice.h"

class DamperMotor : public SerialDevice
{
protected:
	bool FHomeInitialized;
	bool FEnabled;
	unsigned long FAlarmCode;
	int FCurrentPos;

	virtual bool IsRxDone() = 0;
	virtual void DecodeMonitor() = 0;
	virtual void GetMonitorCommands(TStringList *list) = 0;

public:
	DamperMotor(BaseChamber *chamber, PORTINFO portInfo, bool createPort = true);
	__fastcall ~DamperMotor();

	__property bool HomeInitialized = { read = FHomeInitialized, write = FHomeInitialized };
	__property bool Enabled = { read = FEnabled, write = FEnabled };
	__property unsigned long AlarmCode = { read = FAlarmCode, write = FAlarmCode };
	__property int CurrentPos = { read = FCurrentPos, write = FCurrentPos };

	virtual bool IsAlarm() = 0;
	virtual bool AlarmClear() = 0;
	virtual bool MotorEnable(bool enable=true) = 0;
	virtual bool Stop() = 0;

	virtual bool SearchHome() = 0;
	virtual bool Open(int percent = 100) = 0;
	virtual bool Close() = 0;
};
#endif
