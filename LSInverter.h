//---------------------------------------------------------------------------

#ifndef LSInverterH
#define LSInverterH
//---------------------------------------------------------------------------
#include "SerialDevice.h"

//	"W" ENQ(1) ����(2) CMD(1) ����(4) ��������(1) data(n*4) sum(2) EOT(1)
//	"R" ENQ(1) ����(2) CMD(1) ����(4) ��������(1) 			sum(2) EOT(1)
//	"A" ENQ(1) ����(2) CMD(1) 		  			  data(n*4) sum(2) EOT(1)
//	"N" ENQ(1) ����(2) CMD(1)         �����ڵ�(2) data(n*4) sum(2) EOT(1)

const int ADDR_PARAM_SET_ENABLE	= 0x0004;		// �Ķ���� �������
const int ADDR_SET_FREQUENCY	= 0x0005;		// ���� ���ļ�
const int ADDR_DRV_COMMAND		= 0x0006;		// ���� ����
const int ADDR_INVERTER_STATUS	= 0x000E;		// ���� ���� (status of Inverter)
const int ADDR_TRIP_INFO_A		= 0x000F;		// Ʈ�� ����
const int ADDR_TRIP_INFO_B		= 0x001D;		// Ʈ������-B

const int INVERTER_STOP			= 1;
const int FORWARD_DIR 			= 2;
const int REVERSE_DIR			= 4;

typedef struct _LSInverterDATA
{
	bool ParamSetEnable;
	double Frequency;
	unsigned int DrvCommand;
	unsigned int InverterStatus;
	unsigned int TripA;
	unsigned int TripB;
	AnsiString ErrorCode;

	void Init()
	{
		ParamSetEnable = false;
		Frequency = 0.0;
		DrvCommand = 0;
		InverterStatus = 0;
		TripA = 0;
		TripB = 0;
		ErrorCode = "";
    }
} LSInverterDATA;

class LSInverter : public SerialDevice
{
	enum { READ_PARAM_SET_ENABLE = 0, READ_REQUENCY, READ_DRV_COMMAND, READ_INVERTER_STATUS/*, READ_TRIP_A, READ_TRIP_B*/ };

protected:
	int DecodeMode;
	LSInverterDATA ReceivedData;

	void DecodeMonitor();
	void DoCommunicate();
	void DoMonitor();
	bool IsRxDone();
	void GetMonitorCommands(TStringList *list);

	void DoSetParameterChangePermit(PARAM *param);
	void DoReadParameterChangePermit(PARAM *param);
	void DoSetRunFreq(PARAM *param);
	void DoSetDrvCommnad(PARAM *param);

	int CheckSum(AnsiString strData);
	AnsiString MakeCheckSum(AnsiString strData);
	AnsiString MakeWriteString(int regAddr, AnsiString value);
	AnsiString MakeReadString(int regAddr);

public:
	LSInverter(BaseChamber *chamber, PORTINFO portInfo);
	__fastcall ~LSInverter();

	bool IsParameterSetEnabled() { return ReceivedData.ParamSetEnable; }
	bool IsTrip();
	int GetMotorRunDir();
	double GetFrequency();

	bool SetParameterChangePermit();
	bool SetRunFreq(double freq);
	bool SetMotorRunForward();
	bool SetMotorRunReverse();
	bool SetMotorStop();
};
#endif
