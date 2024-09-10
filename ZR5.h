//---------------------------------------------------------------------------

#ifndef ZR5H
#define ZR5H
//---------------------------------------------------------------------------
#include "SerialDevice.h"

class ZR5 : public SerialDevice
{
private:
enum { DECODE_DENSITY, DECODE_FLOWRATE };

	int DecodeMode;
	bool IsValidO2Value;
	bool IsValidFlowRateValue;
	bool FIsValid;
	bool FIsCommandInit;
	double FO2Density;
	double FFlowRate;

	void DoMonitor();
	void DecodeMonitor();
	bool IsRxDone();
	void GetMonitorCommands(TStringList *list);
	bool CheckDoubleValid(AnsiString data);

	void DoCommandInit(PARAM *param);

public:
	ZR5(BaseChamber *chamber, PORTINFO portInfo);
	__fastcall ~ZR5();

	__property bool IsValid = { read = FIsValid };
	__property bool IsCommandInit = { read = FIsCommandInit };
	__property double O2Density = { read = FO2Density };
	__property double FlowRate = { read = FFlowRate };

	bool CommandInit();
};
#endif
