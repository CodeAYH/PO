#ifndef DefinesH
#define DefinesH

//#include <boost/shared_ptr>
#include <memory>
#include "VaComm.hpp"

#define readwrite_property(DATATYPE, READ, WRITE) __declspec(property(get=READ, put=WRITE)) DATATYPE
#define readonly_property(DATATYPE, READ) __declspec(property(get=READ)) DATATYPE

#define INC(x) (((x)==0xFFFFFFFF)?0:(x)++)

#define THREAD_WAIT_TIME				1

#define MAX_DIO_COUNT					64     //2023-03-09 256 -> 64
#define MAX_AO_COUNT					4
#define MAX_AI_COUNT					8

#define MAX_ALARM_COUNT					256
#define	MAX_CHAMBER_COUNT				4
#define MAX_LOGGER_COUNT				2
#define MAX_POINT_COUNT					8
#define MAX_LOGGERPOINT_COUNT			MAX_LOGGER_COUNT*MAX_POINT_COUNT
#define MAX_COLOR_COUNT					18
#define MAX_EVENT_COUNT					10

#define TCARD_COUNT						10

#define LANGUAGEINI						"Language.ini"
#define OVENDB							"VSOven.mdb"
#define PLASMADB						"VSPlasma.mdb"
#define ERRDEFDB						"ErrDef.mdb"
#define OVENSYSTEMINI					"VSOven.ini"
#define PLASMASYSTEMINI					"VSPlasma.ini"
#define PATTERN_FILE					"PATTERN_CM_%c"
#define REX_P250INI						"REX_P250.ini"

#define SEC_SYSTEM						"SYSTEM"
#define SEC_CHAMBERA					"CHAMBER A"
#define SEC_CHAMBERB					"CHAMBER B"
#define SEC_CHAMBERC					"CHAMBER C"
#define SEC_CHAMBERD					"CHAMBER D"

#define KEY_LANGUAGE					"Language"
#define KEY_CHAMBERCOUNT				"Chamber Count"
#define KEY_LOGGERCOUNT					"Logger Count"
#define KEY_LOGGERPOINTCOUNT			"Logger Point Count"
#define KEY_MAINTEMPPORT				"Main Temp. Port"
#define KEY_LOGGERPORT					"Logger Port"
#define KEY_TEMPLIMITPPORT				"Temp. Limit Port"
#define KEY_INVERTERPORT				"Inverter Port"		// 20170226 - InverterIG5A Add - jhlim
#define KEY_EMOOPTION					"EMO Option"
#define KEY_USEAUTOMATION				"Use Automation"
#define KEY_AUTOMATIONKIND				"Automation Kind"
#define KEY_CHAMBERTEMPLIMITOFFSET		"Chamber Temp. Limit Offset"
#define KEY_CHAMBERTEMPLIMITBIAS		"Chamber Temp. Limit Bias"
#define KEY_MAINTEMPTIMEOUTINT			"Main Temp. Timeout"
#define KEY_LIMITTIMEOUTINT				"Limit Timeout"
#define KEY_LOGGERTIMEOUTINT			"Logger Timeout"
#define KEY_INVERTERTIMEOUTINT			"Inverter Timeout"		// 20170226 - InverterIG5A Add - jhlim
#define KEY_GAS							"Gas"
#define KEY_USEGEM						"Use GEM"
#define KEY_MAINTEMPCTRLTYPE			"Main Temp. Controller Type"
#define KEY_LIMITCTRLTYPE				"Limit Controller Type"
#define KEY_RFGENPORT					"RF Generator Port"
// 20180208 - FTP (OFILM MES) - jhlim
#define KEY_FTP_DEVICEID				"OFilm FTP DeviceID"
#define KEY_FTP_IP						"OFilm FTP IP"
#define KEY_FTP_PORT					"OFilm FTP Port"
#define KEY_FTP_ID						"OFilm FTP ID"
#define KEY_FTP_PW						"OFilm FTP PW"
#define KEY_FTP_FOLDER					"OFilm FTP Folder"
// 20181029 - ScreenSaver (infineon) - jhlim
#define KEY_SCREENSAVER_USE				"Use ScreenSaver"
#define KEY_SCREENSAVER_TIME			"ScreenSaver Time"
#define KEY_USE_PRESSURE_SENSOR			"Use Pressure Sensor"
#define KEY_CHAMBER_PRESSURE_PORT		"Chamber Pressure Port"
#define KEY_CHAMBER_PRESSURE_TIMEOUT	"Chamber Pressure Timeout"
#define KEY_HEATER_OUTPUT_LIMIT			"Heater Output Limit"
#define KEY_DIO_TYPE					"DIO Type"
#define KEY_JOBEND_TIMER				"Job End Timer"
#define KEY_DOORLOCK_TEMP				"Door Lock Temp"		// 2020-12-16 Add 챔버온도 도어락
#define KEY_USE_REMOTE_CHECK			"Use Remote Check"
#define KEY_USE_CSV_DATA_SAVE           "Use Csv Data Save"
#define KEY_MACHINE_ID			        "Machine ID"

