object FmTCardValidation: TFmTCardValidation
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'T-Card Recipe Validation'
  ClientHeight = 465
  ClientWidth = 624
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object TCardListGrid: TAdvStringGrid
    Left = 8
    Top = 91
    Width = 264
    Height = 366
    ColCount = 2
    DefaultColWidth = 130
    DefaultRowHeight = 25
    DrawingStyle = gdsClassic
    RowCount = 2
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goEditing]
    ParentFont = False
    TabOrder = 0
    OnCanEditCell = TCardListGridCanEditCell
    ActiveCellFont.Charset = DEFAULT_CHARSET
    ActiveCellFont.Color = clWindowText
    ActiveCellFont.Height = -11
    ActiveCellFont.Name = 'Tahoma'
    ActiveCellFont.Style = [fsBold]
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
    FixedColWidth = 25
    FixedRowHeight = 25
    FixedColAlways = True
    FixedFont.Charset = DEFAULT_CHARSET
    FixedFont.Color = clWindowText
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
    SelectionColor = clWhite
    ShowDesignHelper = False
    VAlignment = vtaCenter
    Version = '8.7.0.1'
    ColWidths = (
      25
      130)
  end
  object InputTCardEdit: TLabeledEdit
    Left = 366
    Top = 91
    Width = 243
    Height = 29
    EditLabel.Width = 69
    EditLabel.Height = 29
    EditLabel.Caption = 'T-card'
    EditLabel.Font.Charset = DEFAULT_CHARSET
    EditLabel.Font.Color = clWindowText
    EditLabel.Font.Height = -24
    EditLabel.Font.Name = 'Tahoma'
    EditLabel.Font.Style = []
    EditLabel.ParentFont = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    LabelPosition = lpLeft
    LabelSpacing = 10
    ParentFont = False
    TabOrder = 1
    OnKeyDown = InputTCardEditKeyDown
  end
  object btnCancel: TAdvGlassButton
    Left = 470
    Top = 413
    Width = 149
    Height = 45
    BackColor = clWhite
    Caption = 'Cancel'
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlack
    Font.Height = -15
    Font.Name = #47569#51008' '#44256#46357
    Font.Style = [fsBold]
    ForeColor = clWhite
    GlowColor = clRed
    ImageIndex = -1
    InnerBorderColor = clBlack
    OuterBorderColor = clWhite
    Picture.Data = {
      89504E470D0A1A0A0000000D4948445200000016000000160806000000C4B46C
      3B0000000473424954080808087C086488000000097048597300000DD700000D
      D70142289B780000001974455874536F667477617265007777772E696E6B7363
      6170652E6F72679BEE3C1A0000053049444154388D7DD55B4CD3771407F02314
      54700C452EA220B485B414845A24D60B4A6696B8CDCDBDECC517B32779151341
      2201832E9946016714615CD4C4659B4B8C531187A023ACB4B45CDA52E4564065
      60EF17FAEFF5FFDD4329023A7FC93FFF87F3CFE77FF23BE7E4AC0140A1736BD7
      AE6FB649A5159E8505A3B9BFFFC7632A55077DE434F07899F112C995C898980D
      6F158A1F8E0F0EB62D050110006AC9CFFF7AA8B1719A6559B02C0BD5B56BFA96
      DCDCCF43F1D5CF8DD4D4ACCED25235C33070B95C50D5D74F36E6E57D198A1300
      BA5350F095BAA92988CABB10D08FC2EFF7435957A76F118B3F5B8DDE4C4D1575
      9695A91986814DA786A54F8685850528EBEBF52D8B38DDD9BDFBB0BAB93988B6
      DF43202B021E69127CA31AF87C3E28EAEA265B2592A210DAC8E3653F2B2DD532
      0C03ABBC076A7E2C0652A230DFFE104EA713CA8686A9E61D3B8E507B717107CB
      B26087E4080839F066105C3C82A320099E971A78BD5EC86B6A265AC4E2C27A2E
      37A7ABAC6C09D5F063A1DA4C50C4117A93A360D2AAE17038F0B8B858CEF1582C
      4E222224A55020398DFCD3E3140091DF30479E638728EA763BE51617730134BB
      4D264F4179B9D0A3EEA7D7C7BE209FD51AFC96258AE46610272E9E0050C0E3F1
      520B9FCFEBAEAA1A0C0402F0BD9E82B388074B3AC19846984B25FCBB33114EED
      20DC6E3742996A3362D1BF98A96C2341B92F17965733B0DBEDE8BE7061BC5128
      2C2400D4CCE5667457550DF97C3E785FE9613EC0C37C2AE14D0A617A2B612A37
      1176CDC00AB42F8E20DBF41E3AD62814162E75C5125E59A9F67ABD70CFE831B7
      8F87996D84C964C2E816C2687622B419B118887F87AA96A17F9F3F3F5A9F99B9
      6F45BB2DB5914090D95D59A9F6783C704D4D6046CAC3D816822E91A04EA025B4
      77355A5D3D7283CF972EB7DE6BFC9B0241667775B5C6ED76C3A91FC7D8AE3468
      1651E522AADC9B03EBBB4C75D7D3D30B563B61ABC7745D7878426474740400F2
      CFCD92CF6C263F2858FDC5B777FE2D790D6F8393CBB2405858E0BD795FFE9796
      9C9CC2DE2B57C6188681F59F1718E6C560309EA0DC4C906F22F46C24BCF894D0
      1943E8E225624EA980D56A45674585AE9ECFDFF9C1AB68CECB3B28AFAD1D0FA1
      3A7E10552DA2AADD5990E771D1194378FA09A12D9AD09E9688D93E392C160B9E
      9D39A3BBBE7DBB7805DC2A9114C96B6B27188681ED0368BF5404CB941EC6973A
      74E770D1164D781845B8BF9EF0202501AFE57298CD66FC75FAF4F0B5B4B43C00
      44B7A4D2038AAB572797A3438BA822EE1D6AB7DB61B7DB6118D1A143C4C5FDF5
      843FD6127E8D24DC4B4EC0B44C0693C984272525BA9F525345F4E4C4892E8661
      E018D160841F83A1045A9AAAE5E8D3929291CEB367C76D361BE675C3782CE0E2
      D748C2DD08C2ED70C2DDA404BC191981D168C4EF478FB671D670386B8988D840
      800261E1146083D58FC814D1F6BB7F12276E33C92A2A5E4E3D7AF41D0BC480A8
      557CF224577CEF01F57E7B84EC1393140051048743C4B20480381B36ACA5D6FC
      FCC3BD3535D32E970BE63E19FA33E230B02798A9C3E140C7A953BA46A1303B54
      9486ACACFDCF2B2B27CC663366351AFCC64BC72FDB9231AD50C06030A0FBD2A5
      F19F45A22202404D62F121D9E5CB934EA713D6693D6CF3F341B4B474B84924CA
      5ADDFC8D4261E1F3AAAA0993C98439BD1EB363A330180CE8A9A9996C128B0FAD
      68B786ECEC833D172F8E391C0E381C0E7495976B9B737204FFB79A1A048283CF
      ABAB278C466310ADAD9D5ABECAD62C5FA60D42E19EAD7BF79EF5D86C7E9B567B
      F2F8F0F0D8C796698B48B43F4E2239171E15B56E4E2E3FF7BD4AF53814FB0F09
      8F74B2B4C34EBD0000000049454E44AE426082}
    ParentFont = False
    ShineColor = clWhite
    TabOrder = 2
    Version = '1.3.3.0'
    OnClick = btnCancelClick
  end
  object RecipeListComboBox: TAdvComboBox
    Left = 285
    Top = 18
    Width = 236
    Height = 32
    Color = clWindow
    Version = '2.0.0.6'
    Visible = True
    ButtonWidth = 17
    EmptyTextStyle = []
    DropWidth = 0
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ItemIndex = -1
    LabelFont.Charset = DEFAULT_CHARSET
    LabelFont.Color = clWindowText
    LabelFont.Height = -14
    LabelFont.Name = 'Tahoma'
    LabelFont.Style = []
    ParentFont = False
    TabOrder = 3
    Text = 'RecipeListComboBox'
    OnSelect = RecipeListComboBoxSelect
  end
  object ChamberPanel: TAdvPanel
    Left = 8
    Top = 18
    Width = 263
    Height = 30
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    UseDockManager = True
    Version = '2.6.3.2'
    BorderColor = clBlack
    Caption.Color = clHighlight
    Caption.ColorTo = clNone
    Caption.Font.Charset = DEFAULT_CHARSET
    Caption.Font.Color = clWindowText
    Caption.Font.Height = -11
    Caption.Font.Name = 'Tahoma'
    Caption.Font.Style = []
    Caption.GradientDirection = gdVertical
    Caption.Indent = 0
    Caption.ShadeLight = 255
    CollapsColor = clNone
    CollapsDelay = 0
    DoubleBuffered = True
    ShadowColor = clBlack
    ShadowOffset = 0
    StatusBar.BorderColor = clNone
    StatusBar.BorderStyle = bsSingle
    StatusBar.Font.Charset = DEFAULT_CHARSET
    StatusBar.Font.Color = clWindowText
    StatusBar.Font.Height = -11
    StatusBar.Font.Name = 'Tahoma'
    StatusBar.Font.Style = []
    StatusBar.Color = clWhite
    StatusBar.GradientDirection = gdVertical
    Text = 'Recipe'
    FullHeight = 30
  end
end
