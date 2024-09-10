//---------------------------------------------------------------------------

#ifndef TerminalMsgFormH
#define TerminalMsgFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

//#include <memory>
#include <boost/shared_ptr.hpp>
//---------------------------------------------------------------------------
class TFmTerminalMsg : public TForm
{
__published:	// IDE-managed Components
	TMemo *mmMsg;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall mmMsgClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);

private:	// User declarations
	const boost::shared_ptr<TStringList> m_slMsg;

	void __fastcall AlignComps();

public:		// User declarations
	__fastcall TFmTerminalMsg(boost::shared_ptr<TStringList>slMsg, TComponent* Owner);
	__fastcall ~TFmTerminalMsg();
};
//---------------------------------------------------------------------------
typedef boost::shared_ptr<TFmTerminalMsg> GemTermianlPtr;
extern GemTermianlPtr TerminalDlg;
//---------------------------------------------------------------------------
#endif