#define KOR_TAG							"_K"
#define ENG_TAG							"_E"
#define JPN_TAG							"_J"
#define CHN_TAG							"_C"

// 실수형을 문자형으로, 문자형을 실수형으로 변환하면 변환 오류가 발생함.
// ex) 0.1(문자열) --> 0.1000000000000134(실수형)
// 이를 없애기 위해 문자열로 변환할 때는 실수형에 10의 배수를 곱하고
// 정수형으로 변환한 다음, 정수형을 문자열로 변환한다.
// 반대의 경우에는 문자열을 정수형으로 변환한 다음 10의 배수로 나누어
// 실수형을 구한다.
#define DEFAULT_SCALEFACTOR				10

enum AUTOMATIONKIND
{
	SECS_GEM=0,
	AMKOR_ECIM,
	SFA_ATK,
	SCK_CIM,
	BARUN_MES,
	ITM_SEMICON,
	OFILM_MES,
	MINEBEA_MITSUMI,
	SCK_MES,
	TCARD_RECIPE_LOADING,
	HANA_CIM,
	FUJITSU_TCARD,
	SECS_GEM_AMD,
	SECS_GEM_COWELL,
	SECS_GEM_SCK,
	AUTOMATIONKIND_COUNT
};

enum LANGUAGE
{ KOR=0, ENG, CHN, LANG_COUNT };

enum USERLEVEL
{ LV_NONE=0, LV_OPERATOR, LV_ENGINEER, LV_SUPERVISOR, LV_DEVELOPER, LV_COUNT };

enum CHAMBERID
{ CHAMBER_A=0, CHAMBER_B, CHAMBER_C, CHAMBER_D, CHAMBER_COUNT };

enum COMPORT
{ COM1=1, COM2, COM3, COM4, COM5, COM6, COM7, COM8, COM9, COM10, COM11, COM12, COM13, COM14, COM15, COM16, COM17, COM18, COM19, COM20};

enum BUZZERTYPE
{ BUZZER_SOUND=0, BUZZER_SIREN  };

enum GASTYPE
{ N2=0, AIR };

enum EMOOPTION
{ KEEP_WORKING=0, STOP_WORKING };
// 20170406 - FCFC Unit - jhlim
enum FCFCUNITSEL
{ FCFC_OFF=0, FCFC_ON };
// 20170407 - DVF - jhlim
enum DVFSEL
{ DVF_OFF=0, DVF_ON };

enum STOPTYPE
{ NORMAL_STOP=0, FORCED_STOP, LOWPRESSURE_STOP, EMERGENCY_STOP, MDCOOLFAIL_STOP, O2FAIL_STOP, VACUUMLEVEL_STOP, LOCALMODE_STOP };

