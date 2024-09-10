//---------------------------------------------------------------------------

#ifndef AlarmFormH
#define AlarmFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "AdvGlassButton.hpp"
#include <memory>
#include <boost/shared_ptr.hpp>
#include <SyncObjs.hpp>
#include "Defines.h"
//---------------------------------------------------------------------------
class NormalOvenChamber;

class TFmAlarm : public TForm
{
__published:	// IDE-managed Components
    TPanel *pnlErrNo;
    TPanel *pnlErrName;
    TPanel *Panel3;
	TPanel *pnlCause;
    TPanel *Panel5;
    TPanel *pnlAction;
    TStaticText *stCause;
    TStaticText *stAction;
	TPanel *pnlImg;
    TImage *ImgWarn;
    TImage *ImgErr;
	TTimer *Blink;
	TAdvGlassButton *btnAllClear;
	TAdvGlassButton *btnBuzzerOff;
	TAdvGlassButton *btnHide;
	TAdvGlassButton *btnShowPrevError;
	TAdvGlassButton *btnShowNextError;
	TAdvGlassButton *btnClear;
	TPanel *pnlCount;
	TTimer *Update;
    void __fastcall BlinkTimer(TObject *Sender);
    void __fastcall btnAllClearClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnBuzzerOffClick(TObject *Sender);
	void __fastcall btnHideClick(TObject *Sender);
	void __fastcall btnShowPrevErrorClick(TObject *Sender);
	void __fastcall btnShowNextErrorClick(TObject *Sender);
	void __fastcall btnClearClick(TObject *Sender);
	void __fastcall UpdateTimer(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:	// User declarations
	NormalOvenChamber *Chamber;
	int ChamberNo;
	int Index;
	bool DoBuzzerOn;

 	void __fastcall AlignComps();
	void __fastcall Localize();

    void __fastcall ViewAllClearButton();

public:		// User declarations
	__fastcall TFmAlarm(TComponent* Owner, int chamberNo);
	void __fastcall UpdateInfo(int idx);
};
//---------------------------------------------------------------------------
typedef boost::shared_ptr<TFmAlarm> AlarmPtr;
extern AlarmPtr AlarmDlg[MAX_CHAMBER_COUNT];
//---------------------------------------------------------------------------
#endif
