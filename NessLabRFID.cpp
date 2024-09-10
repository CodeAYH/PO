//---------------------------------------------------------------------------
#include <System.AnsiStrings.hpp>

#pragma hdrstop

#include "NessLabRFID.h"
#include "HelperFunctions.h"
#include "SerialPortManager.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define CIRCULAR_INC(x) ((x) = ((x) + 1) % CMD_COUNT)

enum
{
	CMD_CARD_DETECT = 0,
	CMD_CARD_READ,
	CMD_COUNT = 5
};

String CommandStringArray[CMD_COUNT] =
{
	"CARD DETECT",
	"CARD READ"
};

enum ERROR_CODE
{
	EC_BCC 				= 0x00,
	EC_NO_ANSWER    	= 0x31,
	EC_NO_CARD      	= 0x02,
	EC_CARD_ERROR   	= 0x03,
	EC_BLOCK_ACCESS 	= 0x04,
	EC_PARAM_ERROR  	= 0x05,
	EC_CARD_MODE_ERROR  = 0x06,
	EC_COUNT            = 7
};

int ErrorCodeArray[EC_COUNT] =
{
	EC_BCC,
	EC_NO_ANSWER,
	EC_NO_CARD,
	EC_CARD_ERROR,
	EC_BLOCK_ACCESS,
	EC_PARAM_ERROR,
	EC_CARD_MODE_ERROR
};

String ErrorCodeStringArray[EC_COUNT] =
{
	"BCC Error",
	"No Answer",
	"No Card",
	"Card Error",
	"Block Access Error",
	"Parameter Error",
	"Card Mode Error"
};

enum
{
	CT_UNKNOWN = 0,
	CT_MIFARE,
	CT_15693,
	CT_COUNT
};

String CardTypeString[] =
{
	"UNKNOWN",
	"MIFARE",
	"15693"
};

unsigned char CardDetect[STREAM_BYTE_COUNT] =
{
	0x44, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00
//	44002020202020202020202020202020202044
//	0x44, 0x02, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
//	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x46
};
unsigned char DataReadBlock1[STREAM_BYTE_COUNT] =
{
	0x52, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00
};
unsigned char DataReadBlock2[STREAM_BYTE_COUNT] =
{
	0x52, 0x01, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00
};
unsigned char DataReadBlock3[STREAM_BYTE_COUNT] =
{
	0x52, 0x02, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00
};
unsigned char DataReadBlock4[STREAM_BYTE_COUNT] =
{
	0x52, 0x03, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00
};
unsigned char *CommandArrary[CMD_COUNT] =
{
	CardDetect,
	DataReadBlock1,
	DataReadBlock2,
	DataReadBlock3,
	DataReadBlock4
};

NessLabRFID::NessLabRFID(BaseChamber *chamber, PORTINFO portInfo)
	: SerialDevice(chamber, portInfo)
{
	CardType = -1;
	TimeoutCount = 0;
	CommandMode = CMD_CARD_DETECT;
	FDevName = "NessLabRFID";
	CurrentCommand = nullptr;

	for (int i = 0; i < EC_COUNT; i++)
	{
		ErrorCodeString.insert(std::pair<int, String>(ErrorCodeArray[i], ErrorCodeStringArray[i]));
	}
}

__fastcall NessLabRFID::~NessLabRFID()
{

}

bool NessLabRFID::TxBinData(void *buf)
{
	if (ComPort == nullptr)
	{
		return false;
	}

	ZeroMemory(RcvdData, STREAM_BYTE_COUNT);
	ComPort->ReadBufFree();
	ComPort->WriteBufFree();

	RcvdError = false;
	ValidRxData = false;
	Timeout = false;
	ErrorCode = -1;
	TimeoutTimer->Enabled = true;
	ComPort->WriteBuf(buf, STREAM_BYTE_COUNT);

	return true;
}

void __fastcall NessLabRFID::OnRxChar(TObject *Sender, int Count)
{
	if (Count != STREAM_BYTE_COUNT)
	{
		char *temp = new char[Count];
		ComPort->ReadBuf(temp, Count);
        delete[] temp;
		return;
	}

	ComPort->ReadBuf(RcvdData, Count);

	if (IsRxDone() == false)
	{
		return;
	}

	TimeoutTimer->Enabled = false;
}