enum UP55XMODE
{ UP55X_RESET=0, UP55X_PROGRAM, UP55X_LOCAL, UP55X_REM, UP55X_MODE_COUNT };

enum DOORSTATE
{ DR_NONE=0, DR_OPENED, DR_CLOSED };
// 20190221 - Door Setting - jhlim
enum DAMPERDIRECTION
{ CCW=0, CW, DIRECTION_COUNT };

enum O2ANALYZER_POWER
{ OP_NONE=0, OP_OFF_AFTER_PURGE };

enum UP550EVENT
{
	ET_NONE=0,
	ET_TIMEEVENT1,
	ET_TIMEEVENT2,
	ET_TIMEEVENT3,
	ET_TIMEEVENT4,
	ET_TIMEEVENT5,
	ET_TIMEEVENT6,
	ET_TIMEEVENT7,
	ET_PVHIGH=21,
	ET_PVLOW,
	ET_PVHIGH_CLEAR,
	ET_PVLOW_CLEAR
};

enum PLASMA_RECIPE_TYPE
{
	CLEANING_PARAM = 0, MOTOR_PARAM, PLASMA_RECIPE
};

// 20170226 - InverterIG5A Add - jhlim
enum ST_INVERTER_STATUS
{
	SIS_MOTOR_STOP = 0,
 	SIS_MOTOR_RUN_FORWARD,
	SIS_MOTOR_RUN_BACKWARD,
	SIS_MOTOR_TRIP_FAULT,
	SIS_MOTOR_RUN_ACCEL,
	SIS_MOTOR_RUN_DEACCEL,
	SIS_MOTOR_SPEED_REACHED,
	SIS_MOTOR_DC_BRAKING,
	SIS_MOTOR_DURING_STOP,
	SIS_MOTOR_BRAKE_OPEN_SIGNAL,
 	SIS_MOTOR_RUN_FORWARD_CMD,
	SIS_MOTOR_RUN_BACKWARD_CMD,
	SIS_MOTOR_REM_RS,
	SIS_MOTOR_REM_FREQ,

};

// 20170226 - InverterIG5A Add - jhlim
enum ST_INVERTER_TIP_STATUS
{
	SITS_OCT = 0,
	SITS_OVT,
	SITS_EXT_A,
	SITS_EST,  		// BX
	SITS_COL,
	SITS_GFT,       // GROUND FAULT
	SITS_OHT,       // INVERTER OVERHEAT
	SITS_ETH,       // MOTOR OVERHEAT
	SITS_OLT,       // OVERLOAD
	SITS_HW_DIAG,
	SITS_EXT_B,
	SITS_EEP,      	// PARAMETER WRITE ERROR
	SITS_FAN,       // LOCK & OPEN ERROR
	SITS_PO,        // PHASE OPEN
	SITS_IOLT,
	SITS_LVT,

};

// 싱글톤 객체를 생성하기 위한 템플릿
// 하나의 객체만 필요하다면 이 템플릿을 상속받아 구현한다.
// 단, 멀티 쓰레드 환경일 경우 중복 생성될 수 있으므로 주의한다.
template <typename T>
class Singleton
{
private:
	static T *Instance;

protected:
	Singleton() {}
	virtual ~Singleton() {}

public:
	static T* GetInstance()
	{
		if( Instance == nullptr )
			Instance = new T;
		return Instance;
	}
	static void ReleaseInstance()
	{
		if( Instance != nullptr )
		{
			delete Instance;
			Instance = nullptr;
		}
	}
};
template <typename T> T* Singleton<T>::Instance = nullptr;

// STC : Start Code
// 0 - SSP Start
// 1 - PV Start (구배우선)
// 2 - PV Start (시간우선)
typedef struct _STC
{
	int SegCount;
	double Ssp;
	int Stc;
	double Wz[5];
	int WTm[5];
	int Rcy, Rst, Ren;

	_STC() { ZeroMemory(this, sizeof(_STC)); }
} STC;

