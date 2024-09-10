//---------------------------------------------------------------------------
#ifndef NessLabRFIDH
#define NessLabRFIDH
//---------------------------------------------------------------------------
#include <map>
#include "SerialDevice.h"
#include <Vcl.Grids.hpp>

#define STREAM_BYTE_COUNT   19

class NessLabRFID : public SerialDevice
{
protected:
	std::map<int, String> ErrorCodeString;
	unsigned char RcvdData[STREAM_BYTE_COUNT];
	unsigned char *CurrentCommand;

    bool RcvdError;
	int CommandMode;
	int CardType;
	int ErrorCode;
    int TimeoutCount;
	String CardUID, NewCardUID;
	String CardData;

	bool TxBinData(void *buf);
	void MakeCommand(unsigned char *cmd);
	unsigned char CalcBCC(unsigned char *data, int length);
	void DecodeMonitor();
	bool IsRxDone();
	unsigned char* GetMonitorCommands();

	void SendBeep(bool isGood);
	void DoCommunicate();
	void DoMonitor();
	bool IsNewCardID();
	bool IsAllSpace(String data);
	void InitCardData();

	void __fastcall OnRxChar(TObject *Sender, int Count);

public:
	NessLabRFID(BaseChamber *chamber, PORTINFO portInfo);
	__fastcall ~NessLabRFID();

	String GetCardData();
};
#endif
