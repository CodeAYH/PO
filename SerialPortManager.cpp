//---------------------------------------------------------------------------

#pragma hdrstop

#include "SerialPortManager.h"
#include "HelperFunctions.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma comment(lib, "vacommpkgdXE12.lib")

AnsiString SerialPortName(int port)
{
	AnsiString str;
	str.sprintf("COM%d", port);
	return str;
}

SerialPort::SerialPort()
	: TVaComm(nullptr)
{
	csLock = std::shared_ptr<TCriticalSection>(new TCriticalSection());
	FRegisteredDeviceCount = 0;
	FCurrentDeviceIndex = 0;
	FOpened = false;
}

__fastcall SerialPort::~SerialPort()
{
}

bool SerialPort::OpenPort(int portNo, PORTSETTING portSetting)
{
	AutoOpen = false;
	Close();

	Baudrate = (TVaBaudrate)portSetting.baudrate;
	Parity = (TVaParity)portSetting.parity;
	Databits = (TVaDatabits)portSetting.databits;
	Stopbits = (TVaStopbits)portSetting.stopbits;
	PortNum = portNo;

	try
	{
		Open();
 	}
 	catch (...)
	{
  		String str;
		SetMessageStr(GetResStr("Error", "Serial Connection Fail"));
		str.sprintf(L"[COM%d]", portNo);
		SetMessageStr(str);
		ShowMessageDlg();

		FOpened = false;
		return false;
	}

	FOpened = true;
	return true;
}

bool SerialPort::ClosePort()
{
	try
	{
		Close();
	}
	catch (...)
	{
		return false;
	}

	return true;
}

void SerialPort::IncCurrentDeviceIndex()
{
	FCurrentDeviceIndex++;
	if (FCurrentDeviceIndex >= FRegisteredDeviceCount)
	{
		FCurrentDeviceIndex = 0;
	}
}

SerialPortManager::SerialPortManager()
{
	PortList = new TStringList;
}

SerialPortManager::~SerialPortManager()
{
	if( PortList )
	{
		SerialPort *port;
		for(int i=0; i<PortList->Count; i++)
		{
			port = (SerialPort*)PortList->Objects[i];
			if( port )
			{
				port->ClosePort();
				delete port;
			}
		}
		delete PortList;
	}
}

SerialPort* SerialPortManager::AddPort(int portNo)
{
	SerialPort *port = new SerialPort;
	if( port )
	{
		port->PortNum = portNo;
		PortList->AddObject(SerialPortName(portNo), port);
	}
	return port;
}

SerialPort* SerialPortManager::DeletePort(int portNo)
{
	if( PortList == nullptr )
		return nullptr;

	if( PortList->Count == 0 )
		return nullptr;

	int idx = PortList->IndexOf(SerialPortName(portNo));
	if( idx >= 0 )
	{
		SerialPort *port = (SerialPort*)PortList->Objects[idx];
		PortList->Delete(idx);
		return port;
	}

	return nullptr;
}

SerialPort* SerialPortManager::FindPort(int portNo)
{
	if( PortList == nullptr )
		return nullptr;

	if( PortList->Count == 0 )
		return nullptr;

	for(int i=0; i<PortList->Count; i++)
	{
		SerialPort *port = (SerialPort*) PortList->Objects[i];
		if( port->PortNum == portNo )
			return port;
	}

	return nullptr;
}

SerialPort* SerialPortManager::GetPort(int portNo, PORTSETTING portSetting)
{
	SerialPort *port = FindPort(portNo);
	if( port == nullptr )
	{
		port = AddPort(portNo);
		if( port )
			port->OpenPort(portNo, portSetting);
	}

	return port;
}

bool SerialPortManager::RemovePort(int portNo)
{
	SerialPort *port = FindPort(portNo);
	if( port == nullptr )
		return false;

	if( port->ClosePort() )
	{
		if( DeletePort(portNo) )
		{
			delete port;
			return true;
		}
	}

	return false;
}
