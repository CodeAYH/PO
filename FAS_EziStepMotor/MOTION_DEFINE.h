
#pragma once
#include <tchar.h>

#include "MOTION_EziSERVO_DEFINE.h"
#include "MOTION_EziSERVO_ADC_DEFINE.h"
#include "MOTION_EziSERVO_ALL_DEFINE.h"
#include "MOTION_EziSERVO_ALL_ABS_DEFINE.h"
#include "MOTION_EziSERVO_mini_DEFINE.h"

#include "MOTION_EziSTEP_DEFINE.h"
#include "MOTION_EziSTEP_ALL_DEFINE.h"
#include "MOTION_EziSTEP_mini_DEFINE.h"

#include "MOTION_EziMotionLink_DEFINE.h"
#include "MOTION_EziMotionLink2_DEFINE.h"

#include "MOTION_EziSERVO2_DEFINE.h"
#include "MOTION_SSERVO_DEFINE.h"
#include "MOTION_EziSERVO2_86_DEFINE.h"
#include "MOTION_EziSTEP2_DEFINE.h"

#include "MOTION_EziSERVO_ALL_28_DEFINE.h"
#include "MOTION_EziSERVO_ALL_28_V2_DEFINE.h"

//------------------------------------------------------------------
//                 Device Type Defines.
//------------------------------------------------------------------
#ifndef DEVTYPE_EZI_SERVO_PLUS_R_ST
#define	DEVTYPE_EZI_SERVO_PLUS_R_ST				1
#define DEVNAME_EZI_SERVO_PLUS_R_ST				"Ezi-SERVO Plus-R-ST"
#endif

#ifndef DEVTYPE_EZI_SERVO_PLUS_R_ADC_ST
#define	DEVTYPE_EZI_SERVO_PLUS_R_ADC_ST			3
#define DEVNAME_EZI_SERVO_PLUS_R_ADC_ST			"Ezi-SERVO Plus-R-ADC"
#endif

#ifndef DEVTYPE_EZI_MOTIONLINK
#define	DEVTYPE_EZI_MOTIONLINK					10
#define DEVNAME_EZI_MOTIONLINK					"Ezi-MotionLink"
#endif

#ifndef DEVTYPE_EZI_MOTIONLINK2
#define	DEVTYPE_EZI_MOTIONLINK2					11
#define DEVNAME_EZI_MOTIONLINK2					"Ezi-MotionLink2"
#endif

#ifndef DEVTYPE_EZI_STEP_PLUS_R_ST
#define	DEVTYPE_EZI_STEP_PLUS_R_ST				20
#define DEVNAME_EZI_STEP_PLUS_R_ST				"Ezi-STEP Plus-R-ST"
#endif

#ifndef DEVTYPE_EZI_SERVO_PLUS_R_ALL_ST
#define	DEVTYPE_EZI_SERVO_PLUS_R_ALL_ST			30
#define DEVNAME_EZI_SERVO_PLUS_R_ALL_ST			"Ezi-SERVO Plus-R-ALL"
#endif

#ifndef DEVTYPE_EZI_SERVO_PLUS_R_ALL_ABS
#define	DEVTYPE_EZI_SERVO_PLUS_R_ALL_ABS		35
#define DEVNAME_EZI_SERVO_PLUS_R_ALL_ABS		"Ezi-SERVO Plus-R-ALL Abs"
#endif

#ifndef DEVTYPE_EZI_STEP_PLUS_R_ALL_ST
#define	DEVTYPE_EZI_STEP_PLUS_R_ALL_ST			40
#define DEVNAME_EZI_STEP_PLUS_R_ALL_ST			"Ezi-STEP Plus-R-ALL"
#endif

#ifndef DEVTYPE_EZI_SERVO_PLUS_R_MINI
#define	DEVTYPE_EZI_SERVO_PLUS_R_MINI			50
#define DEVNAME_EZI_SERVO_PLUS_R_MINI			"Ezi-SERVO Plus-R-Mini"
#endif

#ifndef DEVTYPE_EZI_STEP_PLUS_R_MINI
#define	DEVTYPE_EZI_STEP_PLUS_R_MINI			60
#define DEVNAME_EZI_STEP_PLUS_R_MINI			"Ezi-STEP Plus-R-Mini"
#endif

