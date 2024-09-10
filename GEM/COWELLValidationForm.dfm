object CowellValidationFrm: TCowellValidationFrm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'COWELL GEM'
  ClientHeight = 382
  ClientWidth = 340
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object lblChamber: TEllipsLabel
    Left = -144
    Top = 8
    Width = 484
    Height = 36
    Alignment = taCenter
    AutoSize = False
    Caption = 'CHAMBER'
    Color = clNavy
    EllipsisPosition = epEndEllipsis
    Font.Charset = ANSI_CHARSET
    Font.Color = clWhite
    Font.Height = -27
    Font.Name = #47569#51008' '#44256#46357
    Font.Style = [fsBold]
    GlowSize = 10
    ParentColor = False
    ParentFont = False
    EllipsType = etNone
    Version = '1.0.0.0'
  end
  object ListGrid: TAdvStringGrid
    Left = 8
    Top = 50
    Width = 325
    Height = 279
    ColCount = 2
    DefaultColWidth = 260
    DefaultRowHeight = 25
    DoubleBuffered = True
    DrawingStyle = gdsClassic
    FixedColor = clWhite
    RowCount = 11
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goEditing]
    ParentDoubleBuffered = False
    ParentFont = False
    TabOrder = 0
    OnGetCellColor = ListGridGetCellColor
    OnCanEditCell = ListGridCanEditCell
    OnGetEditorType = ListGridGetEditorType
    OnEditCellDone = ListGridEditCellDone
    OnResize = ListGridResize
    ActiveCellFont.Charset = DEFAULT_CHARSET
    ActiveCellFont.Color = clWindowText
    ActiveCellFont.Height = -11
    ActiveCellFont.Name = 'Tahoma'
    ActiveCellFont.Style = [fsBold]
    ActiveCellColor = clBtnFace
    ActiveCellColorTo = clBtnFace
    ControlLook.FixedGradientHoverFrom = clGray
    ControlLook.FixedGradientHoverTo = clWhite
    ControlLook.FixedGradientDownFrom = clGray
    ControlLook.FixedGradientDownTo = clSilver
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
    DefaultAlignment = taCenter
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
    FixedColWidth = 90
    FixedRowHeight = 25
    FixedColAlways = True
    FixedFont.Charset = DEFAULT_CHARSET
    FixedFont.Color = clBlack
    FixedFont.Height = -13
    FixedFont.Name = 'Tahoma'
    FixedFont.Style = []
    FloatFormat = '%.2f'
    HoverButtons.Buttons = <>
    HTMLSettings.ImageFolder = 'images'
    HTMLSettings.ImageBaseName = 'img'
    Navigation.AlwaysEdit = True
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
    ScrollWidth = 21
    SearchFooter.Color = clBtnFace
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
    SelectionColor = clHighlight
    SelectionTextColor = clHighlightText
    ShowDesignHelper = False
    UIStyle = tsWindowsXP
    VAlignment = vtaCenter
    Version = '8.7.2.9'
    ColWidths = (
      90
      260)
    RowHeights = (
      25
      25
      25
      25
      25
      25
      25
      25
      25
      25
      25)
  end
  object RequestValidationButton: TAdvGlowButton
    Left = 187
    Top = 335
    Width = 145
    Height = 37
    Caption = 'validation'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    NotesFont.Charset = DEFAULT_CHARSET
    NotesFont.Color = clWindowText
    NotesFont.Height = -11
    NotesFont.Name = 'Tahoma'
    NotesFont.Style = []
    ParentFont = False
    Rounded = True
    TabOrder = 1
    OnClick = RequestValidationButtonClick
    Appearance.BorderColor = 11382963
    Appearance.BorderColorHot = 11565130
    Appearance.BorderColorCheckedHot = 11565130
    Appearance.BorderColorDown = 11565130
    Appearance.BorderColorChecked = 13744549
    Appearance.BorderColorDisabled = 13948116
    Appearance.Color = clWhite
    Appearance.ColorTo = clWhite
    Appearance.ColorChecked = 13744549
    Appearance.ColorCheckedTo = 13744549
    Appearance.ColorDisabled = clWhite
    Appearance.ColorDisabledTo = clNone
    Appearance.ColorDown = 11565130
    Appearance.ColorDownTo = 11565130
    Appearance.ColorHot = 16444643
    Appearance.ColorHotTo = 16444643
    Appearance.ColorMirror = clWhite
    Appearance.ColorMirrorTo = clWhite
    Appearance.ColorMirrorHot = 16444643
    Appearance.ColorMirrorHotTo = 16444643
    Appearance.ColorMirrorDown = 11565130
    Appearance.ColorMirrorDownTo = 11565130
    Appearance.ColorMirrorChecked = 13744549
    Appearance.ColorMirrorCheckedTo = 13744549
    Appearance.ColorMirrorDisabled = clWhite
    Appearance.ColorMirrorDisabledTo = clNone
    Appearance.GradientHot = ggVertical
    Appearance.GradientMirrorHot = ggVertical
    Appearance.GradientDown = ggVertical
    Appearance.GradientMirrorDown = ggVertical
    Appearance.GradientChecked = ggVertical
    Appearance.TextColorChecked = 3750459
    Appearance.TextColorDown = 2303013
    Appearance.TextColorHot = 2303013
    Appearance.TextColorDisabled = 13948116
  end
end