// JC : Junction Code (0, 1, 2, 11, 21)
// 0 - OFF, 1 - HOLD, 2 - LOCAL
// 11~15 - WAIT(평평한 세그먼트에서 동작)
// 21~25 - WAIT(경사진 세그먼트에서 동작)
// WAIT JC의 끝자리는 Wait Zone의 번호와 대응한다.
// 즉, 11, 21 은 Wait Zone 1번(Wz1, WTm1)과 짝을 이룬다.
#define EVENT_COUNT					8	// 오리지널 UP55x 이벤트

typedef struct _SEG
{
	double Tsp;
	int Time;
	int TmRt;
	int Pid;
	int EvKind[8];
	int OnType[8];
	int OffPe[8];
	int Jc;

	_SEG() { ZeroMemory(this, sizeof(_SEG)); }
} SEG;

typedef struct _DAMPER
{
	int InPercent; 		// 흡입 댐퍼 각도 (퍼센트)
	int OutPercent;		// 배출 댐퍼 각도 (퍼센트)
} DAMPERINFO;

typedef struct _DOORSETTING
{
	int ReleaseStep;
	int ReleaseRPM;
	int LockStep;
	int LockRPM;
	int StepMotorRPM;

	_DOORSETTING() { ZeroMemory(this, sizeof(_DOORSETTING)); }
} DOORSETTING;

typedef struct _DAMPERSETTING
{
	int InPercent;
	int InRPM;
	int InMaxStep;
	int InDir;

	int OutPercent;
	int OutRPM;
	int OutMaxStep;
	int OutDir;

	_DAMPERSETTING() { ZeroMemory(this, sizeof(_DAMPERSETTING)); }
} DAMPERSETTING;

typedef struct _PTN
{
	bool IsLoaded;
	int TotalTime;
	double MaxTemp;
	int PtnNo;

	// 가압오븐용
	int PressOption;
	int DepressOption;
	double DepressLevel;

	// Discolor 방지 임시 기능 용
	bool UsePreventOxydation;
	double PreventOxydationStartPress;
	int PreventOxydationTime;

	// 가압오븐 진공펑션용
	int VacuumOption;
	int VacuumTemp;
	int VacuumRampupTime;
	int VacuumHoldingTime;

	// 진공오븐용
	int VacSP;
	int VacHighLimit;
	int VacLowLimit;
	int CuringVacuumLevel;
	int CoolingVacuumLevel;
	bool UseN2Purge;

	// 고온오븐 용
	int CureStartOption;
	int O2AnalyzerPowerOff;

    bool UseLocalMode;
    double LocalTemp;

	STC Stc;
	SEG Seg[99];
	DAMPERINFO Damper[99];
	double InverterFrequency[99];		// 인버터 주파수
	// 가압오븐 FCFC 용
	bool UseFumeCollector[99];

	_PTN() { ZeroMemory(this, sizeof(_PTN)); }
	void Empty() { ZeroMemory(this, sizeof(_PTN)); }
} PTN;

typedef struct _PTNEVENT
{
	String Name;
	int EventNo;
	String ParamName1;
	String ParamName2;
} PTNEVENT;

//---------------------------------------------------------------------------
struct SFA_LOTINFO
{
	String lotID;
	String cstID;

	SFA_LOTINFO(const String sLot = "", const String sCst = "")
		: lotID(sLot), cstID(sCst) {}

	SFA_LOTINFO& operator=(const SFA_LOTINFO& arg)
	{
		lotID 	= arg.lotID;
		cstID 	= arg.cstID;
		return *this;
	}
	bool operator==(const SFA_LOTINFO& arg) const
	{
		if( lotID == arg.lotID &&
			cstID == arg.cstID )
			return true;
		else
			return false;
	}
	bool operator!=(const SFA_LOTINFO& arg) const
	{
		if( this->operator==(arg) )
			return false;
		else
			return true;
	}
};

