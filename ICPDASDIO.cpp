//---------------------------------------------------------------------------

#pragma hdrstop

#include "ICPDASDIO.h"
#include "I7000DIOModule.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

ICPDASDIO::ICPDASDIO()
	: SerialDIO()
{
}
//---------------------------------------------------------------------------
ICPDASDIO::~ICPDASDIO()
{
}
//---------------------------------------------------------------------------
void ICPDASDIO::SetIO(const UINT nIdx)
{
	if (DIOModule == nullptr)
	{
		return;
	}

	Connected = DIOModule->IsOpened;
	FIndex = nIdx;
	FIsDO = false;
	Offset = (FIndex%16) & 0xFF;

	if (Connected == false)
	{
		return;
	}

	switch(((I7000DIOModule*)DIOModule)->ModuleID)
	{
	case I_7045:
		FIsDO = true;
	break;
	case I_7051:
		FIsDO = false;
	break;
	case I_7055:
		if (Offset < 8)
		{
			FIsDO = false;
		}
		else
		{
			FIsDO = true;
		}
	break;
	}
}
//---------------------------------------------------------------------------