unsigned char NessLabRFID::CalcBCC(unsigned char *data, int length)
{
	unsigned char res = data[0];
	for (int i = 1; i < length; i++)
	{
		res = res ^ data[i];
	}
    return res;
}

void NessLabRFID::MakeCommand(unsigned char *cmd)
{
	unsigned char bcc;
	int length = STREAM_BYTE_COUNT - 1;
	bcc = CalcBCC(cmd, length);
	cmd[length] = bcc;
}

unsigned char* NessLabRFID::GetMonitorCommands()
{
	unsigned char *command = CommandArrary[CommandMode];
	return command;
}

void NessLabRFID::SendBeep(bool isGood)
{
	unsigned char goodBeep[STREAM_BYTE_COUNT] =
		{ 0x43, 0x01, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
		  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00 };
	unsigned char errorBeep[STREAM_BYTE_COUNT] =
		{ 0x43, 0x02, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
		  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00 };

	unsigned char *command = (isGood) ? goodBeep : errorBeep;
	MakeCommand(command);
	TxBinData(command);
}

#include <string>
bool NessLabRFID::IsAllSpace(String data)
{
	int length = data.Length() / 2;
	for (int i = 1; i <= length * 2; i+=2)
	{
		String str = data.SubString(i, 2);
		int ascii = std::stoi(str.c_str(), nullptr, 16);
		if (ascii != 0x20)
		{
			return false;
        }
	}
    return true;
}

void NessLabRFID::DecodeMonitor()
{
	if (RcvdData[0] == 'E')
	{
		ErrorCode = RcvdData[1];
		if (CommandMode == CMD_CARD_DETECT)
		{
			if (ErrorCode == EC_NO_CARD)
			{
				RcvdError = false;
			}
			else
			{
				RcvdError = true;
			}
		}
		else if (CommandMode >= CMD_CARD_READ)
		{
			RcvdError = true;
        }

		String str;
		str.sprintf(L"[%s] Received Error Code = %02X (%s)", String(DevName).c_str(), ErrorCode, ErrorCodeString[ErrorCode].c_str());
		OutputDebugString(str.c_str());
	}
	else
	{
		if (CommandMode == CMD_CARD_DETECT)
		{
			String uid;
			CardType = RcvdData[1];

			int length = (CardType == CT_MIFARE) ? 4 : 8;
			for (int i = 2; i < length + 2; i++)
			{
				String str;
				str.sprintf(L"%02X", RcvdData[i]);
				uid += str;
			}
			uid.Trim();
			if (IsAllSpace(uid))
			{
                ErrorCode = EC_NO_CARD;
				InitCardData();
			}
			else
			{
				uid = "0x" + uid;
				if (CardUID.IsEmpty())
				{
					CardUID = uid;
				}
				else
				{
					NewCardUID = uid;
				}
			}

			String str;
			str.sprintf(L"[%s] Received Card Type = %s, UID = %s, New UID = %s",
				String(DevName).c_str(), CardTypeString[CardType].c_str(), CardUID.c_str(), NewCardUID.c_str());
			OutputDebugString(str.c_str());
		}
		else if (CommandMode >= CMD_CARD_READ)
		{
			String rcvdData;
			int length = (CardType == CT_MIFARE) ? 16 : 4;
			for (int i = 2; i < length + 2; i++)
			{
				if (RcvdData[i] > 0x20 && RcvdData[i] < 0x7F)
				{
					String str;
					str.sprintf(L"%c", RcvdData[i]);
					rcvdData += str;
				}
			}
			rcvdData.Trim();

			if (rcvdData.IsEmpty())
			{
				RcvdError = true;
			}
			else
			{
				CardData += rcvdData;
            }

			String str;
			str.sprintf(L"[%s] Received Card Type = %s, Data = %s", String(DevName).c_str(), CardTypeString[CardType].c_str(), CardData.c_str());
			OutputDebugString(str.c_str());
		}
	}
}

bool NessLabRFID::IsRxDone()
{
	unsigned char rcvdBcc = RcvdData[STREAM_BYTE_COUNT - 1];
	unsigned char bcc = CalcBCC(RcvdData, STREAM_BYTE_COUNT - 1);
	bool ret = false;

	if ((RcvdData[0] == 'A' || RcvdData[0] == 'E') && rcvdBcc == bcc)
	{
		ValidRxData = true;
		ret = true;
	}
	else
	{
		RcvdError = true;
    }
	return ret;
}

