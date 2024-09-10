//---------------------------------------------------------------------------

#ifndef GEMFormH
#define GEMFormH
//---------------------------------------------------------------------------
#include <time.h>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AdvPageControl.hpp"
#include <ComCtrls.hpp>
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "BaseGrid.hpp"
#include <Grids.hpp>
#include "AdvGlassButton.hpp"
#include "EZGEMLib_OCX.h"
#include <OleCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "AdvEdit.hpp"
#include "AdvGroupBox.hpp"
#include "AdvOfficeButtons.hpp"
#include "AdvUtil.hpp"

#include "GEMDefines.h"
#include <memory>
#include <boost/shared_ptr.hpp>
//---------------------------------------------------------------------------
enum { SB_LOG, SB_UP550, SB_LIMITCTRL, SB_GEM_CONN, SB_GEM_ENABLE, SB_GEM_ONLINE, SB_GEM_IP, SB_TIME };

///////////CONTROL STATE///////////////////

//typedef enum
//{
//	CONTROL_EQUIPMENT_OFFLINE 	= 1,
//	CONTROL_ATTEMPT_ONLINE    	= 2,
//	CONTROL_HOST_OFFLINE   	 	= 3,
//	CONTROL_ONLINE_LOCAL   		= 4,
//	CONTROL_ONLINE_REMOTE       = 5
//} CONTROL_STATE;
//
/////////////PROCESS STATE///////////////////
//typedef enum
//{
//	STATE_INIT = 0,
//	STATE_IDLE,
//	STATE_SETUP,
//	STATE_READY,
//	STATE_EXECUTING,
//	STATE_DONE
//} PROCESS_STATE;
//
//typedef enum
//{
//	DFFAULT_EQUIPMENT_OFFLINE = 1,
//	DEFAULT_ATTEMP_ONLINE  = 2,
//	DEFAULT_HOST_OFFLINE   = 3,
//	DEFAULT_ONLINE_LOCAL   = 4,
//	DEFAULT_ONLINE_REMOTE  = 5
//} DEFAULT_CONTROL_STATE;
////////////////////////////////////////////////
//typedef enum
//{
//	ITEM_LIST	=  0,		ITEM_BINARY	= 10,		ITEM_BOOLEAN= 11,
//	ITEM_ASCII	= 20,		ITEM_JIS8	= 21,		ITEM_INT8B	= 30,
//	ITEM_INT1B	= 31,		ITEM_INT2B	= 32,		ITEM_INT4B	= 34,
//	ITEM_FLOAT8B= 40,		ITEM_FLOAT4B= 44,		ITEM_UINT8B	= 50,
//	ITEM_UINT1B	= 51,		ITEM_UINT2B	= 52,		ITEM_UINT4B	= 54,
//	ITEM_NULL = -1
//} ITEMTYPE;

//호스트에서 S2F13으로 can read value
//////////ECID//////////////
//#define ECID_PORT		                3001
//#define ECID_DEVICEID	                3002
//#define ECID_T3			                3003
//#define ECID_T5			                3005
//#define ECID_T6			                3006
//#define ECID_T7			                3007
//#define ECID_T8			                3008
//#define ECID_LINKTEST	                3009
//#define ECID_RETRY		                3010
//
//#define ECID_DEFAULT_COMM_STATE		    3100
//#define ECID_DEFAULT_CONTROL_STATE	    3101
//#define ECID_ESTABLISH_TIMEOUT		    3102
//#define ECID_MDLN					    3103
//#define ECID_SOFTREV				    3104
//
//#define ECID_MAXSPOOL_TRANSMIT     	    3201
//#define ECID_SPOOL_OVERWRITE       	    3202
//#define ECID_SPOOLING_MODE	       	    3203
//#define ECID_MAX_SPOOL_SIZE	       	    3204
//#define ECID_TIME_FORMAT	       	    3300
////#define ECID_JOBINFO_OPCONFIRM	3301
////#define ECID_T9					   3320

//////////////////////////////////////////////

