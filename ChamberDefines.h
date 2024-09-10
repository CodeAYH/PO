#ifndef ChamberDefinesH
#define ChamberDefinesH

#define KEY_DAMPERMOTORPORT						"Damper Motor Port"
#define KEY_DIOPORT								"DIO Port"
#define KEY_DPPORT								"DP Port"
#define KEY_O2ANALYZER_A						"Chamber A O2 Aanalyzer Port"
#define KEY_O2ANALYZER_B						"Chamber B O2 Aanalyzer Port"
#define	KEY_DAMPERMOTORTIMEOUT					"Damper Motor Timeout"
#define KEY_DPTIMEOUT							"DP Timeout"
#define KEY_DIOTIMEOUT							"DIO Timeout"
#define KEY_O2ANALYZER_A_TIMEOUT				"Chamber A O2 Aanalyzer Timeout"
#define KEY_O2ANALYZER_B_TIMEOUT				"Chamber B O2 Aanalyzer Timeout"
#define KEY_O2ANALYZER_C_TIMEOUT				"Chamber C O2 Aanalyzer Timeout"
#define KEY_O2ANALYZER_D_TIMEOUT				"Chamber D O2 Aanalyzer Timeout"
#define KEY_CANNOTSTARTWITHOUTLOTINFO			"Cannot Start Without Lot Info"
#define KEY_DPTYPE								"DP Type"
#define KEY_O2_DENSITY_ALARM_ENABLE_TIME		"O2 Density Alarm Enable Time"
#define KEY_CURE_STOP_ON_O2_DENSITY_ALARM		"Cure Stop On O2 Density Alarm"
#define KEY_USE_O2_ANALYZER_RUN_LIMIT			"Use O2 Analyzer Run Limit"		// 20180305 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
#define KEY_O2_DENSITY_RUN_LIMIT       			"O2 Density Run Limit"			// 20180305 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
#define KEY_O2_DENSITY_ALARM_ENABLE_TIME1		"O2 Density Alarm Enable Time1" // 20180305 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
#define KEY_O2_DENSITY_LIMIT					"O2 Density Limit"
#define KEY_LARGE_N2VALVE_OPEN_O2_DENSITY		"Large N2 Valve Open O2 Density"
#define KEY_HEATING_WITHOUT_N2					"Heating Without N2"
#define KEY_USE_O2_ANALYZER						"Use O2 Analyzer"
#define KEY_DAMPER_CONTROL_TYPE					"Damper Control Type"
#define KEY_AUTO_START							"Auto Start"
#define KEY_USE_WATERFLOW_DETECTOR				"Use Water Flow Detector"
#define KEY_USE_N2FLOW_DETECTOR					"Use N2 Flow Detector"
#define KEY_USE_N2FLOW_CONTROL					"Use N2 Flow Control"
#define KEY_USE_MAGAZINE_DETECTOR				"Use Magazine Detector"
#define KEY_MACHINEID							"MachineID"
#define KEY_USE_MANUAL_MODE						"Use Manual Mode"
#define KEY_USE_AIRPRESSURE_DETECTOR			"Use Air Pressure Detector"
#define KEY_USE_ELECTRIC_PANEL_DOOR_DETECTOR	"Use Electric Panel Door Detector"
#define KEY_USE_INVERTER						"Use Inverter"
#define KEY_INVERTER_PORT						"Inverter Port"

#define KEY_N2_LARGE_VALVE_CLOSE				"N2 Large Valve Close"
#define KEY_N2_MEDIUM_VALVE_CLOSE				"N2 Medium Valve Close"
#define KEY_N2_SMALL_VALVE_CLOSE				"N2 Small Valve Close"

#define KEY_USE_IDLE_COOLING					"Use IDLE Cooling"
#define KEY_IDLECOOLINGTEMP						"IDLE Cooling Temp."
#define KEY_IDLECOOLINGOFFSET					"IDLE Cooling Offset"

