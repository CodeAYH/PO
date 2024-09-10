//---------------------------------------------------------------------------

#ifndef ShowPtnFormH
#define ShowPtnFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.TeeSpline.hpp>
#include "AdvGlassButton.hpp"
#include <VclTee.TeeGDIPlus.hpp>

#include <vector>
//---------------------------------------------------------------------------
class TFmShowPtn : public TForm
{
__published:	// IDE-managed Components
	TChart *PtnChart;
	TLineSeries *Series1;
	TAdvGlassButton *btnPtnOk;
	TAdvGlassButton *btnCancel;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnPtnOkClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:	// User declarations
	const int FnChamber;

	std::vector<float> FvTsp;

	void __fastcall InitPtnChart();

	int m_Ret;

public:		// User declarations
	__fastcall TFmShowPtn(const int nChamber, TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmShowPtn *frmShowPtn;
//---------------------------------------------------------------------------
#endif
