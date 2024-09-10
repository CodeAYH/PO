//---------------------------------------------------------------------------

#ifndef EziStepMotorH
#define EziStepMotorH
//---------------------------------------------------------------------------
#include "DamperMotor.h"

class EziStepMotor : public DamperMotor
{
protected:
	static int PortNo;
	static bool IsConnected;
	static std::shared_ptr<TCriticalSection> csLock;

private:
	int FRevolutionCounter;	// 한바퀴 돌리는데 필요한 펄스 수

	int FuncResult;

	void __fastcall Execute();
	void DoMonitor();
	void DecodeMonitor() {}
	bool IsRxDone() { return true; }
	void GetMonitorCommands(TStringList *list) {}
	bool WaitRx1();
	bool WaitRx2();
	bool RetryCommand();

	void Homing();

	void DoStop(PARAM* param);
	void DoMotorEnable(PARAM* param);
	void DoAlarmClear(PARAM* param);
	void DoSearchHome(PARAM* param);
	void DoMoveStepAbsolute(PARAM* param);
	bool MoveStepAbsolute(int pos);

public:
	EziStepMotor(BaseChamber *chamber, PORTINFO portInfo);
	__fastcall ~EziStepMotor();

	bool IsAlarm();
	static bool Connect(int portNo);
	static bool Disconnect(int axisCount);
	static bool SetParameter(int axisCount);

	bool AlarmClear();
	bool MotorEnable(bool enable=true);
	bool Stop();

	bool SearchHome();
	bool Open(int percent = 100);
	bool Close();
};
#endif
