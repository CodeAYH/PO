//---------------------------------------------------------------------------

#ifndef ICPDASDIOH
#define ICPDASDIOH
//---------------------------------------------------------------------------
#include "IOBase.h"

class ICPDASDIO : public SerialDIO
{
protected:

public:
	ICPDASDIO();
	~ICPDASDIO();

	void SetIO(const UINT nIdx);
};
#endif
