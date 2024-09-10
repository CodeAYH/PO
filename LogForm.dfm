object FmLog: TFmLog
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Log'
  ClientHeight = 739
  ClientWidth = 1018
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlBtm: TPanel
    Left = 0
    Top = 608
    Width = 1018
    Height = 131
    Align = alBottom
    Color = 16777177
    TabOrder = 0
    object SelectLog: TAdvOfficeRadioGroup
      Left = 8
      Top = 7
      Width = 296
      Height = 115
      CaptionFont.Charset = DEFAULT_CHARSET
      CaptionFont.Color = clWindowText
      CaptionFont.Height = -11
      CaptionFont.Name = 'Tahoma'
      CaptionFont.Style = []
      Version = '1.8.1.3'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = #47569#51008' '#44256#46357
      Font.Style = [fsBold]
      ParentBackground = False
      ParentCtl3D = True
      ParentFont = False
      TabOrder = 0
      OnClick = SelectLogClick
      Items.Strings = (
        #47784#46304' '#44592#47197
        #50508#46988
        #49373#49328)
      Ellipsis = False
      ShowFocus = False
    end
    object AdvGroupBox1: TAdvGroupBox
      Left = 313
      Top = 7
      Width = 390
      Height = 114
      CaptionFont.Charset = DEFAULT_CHARSET
      CaptionFont.Color = clWindowText
      CaptionFont.Height = -11
      CaptionFont.Name = 'Tahoma'
      CaptionFont.Style = []
      ParentCtl3D = True
      TabOrder = 1
      object Label1: TLabel
        Left = 173
        Top = 62
        Width = 49
        Height = 42
        Caption = ' ~ '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -35
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object AllTime: TCheckBox
        Left = 20
        Top = 16
        Width = 97
        Height = 33
        Caption = #47784#46160
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        OnClick = AllTimeClick
      end
      object dtPickerEnd: TAdvDateTimePicker
        Left = 225
        Top = 64
        Width = 150
        Height = 38
        BevelInner = bvNone
        BevelOuter = bvNone
        Date = 40932.000000000000000000
        Format = ''
        Time = 0.748807870368182200
        DoubleBuffered = True
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = #47569#51008' '#44256#46357
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        Kind = dkDate
        ParentDoubleBuffered = False
        ParentFont = False
        TabOrder = 1
        OnCloseUp = SelectLogClick
        BorderStyle = bsSingle
        Ctl3D = False
        DateTime = 40932.748807870370000000
        Version = '1.3.6.6'
        LabelFont.Charset = ANSI_CHARSET
        LabelFont.Color = clNavy
        LabelFont.Height = -11
        LabelFont.Name = #47569#51008' '#44256#46357
        LabelFont.Style = [fsBold]
      end
      object dtPickerStart: TAdvDateTimePicker
        Left = 20
        Top = 64
        Width = 150
        Height = 38
        BevelInner = bvNone
        BevelOuter = bvNone
        Date = 40932.000000000000000000
        Format = ''
        Time = 0.748715277775772800
        DoubleBuffered = True
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = #47569#51008' '#44256#46357
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        Kind = dkDate
        ParentDoubleBuffered = False
        ParentFont = False
        TabOrder = 2
        OnCloseUp = SelectLogClick
        BorderStyle = bsSingle
        Ctl3D = False
        DateTime = 40932.748715277780000000
        Version = '1.3.6.6'
        LabelFont.Charset = ANSI_CHARSET
        LabelFont.Color = clNavy
        LabelFont.Height = -11
        LabelFont.Name = #47569#51008' '#44256#46357
        LabelFont.Style = [fsBold]
      end
    end
    object BtnClose: TAdvGlassButton
      Left = 864
      Top = 7
      Width = 147
      Height = 114
      BackColor = clRed
      Caption = #45803#44592
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clWhite
      GlowColor = 16760205
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clWhite
      ParentFont = False
      ShineColor = clWhite
      TabOrder = 2
      Version = '1.3.3.1'
      OnClick = BtnCloseClick
    end
    object SaveToCSVButton: TAdvGlassButton
      Left = 711
      Top = 7
      Width = 147
      Height = 115
      BackColor = clLime
      Caption = 'Save to CSV'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clWhite
      GlowColor = 16760205
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clWhite
      ParentFont = False
      ShineColor = clWhite
      TabOrder = 3
      Version = '1.3.3.1'
      OnClick = SaveToCSVButtonClick
    end
  end
  object LogGrid: TAdvStringGrid
    Left = 0
    Top = 0
    Width = 1018
    Height = 608
    ColCount = 4
    DoubleBuffered = True
    DrawingStyle = gdsClassic
    FixedCols = 0
    RowCount = 2
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
    ParentDoubleBuffered = False
    ParentFont = False
    TabOrder = 1
    ActiveRowColor = clHighlight
    GridLineColor = 15855083
    GridFixedLineColor = 13745060
    HoverRowColor = clHighlight
    HintColor = clBtnHighlight
    ActiveCellFont.Charset = DEFAULT_CHARSET
    ActiveCellFont.Color = clWindowText
    ActiveCellFont.Height = -11
    ActiveCellFont.Name = 'Tahoma'
    ActiveCellFont.Style = [fsBold]
    ActiveCellColor = 10344697
    ActiveCellColorTo = 6210033
    Bands.PrimaryColor = clHighlight
    ControlLook.FixedGradientFrom = 16513526
    ControlLook.FixedGradientTo = 15260626
    ControlLook.FixedGradientHoverFrom = 15000287
    ControlLook.FixedGradientHoverTo = 14406605
    ControlLook.FixedGradientHoverMirrorFrom = 14406605
    ControlLook.FixedGradientHoverMirrorTo = 13813180
    ControlLook.FixedGradientHoverBorder = 12033927
    ControlLook.FixedGradientDownFrom = 14991773
    ControlLook.FixedGradientDownTo = 14991773
    ControlLook.FixedGradientDownMirrorFrom = 14991773
    ControlLook.FixedGradientDownMirrorTo = 14991773
    ControlLook.FixedGradientDownBorder = 14991773
    ControlLook.DropDownHeader.Font.Charset = DEFAULT_CHARSET
    ControlLook.DropDownHeader.Font.Color = clWindowText
    ControlLook.DropDownHeader.Font.Height = -11
    ControlLook.DropDownHeader.Font.Name = 'Tahoma'
    ControlLook.DropDownHeader.Font.Style = []
    ControlLook.DropDownHeader.Visible = True
    ControlLook.DropDownHeader.Buttons = <>
    ControlLook.DropDownFooter.Font.Charset = DEFAULT_CHARSET
    ControlLook.DropDownFooter.Font.Color = clWindowText
    ControlLook.DropDownFooter.Font.Height = -11
    ControlLook.DropDownFooter.Font.Name = 'Tahoma'
    ControlLook.DropDownFooter.Font.Style = []
    ControlLook.DropDownFooter.Visible = True
    ControlLook.DropDownFooter.Buttons = <>
    Filter = <>
    FilterDropDown.Font.Charset = DEFAULT_CHARSET
    FilterDropDown.Font.Color = clWindowText
    FilterDropDown.Font.Height = -11
    FilterDropDown.Font.Name = 'Tahoma'
    FilterDropDown.Font.Style = []
    FilterDropDown.TextChecked = 'Checked'
    FilterDropDown.TextUnChecked = 'Unchecked'
    FilterDropDownClear = '(All)'
    FilterEdit.TypeNames.Strings = (
      'Starts with'
      'Ends with'
      'Contains'
      'Not contains'
      'Equal'
      'Not equal'
      'Clear')
    FixedRowHeight = 22
    FixedRowAlways = True
    FixedFont.Charset = DEFAULT_CHARSET
    FixedFont.Color = clBlack
    FixedFont.Height = -11
    FixedFont.Name = 'Tahoma'
    FixedFont.Style = [fsBold]
    FloatFormat = '%.2f'
    HoverButtons.Buttons = <>
    HTMLSettings.ImageFolder = 'images'
    HTMLSettings.ImageBaseName = 'img'
    Look = glOffice2007
    MouseActions.DirectEdit = True
    PrintSettings.DateFormat = 'dd/mm/yyyy'
    PrintSettings.Font.Charset = DEFAULT_CHARSET
    PrintSettings.Font.Color = clWindowText
    PrintSettings.Font.Height = -11
    PrintSettings.Font.Name = 'Tahoma'
    PrintSettings.Font.Style = []
    PrintSettings.FixedFont.Charset = DEFAULT_CHARSET
    PrintSettings.FixedFont.Color = clWindowText
    PrintSettings.FixedFont.Height = -11
    PrintSettings.FixedFont.Name = 'Tahoma'
    PrintSettings.FixedFont.Style = []
    PrintSettings.HeaderFont.Charset = DEFAULT_CHARSET
    PrintSettings.HeaderFont.Color = clWindowText
    PrintSettings.HeaderFont.Height = -11
    PrintSettings.HeaderFont.Name = 'Tahoma'
    PrintSettings.HeaderFont.Style = []
    PrintSettings.FooterFont.Charset = DEFAULT_CHARSET
    PrintSettings.FooterFont.Color = clWindowText
    PrintSettings.FooterFont.Height = -11
    PrintSettings.FooterFont.Name = 'Tahoma'
    PrintSettings.FooterFont.Style = []
    PrintSettings.PageNumSep = '/'
    SearchFooter.Color = 16513526
    SearchFooter.ColorTo = clNone
    SearchFooter.FindNextCaption = 'Find &next'
    SearchFooter.FindPrevCaption = 'Find &previous'
    SearchFooter.Font.Charset = DEFAULT_CHARSET
    SearchFooter.Font.Color = clWindowText
    SearchFooter.Font.Height = -11
    SearchFooter.Font.Name = 'Tahoma'
    SearchFooter.Font.Style = []
    SearchFooter.HighLightCaption = 'Highlight'
    SearchFooter.HintClose = 'Close'
    SearchFooter.HintFindNext = 'Find next occurrence'
    SearchFooter.HintFindPrev = 'Find previous occurrence'
    SearchFooter.HintHighlight = 'Highlight occurrences'
    SearchFooter.MatchCaseCaption = 'Match case'
    SearchFooter.ResultFormat = '(%d of %d)'
    SelectionColor = 6210033
    UIStyle = tsOffice2007Luna
    VAlignment = vtaCenter
    Version = '8.7.2.4'
  end
  object SaveTextFileDialog: TSaveTextFileDialog
    DefaultExt = 'csv'
    Filter = 'CSV|csv'
    Left = 616
    Top = 624
  end
end
