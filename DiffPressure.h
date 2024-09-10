//---------------------------------------------------------------------------

#ifndef DiffPressureH
#define DiffPressureH
//---------------------------------------------------------------------------
#include "SerialDevice.h"

class DiffPressure : public SerialDevice
{
private:
	char RxDPData[7];
	double FPV;
	int SkipCount;

	bool TxBinData(void *buf, int len);
	void DoMonitor();
	void DecodeMonitor();
	bool IsRxDone();
	void GetMonitorCommands(TStringList *list) {};

	void __fastcall OnRxChar(TObject *Sender, int Count);

public:
	DiffPressure(BaseChamber *chamber, PORTINFO portInfo);
	__fastcall ~DiffPressure();

	__property double PV = { read = FPV };
};
#endif
