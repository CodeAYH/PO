//---------------------------------------------------------------------------
#include <System.DateUtils.hpp>
#include "Dask.h"

#pragma hdrstop

#include "IOBase.h"
#include "SerialDevice.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

DIO::DIO()
{
	Connected = false;
	Test = false;

	StableOnTimer.Val = 0.0;
	StableOffTimer.Val = 0.0;
}
//---------------------------------------------------------------------------
DIO::~DIO()
{
}
//---------------------------------------------------------------------------
bool DIO::IsOn(const DWORD onTime/* = 0*/)
{
	if (onTime == 0)
	{
		bool on = this->GetIsOn();
		return on;
	}

	if (GetIsOn())
	{
		if (StableOnTimer.Val == 0.0)
		{
			StableOnTimer = Now();
		}
		if (MilliSecondsBetween(StableOnTimer, Now()) >= onTime)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
bool DIO::IsOff(const DWORD offTime/* = 0*/)
{
	if (offTime == 0)
	{
		return GetIsOff();
	}

	if (GetIsOff())
	{
		if (StableOffTimer.Val == 0.0)
		{
			StableOffTimer = Now();
		}
		if (MilliSecondsBetween(StableOffTimer, Now()) >= offTime)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
SerialDIO::SerialDIO() : DIO()
{
	DIOModule = nullptr;
}
//---------------------------------------------------------------------------
SerialDIO::~SerialDIO()
{
}
//---------------------------------------------------------------------------
bool SerialDIO::GetIsOn()
{
	if (DIOModule == nullptr)
	{
		return Test;
	}

	if (Connected == false)
	{
		return Test;
	}

	return DIOModule->ReadValue[Offset];
}
//---------------------------------------------------------------------------
bool SerialDIO::GetIsOff()
{
    return !IsOn();
}
//---------------------------------------------------------------------------
void SerialDIO::On()
{
	if (Connected == false || DIOModule == nullptr || DIOModule->IsFail)
	{
		Test = true;
	}
	else
	{
		if (FIsDO)
		{
			DIOModule->SetPortOn(Offset);
		}
	}
}
//---------------------------------------------------------------------------
void SerialDIO::Off()
{
	if (Connected == false || DIOModule == nullptr || DIOModule->IsFail)
	{
		Test = false;
	}
	else
	{
		if (FIsDO)
		{
			DIOModule->SetPortOff(Offset);
		}
	}
}
//---------------------------------------------------------------------------
AnalogIO::AnalogIO()
{
	FIndex = -1;
	Connected = false;

	Resolution = 0;
	DevRange = 0;
	VoltRange = 0;
	Calibrate = 0;
}
//---------------------------------------------------------------------------
AnalogIO::~AnalogIO()
{
}
//---------------------------------------------------------------------------
AnalogInput::AnalogInput()
{
	FAveValueInt = 0;
	FAveValueDouble = 0;
}
//---------------------------------------------------------------------------
AnalogInput::~AnalogInput()
{
}
//---------------------------------------------------------------------------
AnalogOutput::AnalogOutput()
{
}
//---------------------------------------------------------------------------
AnalogOutput::~AnalogOutput()
{
}
//---------------------------------------------------------------------------
