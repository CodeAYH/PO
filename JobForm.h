//---------------------------------------------------------------------------

#ifndef JobFormH
#define JobFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvCombo.hpp"
#include "AdvGlassButton.hpp"
#include "AdvGrid.hpp"
#include "AdvMemo.hpp"
#include "AdvMenus.hpp"
#include "AdvObj.hpp"
#include "AdvOfficeStatusBar.hpp"
#include "AdvOfficeStatusBarStylers.hpp"
#include "AdvPanel.hpp"
#include "AdvSmoothProgressBar.hpp"
#include "AdvSmoothStatusIndicator.hpp"
#include "AdvStickyPopupMenu.hpp"
#include "BaseGrid.hpp"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.TeeSpline.hpp>

#include "Defines.h"
#include "ChamberFrame.h"
#include "BarunMES.h"
//---------------------------------------------------------------------------
class TFrmChamber;
class TFmJob : public TForm
{
__published:	// IDE-managed Components
	TAdvPanel *CaptionPanel;
	TAdvGlassButton *MenuButton;
	TAdvPanel *LoginModePanel;
	TAdvOfficeStatusBar *AutomationStatusBar;
	TAdvMenuStyler *AdvMenuStyler1;
	TAdvStickyPopupMenu *PopupMenu;
	TAdvOfficeStatusBarOfficeStyler *GemStatusStyler;
	TTimer *UpdateAutomationBar;
	TTimer *OFilmMESDataSave;
	TTimer *OFilmMESSendFile;
	TTimer *ScreenSaverTimer;
	TAdvGlassButton *BtnTCard;
	TEdit *OPIDEdit;
	TAdvGlassButton *OPIDChangeButton;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CaptionPanelMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall MenuButtonClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall UpdateAutomationBarTimer(TObject *Sender);
	void __fastcall AutomationStatusBarClick(TObject *Sender);
	// 20180209 - FTP (OFILM MES) - jhlim
	void __fastcall OFilmMESDataSaveTimer(TObject *Sender);
	void __fastcall OFilmMESSendFileTimer(TObject *Sender);
	void __fastcall ScreenSaverTimerTimer(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormMouseActivate(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y, int HitTest, TMouseActivate &MouseActivate);
	void __fastcall BtnTCardClick(TObject *Sender);
	void __fastcall OPIDChangeButtonClick(TObject *Sender);



private:	// User declarations

	void __fastcall OnLogout(TObject *Sender);
	void __fastcall OnLog(TObject *Sender);
	void __fastcall OnGraph(TObject *Sender);
	void __fastcall OnRegisterUser(TObject *Sender);
	void __fastcall OnIOTest(TObject *Sender);
	void __fastcall OnExit(TObject *Sender);
	void __fastcall OnSetting(TObject *Sender);

	void __fastcall Localize();

public:		// User declarations
	/**
	 * @brief 4챔버일 경우 사용.
	 * @details true : 한 챔버가 전체 뷰를 사용, false : 4챔버로 나누고 꼭 필요한 컴포넌트만 보여준다.
	 * @author 유영재, yjyoo@visionsemicon.co.kr
	 */
	bool ShowDetailMode;

	/**
	 * @brief 4챔버이고 ShowDetailMode가 true 일 경우 사용
	 * @details 현재 전체 모드로 보여 주는 챔버의 번호
	 * @author 유영재, yjyoo@visionsemicon.co.kr
	 */
	int CurrentChamber;
	ChamberFramePtr ChamberFrame[MAX_CHAMBER_COUNT];

	// 20181022 - ScreenSaver (infineon) - jhlim
	int nScreenSaverCnt;

	__fastcall TFmJob(TComponent* Owner);

	bool __fastcall IsChamberStarting();
	void __fastcall ArrangeChamber();
	void __fastcall UpdateAutomationStatus();
};
//---------------------------------------------------------------------------
extern PACKAGE TFmJob *FmJob;
//---------------------------------------------------------------------------
#endif