#define KEY_USE_N2PRESSURE_DETECTOR				"Use N2 Pressure Detector"
#define KEY_USE_HEATER_OVERLOAD_DETECTOR		"Use Heater Overload Detector"
#define KEY_USE_DIFF_PRESSURE_DETECTOR			"Use Diff. Pressure Detector"
#define KEY_USE_SSR_OVERTEMP_DETECTOR			"Use SSR Over Temp. Detector"
#define KEY_USE_CHAMBER_DOOR_TEMP_INTERLOCK		"Use Chamber Door Temp. Interlock"
#define KEY_USE_GROUND_ALARM_DETECTOR			"Use Ground Alarm Detector"
#define KEY_USE_DOOR_OPEN_DETECTOR_WHEN_IDLE	"Use Door Open Detector When Idle"
#define KEY_DOOR_OPEN_DETECTED_INTERVAL         "Door Open Detected Interval"
#define KEY_USE_BLOWMOTOR_LOWCURRENT_DETECTOR	"Use Blow Motor Low Current Detector"
#define KEY_USE_DOORLOCK_CYLINDER				"Use Door Lock Cylinder"
#define KEY_ALLOW_RECIPE_SELECTION				"Allow Recipe Selection"
#define KEY_DOOR_OPEN_DETECTED_WHEN_IDLE        "Use Door Open Detected When Idle"
#define KEY_GOTO_IDLE_IN_LOCAL					"Go to idle step in local mode"
#define KEY_MAINTAIN_RECIPE						"Recipe Maintain After JobEnd"
#define KEY_USE_TOWERLAMP_CONTROL				"Use Towerlamp Control"
#define KEY_USE_DOORLOCKTEMP_CONTROL			"Use Door Lock Temp Control"			// 2020-12-16 Add 챔버온도 도어락
#define KEY_USE_PASSWORD_WHEN_IDLECOOLING_STOP	"Use Password When IdleCooling Stop"	// 2021-03-05 Add IdleCooling일때 패스워스 사용유무
#define KEY_USE_LOGGER_DATA_SAVE				"Use Logger Data Save"					// 2021-03-15 ADD Logger 데이터 저장 사용유무
#define KEY_USE_LOCAL_MODE_ALARM				"Use Local Mode N2 Alarm Check"         // 2021-09-09 Local Mode N2 Alarm Check
#define KEY_DRAW_BG_SP							"Draw SP Background"

// 2021-06-08 KJH 쿨링워터 배출 시간
#define KEY_COOLING_VENT_EXUAST_TIME			"Cooling Vent Exuast Time"

#define KEY_HANA_OVEN_A_PORT				"Chamber A Hana Oven Port"
#define KEY_HANA_OVEN_B_PORT				"Chamber B Hana Oven Port"
#define KEY_HANA_OVEN_C_PORT				"Chamber C Hana Oven Port"
#define KEY_HANA_OVEN_D_PORT				"Chamber D Hana Oven Port"

#define KEY_RFID_READER_A_PORT				"RFID Reader A Port"
#define KEY_RFID_READER_B_PORT				"RFID Reader B Port"
#define KEY_RFID_READER_C_PORT				"RFID Reader C Port"
#define KEY_RFID_READER_D_PORT				"RFID Reader D Port"

//2021-08-10 hcw
#define KEY_CHAMBERLOGGERDEVIATION			"Chamber Logger Temp. Deviation"
#define KEY_DOOROPENCONDITIONTEMP           "Door Open Temp. Condition"
#define KEY_WORKCOMPLETIONTEMP             "Work Completion Temp."
//2021-09-29 Fujitsu
#define KEY_CHAMBER_SELECT_BACODE			"Chamber Select Barcode"
#define KEY_TCARD_START_CURE_BACODE			"TCard Start Cure Barcode"

// 2021-12-01 KJH 큐어 시작 알람
#define KEY_CURE_START_ALARM_TIME			"Cure Start Alarm Time"

#define KEY_AUTO_MODE_CHANGE_TIMER			"Auto Mode Change Timer"
//#define KEY_USE_N2_PURGE_DETECTOR        	"Use N2 Purge Detector"   //2023-04-17 restore
//#define KEY_USE_CDA_FLOW_DETECTOR           "Use CDA Flow Detector"  //2023-04-17 restore
//#define KEY_USE_SAFETY_RESET                "Use Safety Reset"  //2023-04-17 restore