///////SVID/////
//#define SVID_COMMSTATE                  2001
//#define SVID_CONTROLSTATE               2002
//#define SVID_PREV_CONTROLSTATE          2003
//#define SVID_ALID                       2005
//#define SVID_ALARM_ENABLED              2007
//#define SVID_ALARM_DISABLED             2008
//#define SVID_ALARM_SET                  2009
//#define SVID_EVENT_ENABLED	            2010
//#define SVID_EVENT_DISABLED             2011
//#define SVID_CLOCK                      2012
//
//#define SVID_SPOOLCOUNT_TOTAL           2030
//#define SVID_SPOOLCOUNT_ACTUAL          2031
//#define SVID_SPOOL_FULLTIME             2032
//#define SVID_SPOOL_STARTTIME            2033
//#define SVID_SPOOL_STATE                2034
//
//#define SVID_ECID_CHANGED               2041
//#define SVID_ECV_CHANGED                2042
//
//#define SVID_PORT_STATUS                2061
//
//#define SVID_PROC_STATE_A               2100
//#define SVID_PREV_PROCSTATE_A           2101
//#define SVID_PROC_STATE_B               2110
//#define SVID_PREV_PROCSTATE_B           2111
//#define SVID_PROC_STATE_C               2120
//#define SVID_PREV_PROCSTATE_C           2121
//#define SVID_PROC_STATE_D               2130
//#define SVID_PREV_PROCSTATE_D           2131
//
//#define SVID_PPID_A						2200		// 2022-03-07
//#define SVID_PPID_B						2201
//#define SVID_PPID_C						2202
//#define SVID_PPID_D						2203
//#define SVID_PPID						2210
//#define SVID_PPID_CHANGE_NAME           2211
//#define SVID_PPID_CHANGE_STATUS         2212
//
//#define SVID_PORTID                     2300
//#define SVID_LOTID			            2310
//#define	SVID_USERID			            2320
//#define SVID_UNITID			            2330
//#define SVID_DOOR_STATUS_A	            2340		// 2021-11-02
//#define SVID_DOOR_STATUS_B	            2341
//#define SVID_DOOR_STATUS_C	            2342
//#define SVID_DOOR_STATUS_D	            2343
//
//#define SVID_TOTAL_SEG_COUNT_A			2400
//#define SVID_CURRENT_SEG_NO_A			2401
//#define SVID_TOTAL_CURE_TIME_A			2402
//#define	SVID_CURRENT_SP_A				2403
//#define SVID_CURRENT_SET_TIME_A			2404
//#define SVID_MAIN_TEMP_A		   	    2405
//#define SVID_DIFF_PRESSURE_A			2406
//#define SVID_O2_DENSITY_A				2407
//
//#define SVID_LOGGER1_A			        2420
//#define SVID_LOGGER2_A			        2421
//#define SVID_LOGGER3_A			        2422
//#define SVID_LOGGER4_A			        2423
//#define SVID_LOGGER5_A			        2424
//
//#define SVID_TOTAL_SEG_COUNT_B			2500
//#define SVID_CURRENT_SEG_NO_B			2501
//#define SVID_TOTAL_CURE_TIME_B			2502
//#define	SVID_CURRENT_SP_B				2503
//#define SVID_CURRENT_SET_TIME_B			2504
//#define SVID_MAIN_TEMP_B		   	    2505
//#define SVID_DIFF_PRESSURE_B			2506
//#define SVID_O2_DENSITY_B				2507
//
//#define SVID_LOGGER1_B			        2520
//#define SVID_LOGGER2_B			        2521
//#define SVID_LOGGER3_B			        2522
//#define SVID_LOGGER4_B			        2523
//#define SVID_LOGGER5_B			        2524
//
//#define SVID_TOTAL_SEG_COUNT_C			2600
//#define SVID_CURRENT_SEG_NO_C			2601
//#define SVID_TOTAL_CURE_TIME_C			2602
//#define	SVID_CURRENT_SP_C				2603
//#define SVID_CURRENT_SET_TIME_C			2604
//#define SVID_MAIN_TEMP_C		   	    2605
//#define SVID_DIFF_PRESSURE_C			2606
//#define SVID_O2_DENSITY_C				2607
//
//#define SVID_LOGGER1_C			        2620
//#define SVID_LOGGER2_C			        2621
//#define SVID_LOGGER3_C			        2622
//#define SVID_LOGGER4_C			        2623
//#define SVID_LOGGER5_C			        2624
//
//#define SVID_TOTAL_SEG_COUNT_D			2700
//#define SVID_CURRENT_SEG_NO_D			2701
//#define SVID_TOTAL_CURE_TIME_D			2702
//#define	SVID_CURRENT_SP_D				2703
//#define SVID_CURRENT_SET_TIME_D			2704
//#define SVID_MAIN_TEMP_D		   	    2705
//#define SVID_DIFF_PRESSURE_D			2706
//#define SVID_O2_DENSITY_D				2707
//
//#define SVID_LOGGER1_D			        2720
//#define SVID_LOGGER2_D			        2721
//#define SVID_LOGGER3_D			        2722
//#define SVID_LOGGER4_D			        2723
//#define SVID_LOGGER5_D			        2724
//#define SVID_RECIPE_PARAM				2725
//#define SVID_LOT_ID_LIST                2726
//
//#define PPCHANGESTATUS_ADDED			1
//#define PPCHANGESTATUS_CHANGED			2
//#define PPCHANGESTATUS_DELETED			3
//
///////CEID////
//#define CEID_OFFLINE                    1001
//#define CEID_LOCAL				        1002
//#define CEID_REMOTE				        1003
//#define CEID_PROCSTATE_CHANGE_A	        1010
//#define CEID_PROCSTATE_CHANGE_B	        1011
//#define CEID_PROCSTATE_CHANGE_C	        1012
//#define CEID_PROCSTATE_CHANGE_D	        1013
//#define CEID_JOB_START			        1014
//#define CEID_JOB_END		   	        1015
//#define CEID_JOB_CANCEL			        1016
//#define CEID_JOB_START_FAIL		        1017
//#define CEID_JOB_CANCEL_FAIL	        1018
//#define CEID_JOB_VALIDATION		        1019 //COWELL Add 2023-02-07
//
//#define CEID_ALARM_SET			        1025
//#define CEID_ALARM_CLEAR		        1026
//
//#define CEID_PP_CHANGED			        1051
//#define CEID_PP_SELECTED		        1052
//
//#define CEID_SPOOLSTATE_CHANGE          1070
//#define CEID_SPOOL_ACTIVE               1071
//#define CEID_SPOOL_INACTIVE             1072
//#define CEID_SPOOL_FAILED               1073
//
//#define CEID_ECID_CHANGED               1075
//
//#define CEID_HOSTMSG_ACK                1081
////#define CEID_LOAD_COMP                  1092
////#define CEID_UNLOAD_REQUEST             1093
////#define CEID_UNLOAD_COMP                1094
//#define CEID_BCR_READ                   1101
//#define CEID_BCR_READ_FAIL              1102
//#define CEID_CURRENT_CURE_DATA_A		1103
//#define CEID_CURRENT_CURE_DATA_B		1104
//#define CEID_CURRENT_CURE_DATA_C		1105
//#define CEID_CURRENT_CURE_DATA_D		1106
//
//#define CEID_DOOR_STATUS_A				1110		// 2021-11-02
//#define CEID_DOOR_STATUS_B				1111
//#define CEID_DOOR_STATUS_C				1112
//#define CEID_DOOR_STATUS_D				1113

