//---------------------------------------------------------------------------

#ifndef SerialDeviceH
#define SerialDeviceH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <memory>
#include "Defines.h"

// 20181207 - Retry Count change - jhlim
//#define MAX_RETRY_COUNT			30
#define MAX_RETRY_COUNT			3

const char STX = 0x02;
const char ETX = 0x03;
const char CR  = 0x0D;
const char LF  = 0x0A;

// 20170225 - InverterIG5A Add - jhlim
const char ENQ 				= 0x05;
const char EOT 				= 0x04;
const char ACK 				= 0x06;
const char NAK 				= 0x15;
const char CMD_READ 		= 0x52;
const char CMD_WRITE 		= 0x57;
const char CMD_MONITOR_SET 	= 0x58;
const char CMD_MONITOR_RUN 	= 0x59;

// 20170317 - InverterVFDB Add - jhlim
const char STX_VFDB			= 0x3A;
const char CR_VFDB			= 0x0D;
const char LF_VFDB			= 0x0A;
const char DEVIDER_VFDB		= 0x5C;
const char CR1				= 0x72;
const char LF1				= 0x6E;

//// 20170427 - DIO Board - jhlim
//const char STX_DIOB = 0x02;
//const char ETX_DIOB = 0x0D;

/**
 * @brief ������ ������ ȣ���ϴ� �Լ��鿡 ������ �Ķ���� ����ü
 * @details
 * ���� �����忡�� ȣ���ϴ� �Լ��� ���޵� �Ķ���͸� Ŭ���� �� �����忡��
 * ȣ���ϴ� private �Լ� �鿡�� �����ϱ� ���ؼ� ����Ѵ�.
 * @author ������, yjyoo@visionsemicon.co.kr
*/
typedef struct _PARAM
{
	bool Bool;
	int Int;
	unsigned int Unsigned;
	double Double;
	AnsiString Char;
	void *Param;
} PARAM;

typedef void (__closure *CommandFuncPtr)(PARAM*);

class SerialPort;
class BaseChamber;
class SerialDevice : public TThread
{
protected:
	BaseChamber *MyChamber;
    int MyIndex;
	AnsiString RxBuffer;
	AnsiString RxTempBuffer;

	// ��ü ���� ������ �� ��ŸƮ �ڵ�, �ּ� ���� ������ ���� ������
	AnsiString RxData;

	// ��� ���н� ��۽� ī����
	int RetryCount;

	// �۽��� ������
	AnsiString TxBuffer;

	// ��� ���� ī����. ������ Ƚ���� �ʰ��ϸ� 0���� �ʱ�ȭ�ȴ�.
	unsigned int CommFailCount;

	// �� ���� ī����
	unsigned int FFailCount;

	// �� ���� ī����
	unsigned int FSuccessCount;

	// �ܺ� �Լ��� ����ϴ� ���� �Լ�. �����峻���� ȣ��ȴ�.
	CommandFuncPtr CommandFunc;

	// ��� ���� �� ������ ���Ž� ���� ��ٸ��� Ÿ�̸�
	TTimer *TimeoutTimer;

	// ���� Ŀ�ǵ� �Լ��� �Ѱ��� �Ķ���͸� ����Ű�� ������
	PARAM *Param;

	// Ŀ�ǵ� ��� ���� �÷���. true = Ŀ�ǵ� ���, false = ����͸� ���
	bool CommandMode;

	// Ŀ�ǵ� ��� �Ϸ� ���� �÷���
	bool CommandDone;
	bool WaitCommandResult;

	// ���ŵ� �������� ���Ἲ ���� �÷���. IsRxDone �Լ��� ����
	bool ValidRxData;

	// Ÿ�� �ƿ� ���� �÷���
	bool Timeout;

	// ��� ���� ���� �÷���. ��� ���а� ���� Ƚ�� �̻�Ǹ� ��� ���з� �����ȴ�.
	bool FCommFail;

	// ����� ��Ʈ�� �ּ�.
	//���� 1�̻��� ���� ������ ���� ��Ʈ �ȿ����� ������ ���� �������Ѵ�.
	int FAddr;

	// ��� ��Ʈ ������
	SerialPort *ComPort;

	// ����̽� �̸�
	AnsiString FDevName;

	// �Ϻ� ����̽��� �Ǽ��� �����͸� ���������� ������. �̶� �Ǽ������� ��ȯ�ϱ� ���� ����.
	double FScaleFactor;

	// �ֱ����� ����͸� �̿ܿ� ���� ��� Ŀ�ǵ带 �ֱ������� ������ �ʿ䰡 ���� �� ����ϴ� ����
	int MonitorCount;

	// ������ �Լ�
	virtual void __fastcall Execute();

	// ��Ʈ���� �����͸� ������ �� ȣ��Ǵ� �ݹ� �Լ�
	// ���ŵ� �����͸� �˻��Ͽ� �ùٸ� �����Ͱ� ���ſϷ� �Ǿ����� ���� ���� Ÿ�̸Ӹ� �����ϰ� �����Ѵ�.
	virtual void __fastcall OnRxChar(TObject *Sender, int Count);

