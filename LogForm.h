//---------------------------------------------------------------------------

#ifndef LogFormH
#define LogFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvDateTimePicker.hpp"
#include "AdvGlassButton.hpp"
#include "AdvGrid.hpp"
#include "AdvGroupBox.hpp"
#include "AdvObj.hpp"
#include "AdvOfficeButtons.hpp"
#include "AdvPanel.hpp"
#include "BaseGrid.hpp"
#include <Data.DB.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include "DBAdvGrid.hpp"
#include <Vcl.Graphics.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include "AdvUtil.hpp"
//---------------------------------------------------------------------------
class TFmLog : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnlBtm;
	TAdvOfficeRadioGroup *SelectLog;
	TAdvDateTimePicker *dtPickerStart;
	TAdvDateTimePicker *dtPickerEnd;
	TAdvGroupBox *AdvGroupBox1;
	TCheckBox *AllTime;
	TLabel *Label1;
	TAdvGlassButton *BtnClose;
	TAdvStringGrid *LogGrid;
	TAdvGlassButton *SaveToCSVButton;
	TSaveTextFileDialog *SaveTextFileDialog;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BtnCloseClick(TObject *Sender);
	void __fastcall AllTimeClick(TObject *Sender);
	void __fastcall SelectLogClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall SaveToCSVButtonClick(TObject *Sender);
private:	// User declarations
	TDateTime m_dtSel;
	void __fastcall Localize();
	void __fastcall InitGrid();
	void __fastcall SearchLog();

public:		// User declarations
	__fastcall TFmLog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFmLog *FmLog;
//---------------------------------------------------------------------------
#endif
