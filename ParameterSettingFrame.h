//---------------------------------------------------------------------------

#ifndef ParameterSettingFrameH
#define ParameterSettingFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvGlassButton.hpp"
#include "AdvGrid.hpp"
#include "AdvGroupBox.hpp"
#include "AdvObj.hpp"
#include "AdvOfficeButtons.hpp"
#include "AdvPanel.hpp"
#include "AdvSmoothProgressBar.hpp"
#include "BaseGrid.hpp"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include "AdvUtil.hpp"
//---------------------------------------------------------------------------
class TFrmParameterSetting : public TFrame
{
__published:	// IDE-managed Components
	TAdvPanel *AdvPanel1;
	TAdvStringGrid *GrdUP550Instruction;
	TAdvGlassButton *BtnUP550Setting;
	TAdvSmoothProgressBar *UP550ProgressBar;
	TAdvPanel *AdvPanel2;
	TAdvStringGrid *GrdLoggerInstruction;
	TAdvSmoothProgressBar *LoggerProgressBar;
	TAdvGlassButton *BtnLoggerSetting;
	TAdvOfficeRadioGroup *SelectAddr;
	void __fastcall BtnUP550SettingClick(TObject *Sender);
private:	// User declarations
	int ChamberNo;

public:		// User declarations
	__fastcall TFrmParameterSetting(TComponent* Owner);
	__fastcall TFrmParameterSetting(TComponent* Owner, int chamberNo);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmParameterSetting *FrmParameterSetting;
//---------------------------------------------------------------------------
#endif
