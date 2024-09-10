//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TempGraphFrame.h"
#include "Database.h"
#include "JobForm.h"
#include "HelperFunctions.h"
#include "NormalOvenSystemManager.h"
#include "NormalOvenChamber.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvEdit"
#pragma link "AdvGlassButton"
#pragma link "AdvOfficeButtons"
#pragma link "AdvSmoothCalendar"
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "AdvPanel"
#pragma link "BaseGrid"
#pragma link "FolderDialog"
#pragma link "AdvOfficeImage"
#pragma link "AdvPicture"
#pragma link "AdvUtil"
#pragma resource "*.dfm"
TFrmTempGraph *FrmTempGraph;
//---------------------------------------------------------------------------

__fastcall TFrmTempGraph::TFrmTempGraph(TComponent* Owner)
	: TFrame(Owner)
{
	for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++)
	{
		Srs_Ch[i] = nullptr;
	}
	DisplayAbsTime = false;
	Init();
}
//---------------------------------------------------------------------------
__fastcall TFrmTempGraph::TFrmTempGraph(TComponent* Owner, int chamberNo)
	: TFrame(Owner)
{
	Chamber = GetChamber(chamberNo);
	Init();

	String str;
	str.sprintf(L"Chamber %c", chamberNo + 'A');
	Hint = str;
}
//---------------------------------------------------------------------------
__fastcall TFrmTempGraph::~TFrmTempGraph()
{
	delete SrsMainTempPv;
	delete SrsMainTempSp;
	delete SrsMainTempOut;
	delete SrsLimit;
	delete SrsO2;
	for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++)
	{
		delete Srs_Ch[i];
    }
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::Localize()
{
	ProfileBackupButton->Caption = GetResStr("Graph", "BackupProfile");
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::Init()
{
	String str;
	for (int i=0; i < MAX_LOGGERPOINT_COUNT; i++)
	{
		str.sprintf(L"lblCh%d", i+1);
		Ch[i] = (TAdvEdit*)FindComponent(str);
	}

	InitGrid();
	InitSeries();
	InitChart(Now(), 180, 500);
	CurrentMouseX = -1;

	LotInfoGrid->ColWidths[0] = 30;
	LotInfoGrid->ColWidths[1] = 132;
	LotInfoGrid->ColWidths[2] = 50;

	LotInfoGrid->Cells[0][0] = "No.";
	LotInfoGrid->Cells[1][0] = "Lot ID";
	LotInfoGrid->Cells[2][0] = "Q'ty";
	for(int j=0; j<LOT_COUNT; j++)
		LotInfoGrid->Cells[0][j+1] = IntToStr(j+1);

	Localize();

	Calendar->DoubleBuffered = true;
	Calendar->SelectedDate = Now().CurrentDate();
	CalendarMonthChanged(nullptr, Calendar->CurrentMonth());
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::InitGrid()
{
	WorkGrid->Columns->Clear();
	// 20180514 - ProductCode empty save - jhlim
	//for(int i=0; i<6; i++)
	for(int i=0; i<26; i++)
	{
		WorkGrid->Columns->Add();
		WorkGrid->Columns->Items[i]->Title->Font->Size = 10;
		WorkGrid->Columns->Items[i]->Title->Font->Style = TFontStyles() << fsBold;
		WorkGrid->Columns->Items[i]->Title->Alignment = taCenter;
		WorkGrid->Columns->Items[i]->Font->Size = 10;
		WorkGrid->Columns->Items[i]->Alignment = taCenter;
	}

	WorkGrid->Columns->Items[0]->Title->Caption = GetResStr("Graph", "StartTime");
	WorkGrid->Columns->Items[0]->FieldName = "StartTime";
	WorkGrid->Columns->Items[1]->Title->Caption = GetResStr("Graph", "EndTime");
	WorkGrid->Columns->Items[1]->FieldName = "EndTime";
	WorkGrid->Columns->Items[2]->Title->Caption = GetResStr("Graph", "Recipe");
	WorkGrid->Columns->Items[2]->FieldName = "Recipe";
	WorkGrid->Columns->Items[3]->Title->Caption = GetResStr("Graph", "PatternNo");
	WorkGrid->Columns->Items[3]->FieldName = "PtnNo";
	WorkGrid->Columns->Items[4]->Title->Caption = GetResStr("Graph", "UserId");
	WorkGrid->Columns->Items[4]->FieldName = "UserID";
	WorkGrid->Columns->Items[5]->Title->Caption = "ProductCode";
	WorkGrid->Columns->Items[5]->FieldName = "ProductCode";

	WorkGrid->Columns->Items[0]->Width = 180;
	WorkGrid->Columns->Items[1]->Width = 180;
	WorkGrid->Columns->Items[2]->Width = 80;
	WorkGrid->Columns->Items[3]->Width = 100;
	WorkGrid->Columns->Items[4]->Width = 100;
	WorkGrid->Columns->Items[5]->Width = 110;

	// 20180514 - ProductCode empty save - jhlim
	int nLotID = 0;
	String str;
	for(int i=6; i<26; i++)
	{
		str.sprintf(L"LOTID_%d", nLotID+1);
		WorkGrid->Columns->Items[i]->Title->Caption = str;
		str.sprintf(L"LotID_%d", nLotID);
		WorkGrid->Columns->Items[i]->FieldName = str;
		WorkGrid->Columns->Items[i]->Width = 110;
		nLotID++;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::InitChart(TDateTime dtStart,
										int cureTime,
										int maxTemp)
{
	TempChart->Title->Clear();
	TempChart->Title->Caption = "";
	TempChart->View3D = false;
	TempChart->Color = clWhite;

	for(int i=0; i<TempChart->SeriesCount(); i++)
		TempChart->Series[i]->Clear();

	TempChart->Legend->Visible = true;

	TempChart->Foot->Text->Clear();

	TempChart->LeftAxis->Title->Font->Size = 9;
	TempChart->LeftAxis->Title->Font->Style = TFontStyles() << fsBold;
	TempChart->LeftAxis->Title->Font->Name = "Tahoma";
	TempChart->LeftAxis->Title->Font->Color = clBlue;
	TempChart->LeftAxis->Title->Caption = L"Temp. (℃)";
	TempChart->LeftAxis->Grid->Color = clGray;
	TempChart->LeftAxis->Grid->Visible = true;
	TempChart->LeftAxis->Grid->Style = psSolid;
	TempChart->LeftAxis->Labels = true;
	TempChart->LeftAxis->LabelsOnAxis = true;
	TempChart->LeftAxis->LabelsSize = 25;
	TempChart->LeftAxis->LabelStyle = talValue;
	TempChart->LeftAxis->LabelsFont->Name = "Tahoma";
	TempChart->LeftAxis->LabelsFont->Size = 10;
	TempChart->LeftAxis->LabelsFont->Style = TFontStyles() << fsBold;
	TempChart->LeftAxis->Ticks->Visible = false;
	TempChart->LeftAxis->MinorTicks->Visible = false;
	TempChart->LeftAxis->Automatic = false;
	TempChart->LeftAxis->AutomaticMaximum = false;
	TempChart->LeftAxis->AutomaticMinimum = false;
	TempChart->LeftAxis->Increment = 10;
	TempChart->LeftAxis->SetMinMax(0, maxTemp + 10);

	if( GetManager()->UseO2Analyzer )
	{
		TempChart->RightAxis->Title->Font->Size = 9;
		TempChart->RightAxis->Title->Font->Style = TFontStyles() << fsBold;
		TempChart->RightAxis->Title->Font->Name = "Tahoma";
		TempChart->RightAxis->Title->Font->Color = clBlue;
		TempChart->RightAxis->Grid->Color = clGray;
		TempChart->RightAxis->Grid->Visible = true;
		TempChart->RightAxis->Grid->Style = psSolid;
		TempChart->RightAxis->Labels = true;
		TempChart->RightAxis->LabelsOnAxis = true;
		TempChart->RightAxis->LabelsSize = 25;
		TempChart->RightAxis->LabelStyle = talValue;
		TempChart->RightAxis->LabelsFont->Name = "Tahoma";
		TempChart->RightAxis->LabelsFont->Size = 10;
		TempChart->RightAxis->LabelsFont->Style = TFontStyles() << fsBold;
		TempChart->RightAxis->LabelsAngle = 0;
		TempChart->RightAxis->Ticks->Visible = false;
		TempChart->RightAxis->MinorTicks->Visible = false;
		TempChart->RightAxis->Automatic = false;
		TempChart->RightAxis->AutomaticMaximum = false;
		TempChart->RightAxis->AutomaticMinimum = false;
		TempChart->RightAxis->Increment = 10;
		TempChart->RightAxis->Title->Caption = "O2 (ppm)";
		TempChart->RightAxis->Logarithmic = false;
		TempChart->RightAxis->LabelsSize = 40;
		TempChart->RightAxis->LabelsSeparation = 10;
		TempChart->RightAxis->SetMinMax(0, 200);
	}

	TempChart->BottomAxis->Title->Font->Size = 9;
	TempChart->BottomAxis->Title->Font->Style = TFontStyles() << fsBold;
	TempChart->BottomAxis->Title->Font->Name = "Tahoma";
	TempChart->BottomAxis->Title->Font->Color = clBlue;
	TempChart->BottomAxis->Title->Caption = "Time";
	TempChart->BottomAxis->Grid->Color = clGray;
	TempChart->BottomAxis->Grid->Visible = true;
	TempChart->BottomAxis->Grid->Style = psSolid;
	TempChart->BottomAxis->Labels = true;
	TempChart->BottomAxis->LabelsOnAxis = true;
	TempChart->BottomAxis->LabelsSize = 15;
	TempChart->BottomAxis->LabelStyle = talAuto;
	TempChart->BottomAxis->LabelsFont->Name = "Tahoma";
	TempChart->BottomAxis->LabelsFont->Size = 10;
	TempChart->BottomAxis->LabelsFont->Style = TFontStyles() << fsBold;
	TempChart->BottomAxis->LabelsMultiLine = false;
	TempChart->BottomAxis->Ticks->Visible = true;
	TempChart->BottomAxis->MinorTicks->Visible = false;
	if( DisplayAbsTime )
		TempChart->BottomAxis->DateTimeFormat = "";
	else
		TempChart->BottomAxis->DateTimeFormat = "hh:mm";
	TempChart->BottomAxis->Automatic = false;
	TempChart->BottomAxis->AutomaticMaximum = false;
	TempChart->BottomAxis->AutomaticMinimum = false;

	double dInc = TDateTime(0, 1, 0, 0); 	// 단위 1분
	int step;

	if( cureTime<60 )
		step = 1;
	else
	if( cureTime<360 )
		step = 10;
	else
		step = 15;
	TempChart->BottomAxis->Increment = dInc*step;
	if( DisplayAbsTime )
		TempChart->BottomAxis->SetMinMax(0 + (double)dtStart, dInc*cureTime + (double)dtStart);
	else
		TempChart->BottomAxis->SetMinMax(0, dInc*cureTime);
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::InitSeries()
{
	int ch = 0;
	String str;

	if( GetManager()->LoginLevel == LV_DEVELOPER )
	{
		lblSp->Visible = true;
		lblOut->Visible = true;
		lblLimit->Visible = true;
	}
	else
	{
		lblSp->Visible = false;
		lblOut->Visible = false;
		lblLimit->Visible = false;
	}

	for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++)
	{
		if( Chamber->UseChannel[i] )
		{
			Srs_Ch[ch] = new TPointSeries(TempChart);
			Srs_Ch[ch]->XValues->DateTime = true;
			Srs_Ch[ch]->ParentChart = TempChart;
			Srs_Ch[ch]->HorizAxis = aBottomAxis;
			Srs_Ch[ch]->VertAxis = aLeftAxis;
			Srs_Ch[ch]->Pointer->Style = psCircle;
			Srs_Ch[ch]->Pointer->Color = TColor(Colors[ch]);
			Srs_Ch[ch]->Color = TColor(Colors[ch]);
			Srs_Ch[ch]->Pointer->Size = 2;
			Srs_Ch[ch]->Pointer->Frame->Visible = false;
			Srs_Ch[ch]->Visible = true;
			str.sprintf(L"Ch. %d", ch+1);
			Srs_Ch[ch]->LegendTitle = str;
			Srs_Ch[ch]->Title = str;

			Ch[ch]->Visible = true;
			Ch[ch]->LabelCaption = str;
			ch++;
		}
	}

	if( GetManager()->LoginLevel == LV_DEVELOPER )
	{
		SrsLimit = new TPointSeries(TempChart);
		SrsLimit->Title = "Limit";
		SrsLimit->LegendTitle = "Limit";
		SrsLimit->XValues->DateTime = true;
		SrsLimit->ParentChart = TempChart;
		SrsLimit->HorizAxis = aBottomAxis;
		SrsLimit->VertAxis  = aLeftAxis;
		SrsLimit->Pointer->Style = psCircle;
		SrsLimit->Pointer->Color = TColor(0x785219);
		SrsLimit->Color = TColor(0x785219);
		SrsLimit->Pointer->Size  = 2;
		SrsLimit->Pointer->Frame->Visible = false;
		SrsLimit->Visible = true;

		SrsMainTempOut = new TPointSeries(TempChart);
		SrsMainTempOut->Title = "Out";
		SrsMainTempOut->LegendTitle = "Out";
		SrsMainTempOut->XValues->DateTime = true;
		SrsMainTempOut->ParentChart = TempChart;
		SrsMainTempOut->HorizAxis = aBottomAxis;
		SrsMainTempOut->VertAxis  = aLeftAxis;
		SrsMainTempOut->Pointer->Style = psCircle;
		SrsMainTempOut->Pointer->Color = clGray;
		SrsMainTempOut->Color = clGray;
		SrsMainTempOut->Pointer->Size  = 2;
		SrsMainTempOut->Pointer->Frame->Visible = false;
		SrsMainTempOut->Visible = true;

		SrsMainTempSp = new TPointSeries(TempChart);
		SrsMainTempSp->Title = "SP";
		SrsMainTempSp->LegendTitle = "SP";
		SrsMainTempSp->XValues->DateTime = true;
		SrsMainTempSp->ParentChart = TempChart;
		SrsMainTempSp->HorizAxis = aBottomAxis;
		SrsMainTempSp->VertAxis  = aLeftAxis;
		SrsMainTempSp->Pointer->Style = psCircle;
		SrsMainTempSp->Pointer->Color = clAqua;
		SrsMainTempSp->Color = clAqua;
		SrsMainTempSp->Pointer->Size  = 2;
		SrsMainTempSp->Pointer->Frame->Visible = false;
		SrsMainTempSp->Visible = true;
	}

	if( GetManager()->UseO2Analyzer )
	{
		SrsO2 = new TPointSeries(TempChart);
		SrsO2->Title = "O2";
		SrsO2->LegendTitle = "O2";
		SrsO2->XValues->DateTime = true;
		SrsO2->ParentChart = TempChart;
		SrsO2->HorizAxis = aBottomAxis;
		SrsO2->VertAxis  = aRightAxis;
		SrsO2->Pointer->Style = psCircle;
		SrsO2->Pointer->Color = TColor(Colors[MAX_COLOR_COUNT-2]);
		SrsO2->Color = TColor(Colors[MAX_COLOR_COUNT-2]);
		SrsO2->Pointer->Size  = 2;
		SrsO2->Pointer->Frame->Visible = false;
		SrsO2->Visible = true;
	}

	SrsMainTempPv = new TPointSeries(TempChart);
	SrsMainTempPv->Title = "PV";
	SrsMainTempPv->LegendTitle = "PV";
	SrsMainTempPv->XValues->DateTime = true;
	SrsMainTempPv->ParentChart = TempChart;
	SrsMainTempPv->HorizAxis = aBottomAxis;
	SrsMainTempPv->VertAxis  = aLeftAxis;
	SrsMainTempPv->Pointer->Style = psCircle;
	SrsMainTempPv->Pointer->Color = TColor(Colors[MAX_COLOR_COUNT-1]);
	SrsMainTempPv->Color = TColor(Colors[MAX_COLOR_COUNT-1]);
	SrsMainTempPv->Pointer->Size  = 2;
	SrsMainTempPv->Pointer->Frame->Visible = false;
	SrsMainTempPv->Visible = true;
}
//---------------------------------------------------------------------------
int __fastcall TFrmTempGraph::FindClosestXIndex(TPointSeries *serie, double xValue)
{
	int high, low, i;

	if (serie->XValues->Count <= 0)
	{
		return -1;
	}

	int firstIdx, lastIdx;
	firstIdx = serie->FirstValueIndex;
	lastIdx = serie->LastValueIndex;

	if (firstIdx < 0 || lastIdx < 0)
	{
		return -1;
	}

	double firstValue, lastValue;
	firstValue = serie->XValues->Value[firstIdx];
	lastValue = serie->XValues->Value[lastIdx];

	if (xValue <= firstValue)
	{
		return firstIdx;
	}
	if (xValue >= lastValue)
	{
		return lastIdx;
	}

	// 이진탐색
	low = firstIdx;
	high = lastIdx;
	while (high - low > 1)
	{
		i = (high + low) / 2;
		if (xValue <= serie->XValues->Value[i])
		{
			high = i;
		}
		else
		{
			low = i;
        }
	}

	int temp = ((serie->XValues->Value[high] - xValue) >= (xValue - serie->XValues->Value[low])) ? 1 : 0;
	int index = high - temp;
	if (index <= firstIdx)
	{
		index = firstIdx;
	}
	else if (index >= lastIdx)
	{
		index = lastIdx;
	}
	return index;
}
//---------------------------------------------------------------------------
bool  __fastcall TFrmTempGraph::GetValue(TPointSeries* series, double &value)
{
	double xValue = TempChart->Axes->Bottom->CalcPosPoint(CurrentMouseX);
	int idx = FindClosestXIndex(series, xValue);
	if (idx < 0)
	{
		return false;
	}
	value = series->YValues->Items[idx];
	return true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::LoadMainTempData(String filename, TDateTime start)
{
	AnsiString oldfile, newfile;

	// 파일이름이 UP550_ 에서 MainTemp_ 로 바뀌었기 때문에
	// 새 이름으로 바꾼 후 로딩한다.
	// 새 이름으로 파일이 존재하면 그냥 로딩한다.
	newfile = GetReportFolder() + "MainTemp_" + filename + ".cht";
	oldfile = GetReportFolder() + "UP550_" + filename + ".cht";
	if( FileExists(oldfile) )
		RenameFile(oldfile, newfile);

	if( !FileExists(newfile) )
	{
		SetMessageStr(GetResStr("Warning", "NoFileSelectedDate"));
		SetMessageStr(newfile);
		ShowMessageDlg();
		return;
	}

	std::auto_ptr<TStringList> lineList (new TStringList());
	std::auto_ptr<TStringList> dataList (new TStringList());

	lineList->LoadFromFile(newfile);

	String str;
	TDateTime prev, date, time, xtime;

	date = StrToDate(start.DateString(), GetDateTimeFormat());
	for(int i=0; i<lineList->Count; i++)
	{
		dataList->CommaText = lineList->Strings[i];

		if( dataList->Count != 4)
			continue;
		else
		{
			str = dataList->Strings[0];
			time = StrToTime(str);

			// 날짜가 바뀌는지 체크
			if( i != 0 )
			{
				unsigned short hour, min, sec, mil;
				time.DecodeTime(&hour, &min, &sec, &mil);
				if( time.Val - prev.Val < 0.0 &&
					hour == 0 && min == 0 )
					date = IncDay(date);
			}

			xtime = date + time;
			if( DisplayAbsTime == false )
				xtime -= start;

			if( SrsMainTempPv )
				SrsMainTempPv->AddXY(xtime, dataList->Strings[1].ToDouble());
			if( SrsMainTempSp )
				SrsMainTempSp->AddXY(xtime, dataList->Strings[2].ToDouble());
			if( SrsMainTempOut )
				SrsMainTempOut->AddXY(xtime, dataList->Strings[3].ToDouble());

			prev = time;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::LoadLoggerData(String filename, TDateTime start)
{
	AnsiString file = GetReportFolder();
	file += "LOGGER_" + filename + ".cht";

	if( !FileExists(file) )
	{
		SetMessageStr(GetResStr("Warning", "NoFileSelectedDate"));
		SetMessageStr(file);
		ShowMessageDlg();
		return;
	}

	std::auto_ptr<TStringList> lineList (new TStringList());
	std::auto_ptr<TStringList> dataList (new TStringList());

	lineList->LoadFromFile(file);

	String str;
	TDateTime prev, date, time, xtime;

	date = StrToDate(start.DateString(), GetDateTimeFormat());
	for(int i=0; i<lineList->Count; i++)
	{
		dataList->CommaText = lineList->Strings[i];

		if( dataList->Count != MAX_LOGGERPOINT_COUNT+1)
			continue;
		else
		{
			str = dataList->Strings[0];
			time = StrToTime(str);

			// 날짜가 바뀌는지 체크
			if( i != 0 )
			{
				unsigned short hour, min, sec, mil;
				time.DecodeTime(&hour, &min, &sec, &mil);
				if( time.Val - prev.Val < 0.0 &&
					hour == 0 && min == 0 )
					date = IncDay(date);
			}

			xtime = date + time;
			if( DisplayAbsTime == false )
				xtime -= start;

			double data = 0.0;
			int ch = 0;
			for (int j=1; j<=MAX_LOGGERPOINT_COUNT; j++)
			{
				if (Chamber->UseChannel[j-1] == true)
				{
					if( dataList->Strings[j] == "---" )
						data = 0.0;
					else
						data = dataList->Strings[j].ToDouble();
					if( Srs_Ch[ch] )
						Srs_Ch[ch]->AddXY(xtime, data);
					ch++;
				}
			}

			prev = time;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::LoadOxygenData(String filename, TDateTime start)
{
	AnsiString file = GetReportFolder();
	file += "O2_" + filename + ".cht";

	if( !FileExists(file) )
	{
		SetMessageStr(GetResStr("Warning", "NoFileSelectedDate"));
		SetMessageStr(file);
		ShowMessageDlg();
		return;
	}

	std::auto_ptr<TStringList> lineList (new TStringList());
	std::auto_ptr<TStringList> dataList (new TStringList());

	lineList->LoadFromFile(file);

	String str;
	int cnt = 1;
	TDateTime prev, date, time, xtime;

	date = StrToDate(start.DateString(), GetDateTimeFormat());
	for(int i=0; i<lineList->Count; i++)
	{
		dataList->CommaText = lineList->Strings[i];

		if( dataList->Count != cnt+1 )
			continue;
		else
		{
			str = dataList->Strings[0];
			time = StrToTime(str);

			// 날짜가 바뀌는지 체크
			if( i != 0 )
			{
				unsigned short hour, min, sec, mil;
				time.DecodeTime(&hour, &min, &sec, &mil);
				if( time.Val - prev.Val < 0.0 &&
					hour == 0 && min == 0 )
					date = IncDay(date);
			}

			xtime = date + time;
			if( DisplayAbsTime == false )
				xtime -= start;

			if( SrsO2 )
				SrsO2->AddXY(xtime, dataList->Strings[1].ToDouble());

			prev = time;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::LoadLimitData(String filename, TDateTime start)
{
	AnsiString file = GetReportFolder();
	file += "LIMIT_" + filename + ".cht";

	if( !FileExists(file) )
	{
	//	SetMessageStr(GetResStr("Warning", "NoFileSelectedDate"));
	//	ShowMessageDlg();
		return;
	}

	std::auto_ptr<TStringList> lineList (new TStringList());
	std::auto_ptr<TStringList> dataList (new TStringList());

	lineList->LoadFromFile(file);

	String str;
	TDateTime prev, date, time, xtime;

	date = StrToDate(start.DateString(), GetDateTimeFormat());
	for(int i=0; i<lineList->Count; i++)
	{
		dataList->CommaText = lineList->Strings[i];

		if( dataList->Count != 3)
			continue;
		else
		{
			str = dataList->Strings[0];
			time = StrToTime(str);

			// 날짜가 바뀌는지 체크
			if( i != 0 )
			{
				unsigned short hour, min, sec, mil;
				time.DecodeTime(&hour, &min, &sec, &mil);
				if( time.Val - prev.Val < 0.0 &&
					hour == 0 && min == 0 )
					date = IncDay(date);
			}

			xtime = date + time;
			if( DisplayAbsTime == false )
				xtime -= start;

			if( SrsLimit )
				SrsLimit->AddXY(xtime, dataList->Strings[1].ToDouble());

			prev = time;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::ArrangeLabels()
{
	TAdvEdit *tmp[10];
	int idx = 0;

	for(int i=0; i<10; i++)
		tmp[i] = nullptr;

	if( SrsMainTempPv->Visible )
	{
		lblPv->Visible = true;
		tmp[idx++] = lblPv;
	}
	else
		lblPv->Visible = false;

	if( SrsMainTempSp->Visible )
	{
		lblSp->Visible = true;
		tmp[idx++] = lblSp;
	}
	else
		lblSp->Visible = false;

	if( SrsMainTempOut->Visible )
	{
		lblOut->Visible = true;
		tmp[idx++] = lblOut;
	}
	else
		lblOut->Visible = false;

	if( SrsLimit->Visible )
	{
		lblLimit->Visible = true;
		tmp[idx++] = lblLimit;
	}
	else
		lblLimit->Visible = false;

	if( SrsO2->Visible )
	{
		lblO2->Visible = true;
		tmp[idx++] = lblLimit;
	}
	else
		lblO2->Visible = false;

	for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++)
	{
		if( Chamber->UseChannel[i] )
		{
			if( Srs_Ch[i] &&
				Srs_Ch[i]->Visible )
			{
				Ch[i]->Visible = true;
				tmp[idx++] = Ch[i];
			}
			else
				Ch[i]->Visible = false;
		}
	}

	for(int i=0; i<10; i++)
	{
		if( tmp[i] )
		{
			tmp[i]->Left = 113 + i*49;
			tmp[i]->Top = 25;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::FindData()
{
/*	if( edtTime->Text == "" ||
		edtTime->Text.IsEmpty() )
		return;

	if( WorkGrid->DataSource->DataSet == nullptr )
		return;

	String filename = WorkGrid->DataSource->DataSet->FieldByName("Filename")->AsString;
	TDateTime start = WorkGrid->DataSource->DataSet->FieldByName("StartTime")->AsDateTime;

	AnsiString file = GetReportFolder();
	file += "LOGGER_" + filename + ".cht";
	if( !FileExists(file) )
		return;

	int time = edtTime->Text.ToInt();
	int hour = time / 60;
	int min = time - hour*60;
	TDateTime dtSel = TDateTime(hour, min, 0, 0);
	lblTime->Text = dtSel.FormatString("hh:nn:ss");
	String str;
	int cnt = 6;//GetManager()->LoggerPointCount;

	for(int j=0; j<cnt; j++)
		Ch[j]->Text = "";
	lblPv->Text = "";
	lblSp->Text = "";
	lblOut->Text = "";
	lblLimit->Text = "";
	lblO2->Text = "";

	std::auto_ptr<TStringList> lineList (new TStringList());
	std::auto_ptr<TStringList> dataList (new TStringList());

	lineList->LoadFromFile(file);

	for(int i=0; i<lineList->Count; i++)
	{
		dataList->CommaText = lineList->Strings[i];

		if(dataList->Count != cnt+1)
			continue;
		else
		{
			str = dataList->Strings[0];
			TDateTime draw = StrToTime(str);
			TDateTime dtCurr = StrToDate(start.DateString(), GetDateTimeFormat()) + draw - start;

			if( dtCurr >= dtSel )
			{
				for(int j=0; j<cnt; j++)
					Ch[j]->Text = dataList->Strings[j+1];
				break;
			}
		}
	}

	lineList->Clear();
	dataList->Clear();

	file = GetReportFolder();
	file += "MainTemp_" + filename + ".cht";

	if( !FileExists(file) )
		return;

	lineList->LoadFromFile(file);

	for(int i=0; i<lineList->Count; i++)
	{
		dataList->CommaText = lineList->Strings[i];

		if(dataList->Count != 4)
			continue;
		else
		{
			str = dataList->Strings[0];
			TDateTime draw = StrToTime(str);
			TDateTime dtCurr = StrToDate(start.DateString(), GetDateTimeFormat()) + draw - start;

			if( dtCurr >= dtSel )
			{
				lblPv->Text = dataList->Strings[1];
				lblSp->Text = dataList->Strings[2];
				lblOut->Text = dataList->Strings[3];
				break;
			}
		}
	}

	lineList->Clear();
	dataList->Clear();

	file = GetReportFolder();
	file += "LIMIT_" + filename + ".cht";

	if( !FileExists(file) )
		return;

	lineList->LoadFromFile(file);

	for(int i=0; i<lineList->Count; i++)
	{
		dataList->CommaText = lineList->Strings[i];

		if(dataList->Count != 4)
			continue;
		else
		{
			str = dataList->Strings[0];
			TDateTime draw = StrToTime(str);
			TDateTime dtCurr = StrToDate(start.DateString(), GetDateTimeFormat()) + draw - start;

			if( dtCurr >= dtSel )
			{
				lblLimit->Text = dataList->Strings[1];
				break;
			}
		}
	} */
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::WorkGridCellClick(TColumn *Column)
{
	String filename = WorkGrid->DataSource->DataSet->FieldByName("Filename")->AsString;
	TDateTime start = WorkGrid->DataSource->DataSet->FieldByName("StartTime")->AsDateTime;
	int cureTime = WorkGrid->DataSource->DataSet->FieldByName("cureTime")->AsInteger;
	int cureTemp = WorkGrid->DataSource->DataSet->FieldByName("cureTemp")->AsInteger;
	int totalLotCount = WorkGrid->DataSource->DataSet->FieldByName("TotalLotCount")->AsInteger;

	InitLotInfoGrid(Chamber->ChamberID);

	if( filename.IsEmpty() )
	{
		SelectedFile = "";
		InitChart(Now(), 180, 500);
		return;
	}

	// 20180607 - Separation ProductCode & LotID - jhlim
	String str;
//	TField *field = WorkGrid->DataSource->DataSet->FindField("ProductCode");
//	if( field )
//		str = WorkGrid->DataSource->DataSet->FieldByName("ProductCode")->AsString;
//	if( str.IsEmpty() )
//	{
//		for(int i=0; i<totalLotCount; i++)
//		{
//			str.sprintf(L"LotID_%d", i);
//			LotInfoGrid->Cells[1][i+1] = WorkGrid->DataSource->DataSet->FieldByName(str)->AsString;
//			str.sprintf(L"LotCount_%d", i);
//			LotInfoGrid->Cells[2][i+1] = WorkGrid->DataSource->DataSet->FieldByName(str)->AsInteger;
//		}
//	}
//	else
//		LotInfoGrid->Cells[1][1] = str;

	// Product Code
	TField *field = WorkGrid->DataSource->DataSet->FindField("ProductCode");
	if( field )
		str = WorkGrid->DataSource->DataSet->FieldByName("ProductCode")->AsString;
	if( str.IsEmpty() == false)
		LotInfoGrid->Cells[1][1] = str;
	// Lot ID
		for(int i=0; i<totalLotCount; i++)
		{
			str.sprintf(L"LotID_%d", i);
			LotInfoGrid->Cells[1][i+1] = WorkGrid->DataSource->DataSet->FieldByName(str)->AsString;
	}

	InitChart(start, cureTime+30, cureTemp+70);
	LoadMainTempData(filename, start);
	LoadLimitData(filename, start);
	if( GetManager()->UseO2Analyzer )
		LoadOxygenData(filename, start);
	LoadLoggerData(filename, start);

	SelectedFile = filename;
}
//---------------------------------------------------------------------------

void __fastcall TFrmTempGraph::CalendarSelectDate(TObject *Sender, TAdvSmoothCalendarDateMode Mode,
          TDateTime Date)
{
	if( Chamber == nullptr )
		return;

	if( GetOvenDB()->QueryWork(Chamber->ChamberID, Date, Date+1) )
	{
		WorkSrc->DataSet = GetOvenDB()->Query.get();

		if( WorkSrc->DataSet )
		{
			WorkGrid->DataSource = WorkSrc;
		}
	}

	WorkGridCellClick(nullptr);
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::TempChartAllowScroll(TChartAxis *Sender, double &AMin,
          double &AMax, bool &AllowScroll)
{
	AllowScroll = (Sender == TempChart->BottomAxis);
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::TempChartMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
	if( SelectedFile.IsEmpty() )
		return;

	if (CurrentMouseX != X)
	{
		CurrentMouseX = X;
	}

	// Locate nearest point to mouse...
	double xValue = TempChart->Axes->Bottom->CalcPosPoint(CurrentMouseX);
	TDateTime dtSel(xValue);
	lblTime->Text = dtSel.FormatString("hh:nn:ss");

	AnsiString str;
	double value;

	if( SrsMainTempSp &&
		GetValue(SrsMainTempSp, value) )
	{
		str.sprintf("%.1f", value);
		lblSp->Text = str;
	}
	else
		lblSp->Text = "";

	if( SrsMainTempPv &&
		GetValue(SrsMainTempPv, value) )
	{
		str.sprintf("%.1f", value);
		lblPv->Text = str;
	}
	else
		lblPv->Text = "";

	if( SrsMainTempOut &&
		GetValue(SrsMainTempOut, value) )
	{
		str.sprintf("%.1f", value);
		lblOut->Text = str;
	}
	else
		lblOut->Text = "";

	if( SrsLimit &&
		GetValue(SrsLimit, value) )
	{
		str.sprintf("%.1f", value);
		lblLimit->Text = str;
	}
	else
		lblLimit->Text = "";

	if( SrsO2 &&
		GetValue(SrsO2, value) )
	{
		str.sprintf("%.2f", value);
		lblO2->Text = str;
	}
	else
		lblO2->Text = "";

	int ch = 0;

	for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++)
	{
		if( Chamber->UseChannel[i] )
		{
			if( Srs_Ch[ch] &&
				GetValue(Srs_Ch[ch], value) )
			{
				str.sprintf("%.1f", value);
				Ch[ch]->Text = str;
			}
			else
				Ch[ch]->Text = "";
			ch++;
		}
	}

	TempChart->AutoRepaint = true;
	TempChart->Invalidate();
	TempChart->AutoRepaint = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::edtTimeChange(TObject *Sender)
{
	FindData();
}
//---------------------------------------------------------------------------
//#include <stdimage.hpp>
void __fastcall TFrmTempGraph::ProfileBackupButtonClick(TObject *Sender)
{
	if( FolderDialog->Execute() == false )
		return;

	String dstFolder = FolderDialog->Directory;
	dstFolder += "\\";

	String org, dst, filename;

	filename = "MainTemp_" + SelectedFile + ".cht";
	org = GetReportFolder() + filename;
	dst = dstFolder + filename;
	if( CopyFile(org.c_str(), dst.c_str(), false) == false )
	{
		SetMessageStr(GetResStr("Report", "SaveFileErr"));
		ShowMessageDlg();
		return;
	}

	filename = "LOGGER_" + SelectedFile + ".cht";
	org = GetReportFolder() + filename;
	dst = dstFolder + filename;
	if( CopyFile(org.c_str(), dst.c_str(), false) )
	{
		SetMessageStr(GetResStr("Report", "SaveFileSuccess"));
		ShowMessageDlg();
	}
	else
	{
		SetMessageStr(GetResStr("Report", "SaveFileErr"));
		ShowMessageDlg();
		return;
	}

	filename = "LIMIT_" + SelectedFile + ".cht";
	org = GetReportFolder() + filename;
	dst = dstFolder + filename;
	if( CopyFile(org.c_str(), dst.c_str(), false) )
	{
		SetMessageStr(GetResStr("Report", "SaveFileSuccess"));
		ShowMessageDlg();
	}
	else
	{
		SetMessageStr(GetResStr("Report", "SaveFileErr"));
		ShowMessageDlg();
		return;
	}

	filename = "O2_" + SelectedFile + ".cht";
	org = GetReportFolder() + filename;
	dst = dstFolder + filename;
	if( CopyFile(org.c_str(), dst.c_str(), false) )
	{
		SetMessageStr(GetResStr("Report", "SaveFileSuccess"));
		ShowMessageDlg();
	}
	else
	{
		SetMessageStr(GetResStr("Report", "SaveFileErr"));
		ShowMessageDlg();
		return;
	}

	Graphics::TBitmap *bmp = CaptureControl(TempChart);
	dst = dstFolder + "Graph_" + SelectedFile + ".png";

	TPngImage* png = new TPngImage();
	png->Assign(bmp);
	png->SaveToFile(dst);
	delete png;
	delete bmp;
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::SearchProductCodeButtonClick(TObject *Sender)
{
	if( Chamber == nullptr )
		return;

	if( ProductCodeEdit->Text.IsEmpty() )
		return;

	if( GetOvenDB()->QueryProductCode(Chamber->ChamberID, ProductCodeEdit->Text) )
	{
		WorkSrc->DataSet = GetOvenDB()->Query.get();

		if( WorkSrc->DataSet )
		{
			WorkGrid->DataSource = WorkSrc;
		}
	}

	WorkGridCellClick(nullptr);
}
//---------------------------------------------------------------------------

void __fastcall TFrmTempGraph::TempChartDblClick(TObject *Sender)
{
	DisplayAbsTime = !DisplayAbsTime;

	String filename = WorkGrid->DataSource->DataSet->FieldByName("Filename")->AsString;
	TDateTime start = WorkGrid->DataSource->DataSet->FieldByName("StartTime")->AsDateTime;
	int cureTime = WorkGrid->DataSource->DataSet->FieldByName("cureTime")->AsInteger;
	int cureTemp = WorkGrid->DataSource->DataSet->FieldByName("cureTemp")->AsInteger;

	if( filename.IsEmpty() )
		InitChart(Now(), 180, 500);
	else
	{
		InitChart(start, cureTime+30, cureTemp+70);
		LoadMainTempData(filename, start);
		LoadLimitData(filename, start);
		if( GetManager()->UseO2Analyzer )
			LoadOxygenData(filename, start);
		LoadLoggerData(filename, start);
	}
    CurrentMouseX = -1;
}
//---------------------------------------------------------------------------

void __fastcall TFrmTempGraph::LotInfoGridGetAlignment(TObject *Sender, int ARow,
          int ACol, TAlignment &HAlign, TVAlignment &VAlign)
{
	HAlign = taCenter;
	VAlign = Advobj::vtaCenter;

}
//---------------------------------------------------------------------------

void __fastcall TFrmTempGraph::LotInfoGridGetCellColor(TObject *Sender, int ARow,
          int ACol, TGridDrawState AState, TBrush *ABrush, TFont *AFont)
{
	ABrush->Color = clInactiveCaption;
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::InitLotInfoGrid(int chamberNo)
{
	LotInfoGrid->Clear();

	LotInfoGrid->ColWidths[0] = 30;
	LotInfoGrid->ColWidths[1] = 132;
	LotInfoGrid->ColWidths[2] = 50;

	LotInfoGrid->Cells[0][0] = "No.";
	LotInfoGrid->Cells[1][0] = "Lot ID";
	LotInfoGrid->Cells[2][0] = "Q'ty";
	for(int i=0; i<LOT_COUNT; i++)
		LotInfoGrid->Cells[0][i+1] = IntToStr(i+1);
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::FillLotInfoGrid(int chamberNo, LOTINFO lotInfo)
{
	for(int i=0; i<lotInfo.TotalLotCount; i++)
	{
		LotInfoGrid->Cells[1][i+1] = lotInfo.LotID[i];
        LotInfoGrid->Cells[2][i+1] = IntToStr(lotInfo.LotQuantity[i]);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::CalendarDateFill(TObject *Sender, TGDIPFill *AFill,
		  TFont *AFont, TDateTime Date, TAdvSmoothCalendarDateKind DateKind)
{
	unsigned short year, month, day;
	Date.DecodeDate(&year, &month, &day);
	if( DataFileCount[day-1] > 0 )
		AFill->Color = clRed;
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::CalendarDateStatus(TObject *Sender, TDateTime Date,
		  UnicodeString &StatusMessage, TGDIPStatus *Fill, int &OffsetX, int &OffsetY)
{
	unsigned short year, month, day;
	Date.DecodeDate(&year, &month, &day);

	if( DataFileCount[day-1] > 0 )
	{
		StatusMessage.sprintf(L"%d", DataFileCount[day-1]);
		Fill->Fill->Color = clYellow;
		OffsetX = 0;
		OffsetY = 0;
	}
}
//---------------------------------------------------------------------------
int MonthDay[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
void __fastcall TFrmTempGraph::CalcDataFileCount(int year, int month)
{
	if( Chamber == nullptr )
		return;

	TDateTime date;
	int day;

	if( month == 2 )
	{
		if( year % 4 == 0 )
			day = 29;
		else
			day = 28;
	}
	else
		day = MonthDay[month-1];

	for(int i=0; i<day; i++)
		DataFileCount[i] = 0;

	if( Screen->Cursor == crDefault )
		Screen->Cursor = crHourGlass;
	for(int i=0; i<day; i++)
	{
		try
		{
			date = EncodeDate(year, month, i+1);
		}
		catch(...)
		{
			return;
		}
		DataFileCount[i] = GetOvenDB()->GetDataFileCount(Chamber->ChamberID, date);
		Application->ProcessMessages();
	}
	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::CalendarMonthChanged(TObject *Sender, int Month)
{
	for(int i=0; i<31; i++)
		DataFileCount[i] = 0;
	Calendar->Invalidate();
	GetDayInfoTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::CalendarYearChanged(TObject *Sender, int Year)
{
	for(int i=0; i<31; i++)
		DataFileCount[i] = 0;
	Calendar->Invalidate();
	GetDayInfoTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrmTempGraph::GetDayInfoTimerTimer(TObject *Sender)
{
	GetDayInfoTimer->Enabled = false;
	CalcDataFileCount(Calendar->CurrentYear(), Calendar->CurrentMonth());
	Calendar->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TFrmTempGraph::TempChartZoom(TObject *Sender)
{
	TempChart->LeftAxis->Increment = 1;
}
//---------------------------------------------------------------------------

void __fastcall TFrmTempGraph::TempChartUndoZoom(TObject *Sender)
{
	TempChart->LeftAxis->Increment = 10;
}
//---------------------------------------------------------------------------

void __fastcall TFrmTempGraph::TempChartAfterDraw(TObject *Sender)
{
    int x, y;
	int xStart, xEnd;
	double xMin, xMax;
	double yMax;

	xMin = TempChart->BottomAxis->Minimum;
	xMax = TempChart->BottomAxis->Maximum;
	yMax = TempChart->LeftAxis->Maximum;

	xStart = SrsMainTempPv->CalcXPosValue(xMin);
	xEnd = SrsMainTempPv->CalcXPosValue(xMax);
	if (CurrentMouseX >= xStart && CurrentMouseX <= xEnd)
	{
		x = CurrentMouseX;
		y = SrsMainTempPv->CalcYPosValue(yMax);

		double xValue = TempChart->Axes->Bottom->CalcPosPoint(x);
		TDateTime time(xValue);
		String str = time.FormatString("hh:mm:ss");

		TempChart->Canvas->Font->Color = clBlack;
		TempChart->Canvas->Font->Name = "Tahoma";
		TempChart->Canvas->Font->Size = 8;
		TempChart->Canvas->Font->Color = clRed;
		TempChart->Canvas->BackColor = clWhite;
		TempChart->Canvas->TextOutW(x + 5, y, str);
	}
}
//---------------------------------------------------------------------------


void __fastcall TFrmTempGraph::TempChartBeforeDrawSeries(TObject *Sender)
{
	int x0, x1, y0, y1;
	int xStart, xEnd;
	double xMin, xMax;
	double yMin, yMax;

	xMin = TempChart->BottomAxis->Minimum;
	xMax = TempChart->BottomAxis->Maximum;
	yMin = TempChart->LeftAxis->Minimum;
	yMax = TempChart->LeftAxis->Maximum;

	xStart = SrsMainTempPv->CalcXPosValue(xMin);
	xEnd = SrsMainTempPv->CalcXPosValue(xMax);
	if (CurrentMouseX >= xStart && CurrentMouseX <= xEnd)
	{
		// 수직선 그리기
		x0 = x1 = CurrentMouseX;
		y0 = SrsMainTempPv->CalcYPosValue(yMax);
		y1 = SrsMainTempPv->CalcYPosValue(yMin);
		TempChart->Canvas->Pen->Style = psDot;
		TempChart->Canvas->Pen->Color = clRed;
		TempChart->Canvas->Line(x0, y0, x1, y1);
	}
}
//---------------------------------------------------------------------------

