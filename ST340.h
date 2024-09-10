//---------------------------------------------------------------------------
#ifndef ST340H
#define ST340H
//---------------------------------------------------------------------------
#include "LimitController.h"

// 20191011 YJYOO - 한영넉스 VX Series 추가로 인한  부모 클래스 변경
class ST340 : public DevLimitController
{
private:
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
	ST340(BaseChamber *chamber, PORTINFO portInfo, int limitType);
	__fastcall ~ST340();

	bool IsValidLimitParam();
};
#endif
