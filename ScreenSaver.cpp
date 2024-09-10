//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ScreenSaver.h"
#include "JobForm.h"
#include "NormalOvenChamber.h"
#include "NormalOvenSystemManager.h"

#include "HelperFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvPanel"
#pragma resource "*.dfm"
TScreenSaver *ScreenSaver;
//---------------------------------------------------------------------------
__fastcall TScreenSaver::TScreenSaver(TComponent* Owner)
	: TForm(Owner)
{
	Panel[0] = nullptr;
	Panel[1] = nullptr;
	Panel[2] = nullptr;
	Panel[3] = nullptr;
}
//---------------------------------------------------------------------------
void __fastcall TScreenSaver::FormCreate(TObject *Sender)
{
	Panel[0] = Panel1;
	Panel[1] = Panel2;
	Panel[2] = Panel3;
	Panel[3] = Panel4;
}
//---------------------------------------------------------------------------
void __fastcall TScreenSaver::FormDestroy(TObject *Sender)
{
	Panel[0] = nullptr;
	Panel[1] = nullptr;
	Panel[2] = nullptr;
	Panel[3] = nullptr;
}
//---------------------------------------------------------------------------
void __fastcall TScreenSaver::FormMouseActivate(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y, int HitTest, TMouseActivate &MouseActivate)

