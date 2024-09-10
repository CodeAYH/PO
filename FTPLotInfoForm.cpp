//---------------------------------------------------------------------------
#pragma hdrstop

#include "FTPLotInfoForm.h"
#include "NormalOvenChamber.h"
#include "NormalOvenSystemManager.h"
#include "HelperFunctions.h"
#include "Defines.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvGlassButton"
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "BaseGrid"
#pragma link "AdvEdit"
#pragma link "AdvUtil"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TFTPLotInfoForm::TFTPLotInfoForm(const int nChamber, TComponent* Owner)
	: FnChamber(nChamber), TForm(Owner)
{
	m_Ret = mrCancel;
	CurrentMaxRow = 0;
	IsInsertLotInfoGrid = false;
	Caption = "INPUT LOT ID & DEVICE TYPE";
}
//---------------------------------------------------------------------------
void __fastcall TFTPLotInfoForm::FormCreate(TObject *Sender)
{
	InitLotInfoGrid();
}
//---------------------------------------------------------------------------
void __fastcall TFTPLotInfoForm::InitLotInfoGrid()
{
    // 20181010 - Add to User ID Edit - mtcho
    UserIDEdit->Clear();
    UserIDEdit->Width = 200;

	LotInfoGrid->Clear();

	LotInfoGrid->ColWidths[0] = 50;
	LotInfoGrid->ColWidths[1] = 300;
	LotInfoGrid->ColWidths[2] = 300;

	LotInfoGrid->Cells[0][0] = "No.";
	LotInfoGrid->Cells[1][0] = "Lot ID";
	LotInfoGrid->Cells[2][0] = "Device Type";
	for(int i=0; i<FTP_LOT_COUNT; i++)
		LotInfoGrid->Cells[0][i+1] = IntToStr(i+1);

	NormalOvenChamber *Chamber = GetChamber(FnChamber);
	if( Chamber != nullptr )
	{
        // 20181010 - Add to User ID Edit - mtcho
        UserIDEdit->Text = Chamber->FTPLotInfo.UserID;
		// Data Insert
		if(Chamber->FTPLotInfo.TotalLotCount > 0)
		{
			if(Chamber->FTPLotInfo.TotalLotCount >= FTP_LOT_COUNT)
				CurrentMaxRow = FTP_LOT_COUNT;
			else
				CurrentMaxRow = Chamber->FTPLotInfo.TotalLotCount + 1;

			for(int i=1; i<=CurrentMaxRow; i++)
			{
				LotInfoGrid->Cells[1][i] = Chamber->FTPLotInfo.LotID[i-1];
				LotInfoGrid->Cells[2][i] = Chamber->FTPLotInfo.DeviceType[i-1];
			}
		}
		else
		{
			CurrentMaxRow = 1;
		}
	}
	else
	{
		CurrentMaxRow = 1;
	}
	SetLotGridFocus(1, CurrentMaxRow);

	// Save Button Setting
	if( Chamber->Step > STEP_IDLE
		&& Chamber->Step < STEP_DONE )
	{
		btnPtnOk->Enabled = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFTPLotInfoForm::btnPtnOkClick(TObject *Sender)
{
	String str;
	NormalOvenChamber *Chamber = GetChamber(FnChamber);
	if( Chamber == nullptr )
	{
		m_Ret = mrCancel;
		Close();
	}

    // 20181012 - User ID is empty (Notify) - mtcho
    if( UserIDEdit->Text == "" )
    {
        str.sprintf(L"UserID is empty.");
    SetMessageStr(str);
    ShowMessageDlg();
    UserIDEdit->Text = "";
            return;
    }

	if(CurrentMaxRow >= 1 && CurrentMaxRow <= FTP_LOT_COUNT)
	{
		// Value Check
		if(CheckValidLotInfo() == true)
		{
			// Save LotInfo, OK
			str.sprintf(L"LOT information Saved.");
			SetMessageStr(str);
			ShowMessageDlg();

			m_Ret = mrOk;
			// 저장시 다이얼로그 출력 유지 Close();

		}
		else
		{
			// Invalid Value, Cancel
			str.sprintf(L"Invalid LOT information entered.");
			SetMessageStr(str);
			ShowMessageDlg();

			if(IsInsertLotInfoGrid == true)
			{
				InitLotInfoGrid();
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFTPLotInfoForm::btnCancelClick(TObject *Sender)
{
	m_Ret = mrCancel;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFTPLotInfoForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	ModalResult = m_Ret;
}
//---------------------------------------------------------------------------

void __fastcall TFTPLotInfoForm::LotInfoGridCanEditCell(TObject *Sender, int ARow,
          int ACol, bool &CanEdit)
{
	if( ARow != 0 && ACol != 0 )
	{
		if( ARow < CurrentMaxRow )
			CanEdit = true;
		else
		if( ARow == CurrentMaxRow )
		{
			if( ACol == 1)
				CanEdit = true;
			else
			{
				// Lot ID 란이 비어 있으면 비활성
				if( LotInfoGrid->Cells[1][ARow].IsEmpty() )
					CanEdit = false;
				else
					CanEdit = true;
			}
		}
		else
			CanEdit = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFTPLotInfoForm::LotInfoGridEditCellDone(TObject *Sender, int ACol,
          int ARow)
{
	String str;

	// 20180713 - 문자 공백 지우기 - jhlim
	TReplaceFlags Flags = TReplaceFlags() << rfReplaceAll;

	// LOT ID 입력
	if( ARow >= 1 && ACol == 1 )
	{
		AnsiString lotId = LotInfoGrid->Cells[ACol][ARow];

		// 20180713 - 문자 공백 지우기 - jhlim
		lotId = StringReplace(lotId, " ", "", Flags);
		LotInfoGrid->Cells[ACol][ARow] = lotId;

		if( lotId.IsEmpty() )
		{
			SetLotGridFocus(1, CurrentMaxRow);
			return;
		}
		// 중복 체크
		if( IsLotExist(lotId, ARow) )
		{
			str.sprintf(L"Duplicate LOT ID.");
			SetMessageStr(str);
			ShowMessageDlg();
			LotInfoGrid->Cells[ACol][ARow] = "";
			return;
		}
		else
		{
			SetLotGridFocus(2, CurrentMaxRow);
		}

	}
	// Device Type 입력
	else if( ARow >= 1 && ACol == 2 )
	{
		AnsiString lotId = LotInfoGrid->Cells[ACol-1][ARow];
		AnsiString DeviceType = LotInfoGrid->Cells[ACol][ARow];

		// 20180713 - 문자 공백 지우기 - jhlim
		lotId = StringReplace(lotId, " ", "", Flags);
		DeviceType = StringReplace(DeviceType, " ", "", Flags);
		LotInfoGrid->Cells[ACol-1][ARow] = lotId;
		LotInfoGrid->Cells[ACol][ARow] = DeviceType;

		if( lotId.IsEmpty() )
		{
			SetLotGridFocus(1, CurrentMaxRow);
			return;
		}
		else if( DeviceType.IsEmpty() )
		{
			SetLotGridFocus(2, CurrentMaxRow);
			return;
		}
		else
		{
			if( CurrentMaxRow == ARow )
			{
				CurrentMaxRow = ARow + 1;

				if(CurrentMaxRow > FTP_LOT_COUNT)
				{
					CurrentMaxRow--;
					SetLotGridFocus(1, 1);
				}
				else
					SetLotGridFocus(1, CurrentMaxRow);
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFTPLotInfoForm::LotInfoGridGetAlignment(TObject *Sender, int ARow,
          int ACol, TAlignment &HAlign, TVAlignment &VAlign)
{
	HAlign = taCenter;
	VAlign = Advobj::vtaCenter;
}
//---------------------------------------------------------------------------

void __fastcall TFTPLotInfoForm::LotInfoGridGetCellColor(TObject *Sender, int ARow,
          int ACol, TGridDrawState AState, TBrush *ABrush, TFont *AFont)
{
	NormalOvenChamber *Chamber = GetChamber(FnChamber);
	if( Chamber == nullptr )
		return;

	if( Chamber->Step != STEP_IDLE )
	{
		ABrush->Color = clInactiveCaption;
		return;
	}

	if( ARow != 0 && ACol != 0 )
	{
		if( ARow < CurrentMaxRow )
			ABrush->Color = clWhite;
		else
		if( ARow == CurrentMaxRow )
		{
			if( ACol == 1)
				ABrush->Color = clWhite;
			else
			{
				// Lot ID 란이 비어 있으면 비활성
				if( LotInfoGrid->Cells[1][ARow].IsEmpty() )
					ABrush->Color = clInactiveCaption;
				else
					ABrush->Color = clWhite;
			}
		}
		else
			ABrush->Color = clWhite;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFTPLotInfoForm::LotInfoGridGetEditorType(TObject *Sender, int ACol,
          int ARow, TEditorType &AEditor)
{
	if( ARow != 0 )
	{
		if( ACol == 1 )
			AEditor = edNormal;
		else
		if( ACol == 2 )
			AEditor = edNormal;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFTPLotInfoForm::SetLotGridFocus(int col, int row)
{
	try
	{
		if( this->Showing &&
			LotInfoGrid->Visible &&
			LotInfoGrid->Enabled )
		{
			LotInfoGrid->Col = col;
			LotInfoGrid->Row = row;
			LotInfoGrid->SetFocus();
		}
	}
	catch(...)
	{
		return;
    }
}
//---------------------------------------------------------------------------
bool __fastcall TFTPLotInfoForm::CheckValidLotInfo()
{
	NormalOvenChamber *Chamber = GetChamber(FnChamber);
	if( Chamber == nullptr )
		return false;
        // 20181010 - Add to User ID Edit - mtcho
	AnsiString UserId = "";

	AnsiString lotId = "";
	AnsiString DeviceType = "";
	IsInsertLotInfoGrid = false;

	// Init
	Chamber->FTPLotInfo.TotalLotCount = 0;
        // 20181010 - Add to User ID Edit - mtcho
        Chamber->FTPLotInfo.UserID = "";

	for(int j=0; j<FTP_LOT_COUNT; j++)
	{
		Chamber->FTPLotInfo.LotID[j] = "";
		Chamber->FTPLotInfo.DeviceType[j] = "";
	}

	// 20180709 - 문자 공백 지우기 - jhlim
	TReplaceFlags Flags = TReplaceFlags() << rfReplaceAll;
        
	if(CurrentMaxRow == 1)
	{
		UserId = "";  // 20181010 - Add to User ID Edit - mtcho
		lotId = "";
		DeviceType = "";

                UserId 		= UserIDEdit->Text;  // 20181010 - Add to User ID Edit - mtcho
		lotId 		= LotInfoGrid->Cells[1][1];
		DeviceType 	= LotInfoGrid->Cells[2][1];

		// 20180709 - 문자 공백 지우기 - jhlim
		lotId = StringReplace(lotId, " ", "", Flags);
		DeviceType = StringReplace(DeviceType, " ", "", Flags);
		LotInfoGrid->Cells[1][1] = lotId;
		LotInfoGrid->Cells[2][1] = DeviceType;

		if( lotId.IsEmpty() == false || DeviceType.IsEmpty() == false )
			IsInsertLotInfoGrid = true;

		if(lotId.IsEmpty() == false || DeviceType.IsEmpty() == true)
			return false;
		else if(lotId.IsEmpty() == true || DeviceType.IsEmpty() == true)
			return true;
	}
	else if(CurrentMaxRow > 1)
	{
		for(int i=1; i<=CurrentMaxRow; i++)
		{
               		UserId = "";  // 20181010 - Add to User ID Edit - mtcho
			lotId = "";
			DeviceType = "";

                        UserId 		= UserIDEdit->Text;  // 20181010 - Add to User ID Edit - mtcho
			lotId 		= LotInfoGrid->Cells[1][i];
			DeviceType 	= LotInfoGrid->Cells[2][i];

			// 20180709 - 문자 공백 지우기 - jhlim
			lotId = StringReplace(lotId, " ", "", Flags);
			DeviceType = StringReplace(DeviceType, " ", "", Flags);
			LotInfoGrid->Cells[1][i] = lotId;
			LotInfoGrid->Cells[2][i] = DeviceType;

			if( lotId.IsEmpty() == false || DeviceType.IsEmpty() == false )
				IsInsertLotInfoGrid = true;

			if( i == CurrentMaxRow )
			{
				if(lotId.IsEmpty() == false && DeviceType.IsEmpty() == true)
					return false;
				else if(lotId.IsEmpty() == false && DeviceType.IsEmpty() == false)
				{
                                        Chamber->FTPLotInfo.UserID 	    = UserId; // 20181010 - Add to User ID Edit - mtcho
					Chamber->FTPLotInfo.LotID[i-1] 		= lotId;
					Chamber->FTPLotInfo.DeviceType[i-1] = DeviceType;
					Chamber->FTPLotInfo.TotalLotCount++;

					return true;
				}
			}
			else
			{
				if(lotId.IsEmpty() == true || DeviceType.IsEmpty() == true)
					return false;
				else if(lotId.IsEmpty() == false && DeviceType.IsEmpty() == false)
				{
                                        Chamber->FTPLotInfo.UserID 	    = UserId; // 20181010 - Add to User ID Edit - mtcho
					Chamber->FTPLotInfo.LotID[i-1] 		= lotId;
					Chamber->FTPLotInfo.DeviceType[i-1] = DeviceType;
					Chamber->FTPLotInfo.TotalLotCount++;
				}
			}
		}
	}

	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TFTPLotInfoForm::IsLotExist(String strValue, int row/*=-1"*/)
{
	String strInfo;

	for(int i=1; i<=FTP_LOT_COUNT; i++)
	{
		if( row > 0 && row == i )
			continue;

		strInfo = LotInfoGrid->Cells[1][i];

		if( strInfo == strValue )
			return true;
	}

	return false;
}
//---------------------------------------------------------------------------
// 20181012 - User ID Set Focus - mtcho
void __fastcall TFTPLotInfoForm::FormShow(TObject *Sender)
{
	if(UserIDEdit->Text == "")
        {
        	UserIDEdit->SetFocus();
                LotInfoGrid->Enabled = false;
        }
}
//---------------------------------------------------------------------------
// 20181012 - LotInfoGrid Enable by Input UserID - mtcho
void __fastcall TFTPLotInfoForm::UserIDEditChange(TObject *Sender)
{
	LotInfoGrid->Enabled = true;
        if (UserIDEdit->Text == "") {
		LotInfoGrid->Enabled = false;
        }
}
//---------------------------------------------------------------------------