#define KEY_USE_CURE_START_ENABLE_FROM_LOCAL_STATE	"Use Cure Start Enable From Local State"

#define KEY_AUTO_CURE_START_AFTER_VALID		"Auto Cure Start After Validation"

#define TEMP_LIMIT						500
#define DIO_COUNT						64   //JSCK 23-03-09  48 -> 64


enum INPUTS
{
	I_EMERGENCY	= 0,								// X000
	I_BUZZER_OFF,
	I_DOOR_OPEN_LOCK_OFF,							// 옵션처리 - Use Door Lock Cylinde
	I_DOOR_CLOSE_LOCK_ON,							// 옵션처리 - Use Door Lock Cylinder
	I_AIR_PRESSURE_DETECTED,						// 옵션처리 - Use Air Pressure Detector
	I_N2_PRESSURE_DETECTED,							// X005, 옵션처리 - Use N2 Pressure Detector
	I_HEATER_OVERLOAD_DETECTED,                     // 옵션처리 - Use Heater Overload Detector
	I_BLOWER_OVERLOAD_DETECTED,
	I_BLOWER_LOW_CURRENT_DETECTED,
	I_CHAMBER_OVERTEMP_DETECTED,
	I_DAMPER_MOTOR_ALARM_DETECTED,					// 옵션처리 - X00A (DamperControlType != CYLINDER_DAMPER)
	I_CDA_FLOW_DETECTED,							// 옵션처리 - X00B Change from flow meter to flow switch
	I_DIFFERENTIAL_PRESSURE_DETECTED, 				// 옵션처리 - Use Diff. Pressure Detector
	I_SSR_OVERTEMP_DETECTED,						// 옵션처리 - Use SSR Over Temp. Detector
	I_CHAMBER_DOOR_TEMP_INTERLOCK_DETECTED,			// 옵션처리 - Use Chamber Door Temp. Interlock
	I_CHAMBER_WATER_FLOW_DETECTED,					// 옵션처리 - Use Water Flow Detector
	I_ELECTRONICS_DOORLOCK_DETECTED,				// 옵션처리 - X010 Use Electric Panel Door Detector
	I_N2_PURGE_FLOW_DETECTED,                		// 옵션처리 - X011 Change from flow meter to flow switch
	I_GROUND_ALARM_DETECTED,						// X012, 옵션처리 - Use Ground Alarm Detector or Use Door Open Detector When Idle
	I_SAFETY_STATUS_ALARM_DETECTED,					// 옵션처리 - X013 Input Safety Status
	I_DAMPER_CLOSE_DETECTED,                        // for Infenion
	I_MAGAZINE_DETECTED,                            // 옵션처리 - X015 Use Magazine Detector for Infenion
	I_DOOR_CLOSED,
	X017,											// for AOS, 옵션처리 - Use Ne Flow Detector
	X018,
	X019
};

//int I_N2_FLOW_DETECTED;

enum OUTPUTS
{
	O_TOWER_LAMP_RED = 32,							// Y000   //2023-03-09  24-> 32
	O_TOWER_LAMP_GREEN,
	O_TOWER_LAMP_YELLOW,
	O_BUZZER_ON,
	O_BUZZER_KIND,
	O_BUZZER_OFF_SWITCH,							// Y005
	Y006,						        			// Y006
	O_DOOR_LOCK,
	O_N2,
	Y009,
	O_N2_LARGE,										// Y00A
	O_N2_MEDIUM,
	O_SAFETY_RESET_ON,                              //Y00C
	O_N2_SMALL,
	O_HEATER_PROBLEM_LAMP,
	O_COOLING_WATER_VENT	//Y00F
};