//****SECS/GEM  ALARM CATEGORY***********************************
//#define PERSONAL_SAFETY      	        1
//#define EQUIP_SAFETY         	        2
//#define PARA_CONTROL_WARNING 	        4
//#define PARA_CONTROL_ERROR   	        8
//#define IRRECOVERABLE ERROR  	        16
//#define EQUIP_STATUS_WARNING 	        32
//#define ATTENTION_FLAG       	        64
////************************************
////---Error Code----------------------
//#define LICENSE_PROBLEM               -1   //라이센스(License)문제발생
//#define CONFIG_FILE_NOTFOUND          -101 //환경설정파일(Configuration File)이존재하지않음
//#define FILE_NOT_FOUND                -102 //파일이존재하지않음
//#define FILE_IO_FAILURE               -103 //파일입출력(I/O)  실패
//#define FILE_CREATION_ERROR           -104 //파일생성실패
//#define NO_ITEM                       -201 //아이템(Item)이존재하지않음
//#define TYPE_MISMATCH                 -202 //ITEM의타입이맞지않음
//#define SIZE_MISMATCH                 -203 //ITEM의크기/개수가맞지않음
//#define ITEM_I1_OVERFLOW              -204 //I1(1Byte Integer) Overflow 발생
//#define ITEM_I2_OVERFLOW              -205 //I2(2Byte Integer) Overflow 발생
//#define ITEM_I4_OVERFLOW              -206 //I4(4Byte Integer) Overflow 발생
//#define ITEM_U1_OVERFLOW              -207 //U1(1Byte Unsigned Integer) Overflow 발생
//#define ITEM_U2_OVERFLOW              -208 //U2(2Byte Unsigned Integer) Overflow 발생
//#define ITEM_U4_OVERFLOW              -209 //U4(4Byte Unsigned Integer) Overflow 발생
//#define INVALID_MSGID                 -210 //유효하지않은 Message ID
//#define INVALID_STREAM                -211 //유효하지않은Stream번호
//#define INVALID_FUNCTION              -212 //유효하지않은 Function 번호
//#define DUPLICATE_SYSTEMBYTE          -213 //시스템바이트의중복
//#define UNDEFINED_STRUCTURE           -214 //정의되지않은Message Structure 를수신
//#define NOT_CONNECTED                 -301 //아직연결이안된상태임
//#define LOW_LEVEL_ERROR               -302 //Operating System 상의  Error
//#define NOT_SUPPORTED                 -303 //지원되지 않는기능
//#define ALREADY_CONNECTED             -304 //이미연결되어 있음
//#define ALREADY_STATRED               -305 //이미 구동된 상태임
//#define THREAD_NULL                   -306 //각종 쓰레드(THREAD) 에러
//#define CREATE_EVENT_FAIL             -307 //이벤트(EVENT)발생 에러
//#define SERIAL_OPEN_FAIL              -308 //시리얼포트구동에러
//#define SERIAL_SETUP_FAIL             -309 //시리얼포트초기화에러
//#define TIMER_CREATE_FAIL             -310 //타이머 생성실패
//#define NOT_STARTED_YET               -311 //프로그램이구동되지않았음
//#define SOCKET_INUSE                  -501 //소켓(Socket)이이미사용중임69
//#define SOCKET_STARTERROR             -502 //소켓 구동 에러
//#define SOCKET_INVALID                -503 //소켓이유효하지않음
//#define SOCKET_WINDOWERROR            -504 //메시지통신을 위한 Worker Window 에러
//#define SOCKET_LOCALNAME              -505 //Host Name 을알아오는과정에서 에러발생
//#define SOCKET_CONNECTERROR           -506 //소켓접속에러
//#define SOCKET_SETEVENT               -507 //소켓이벤트설정에러
//#define SOCKET_RESOLVE                -508 //소켓함수중 Resolve()수행 중 에러
////////////////////////
class CHSMSPARA
{
public:
	CHSMSPARA()
	{
		sHostIP = "";
		ZeroMemory(this, sizeof(*this));
	}
	~CHSMSPARA() {}

