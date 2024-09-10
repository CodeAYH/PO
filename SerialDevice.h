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
 * @brief 쓰레드 내에서 호출하는 함수들에 전달할 파라미터 구조체
 * @details
 * 메인 쓰레드에서 호출하는 함수로 전달된 파라미터를 클래스 내 쓰레드에서
 * 호출하는 private 함수 들에게 전달하기 위해서 사용한다.
 * @author 유영재, yjyoo@visionsemicon.co.kr
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

	// 전체 수신 데이터 중 스타트 코드, 주소 등을 제외한 실제 데이터
	AnsiString RxData;

	// 통신 실패시 재송신 카운터
	int RetryCount;

	// 송신할 데이터
	AnsiString TxBuffer;

	// 통신 실패 카운터. 지정된 횟수를 초과하면 0으로 초기화된다.
	unsigned int CommFailCount;

	// 총 실패 카운터
	unsigned int FFailCount;

	// 총 성공 카운터
	unsigned int FSuccessCount;

	// 외부 함수가 등록하는 내부 함수. 쓰레드내에서 호출된다.
	CommandFuncPtr CommandFunc;

	// 명령 전송 후 데이터 수신시 까지 기다리는 타이머
	TTimer *TimeoutTimer;

	// 내부 커맨드 함수에 넘겨줄 파라미터를 가리키는 포인터
	PARAM *Param;

	// 커맨드 모드 여부 플래그. true = 커맨드 모드, false = 모니터링 모드
	bool CommandMode;

	// 커맨드 모드 완료 여부 플래그
	bool CommandDone;
	bool WaitCommandResult;

	// 수신된 데이터의 무결성 여부 플래그. IsRxDone 함수가 설정
	bool ValidRxData;

	// 타임 아웃 여부 플래그
	bool Timeout;

	// 통신 실패 여부 플래그. 통신 실패가 일정 횟수 이상되면 통신 실패로 설정된다.
	bool FCommFail;

	// 통신할 파트의 주소.
	//보통 1이상의 값을 가지며 같은 포트 안에서는 유일한 값을 가져야한다.
	int FAddr;

	// 통신 포트 포인터
	SerialPort *ComPort;

	// 디바이스 이름
	AnsiString FDevName;

	// 일부 디바이스는 실수형 데이터를 정수형으로 보낸다. 이때 실수형으로 변환하기 위한 배율.
	double FScaleFactor;

	// 주기적인 모니터링 이외에 실행 명령 커맨드를 주기적으로 전송할 필요가 있을 때 사용하는 변수
	int MonitorCount;

	// 쓰레드 함수
	virtual void __fastcall Execute();

	// 포트에서 데이터를 수신할 때 호출되는 콜백 함수
	// 수신된 데이터를 검사하여 올바른 데이터가 수신완료 되었으면 수신 응답 타이머를 종료하고 리턴한다.
	virtual void __fastcall OnRxChar(TObject *Sender, int Count);

	// 수신응답 타이머가  호출하는 콜백 함수
	// 타임아웃 플래그를 설정하여 응답 시간이 지났음을 알린다.
	virtual void __fastcall OnTimeout(TObject *Sender);

	// 수신된 데이터가 프로토콜에 맞게 수신되었는지 검사한다.
	virtual bool IsRxDone() = 0;

	// 수신된 데이터를 해석하여 해당 변수에 저장한다.
	virtual void DecodeMonitor() = 0;

	// 쓰레드에서 주기적으로 송신할 명령을 생성한다.
	// TStringList *list : 송신할 명령어 리스트. 이 함수를 호출한 쪽에서 생성과 해제를 책임진다.
	virtual void GetMonitorCommands(TStringList *list) {};

	// 쓰레드에서 주기적으로 송신할 명령을 전송하고 수신된 데이터를 해석한다.
	// GetMonitorCommands 함수를 호출하여 명령어 리스트를 얻어와 전송한 후 수신된 결과를DecodeMonitor 함수를 호출하여 해석한다.
	virtual void DoMonitor();

	// 데이터 수신이 완료될 때까지 기다리는 함수
	// 클래스 내 쓰레드에서 사용한다.
	virtual bool WaitRx1();

	// 데이터 수신이 완료될 때까지 기다리는 함수
	// 메인 쓰레드(UI 쓰레드)에서 사용한다.
	virtual bool WaitRx2();

	// Execute 함수에서 직접 호출하는 함수
	// 쓰레드는 항상 수행해야 하는 모니터링 모드와 외부 함수를 실행해야 하는 커맨드 모드로 나뉜다.
	// 모니터렁 모드는 DoMonitor 함수를, 커맨드 모드는 각 외부 함수가 등록한 내부 함수를 호출한다.
	virtual void DoCommunicate();

	// 외부 함수를 실행할 때 통신 오류가 날 경우 지정된 횟수만큼 재실행한다.
	virtual bool RetryCommand();

	// 포트가 열려 있는지 확인해주는 플래그.
	virtual bool GetIsOpened();

	// 데이터 수신에 필요한 변수들을 초기화한다.
	virtual void InitRxValues();

	// 데이터를 송신한다.
	bool TxData(const AnsiString data);

public:
	// 멤버 변수 초기화와 응답 타이머를 생성하고 초기화한다.
	// BaseChamber *chamber : 소속 챔버의 포인터
	// PORTINFO portInfo : 속도 등 시리얼 통신 설정관련 변수
	SerialDevice(BaseChamber *chamber, PORTINFO portInfo, bool createPort = true);

	// 응답 타이머를 제거한다.
	__fastcall ~SerialDevice();

	// 지정된 시간만큼 기다린다.
	// unsigned int milliSeconds : 기다릴 시간으로 단위는 밀리세컨드
	// bool workInThread : true = 클래스 내 쓰레드에서 사용, false = 메인 쓰레드(UI 쓰레드)에서 사용
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