	// �������� Ÿ�̸Ӱ�  ȣ���ϴ� �ݹ� �Լ�
	// Ÿ�Ӿƿ� �÷��׸� �����Ͽ� ���� �ð��� �������� �˸���.
	virtual void __fastcall OnTimeout(TObject *Sender);

	// ���ŵ� �����Ͱ� �������ݿ� �°� ���ŵǾ����� �˻��Ѵ�.
	virtual bool IsRxDone() = 0;

	// ���ŵ� �����͸� �ؼ��Ͽ� �ش� ������ �����Ѵ�.
	virtual void DecodeMonitor() = 0;

	// �����忡�� �ֱ������� �۽��� ����� �����Ѵ�.
	// TStringList *list : �۽��� ��ɾ� ����Ʈ. �� �Լ��� ȣ���� �ʿ��� ������ ������ å������.
	virtual void GetMonitorCommands(TStringList *list) {};

	// �����忡�� �ֱ������� �۽��� ����� �����ϰ� ���ŵ� �����͸� �ؼ��Ѵ�.
	// GetMonitorCommands �Լ��� ȣ���Ͽ� ��ɾ� ����Ʈ�� ���� ������ �� ���ŵ� �����DecodeMonitor �Լ��� ȣ���Ͽ� �ؼ��Ѵ�.
	virtual void DoMonitor();

	// ������ ������ �Ϸ�� ������ ��ٸ��� �Լ�
	// Ŭ���� �� �����忡�� ����Ѵ�.
	virtual bool WaitRx1();

	// ������ ������ �Ϸ�� ������ ��ٸ��� �Լ�
	// ���� ������(UI ������)���� ����Ѵ�.
	virtual bool WaitRx2();

	// Execute �Լ����� ���� ȣ���ϴ� �Լ�
	// ������� �׻� �����ؾ� �ϴ� ����͸� ���� �ܺ� �Լ��� �����ؾ� �ϴ� Ŀ�ǵ� ���� ������.
	// ����ͷ� ���� DoMonitor �Լ���, Ŀ�ǵ� ���� �� �ܺ� �Լ��� ����� ���� �Լ��� ȣ���Ѵ�.
	virtual void DoCommunicate();

	// �ܺ� �Լ��� ������ �� ��� ������ �� ��� ������ Ƚ����ŭ ������Ѵ�.
	virtual bool RetryCommand();

	// ��Ʈ�� ���� �ִ��� Ȯ�����ִ� �÷���.
	virtual bool GetIsOpened();

	// ������ ���ſ� �ʿ��� �������� �ʱ�ȭ�Ѵ�.
	virtual void InitRxValues();

	// �����͸� �۽��Ѵ�.
	bool TxData(const AnsiString data);

public:
	// ��� ���� �ʱ�ȭ�� ���� Ÿ�̸Ӹ� �����ϰ� �ʱ�ȭ�Ѵ�.
	// BaseChamber *chamber : �Ҽ� è���� ������
	// PORTINFO portInfo : �ӵ� �� �ø��� ��� �������� ����
	SerialDevice(BaseChamber *chamber, PORTINFO portInfo, bool createPort = true);

	// ���� Ÿ�̸Ӹ� �����Ѵ�.
	__fastcall ~SerialDevice();

	// ������ �ð���ŭ ��ٸ���.
	// unsigned int milliSeconds : ��ٸ� �ð����� ������ �и�������
	// bool workInThread : true = Ŭ���� �� �����忡�� ���, false = ���� ������(UI ������)���� ���
	void DelayTime(unsigned int milliSeconds, bool workInThread=true);

	__property int Addr = { read = FAddr, write = FAddr };
	__property double ScaleFactor = { read = FScaleFactor };
	__property AnsiString DevName = { read = FDevName };
	__property bool IsFail = { read = FCommFail };
	__property bool IsOpened = { read = GetIsOpened };
	__property unsigned int SuccessCount = { read = FSuccessCount };
	__property unsigned int FailCount = { read = FFailCount };
};

#include <bitset>
#define MAX_IO_COUNT	32
class SerialIODevice : public SerialDevice
{
protected:
	std::bitset<MAX_IO_COUNT> FReadValue;
	std::bitset<MAX_IO_COUNT> FWriteValue;
	bool IsFirstRun;

	virtual bool GetReadValue(int idx) = 0;

	virtual void DoCommunicate() = 0;
	virtual void DoWriteOutput() = 0;

public:
	__property bool ReadValue[int idx] = { read = GetReadValue };

	SerialIODevice(BaseChamber *chamber, PORTINFO portInfo, bool createPort = true);
	__fastcall ~SerialIODevice();

	virtual void SetPortOn(int portIdx) = 0;
	virtual void SetPortOff(int portIdx) = 0;
};
#endif
