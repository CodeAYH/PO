//---------------------------------------------------------------------------

#ifndef I7000DIOModuleH
#define I7000DIOModuleH
//---------------------------------------------------------------------------
#include "SerialDevice.h"

enum ICPDAS_MODULEID
{
	I_7045,		// Digital Output 16 Port
	I_7051,		// Digital Input 16 Port
	I_7055		// Digital Input 8 Port, Digital Output 8 Port
};

class I7000DIOModule : public SerialIODevice
{
private:
	int FModuleID;

	void DoCommunicate();
	void DoMonitor();
	void DecodeMonitor();
	bool IsRxDone();
	void GetMonitorCommands(TStringList *list);
	bool GetReadValue(int idx);
//	bool GetWriteValue(int idx);
//	void SetWriteValue(int idx, bool value);
	void DoWriteOutput();

public:
	I7000DIOModule(BaseChamber *chamber, PORTINFO portInfo, int moduleid);
	__fastcall ~I7000DIOModule();

	__property int ModuleID = { read = FModuleID, write = FModuleID };
	__property bool ReadValue[int idx] = { read = GetReadValue };

	void SetPortOn(int portIdx);
	void SetPortOff(int portIdx);
};
#endif
