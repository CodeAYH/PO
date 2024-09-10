//---------------------------------------------------------------------------

#ifndef ScreenSaverH
#define ScreenSaverH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvPanel.hpp"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TScreenSaver : public TForm
{
__published:	// IDE-managed Components
	TTimer *RefreshDataTimer;
	TAdvPanel *Panel1;
	TAdvPanel *Panel2;
	TAdvPanel *Panel3;
	TAdvPanel *Panel4;

	void __fastcall FormMouseActivate(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y, int HitTest, TMouseActivate &MouseActivate);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall RefreshDataTimerTimer(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);

private:	// User declarations
	TAdvPanel* Panel[4];

public:		// User declarations
	__fastcall TScreenSaver(TComponent* Owner);

	void __fastcall Localize();
	void __fastcall RefreshData();
};
//---------------------------------------------------------------------------
extern PACKAGE TScreenSaver *ScreenSaver;
//---------------------------------------------------------------------------
#endif