enum ALARMS
{
	ERR_EMERGENCY_SW = 0,
	ERR_DOOR_OPENED,
	ERR_AIR_PRESSURE_NOT_DETECTED,
	ERR_N2PRESS_NOT_DETECTED,
	ERR_HEATER_OVERLOAD,
	ERR_BLOWER_OVERLOAD,							// 5
	ERR_BLOWER_LOW_CURRENT,
	ERR_CHAMBER_OVERTEMP,
	ERR_DAMPER_MOTOR_ALARM,
	ERR_HEPAFILTER_CHANGE,
	ERR_SSR_OVERTEMP,    							// 10
	ERR_CHAMBER_WATER_FLOW_NOT_DETECTED,
	ERR_ELECTRONICS_DOORLOCK_NOT_DETECTED,
	ERR_O2_DENSITY_ALARM,
	ERR_GROUND_NOT_DETECTED,
	ERR_PV_HIGH, 									// 15
	ERR_PV_LOW,
	ERR_MAINTEMP_TIMEOUT,
	ERR_LIMIT_TIMEOUT,
	ERR_LOGGER_TIMEOUT,
	ERR_DAMPER_MOTOR_TIMEOUT,						// 20
	ERR_DP_TIMEOUT,
	ERR_O2ANALYZER_TIMEOUT,
	ERR_DI_TIMEOUT,
	ERR_DO_TIMEOUT,
	ERR_DIO_TIMEOUT,								// 25
	ERR_DOOR_UNLOCKED,
	ERR_ABNORMAL_O2_FLOWRATE,
	ERR_CHAMBER_TEMP_DOOR_INTERLOCK,
	ERR_N2FLOW_NOT_DETECTED,
	ERR_INVERTER_TRIP_DETECTED,                     // 30
	ERR_INVERTER_TIMEOUT,
	ERR_CHAMBER_LOGGER_DEVIATION_ALARM_1,           //2021-08-12 hcw.
	ERR_CHAMBER_LOGGER_DEVIATION_ALARM_2,
	ERR_CHAMBER_LOGGER_DEVIATION_ALARM_3,
	ERR_CHAMBER_LOGGER_DEVIATION_ALARM_4,
	ERR_CHAMBER_LOGGER_DEVIATION_ALARM_5,
	ERR_CHAMBER_LOGGER_DEVIATION_ALARM_6,
	ERR_CHAMBER_LOGGER_DEVIATION_ALARM_7,
	ERR_CHAMBER_LOGGER_DEVIATION_ALARM_8,
	ERR_CHAMBER_LOGGER_DEVIATION_ALARM_9,			// 40
	ERR_CHAMBER_LOGGER_DEVIATION_ALARM_10,
	ERR_CURE_NOT_STARTED,
	ERR_TEMP_CONTROLLER_NOT_RUN,
	ERR_TEMP_CONTROLLER_NOT_HOLD,
	ERR_DOOR_OPEN_TEMP_CONDITION_ALARM,
	ERR_CDA_FLOW_SWITCH_NOT_DETECCTED,             //Change from flow meter to flow switch
	ERR_N2PURGE_FLOW_SWITCH_NOT_DETECCTED,          //Change from flow meter to flow switch
	ERR_RUN_STATE_GEM_REMOTE_CHECK,                 //COWELL
	ERR_IDLE_STATUS_DOOR_CLOSED_ALARM               // 49 Door Open Detected When Idle
	};

enum STEP
{
	STEP_IDLE = 0,
	STEP_PURGE,		// 20180305 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
	STEP_RUNNING,
	STEP_COOLING,
	STEP_DONE,
	STEP_LOCAL,
	STEP_COUNT
};

enum DAMPER_CONTROL_TYPE
{
	CYLINDER_DAMPER = 0,
	MOONS_STEP_MOTOR_DAMPER,
	EZI_STEP_MOTOR_DAMPER
};

enum DIO_TYPE
{
	ICPDAS = 0,
	OVEN_DIO
};

enum HANA_CIM_TYPE
{
  INIT_HANA = 0,
  EXECUTION_HANA,
  WAIT_HANA
};

enum GEM_DOOR_STATUS
{
	DOOR_UNKOWN = -1,
	DOOR_CLOSED,
	DOOR_OPENED
};

#endif