#ifndef DEVTYPE_EZI_SERVO_PLUS_R_ALL60I
#define	DEVTYPE_EZI_SERVO_PLUS_R_ALL60I			31
#define DEVNAME_EZI_SERVO_PLUS_R_ALL60I			"Ezi-SERVO Plus-R-ALL 60i"
#endif

#ifndef DEVTYPE_EZI_SERVO2_PLUS_R_ST
#define	DEVTYPE_EZI_SERVO2_PLUS_R_ST			100
#define DEVNAME_EZI_SERVO2_PLUS_R_ST			"Ezi-SERVO II Plus-R-ST"
#endif

#ifndef DEVTYPE_S_SERVO_PLUS_R_ST
#define	DEVTYPE_S_SERVO_PLUS_R_ST				102
#define DEVNAME_S_SERVO_PLUS_R_ST				"S-SERVO Plus-R-ST"
#endif

#ifndef DEVTYPE_EZI_SERVO2_PLUS_E_ST
#define	DEVTYPE_EZI_SERVO2_PLUS_E_ST			100
#define DEVNAME_EZI_SERVO2_PLUS_E_ST			"Ezi-SERVO II Plus-E-ST"
#endif

#ifndef DEVTYPE_EZI_SERVO2_PLUS_E_ST_86
#define	DEVTYPE_EZI_SERVO2_PLUS_E_ST_86			101
#define DEVNAME_EZI_SERVO2_PLUS_E_ST_86			"Ezi-SERVO II Plus-E-ST-86"
#endif

#ifndef DEVTYPE_EZI_SERVO2_PLUS_E_ALL60
#define	DEVTYPE_EZI_SERVO2_PLUS_E_ALL60			90
#define DEVNAME_EZI_SERVO2_PLUS_E_ALL60			"Ezi-SERVO II Plus-E-ALL60"
#endif

#ifndef DEVTYPE_S_SERVO_PLUS_E_ST
#define	DEVTYPE_S_SERVO_PLUS_E_ST				102
#define DEVNAME_S_SERVO_PLUS_E_ST				"S-SERVO Plus-E-ST"
#endif

#ifndef DEVTYPE_EZI_SERVO_PLUS_R_ALL_28
#define	DEVTYPE_EZI_SERVO_PLUS_R_ALL_28			105
#define DEVNAME_EZI_SERVO_PLUS_R_ALL_28			"Ezi-SERVO Plus-R ALL-28"
#endif

#ifndef DEVTYPE_EZI_SERVO_PLUS_R_ALL_28_V2
#define	DEVTYPE_EZI_SERVO_PLUS_R_ALL_28_V2		108
#define DEVNAME_EZI_SERVO_PLUS_R_ALL_28_V2		"Ezi-SERVO Plus-R ALL-28 V2"
#endif

#ifndef DEVTYPE_EZI_SERVO2_CC_LINK_ST
#define	DEVTYPE_EZI_SERVO2_CC_LINK_ST			120
#define DEVNAME_EZI_SERVO2_CC_LINK_ST			"Ezi-SERVO II CC-LINK-ST"
#endif

#ifndef DEVTYPE_EZI_STEP2_PLUS_R_ST
#define	DEVTYPE_EZI_STEP2_PLUS_R_ST				130
#define DEVNAME_EZI_STEP2_PLUS_R_ST				"Ezi-STEP II Plus-R-ST"
#endif

#ifndef DEVTYPE_EZI_STEP2_PLUS_E_ST
#define	DEVTYPE_EZI_STEP2_PLUS_E_ST				130
#define DEVNAME_EZI_STEP2_PLUS_E_ST				"Ezi-STEP II Plus-E-ST"
#endif

//------------------------------------------------------------------
//                 Device Type Defines. (for V8)
//------------------------------------------------------------------
// Driver Type A group : Ezi-SERVO Plus-R Inc. Family
#ifndef DEVTYPEV8_EZI_SERVO_PLUS_R_ST
#define	DEVTYPEV8_EZI_SERVO_PLUS_R_ST			1
#define DEVNAMEV8_EZI_SERVO_PLUS_R_ST			"Ezi-SERVO Plus-R Inc."
#endif

