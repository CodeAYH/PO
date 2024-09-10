//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "IOFrame.h"
#include "HelperFunctions.h"
#include "NormalOvenChamber.h"
#include "NormalOvenSystemManager.h"
#include "MainTempController.h"
#include "ST340.h"
#include "I7018.h"
#include "ZR5.h"
#include "I7000DIOModule.h"
#include "MoonsStepMotor.h"
#include "DiffPressure.h"
#include "Database.h"
#include "VSOvenBoard.h"
#include "LSInverter.h"
#include "REX_P250.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvGlassButton"
#pragma link "AdvPanel"
#pragma link "AdvSmoothLabel"
#pragma resource "*.dfm"
TFrmIO *FrmIO;
//---------------------------------------------------------------------------
__fastcall TFrmIO::TFrmIO(TComponent* Owner)
	: TFrame(Owner)
{
	Init();
}
//---------------------------------------------------------------------------
__fastcall TFrmIO::TFrmIO(TComponent* Owner, int chamberNo)
	: TFrame(Owner), ChamberNo(chamberNo)
{
	Init();

	String str;
	str.sprintf(L"Chamber %c", chamberNo+'A');
	Hint = str;
}
//---------------------------------------------------------------------------
void __fastcall TFrmIO::Init()
{
	CurrentInputIndex = 0;
	CurrentOutputIndex = 0;

	MaxInputIndex = 2;
	MaxOutputIndex = 1;

	String str;
	int idx, ioNum;
	int height = 35;
	for(int i=0; i<16; i++)
	{
		idx = 16*CurrentInputIndex+i;
		BtnInputLabel[i] = new TAdvGlassButton(PnlInput);
		BtnInputLabel[i]->Parent = PnlInput;
		BtnInputLabel[i]->Left = 4;
		BtnInputLabel[i]->Top = 38 + i*height + 2;
		BtnInputLabel[i]->Width = 82;
		BtnInputLabel[i]->Height = height;
		BtnInputLabel[i]->Font->Style = TFontStyles() << fsBold;
		BtnInputLabel[i]->Font->Size = 12;
		BtnInputLabel[i]->BackColor = clWhite;
		BtnInputLabel[i]->ShineColor = clWhite;
		BtnInputLabel[i]->CornerRadius = 0;
		str.sprintf(L"X%03X", idx);
		BtnInputLabel[i]->Caption = str;

		BtnInput[i] = new TAdvGlassButton(PnlInput);
		BtnInput[i]->Parent = PnlInput;
		BtnInput[i]->Left = 85;
		BtnInput[i]->Top = 38 + i*height + 2;
		BtnInput[i]->Width = 416;
		BtnInput[i]->Height = height;
		BtnInput[i]->Font->Style = TFontStyles() << fsBold;
		BtnInput[i]->Font->Size = 12;
		BtnInput[i]->BackColor = clWhite;
		BtnInput[i]->ShineColor = clWhite;
		BtnInput[i]->CornerRadius = 0;
		BtnInput[i]->Caption = GetIOName(str);
		BtnInput[i]->Tag = idx;

		idx = 16*CurrentOutputIndex+i;
		ioNum = idx + DIO_COUNT/2;
		BtnOutputLabel[i] = new TAdvGlassButton(PnlOutput);
		BtnOutputLabel[i]->Parent = PnlOutput;
		BtnOutputLabel[i]->Left = 4;
		BtnOutputLabel[i]->Top = 38 + i*height + 2;
		BtnOutputLabel[i]->Width = 82;
		BtnOutputLabel[i]->Height = height;
		BtnOutputLabel[i]->Font->Style = TFontStyles() << fsBold;
		BtnOutputLabel[i]->Font->Size = 12;
		BtnOutputLabel[i]->BackColor = clWhite;
		BtnOutputLabel[i]->ShineColor = clWhite;
		BtnOutputLabel[i]->CornerRadius = 0;
		str.sprintf(L"Y%03d", idx);
		BtnOutputLabel[i]->Caption = str;

		BtnOutput[i] = new TAdvGlassButton(PnlOutput);
		BtnOutput[i]->Parent = PnlOutput;
		BtnOutput[i]->Left = 85;
		BtnOutput[i]->Top = 38 + i*height + 2;
		BtnOutput[i]->Width = 416;
		BtnOutput[i]->Height = height;
		BtnOutput[i]->Font->Style = TFontStyles() << fsBold;
		BtnOutput[i]->Font->Size = 12;
		BtnOutput[i]->BackColor = clWhite;
		BtnOutput[i]->ShineColor = clWhite;
		BtnOutput[i]->CornerRadius = 0;
		BtnOutput[i]->Caption = GetIOName(str);
		BtnOutput[i]->OnClick = BtnOutputClick;
		BtnOutput[i]->Tag = ioNum;
	}

	PnlInput->OptimizePaint = true;
	PnlOutput->OptimizePaint = true;

	if(GetManager()->AutomationKind == HANA_CIM && GetManager()->UseAutomation)
	{
		HanaMESPanel->Visible = true;
	}
	else
	{
	   HanaMESPanel->Visible = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmIO::RefreshButton()
{
	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( CurrentInputIndex > 0 )
		btnPrevInput->Visible = true;
	else
		btnPrevInput->Visible = false;
	if( CurrentInputIndex < MaxInputIndex-1 )
		btnNextInput->Visible = true;
	else
		btnNextInput->Visible = false;

	if( CurrentOutputIndex > 0 )
		btnPrevOutput->Visible = true;
	else
		btnPrevOutput->Visible = false;
	if( CurrentOutputIndex < MaxOutputIndex-1 )
		btnNextOutput->Visible = true;
	else
		btnNextOutput->Visible = false;

	String str, nullstr;
	int idx, ioNum;
	for(int i=0; i<16; i++)
	{
		idx = 16*CurrentInputIndex+i;
		ioNum = idx;
		str.sprintf(L"X%03X", idx);
		if( BtnInput[i] )
		{
			if (idx == I_AIR_PRESSURE_DETECTED)
			{
				BtnInput[i]->Caption = GetManager()->UseAirPressureDetector ? GetIOName(str) : nullstr;
			}
            else if (idx == I_N2_PRESSURE_DETECTED)
			{
               	BtnInput[i]->Caption = GetManager()->UseN2PressureDetector ? GetIOName(str) : nullstr;
            }
            else if (idx == I_HEATER_OVERLOAD_DETECTED)
			{
               	BtnInput[i]->Caption = GetManager()->UseHeaterOverloadDetector ? GetIOName(str) : nullstr;
            }
			else if (idx == I_DIFFERENTIAL_PRESSURE_DETECTED)
			{
            	BtnInput[i]->Caption = GetManager()->UseDiffPressureDetector ? GetIOName(str) : nullstr;
			}
			else if (idx == I_SSR_OVERTEMP_DETECTED)
			{
            	BtnInput[i]->Caption = GetManager()->UseSSROverTempDetector ? GetIOName(str) : nullstr;
			}
			else if (idx == I_CHAMBER_DOOR_TEMP_INTERLOCK_DETECTED)
			{
            	BtnInput[i]->Caption = GetManager()->UseChamberDoorTempInterlock ? GetIOName(str) : nullstr;
			}
			else if (idx == I_CHAMBER_WATER_FLOW_DETECTED)
			{
            	BtnInput[i]->Caption = GetManager()->UseWaterFlowDetector ? GetIOName(str) : nullstr;
			}
			else if( idx == I_ELECTRONICS_DOORLOCK_DETECTED )
            {
            	BtnInput[i]->Caption = GetManager()->UseElectricPanelDoorDetector ? GetIOName(str) : nullstr;
            }
			else if (idx == I_GROUND_ALARM_DETECTED)
			{
				if (GetManager()->UseGroundAlarmDetector)
				{
					BtnInput[i]->Caption = GetIOName(str);
				}
				else if (chamber->DoorOpenDetectedWhenIdle && !(GetManager()->UseGroundAlarmDetector))
				{
					BtnInput[i]->Caption = GetResStr("Common","DoorOpenWhenIdle");
				}
				else
				{
                    BtnInput[i]->Caption = nullstr;
                }
			}
            else if( idx == I_MAGAZINE_DETECTED )
            {
            	BtnInput[i]->Caption = GetManager()->UseMagazineDetector ? GetIOName(str) : nullstr;
			}
			else if( idx == GetManager()->I_N2_FLOW_DETECTED )
            {
				BtnInput[i]->Caption = GetManager()->UseN2FlowDetector ? GetIOName(str) : nullstr;
            }
			else if (idx == I_BLOWER_LOW_CURRENT_DETECTED)
			{
				BtnInput[i]->Caption = GetManager()->UseBlowMotorLowCurrentDetector ? GetIOName(str) : nullstr;
			}
			else if (idx == I_DAMPER_MOTOR_ALARM_DETECTED)
			{
            	BtnInput[i]->Caption = (GetManager()->DamperControlType != CYLINDER_DAMPER) ? GetIOName(str) : nullstr;
			}
//			else if (idx == I_N2_PURGE_FLOW_DETECTED) //2023-04-17 restore
//			{
//				BtnInput[i]->Caption = GetManager()->UseN2PurgeDetector ? GetIOName(str) : nullstr;
//			}
//			else if (idx == I_CDA_FLOW_DETECTED)  //2023-04-17 restore
//			{
//				BtnInput[i]->Caption = GetManager()->UseCDAFlowDetector ? GetIOName(str) : nullstr;
//			}
 //			else if (idx == I_DAMPER_OPEN_DETECTED || idx == I_DAMPER_CLOSE_DETECTED)//2023-01-26  Change from flow meter to flow switch
			else if (idx == I_DAMPER_CLOSE_DETECTED)
			{
				BtnInput[i]->Caption = (GetManager()->DamperControlType == CYLINDER_DAMPER) ? GetIOName(str) : nullstr;
			}
			else if (idx == I_DOOR_CLOSE_LOCK_ON || idx == I_DOOR_OPEN_LOCK_OFF)
			{
            	BtnInput[i]->Caption = GetManager()->UseDoorLockCylinder ? GetIOName(str) : nullstr;
			}
            else
            {
                BtnInput[i]->Caption = GetIOName(str);
            }
			BtnInput[i]->Tag = ioNum;
		}

		idx += ChamberNo*256;
		str.sprintf(L"X%03X", idx);
		if( BtnInputLabel[i] )
			BtnInputLabel[i]->Caption = str;

		idx = 16*CurrentOutputIndex+i;
		ioNum = idx + DIO_COUNT/2;
		str.sprintf(L"Y%03X", idx);
		if( BtnOutput[i] )
		{
			if (ioNum == O_N2_LARGE)
			{
				if( GetManager()->UseN2FlowControl)
					BtnOutput[i]->Caption = "N2 LARGE";
				else
					BtnOutput[i]->Caption = "";
			}
			else if (ioNum == O_N2_MEDIUM)
			{
				if( GetManager()->UseN2FlowControl)
					BtnOutput[i]->Caption = "N2 MEDIUM";
				else
					BtnOutput[i]->Caption = "";
			}
			else if (ioNum == O_N2_SMALL)
			{
				if( GetManager()->UseN2FlowControl)
					BtnOutput[i]->Caption = "N2 SMALL";
				else
					BtnOutput[i]->Caption = "";
			}
			/*else if (ioNum == O_N2)
			{
				if( GetManager()->DIOType == OVEN_DIO)
					BtnOutput[i]->Caption = "N2";
				else
					BtnOutput[i]->Caption = "";
			}*/
			else if (ioNum == O_HEATER_PROBLEM_LAMP)
			{
				if( GetManager()->DIOType == OVEN_DIO)
					BtnOutput[i]->Caption = "HEATER PROBLEM LAMP";
				else
					BtnOutput[i]->Caption = "";
			}
			else
			{
				BtnOutput[i]->Caption = GetIOName(str);
			}
			BtnOutput[i]->Tag = ioNum;
		}

		idx += ChamberNo*256;
		str.sprintf(L"Y%03X", idx);
		if( BtnOutputLabel[i] )
			BtnOutputLabel[i]->Caption = str;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrmIO::btnPrevInputClick(TObject *Sender)
{
	if( CurrentInputIndex > 0 )
		CurrentInputIndex--;
	RefreshButton();
}
//---------------------------------------------------------------------------
void __fastcall TFrmIO::btnNextInputClick(TObject *Sender)
{
	if( CurrentInputIndex < MaxInputIndex-1 )
		CurrentInputIndex++;
	RefreshButton();
}
//---------------------------------------------------------------------------
void __fastcall TFrmIO::btnPrevOutputClick(TObject *Sender)
{
	if( CurrentOutputIndex > 0 )
		CurrentOutputIndex--;
	RefreshButton();
}
//---------------------------------------------------------------------------
void __fastcall TFrmIO::btnNextOutputClick(TObject *Sender)
{
	if( CurrentOutputIndex < MaxInputIndex-1 )
		CurrentOutputIndex++;
	RefreshButton();
}
//---------------------------------------------------------------------------
void __fastcall TFrmIO::RefreshTimer(TObject *Sender)
{
	UINT ioNum, tmp;
	UINT diRead, doRead;
	bool on = false;

	Refresh->Enabled = false;

	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( chamber == nullptr )
		return;

	for(int i=0; i<16; i++)
	{
		ioNum = BtnInput[i]->Tag;
		on = chamber->DIO[ioNum]->IsOn();
		if( on )
		{
			if( BtnInput[i]->BackColor != clAqua )
			{
				BtnInput[i]->BackColor = clAqua;
				BtnInput[i]->ShineColor = clAqua;
			}
		}
		else
		{
			if( BtnInput[i]->BackColor != clWhite )
			{
				BtnInput[i]->BackColor = clWhite;
				BtnInput[i]->ShineColor = clWhite;
			}
		}
	}

	for(int i=0; i<16; i++)
	{
		ioNum = BtnOutput[i]->Tag;
		on = chamber->DIO[ioNum]->IsOn();
		if( on )
		{
			if( BtnOutput[i]->BackColor != clFuchsia )
			{
				BtnOutput[i]->BackColor = clFuchsia;
				BtnOutput[i]->ShineColor = clFuchsia;
			}
		}
		else
		{
			if( BtnOutput[i]->BackColor != clWhite )
			{
				BtnOutput[i]->BackColor = clWhite;
				BtnOutput[i]->ShineColor = clWhite;
			}
		}
	}

	String str, title, value;

	title = (L"<P align=\"right\">");
	value = (L"<P align=\"left\">");

	if( chamber->MainTempController )
	{
		str.sprintf(L": S[%d] F[%d]\r\n",
					chamber->MainTempController->SuccessCount,
					chamber->MainTempController->FailCount);
	}
	else
		str.sprintf(L": S[0] F[0]\r\n");
	title += "Main Temp. Controller\r\n";
	value += str;

	if (chamber->LimitControllerType != LIMIT_NONE)
	{
		if( chamber->ChamberTempLimitController )
		{
			str.sprintf(L": S[%d] F[%d]\r\n",
						chamber->ChamberTempLimitController->SuccessCount,
						chamber->ChamberTempLimitController->FailCount);
		}
		else
			str.sprintf(L": S[0] F[0]\r\n");
		title += "Temp. Limit Controller\r\n";
		value += str;
	}

	if( chamber->LoggerController[0] )
	{
		str.sprintf(L": S[%d] F[%d]\r\n",
					chamber->LoggerController[0]->SuccessCount,
					chamber->LoggerController[0]->FailCount);
	}
	else
		str.sprintf(L": S[0] F[0]\r\n");
	title += "Temp. Logger\r\n";
	value += str;

	if( GetManager()->UseO2Analyzer )
	{
		if( chamber->O2Analyzer )
		{
			str.sprintf(L": S[%d] F[%d]\r\n",
						chamber->O2Analyzer->SuccessCount,
						chamber->O2Analyzer->FailCount);
		}
		else
			str.sprintf(L": S[0] F[0]\r\n");
		title += "O2 Analyzer\r\n";
		value += str;
	}

	if( GetManager()->DPType )
	{
		if( chamber->DPController )
		{
			str.sprintf(L": S[%d] F[%d]\r\n",
						chamber->DPController->SuccessCount,
						chamber->DPController->FailCount);
		}
		else
			str.sprintf(L": S[0] F[0]\r\n");
		title += "Diff. Pressure\r\n";
		value += str;
	}

	title += "</P>";
	value += "</P>";
	CommMonitorPanel1->Text = title;
	CommInfo1->Text = value;

	title = (L"<P align=\"right\">");
	value = (L"<P align=\"left\">");

	if( GetManager()->DamperControlType != CYLINDER_DAMPER )
	{

		if( chamber->DamperMotor )
		{
			str.sprintf(L": S[%d] F[%d]\r\n",
						chamber->DamperMotor->SuccessCount,
						chamber->DamperMotor->FailCount);
		}
		else
			str.sprintf(L": S[0] F[0]\r\n");
		title += "Damper Motor\r\n";
		value += str;
	}

	if (GetManager()->DIOType == ICPDAS)
	{
		if( chamber->ModuleDI )
		{
			str.sprintf(L": S[%d] F[%d]\r\n",
						chamber->ModuleDI->SuccessCount,
						chamber->ModuleDI->FailCount);
		}
		else
			str.sprintf(L": S[0] F[0]\r\n");
		title += "DI Module\r\n";
		value += str;

		if( chamber->ModuleDO )
		{
			str.sprintf(L": S[%d] F[%d]\r\n",
						chamber->ModuleDO->SuccessCount,
						chamber->ModuleDO->FailCount);
		}
		else
			str.sprintf(L": S[0] F[0]\r\n");
		title += "DO Module\r\n";
		value += str;

		if( chamber->ModuleDIO )
		{
			str.sprintf(L": S[%d] F[%d]\r\n",
						chamber->ModuleDIO->SuccessCount,
						chamber->ModuleDIO->FailCount);
		}
		else
			str.sprintf(L": S[0] F[0]\r\n");
		title += "DIO Module\r\n";
		value += str;
	}
	else if (GetManager()->DIOType == OVEN_DIO)
	{
		if( chamber->OvenBoard )
		{
			str.sprintf(L": S[%d] F[%d]\r\n",
						chamber->OvenBoard->SuccessCount,
						chamber->OvenBoard->FailCount);
		}
		else
			str.sprintf(L": S[0] F[0]\r\n");
		title += "Oven Board\r\n";
		value += str;
	}

	if (GetManager()->UseInverter)
	{
		if( chamber->Inverter )
		{
			str.sprintf(L": S[%d] F[%d]\r\n",
						chamber->Inverter->SuccessCount,
						chamber->Inverter->FailCount);
		}
		else
			str.sprintf(L": S[0] F[0]\r\n");
		title += "Inverter\r\n";
		value += str;
	}

	title += "</P>";
	value += "</P>";
	CommMonitorPanel2->Text = title;
	CommInfo2->Text = value;

	if(GetManager()->AutomationKind == HANA_CIM && GetManager()->UseAutomation)
	{
		HanaCommStatus();
	}

	Refresh->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrmIO::BtnOutputClick(TObject *Sender)
{
	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( chamber == nullptr )
		return;

	UINT ioNum = (UINT)((TAdvGlassButton*)Sender)->Tag;
	bool on = chamber->DIO[ioNum]->IsOn();
	if( on )
		chamber->DIO[ioNum]->Off();
	else
		chamber->DIO[ioNum]->On();
}
//---------------------------------------------------------------------------
void __fastcall TFrmIO::FrameResize(TObject *Sender)
{
	RefreshButton();
}
//---------------------------------------------------------------------------

void __fastcall TFrmIO::HanaCommStatus()
{
	TColor clr;
	NormalOvenChamber *chamber = GetChamber(ChamberNo);
	if( chamber == nullptr )
		return;

	if(GetManager()->AutomationKind == HANA_CIM && GetManager()->UseAutomation)
	{
		clr = chamber->HanOvenCim->IsFail ? clSilver : clRed;
		String str;
		str.sprintf(L"Hana Comm_%d", ChamberNo);
		HanaComm->EditLabel->Caption = str;
		if( HanaComm->Color != clr )
		HanaComm->Color = clr;
	}
}
