//---------------------------------------------------------------------------
#ifndef VXSeriesH
#define VXSeriesH
//---------------------------------------------------------------------------
#include "LimitController.h"

// 20191011 YJYOO - 한영넉스 VX Series 추가
class VXSeries : public DevLimitController
{
protected:
	void DecodeMonitor();
	bool IsRxDone();
	void GetMonitorCommands(TStringList *list);

	bool IsValidHexParam();
	void DoWriteLimitConfig(PARAM *param);
	void DoReadLimitConfig(PARAM *param);
	void DoWriteBias(PARAM *param);
	void DoWriteSP(PARAM *param);

	void DoCommunicate();
	void DoMonitor();

public:
	VXSeries(BaseChamber *chamber, PORTINFO portInfo, int limitType);
	__fastcall ~VXSeries();

	bool IsValidLimitParam();
};
#endif