// Driver Type B group : only Motion Controller (without Drive) family
#ifndef DEVTYPEV8_EZI_MOTIONLINK
#define	DEVTYPEV8_EZI_MOTIONLINK				10
#define DEVNAMEV8_EZI_MOTIONLINK				"Ezi-MotionLink"
#endif

// Driver Type C group : Ezi-STEP Plus-R Family
#ifndef DEVTYPEV8_EZI_STEP_PLUS_R_ST
#define	DEVTYPEV8_EZI_STEP_PLUS_R_ST			20
#define DEVNAMEV8_EZI_STEP_PLUS_R_ST			"Ezi-STEP Plus-R"
#endif

// Driver Type D group : Ezi-SERVO Plus-R Abs. Family
#ifndef DEVTYPEV8_EZI_SERVO_PLUS_R_ABS_ST
#define	DEVTYPEV8_EZI_SERVO_PLUS_R_ABS_ST		30
#define DEVNAMEV8_EZI_SERVO_PLUS_R_ABS_ST		"Ezi-SERVO Plus-R Abs."
#endif

// Driver Type E group : Ezi-SERVO Plus-R BLDC Family
#ifndef DEVTYPEV8_EZI_SERVO_PLUS_R_BLDC_ST
#define	DEVTYPEV8_EZI_SERVO_PLUS_R_BLDC_ST		40
#define DEVNAMEV8_EZI_SERVO_PLUS_R_BLDC_ST		"Ezi-SERVO Plus-R BLDC F60"
#endif

// Driver Type F group : only Motion Controller (without Drive) family
#ifndef DEVTYPEV8_EZI_MOTIONLINK2
#define	DEVTYPEV8_EZI_MOTIONLINK2				110
#define DEVNAMEV8_EZI_MOTIONLINK2				"Ezi-MotionLink2"
#endif

//------------------------------------------------------------------
//                 Device Type Defines. (Ezi-IO)
//------------------------------------------------------------------

#ifndef DEVTYPE_EZI_IO_PLUS_R_IN16
#define	DEVTYPE_EZI_IO_PLUS_R_IN16			150
#define DEVNAME_EZI_IO_PLUS_R_IN16			"Ezi-IO Plus-R-IN16"
#endif

#ifndef DEVTYPE_EZI_IO_PLUS_R_OUT16
#define	DEVTYPE_EZI_IO_PLUS_R_OUT16			160
#define DEVNAME_EZI_IO_PLUS_R_OUT16			"Ezi-IO Plus-R-OUT16"
#endif


#ifndef DEVTYPE_EZI_IO_PLUS_R_I8O8
#define	DEVTYPE_EZI_IO_PLUS_R_I8O8			155
#define DEVNAME_EZI_IO_PLUS_R_I8O8			"Ezi-IO Plus-R-I8O8"
#endif
 

#ifndef DEVTYPE_EZI_IO_PLUS_E_IN16
#define	DEVTYPE_EZI_IO_PLUS_E_IN16			150
#define DEVNAME_EZI_IO_PLUS_E_IN16			"Ezi-IO Plus-E-IN16"
#endif

#ifndef DEVTYPE_EZI_IO_PLUS_E_IN32
#define	DEVTYPE_EZI_IO_PLUS_E_IN32			151
#define DEVNAME_EZI_IO_PLUS_E_IN32			"Ezi-IO Plus-E-IN32"
#endif

#ifndef DEVTYPE_EZI_IO_PLUS_E_OUT16
#define	DEVTYPE_EZI_IO_PLUS_E_OUT16			160
#define DEVNAME_EZI_IO_PLUS_E_OUT16			"Ezi-IO Plus-E-OUT16"
#endif

#ifndef DEVTYPE_EZI_IO_PLUS_E_OUT32
#define	DEVTYPE_EZI_IO_PLUS_E_OUT32			161
#define DEVNAME_EZI_IO_PLUS_E_OUT32			"Ezi-IO Plus-E-OUT32"
#endif

#ifndef DEVTYPE_EZI_IO_PLUS_E_I8O8
#define	DEVTYPE_EZI_IO_PLUS_E_I8O8			155
#define DEVNAME_EZI_IO_PLUS_E_I8O8			"Ezi-IO Plus-E-I8O8"
#endif

