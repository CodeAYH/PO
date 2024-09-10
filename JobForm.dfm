object FmJob: TFmJob
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'FmJob'
  ClientHeight = 768
  ClientWidth = 1024
  Color = clSilver
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnKeyPress = FormKeyPress
  OnMouseActivate = FormMouseActivate
  OnMouseMove = FormMouseMove
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object CaptionPanel: TAdvPanel
    Left = 0
    Top = 0
    Width = 1024
    Height = 30
    Color = clNavy
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -24
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    UseDockManager = True
    OnMouseDown = CaptionPanelMouseDown
    Version = '2.6.3.4'
    BorderColor = clBlack
    Caption.Color = clNavy
    Caption.ColorTo = clNone
    Caption.Font.Charset = DEFAULT_CHARSET
    Caption.Font.Color = clWhite
    Caption.Font.Height = -24
    Caption.Font.Name = 'Tahoma'
    Caption.Font.Style = [fsBold]
    Caption.Height = 31
    Caption.Indent = 0
    ColorTo = clNavy
    DoubleBuffered = True
    FreeOnClose = True
    StatusBar.Font.Charset = DEFAULT_CHARSET
    StatusBar.Font.Color = clWindowText
    StatusBar.Font.Height = -11
    StatusBar.Font.Name = 'Tahoma'
    StatusBar.Font.Style = []
    Text = ''
    FullHeight = 200
    object MenuButton: TAdvGlassButton
      Left = 820
      Top = 0
      Width = 204
      Height = 28
      BackColor = clLime
      BackGroundSymbolColor = clLime
      Caption = 'menu'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -24
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clBlue
      GlowColor = 16760205
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clLime
      ParentFont = False
      ShineColor = clWhite
      TabOrder = 0
      Version = '1.3.3.1'
      OnClick = MenuButtonClick
    end
    object LoginModePanel: TAdvPanel
      Left = 3
      Top = 2
      Width = 175
      Height = 29
      Color = clNavy
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -21
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      UseDockManager = True
      Version = '2.6.3.4'
      BorderColor = clBlack
      Caption.Color = clHighlight
      Caption.ColorTo = clNone
      Caption.Font.Charset = DEFAULT_CHARSET
      Caption.Font.Color = clBlack
      Caption.Font.Height = -24
      Caption.Font.Name = 'Tahoma'
      Caption.Font.Style = [fsBold]
      Caption.Indent = 0
      ColorTo = clNavy
      DoubleBuffered = True
      StatusBar.Font.Charset = DEFAULT_CHARSET
      StatusBar.Font.Color = clWindowText
      StatusBar.Font.Height = -11
      StatusBar.Font.Name = 'Tahoma'
      StatusBar.Font.Style = []
      Text = 'OPERATOR'
      FullHeight = 29
    end
    object OPIDEdit: TEdit
      Left = 180
      Top = 0
      Width = 121
      Height = 37
      TabOrder = 2
      Text = 'OPIDEdit'
      Visible = False
    end
    object OPIDChangeButton: TAdvGlassButton
      Left = 302
      Top = 0
      Width = 80
      Height = 28
      BackColor = clAqua
      BackGroundSymbolColor = clAqua
      Caption = 'Change'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clBlue
      GlowColor = 16760205
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clAqua
      ParentFont = False
      ShineColor = clWhite
      TabOrder = 3
      Version = '1.3.3.1'
      Visible = False
      OnClick = OPIDChangeButtonClick
    end
  end
  object AutomationStatusBar: TAdvOfficeStatusBar
    Left = 0
    Top = 749
    Width = 1024
    Height = 19
    AnchorHint = False
    Align = alNone
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Panels = <
      item
        Alignment = taCenter
        AppearanceStyle = psLight
        DateFormat = 'yyyy/MM/dd'
        Progress.BackGround = clNone
        Progress.Indication = piPercentage
        Progress.Min = 0
        Progress.Max = 100
        Progress.Position = 0
        Progress.Level0Color = clLime
        Progress.Level0ColorTo = 14811105
        Progress.Level1Color = clYellow
        Progress.Level1ColorTo = 13303807
        Progress.Level2Color = 5483007
        Progress.Level2ColorTo = 11064319
        Progress.Level3Color = clRed
        Progress.Level3ColorTo = 13290239
        Progress.Level1Perc = 70
        Progress.Level2Perc = 90
        Progress.BorderColor = clBlack
        Progress.ShowBorder = False
        Progress.Stacked = False
        Style = psDate
        TimeFormat = 'AMPM h:mm:ss'
        Width = 80
      end
      item
        Alignment = taCenter
        AppearanceStyle = psLight
        DateFormat = 'yyyy/MM/dd'
        Progress.BackGround = clNone
        Progress.Indication = piPercentage
        Progress.Min = 0
        Progress.Max = 100
        Progress.Position = 0
        Progress.Level0Color = clLime
        Progress.Level0ColorTo = 14811105
        Progress.Level1Color = clYellow
        Progress.Level1ColorTo = 13303807
        Progress.Level2Color = 5483007
        Progress.Level2ColorTo = 11064319
        Progress.Level3Color = clRed
        Progress.Level3ColorTo = 13290239
        Progress.Level1Perc = 70
        Progress.Level2Perc = 90
        Progress.BorderColor = clBlack
        Progress.ShowBorder = False
        Progress.Stacked = False
        Style = psTime
        TimeFormat = 'AMPM h:mm:ss'
        Width = 90
      end
      item
        Alignment = taCenter
        AppearanceStyle = psDark
        DateFormat = 'yyyy/MM/dd'
        HTMLOffsetY = 4
        Progress.BackGround = clNone
        Progress.Indication = piPercentage
        Progress.Min = 0
        Progress.Max = 100
        Progress.Position = 0
        Progress.Level0Color = clLime
        Progress.Level0ColorTo = 14811105
        Progress.Level1Color = clYellow
        Progress.Level1ColorTo = 13303807
        Progress.Level2Color = 5483007
        Progress.Level2ColorTo = 11064319
        Progress.Level3Color = clRed
        Progress.Level3ColorTo = 13290239
        Progress.Level1Perc = 70
        Progress.Level2Perc = 90
        Progress.BorderColor = clBlack
        Progress.ShowBorder = False
        Progress.Stacked = False
        Style = psText
        Text = '255.255.255.255'
        TimeFormat = 'AMPM h:mm:ss'
        Width = 110
      end
      item
        Alignment = taCenter
        AppearanceStyle = psDark
        DateFormat = 'yyyy/MM/dd'
        HTMLOffsetY = 4
        Progress.BackGround = clNone
        Progress.Indication = piPercentage
        Progress.Min = 0
        Progress.Max = 100
        Progress.Position = 0
        Progress.Level0Color = clLime
        Progress.Level0ColorTo = 14811105
        Progress.Level1Color = clYellow
        Progress.Level1ColorTo = 13303807
        Progress.Level2Color = 5483007
        Progress.Level2ColorTo = 11064319
        Progress.Level3Color = clRed
        Progress.Level3ColorTo = 13290239
        Progress.Level1Perc = 70
        Progress.Level2Perc = 90
        Progress.BorderColor = clBlack
        Progress.ShowBorder = False
        Progress.Stacked = False
        Style = psText
        Text = 'OFFLINE'
        TimeFormat = 'AMPM h:mm:ss'
        Width = 60
      end
      item
        Alignment = taCenter
        AppearanceStyle = psDark
        DateFormat = 'yyyy/MM/dd'
        HTMLOffsetY = 4
        Progress.BackGround = clNone
        Progress.Indication = piPercentage
        Progress.Min = 0
        Progress.Max = 100
        Progress.Position = 0
        Progress.Level0Color = clLime
        Progress.Level0ColorTo = 14811105
        Progress.Level1Color = clYellow
        Progress.Level1ColorTo = 13303807
        Progress.Level2Color = 5483007
        Progress.Level2ColorTo = 11064319
        Progress.Level3Color = clRed
        Progress.Level3ColorTo = 13290239
        Progress.Level1Perc = 70
        Progress.Level2Perc = 90
        Progress.BorderColor = clBlack
        Progress.ShowBorder = False
        Progress.Stacked = False
        Style = psText
        Text = 'DISCONNECTED'
        TimeFormat = 'AMPM h:mm:ss'
        Width = 110
      end
      item
        Alignment = taCenter
        AppearanceStyle = psDark
        DateFormat = 'yyyy/MM/dd'
        HTMLOffsetY = 4
        Progress.BackGround = clNone
        Progress.Indication = piPercentage
        Progress.Min = 0
        Progress.Max = 100
        Progress.Position = 0
        Progress.Level0Color = clLime
        Progress.Level0ColorTo = 14811105
        Progress.Level1Color = clYellow
        Progress.Level1ColorTo = 13303807
        Progress.Level2Color = 5483007
        Progress.Level2ColorTo = 11064319
        Progress.Level3Color = clRed
        Progress.Level3ColorTo = 13290239
        Progress.Level1Perc = 70
        Progress.Level2Perc = 90
        Progress.BorderColor = clBlack
        Progress.ShowBorder = False
        Progress.Stacked = False
        Style = psText
        Text = 'DISABLED'
        TimeFormat = 'AMPM h:mm:ss'
        Width = 70
      end
      item
        AppearanceStyle = psLight
        DateFormat = 'yyyy/MM/dd'
        Progress.BackGround = clNone
        Progress.Indication = piPercentage
        Progress.Min = 0
        Progress.Max = 100
        Progress.Position = 0
        Progress.Level0Color = clLime
        Progress.Level0ColorTo = 14811105
        Progress.Level1Color = clYellow
        Progress.Level1ColorTo = 13303807
        Progress.Level2Color = 5483007
        Progress.Level2ColorTo = 11064319
        Progress.Level3Color = clRed
        Progress.Level3ColorTo = 13290239
        Progress.Level1Perc = 70
        Progress.Level2Perc = 90
        Progress.BorderColor = clBlack
        Progress.ShowBorder = False
        Progress.Stacked = False
        TimeFormat = 'AMPM h:mm:ss'
        Width = 50
      end>
    ShowSplitter = True
    SimplePanel = False
    Styler = GemStatusStyler
    UseSystemFont = False
    OnClick = AutomationStatusBarClick
    Version = '1.7.3.4'
  end
  object BtnTCard: TAdvGlassButton
    Left = 696
    Top = 1
    Width = 105
    Height = 27
    BackColor = clFuchsia
    BackGroundSymbolColor = clFuchsia
    Caption = 'T-Card'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -24
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ForeColor = clBlue
    GlowColor = 16760205
    ImageIndex = -1
    InnerBorderColor = clBlack
    OuterBorderColor = clFuchsia
    ParentFont = False
    ShineColor = clWhite
    TabOrder = 3
    TabStop = False
    Version = '1.3.3.1'
    OnClick = BtnTCardClick
  end
  object AdvMenuStyler1: TAdvMenuStyler
    AntiAlias = aaNone
    Background.Position = bpCenter
    SelectedItem.Font.Charset = DEFAULT_CHARSET
    SelectedItem.Font.Color = clWindowText
    SelectedItem.Font.Height = -21
    SelectedItem.Font.Name = #47569#51008' '#44256#46357
    SelectedItem.Font.Style = [fsBold]
    SelectedItem.NotesFont.Charset = DEFAULT_CHARSET
    SelectedItem.NotesFont.Color = clWindowText
    SelectedItem.NotesFont.Height = -9
    SelectedItem.NotesFont.Name = #47569#51008' '#44256#46357
    SelectedItem.NotesFont.Style = []
    RootItem.Font.Charset = DEFAULT_CHARSET
    RootItem.Font.Color = clMenuText
    RootItem.Font.Height = -11
    RootItem.Font.Name = #47569#51008' '#44256#46357
    RootItem.Font.Style = [fsBold]
    Glyphs.SubMenu.Data = {
      5A000000424D5A000000000000003E0000002800000004000000070000000100
      0100000000001C0000000000000000000000020000000200000000000000FFFF
      FF0070000000300000001000000000000000100000003000000070000000}
    Glyphs.Check.Data = {
      7E000000424D7E000000000000003E0000002800000010000000100000000100
      010000000000400000000000000000000000020000000200000000000000FFFF
      FF00FFFF0000FFFF0000FFFF0000FFFF0000FDFF0000F8FF0000F07F0000F23F
      0000F71F0000FF8F0000FFCF0000FFEF0000FFFF0000FFFF0000FFFF0000FFFF
      0000}
    Glyphs.Radio.Data = {
      7E000000424D7E000000000000003E0000002800000010000000100000000100
      010000000000400000000000000000000000020000000200000000000000FFFF
      FF00FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FC3F0000F81F0000F81F
      0000F81F0000F81F0000FC3F0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000}
    SideBar.Font.Charset = DEFAULT_CHARSET
    SideBar.Font.Color = clWhite
    SideBar.Font.Height = -19
    SideBar.Font.Name = 'Tahoma'
    SideBar.Font.Style = [fsBold, fsItalic]
    SideBar.Image.Position = bpCenter
    SideBar.Background.Position = bpCenter
    SideBar.SplitterColorTo = clBlack
    Separator.GradientType = gtBoth
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuText
    Font.Height = -21
    Font.Name = #47569#51008' '#44256#46357
    Font.Style = [fsBold]
    NotesFont.Charset = DEFAULT_CHARSET
    NotesFont.Color = clGray
    NotesFont.Height = -21
    NotesFont.Name = #47569#51008' '#44256#46357
    NotesFont.Style = [fsBold]
    ButtonAppearance.CaptionFont.Charset = DEFAULT_CHARSET
    ButtonAppearance.CaptionFont.Color = clWindowText
    ButtonAppearance.CaptionFont.Height = -21
    ButtonAppearance.CaptionFont.Name = 'Segoe UI'
    ButtonAppearance.CaptionFont.Style = [fsBold]
    ButtonAppearance.UseSystemFont = False
    Left = 536
    Top = 320
  end
  object PopupMenu: TAdvStickyPopupMenu
    ButtonBar = <>
    MenuItems = <
      item
        Style = isMenuItem
        Caption = 'aaa'
      end
      item
        Style = isMenuItem
        Caption = 'bbb'
      end>
    ShowButtonBar = False
    Styler = AdvMenuStyler1
    Version = '1.6.2.2'
    Left = 624
    Top = 320
  end
  object GemStatusStyler: TAdvOfficeStatusBarOfficeStyler
    Style = psOffice2003Silver
    BorderColor = 11769496
    PanelAppearanceLight.BorderColor = clNone
    PanelAppearanceLight.BorderColorHot = clGray
    PanelAppearanceLight.BorderColorDown = 12032925
    PanelAppearanceLight.Color = 16249843
    PanelAppearanceLight.ColorTo = 15128792
    PanelAppearanceLight.ColorHot = 13958143
    PanelAppearanceLight.ColorHotTo = 6538487
    PanelAppearanceLight.ColorDown = 9232890
    PanelAppearanceLight.ColorDownTo = 1940207
    PanelAppearanceLight.ColorMirror = 15128792
    PanelAppearanceLight.ColorMirrorTo = 15128792
    PanelAppearanceLight.ColorMirrorHot = 6538487
    PanelAppearanceLight.ColorMirrorHotTo = 6538487
    PanelAppearanceLight.ColorMirrorDown = 1940207
    PanelAppearanceLight.ColorMirrorDownTo = 1940207
    PanelAppearanceLight.TextColor = clBlack
    PanelAppearanceLight.TextColorHot = clBlack
    PanelAppearanceLight.TextColorDown = clBlack
    PanelAppearanceLight.TextStyle = [fsBold]
    PanelAppearanceDark.BorderColor = clNone
    PanelAppearanceDark.BorderColorHot = clGray
    PanelAppearanceDark.BorderColorDown = 12032925
    PanelAppearanceDark.Color = 15128792
    PanelAppearanceDark.ColorTo = 12032925
    PanelAppearanceDark.ColorHot = 13958143
    PanelAppearanceDark.ColorHotTo = 6538487
    PanelAppearanceDark.ColorDown = 9232890
    PanelAppearanceDark.ColorDownTo = 1940207
    PanelAppearanceDark.ColorMirror = 12032925
    PanelAppearanceDark.ColorMirrorTo = 12032925
    PanelAppearanceDark.ColorMirrorHot = 6538487
    PanelAppearanceDark.ColorMirrorHotTo = 6538487
    PanelAppearanceDark.ColorMirrorDown = 1940207
    PanelAppearanceDark.ColorMirrorDownTo = 1940207
    PanelAppearanceDark.TextColor = clWhite
    PanelAppearanceDark.TextColorHot = clWhite
    PanelAppearanceDark.TextColorDown = clWhite
    PanelAppearanceDark.TextStyle = [fsBold]
    Left = 704
    Top = 320
  end
  object UpdateAutomationBar: TTimer
    Interval = 100
    OnTimer = UpdateAutomationBarTimer
    Left = 288
    Top = 344
  end
  object OFilmMESDataSave: TTimer
    Enabled = False
    Interval = 5000
    OnTimer = OFilmMESDataSaveTimer
    Left = 64
    Top = 72
  end
  object OFilmMESSendFile: TTimer
    Enabled = False
    Interval = 600000
    OnTimer = OFilmMESSendFileTimer
    Left = 64
    Top = 144
  end
  object ScreenSaverTimer: TTimer
    Enabled = False
    Interval = 900
    OnTimer = ScreenSaverTimerTimer
    Left = 72
    Top = 344
  end
end