	__property int nDeviceId	= {read = FnDeviceId,	write = SetDeviceId };
	__property int nPort		= {read = FnPort,		write = SetPort };
	__property int nLnkTest		= {read = FnLnkTest,	write = SetLnkTest };
	__property int nRetry		= {read = FnRetry,		write = SetRetry };
	__property int nT3			= {read = FnT3,			write = SetT3 };
	__property int nT5			= {read = FnT5,			write = SetT5 };
	__property int nT6			= {read = FnT6,			write = SetT6 };
	__property int nT7			= {read = FnT7,			write = SetT7 };
	__property int nT8			= {read = FnT8,			write = SetT8 };
	__property int nCTOut		= {read = FnCTOut,			write = SetCTOut };
	__property bool bPassiveMode	= {read = FbPassiveMode,	write = SetPassiveMode };
	__property AnsiString sHostIP	= {read = FsHostIP,		write = SetHostIP };

private:
	CHSMSPARA(const CHSMSPARA& rhs) {}

	int FnDeviceId;
	int FnPort;
	int FnLnkTest;
	int FnRetry;
	int FnT3;
	int FnT5;
	int FnT6;
	int FnT7;
	int FnT8;
	int FnCTOut;
	bool FbPassiveMode;
	AnsiString FsHostIP;

	void SetDeviceId(const int nVal)	{ FnDeviceId = nVal;}
	void SetPort(const int nVal) 		{ FnPort = nVal;	}
	void SetLnkTest(const int nVal)		{ FnLnkTest = nVal;	}
	void SetRetry(const int nVal) 		{ FnRetry = nVal;	}
	void SetT3(const int nVal) 			{ FnT3 = nVal;	}
	void SetT5(const int nVal) 			{ FnT5 = nVal;	}
	void SetT6(const int nVal) 			{ FnT6 = nVal;	}
	void SetT7(const int nVal) 			{ FnT7 = nVal;	}
	void SetT8(const int nVal) 			{ FnT8 = nVal;	}
	void SetCTOut(const int nVal) 		{ FnCTOut = nVal;	}
	void SetPassiveMode(const bool bVal)	{ FbPassiveMode = bVal;	}
	void SetHostIP(const AnsiString sVal)	{ FsHostIP = sVal;	}
};
//---------------------------------------------------------------------------
typedef boost::shared_ptr<CHSMSPARA> HsmsParaPtr;
//---------------------------------------------------------------------------

