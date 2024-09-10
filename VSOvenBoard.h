//---------------------------------------------------------------------------

#ifndef VSOvenBoardH
#define VSOvenBoardH
//---------------------------------------------------------------------------
#include "SerialDevice.h"

#define BUF_LEN		30

class VSOvenBoard : public SerialIODevice
{
private:
	unsigned int DIValue;
//	unsigned int DOValue;
//	unsigned int DOWrite;
//	bool DOWriteCheck;

	unsigned char RxData[BUF_LEN];
	int ExpectedRcvLength;
	int CurrentRcvLength;

	void DoCommunicate();
	void DoMonitor();
	void DoWriteOutput();
	void DecodeMonitor();
	bool IsRxDone();
	void GetMonitorCommands(TStringList *list) {}

	bool TxBinData(unsigned char *data, int len);
	unsigned short CRC16(unsigned char *puchMsg, int usDataLen);
	void __fastcall OnRxChar(TObject *Sender, int Count);

public:
	VSOvenBoard(BaseChamber *chamber, PORTINFO portInfo);
	__fastcall ~VSOvenBoard();

	bool GetReadValue(int idx);
	void SetPortOn(int portIdx);
	void SetPortOff(int portIdx);
};
#endif
