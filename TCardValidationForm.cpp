//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TCardValidationForm.h"
#include "NormalOvenChamber.h"
#include "NormalOvenSystemManager.h"
#include "HelperFunctions.h"
#include "Database.h"
#include "MainForm.h"
#include "JobForm.h"
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
TFmTCardValidation *FmTCardValidation;
String FujitsuFile = "Fujitsu.ini";
//---------------------------------------------------------------------------
__fastcall TFmTCardValidation::TFmTCardValidation(TComponent* Owner, int chamberNo)
	: TForm(Owner), FnChamber(chamberNo)
{
	Chamber = GetChamber(chamberNo);

	for(int i=0; i<TCARD_COUNT; i++)
	{
		DeviceList[i] = new TStringList;
		DeviceList[i]->CaseSensitive = true;
	}
	RecipeDeviceList = nullptr;
}

__fastcall TFmTCardValidation::~TFmTCardValidation()
{
 	try
	{
		for(int i=0; i<TCARD_COUNT; i++)
		{
			if( DeviceList[i] )
			{
				delete DeviceList[i];
				DeviceList[i] = nullptr;
			}
		}

		ClearRecipeDeviceList();
	}
	catch(...)
	{
		return;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmTCardValidation::FormShow(TObject *Sender)
{
	LastTCardList = "";
	TCardInputList.clear();
	String str;
	str.sprintf(L"<P align=\"center\">%c CHAMBER RECIPE:</P>", Chamber->ChamberID + 'A');
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
	LoadDeviceList();
  //UpdateDeviceListGrid();
}
//---------------------------------------------------------------------------
void __fastcall TFmTCardValidation::InitRecipeListComboBox()
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

			GetOvenDB()->Query->Next();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFmTCardValidation::TCardListGridCanEditCell(TObject *Sender,
		  int ARow, int ACol, bool &CanEdit)
{
	CanEdit = false;
}
//---------------------------------------------------------------------------

void __fastcall TFmTCardValidation::btnOkClick(TObject *Sender)
{
	Result = mrOk;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFmTCardValidation::btnCancelClick(TObject *Sender)
{
	FmJob->ChamberFrame[Chamber->ChamberID]->FujisuTCard_RecipeComboInit(true);
	Result = mrCancel;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFmTCardValidation::InputTCardEditKeyDown(TObject *Sender, WORD &Key,
		  TShiftState Shift)
{
	if (Key == VK_RETURN)
	{
		String str;
		String currID = InputTCardEdit->Text;
		if (currID.IsEmpty() == false)
		{
			if (currID == Chamber->TCardStartCure)
			{
				if (RecipeListComboBox->ItemIndex == -1)
				{
					str = GetResStr("Warning", "NotExistedRecipe") + " : " + currID;
					SetMessageStr(str);
					ShowMessageDlg();
					InputTCardEdit->Text = "";
					return;
				}

				String sRecipe = RecipeListComboBox->Items->Strings[RecipeListComboBox->ItemIndex];

				if( SelectRecipeExists(RecipeListComboBox->ItemIndex) == false )
				{
					str = GetResStr("Warning", "NotExistedRecipe") + " : " + sRecipe;
					SetMessageStr(str);
					ShowMessageDlg();
					return;
				}

				str = GetResStr("Question", "QuestStartCureTCard") + " : " + currID;
				SetMessageStr(str);
				str = GetResStr("Recipe", "Recipe") + " : " + RecipeListComboBox->Items->Strings[RecipeListComboBox->ItemIndex];
				SetMessageStr(str);

				str.sprintf(L"T-card StartCure[%s] Recipe[%s]", currID.c_str(), RecipeListComboBox->Items->Strings[RecipeListComboBox->ItemIndex].c_str());
				GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);
				if( ShowQuestionDlg() )
				{
					if( FmJob->ChamberFrame[Chamber->ChamberID]->CureStart(false) == false )
					{
						ShowMessageDlg();
						return;
					}
					else
					{
						btnOkClick(this);
						return;
					}
				}
				else
				{
					FmJob->ChamberFrame[Chamber->ChamberID]->FujisuTCard_RecipeComboInit(true);
                }
			}
			else
			{
				if (LastTCardList.IsEmpty() || LastTCardList != currID)
				{
					int count, rows, idx;

					for (int i = 0; i < RecipeListComboBox->Items->Count; i++)
					{
 //						if( DeviceList[i]->Find(currID, idx) )
						if (DeviceList[i]->Count > 0)
						{
							for (int j = 0; j < DeviceList[i]->Count; j++)
							{
								if (currID == DeviceList[i]->Strings[j])
								{
									if (RecipeListComboBox->ItemIndex != -1)
									{
										if (RecipeListComboBox->ItemIndex != i)
										{
											str = GetResStr("Warning", "TCardMismatch") + " : " + currID;
											SetMessageStr(str);
											ShowMessageDlg();
											InputTCardEdit->Text = "";
											return;
										}
									}
									LastTCardList = currID;
									TCardInputList.push_back(LastTCardList);
									count = TCardInputList.size();
									TCardListGrid->RowCount = count + 1;
									rows = count;
									TCardListGrid->Cells[0][rows] = count;
									TCardListGrid->Cells[1][rows] = LastTCardList;
									RecipeListComboBox->ItemIndex = i;
									InputTCardEdit->Text = "";
									String str;
									str.sprintf(L"T-card Recipe Validation[%s]", LastTCardList.c_str());
									GetOvenDB()->SaveLog(str, MESSAGES, Chamber->ChamberID);
									return;
								}
								else
								{
								}
							}
						}
						else
						{
							if (i == RecipeListComboBox->Items->Count -1)
							{
								str = GetResStr("Warning", "NotExistedTCard") + " : " + currID;
								SetMessageStr(str);
								ShowMessageDlg();
								InputTCardEdit->Text = "";
								return;
							}
						}
					}
				}
				else
				{
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
}
//---------------------------------------------------------------------------
bool __fastcall TFmTCardValidation::SelectRecipeExists(int RecipeID)
{
	if( Chamber == nullptr )
		return false;

	if( Chamber->Step != STEP_IDLE )
	{
		SetMessageStr(GetResStr("Warning", "CuringJob"));
		ShowMessageDlg();
		return false;
	}

	String sRecipe = RecipeListComboBox->Items->Strings[RecipeID];

	if (FmJob->ChamberFrame[Chamber->ChamberID]->SelectRecipe_FujitsuLotInfo(sRecipe) == false)
	{
		return false;
    }

	return true;
}

void __fastcall TFmTCardValidation::ClearRecipeDeviceList()
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

void __fastcall TFmTCardValidation::RecipeListComboBoxSelect(TObject *Sender)
{
	TCardListGrid->Clear();
	for(int i=0; i<TCARD_COUNT; i++)
		DeviceList[i]->Clear();

	LoadDeviceList();
}
//---------------------------------------------------------------------------
bool __fastcall TFmTCardValidation::LoadDeviceList()
{
	for (int i = 0; i < RecipeListComboBox->Items->Count; i++)
	{
		String rec = RecipeListComboBox->Items->Strings[i];
		if( rec.IsEmpty() )
		{
			SetMessageStr(GetResStr("Warning", "NotExistedRecipe"));
			ShowMessageDlg();
			return false;
		}

		String filename = GetDataFolder() + FujitsuFile;
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
			DeviceList[i]->Clear();
			continue;
		}
		else
		{
			devListString = devListFile->ReadString(sec, rec, devListString);

			DeviceList[i]->Clear();
			DeviceList[i]->CommaText = devListString;

		   /*	for (int j = 0; j < DeviceList->Count; j++)
			{
				TCardList[i][j] = DeviceList->Strings[j];
			} */
		}
	}

  /*	if( devListFile->ValueExists(sec, rec) == false )
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
	TCardListGrid->RowCount = DeviceList->Count + 1;*/
	return true;
}
//---------------------------------------------------------------------
bool __fastcall TFmTCardValidation::LoadDeviceListAll()
{
	String filename = GetDataFolder() + FujitsuFile;

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
void __fastcall TFmTCardValidation::UpdateDeviceListGrid()
{
	TCardListGrid->Clear();
	TCardListGrid->Cells[0][0] = L"No.";
	TCardListGrid->Cells[1][0] = L"T-Card ID";

	for(int i=0; i<TCardListGrid->RowCount; i++)
		TCardListGrid->Cells[0][i+1] = i+1;

   /*	for(int i=0; i<DeviceList->Count; i++)
		TCardListGrid->Cells[1][i+1] = DeviceList->Strings[i]; */
}
//---------------------------------------------------------------------------
bool __fastcall TFmTCardValidation::SaveDeviceList()
{
 /*	if( RecipeListComboBox->ItemIndex < 0 )
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

	String filename = GetDataFolder() + FujitsuFile;
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
	devListFile->WriteString(sec, rec, devListString); */

	return true;
}
//---------------------------------------------------------------------------