void NessLabRFID::DoCommunicate()
{
	DoMonitor();
}

bool NessLabRFID::IsNewCardID()
{
	bool ret = false;
	if (CardUID.IsEmpty() == false && NewCardUID.IsEmpty())
	{
		ret = true;
	}
	else if (CardUID.IsEmpty() == false && NewCardUID.IsEmpty() == false)
	{
		ret = CardUID != NewCardUID;
	}
    return ret;
}

void NessLabRFID::InitCardData()
{
	CardUID = "";
	NewCardUID = "";
	CardData = "";
}

void NessLabRFID::DoMonitor()
{
	if (ComPort == nullptr)
	{
		return;
	}

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 1000;

	ComPort->OnRxChar = OnRxChar;

	TStringList *list = new TStringList;

	CurrentCommand = GetMonitorCommands();
	MakeCommand(CurrentCommand);
	TxBinData(CurrentCommand);

	String str;
	str.sprintf(L"[%s] Command %s Sended", String(DevName).c_str(), CommandStringArray[CommandMode].c_str());
	OutputDebugString(str.c_str());

	if (WaitRx1())
	{
		DecodeMonitor();
		if (CommandMode == CMD_CARD_DETECT)
		{
			if (ErrorCode == EC_NO_CARD)
			{
				InitCardData();
			}
			else
			{
				if (RcvdError)
				{
					InitCardData();
					SendBeep(false);
				}
				else
				{
					if (CardData.IsEmpty())
					{
						CIRCULAR_INC(CommandMode);
                    }
                }
			}
		}
		else if (CommandMode >= CMD_CARD_READ)
		{
			if (ErrorCode == EC_NO_CARD || RcvdError)
			{
				InitCardData();
				SendBeep(false);
//				CIRCULAR_INC(CommandMode);
				CommandMode = CMD_CARD_DETECT;
			}
			else
			{
				if (CommandMode >= CMD_COUNT - 1)
				{
//					// 챔버 구분을 위한 Address 번호
//					INPUT addr = { 0 };
//					addr.type = INPUT_KEYBOARD;
//					addr.ki.wVk = 0x30 + Addr;
//					SendInput(1, &addr, sizeof(INPUT));

					int length = CardData.Length();

					for (int i = 0; i < length; i++)
					{
						const char *text = AnsiString(CardData).c_str();
						unsigned char ch = text[i];
						if (ch >= 'A' && ch <= 'Z')
						{
							INPUT input = { 0 };

							input.type = INPUT_KEYBOARD;
							input.ki.dwFlags = 0;
							input.ki.wVk = VK_SHIFT;
							SendInput(1, &input, sizeof(INPUT));

							input.ki.wVk = ch;
							SendInput(1, &input, sizeof(INPUT));

							input.ki.dwFlags = KEYEVENTF_KEYUP;
							SendInput(1, &input, sizeof(INPUT));

							input.ki.wVk = VK_SHIFT;
							SendInput(1, &input, sizeof(INPUT));
						}
						else
						{
							INPUT input = { 0 };

							input.type = INPUT_KEYBOARD;
							input.ki.wVk = ch;
							SendInput(1, &input, sizeof(INPUT));

							input.ki.dwFlags = KEYEVENTF_KEYUP;
							SendInput(1, &input, sizeof(INPUT));
						}
					}

					INPUT key = { 0 };
					key.type = INPUT_KEYBOARD;
					key.ki.wVk = VK_RETURN;
					SendInput(1, &key, sizeof(INPUT));

					SendBeep(true);
					Sleep(300);

					CommandMode = CMD_CARD_DETECT;
				}
				else
				{
					CIRCULAR_INC(CommandMode);
				}
			}
		}
	}
	else if (Timeout)
	{
//		TimeoutCount++;
//		if (TimeoutCount >= 2)
//		{
//			TimeoutCount = 0;
			CardUID = "";
			NewCardUID = "";
			CardData = "";
//		}

		String str;
		str.sprintf(L"[%s] Card Detect Timeout [%d]", String(DevName).c_str(), TimeoutCount);
		OutputDebugString(str.c_str());
	}

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

String NessLabRFID::GetCardData()
{
	String data = CardData;
	CardData = "";
	return data;
}
