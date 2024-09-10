//---------------------------------------------------------------------------

#ifndef VSOVENBOARDDIOH
#define VSOVENBOARDDIOH
//---------------------------------------------------------------------------
#include "IOBase.h"

class VSOvenBoardDIO : public SerialDIO
{
private:
	bool GetIsOn();
	bool GetIsOff();

public:
	VSOvenBoardDIO();
	~VSOvenBoardDIO();

	void SetIO(const UINT nIdx);
};
#endif
