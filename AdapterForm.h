//---------------------------------------------------------------------------

#ifndef AdapterFormH
#define AdapterFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvOfficePager.hpp"
#include "AdvOfficePagerStylers.hpp"
#include "Defines.h"
//---------------------------------------------------------------------------

enum ADAPTER_TYPE
{ AD_IO, AD_SETTING, AD_GRAPH };

class TFmAdapter : public TForm
{
__published:	// IDE-managed Components
	TAdvOfficePager *ChamberPager;
	TAdvOfficePage *Page1;
	TAdvOfficePage *Page2;
	TAdvOfficePage *Page3;
	TAdvOfficePagerOfficeStyler *AdvOfficePagerOfficeStyler1;
	TAdvOfficePage *Page4;
	TAdvOfficePage *Page5;
	TAdvOfficePage *Page6;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ChamberPagerChanging(TObject *Sender, int FromPage, int ToPage,
          bool &AllowChange);
	void __fastcall ChamberPagerChange(TObject *Sender);

private:	// User declarations
	int AdapterType;
	TAdvOfficePage *Page[6];
	TFrame *FFrame[6];
 	int PageNo;

	TFrame* __fastcall GetFrame(int idx);
	void __fastcall SetFrame(int idx, TFrame *frame);

public:		// User declarations
	__property TFrame* Frame[int idx] = { read = GetFrame, write = SetFrame };

	__fastcall TFmAdapter(TComponent* Owner);
	void __fastcall Init(int type);
};
//---------------------------------------------------------------------------
extern PACKAGE TFmAdapter *FmAdapter;
//---------------------------------------------------------------------------
#endif
