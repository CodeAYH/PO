//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TCardChamberRecipeForm.h"
#include "NormalOvenChamber.h"
#include "NormalOvenSystemManager.h"
#include "HelperFunctions.h"
#include "Database.h"
#include "MainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvGlassButton"
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "BaseGrid"
#pragma link "AdvUtil"
#pragma link "AdvCombo"
#pragma link "AdvPanel"
#pragma resource "*.dfm"
TFmTCardChamberRecipe *FmTCardChamberRecipe;
String FujitsuListFile = "Fujitsu.ini";
//---------------------------------------------------------------------------
__fastcall TFmTCardChamberRecipe::TFmTCardChamberRecipe(TComponent* Owner, int chamberNo)
	: TForm(Owner)
{
	Chamber = GetChamber(chamberNo);

	DeviceList = new TStringList;
	DeviceList->CaseSensitive = true;
	RecipeDeviceList = nullptr;
}

__fastcall TFmTCardChamberRecipe::~TFmTCardChamberRecipe()
{
 	try
	{
		if( DeviceList )
		{
			delete DeviceList;
			DeviceList = nullptr;
		}

		ClearRecipeDeviceList();
	}
	catch(...)
	{
		return;
	}

}
//---------------------------------------------------------------------------
void __fastcall TFmTCardChamberRecipe::FormShow(TObject *Sender)
{
	LastTCardList = "";
	TCardInputList.clear();
	String str;
	str.sprintf(L"<P align=\"center\">%c CHAMBER</P>", Chamber->ChamberID + 'A');
	ChamberPanel->Text = str;

	TCardListGrid->Clear();
	TCardListGrid->RowCount = 2;
	TCardListGrid->ColCount = 2;
	TCardListGrid->FixedRows = 1;
	TCardListGrid->FixedCols = 2;
	TCardListGrid->ColWidths[0] = 25;
	TCardListGrid->ColWidths[1] = 240;
	TCardListGrid->Cells[0][0] = L"No.";
	TCardListGrid->Cells[1][0] = L"T-Card ID";
	TCardListGrid->RowCount = 1;

	InputTCardEdit->Text = "";
	InputTCardEdit->SetFocus();

	int x, y;

	x = Left + (Width - FmMain->Keyboard->KeyboardWidth)/2;
	y = 768 - FmMain->Keyboard->KeyboardHeight;
	FmMain->Keyboard->KeyboardType = ktQWERTY;
	FmMain->Keyboard->ShowAtXY(x, y);

	InitRecipeListComboBox();
	if( LoadDeviceList() )
		UpdateDeviceListGrid();
}
//---------------------------------------------------------------------------
void __fastcall TFmTCardChamberRecipe::InitRecipeListComboBox()
{
	if( Chamber == nullptr )
		return;

	String recipe;

	RecipeListComboBox->Clear();

	if( GetOvenDB()->QueryRecipe(Chamber->ChamberID) )
	{
		int sel = -1;
		for(int j=0; j<GetOvenDB()->Query->RecordCount; j++)
		{
			recipe = GetOvenDB()->Query->FieldByName("Recipe")->AsString;
			RecipeListComboBox->Items->Add(recipe);
			if( RecipeName == recipe )
				sel = j;
			GetOvenDB()->Query->Next();
		}

		RecipeListComboBox->ItemIndex = sel;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmTCardChamberRecipe::TCardListGridCanEditCell(TObject *Sender,
		  int ARow, int ACol, bool &CanEdit)
{
	CanEdit = false;
}
//---------------------------------------------------------------------------

void __fastcall TFmTCardChamberRecipe::btnCancelClick(TObject *Sender)
{
	Result = mrCancel;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFmTCardChamberRecipe::InputTCardEditKeyDown(TObject *Sender, WORD &Key,
		  TShiftState Shift)
{
	if (Key == VK_RETURN)
	{
		String currID = InputTCardEdit->Text;
		if (currID.IsEmpty() == false)
		{
			if (LastTCardList.IsEmpty() || LastTCardList != currID)
			{
				int count, rows, idx;

			if( DeviceList->Find(currID, idx) )
			{
				SetMessageStr(GetResStr("Warning", "TCardSameName"));
				ShowMessageDlg();
				return;
			}

				DeviceList->Add(currID);

				LastTCardList = currID;
				TCardInputList.push_back(LastTCardList);
				count = TCardInputList.size();
				TCardListGrid->RowCount = count + 1;
				rows = count;
				TCardListGrid->Cells[0][rows] = count;
				TCardListGrid->Cells[1][rows] = LastTCardList;
				String str;
				str.sprintf(L"T-card recipe ID[%s] Add.", LastTCardList.c_str());
				GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);
			}
			else
			{
				String str;
				SetMessageStr(GetResStr("Warning", "TCardSameName"));
				SetMessageStr("");
				str = GetResStr("Common", "PrevInput") + " : " + LastTCardList;
				SetMessageStr(str);
				str = GetResStr("Common", "CurrentInput") + " : " + currID;
				SetMessageStr(str);
				ShowMessageDlg();
			}
			InputTCardEdit->Text = "";
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmTCardChamberRecipe::ClearRecipeDeviceList()
{
	if ( RecipeDeviceList == nullptr )
		return;

 	std::map<String, TStringList*>::iterator iter;

	for(iter=RecipeDeviceList->begin(); iter != RecipeDeviceList->end(); iter++)
	{
		TStringList *list = iter->second;
		if( list )
			delete list;
	}
	delete RecipeDeviceList;
}
void __fastcall TFmTCardChamberRecipe::RecipeListComboBoxSelect(TObject *Sender)
{
	TCardListGrid->Clear();
	DeviceList->Clear();
	TCardInputList.clear();

	if( LoadDeviceList() )
		UpdateDeviceListGrid();
}
//---------------------------------------------------------------------------
bool __fastcall TFmTCardChamberRecipe::LoadDeviceList()
{
	if( RecipeListComboBox->ItemIndex < 0 )
	{
		SetMessageStr(GetResStr("Warning", "NotExistedRecipe"));
		ShowMessageDlg();
		return false;
	}

	String rec = RecipeListComboBox->Items->Strings[RecipeListComboBox->ItemIndex];
	if( rec.IsEmpty() )
	{
		SetMessageStr(GetResStr("Warning", "NotExistedRecipe"));
		ShowMessageDlg();
		return false;
	}

	String filename = GetDataFolder() + FujitsuListFile;
	if( FileExists(filename) == false )
	{
		unsigned int handle = FileCreate(filename);
		if( handle == (unsigned int)INVALID_HANDLE_VALUE )
		{
			SetMessageStr(GetResStr("Warning", "NoFileSelectedDate"));
			ShowMessageDlg();
			return false;
		}
		FileClose(handle);
	}

	String sec;
	sec.sprintf(L"Device List_%d", Chamber->ChamberID);
	String devListString;
	std::auto_ptr<TIniFile> devListFile(new TIniFile(filename));

	if( devListFile->ValueExists(sec, rec) == false )
	{
		TCardListGrid->Clear();
		TCardListGrid->RowCount = 1;
		TCardListGrid->Cells[0][0] = L"No.";
		TCardListGrid->Cells[1][0] = L"T-Card ID";
		return false;
	}

	devListString = devListFile->ReadString(sec, rec, devListString);
	DeviceList->Clear();
	DeviceList->CommaText = devListString;
	TCardListGrid->RowCount = DeviceList->Count + 1;

	for(int i=0; i<DeviceList->Count; i++)
	{
		TCardInputList.push_back(DeviceList->Strings[i]);
	}

	return true;
}
//---------------------------------------------------------------------

bool __fastcall TFmTCardChamberRecipe::LoadDeviceListAll()
{
	String filename = GetDataFolder() + FujitsuListFile;

	ClearRecipeDeviceList();

	RecipeDeviceList = new std::map<String, TStringList*>;

	boost::shared_ptr<TIniFile> devListFile(new TIniFile(filename));

	String sec;
	sec.sprintf(L"Device List_%d", Chamber->ChamberID);

	TStringList *tmpKeyList = new TStringList;

	devListFile->ReadSection(sec, tmpKeyList);
	for(int i=0; i<tmpKeyList->Count; i++)
	{
		TStringList *devList = new TStringList;
		String key = tmpKeyList->Strings[i];
		devList->CommaText = devListFile->ReadString(sec, key, key);
		(*RecipeDeviceList)[key] = devList;
	}

	delete tmpKeyList;
	return true;
}
//---------------------------------------------------------------------------
void __fastcall TFmTCardChamberRecipe::AddDeviceButtonClick(TObject *Sender)
{
	String currID = InputTCardEdit->Text;

	if (currID.IsEmpty() == false)
	{
		if (LastTCardList.IsEmpty() || LastTCardList != currID)
		{
			int count, rows, idx;

		if( DeviceList->Find(currID, idx) )
		{
			SetMessageStr(GetResStr("Warning", "TCardSameName"));
			ShowMessageDlg();
			return;
		}

			DeviceList->Add(currID);

			LastTCardList = currID;
			TCardInputList.push_back(LastTCardList);
			count = TCardInputList.size();
			TCardListGrid->RowCount = count + 1;
			rows = count;
			TCardListGrid->Cells[0][rows] = count;
			TCardListGrid->Cells[1][rows] = LastTCardList;
		}
		else
		{
			String str;
			SetMessageStr(GetResStr("Warning", "TCardSameName"));
			SetMessageStr("");
			str = GetResStr("Common", "PrevInput") + " : " + LastTCardList;
			SetMessageStr(str);
			str = GetResStr("Common", "CurrentInput") + " : " + currID;
			SetMessageStr(str);
			ShowMessageDlg();
		}
		InputTCardEdit->Text = "";
	}
}
//---------------------------------------------------------------------------

void __fastcall TFmTCardChamberRecipe::DeleteDeviceButtonClick(TObject *Sender)
{
	int count;
	String selectedDev;
	count = TCardInputList.size();

	if (DeviceList->Count == 0)
	{
		TCardListGrid->Clear();
		TCardListGrid->Cells[0][0] = L"No.";
		TCardListGrid->Cells[1][0] = L"T-Card ID";
		return;
	}

	selectedDev = TCardListGrid->Cells[1][count];

	//String selectedDev = TCardListGrid->GetSelectionAsText();
	if( selectedDev.IsEmpty() )
		return;

	int idx = DeviceList->IndexOf(selectedDev);
	if( idx >= 0 )
	{
		SetMessageStr(GetResStr("Question", "QuestDelete") + " : " + selectedDev);
		if( ShowQuestionDlg() )
		{
			DeviceList->Delete(idx);
			TCardListGrid->RowCount = count;
			if (idx == 0)
			{
				LastTCardList = L"";
			}
			else
				LastTCardList = DeviceList->Strings[idx-1];

			int nCnt = TCardInputList.size();
			TCardInputList.pop_back();
			UpdateDeviceListGrid();
		}
	}
	else
	{
		SetMessageStr(GetResStr("Warning", "TCardNotExist"));
		ShowMessageDlg();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmTCardChamberRecipe::UpdateDeviceListGrid()
{
	TCardListGrid->Clear();
	TCardListGrid->Cells[0][0] = L"No.";
	TCardListGrid->Cells[1][0] = L"T-Card ID";

	for(int i=0; i<TCardListGrid->RowCount; i++)
		TCardListGrid->Cells[0][i+1] = i+1;

	for(int i=0; i<DeviceList->Count; i++)
	{
		TCardListGrid->Cells[1][i+1] = DeviceList->Strings[i];
	}
}
//-----------------------------------------------------------------------------
void __fastcall TFmTCardChamberRecipe::SaveDeviceListButtonClick(TObject *Sender)
{
	SetMessageStr(GetResStr("Question", "QuestSave"));
	if( ShowQuestionDlg() )
	{
		SaveDeviceList();
	}
}
//---------------------------------------------------------------------------
bool __fastcall TFmTCardChamberRecipe::SaveDeviceList()
{
	if( RecipeListComboBox->ItemIndex < 0 )
	{
		SetMessageStr(GetResStr("Warning", "SelectRecipe"));
		ShowMessageDlg();
		return false;
	}

	String rec = RecipeListComboBox->Items->Strings[RecipeListComboBox->ItemIndex];
	if( rec.IsEmpty() )
	{
		SetMessageStr(GetResStr("Warning", "SelectRecipe"));
		ShowMessageDlg();
		return false;
	}

	String filename = GetDataFolder() + FujitsuListFile;
	String sec;
	sec.sprintf(L"Device List_%d", Chamber->ChamberID);

	String str, devListString;
	std::auto_ptr<TIniFile> devListFile(new TIniFile(filename));
	int count = DeviceList->Count;

	for(int i=0; i<count; i++)
	{
		str = DeviceList->Strings[i];
		if( i == 0 )
			devListString += str;
		else
			devListString += "," + str;
	}
	devListFile->WriteString(sec, rec, devListString);

	return true;
}
//---------------------------------------------------------------------------
