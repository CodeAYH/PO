//---------------------------------------------------------------------------
#pragma hdrstop

#include "RegisterUserForm.h"
#include "Database.h"
#include "JobForm.h"
#include "HelperFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvCombo"
#pragma link "AdvEdit"
#pragma link "AdvGlassButton"
#pragma link "AdvGroupBox"
#pragma link "AdvOfficeButtons"
#pragma link "AdvVerticalPolyList"
#pragma link "CustomItemsContainer"
#pragma link "GDIPCustomItem"
#pragma link "GDIPExpandableImageSectionItem"
#pragma link "GDIPHTMLItem"
#pragma link "GDIPImageSectionItem"
#pragma link "GDIPImageTextItem"
#pragma link "GDIPSectionItem"
#pragma link "GDIPTextItem"
#pragma link "AdvSmoothTouchKeyBoard"
#pragma resource "*.dfm"
TFmRegisterUser *FmRegisterUser;
//---------------------------------------------------------------------------
__fastcall TFmRegisterUser::TFmRegisterUser(TComponent* Owner)
	: TForm(Owner)
{
	Caption = UpperCase(Application->Title);
}
//---------------------------------------------------------------------------

void __fastcall TFmRegisterUser::Localize()
{
	btnAdd->Caption		= GetResStr("User", "Add");
	btnEdit->Caption    = GetResStr("User", "Edit");
	btnDelete->Caption	= GetResStr("User", "Delete");

	btnOk->Caption    	= GetResStr("Common", "OK");
	btnCancel->Caption	= GetResStr("Common", "Cancel");

	edtUserId->LabelCaption 	= GetResStr("User", "UserID");
	edtUserName->LabelCaption 	= GetResStr("User", "UserName");
	edtPassword->LabelCaption 	= GetResStr("User", "Password");

	rdgLevel->Caption = " " + GetResStr("User", "LevelSel") + " ";
	rdgLevel->Items->Clear();
	rdgLevel->Items->Add(GetResStr("User", "Level1"));
	rdgLevel->Items->Add(GetResStr("User", "Level2"));
	rdgLevel->Items->Add(GetResStr("User", "Level3"));

	BtnClose->Caption = GetResStr("Common", "Close");

	FillHowTo();
}
//---------------------------------------------------------------------------
void __fastcall TFmRegisterUser::FillHowTo()
{
	expSecItem0->Caption	= GetResStr("User", "HowToAdd");
	HTMLItem0_HowToAddUser->Description = "";
	HTMLItem0_HowToAddUser->ImageHeight	= 0;
	HTMLItem0_HowToAddUser->CaptionLocation	= tlTopLeft;
	HTMLItem0_HowToAddUser->Height		= 155;
	HTMLItem0_HowToAddUser->Caption = "<FONT  size=\"10\" face=\"¸¼Àº °íµñ\"><B><SHAD>"
		+ GetResStr("User", "Step_Add1") + "<BR>"
		+ GetResStr("User", "Step_Add2") + "<BR>"
		+ GetResStr("User", "Step_Add3") + "<BR>"
		+ GetResStr("User", "Step_Add4") + "<BR>"
		+ GetResStr("User", "Step_Add5") + "<BR>"
//		+ GetResStr("Register", "Step_Add6") + "<BR>"
		+ GetResStr("User", "Step_Add7") + "<BR>"
		+ "</SHAD></B></FONT>";

	expSecItem1->Caption	= GetResStr("User", "HowToEdit");
	HTMLItem1_HowToEditUser->Description = "";
	HTMLItem1_HowToEditUser->ImageHeight	= 0;
	HTMLItem1_HowToEditUser->CaptionLocation	= tlTopLeft;
	HTMLItem1_HowToEditUser->Height		= 175;
	HTMLItem1_HowToEditUser->Caption = "<FONT  size=\"10\" face=\"¸¼Àº °íµñ\"><B><SHAD>"
		+ GetResStr("User", "Step_Edit1") + "<BR>"
		+ GetResStr("User", "Step_Edit2") + "<BR>"
		+ GetResStr("User", "Step_Edit3") + "<BR>"
		+ GetResStr("User", "Step_Edit4") + "<BR>"
		+ GetResStr("User", "Step_Edit5") + "<BR>"
		+ GetResStr("User", "Step_Edit6") + "<BR>"
//		+ GetResStr("Register", "Step_Edit7") + "<BR>"
		+ GetResStr("User", "Step_Edit8") + "<BR>"
		+ "</SHAD></B></FONT>";

	expSecItem2->Caption	= GetResStr("User", "HowToDel");
	HTMLItem2_HowToDelUser->Description = "";
	HTMLItem2_HowToDelUser->ImageHeight	= 0;
	HTMLItem2_HowToDelUser->CaptionLocation	= tlTopLeft;
	HTMLItem2_HowToDelUser->Height		= 75;
	HTMLItem2_HowToDelUser->Caption = "<FONT  size=\"10\" face=\"¸¼Àº °íµñ\"><B><SHAD>"
		+ GetResStr("User", "Step_Del1") + "<BR>"
		+ GetResStr("User", "Step_Del2") + "<BR>"
		+ GetResStr("User", "Step_Del3") + "<BR>"
		+ "</SHAD></B></FONT>";

/*	expSecItem3->Caption	= GetResStr("Register", "HowToDepart");
	HTMLItem3_HowToDepart->Description = "";
	HTMLItem3_HowToDepart->ImageHeight	= 0;
	HTMLItem3_HowToDepart->CaptionLocation	= tlTopLeft;
	HTMLItem3_HowToDepart->Height		= 70;
	HTMLItem3_HowToDepart->Caption = "<FONT  size=\"10\" face=\"¸¼Àº °íµñ\"><B><SHAD>&nbsp;&nbsp;<LI>"
		+ GetResStr("Register", "Step_Depart1") + "<BR>&nbsp;&nbsp;<LI>"
		+ GetResStr("Register", "Step_Depart2") + "<BR>"
		+ "</SHAD></B></FONT>";  */
}
//---------------------------------------------------------------------------
void __fastcall TFmRegisterUser::FillUserGrid()
{
	UserGrid->Columns->Clear();
	UserGrid->Columns->Add();
//	UserGrid->Columns->Items[0]->Title->Font->Name = Global->sFontName;
	UserGrid->Columns->Items[0]->Title->Font->Size = 11;
	UserGrid->Columns->Items[0]->Title->Font->Style = TFontStyles() << fsBold;
	UserGrid->Columns->Items[0]->Title->Caption = edtUserId->LabelCaption;
	UserGrid->Columns->Items[0]->Title->Alignment = taCenter;

//	UserGrid->Columns->Items[0]->Font->Name = Global->sFontName;
	UserGrid->Columns->Items[0]->Font->Size = 11;
	UserGrid->Columns->Items[0]->Alignment = taCenter;

	if( GetOvenDB()->QueryUser() )
	{
		SrcUser->DataSet = GetOvenDB()->Query.get();

		if( SrcUser->DataSet )
		{
			UserGrid->DataSource = SrcUser;
			UserGrid->Columns->Items[0]->Width = 240;
			UserGrid->Columns->Items[0]->FieldName = "ID";

			rdgLevel->ItemIndex = SrcUser->DataSet->FieldByName("Grade")->AsInteger - 1;
			edtUserId->Text		= SrcUser->DataSet->FieldByName("ID")->AsString;
			edtUserName->Text	= SrcUser->DataSet->FieldByName("Name")->AsString;
			edtPassword->Text 	= SrcUser->DataSet->FieldByName("PW")->AsString;
		}
	}

	EnableCtrls();
}
//---------------------------------------------------------------------------
void __fastcall TFmRegisterUser::FormShow(TObject *Sender)
{
	m_bEditMode = false;
	m_bAddMode  = false;

	Localize();
	FillHowTo();
	FillUserGrid();

	String str;
	str = "User UI Executed.";
	GetOvenDB()->SaveLog(str, MESSAGES);
}
//---------------------------------------------------------------------------
void __fastcall TFmRegisterUser::EnableCtrls()
{
	UserGrid->Enabled 	= !m_bEditMode && !m_bAddMode;
	btnAdd->Enabled 	= !m_bEditMode && !m_bAddMode;
	btnEdit->Enabled 	= !m_bEditMode && !m_bAddMode;
	btnDelete->Enabled 	= !m_bEditMode && !m_bAddMode;

	rdgLevel->Enabled 		= m_bAddMode || m_bEditMode;
	edtUserId->ReadOnly  	= !m_bAddMode;
	edtUserName->ReadOnly  	= !m_bAddMode && !m_bEditMode;
	edtPassword->ReadOnly  	= !m_bAddMode && !m_bEditMode;
}
//---------------------------------------------------------------------------
void __fastcall TFmRegisterUser::EmptySelInfo()
{
	UserGrid->Enabled 	= !m_bEditMode && !m_bAddMode;
	btnAdd->Enabled 	= !m_bEditMode && !m_bAddMode;
	btnEdit->Enabled 	= !m_bEditMode && !m_bAddMode;
	btnDelete->Enabled 	= !m_bEditMode && !m_bAddMode;

	rdgLevel->Enabled 		= m_bAddMode || m_bEditMode;
	edtUserId->ReadOnly  	= !m_bAddMode;
	edtUserName->ReadOnly  	= !m_bAddMode && !m_bEditMode;
	edtPassword->ReadOnly  	= !m_bAddMode && !m_bEditMode;
}
//---------------------------------------------------------------------------
void __fastcall TFmRegisterUser::UpdateSelInfo()
{
	if( SrcUser->DataSet )
	{
		rdgLevel->ItemIndex = SrcUser->DataSet->FieldByName("Grade")->AsInteger - 1;
		edtUserId->Text		= SrcUser->DataSet->FieldByName("ID")->AsString;
		edtUserName->Text	= SrcUser->DataSet->FieldByName("Name")->AsString;
		edtPassword->Text 	= SrcUser->DataSet->FieldByName("PW")->AsString;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmRegisterUser::UserGridCellClick(TColumn *Column)
{
	UpdateSelInfo();
}
//---------------------------------------------------------------------------
void __fastcall TFmRegisterUser::btnAddClick(TObject *Sender)
{
	m_bAddMode = true;
	EnableCtrls();
	EmptySelInfo();
	edtUserId->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFmRegisterUser::btnEditClick(TObject *Sender)
{
	m_bEditMode = true;
	EnableCtrls();
	edtUserName->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFmRegisterUser::btnDeleteClick(TObject *Sender)
{
	AnsiString sQuestion = GetResStr("Question", "QuestDelete");

	SetMessageStr(sQuestion);
	if( !ShowQuestionDlg() )
        return;

	DB_USER UsrInfo;
	UsrInfo.ID = edtUserId->Text;
	GetOvenDB()->DeleteUser(UsrInfo);

	FillUserGrid();
}
//---------------------------------------------------------------------------
void __fastcall TFmRegisterUser::edtUserIdKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if( Key == VK_RETURN )
		edtUserName->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFmRegisterUser::btnOkClick(TObject *Sender)
{
	DB_USER UsrInfo;

	UsrInfo.ID = edtUserId->Text;
	UsrInfo.Name = edtUserName->Text;
	UsrInfo.PW = edtPassword->Text;
	UsrInfo.Grade = rdgLevel->ItemIndex + 1;

	if( m_bEditMode )
	{
		AnsiString sQuestion = GetResStr("Question", "QuestUpdate");
		SetMessageStr(sQuestion);
		if( !ShowQuestionDlg() )
            return;

		GetOvenDB()->EditUser(UsrInfo);
		m_bEditMode = false;
	}
	if( m_bAddMode )
	{
		AnsiString sQuestion = GetResStr("Question", "QuestAdd");
		SetMessageStr(sQuestion);
		if( !ShowQuestionDlg() )
            return;

		GetOvenDB()->AddUser(UsrInfo);
		m_bAddMode = false;
	}

	FillUserGrid();
}
//---------------------------------------------------------------------------
void __fastcall TFmRegisterUser::btnCancelClick(TObject *Sender)
{
	if( m_bEditMode )
		m_bEditMode = false;

	if( m_bAddMode )
		m_bAddMode = false;

	FillUserGrid();
}
//---------------------------------------------------------------------------
void __fastcall TFmRegisterUser::edtUserNameKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if( Key == VK_RETURN )
		edtUserName->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFmRegisterUser::edtPasswordKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if( Key == VK_RETURN )
		edtPassword->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFmRegisterUser::BtnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFmRegisterUser::edtUserIdEnter(TObject *Sender)
{
	Keyboard->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TFmRegisterUser::edtUserIdExit(TObject *Sender)
{
	Keyboard->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFmRegisterUser::FormClose(TObject *Sender, TCloseAction &Action)
{
	String str;
	str = "User UI Closed.";
	GetOvenDB()->SaveLog(str, MESSAGES);

	FmJob->Show();
}
//---------------------------------------------------------------------------