#ifndef DEVTYPE_EZI_IO_PLUS_E_I16O16
#define	DEVTYPE_EZI_IO_PLUS_E_I16O16		156
#define DEVNAME_EZI_IO_PLUS_E_I16O16		"Ezi-IO Plus-E-I16O16"
#endif


//------------------------------------------------------------------
//                 Device Type Defines. (Etc)
//------------------------------------------------------------------
#define DEVTYPE_BOOT_ROM						0xFF
#define DEVTYPE_BOOT_ROM_2						0xFE

//------------------------------------------------------------------
//                 Motion Direction Defines.
//------------------------------------------------------------------

static const int	DIR_INC = 1;
static const int	DIR_DEC = 0;

//------------------------------------------------------------------
//                 Axis Status Flag Defines.
//------------------------------------------------------------------

static const int	MAX_AXIS_STATUS		= 32;

//------------------------------------------------------------------
//           GetAllStatusEx Function
//------------------------------------------------------------------

#define ALLSTATUSEX_ITEM_COUNT		12

#define STATUSEX_TYPE_NONE			0
#define STATUSEX_TYPE_INPUT			1
#define STATUSEX_TYPE_OUTPUT		2
#define STATUSEX_TYPE_AXISSTATUS	3
#define STATUSEX_TYPE_CMDPOS		4
#define STATUSEX_TYPE_ACTPOS		5
#define STATUSEX_TYPE_ACTVEL		6
#define STATUSEX_TYPE_POSERR		7
#define STATUSEX_TYPE_PTNO			8
#define STATUSEX_TYPE_ALARMTYPE		9
#define STATUSEX_TYPE_TEMPERATURE	10
#define STATUSEX_TYPE_CURRENT		11
#define STATUSEX_TYPE_LOAD			12
#define STATUSEX_TYPE_PEAKLOAD		13
#define STATUSEX_TYPE_ENCVEL		14
#define STATUSEX_TYPE_INPUT_HIGH	15
#define STATUSEX_TYPE_PTNO_RUNNING	16

//------------------------------------------------------------------
//                 Input/Output Assigning Defines.
//------------------------------------------------------------------

static const BYTE	LEVEL_LOW_ACTIVE	= 0;
static const BYTE	LEVEL_HIGH_ACTIVE	= 1;

static const BYTE	IN_LOGIC_NONE	= 0;
static const BYTE	OUT_LOGIC_NONE	= 0;

//------------------------------------------------------------------
//                 POSITION TABLE Defines.
//------------------------------------------------------------------

static const WORD	MAX_LOOP_COUNT	= 100;
static const WORD	MAX_WAIT_TIME	= 60000;

static const WORD	PUSH_RATIO_MIN	= 20;
static const WORD	PUSH_RATIO_MAX	= 90;

static const DWORD	PUSH_SPEED_MIN	= 1;
static const DWORD	PUSH_SPEED_MAX	= 100000;

static const DWORD	PUSH_PULSECOUNT_MIN	= 1;
static const DWORD	PUSH_PULSECOUNT_MAX	= 10000;

typedef enum
{
	CMD_ABS_LOWSPEED = 0,
	CMD_ABS_HIGHSPEED,
	CMD_ABS_HIGHSPEEDDECEL,
	CMD_ABS_NORMALMOTION,
	CMD_INC_LOWSPEED,
	CMD_INC_HIGHSPEED,
	CMD_INC_HIGHSPEEDDECEL,
	CMD_INC_NORMALMOTION,
	CMD_MOVE_ORIGIN,
	CMD_COUNTERCLEAR,
	CMD_PUSH_ABSMOTION,
	CMD_STOP,

	CMD_MAX_COUNT,

	CMD_NO_COMMAND = 0xFFFF,
} COMMAND_LIST;

#ifndef	DEFINE_ITEM_NODE
#define DEFINE_ITEM_NODE

#pragma pack(2)

