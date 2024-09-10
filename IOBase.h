//---------------------------------------------------------------------------

#ifndef IOBaseH
#define IOBaseH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "Defines.h"

enum { ANALOG_12BIT_RES=12, ANALOG_16BIT_RES=16 };

//---------------------------------------------------------------------------
class SerialIODevice;
class DIO
{
protected:
	int  FIndex;

	bool FIsDO;
	bool Connected;
	bool Test;
	TDateTime StableOnTimer;
	TDateTime StableOffTimer;

	virtual bool GetIsOn() = 0;
	virtual bool GetIsOff() = 0;

public:
	DIO();
	~DIO();

	__property int Index = { read = FIndex, write = FIndex };
	__property bool IsDO = { read = FIsDO };

	bool IsOn(const DWORD onTime = 0);
	bool IsOff(const DWORD offTime = 0);
	virtual void SetIO(const UINT nIdx) = 0;
	virtual void On() = 0;
	virtual void Off() = 0;
};

class SerialDIO : public DIO
{
protected:
	unsigned short Offset;

	virtual bool GetIsOn();
	virtual bool GetIsOff();

public:
	SerialDIO();
	~SerialDIO();

	SerialIODevice *DIOModule;

	virtual void On();
	virtual void Off();
};

class AnalogIO
{
protected:
	int  FIndex;
	bool Connected;

	long Resolution;
	unsigned short DevRange;
	double VoltRange;
	double Calibrate;

public:
	__property int Index = { read = FIndex, write = FIndex };

	AnalogIO();
	~AnalogIO();

	virtual void Set(unsigned short bitRes,
							unsigned short devRange,
							double voltRange,
							double cal) = 0;
};

class AnalogInput : public AnalogIO
{
protected:
	double FAveValueDouble;
	unsigned short FAveValueInt;

public:
	AnalogInput();
	~AnalogInput();

	__property double AveValueDouble = { read = FAveValueDouble, write = FAveValueDouble };
	__property unsigned short AveValueInt = { read = FAveValueInt, write = FAveValueInt };

	virtual bool Read(double &val) = 0;
	virtual bool ReadRawData(unsigned short &val) = 0;
};

class AnalogOutput : public AnalogIO
{
protected:

public:
	AnalogOutput();
	~AnalogOutput();

	virtual double Read() = 0;
	virtual bool Write(double val) = 0;
};

#endif
