//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PatternSettingFrame.h"
#include "NormalOvenChamber.h"
#include "NormalOvenSystemManager.h"
#include "HelperFunctions.h"
#include "Database.h"
#include "GEMForm.h"
#include "EzNetForm.h"
#include "MainForm.h"
#include "TCardChamberRecipeForm.h"
#include "MainTempController.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvCombo"
#pragma link "AdvEdit"
#pragma link "AdvGlassButton"
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "AdvPanel"
#pragma link "BaseGrid"
#pragma link "ToolPanels"
#pragma link "AdvUtil"
#pragma resource "*.dfm"

String STC_NAME[3] = { "SSP Start", "PV Start(Rate)", "PV Start(Time)" };

//---------------------------------------------------------------------------
__fastcall TFrmPatternSetting::TFrmPatternSetting(TComponent* Owner, int chamberNo)
	: TFrame(Owner), ChamberNo(chamberNo)
{
	AreaStartTime = 0;
	AreaEndTime = 0;
	MaxTemp = 0;

	Init();
	UpdateRecipeGrid();
	EventTypeComboBoxChange(nullptr);
}
//---------------------------------------------------------------------------
__fastcall TFrmPatternSetting::~TFrmPatternSetting()
{
	delete TempSeries;
}
//---------------------------------------------------------------------------
int __fastcall TFrmPatternSetting::GetEventIndex(int eventKind)
{
	int index = -1;
	for (int i = 0; i < MAX_EVENT_COUNT; i++)
	{
		if (EventList[i].EventNo == eventKind)
		{
			index = i;
			break;
		}
	}
	return index;
}
//---------------------------------------------------------------------------
int __fastcall TFrmPatternSetting::GetEventType(int eventIndex)
{
	if (eventIndex < 0 || eventIndex >= MAX_EVENT_COUNT)
	{
		return -1;
	}

	return EventList[eventIndex].EventNo;
}
//---------------------------------------------------------------------------
int __fastcall TFrmPatternSetting::GetEventCount(int segNo)
{
	int count = 0;
	for (int i = 0; i < 8; i++)
	{
		if (TempPtn.Seg[segNo].EvKind[i] == 0)
		{
			break;
		}
		else
		{
			count++;
        }
	}
	return count;
}
//---------------------------------------------------------------------------
int __fastcall TFrmPatternSetting::GetSegNoByPosX(int x)
{
	int segNo = -1;
	int segCount = TempPtn.Stc.SegCount;
	int curSegStartTime = 0, curSegEndTime = 0;
	int segStart, segEnd;
	double dInc = TDateTime(0, 1, 0, 0); 	// 단위 1분
	for (int i = 0; i < segCount; i++)
	{
		curSegStartTime = curSegEndTime;
		curSegEndTime += TempPtn.Seg[i].Time;

		segStart = TempSeries->CalcXPosValue(curSegStartTime * dInc);
		segEnd = TempSeries->CalcXPosValue(curSegEndTime * dInc);
		if (x >= segStart && x < segEnd)
		{
			segNo = i;
			break;
		}
	}
	return segNo;
}
//---------------------------------------------------------------------------
int __fastcall TFrmPatternSetting::GetSegNoByTime(int minutes)
{
	int segNo = -1;
	int segCount = TempPtn.Stc.SegCount;
	int curSegStartTime = 0, curSegEndTime = 0;
	for (int i = 0; i < segCount; i++)
	{
		curSegStartTime = curSegEndTime;
		curSegEndTime += TempPtn.Seg[i].Time;

		if (minutes >= curSegStartTime && minutes < curSegEndTime)
		{
			segNo = i;
			break;
		}
	}
	return segNo;
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::Init()
{
    NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if (chamber == nullptr)
	{
		return;
	}

	BtnPattern[0] = BtnPattern1;
	BtnPattern[1] = BtnPattern2;
	BtnPattern[2] = BtnPattern3;
	BtnPattern[3] = BtnPattern4;
	BtnPattern[4] = BtnPattern5;
	BtnPattern[5] = BtnPattern6;
	BtnPattern[6] = BtnPattern7;
	BtnPattern[7] = BtnPattern8;
	BtnPattern[8] = BtnPattern9;
	BtnPattern[9] = BtnPattern10;
	BtnPattern[10] = BtnPattern11;
	BtnPattern[11] = BtnPattern12;
	BtnPattern[12] = BtnPattern13;
	BtnPattern[13] = BtnPattern14;
	BtnPattern[14] = BtnPattern15;
	BtnPattern[15] = BtnPattern16;
	BtnPattern[16] = BtnPattern17;
	BtnPattern[17] = BtnPattern18;
	BtnPattern[18] = BtnPattern19;
	BtnPattern[19] = BtnPattern20;
	BtnPattern[20] = BtnPattern21;
	BtnPattern[21] = BtnPattern22;
	BtnPattern[22] = BtnPattern23;
	BtnPattern[23] = BtnPattern24;
	BtnPattern[24] = BtnPattern25;
	BtnPattern[25] = BtnPattern26;
	BtnPattern[26] = BtnPattern27;
	BtnPattern[27] = BtnPattern28;
	BtnPattern[28] = BtnPattern29;
	BtnPattern[29] = BtnPattern30;

	SelectedPtnNo = 0;
	CurrentSegNo = 0;
	CurrentEventIndex = 0;
	CurrentInverterFreq = 0;
	TempPtn.Empty();

	STCComboBox->ItemIndex = 0;
	EditorPanel->OptimizePaint = true;
	EventTypeComboBox->Clear();
	DamperEdit->Visible = (GetManager()->DamperControlType != CYLINDER_DAMPER);
	InverterEdit->Visible = GetManager()->UseInverter;

	int evtCount = MAX_EVENT_COUNT;
	for(int i=0; i<evtCount; i++)
	{
		bool add = true;

		if (EventList[i].EventNo != ET_NONE)
		{
			add = EventList[i].EventNo == ET_PVHIGH_CLEAR || EventList[i].EventNo == ET_PVLOW_CLEAR ? false : true;
			if (chamber->MainTempControllerType == TEMPCTRL_UP55A && add == true)
			{
				EventTypeComboBox->AddItem(EventList[i].Name, (TObject*)&EventList[i]);
			}
			else if (chamber->MainTempControllerType == TEMPCTRL_UP550)
			{
                EventTypeComboBox->AddItem(EventList[i].Name, (TObject*)&EventList[i]);
            }
		}
	}

	InitButtons();
	InitChart();
	InitSeries();
	InitEventInfoGrid();
	InitSegInfoGrid();
	UpdateChart();

	FillSegInfo();
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::BtnPattern1Click(TObject *Sender)
{
	SelectedPtnNo = (int)((TAdvSmoothToggleButton*)Sender)->Tag;
	for(int i=0; i<30; i++)
	{
		if( i != SelectedPtnNo-1 )
		{
			BtnPattern[i]->Down = false;
			BtnPattern[i]->Invalidate();
		}
	}

	CurrentSegNo = 0;
	CurrentEventIndex = 0;
	TempPtn.Empty();

	String strFolder, strFile;
	strFolder.sprintf(L"%s%s%c\\",
					  GetRootFolder().c_str(),
					  L"PATTERN_CM_",
					  'A'+ChamberNo);
	strFile.sprintf(L"%s%d.ptn", strFolder.c_str(), SelectedPtnNo);
	if( FileExists(strFile) )
	{
		LoadTempPtn(ChamberNo, SelectedPtnNo, TempPtn);
	}
	UpdateChart();
	FillSegInfo();
	FillSegAdd();
	FillEventAdd();
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::BtnPtnSaveClick(TObject *Sender)
{
	String strFolder, strFile;
	strFolder.sprintf(L"%s%s%c\\",
					  GetRootFolder().c_str(),
					  L"PATTERN_CM_",
					  'A'+ChamberNo);
	strFile.sprintf(L"%s%d.ptn", strFolder.c_str(), SelectedPtnNo);

	bool overwrite = false;
	if( FileExists(strFile) )
	{
		SetMessageStr(GetResStr("Warning", "ExistedPtn"));
		if( !ShowQuestionDlg() )
			return;

		overwrite = true;
	}

	TempPtn.PtnNo = SelectedPtnNo;
	if (SaveTempPtn(ChamberNo, TempPtn))
	{
		SetMessageStr(GetResStr("Message", "SaveDone"));
		ShowMessageDlg();
	}
	InitButtons();

	// 이미 있던 패턴을 수정한 경우
	// 데이터베이스에서 관련된 레시피 정보를 업데이트한다.
	if( overwrite )
	{
		GetOvenDB()->UpdateRecipeWithPtnNo(ChamberNo, SelectedPtnNo);
		UpdateRecipeGrid();

		if (GetManager()->UseAutomation == true)
		{
			String recipeName;
			if (GetManager()->IsUseGem() &&
				GemDlg && GemDlg->IsGemConnected())
			{
				if( GetOvenDB()->GetRecipeName(ChamberNo, SelectedPtnNo, recipeName) )
					GemDlg->SendPPChangedEvent(ChamberNo, SelectedPtnNo, recipeName, PPCHANGESTATUS_CHANGED);
			}
			else
			if (GetManager()->AutomationKind == SECS_GEM_SCK && EzNetDlg[ChamberNo] != nullptr &&
				EzNetDlg[ChamberNo]->IsGemConnected())
			{
				if( GetOvenDB()->GetRecipeName(ChamberNo, SelectedPtnNo, recipeName) )
					EzNetDlg[ChamberNo]->SendPPChangedEvent(recipeName, PPCHANGESTATUS_CHANGED);
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::BtnPtnDeleteClick(TObject *Sender)
{
	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( chamber == nullptr )
		return;

	if( SelectedPtnNo == chamber->Recipe.TempPtn.PtnNo &&
		chamber->Step != STEP_IDLE )
	{
		SetMessageStr(GetResStr("Warning", "RunningPtnNo"));
		ShowMessageDlg();
		return;
	}

	String strFolder, strFile;
	strFolder.sprintf(L"%s%s%c\\",
					  GetRootFolder().c_str(),
					  L"PATTERN_CM_",
					  'A'+ChamberNo);
	strFile.sprintf(L"%s%d.ptn", strFolder.c_str(), SelectedPtnNo);

	if( FileExists(strFile) )
	{
		SetMessageStr(GetResStr("Question", "QuestDelete"));
		if( ShowQuestionDlg() == true )
		{
			DeleteFile(strFile);

			if (GetManager()->UseAutomation == true)
			{
				String recipeName;
				if(GetManager()->IsUseGem() &&
						GemDlg && GemDlg->IsGemConnected())
				{
					if( GetOvenDB()->GetRecipeName(ChamberNo, SelectedPtnNo, recipeName) )
						GemDlg->SendPPChangedEvent(ChamberNo, SelectedPtnNo, recipeName, PPCHANGESTATUS_DELETED);
				}
				else
				if (GetManager()->AutomationKind == SECS_GEM_SCK && EzNetDlg[ChamberNo] != nullptr &&
					EzNetDlg[ChamberNo]->IsGemConnected())
				{
					if( GetOvenDB()->GetRecipeName(ChamberNo, SelectedPtnNo, recipeName) )
						EzNetDlg[ChamberNo]->SendPPChangedEvent(recipeName, PPCHANGESTATUS_DELETED);
				}
			}

			GetOvenDB()->DeleteRecipeWithPtnNo(ChamberNo, SelectedPtnNo);

			CurrentSegNo = 0;
			CurrentEventIndex = 0;
			TempPtn.Empty();

			UpdateRecipeGrid();
			InitButtons();
			UpdateChart();
			FillSegInfo();
			FillSegAdd();
			FillEventAdd();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::BtnRecipeAddClick(TObject *Sender)
{
	if( RecipeNameEdit->Text.IsEmpty() )
	{
		SetMessageStr(GetResStr("Warning", "InputRecipeName"));
		ShowMessageDlg();
		return;
	}

	if( SelectPatternComboBox->ItemIndex < 0 )
	{
		SetMessageStr(GetResStr("Warning", "SelectPattern"));
		ShowMessageDlg();
		return;
	}

	OVEN_RECIPE rec;
	PTN ptn;
	String str;
	str = SelectPatternComboBox->Items->Strings[SelectPatternComboBox->ItemIndex];

	rec.ChamberNo = ChamberNo;
	rec.Recipe = RecipeNameEdit->Text;
	rec.PtnNo = str.ToInt();
	LoadTempPtn(rec.ChamberNo, rec.PtnNo, ptn);
	rec.CureTemp = ptn.MaxTemp;
	rec.CureTime = ptn.TotalTime;


	str.sprintf(L"The recipe Add Click. Recipe = [%s] PTN = %d", rec.Recipe.c_str(), rec.PtnNo);
	GetOvenDB()->SaveLog(str, MESSAGES, ChamberNo);

	if( GetOvenDB()->IsRecipeExist(rec.ChamberNo,
							   rec.Recipe)==false )
	{
		GetOvenDB()->AddRecipe(rec);

		if (GetManager()->UseAutomation == true)
		{
			if (GetManager()->IsUseGem() &&
				GemDlg && GemDlg->IsGemConnected())
			{
				GemDlg->SendPPChangedEvent(rec.ChamberNo, rec.PtnNo, rec.Recipe, PPCHANGESTATUS_ADDED);
			}
			else
			if (GetManager()->AutomationKind == SECS_GEM_SCK && EzNetDlg[ChamberNo] != nullptr &&
				EzNetDlg[ChamberNo]->IsGemConnected())
			{
				EzNetDlg[ChamberNo]->SendPPChangedEvent(rec.Recipe, PPCHANGESTATUS_ADDED);
			}
		}
	}
	else
	{
		SetMessageStr(GetResStr("Warning", "ExistedRecipe"));
		ShowMessageDlg();
		return;
	}
	UpdateRecipeGrid();
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::BtnRecipeDeleteClick(TObject *Sender)
{
	if( SelectedRow < 1 )
	{
		SetMessageStr(GetResStr("Warning", "NoSelectedRecipe"));
		ShowMessageDlg();
		return;
	}

	SetMessageStr(GetResStr("Question", "QuestDelete"));
	if( ShowQuestionDlg() == false )
		return;

	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( chamber == nullptr )
		return;

	int ptnNo = GrdRecipeEdit->Cells[1][SelectedRow].ToInt();
	if( ptnNo == chamber->Recipe.TempPtn.PtnNo &&
		chamber->Step != STEP_IDLE )
	{
		SetMessageStr(GetResStr("Warning", "RunningPtnNo"));
		ShowMessageDlg();
		return;
	}

	OVEN_RECIPE recipeInfo;

	recipeInfo.Recipe = GrdRecipeEdit->Cells[0][SelectedRow];
	recipeInfo.ChamberNo = chamber->ChamberID;

    String str;
	str.sprintf(L"The recipe Delete Click. Recipe = [%s]", recipeInfo.Recipe.c_str());
	GetOvenDB()->SaveLog(str, MESSAGES, chamber->ChamberID);

	if( GetOvenDB()->IsRecipeExist(chamber->ChamberID, GrdRecipeEdit->Cells[0][SelectedRow]) )
	{
		GetOvenDB()->DeleteRecipe(recipeInfo);

		if (GetManager()->UseAutomation == true)
		{
			if (GetManager()->IsUseGem() &&
				GemDlg && GemDlg->IsGemConnected())
			{
				GemDlg->SendPPChangedEvent(recipeInfo.ChamberNo, ptnNo, recipeInfo.Recipe, PPCHANGESTATUS_DELETED);
			}
			else
			if (GetManager()->AutomationKind == SECS_GEM_SCK && EzNetDlg[ChamberNo] != nullptr &&
				EzNetDlg[ChamberNo]->IsGemConnected())
			{
				EzNetDlg[ChamberNo]->SendPPChangedEvent(recipeInfo.Recipe, PPCHANGESTATUS_DELETED);
			}
		}

		UpdateRecipeGrid();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::GrdRecipeEditClickCell(TObject *Sender, int ARow,
          int ACol)
{
	SelectedRow = ARow;
	RecipeName = GrdRecipeEdit->Cells[0][ARow];
	GrdRecipeEdit->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::GrdRecipeEditDblClickCell(TObject *Sender, int ARow,
		  int ACol)
{
	SelectedRow = ARow;
	RecipeName = GrdRecipeEdit->Cells[0][ARow];
	GrdRecipeEdit->Invalidate();

	if( GetManager()->UseAutomation &&
		GetManager()->AutomationKind == FUJITSU_TCARD )
	{
		NormalOvenChamber *chamber = GetChamber(ChamberNo);

		boost::shared_ptr<TFmTCardChamberRecipe> TCardChamberRecipeDlg(new TFmTCardChamberRecipe(this, chamber->ChamberID));
		TCardChamberRecipeDlg->RecipeName = RecipeName;
		if( TCardChamberRecipeDlg->ShowModal() == mrCancel )
		{
			return;
		}
		//TCardRecipeDlg[chamber->ChamberID]->RecipeName = RecipeName;
		//TCardRecipeDlg[chamber->ChamberID]->Show();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::GrdRecipeEditDrawCell(TObject *Sender, int ACol,
		  int ARow, TRect &Rect, TGridDrawState State)
{
	if( ARow == 0 )
		GrdRecipeEdit->Canvas->Brush->Color = clSkyBlue;
	else
	if( SelectedRow == ARow )
		GrdRecipeEdit->Canvas->Brush->Color = clHighlight;
	else
		GrdRecipeEdit->Canvas->Brush->Color = clWhite;
	GrdRecipeEdit->Canvas->FillRect(Rect);

	DrawText(GrdRecipeEdit->Canvas->Handle,
			 String(GrdRecipeEdit->Cells[ACol][ARow]).c_str(),
			 -1,
			 &Rect,
			 DT_SINGLELINE|DT_VCENTER|DT_CENTER);
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::SelectPatternComboBoxEnter(TObject *Sender)
{
 	String strFile, strPattern;

	SelectPatternComboBox->Items->Clear();
	for(int i=0; i<30; i++)
	{
		strPattern.sprintf(L"%d", i+1);
		strFile.sprintf(L"%s%s%c\\%s.ptn",
						GetRootFolder().c_str(),
						L"PATTERN_CM_",
						'A'+ChamberNo,
						strPattern.c_str());
		if( FileExists(strFile) )
			SelectPatternComboBox->Items->Add(strPattern);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::OnEditEnter(TObject *Sender)
{
	ShowKeyboard(false);
	PostMessage(Handle, WM_USER+1, (WPARAM)Sender, 0);
	((TAdvEdit*)Sender)->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::OnEditExit(TObject *Sender)
{
	FmMain->Keyboard->Hide();

	TAdvEdit *edit = (TAdvEdit*)Sender;
	int tag = edit->Tag;
	String str;

	if (tag == 100)		// 정수값 범위 체크
	{
		int min = edit->MinValue;
		int max = edit->MaxValue;
		int value = edit->Text.ToInt();
		if (value < min || value > max)
		{
			SetMessageStr(GetResStr("Warning", "MaxLimitOver"));
			str.sprintf(L"[Range : %d ~ %d]", min, max);
			SetMessageStr(str);
			ShowMessageDlg();
		}
	}
	else if (tag == 200)	// 실수값 범위 체크
	{
		double min = edit->MinFloatValue;
		double max = edit->MaxFloatValue;
		double value = edit->Text.ToDouble();
		if (value < min || value > max)
		{
			SetMessageStr(GetResStr("Warning", "MaxLimitOver"));
			str.sprintf(L"[Range : %.2f ~ %.2f]", min, max);
			SetMessageStr(str);
			ShowMessageDlg();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::EventTypeComboBoxChange(TObject *Sender)
{
	int eventIndex = EventTypeComboBox->ItemIndex;
	int eventKind = GetEventType(eventIndex);
	if (eventKind > 0)
	{
		if (eventKind == ET_PVHIGH || eventKind == ET_PVLOW)
		{
			OffEdit->Visible = false;
			OnEdit->Visible = true;
			OnEdit->LabelCaption = "Offset";
		}
		else if (eventKind == ET_PVHIGH_CLEAR || eventKind == ET_PVLOW_CLEAR)
		{
			OnEdit->Visible = false;
			OffEdit->Visible = false;
        }
		else
		{
			SEG seg = TempPtn.Seg[CurrentSegNo];
			OnEdit->Visible = true;
			OnEdit->LabelCaption = "On";
			OnEdit->Text = 0;
			OffEdit->Visible = true;
			OffEdit->Text = seg.Time;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::EditorPanelClick(TObject *Sender)
{
	EditorPanel->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::OnSelectAll(TMessage &Message)
{
	TAdvEdit *edit = (TAdvEdit*)Message.WParam;
	edit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::ShowKeyboard(bool isNemeric)
{
	int x, y;

	if( isNemeric )
	{
		FmMain->Keyboard->KeyboardHeight = 162;
		FmMain->Keyboard->KeyboardWidth = 122;
		FmMain->Keyboard->KeyboardType = ktCELLPHONE;
		x = (1024 - FmMain->Keyboard->KeyboardWidth) / 2;
		x += FmMain->Left;
		FmMain->Keyboard->ShowAtXY(x, 200+FmMain->Top);
	}
	else
	{
		FmMain->Keyboard->KeyboardHeight = 202;
		FmMain->Keyboard->KeyboardWidth = 602;
		FmMain->Keyboard->KeyboardType = ktQWERTY;
		x = (1024 - FmMain->Keyboard->KeyboardWidth) / 2;
		x += FmMain->Left;
		y = 100+FmMain->Top;
		FmMain->Keyboard->ShowAtXY(x, y);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::InitButtons()
{
	String strFolder, strFile;
	strFolder.sprintf(L"%s%s%c\\",
					  GetRootFolder().c_str(),
					  L"PATTERN_CM_",
					  'A'+ChamberNo);
	for(int i=0; i<30; i++)
	{
		BtnPattern[i]->Down = false;
		strFile.sprintf(L"%d.ptn", i+1);
		if( FileExists(strFolder+strFile) )
		{
			BtnPattern[i]->Color = clLime;
			BtnPattern[i]->BorderInnerColor = clMenuHighlight;
			BtnPattern[i]->BorderColor = clSilver;
			BtnPattern[i]->BevelColor = clSkyBlue;
			BtnPattern[i]->BevelColorDown = clSkyBlue;
		}
		else
		{
			BtnPattern[i]->Color = clSilver;
			BtnPattern[i]->BorderInnerColor = clSilver;
			BtnPattern[i]->BorderColor = clSilver;
			BtnPattern[i]->BevelColor = clWhite;
			BtnPattern[i]->BevelColorDown = clWhite;
		}

		BtnPattern[i]->ColorDown = clYellow;
		BtnPattern[i]->ShowFocus = false;
		BtnPattern[i]->BevelWidth = 1;
		BtnPattern[i]->Tag = i+1;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::UpdateRecipeGrid()
{
	GrdRecipeEdit->BeginUpdate();
	InitRecipeGrid();

	if( GetOvenDB()->QueryRecipe(ChamberNo) )
	{
		GrdRecipeEdit->RowCount = GetOvenDB()->Query->RecordCount+1;

		String str;
		int ptnNo;
		for(int j=0; j<GetOvenDB()->Query->RecordCount; j++)
		{
			str = GetOvenDB()->Query->FieldByName("Recipe")->AsString;
			ptnNo = GetOvenDB()->Query->FieldByName("PtnNo")->AsInteger;
			GrdRecipeEdit->Cells[0][j+1] = str;
			GrdRecipeEdit->Cells[1][j+1] = ptnNo;
			GrdRecipeEdit->Cells[2][j+1] = GetOvenDB()->Query->FieldByName("CureTemp")->AsInteger;
			GrdRecipeEdit->Cells[3][j+1] = GetOvenDB()->Query->FieldByName("CureTime")->AsInteger;

			GetOvenDB()->Query->Next();
		}
	}
	GrdRecipeEdit->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::InitRecipeGrid()
{
	GrdRecipeEdit->Clear();
	GrdRecipeEdit->ColumnHeaders->Add("Recipe");
	GrdRecipeEdit->ColumnHeaders->Add("Pattern No.");
	GrdRecipeEdit->ColumnHeaders->Add("Temp.");
	GrdRecipeEdit->ColumnHeaders->Add("Time");

	for(int i=0; i<4; i++)
		GrdRecipeEdit->ColWidths[i] = 121;
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::InitEventInfoGrid()
{
	EventInfoGrid->Clear();
	EventInfoGrid->RowCount = 3;
	EventInfoGrid->ColCount = 3;
	EventInfoGrid->FixedRows = 2;
	EventInfoGrid->FixedCols = 0;
	EventInfoGrid->MergeCells(0, 0, EventInfoGrid->ColCount, 1);
	EventInfoGrid->ColumnHeaders->Add("Event Info.");

	EventInfoGrid->Cells[0][1] = "Event Type";
	EventInfoGrid->Cells[1][1] = "On";
	EventInfoGrid->Cells[2][1] = "Off / Offset";

	for(int i=0; i<EventInfoGrid->ColCount; i++)
		EventInfoGrid->ColWidths[i] = (EventInfoGrid->Width - EventInfoGrid->ColCount) / EventInfoGrid->ColCount;
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::InitSegInfoGrid()
{
	SegInfoGrid->Clear();
	SegInfoGrid->RowCount = 6;
	SegInfoGrid->ColCount = 2;
	SegInfoGrid->FixedRows = 1;
	SegInfoGrid->FixedCols = 1;

	SegInfoGrid->ControlLook->NoDisabledButtonLook = true;

	SegInfoGrid->MergeCells(0, 0, SegInfoGrid->ColCount, 1);
	SegInfoGrid->ColumnHeaders->Add("Segment Info.");

	SegInfoGrid->Cells[0][1] = "STC";
	SegInfoGrid->Cells[0][2] = "SSP";
	SegInfoGrid->Cells[0][3] = "TOTAL TIME";
	SegInfoGrid->Cells[0][4] = "TEMP.";
	SegInfoGrid->Cells[0][5] = "TIME";

	for(int i=0; i<SegInfoGrid->ColCount; i++)
		SegInfoGrid->ColWidths[i] = (SegInfoGrid->Width - SegInfoGrid->ColCount) / SegInfoGrid->ColCount;
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::InitChart()
{
	for(int i=0; i<TempGraph->SeriesCount(); i++)
		TempGraph->Series[i]->Clear();

	TempGraph->Legend->Visible = true;
	TempGraph->Foot->Text->Clear();

	TempGraph->LeftAxis->Title->Font->Size = 9;
	TempGraph->LeftAxis->Title->Font->Style = TFontStyles() << fsBold;
	TempGraph->LeftAxis->Title->Font->Name = "Tahoma";
	TempGraph->LeftAxis->Title->Font->Color = clBlue;
	TempGraph->LeftAxis->Title->Caption = L"Temp. (℃)";
	TempGraph->LeftAxis->Grid->Color = clGray;
	TempGraph->LeftAxis->Grid->Visible = true;
	TempGraph->LeftAxis->Grid->Style = psSolid;
	TempGraph->LeftAxis->Labels = true;
	TempGraph->LeftAxis->LabelsOnAxis = true;
	TempGraph->LeftAxis->LabelsSize = 25;
	TempGraph->LeftAxis->LabelStyle = talValue;
	TempGraph->LeftAxis->LabelsFont->Name = "Tahoma";
	TempGraph->LeftAxis->LabelsFont->Size = 9;
	TempGraph->LeftAxis->LabelsFont->Style = TFontStyles() << fsBold;
	TempGraph->LeftAxis->Ticks->Visible = false;
	TempGraph->LeftAxis->MinorTicks->Visible = false;
	TempGraph->LeftAxis->Automatic = false;
	TempGraph->LeftAxis->AutomaticMaximum = false;
	TempGraph->LeftAxis->AutomaticMinimum = false;
	TempGraph->LeftAxis->Increment = 50;

	TempGraph->BottomAxis->Title->Font->Size = 9;
	TempGraph->BottomAxis->Title->Font->Style = TFontStyles() << fsBold;
	TempGraph->BottomAxis->Title->Font->Name = "Tahoma";
	TempGraph->BottomAxis->Title->Font->Color = clBlue;
	TempGraph->BottomAxis->Title->Caption = "Time";
	TempGraph->BottomAxis->Grid->Color = clGray;
	TempGraph->BottomAxis->Grid->Visible = true;
	TempGraph->BottomAxis->Grid->Style = psSolid;
	TempGraph->BottomAxis->Labels = true;
	TempGraph->BottomAxis->LabelsOnAxis = true;
	TempGraph->BottomAxis->LabelsSize = 15;
	TempGraph->BottomAxis->LabelStyle = talAuto;
	TempGraph->BottomAxis->LabelsFont->Name = "Tahoma";
	TempGraph->BottomAxis->LabelsFont->Size = 9;
	TempGraph->BottomAxis->LabelsFont->Style = TFontStyles() << fsBold;
	TempGraph->BottomAxis->LabelsMultiLine = false;
	TempGraph->BottomAxis->Ticks->Visible = true;
	TempGraph->BottomAxis->MinorTicks->Visible = false;
	TempGraph->BottomAxis->DateTimeFormat = "hh:mm";
	TempGraph->BottomAxis->Automatic = false;
	TempGraph->BottomAxis->AutomaticMaximum = false;
	TempGraph->BottomAxis->AutomaticMinimum = false;

	TempGraph->RightAxis->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::InitSeries()
{
	TempSeries = new TLineSeries(TempGraph);
	TempSeries->Title = "Temp.";
	TempSeries->LegendTitle = "Temp.";
	TempSeries->XValues->DateTime = true;
	TempSeries->ParentChart = TempGraph;
	TempSeries->HorizAxis = aBottomAxis;
	TempSeries->VertAxis  = aLeftAxis;
	TempSeries->Pointer->Style = psCircle;
	TempSeries->Pointer->Color = clRed;
	TempSeries->Color = clRed;
	TempSeries->Pointer->Size  = 2;
	TempSeries->Pointer->Frame->Visible = false;
	TempSeries->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::FillCurSegTitle(int segCount)
{
	String str;
	if (segCount <= 0)
	{
		str = L"<P align=\"center\"><B><FONT size=\"12\" face=\"Tahoma\">Not Loaded</FONT></B></P>";
	}
	else
	{
		str.sprintf(L"<P align=\"center\"><B><FONT size=\"12\" face=\"Tahoma\">Segment No. : %d / %d</FONT></B></P>",
			CurrentSegNo + 1, segCount);
    }
	if (SegNoPanel->Text != str)
	{
		SegNoPanel->Text = str;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::FillTempGraph(int segCount)
{
	SEG seg = TempPtn.Seg[CurrentSegNo];

	// 온도 그래프 설정
	double startTime = 0, endTime = 0;
	double dInc = TDateTime(0, 1, 0, 0); 	// 단위 1분
	if (segCount <= 0)
	{
		startTime = 0.0;
		endTime = 0.0;

		PrevSegButton->Visible = false;
		NextSegButton->Visible = false;
	}
	else
	{
		for (int i=0; i < CurrentSegNo; i++)
		{
			startTime += TempPtn.Seg[i].Time * dInc;
			endTime += TempPtn.Seg[i].Time * dInc;
		}
		endTime += TempPtn.Seg[CurrentSegNo].Time * dInc;

		if (CurrentSegNo <= 0)
		{
			PrevSegButton->Visible = false;
		}
		else
		{
			PrevSegButton->Visible = true;
		}

		if (CurrentSegNo < segCount - 1)
		{
			NextSegButton->Visible = true;
		}
		else
		{
			NextSegButton->Visible = false;
		}
	}

	AreaStartTime = startTime;
	AreaEndTime = endTime;
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::FillSegInfoGrid()
{
	// 세그먼트 정보 그리드 설정
	SEG seg = TempPtn.Seg[CurrentSegNo];
	SegInfoGrid->Cells[1][1] = STC_NAME[TempPtn.Stc.Stc];
	SegInfoGrid->Cells[1][2] = TempPtn.Stc.Ssp;
	SegInfoGrid->Cells[1][3] = TempPtn.TotalTime;
	SegInfoGrid->Cells[1][4] = seg.Tsp;
	SegInfoGrid->Cells[1][5] = seg.Time;
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::FillEventInfoGrid()
{
	int eventCount = GetEventCount(CurrentSegNo);
	SEG seg = TempPtn.Seg[CurrentSegNo];
	String str;

	EventInfoGrid->ClearNormalRows(2, EventInfoGrid->RowCount-2);
	EventInfoGrid->RowCount = 2 + eventCount;
	EventInfoGrid->RemoveAllButtons();

	int eventKind;
	for (int i=0; i < eventCount; i++)
	{
		eventKind = seg.EvKind[i];
		if (eventKind == ET_PVHIGH)
		{
			if (seg.OnType[i] == 0)
			{
				eventKind =  ET_PVHIGH_CLEAR;
			}
			else if (seg.OnType[i] == 3)
			{
				eventKind =  ET_PVHIGH;
			}
		}
		else if (eventKind == ET_PVLOW)
		{
			if (seg.OnType[i] == 0)
			{
				eventKind =  ET_PVLOW_CLEAR;
			}
			else if (seg.OnType[i] == 4)
			{
				eventKind =  ET_PVLOW;
			}
		}

		EventInfoGrid->Cells[0][i+2] = GetEventName(eventKind);
		if (eventKind == ET_PVHIGH || eventKind == ET_PVLOW)
		{
			EventInfoGrid->Cells[1][i+2] = "";
			EventInfoGrid->Cells[2][i+2] = seg.OffPe[i];
		}
		else if (eventKind == ET_PVHIGH_CLEAR || eventKind == ET_PVLOW_CLEAR)
		{
			EventInfoGrid->Cells[1][i+2] = "";
			EventInfoGrid->Cells[2][i+2] = "";
		}
		else
		{
			EventInfoGrid->Cells[1][i+2] = seg.OnType[i];
			EventInfoGrid->Cells[2][i+2] = seg.OffPe[i];
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::FillSegAdd()
{
	// 우측 세그먼트, 이벤트 생성부 설정
	SEG seg = TempPtn.Seg[CurrentSegNo];
	STCComboBox->ItemIndex = TempPtn.Stc.Stc;
	SSPEdit->Text = TempPtn.Stc.Ssp;
	TimeEdit->Text = seg.Time;
	SPEdit->Text = seg.Tsp;
	if (GetManager()->DamperControlType != CYLINDER_DAMPER)
	{
		DamperEdit->Text = TempPtn.Damper[CurrentSegNo].OutPercent;
	}
	if (GetManager()->UseInverter)
	{
		InverterEdit->Text = TempPtn.InverterFrequency[CurrentSegNo];
	}

    UseLocalModeCheckBox->Checked = TempPtn.UseLocalMode;
    LocalTempEdit->Text = TempPtn.LocalTemp;
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::FillEventAdd()
{
	if (CurrentSegNo < 0 || CurrentEventIndex < 0)
	{
		return;
	}

	SEG seg = TempPtn.Seg[CurrentSegNo];
	int eventKind = seg.EvKind[CurrentEventIndex];
	if (eventKind > 0)
	{
		if (eventKind == ET_PVHIGH)
		{
			if (seg.OnType[CurrentEventIndex] == 0)
			{
				eventKind =  ET_PVHIGH_CLEAR;
			}
			else if (seg.OnType[CurrentEventIndex] == 3)
			{
				eventKind =  ET_PVHIGH;
			}
		}
		else if (eventKind == ET_PVLOW)
		{
			if (seg.OnType[CurrentEventIndex] == 0)
			{
				eventKind =  ET_PVLOW_CLEAR;
			}
			else if (seg.OnType[CurrentEventIndex] == 4)
			{
				eventKind =  ET_PVLOW;
			}
		}

		int eventIndex = GetEventIndex(eventKind);
		EventTypeComboBox->ItemIndex = eventIndex;
		if (eventKind == ET_PVHIGH || eventKind == ET_PVLOW)
		{
			OffEdit->Visible = false;
			OnEdit->Visible = true;
			OnEdit->LabelCaption = "Offset";
			OnEdit->Text = seg.OffPe[CurrentEventIndex];
		}
		else if (eventKind == ET_PVHIGH_CLEAR || eventKind == ET_PVLOW_CLEAR)
		{
			OnEdit->Visible = false;
			OffEdit->Visible = false;
		}
		else
		{
			OffEdit->Visible = true;
			OffEdit->LabelCaption = "Off";
			OffEdit->Text = seg.OffPe[CurrentEventIndex];
			OnEdit->Visible = true;
			OnEdit->LabelCaption = "On";
			OnEdit->Text = seg.OnType[CurrentEventIndex];
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::FillSegInfo()
{
	int segCount = TempPtn.Stc.SegCount;
	FillCurSegTitle(segCount);
	FillTempGraph(segCount);
	FillSegInfoGrid();
	FillEventInfoGrid();
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::UpdateChart()
{
	double dInc = TDateTime(0, 1, 0, 0); 	// 단위 1분
	int segCount = TempPtn.Stc.SegCount;
	int cureTime;
	double cureTemp, maxTemp;

	if (segCount > 0)
	{
		cureTime = TempPtn.TotalTime;
		cureTemp = TempPtn.MaxTemp;
	}
	else
	{
		cureTime = 180;
		cureTemp = 175.0;
	}
	maxTemp = cureTemp + 30.0;
	int div = (int)(maxTemp / 50.) + 1;
	maxTemp = 50 * div;
	MaxTemp = maxTemp;

	TempGraph->LeftAxis->SetMinMax(0, maxTemp + 10);
	TempGraph->BottomAxis->Increment = dInc*30;
	TempGraph->BottomAxis->SetMinMax(0, dInc*cureTime);

	TempSeries->Clear();
	TempSeries->AddXY(0, 0);

	double temp = 0, time = 0;
	for(int i=0; i<segCount; i++)
	{
		temp = TempPtn.Seg[i].Tsp;
		time += TempPtn.Seg[i].Time * dInc;
		TempSeries->AddXY(time, temp);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::UpdatePtnTimeTemp()
{
	int segCount = TempPtn.Stc.SegCount;
	int totalTime = 0;
	double maxTemp = 0.0;
	for(int i=0; i<segCount; i++)
	{
		totalTime += TempPtn.Seg[i].Time;
		if (maxTemp < TempPtn.Seg[i].Tsp)
		{
			maxTemp = TempPtn.Seg[i].Tsp;
		}
	}
	TempPtn.TotalTime = totalTime;
	TempPtn.MaxTemp = maxTemp;
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::PrevSegButtonClick(TObject *Sender)
{
	if (CurrentSegNo > 0)
	{
		CurrentSegNo--;
	}

//	FillSegInfo();
	CurrentEventIndex = 0;
	UpdateChart();
	FillSegInfo();
	FillSegAdd();
	FillEventAdd();
}
//---------------------------------------------------------------------------

void __fastcall TFrmPatternSetting::NextSegButtonClick(TObject *Sender)
{
	int segCount = TempPtn.Stc.SegCount;
	if (CurrentSegNo < segCount - 1)
	{
		CurrentSegNo++;
	}

	CurrentEventIndex = 0;
	UpdateChart();
	FillSegInfo();
	FillSegAdd();
	FillEventAdd();
}
//---------------------------------------------------------------------------

void __fastcall TFrmPatternSetting::SegInfoGridGetAlignment(TObject *Sender, int ARow,
		  int ACol, TAlignment &HAlign, TVAlignment &VAlign)
{
	HAlign = taCenter;
	VAlign = Advobj::vtaCenter;
}
//---------------------------------------------------------------------------

void __fastcall TFrmPatternSetting::EventInfoGridGetAlignment(TObject *Sender, int ARow,
		  int ACol, TAlignment &HAlign, TVAlignment &VAlign)
{
	HAlign = taCenter;
	VAlign = Advobj::vtaCenter;
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::EventInfoGridDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State)
{
	if (ARow >= 2)
	{
		int selectedRow = CurrentEventIndex + 2;
		if( selectedRow == ARow )
		{
			EventInfoGrid->Canvas->Brush->Color = clHighlight;
			EventInfoGrid->Canvas->Font->Color = clWhite;
		}
		else
		{
			EventInfoGrid->Canvas->Brush->Color = clWhite;
			EventInfoGrid->Canvas->Font->Color = clBlack;
        }
		EventInfoGrid->Canvas->FillRect(Rect);

		DrawText(EventInfoGrid->Canvas->Handle,
				 String(EventInfoGrid->Cells[ACol][ARow]).c_str(),
				 -1,
				 &Rect,
				 DT_SINGLELINE|DT_VCENTER|DT_CENTER); 

		if (ACol == 1)
		{
			int eventKind = TempPtn.Seg[CurrentSegNo].EvKind[ARow-2];
			if (eventKind == ET_PVHIGH || eventKind == ET_PVLOW ||
				eventKind == ET_PVHIGH_CLEAR || eventKind == ET_PVLOW_CLEAR)
			{
				EventInfoGrid->Canvas->Brush->Color = clSilver;
				EventInfoGrid->Canvas->FillRect(Rect);
			}
		}
		else if (ACol == 2)
		{
			int eventKind = TempPtn.Seg[CurrentSegNo].EvKind[ARow-2];
			if ((eventKind == ET_PVHIGH && TempPtn.Seg[CurrentSegNo].OnType[ARow-2] == 0) ||
				(eventKind == ET_PVLOW && TempPtn.Seg[CurrentSegNo].OnType[ARow-2] == 0))
			{
				EventInfoGrid->Canvas->Brush->Color = clSilver;
				EventInfoGrid->Canvas->FillRect(Rect);
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::EventInfoGridClickCell(TObject *Sender, int ARow,
          int ACol)
{
	if (ARow >= EventInfoGrid->FixedRows)
	{
		CurrentEventIndex = ARow - EventInfoGrid->FixedRows;
		FillEventAdd();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::AddSegmentButtonClick(TObject *Sender)
{
	if (SelectedPtnNo <= 0)
	{
		SetMessageStr(GetResStr("Warning", "SelectPattern"));
		ShowMessageDlg();
		return;
	}

	if (SPEdit->Text.IsEmpty())
	{
		SetMessageStr(GetResStr("Warning", "EmptyItemExist"));
		ShowMessageDlg();
		return;
	}

	if (TimeEdit->Text.IsEmpty())
	{
		SetMessageStr(GetResStr("Warning", "EmptyItemExist"));
		ShowMessageDlg();
		return;
	}

	if (SSPEdit->Text.IsEmpty())
	{
		SetMessageStr(GetResStr("Warning", "EmptyItemExist"));
		ShowMessageDlg();
		return;
	}

	if (GetManager()->DamperControlType != CYLINDER_DAMPER)
	{
		if (DamperEdit->Text.IsEmpty())
		{
			SetMessageStr(GetResStr("Warning", "EmptyItemExist"));
			ShowMessageDlg();
			return;
		}
	}

	if (GetManager()->UseInverter)
	{
		if (InverterEdit->Text.IsEmpty())
		{
			SetMessageStr(GetResStr("Warning", "EmptyItemExist"));
			ShowMessageDlg();
			return;
		}
	}

	{
		double min = InverterEdit->MinFloatValue;
		double max = InverterEdit->MaxFloatValue;
		double value = InverterEdit->Text.ToDouble();
		if (value < min || value > max)
		{
			String str;
			SetMessageStr(GetResStr("Warning", "MaxLimitOver"));
			str.sprintf(L"[Inverter Input Range : %.2f ~ %.2f]", min, max);
			SetMessageStr(str);
			ShowMessageDlg();
			return;
		}
	}

	{
		int min = TimeEdit->MinValue;
		int max = TimeEdit->MaxValue;
		int value = TimeEdit->Text.ToInt();
		if (value < min || value > max)
		{
			String str;
			SetMessageStr(GetResStr("Warning", "MaxLimitOver"));
			str.sprintf(L"[Time Input Range : %d ~ %d]", min, max);
			SetMessageStr(str);
			ShowMessageDlg();
			return;
		}
	}

	ApplyPatternInfo();
	ApplySegmentInfo(true);

	int segCount = TempPtn.Stc.SegCount;
	if (segCount == 0)
	{
		CurrentSegNo = 0;
		TempPtn.Seg[CurrentSegNo] = CurrentSeg;
		TempPtn.Damper[CurrentSegNo] = CurrentDamper;
		TempPtn.InverterFrequency[CurrentSegNo] = CurrentInverterFreq;
		TempPtn.Stc.SegCount++;
	}
	else
	{
		// 다음 세그부터 마지막 세그까지 한 단계 뒤로 이동한 후 다음 세그먼트에
		// 새 세그먼트를 삽입한다.
		CurrentSegNo++;
		for (int i = segCount; i > CurrentSegNo; i--)
		{
			TempPtn.Seg[i] = TempPtn.Seg[i - 1];
			TempPtn.Damper[i] = TempPtn.Damper[i - 1];
		}
		TempPtn.Seg[CurrentSegNo] = CurrentSeg;
		TempPtn.Damper[CurrentSegNo] = CurrentDamper;
		TempPtn.InverterFrequency[CurrentSegNo] = CurrentInverterFreq;
		TempPtn.Stc.SegCount++;
	}

	UpdatePtnTimeTemp();
	UpdateChart();
	FillSegInfo();
	FillSegAdd();
	FillEventAdd();
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::UpdateSegmentButtonClick(TObject *Sender)
{
	if (SelectedPtnNo <= 0)
	{
		SetMessageStr(GetResStr("Warning", "SelectPattern"));
		ShowMessageDlg();
		return;
	}

	if (SPEdit->Text.IsEmpty())
	{
		SetMessageStr(GetResStr("Warning", "EmptyItemExist"));
		ShowMessageDlg();
		return;
	}

	if (TimeEdit->Text.IsEmpty())
	{
		SetMessageStr(GetResStr("Warning", "EmptyItemExist"));
		ShowMessageDlg();
		return;
	}

	if (SSPEdit->Text.IsEmpty())
	{
		SetMessageStr(GetResStr("Warning", "EmptyItemExist"));
		ShowMessageDlg();
		return;
	}

	if (GetManager()->DamperControlType != CYLINDER_DAMPER)
	{
		if (DamperEdit->Text.IsEmpty())
		{
			SetMessageStr(GetResStr("Warning", "EmptyItemExist"));
			ShowMessageDlg();
			return;
		}
	}

	if (GetManager()->UseInverter)
	{
		if (InverterEdit->Text.IsEmpty())
		{
			SetMessageStr(GetResStr("Warning", "EmptyItemExist"));
			ShowMessageDlg();
			return;
		}
	}

	int segCount = TempPtn.Stc.SegCount;
	if (segCount <= 0)
	{
		SetMessageStr(GetResStr("Warning", "NoSegment"));
		ShowMessageDlg();
		return;
	}

	{
		double min = InverterEdit->MinFloatValue;
		double max = InverterEdit->MaxFloatValue;
		double value = InverterEdit->Text.ToDouble();
		if (value < min || value > max)
		{
			String str;
			SetMessageStr(GetResStr("Warning", "MaxLimitOver"));
			str.sprintf(L"[Inverter Input Range : %.2f ~ %.2f]", min, max);
			SetMessageStr(str);
			ShowMessageDlg();
			return;
		}
	}

	{
		int min = TimeEdit->MinValue;
		int max = TimeEdit->MaxValue;
		int value = TimeEdit->Text.ToInt();
		if (value < min || value > max)
		{
			String str;
			SetMessageStr(GetResStr("Warning", "MaxLimitOver"));
			str.sprintf(L"[Time Input Range : %d ~ %d]", min, max);
			SetMessageStr(str);
			ShowMessageDlg();
			return;
		}
	}

	ApplyPatternInfo();
	ApplySegmentInfo(false);
	TempPtn.Seg[CurrentSegNo] = CurrentSeg;
	TempPtn.Damper[CurrentSegNo] = CurrentDamper;
	TempPtn.InverterFrequency[CurrentSegNo] = CurrentInverterFreq;

	UpdatePtnTimeTemp();
	UpdateChart();
	FillSegInfo();
	FillSegAdd();
	FillEventAdd();

    SetMessageStr(GetResStr("Message", "UpdateDone"));
    ShowMessageDlg();
}
//---------------------------------------------------------------------------

void __fastcall TFrmPatternSetting::UpdateSTCButtonClick(TObject *Sender)
{
	if (SelectedPtnNo <= 0)
	{
		SetMessageStr(GetResStr("Warning", "SelectPattern"));
		ShowMessageDlg();
		return;
	}

	if (SSPEdit->Text.IsEmpty())
	{
		SetMessageStr(GetResStr("Warning", "EmptyItemExist"));
		ShowMessageDlg();
		return;
	}

    if (UseLocalModeCheckBox->Checked)
    {
        if (LocalTempEdit->Text.IsEmpty())
        {
            SetMessageStr(GetResStr("Warning", "EmptyItemExist"));
            ShowMessageDlg();
            return;
        }
    }

	ApplyPatternInfo();
	FillSegInfo();

    SetMessageStr(GetResStr("Message", "UpdateDone"));
    ShowMessageDlg();
}
//---------------------------------------------------------------------------

void __fastcall TFrmPatternSetting::DeleteSegmentButtonClick(TObject *Sender)
{
	if (SelectedPtnNo <= 0)
	{
		SetMessageStr(GetResStr("Warning", "SelectPattern"));
		ShowMessageDlg();
		return;
	}

	int segCount = TempPtn.Stc.SegCount;
	if (segCount <= 0)
	{
		SetMessageStr(GetResStr("Warning", "NoSegment"));
		ShowMessageDlg();
		return;
	}

	SEG emptySeg;
	for (int i = CurrentSegNo+1; i < segCount; i++)
	{
		TempPtn.Seg[i-1] = TempPtn.Seg[i];
	}
	// 맨 뒤에 있는 것을 0으로 만든다.
	TempPtn.Seg[segCount-1] = emptySeg;
	if (CurrentSegNo > 0 && CurrentSegNo == segCount-1)
	{
		CurrentSegNo--;
	}
	TempPtn.Stc.SegCount--;

	UpdatePtnTimeTemp();
	UpdateChart();
	FillSegInfo();
	FillSegAdd();
	FillEventAdd();
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::AddEventButtonClick(TObject *Sender)
{
	if (SelectedPtnNo <= 0)
	{
		SetMessageStr(GetResStr("Warning", "SelectPattern"));
		ShowMessageDlg();
		return;
	}

	if (OnEdit->Text.IsEmpty())
	{
		SetMessageStr(GetResStr("Warning", "EmptyItemExist"));
		ShowMessageDlg();
		return;
	}

	if (OffEdit->Text.IsEmpty())
	{
		SetMessageStr(GetResStr("Warning", "EmptyItemExist"));
		ShowMessageDlg();
		return;
	}

	int eventCount = GetEventCount(CurrentSegNo);
	if (eventCount >= 8)
	{
		SetMessageStr(GetResStr("Warning", "Full"));
		ShowMessageDlg();
		return;
	}
	else if (eventCount == 0)
	{
		CurrentEventIndex = 0;
	}
	else
	{
		// 다음 이벤트부터 마지막 이벤트까지 한 단계 뒤로 이동한 후
		// 다음 이벤트에 새 이벤트를 삽입한다.
		CurrentEventIndex++;
		for (int i = eventCount; i > CurrentEventIndex; i--)
		{
			TempPtn.Seg[CurrentSegNo].EvKind[i] = TempPtn.Seg[CurrentSegNo].EvKind[i-1];
			TempPtn.Seg[CurrentSegNo].OnType[i] = TempPtn.Seg[CurrentSegNo].OnType[i-1];
			TempPtn.Seg[CurrentSegNo].OffPe[i] = TempPtn.Seg[CurrentSegNo].OffPe[i-1];
		}
	}

	ApplyEventInfo();
	FillSegInfo();
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::UpdateEventButtonClick(TObject *Sender)
{
	if (SelectedPtnNo <= 0)
	{
		SetMessageStr(GetResStr("Warning", "SelectPattern"));
		ShowMessageDlg();
		return;
	}

	if (OnEdit->Text.IsEmpty())
	{
		SetMessageStr(GetResStr("Warning", "EmptyItemExist"));
		ShowMessageDlg();
		return;
	}

	if (OffEdit->Text.IsEmpty())
	{
		SetMessageStr(GetResStr("Warning", "EmptyItemExist"));
		ShowMessageDlg();
		return;
	}

	int segCount = TempPtn.Stc.SegCount;
	if (segCount <= 0)
	{
		SetMessageStr(GetResStr("Warning", "NoSegment"));
		ShowMessageDlg();
		return;
	}

	int eventCount = GetEventCount(CurrentSegNo);
	if (eventCount <= 0)
	{
		SetMessageStr(GetResStr("Warning", "NoEvent"));
		ShowMessageDlg();
		return;
	}

	ApplyEventInfo();
	FillSegInfo();

    SetMessageStr(GetResStr("Message", "UpdateDone"));
    ShowMessageDlg();
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::DeleteEventButtonClick(TObject *Sender)
{
	if (SelectedPtnNo <= 0)
	{
		SetMessageStr(GetResStr("Warning", "SelectPattern"));
		ShowMessageDlg();
		return;
	}

	int segCount = TempPtn.Stc.SegCount;
	if (segCount <= 0)
	{
		SetMessageStr(GetResStr("Warning", "NoSegment"));
		ShowMessageDlg();
		return;
	}

	int eventCount = GetEventCount(CurrentSegNo);
	if (eventCount <= 0)
	{
		SetMessageStr(GetResStr("Warning", "NoEvent"));
		ShowMessageDlg();
		return;
	}

	for (int i = CurrentEventIndex+1; i < eventCount; i++)
	{
		TempPtn.Seg[CurrentSegNo].EvKind[i-1] = TempPtn.Seg[CurrentSegNo].EvKind[i];
		TempPtn.Seg[CurrentSegNo].OnType[i-1] = TempPtn.Seg[CurrentSegNo].OnType[i];
		TempPtn.Seg[CurrentSegNo].OffPe[i-1] = TempPtn.Seg[CurrentSegNo].OffPe[i];
	}
	// 맨 뒤에 있는 것을 0으로 만든다.
	TempPtn.Seg[CurrentSegNo].EvKind[eventCount-1] = 0;
	TempPtn.Seg[CurrentSegNo].OnType[eventCount-1] = 0;
	TempPtn.Seg[CurrentSegNo].OffPe[eventCount-1] = 0;
	if (CurrentEventIndex > 0 && CurrentEventIndex == eventCount-1)
	{
		CurrentEventIndex--;
	}

	UpdatePtnTimeTemp();
	UpdateChart();
	FillSegInfo();
	FillSegAdd();
	FillEventAdd();
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::ApplyPatternInfo()
{
	TempPtn.Stc.Stc = STCComboBox->ItemIndex;
	TempPtn.Stc.Ssp = SSPEdit->Text.ToDouble();
    TempPtn.UseLocalMode = UseLocalModeCheckBox->Checked;
    TempPtn.LocalTemp = LocalTempEdit->Text.ToDouble();
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::ApplySegmentInfo(bool isNewSeg)
{
	SEG seg;
	if (isNewSeg == false)
		seg = TempPtn.Seg[CurrentSegNo];
	seg.Tsp = SPEdit->Text.ToDouble();
	seg.Time = TimeEdit->Text.ToInt();
	seg.Pid = 1;
	CurrentSeg = seg;

	if (GetManager()->DamperControlType != CYLINDER_DAMPER)
	{
		int value = DamperEdit->Text.ToInt();
		CurrentDamper.InPercent = value;
		CurrentDamper.OutPercent = value;
	}

	if (GetManager()->UseInverter)
	{
		CurrentInverterFreq = InverterEdit->Text.ToDouble();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmPatternSetting::ApplyEventInfo()
{
	int eventKind = GetEventType(EventTypeComboBox->ItemIndex);
	String str;

	TempPtn.Seg[CurrentSegNo].EvKind[CurrentEventIndex] = eventKind;
	if (eventKind == ET_PVHIGH_CLEAR)
	{
		TempPtn.Seg[CurrentSegNo].EvKind[CurrentEventIndex] = ET_PVHIGH;
		TempPtn.Seg[CurrentSegNo].OnType[CurrentEventIndex] = 0;
		TempPtn.Seg[CurrentSegNo].OffPe[CurrentEventIndex] = 0;
	}
	else if (eventKind == ET_PVLOW_CLEAR)
	{
		TempPtn.Seg[CurrentSegNo].EvKind[CurrentEventIndex] = ET_PVLOW;
		TempPtn.Seg[CurrentSegNo].OnType[CurrentEventIndex] = 0;
		TempPtn.Seg[CurrentSegNo].OffPe[CurrentEventIndex] = 0;
	}
	else if (eventKind == ET_PVHIGH)
	{
		TempPtn.Seg[CurrentSegNo].OnType[CurrentEventIndex] = 3;
		str = OnEdit->Text;
		TempPtn.Seg[CurrentSegNo].OffPe[CurrentEventIndex] = str.ToInt();
	}
	else if (eventKind == ET_PVLOW)
	{
		TempPtn.Seg[CurrentSegNo].OnType[CurrentEventIndex] = 4;
		str = OnEdit->Text;
		TempPtn.Seg[CurrentSegNo].OffPe[CurrentEventIndex] = str.ToInt();
	}
	else
	{
		TempPtn.Seg[CurrentSegNo].OnType[CurrentEventIndex] = OnEdit->Text.ToInt();
		TempPtn.Seg[CurrentSegNo].OffPe[CurrentEventIndex] = OffEdit->Text.ToInt();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrmPatternSetting::TempGraphClick(TObject *Sender)
{
	double x;
	int time;
	double minute = TDateTime(0, 1, 0, 0);

	TPoint cursorPos, pos;
	GetCursorPos(&cursorPos);
	pos = TempGraph->ScreenToClient(cursorPos);
	x = TempSeries->XScreenToValue(pos.X);

	int segNo = GetSegNoByPosX(pos.X);
	if (segNo >= 0)
	{
		CurrentSegNo = segNo;
		CurrentEventIndex = 0;
		UpdateChart();
		FillSegInfo();
		FillSegAdd();
		FillEventAdd();
	}
}
//---------------------------------------------------------------------------


void __fastcall TFrmPatternSetting::TempGraphBeforeDrawAxes(TObject *Sender)
{
	TRect rc;
	rc.Left = TempSeries->CalcXPosValue(AreaStartTime);
	rc.Top = TempSeries->CalcYPosValue(MaxTemp);
	rc.Right = TempSeries->CalcXPosValue(AreaEndTime);
	rc.Bottom = TempSeries->CalcYPosValue(0);

	TempGraph->Canvas->Brush->Color = clAqua;
	TempGraph->Canvas->Rectangle(rc, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFrmPatternSetting::TempGraphBeforeDrawSeries(TObject *Sender)
{
	int x0, x1, y0, y1;
	int xStart, xEnd;
	double yMin, yMax;

	yMin = TempGraph->LeftAxis->Minimum;
	yMax = TempGraph->LeftAxis->Maximum;

	for (int i = 1; i < TempSeries->XValues->Count - 1; i++)
	{
		x0 = x1 = TempSeries->CalcXPosValue(TempSeries->XValues->Items[i]);
		y0 = TempSeries->CalcYPosValue(yMax);
		y1 = TempSeries->CalcYPosValue(yMin);
		TempGraph->Canvas->Pen->Style = psDot;
		TempGraph->Canvas->Pen->Color = clSkyBlue;
		TempGraph->Canvas->Line(x0, y0, x1, y1);
	}
}
//---------------------------------------------------------------------------

