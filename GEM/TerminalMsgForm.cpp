//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TerminalMsgForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
GemTermianlPtr TerminalDlg;

__fastcall TFmTerminalMsg::TFmTerminalMsg(boost::shared_ptr<TStringList>slMsg, TComponent* Owner)
	: m_slMsg(slMsg), TForm(Owner)
{
	mmMsg->Clear();
	mmMsg->ReadOnly = true;
}
//---------------------------------------------------------------------------
__fastcall TFmTerminalMsg::~TFmTerminalMsg()
{

}
//---------------------------------------------------------------------------
void __fastcall TFmTerminalMsg::FormCreate(TObject *Sender)
{
	AlignComps();

	mmMsg->Lines->Assign(m_slMsg.get());
}
//---------------------------------------------------------------------------

void __fastcall TFmTerminalMsg::AlignComps()
{
	Width  = 600;
	Height = 150;
	Position = poOwnerFormCenter;

	this->BorderStyle = Forms::bsSingle;
	this->FormStyle = fsStayOnTop;
    TBorderIcons tempBI = BorderIcons;
    tempBI >> biMaximize >> biMinimize >> biHelp;
	BorderIcons = tempBI;
	Font->Name = "Tahoma";
	Caption = "Terminal Message";
}
//---------------------------------------------------------------------------
void __fastcall TFmTerminalMsg::mmMsgClick(TObject *Sender)
{
	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFmTerminalMsg::FormClose(TObject *Sender, TCloseAction &Action)
{
//	Action = caFree;
}
//---------------------------------------------------------------------------

void __fastcall TFmTerminalMsg::FormShow(TObject *Sender)
{
	SetWindowPos(Handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE ||
			SWP_NOSIZE || SWP_NOACTIVATE);
}
//---------------------------------------------------------------------------