typedef union
{
	WORD	wBuffer[32];		// 64 bytes

	struct
	{
		LONG	lPosition;

		DWORD	dwStartSpd;
		DWORD	dwMoveSpd;

		WORD	wAccelRate;
		WORD	wDecelRate;

		WORD	wCommand;
		WORD	wWaitTime;
		WORD	wContinuous;
		WORD	wBranch;

		WORD	wCond_branch0;
		WORD	wCond_branch1;
		WORD	wCond_branch2;
		WORD	wLoopCount;
		WORD	wBranchAfterLoop;
		WORD	wPTSet;
		WORD	wLoopCountCLR;

		WORD	bCheckInpos;		// 0 : Check Inpos, 1 : Don't Check.

		LONG	lTriggerPos;
		WORD	wTriggerOnTime;

		WORD	wPushRatio;
		DWORD	dwPushSpeed;
		LONG	lPushPosition;
		WORD	wPushMode;
	};
} ITEM_NODE, *LPITEM_NODE;

#pragma pack()

#endif

static const WORD	OFFSET_POSITION		= 0;
static const WORD	OFFSET_LOWSPEED		= 4;
static const WORD	OFFSET_HIGHSPEED	= 8;
static const WORD	OFFSET_ACCELTIME	= 12;
static const WORD	OFFSET_DECELTIME	= 14;
static const WORD	OFFSET_COMMAND		= 16;
static const WORD	OFFSET_WAITTIME		= 18;
static const WORD	OFFSET_CONTINUOUS	= 20;
static const WORD	OFFSET_JUMPTABLENO	= 22;
static const WORD	OFFSET_JUMPPT0		= 24;
static const WORD	OFFSET_JUMPPT1		= 26;
static const WORD	OFFSET_JUMPPT2		= 28;
static const WORD	OFFSET_LOOPCOUNT		= 30;
static const WORD	OFFSET_LOOPJUMPTABLENO	= 32;
static const WORD	OFFSET_PTSET			= 34;
static const WORD	OFFSET_LOOPCOUNTCLEAR	= 36;
static const WORD	OFFSET_CHECKINPOSITION	= 38;
static const WORD	OFFSET_TRIGGERPOSITION	= 40;
static const WORD	OFFSET_TRIGGERONTIME	= 44;
static const WORD	OFFSET_PUSHRATIO		= 46;
static const WORD	OFFSET_PUSHSPEED		= 48;
static const WORD	OFFSET_PUSHPOSITION		= 52;
static const WORD	OFFSET_PUSHMODE			= 56;

static const WORD	OFFSET_BLANK			= 58;

//------------------------------------------------------------------
//                 EX Commands Option Defines.
//------------------------------------------------------------------
#pragma pack(1)

typedef union
{
	BYTE	byBuffer[32];

	struct
	{
		union
		{
			DWORD dwOptionFlag;
			struct
			{
				unsigned BIT_IGNOREEXSTOP	: 1;

				unsigned BIT_USE_CUSTOMACCEL	: 1;
				unsigned BIT_USE_CUSTOMDECEL	: 1;

				//unsigned BITS_RESERVED	: 13;
			};
		} flagOption;

		WORD	wCustomAccelTime;
		WORD	wCustomDecelTime;

		//BYTE	buffReserved[24];
	};
} MOTION_OPTION_EX;

typedef union
{
	BYTE	byBuffer[32];

	struct
	{
		union
		{
			DWORD dwOptionFlag;
			struct
			{
				unsigned BIT_IGNOREEXSTOP	: 1;
				unsigned BIT_USE_CUSTOMACCDEC	: 1;

				//unsigned BITS_RESERVED	: 14;
			};
		} flagOption;

		WORD	wCustomAccDecTime;

		//BYTE	buffReserved[26];
	};
} VELOCITY_OPTION_EX;

#pragma pack()