const int LOT_COUNT	= 20;
const int FTP_LOT_COUNT = 10;
typedef struct _LOTINFO
{
	bool UseQuantity;
	int TotalLotCount;
	int LotQuantity[LOT_COUNT];
	String LotID[LOT_COUNT];

	// 20181010 - Add to User ID Edit - mtcho
	String UserID;

	// 20180226 - FTP (OFILM MES) - jhlim
	String DeviceType[LOT_COUNT];

	_LOTINFO() { TotalLotCount = 0; UseQuantity = false; }
	String ToString()
	{
		String ret = "";

		for(int i=0; i<TotalLotCount; i++)
		{
			ret += LotID[i];
			if( i < TotalLotCount-1 )
				ret += ",";
		}
		return ret;
	}
	// 20180226 - FTP (OFILM MES) - jhlim
	String ToString_FTP()
	{
		String ret = "";
#if 0	// 20190828 - User ID Upload Exception - jhlim
		// 20181010 - Add to User ID Edit - mtcho
		ret = UserID;
		ret += ",";
#endif
		for(int i=0; i<TotalLotCount; i++)
		{
			ret += LotID[i];
			ret += ",";
			ret += DeviceType[i];
			ret += ",";
		}
		return ret;
	}
	// 20190828 - Data Initial - jhlim
	void Init()
	{
		UseQuantity = false;
		TotalLotCount = 0;
		UserID = "";

		for(int i=0; i<LOT_COUNT; i++)
		{
			LotQuantity[i] = 0;
			LotID[i] = "";
			DeviceType[i] = "";
		}
	}
	_LOTINFO& operator=(const _LOTINFO& arg)
	{
		TotalLotCount = arg.TotalLotCount;
		for(int i=0; i<TotalLotCount; i++)
		{
			UseQuantity = arg.UseQuantity;
			LotQuantity[i] = arg.LotQuantity[i];
			LotID[i] = arg.LotID[i];
			// 20180226 - FTP (OFILM MES) - jhlim
			DeviceType[i] = arg.DeviceType[i];
		}
		return *this;
	}
} LOTINFO;

typedef struct
{
	String RecipeName;
	TDateTime StartTime;	// 레시피 시작 시간
	TDateTime SegStartTime;	// 첫 세그먼트 시작 시간

	AnsiString MainTempFilename;
	AnsiString PressFilename;
	AnsiString LoggerFilename;
	AnsiString LimitFilename;
	AnsiString VacuumFilename;
	AnsiString HeaterFilename;
	AnsiString O2Filename;

	// 20170308 HWJ
	AnsiString LONGMainTempFilename[2];
	AnsiString LONGLoggerFilename;

	// for WLFO
	AnsiString WLFOMainHeaterFilename[2];
	AnsiString WLFOLimitFilename[2];
	AnsiString WLFOPreHeaterFilename;
	AnsiString WLFOPreHeaterLimitFilename;
	AnsiString WLFOOxygenFilename;
	AnsiString WLFOLoggerFilename;

	int StopType;
	bool JobEnd;
	bool IsGemRecipe;

	PTN TempPtn;
	PTN PressPtn;			// 가압오븐용
	LOTINFO lotInfo;

	// 가압오븐 용
	TDateTime PressingStartTime;
	TDateTime PressingEndTime;
	TDateTime DepressingStartTime;
	TDateTime DepressingEndTime;
} RECIPE;

typedef struct _PORTSETTING
{
	unsigned char baudrate;
	unsigned char databits;
	unsigned char stopbits;
	unsigned char parity;

	bool operator==(const _PORTSETTING& arg) const
	{
		if( baudrate == arg.baudrate &&
			databits == arg.databits &&
			stopbits == arg.stopbits &&
			parity == arg.parity )
			return true;
		return false;
	}
} PORTSETTING;

