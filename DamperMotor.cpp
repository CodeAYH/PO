//---------------------------------------------------------------------------

#pragma hdrstop

#include "DamperMotor.h"
#include "HelperFunctions.h"
#include "SerialPortManager.h"
#include "Chamber.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

DamperMotor::DamperMotor(BaseChamber *chamber, PORTINFO portInfo, bool createPort)
	: SerialDevice(chamber, portInfo, createPort)
{
}

__fastcall DamperMotor::~DamperMotor()
{
}