//------------------------------------------------------------------
//                 Alarm Type Defines.
//------------------------------------------------------------------
typedef enum
{
	ALARM_NONE = 0,

	/*  1 */ ALARM_OVERCURRENT,
	/*  2 */ ALARM_OVERSPEED,
	/*  3 */ ALARM_STEPOUT,
	/*  4 */ ALARM_OVERLOAD,
	/*  5 */ ALARM_OVERTEMPERATURE,
	/*  6 */ ALARM_OVERBACKEMF,
	/*  7 */ ALARM_MOTORCONNECT,
	/*  8 */ ALARM_ENCODERCONNECT,
	/*  9 */ ALARM_LOWMOTORPOWER,
	/* 10 */ ALARM_INPOSITION,
	/* 11 */ ALARM_SYSTEMHALT,
	/* 12 */ ALARM_ROMDEVICE,
	/* 13 */ ALARM_RESERVED0,
	/* 14 */ ALARM_HIGHINPUTVOLTAGE,
	/* 15 */ ALARM_POSITIONOVERFLOW,
	/* 16 */ ALARM_POSITIONCHANGED,

	MAX_ALARM_NUM
} ALARM_TYPE;

static LPCTSTR ALARM_DESCRIPTION[MAX_ALARM_NUM] =
{
	NULL,
	/*  1 */ _T("Over Current"),
	/*  2 */ _T("Over Speed"),
	/*  3 */ _T("Position Tracking"),
	/*  4 */ _T("Over Load"),
	/*  5 */ _T("Over Temperature"),
	/*  6 */ _T("Over Back EMF"),
	/*  7 */ _T("No Motor Connect"),
	/*  8 */ _T("No Encoder Connect"),
	/*  9 */ _T("Low Motor Power"),
	/* 10 */ _T("Inposition Error"),
	/* 11 */ _T("System Halt"),
	/* 12 */ _T("ROM Device Error"),
	/* 13 */ NULL,
	/* 14 */ _T("High Input Voltage"),
	/* 15 */ _T("Position Overflow"),
	/* 16 */ _T("Position Changed")
};

#define MAX_ALARM_LOG		(30)

typedef struct
{
	BYTE nAlarmCount;
	BYTE nAlarmLog[MAX_ALARM_LOG];
} ALARM_LOG;

//------------------------------------------------------------------
//                 Drive Information Defines.
//------------------------------------------------------------------
#pragma pack(1)

typedef struct _DRIVE_INFO
{
	unsigned short nVersionNo[4];	// Drive Version Number (Major Ver/Minor Ver/Bug fix/Build No.) (?)
	char sVersion[30];				// Drive Version string

	unsigned short nDriveType;		// Drive Model
	unsigned short nMotorType;		// Motor Model
	char sMotorInfo[20];			// Motor Info.(?)

	unsigned short nInPinNo;		// Input Pin Number
	unsigned short nOutPinNo;		// Output Pin Number

	unsigned short nPTNum;			// Position Table Item Number

	unsigned short nFirmwareType;	// Firmware Type Information
} DRIVE_INFO;

#pragma pack()

//------------------------------------------------------------------
//                 I/O Module Defines.
//------------------------------------------------------------------

#pragma pack(2)

typedef union
{
	BYTE	byBuffer[12];

	struct
	{
		unsigned short	wPeriod;
		unsigned short	wReserved1;
		unsigned short	wOnTime;
		unsigned short	wReserved2;
		unsigned long	wCount;
	};
} TRIGGER_INFO;

#pragma pack()

//------------------------------------------------------------------
//                 Ez-IO Plus-AD Defines
//------------------------------------------------------------------

#define MAX_AD_CHANNEL		16

enum AD_RANGE
{
	ADRANGE_10_to_10 = 0,	//  -10V ~  +10V [2.441mV]
	ADRANGE_5_to_5,			//   -5V ~   +5V [1.22mV]
	ADRANGE_2_5_to_2_5,		// -2.5V ~ +2.5V [0.61mV]
	ADRANGE_0_to_10,		//    0V ~  +10V [1.22mV]
};

#pragma pack(1)

typedef union
{
	BYTE	byBuffer[48];

	struct DATA
	{
		char	range;
		short	rawdata;
		float	converted;
	} channel[16];
} AD_RESULT;

#pragma pack()

//------------------------------------------------------------------
//                 LOG Level Defines
//------------------------------------------------------------------

enum LOG_LEVEL
{
	LOG_LEVEL_COMM = 0,		// Communication Log only
	LOG_LEVEL_PARAM,		// Communication Log and parameter functions
	LOG_LEVEL_MOTION,		// Communication Log and parameter, motion, I/O functions
	LOG_LEVEL_ALL,			// Communication Log and all functions
};