struct _SPOOLINFO
{
	bool bSpoolEnabled;
	UINT nMaxSpoolSize;
	UINT nMaxSpoolTransmit;
	bool bSpoolOverwrite;
};

class CECID_VAR
{
public:
	CECID_VAR()
	{
		ZeroMemory(this, sizeof(*this));
	}
	~CECID_VAR() {}

	_SPOOLINFO SPOOLINFO;

	// Equipment Constant
	__property UINT nDefaultCommState 	= {read = FnDefaultCommState, write = SetDefaultCommState};
	__property int  nDefaultControlState	= {read = FnDefaultControlState,	write = SetDefaultControlState 	};
	__property UINT nCommReqeustTimeout = {read = FnCommReqeustTimeout, write = SetCommReqeustTimeout};
	__property UINT nTimeFormat			= {read = FnTimeFormat,		write = SetTimeFormat	};
	__property bool	bJobInfoConfirm		= {read = FbJobInfoConfirm,	write = SetJobInfoConfirm};
	__property String sModelName		= {read = FsModelName, 		write = SetModelName	};
	__property String sSoftRev			= {read = FsSoftRev,		write = SetSoftRev		};

private:
	CECID_VAR(const CHSMSPARA& rhs) {}

	// Equipment Constant
	UINT FnDefaultCommState;
	int  FnDefaultControlState;
	UINT FnCommReqeustTimeout;
	UINT FnTimeFormat;	// 14, 16
	bool FbJobInfoConfirm;
	String FsModelName;
	String FsSoftRev;

	// Equipment Constant
	void SetDefaultCommState(const UINT nVal)	{ FnDefaultCommState = nVal;}
	void SetDefaultControlState(const int nVal)	{ FnDefaultControlState = nVal;	}
	void SetCommReqeustTimeout(const UINT nVal)	{ FnCommReqeustTimeout = nVal;}
	void SetTimeFormat(const UINT nVal)			{ FnTimeFormat = nVal;		}
	void SetJobInfoConfirm(const UINT nVal)		{ FbJobInfoConfirm = nVal;	}
	void SetModelName(const String sVal)		{ FsModelName = sVal;		}
	void SetSoftRev(const String sVal)			{ FsSoftRev = sVal;			}
};
//---------------------------------------------------------------------------
typedef boost::shared_ptr<CECID_VAR> EcidPtr;
//---------------------------------------------------------------------------

struct EZGEMSTATE
{
	EZGEMSTATE()
	{
		ZeroMemory(this, sizeof(*this));
	}

	///////// Control State
	int  nControlState;
	int  nPrevControlState;
	int  nLatestControlState;

	///////// Process State
	int  nProcessState;
	int  nPrevProcessState;
	int  nDispFg;
	int  nSoftChkCnt;
	int  nTime500msCnt;
	bool bConnectedFg;
	bool bDoorOpenFg;
	bool bEstablish;
	bool bEnabled;
};
//---------------------------------------------------------------------------

