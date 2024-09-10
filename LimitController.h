//---------------------------------------------------------------------------

#ifndef LimitControllerH
#define LimitControllerH
//---------------------------------------------------------------------------
#include "SerialDevice.h"

// 20191011 YJYOO - �ѿ��˽� VX Series �߰��� ���� ����Ʈ ��Ʈ�ѷ� �߻� Ŭ���� �߰�
// ST340, VXSeries Ŭ������ �θ� Ŭ����

enum CTRLTYPE { LIMIT_ST = 0, LIMIT_VX, LIMIT_NONE };
enum LIMITTYPE { CHAMBER_TEMP_LIMIT, HEATER_TEMP_LIMIT, PRESSURE_LIMIT, HEATER_CONTROLLER };

class DevLimitController : public SerialDevice
{
protected:
	bool FAutoConfig;	// ����Ʈ ��Ʋ�ѷ��� �������� ������� �Ǽ������� ����Ǿ��� ��
						// �ڵ����� �����ϴ� ����� ������� ����
	bool FBiasUpdated;	// bias��������Ʈ �Ǿ��º� ����.
						// bias ������Ʈ�� �ϸ� ST340�� �ݿ��Ǳ���� �ð��� �ɸ��µ�
						// �� ������ �˱����� �÷���
						// �� �÷��װ� false �϶��� WriteBias�� ȣ���ؾ� �Ѵ�.
	bool FSPUpdated;	// SP��������Ʈ �Ǿ��º� ����. ��

	int ConfigRegister[100];
	int ConfigData[100];
	bool ConfigDataChanged[100];
	int ConfigDataCount;

	int FLimitType;
	int FMaxLimit;
	int FLimit1;
	int FLimit2;
	int FLimitOffset;
	int FBias;
	int FDP;
	int FSP;
	LIMITDATA FReceivedData;

	AnsiString RxTempData;

	virtual void SetLimitType(int limitType);
	virtual double GetMaxLimit();
	virtual void SetMaxLimit(double limit);
	virtual double GetLimit1();
	virtual void SetLimit1(double limit);
	virtual double GetLimit2();
	virtual void SetLimit2(double limit);
	virtual double GetLimitOffset();
	virtual void SetLimitOffset(double offset);
	virtual double GetBias();
	virtual void SetBias(double bias);
	virtual void SetDP(int dp);
	virtual double GetPV();
	virtual double GetSP();
	virtual void SetSP(double sp);

	virtual bool IsValidHexParam() = 0;
	virtual void DoWriteLimitConfig(PARAM *param) = 0;
	virtual void DoReadLimitConfig(PARAM *param) = 0;
	virtual void DoWriteBias(PARAM *param) = 0;
	virtual void DoWriteSP(PARAM *param) = 0;

public:
	DevLimitController(BaseChamber *chamber, PORTINFO portInfo, int limitType);
	__fastcall ~DevLimitController();

	__property bool AutoConfig = { read = FAutoConfig, write = FAutoConfig };
	__property bool BiasUpdated = { read = FBiasUpdated, write = FBiasUpdated };
	__property bool SPUpdated = { read = FSPUpdated, write = FSPUpdated };
	__property int LimitType = { read = FLimitType, write = SetLimitType };
	__property double MaxLimit = { read = GetMaxLimit, write = SetMaxLimit };
	__property double Limit1 = { read = GetLimit1, write = SetLimit1 };
	__property double Limit2 = { read = GetLimit2, write = SetLimit2 };
	__property double LimitOffset = { read = GetLimitOffset, write = SetLimitOffset };
	__property double Bias = { read = GetBias, write = SetBias };
	__property int DP = { read = FDP, write = SetDP };
	__property double PV = { read = GetPV };
	__property double SP = { read = GetSP, write = SetSP };

	virtual bool IsValidLimitParam() = 0;

	void SetConfigData(int reg[], int data[], int count);
	virtual bool WriteLimitConfig();
	virtual bool ReadLimitConfig();
	virtual void AutoConfigParam();
	virtual bool WriteBias();
	virtual bool WriteSP();
};
#endif
