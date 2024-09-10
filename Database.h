//---------------------------------------------------------------------------

#ifndef DatabaseH
#define DatabaseH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <memory>
#include <StdCtrls.hpp>
#include "Defines.h"
//---------------------------------------------------------------------------
enum
{
	ALL_ALL_ALL=0,	ALL_ALL_TIME,
	ALL_ALM_ALL,	ALL_ALM_TIME,
	ALL_MSG_ALL,	ALL_MSG_TIME,
	A_ALL_ALL,		A_ALL_TIME,
	A_ALM_ALL,		A_ALM_TIME,
	A_MSG_ALL,		A_MSG_TIME,
	B_ALL_ALL,		B_ALL_TIME,
	B_ALM_ALL,		B_ALM_TIME,
	B_MSG_ALL,		B_MSG_TIME
};

enum LOGTYPE
{ ALARM=0, MESSAGES, PRODUCT };


typedef struct _OVEN_RECIPE
{
	int ChamberNo;
	String Recipe;
	int PtnNo;
	int CureTemp;
	int CureTime;

	void OVEN_RECIPE()
	{
		ChamberNo = 0;
		PtnNo = 0;
		CureTemp = 0;
		CureTime = 0;
		Recipe = "";
	}
} OVEN_RECIPE;

typedef struct _PLASMA_RECIPE
{
	String Recipe;
	String CleanParamName;
	String MotorParamName;
} PLASMA_RECIPE_INFO;

typedef struct _DB_USER
{
	int Grade;
	AnsiString ID;
	AnsiString PW;
	AnsiString Name;

	void USER_INFO()
	{
		Grade = 0;
		ID = "";
		PW = "";
		Name = "";
    }
} DB_USER;

typedef struct _DB_ERROR
{
	String Name;
	String Cause;
	String Action;
} DB_ERROR;

typedef struct _OVEN_WORK
{
	TDateTime StartTime;
	TDateTime EndTime;
	String Recipe;
	int PtnNo;
	int CureTemp;
	int CureTime;
	int Remote;
	String UserID;
	int ChamberNo;
	String Filename;

	// 가압오븐 용
	int PressingTime;	// 가압시간 기록(초단위)
	int DepressingTime;	// 감압시간 기록(초단위)
} OVEN_WORK;

typedef std::shared_ptr<TADOQuery> QueryPtr;

class Database : public Singleton<Database>
{
protected:	// User declarations
	std::shared_ptr<TCriticalSection> m_csLock;

	TADOConnection *ErrDB;
	TADOConnection *MainDB;
	TADOQuery *QryMain;
	TADOQuery *QryErr;

public:		// User declarations
	Database();
	virtual ~Database();

	bool NewLogAdded;

	QueryPtr Query;

	virtual void CheckDBFile() = 0;
	virtual bool ConnectDB() = 0;
	bool ConnectErrDB();

	bool IsFieldExisted(String table, String field);
	void AddStringField(String table, String field, int length);
	void AddIntField(String table, String field);
	void ModifyStringFieldSize(String table, String field, int size);

	bool QueryUser();
	bool AddUser(const DB_USER user);
	void EditUser(const DB_USER user);
	void DeleteUser(const DB_USER user);
	bool IsUserExist(const String userId);
	bool IsCorrectLoginInfo(const AnsiString sId,
									   const AnsiString sPass,
									   const int grade);

	bool GetError(unsigned int code, DB_ERROR &error, int language);
	QueryPtr GetLogQuery(TDateTime start);	// UI에서 로그 데이터 업데으트 할때 사용
};

class OvenDatabase : public Database
{
protected:

public:		// User declarations
	OvenDatabase();
	~OvenDatabase();

	void CheckDBFile();
	bool ConnectDB();

	bool QueryRecipe(int chamberNo);
	void AddRecipe(const OVEN_RECIPE dbRecipe);
	void EditRecipe(const int nCmNo,
							   const String sRecipe,
							   const OVEN_RECIPE newRecipe);
	void DeleteRecipe(const OVEN_RECIPE dbRecipe);
	void DeleteRecipe(const int nCmNo, const String sRecipe);
	bool IsRecipeExist(const int nCmNo, const String sRecipe);
	bool IsRecipeExist(const int nCmNo, const int patNo);
	bool GetRecipe(const int chamberNo,
								   const String recipe,
								   OVEN_RECIPE &recipeInfo);
	void UpdateRecipeWithPtnNo(const int chamberNo, const int patNo);
	void DeleteRecipeWithPtnNoAndRcpName(const int chamberNo, const int patNo, const String sRecipe);
	void DeleteRecipeWithPtnNo(const int chamberNo, const int patNo);
	bool GetRecipeList(int chamber, TStringList *list);
	bool GetRecipeName(const int chamberNo, const int patNo, String &name);

	bool QueryLog(int condition, TDateTime start, TDateTime end, bool isAsc = true);
	void SaveLog(const String msg,
							const int type,
							const int chamberNo = -1);

	bool QueryWork(int chamberNo, TDateTime start, TDateTime end);
	bool QueryProductCode(int chamberNo, String productCode);
	bool GetWorkNotEnd(int chamberNo, OVEN_WORK &work);
	bool GetWorkNotEnd(int chamberNo, OVEN_WORK &work, LOTINFO &lotInfo);
	bool GetWorkEnd(int chamberNo, OVEN_WORK &work);
	bool IsDataFileExist(int chamberNo, TDateTime date);
	int GetDataFileCount(int chamberNo, TDateTime date);

	void SaveWork(OVEN_WORK work);
	void SaveWork(OVEN_WORK work, String productCode);
	void SaveWork(OVEN_WORK work, LOTINFO lotInfo);
	void SaveWorkEnd(OVEN_WORK work, int endType);
	void SaveWorkEndPressure(OVEN_WORK work, int endType);

	void SaveButtonClickLog(String buttonName, int chamberNo = -1);
};

#define GetOvenDB() Singleton<OvenDatabase>::GetInstance()
#define ReleaseOvenDB() Singleton<OvenDatabase>::ReleaseInstance()

//---------------------------------------------------------------------------
#endif
