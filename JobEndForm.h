//---------------------------------------------------------------------------

#ifndef JobEndFormH
#define JobEndFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "EllipsLabel.hpp"
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "BaseGrid.hpp"
#include <Grids.hpp>
#include "AdvGlassButton.hpp"
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.Series.hpp>
#include "AdvUtil.hpp"
#include <memory>
#include <boost/shared_ptr.hpp>
//---------------------------------------------------------------------------
class NormalOvenChamber;

class TFmJobEnd : public TForm
{
__published:	// IDE-managed Components
    TEllipsLabel *lblChamber;
    TAdvStringGrid *JobInfoGrid;
	TAdvGlassButton *btnOk;
	TTimer *BuzzerOffTime;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnOkClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall BuzzerOffTimeTimer(TObject *Sender);

private:	// User declarations
	NormalOvenChamber *Chamber;
	TDateTime EndTime;

public:		// User declarations
	__fastcall TFmJobEnd(TComponent* Owner, int chamberNo);
	void __fastcall UpdateInfo(TDateTime end, int endType, bool remote);
};
//---------------------------------------------------------------------------
typedef boost::shared_ptr<TFmJobEnd> JobEndPtr;
extern JobEndPtr JobEndDlg[MAX_CHAMBER_COUNT];
//---------------------------------------------------------------------------
#endif