class TFmGem : public TForm
{
__published:	// IDE-managed Components
	TAdvPageControl *pcGem;
	TAdvTabSheet *Setup;
	TAdvTabSheet *Test;
	TAdvStringGrid *GemParaGrid;
	TAdvGlassButton *btnGemStart;
	TAdvGlassButton *btnGemStop;
	TAdvGlassButton *btnGemOffline;
	TAdvGlassButton *btnGemRemote;
	TAdvGlassButton *btnGemLocal;
	TAdvStringGrid *StatusGrid;
	TImage *NotConnectedImage;
	TTimer *S9F13Timer;
	TAdvEdit *edtTermMsg;
	TAdvOfficeRadioGroup *rdgSelCm;
	TAdvGlassButton *btnJobStart;
	TAdvGlassButton *btnJobEnd;
	TAdvGlassButton *btnJobCancel;
	TAdvGlassButton *btnBcr;
	TAdvEdit *edtBcr;
	TAdvGlassButton *btnSetAlarm;
	TAdvEdit *edtAlarmNo;
	TAdvOfficeRadioGroup *rdgSelState;
	TAdvEdit *edtRecipe;
	TAdvGlassButton *btnRecipeChange;
	TAdvGlassButton *btnPPSelect;
	TEZGEM *EzGem;
	TImage *ConnectedImage;
    void __fastcall FormCreate(TObject *Sender);
	void __fastcall GemParaGridGetEditorType(TObject *Sender, int ACol, int ARow, TEditorType &AEditor);
	void __fastcall GemParaGridGetAlignment(TObject *Sender, int ARow, int ACol, TAlignment &HAlign,
		  Advobj::TVAlignment &VAlign);
	void __fastcall GemParaGridGetEditMask(TObject *Sender, int ACol, int ARow, UnicodeString &Value);
	void __fastcall GemParaGridEditingDone(TObject *Sender);
	void __fastcall StatusGridCanClickCell(TObject *Sender, int ARow, int ACol, bool &Allow);
	void __fastcall StatusGridGetAlignment(TObject *Sender, int ARow, int ACol, TAlignment &HAlign,
          Advobj::TVAlignment &VAlign);
	void __fastcall GemParaGridButtonClick(TObject *Sender, int ACol, int ARow);
	void __fastcall EzGemConnected(TObject *Sender);
	void __fastcall EzGemDisconnected(TObject *Sender);
	void __fastcall EzGemCommEstablished(TObject *Sender);
	void __fastcall EzGemDateTimeSetRequest(TObject *Sender, long lMsgId, OLECHAR *strNewDateTime);
	void __fastcall EzGemEstablishCommRequest(TObject *Sender, long lMsgId);
	void __fastcall EzGemOfflineRequest(TObject *Sender, long lMsgId);
	void __fastcall EzGemOnlineLocal(TObject *Sender);
	void __fastcall EzGemOnlineRemote(TObject *Sender);
	void __fastcall EzGemOnlineRequest(TObject *Sender, long lMsgId);
	void __fastcall EzGemNewEquipConst(TObject *Sender, long lMsgId, short nCount);
	void __fastcall EzGemRemoteCommand(TObject *Sender, long lMsgId, OLECHAR *strCommand,
          short nParamCount);
	void __fastcall EzGemTerminalMessageMulti(TObject *Sender, long lMsgId, short nCode,
		  short nCount);
	void __fastcall EzGemTerminalMessageSingle(TObject *Sender, long lMsgId, OLECHAR *strCommand,
          short nParamCount);
	void __fastcall EzGemMsgRequested(TObject *Sender, long lMsgId);
	void __fastcall btnGemStartClick(TObject *Sender);
	void __fastcall btnGemStopClick(TObject *Sender);
	void __fastcall btnGemLocalClick(TObject *Sender);
	void __fastcall btnGemOfflineClick(TObject *Sender);
	void __fastcall btnGemRemoteClick(TObject *Sender);
	void __fastcall edtTermMsgKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall S9F13TimerTimer(TObject *Sender);
	void __fastcall rdgSelStateClick(TObject *Sender);
	void __fastcall btnSetAlarmClick(TObject *Sender);
	void __fastcall btnRecipeChangeClick(TObject *Sender);
	void __fastcall btnJobStartClick(TObject *Sender);
	void __fastcall btnJobEndClick(TObject *Sender);
	void __fastcall btnJobCancelClick(TObject *Sender);
	void __fastcall btnPPSelectClick(TObject *Sender);
	void __fastcall btnBcrClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);

private:	// User declarations
	String m_sPath;
	String m_sFileName;
	boost::shared_ptr<TCriticalSection> csLock;

	void __fastcall AlignComps();
	void __fastcall LoadGemPara();
	void __fastcall FillGemPara();
	void __fastcall SaveGemPara();
	bool __fastcall IsValidPara();

	HsmsParaPtr	HsmsPara;
	EcidPtr	Ecid;

	int m_nS9F13TimerDly;

	clock_t FClockStart;
	clock_t FClockEnd;
	clock_t FClockTemp;
	int FnWorkingChamber;

	short AlarmCatetory[100];

	int PrevDoorStatus[4];		// 2021-11-02

	void __fastcall SetClockStart(const clock_t clockVal) 	{	FClockStart = clockVal; }
	void __fastcall SetClockEnd(const clock_t clockVal)		{	FClockEnd = clockVal; }
	void __fastcall SetClockTemp(const clock_t clockVal)	{	FClockTemp = clockVal; }

	WideString GetBstr(const String sVal) const;

	void __fastcall AddSVIDs(void);
	void __fastcall AddCEIDs(void);
	void __fastcall AddALIDs(void);
	void __fastcall AddECIDs(void);
	void __fastcall AddRPTIDs(void);

