//---------------------------------------------------------------------------
#pragma hdrstop

#include "LoginForm.h"
#include "Database.h"
#include "HelperFunctions.h"
#include "NormalOvenSystemManager.h"
#include "MainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvEdit"
#pragma link "AdvGlassButton"
#pragma link "AdvSmoothTouchKeyBoard"
#pragma resource "*.dfm"

bool __fastcall
ShowLogin(const int nLevel, const bool newLogin)
{
	std::shared_ptr<TFmLogin> Login(new TFmLogin(nullptr, nLevel));
	GetManager()->IsShowLoginForm = true;
	Login->ShowModal();
	bool ret = (Login->ModalResult == mrOk);
	if( ret && newLogin )
	{
		GetManager()->LoginLevel = nLevel;
		GetManager()->UserID = Login->edtId->Text;
    }
	Login.reset();
	GetManager()->IsShowLoginForm = false;
	return ret;
}
//---------------------------------------------------------------------------
__fastcall TFmLogin::TFmLogin(TComponent* Owner, const int nLevel)
	: Level(nLevel), TForm(Owner)
{
	m_Ret = mrCancel;
    AlignComps();
	Localize();
}
//---------------------------------------------------------------------------
void __fastcall TFmLogin::AlignComps()
{
   Position = poOwnerFormCenter;

    TBorderIcons tempBI = BorderIcons;
    tempBI >> biMaximize >> biMinimize >> biHelp;
	BorderIcons = tempBI;
    Caption = "Login";
}
//---------------------------------------------------------------------------
void __fastcall TFmLogin::Localize()
{
	edtId->LabelCaption = GetResStr("Login Dialog", "UserID");
	edtPassword->LabelCaption = GetResStr("Login Dialog", "Password");

	btnOk->Caption      = GetResStr("Common", "OK");
	btnCancel->Caption  = GetResStr("Common", "Cancel");

	LevelMode->Caption = LevelStr[Level];
}
//---------------------------------------------------------------------------
void __fastcall TFmLogin::btnOkClick(TObject *Sender)
{
	if( edtId->Text.Trim().IsEmpty() )
	{
		SetMessageStr(GetResStr("Warning", "EmptyUserID"));
		ShowMessageDlg();
		edtId->SetFocus();
		return;
	}
	if( edtPassword->Text.Trim().IsEmpty() )
	{
		SetMessageStr(GetResStr("Warning", "EmptyPassword"));
		ShowMessageDlg();
		edtPassword->SetFocus();
		return;
	}

	if( Level == LV_DEVELOPER )
	{
		if( edtId->Text=="vvv" &&
			edtPassword->Text==SuperKey )
		{
			String sMsg = "Login (ID : " + edtId->Text +")";
			GetOvenDB()->SaveLog(sMsg, MESSAGES);
			m_Ret = mrOk;

			Close();
		}
		else
		{
			edtId->Text = "";
			edtPassword->Text = "";
			edtId->SetFocus();
		}
	}
	else
	if( GetOvenDB()->IsCorrectLoginInfo(edtId->Text, edtPassword->Text, Level) )
	{
		String sMsg = "Login (ID : " + edtId->Text +")";
		GetOvenDB()->SaveLog(sMsg, MESSAGES);
		m_Ret = mrOk;

		Close();
	}
	else
	{
	    edtId->Text = "";
	    edtPassword->Text = "";
        edtId->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmLogin::edtIdKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	if( Key == VK_RETURN )
		edtPassword->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFmLogin::edtPasswordKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	if( Key == VK_RETURN )
		btnOkClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TFmLogin::btnCancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFmLogin::FormClose(TObject *Sender, TCloseAction &Action)
{
	this->ModalResult = m_Ret;
	FmMain->Keyboard->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TFmLogin::FormCreate(TObject *Sender)
{
	edtId->Text = "";
	edtPassword->Text = "";

	FmMain->Keyboard->CreateForm();
}
//---------------------------------------------------------------------------
void __fastcall TFmLogin::FormShow(TObject *Sender)
{
	if( Level == LV_DEVELOPER )
	{
		int r, s, e;
		String str;

		srand(time(nullptr));
		for(int i=0; i<10; i++)
		{
			r = rand()%10;
			if( i==0 )
				s = r;
			else if( i==9 )
				e = r;
			str += r;
		}
		s += e;
		SuperKey = "vvv" + IntToStr(s) + IntToStr(s);
		PwHint->Caption = str;
		PwHint->Visible = true;
	}
	else
		PwHint->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFmLogin::edtIdEnter(TObject *Sender)
{
	int x, y;

	x = Left + (Width - FmMain->Keyboard->KeyboardWidth)/2;
	y = Top + Height;
	FmMain->Keyboard->ShowAtXY(x, y);
}
//---------------------------------------------------------------------------
void __fastcall TFmLogin::edtIdExit(TObject *Sender)
{
	FmMain->Keyboard->Hide();
}
//---------------------------------------------------------------------------




