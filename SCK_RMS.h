//---------------------------------------------------------------------------

#ifndef SCK_RMSH
#define SCK_RMSH
#include "NormalOvenChamber.h"
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
#define WM_SCK_RMS_MSG		(WM_USER+2000)
enum { INVALID_FILE = -1, VALID_FILE };
class SckRmsThread : public TThread
{
private:
protected:
	NormalOvenChamber *Chamber;
	int ChamberNo;
	void __fastcall Execute();
	bool __fastcall CheckWorkFolder();
	String __fastcall GetSCKMESWorkFileFolder();
public:
	__fastcall SckRmsThread(NormalOvenChamber *chamber);
	__fastcall ~SckRmsThread();
};
//---------------------------------------------------------------------------
#endif
