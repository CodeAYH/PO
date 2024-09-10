//---------------------------------------------------------------------------

#ifndef IOFrameH
#define IOFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvGlassButton.hpp"
#include "AdvPanel.hpp"
#include "AdvSmoothLabel.hpp"
#include <Vcl.ExtCtrls.hpp>
#include "Defines.h"
//---------------------------------------------------------------------------
class TFrmIO : public TFrame
{
__published:	// IDE-managed Components
	TAdvPanel *PnlInput;
	TAdvGlassButton *btnPrevInput;
	TAdvGlassButton *btnNextInput;
	TAdvPanel *PnlOutput;
	TAdvGlassButton *btnPrevOutput;
	TAdvGlassButton *btnNextOutput;
	TTimer *Refresh;
	TAdvPanel *CommMonitorPanel1;
	TAdvPanel *CommMonitorPanel2;
	TAdvPanel *CommInfo1;
	TAdvPanel *CommInfo2;
	TAdvPanel *HanaMESPanel;
	TLabeledEdit *HanaComm;
	void __fastcall btnPrevInputClick(TObject *Sender);
	void __fastcall btnNextInputClick(TObject *Sender);
	void __fastcall btnPrevOutputClick(TObject *Sender);
	void __fastcall btnNextOutputClick(TObject *Sender);
	void __fastcall RefreshTimer(TObject *Sender);
	void __fastcall FrameResize(TObject *Sender);
private:	// User declarations
	TAdvGlassButton *BtnInput[16];
	TAdvGlassButton *BtnOutput[16];
	TAdvGlassButton *BtnInputLabel[16];
	TAdvGlassButton *BtnOutputLabel[16];

	int ChamberNo;
	int MaxInputIndex;
	int MaxOutputIndex;
	int CurrentInputIndex;
	int CurrentOutputIndex;

	void __fastcall Init();
	void __fastcall RefreshButton();
	void __fastcall BtnOutputClick(TObject *Sender);
	void __fastcall HanaCommStatus();

public:		// User declarations
	__fastcall TFrmIO(TComponent* Owner);
	__fastcall TFrmIO(TComponent* Owner, int chamberNo);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmIO *FrmIO;
//---------------------------------------------------------------------------
#endif