//	void __fastcall OnS6F15W(long lMsgId);
	void __fastcall OnS7F1W(long lMsgId);
	void __fastcall OnS7F3W(long lMsgId);
	void __fastcall OnS7F5W(long lMsgId);
	void __fastcall OnS7F17W(long lMsgId);
	void __fastcall OnS7F19W(long lMsgId);

	void __fastcall SendS9F13(const String strMEXP, const String strEDID);
	void __fastcall UpdateECidPara(void);
	String __fastcall GetErrorCode(const short ret);
	void  __fastcall Save_Para(const AnsiString filepath);
	void __fastcall SetSVID(const long svid, const String sVal);

	bool __fastcall GetChamberNoAndRecipeNameFromPPID(String ppid, int &chamberNo, String &recipe);

	void __fastcall Init();

	void __fastcall MakeRecipeParamSvid(int chamberNo, int ptnNo, String ppid);
	void __fastcall MakeLotIDList(String lotid); // Cowell China Add

	short __fastcall RemoteCommandPpselect(int chamberNo, String strPPID);

public:		// User declarations
	EZGEMSTATE GemState[4];

	__fastcall TFmGem(TComponent* Owner);
	__fastcall ~TFmGem(void);

	__property clock_t ClockStart   = {read = FClockStart, 	write = SetClockStart };
	__property clock_t ClockEnd   	= {read = FClockEnd, 	write = SetClockEnd };
	__property clock_t ClockTemp   	= {read = FClockTemp, 	write = SetClockTemp };

	void __fastcall SetProcessState(const int chamberNo, const int nProcessState);
	void __fastcall SetAlarm(const int chamberNo, const int errCode);
	void __fastcall ClearAlarm(const int chamberNo, const int errCode);
	void __fastcall ClearAlarmAll(const int chamberNo);
	void __fastcall SendEventReport(const long CEID);
//	long __fastcall ErrToAlmCodeConvert(const int errCode);
	void __fastcall SendTerminalMsg(const String Str);
	void __fastcall DisplayTerminalMsg(const boost::shared_ptr<TStringList> slMsg);

	void __fastcall UpdateSVidPara(const int chamberNo);

	void __fastcall UdpateGemState();
	String __fastcall GetHostIP() { return HsmsPara->sHostIP; }

	void __fastcall SendBarcodeInfoEvent(const int chamberNo, const String barcode);
	void __fastcall SendJobEndEvent(const int chamberNo, const String lotid, const String ppid);
	void __fastcall SendJobStartedEvent(const int chamberNo, int ptnNo, const String lotid, const String ppid);
	void __fastcall SendJobStartFailEvent(const int chamberNo, const String lotid, const String ppid);
	void __fastcall SendJobCancelEvent(const int chamberNo, const String lotid, const String ppid);
	void __fastcall SendJobCancelFailEvent(const int chamberNo, const String lotid, const String ppid);
    void __fastcall SendJobValidation(const int chamberNo, const String lotid, const String ppid);
	void __fastcall SendPPSelectedEvent(const int chamberNo, int ptnNo, const String ppid);
	void __fastcall SendPPChangedEvent(const int chamberNo, int ptnNo, const String ppid, int changeStatus);
	void __fastcall SendDoorStatusEvent(const int chamberNo, const int doorStatus);		// 2021-11-02
	void __fastcall SendDoorEvent(const int chamberNo);
	void __fastcall SendOPIDStatusEvent(const String opid);

	bool __fastcall IsGemConnected();
	bool __fastcall IsOnlineRemote() { return (GemState->nControlState == CONTROL_ONLINE_REMOTE); }

	void __fastcall EnableAutoReply();
	void __fastcall DisableAutoReply();
};
//---------------------------------------------------------------------------
typedef boost::shared_ptr<TFmGem> GemDlgPtr;
extern GemDlgPtr GemDlg;
//---------------------------------------------------------------------------
#endif