typedef struct _PORTINFO
{
	PORTSETTING portSetting;
	int portNo;
	int addr;
	int timeout;

	bool operator==(const _PORTINFO& arg) const
	{
		if( portSetting == arg.portSetting &&
			portNo == arg.portNo &&
			addr == arg.addr &&
			timeout == arg.timeout )
			return true;
		return false;
	}
} PORTINFO;

typedef enum { MES_ACTIVE = 0, MES_PASIVE } MES_CONNECTION_MODE;
typedef enum { MES_LOCAL = 0, MES_REMOTE } ONLINE_MODE;

enum { CMD_UNKNOWN=0, CMD_TRUE, CMD_FALSE };
enum { LSTS_READY = 0, LSTS_LOT_RESERVED, LSTS_STARTED, LSTS_END, LSTS_MAX };
const String sLotStatus[LSTS_MAX]
	= {"LSTS_READY", "LSTS_LOT_RESERVED", "LSTS_STARTED", "LSTS_END"};

struct MES_PARAM
{
	String HostIP;
	String MachineID;
	int Port;
	int ReserveMode; // Common BCR, Each BCR
	int ReportCycle; // Report cycle
	int LinkTestTerm;
	int RetryTerm; // 재접속 시도 시간
	bool AutoConnection;
	ONLINE_MODE OnlineMode; // Online Mode false -> LOCAL or true -> REMOTE
	MES_CONNECTION_MODE ConnectionMode; // MES_ACTIVE = 0, MES_PASIVE

	MES_PARAM() { ZeroMemory(this, sizeof(MES_PARAM)); }

	MES_PARAM& operator=(const MES_PARAM& cVal)
	{
		HostIP = cVal.HostIP;
		Port	= cVal.Port;
		MachineID = cVal.MachineID;
		ReserveMode = cVal.ReserveMode;
		AutoConnection = cVal.AutoConnection;
		ReportCycle = cVal.ReportCycle;
		OnlineMode = cVal.OnlineMode;

		LinkTestTerm = cVal.LinkTestTerm;
		RetryTerm = cVal.RetryTerm;
		ConnectionMode = cVal.ConnectionMode;

		return *this;
	}

	bool operator==(const MES_PARAM& arg) const
	{
		if( HostIP == arg.HostIP &&
			Port == arg.Port &&
			MachineID == arg.MachineID &&
			ReserveMode == arg.ReserveMode &&
			AutoConnection == arg.AutoConnection &&
			ReportCycle == arg.ReportCycle &&
			OnlineMode == arg.OnlineMode &&
			LinkTestTerm == arg.LinkTestTerm &&
			RetryTerm == arg.RetryTerm &&
			ConnectionMode == arg.ConnectionMode )
			return true;
		return false;
	}
};

struct MES_MSG_RESULT
{
	int MsgKind;
	UINT Result;
	String ErrorMsg;

	MES_MSG_RESULT() { Init(); }

	void Init() { MsgKind = -1; ErrorMsg = ""; Result = CMD_UNKNOWN; }
};

typedef struct _MAINTEMPDATA
{
	int OpMode;
	int PtnNo;
	int SegNo;
	int TotalSeg;
	int RemainingTime;
	int PV1Error;
	int PV;
	int SP;
	int HeaterOutput;
	int DIStatus;
	int TimeEventStatus;
	int Mod1;
	// 20181218 - DVF func Add - jhlim
	int SelPIDZone;
	int SelPIDNo;
	// 20191119 - YJYOO - 히터 출력 최대 리미트 설정
	int OutputLimit;

	bool IsHold;
	bool PVEventHigh;
	bool PVEventLow;
	bool RunResetKeylock;
	bool ModeKeylock;
	bool IsAuto;

	_MAINTEMPDATA() { ZeroMemory(this, sizeof(_MAINTEMPDATA)); }
} MAINTEMPDATA;