{
	Hide();
	FmJob->ScreenSaverTimer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TScreenSaver::FormMouseMove(TObject *Sender, TShiftState Shift, int X,
		  int Y)
{
	Hide();
	FmJob->ScreenSaverTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TScreenSaver::FormKeyPress(TObject *Sender, System::WideChar &Key)

{
	Hide();
	FmJob->ScreenSaverTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TScreenSaver::FormShow(TObject *Sender)
{
	Localize();

	RefreshDataTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TScreenSaver::FormHide(TObject *Sender)
{
	RefreshDataTimer->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TScreenSaver::RefreshDataTimerTimer(TObject *Sender)
{
	RefreshDataTimer->Enabled = false;

	RefreshData();

	RefreshDataTimer->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TScreenSaver::Localize()
{
	// Chamber Count
	int nchamberCount = GetManager()->ChamberCount;
	// Size Calc & Adapt
	int nWidth = 0;
	int nHeight = 0;
	switch(nchamberCount)
	{
		case 1:
			nWidth = Width;
			nHeight = Height;
			if( Panel[0] == nullptr )
				break;
			Panel[0]->Width = nWidth;
			Panel[0]->Height = nHeight;
			Panel[0]->Left = 0;
			Panel[0]->Top = 0;
			Panel[0]->Caption->Height = Height * 0.1;
			Panel[0]->StatusBar->Height = 0;//Height * 0.45;
			Panel[0]->StatusBar->TopIndent = -nHeight * 0.45;
			Panel[0]->Caption->Font->Size = 50;
			Panel[0]->Font->Size = Panel[0]->Caption->Font->Size + 10;
			Panel[0]->StatusBar->Font->Size = Panel[0]->Caption->Font->Size + 10;
		break;
		case 2:
			nWidth = Width;
			nHeight = Height / 2;
			for(int i=0; i<nchamberCount; i++)
			{
				if( Panel[i] == nullptr )
					break;
				Panel[i]->Width = nWidth;
				Panel[i]->Height = nHeight;
				Panel[i]->Left = 0;
				Panel[i]->Top = i * nHeight;
				Panel[i]->Caption->Height = nHeight * 0.1;
				Panel[i]->StatusBar->Height = 0;//nHeight * 0.45;
				Panel[i]->StatusBar->TopIndent = -nHeight * 0.45;
				Panel[i]->Caption->Font->Size = 20;
				Panel[i]->Font->Size = Panel[i]->Caption->Font->Size + 15;
				Panel[i]->StatusBar->Font->Size = Panel[i]->Caption->Font->Size + 15;
			}
		break;
		case 4:
			nWidth = Width / 2;
			nHeight = Height / 2;
			for(int i=0; i<nchamberCount; i++)
			{
				if( Panel[i] == nullptr )
					break;
				Panel[i]->Left = (i%2) * nWidth;
				Panel[i]->Top = (i/2) * nHeight;
				Panel[i]->Caption->Height = nHeight * 0.1;
				Panel[i]->StatusBar->Height = 0;//nHeight * 0.45;
				Panel[i]->StatusBar->TopIndent = -nHeight * 0.45;
				Panel[i]->Caption->Font->Size = 20;
				Panel[i]->Font->Size = Panel[i]->Caption->Font->Size + 15;
				Panel[i]->StatusBar->Font->Size = Panel[i]->Caption->Font->Size + 15;
				Panel[i]->Width = nWidth;
				Panel[i]->Height = nHeight;
			}
		break;
	}
	// Panel
	for(int i=0; i<4; i++)
	{
		if(i<nchamberCount)
		{
			Panel[i]->Font->Color = clDkGray;
			Panel[i]->Caption->Color = clBlue;
			Panel[i]->Caption->Font->Color = clWhite;
			Panel[i]->StatusBar->Font->Color = clPurple;
			Panel[i]->Caption->Visible = true;
			Panel[i]->StatusBar->Visible = true;
			Panel[i]->Visible = true;
		}
		else
		{
			Panel[i]->Visible = false;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TScreenSaver::RefreshData()
{
	NormalOvenChamber* chamber;
	TDateTime CurrentTime, EndTime;
	int nchamberCount;
	int nTime, nHour, nMin;
	String str, str1;
	String strText, strStatus;

	CurrentTime = TDateTime::CurrentDateTime();
	nchamberCount = GetManager()->ChamberCount;

	for(int i=0; i<nchamberCount; i++)
	{
		if(Panel[i])
		{
			// Caption
			switch(i)
			{
				case 0:
					str1 = L"CHAMBER A";
				break;
				case 1:
					str1 = L"CHAMBER B";
				break;
				case 2:
					str1 = L"CHAMBER C";
				break;
				case 3:
					str1 = L"CHAMBER D";
				break;
			}
			str.sprintf(L"<P align=\"center\"><B>%s</B></P>", str1.c_str());
			if(Panel[i]->Caption->Text != str)
				Panel[i]->Caption->Text = str;

			chamber = nullptr;
			chamber = GetManager()->AChamber[i];
			if(chamber)
			{
				if(chamber->Step != STEP_IDLE)
				{
					// Text
					str1.sprintf(L"<P align=\"left\">[IN]	%s</P><br>", CurrentTime.FormatString("yyyy/mm/dd").c_str());
					str.sprintf(L"<P align=\"center\"><B>%s</B></P>", chamber->Recipe.StartTime.FormatString("hh:nn").c_str());
					strText.sprintf(L"%s%s", str1.c_str(), str.c_str());

					nTime = chamber->Recipe.TempPtn.TotalTime;
					nHour = nTime / 60;
					nMin = nTime - (nHour * 60);
					EndTime = chamber->Recipe.StartTime + TDateTime(nHour, nMin, 0, 0);

					// StatusBar
					str1.sprintf(L"<P align=\"left\">[OUT]	%s</P><br>", CurrentTime.FormatString("yyyy/mm/dd").c_str());
					str.sprintf(L"<P align=\"center\"><B>%s</B></P>", EndTime.FormatString("hh:nn").c_str());
					strStatus.sprintf(L"%s%s", str1.c_str(), str.c_str());
				}
				else
				{
					// Text
					str1.sprintf(L"<P align=\"left\">[IN]	%s</P><br>", CurrentTime.FormatString("yyyy/mm/dd").c_str());
					str.sprintf(L"<P align=\"center\"><B>00:00</B></P>");
					strText.sprintf(L"%s%s", str1.c_str(), str.c_str());

					// StatusBar
					str1.sprintf(L"<P align=\"left\">[OUT]	%s</P><br>", CurrentTime.FormatString("yyyy/mm/dd").c_str());
					str.sprintf(L"<P align=\"center\"><B>00:00</B></P>");
					strStatus.sprintf(L"%s%s", str1.c_str(), str.c_str());
				}
			}
			else
			{
				// Text
				str1.sprintf(L"<P align=\"left\">[IN]	%s</P><br>", CurrentTime.FormatString("yyyy/mm/dd").c_str());
				str.sprintf(L"<P align=\"center\"><B>00:00</B></P>");
				strText.sprintf(L"%s%s", str1.c_str(), str.c_str());

				// StatusBar
				str1.sprintf(L"<P align=\"left\">[OUT]	%s</P><br>", CurrentTime.FormatString("yyyy/mm/dd").c_str());
				str.sprintf(L"<P align=\"center\"><B>00:00</B></P>");
				strStatus.sprintf(L"%s%s", str1.c_str(), str.c_str());
			}
		}
		// Text
		if(Panel[i]->Text != strText)
			Panel[i]->Text = strText;

		// StatusBar
		if(Panel[i]->StatusBar->Text != strStatus)
			Panel[i]->StatusBar->Text = strStatus;
	}
}
//---------------------------------------------------------------------------



