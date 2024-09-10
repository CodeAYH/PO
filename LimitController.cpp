//---------------------------------------------------------------------------
#include <System.AnsiStrings.hpp>

#pragma hdrstop

#include "LimitController.h"
#include "HelperFunctions.h"
#include "SerialPortManager.h"
#include "Chamber.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

DevLimitController::DevLimitController(BaseChamber *chamber, PORTINFO portInfo, int limitType)
		: SerialDevice(chamber, portInfo)
{
	FAutoConfig = true;
	FBiasUpdated = false;
	FSPUpdated = false;

	FLimitType = limitType;
	if( limitType == CHAMBER_TEMP_LIMIT ||
		limitType == HEATER_TEMP_LIMIT )
	{
		FScaleFactor = 10.0;
		MaxLimit = 300;
		Limit1 = 200;
		Limit2 = 200;
		LimitOffset = 10;
		DP = 1;
		SP = 1750;
	}
	else
	if( limitType == PRESSURE_LIMIT )
	{
		FScaleFactor = 100.0;
		DP = 2;
		MaxLimit = 10;
		Limit1 = 8;
		Limit2 = 0;
		LimitOffset = 0;
		SP = 9;
	}
	else
	if( limitType == HEATER_CONTROLLER )
	{
		FScaleFactor = 10.0;
		MaxLimit = 300;
		Limit1 = 200;
		Limit2 = 200;
		LimitOffset = 10;
		DP = 1;
		SP = 0;
	}
}

DevLimitController::~DevLimitController()
{
}

double DevLimitController::GetMaxLimit()
{
	return FMaxLimit / ScaleFactor;
}

void DevLimitController::SetMaxLimit(double limit)
{
	FMaxLimit = (int)(limit * ScaleFactor);
}

double DevLimitController::GetLimit1()
{
	return (double)FLimit1 / ScaleFactor;
}

void DevLimitController::SetLimit1(double limit)
{
	FLimit1 = (int)(limit * ScaleFactor);
}

double DevLimitController::GetLimit2()
{
	return (double)FLimit2 / ScaleFactor;
}

void DevLimitController::SetLimit2(double limit)
{
	FLimit2 = (int)(limit * ScaleFactor);
}

double DevLimitController::GetLimitOffset()
{
	return FLimitOffset / ScaleFactor;
}

void DevLimitController::SetLimitOffset(double offset)
{
	FLimitOffset = (int)(offset * ScaleFactor);
}

double DevLimitController::GetBias()
{
	return (double)FBias / ScaleFactor;
}

void DevLimitController::SetBias(double bias)
{
	double tmp = (double)(bias * ScaleFactor);
	FBias = (int)tmp;
}

void DevLimitController::SetDP(int dp)
{
	double tmpLimit1, tmpLimit2, tmpLimitOffset, tmpMaxLimit, tmpBias;

	tmpLimit1 = Limit1;
	tmpLimit2 = Limit2;
	tmpLimitOffset = LimitOffset;
	tmpMaxLimit = MaxLimit;
	tmpBias = Bias;

	if( dp == 0 )
		FScaleFactor = 1;
	else
	if( dp == 1 )
		FScaleFactor = 10;
	else
	if( dp == 2 )
		FScaleFactor = 100;
	else
		FScaleFactor = 1;

	FDP = dp;
	Limit1 = tmpLimit1;
	Limit2 = tmpLimit2;
	LimitOffset = tmpLimitOffset;
	MaxLimit = tmpMaxLimit;
	Bias = tmpBias;
}

void DevLimitController::SetLimitType(int limitType)
{
	FLimitType = limitType;
}

double DevLimitController::GetPV()
{
	return FReceivedData.PV / ScaleFactor;
}

double DevLimitController::GetSP()
{
	return FReceivedData.SP / ScaleFactor;
}

void DevLimitController::SetSP(double sp)
{
	double tmp = (double)(sp * ScaleFactor);
	FSP = (int)tmp;
}

void DevLimitController::SetConfigData(int reg[], int data[], int count)
{
	for(int i=0; i<count; i++)
	{
		ConfigRegister[i] = reg[i];
		ConfigData[i] = data[i];
	}
	ConfigDataCount = count;
}

bool DevLimitController::WriteLimitConfig()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoWriteLimitConfig;
	ret = RetryCommand();

	return ret;
}

bool DevLimitController::ReadLimitConfig()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoReadLimitConfig;
	ret = RetryCommand();

	return ret;
}

bool DevLimitController::WriteBias()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoWriteBias;
	ret = RetryCommand();

	FBiasUpdated = true;

	return ret;
}

bool DevLimitController::WriteSP()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoWriteSP;
	ret = RetryCommand();

	FSPUpdated = true;

	return ret;
}

void DevLimitController::AutoConfigParam()
{
	if( FAutoConfig == true )
	{
		MonitorCount++;
		if( MonitorCount > 5 )
		{
			ReadLimitConfig();
			if( IsValidHexParam() )
			{
				if( IsValidLimitParam() == false )
					WriteLimitConfig();
			}
			MonitorCount = 0;
		}
	}
}