// 20191011 YJYOO - 한영넉스 VX Series 추가로 인한 이름 변경
// ST340DATA -> LIMITDATA
typedef struct _LIMITDATA
{
	int PV;
	int SP;

	_LIMITDATA() { ZeroMemory(this, sizeof(_LIMITDATA)); }
} LIMITDATA;

// 20170302 - ST570 Add - wjh
typedef struct _ST570DATA
{
	double NPV; // D0001 : NPV(Present Value) 현재 측정 값
	double NSP; // D0002 : NSP(Set Point)는 현재 설정 값
	double TSP; // D0003 : 목표값
	double SP_SL; // D0005 : 운전 중인 SP 번호
	double H_OUT; // D0007 : 가열측 출력량
	double NOW_STS; // D0010 : 운전 관련 상태 정보
	double ALM_STS; // D0014 : 알람 발생 정보
	double Error; // D0019 : 에러 발생 정보
	double PROC_TIME; // D0020 : 운전 시간 정보

	_ST570DATA()
	{
		ZeroMemory(this, sizeof(_ST570DATA));
	}
} ST570DATA;

typedef struct _SP540DATA
{
	int PV;
	int SP;
	int TSP;
	int HOUT;
	int NOWSTS;
	int ALMSTS;
	BOOL bTimeSignalOn;
	int PTNO;
	int SEGNO;
	int SEGCOUNT;
	int SEGRUNTIME;
	int SEGSETTIME;
	int LOCK;
	int AL1;
	int AL2;

	_SP540DATA() { ZeroMemory(this, sizeof(_SP540DATA)); }
} SP540DATA;

// 20170317 - InverterVFDB Add - jhlim
typedef struct _InverterVFDBDATA
{
	// Inverter 운전상태 모니터링 파라미터
	bool ErrorStatus;
	int ErrorNo;
	AnsiString ErrorInfo;
	AnsiString RunStatus;
	AnsiString RunDirection;
	int CmdFreq;
	int OutputFreq;
	int OutputCur;
	int OutputDCVolt;
	int OutputVolt;
	int OutputRPM;

	AnsiString strData;

	_InverterVFDBDATA() { ZeroMemory(this, sizeof(_InverterVFDBDATA)); }
} InverterVFDBDATA;

// 20180209 - FTP (OFILM MES) - jhlim
typedef struct _FTPConnectInfo
{
	String DeviceID;
	String Ftp_IP;
	int Ftp_PORT;
	String Ftp_ID;
	String Ftp_PW;
	String Ftp_FOLDER;

	_FTPConnectInfo() { ZeroMemory(this, sizeof(_FTPConnectInfo)); }
}FTPConnectInfo;

// 20191216 - SCK MES - jhlim
typedef struct _SCKMESInfo
{
	// LOTID/공정코드/레서피명/장비명
	String FileName;
	String LotID;
	String ProductCode;
	String RecipeName;
	String EquipName;

	void Init()
	{
		FileName = "";
		LotID = "";
		ProductCode = "";
		RecipeName = "";
		EquipName = "";
    }

	_SCKMESInfo() { ZeroMemory(this, sizeof(_SCKMESInfo)); }
}SCKMESInfo;


extern String LangTag;
extern String LangStr[LANG_COUNT];
extern String ChamberStr[CHAMBER_COUNT];
extern String LevelStr[LV_COUNT];
extern String OpmodeStr[UP55X_MODE_COUNT];
extern String StatusStr[];
extern PTNEVENT EventList[MAX_EVENT_COUNT];
extern String EventName[MAX_EVENT_COUNT];
extern unsigned int Colors[MAX_COLOR_COUNT];
extern String AutomationKindStr[AUTOMATIONKIND_COUNT];
// 20190221 - Door Setting - jhlim
extern String MotorDirectionStr[DIRECTION_COUNT];
// 20180209 - FTP (OFILM MES) - jhlim
extern FTPConnectInfo ftpConnectInfo;

#endif	// DefinesH
