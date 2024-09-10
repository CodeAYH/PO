//---------------------------------------------------------------------------

#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvGlassButton.hpp"
#include "AdvSmoothTouchKeyBoard.hpp"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "AdvGlassButton.hpp"
#include "AdvSmoothTouchKeyBoard.hpp"
#include <Vcl.Touch.Keyboard.hpp>
#include "NormalOvenChamber.h"
#include "SCK_RMS.h"
//---------------------------------------------------------------------------
class TFmMain : public TForm
{
__published:	// IDE-managed Components
	TAdvGlassButton *BtnOperatorLogin;
	TAdvGlassButton *BtnEngineerLogin;
	TAdvGlassButton *BtnExit;
	TTimer *ClickCancel;
	TAdvSmoothPopupTouchKeyBoard *Keyboard;
	TTimer *ClearOutput;
	TTimer *AutoModeChange;
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall BtnOperatorLoginClick(TObject *Sender);
	void __fastcall BtnEngineerLoginClick(TObject *Sender);
	void __fastcall BtnExitClick(TObject *Sender);
	void __fastcall FormClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ClickCancelTimer(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall AutoModeTimer(TObject *Sender);
private:	// User declarations
	int ClickCount;

	void __fastcall AppSettingChange(System::TObject* Sender, int Flag, const System::UnicodeString Section, int &Result);

	void __fastcall SetDateTimeFormat();
	void __fastcall Localize();
	void __fastcall OnHanaReceivePtn(TMessage &Msg); //2021-08-09 hcw
	void __fastcall OnHanaOvenStartStop(TMessage &Msg); //2021-08-09 hcw
	void __fastcall OnSCK_RMSStart(TMessage &Msg); // 2021-12-15
	void __fastcall OnAlarmSet(TMessage &Msg);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_USER+2, TMessage, OnHanaReceivePtn); //2021-08-09  hcw
		VCL_MESSAGE_HANDLER(WM_USER+3, TMessage, OnHanaOvenStartStop); //2021-08-09  hcw
		VCL_MESSAGE_HANDLER(WM_SCK_RMS_MSG, TMessage, OnSCK_RMSStart);
		VCL_MESSAGE_HANDLER(WM_GEM_ALARM_SET, TMessage, OnAlarmSet);
	END_MESSAGE_MAP(TForm)

public:		// User declarations
	__fastcall TFmMain(TComponent* Owner);
	void __fastcall EnableAutoModeChangeTimer(int interval);     //2021-12-17 hcw
	TDateTime LogStartTime;

	// Exception ===============================================
	void  __fastcall AppException(TObject* Sender,  Exception* E);
};
//---------------------------------------------------------------------------
extern PACKAGE TFmMain *FmMain;
//---------------------------------------------------------------------------
#endif
