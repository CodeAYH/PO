//---------------------------------------------------------------------------

#pragma hdrstop

#include "VSOvenBoardDIO.h"
#include "VSOvenBoard.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

VSOvenBoardDIO::VSOvenBoardDIO()
	: SerialDIO()
{
}
//---------------------------------------------------------------------------
VSOvenBoardDIO::~VSOvenBoardDIO()
{
}
//---------------------------------------------------------------------------
void VSOvenBoardDIO::SetIO(const UINT nIdx)
{
	if( DIOModule == nullptr )
        return;

	FIndex = nIdx;
	FIsDO = false;
	Connected = DIOModule->IsOpened;

	if (Connected == false)
	{
		return;
	}

	if (FIndex < 32)
	{
		FIsDO = false;
	}
	else if (FIndex < 64)
	{
		FIsDO = true;
		Offset = FIndex - 32;
	}
}
//---------------------------------------------------------------------------
bool VSOvenBoardDIO::GetIsOn()
{
	if( Connected == false ||
		DIOModule == nullptr )
		return Test;

	return ((VSOvenBoard*)DIOModule)->GetReadValue(FIndex);
}
//---------------------------------------------------------------------------
bool VSOvenBoardDIO::GetIsOff()
{
    return !IsOn();
}
//---------------------------------------------------------------------------

