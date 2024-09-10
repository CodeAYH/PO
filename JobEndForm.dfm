object FmJobEnd: TFmJobEnd
  Left = 491
  Top = 139
  BorderStyle = bsDialog
  Caption = 'Job End'
  ClientHeight = 391
  ClientWidth = 507
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = #47569#51008' '#44256#46357
  Font.Style = [fsBold]
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 17
  object lblChamber: TEllipsLabel
    Left = 8
    Top = 8
    Width = 484
    Height = 49
    Alignment = taCenter
    AutoSize = False
    Caption = ' Cure Done'
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
  object JobInfoGrid: TAdvStringGrid
    Left = 8
    Top = 55
    Width = 484
    Height = 243
    ColCount = 2
    DefaultColWidth = 130
    DefaultRowHeight = 38
    DrawingStyle = gdsClassic
    Enabled = False
    RowCount = 6
    FixedRows = 0
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = #47569#51008' '#44256#46357
    Font.Style = [fsBold]
    ParentFont = False
    ScrollBars = ssNone
    TabOrder = 0
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
    ControlLook.DropDownFooter.Font.Name = 'MS Sans Serif'
    ControlLook.DropDownFooter.Font.Style = []
    ControlLook.DropDownFooter.Visible = True
    ControlLook.DropDownFooter.Buttons = <>
    DisabledFontColor = clNavy
    Filter = <>
    FilterDropDown.Font.Charset = DEFAULT_CHARSET
    FilterDropDown.Font.Color = clWindowText
    FilterDropDown.Font.Height = -11
    FilterDropDown.Font.Name = 'MS Sans Serif'
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
    FixedColWidth = 130
    FixedRowHeight = 38
    FixedFont.Charset = ANSI_CHARSET
    FixedFont.Color = clBlack
    FixedFont.Height = -21
    FixedFont.Name = #47569#51008' '#44256#46357
    FixedFont.Style = [fsBold]
    FloatFormat = '%.2f'
    HoverButtons.Buttons = <>
    HTMLSettings.ImageFolder = 'images'
    HTMLSettings.ImageBaseName = 'img'
    PrintSettings.DateFormat = 'dd/mm/yyyy'
    PrintSettings.Font.Charset = DEFAULT_CHARSET
    PrintSettings.Font.Color = clWindowText
    PrintSettings.Font.Height = -11
    PrintSettings.Font.Name = 'MS Sans Serif'
    PrintSettings.Font.Style = []
    PrintSettings.FixedFont.Charset = DEFAULT_CHARSET
    PrintSettings.FixedFont.Color = clWindowText
    PrintSettings.FixedFont.Height = -11
    PrintSettings.FixedFont.Name = 'MS Sans Serif'
    PrintSettings.FixedFont.Style = []
    PrintSettings.HeaderFont.Charset = DEFAULT_CHARSET
    PrintSettings.HeaderFont.Color = clWindowText
    PrintSettings.HeaderFont.Height = -11
    PrintSettings.HeaderFont.Name = 'MS Sans Serif'
    PrintSettings.HeaderFont.Style = []
    PrintSettings.FooterFont.Charset = DEFAULT_CHARSET
    PrintSettings.FooterFont.Color = clWindowText
    PrintSettings.FooterFont.Height = -11
    PrintSettings.FooterFont.Name = 'MS Sans Serif'
    PrintSettings.FooterFont.Style = []
    PrintSettings.PageNumSep = '/'
    RowHeaders.Strings = (
      'Recipe'
      'Cure Temp.'
      'Cure Time'
      'User ID.'
      'Start Time'
      'End Time')
    SearchFooter.Color = clBtnFace
    SearchFooter.ColorTo = clNone
    SearchFooter.FindNextCaption = 'Find &next'
    SearchFooter.FindPrevCaption = 'Find &previous'
    SearchFooter.Font.Charset = DEFAULT_CHARSET
    SearchFooter.Font.Color = clWindowText
    SearchFooter.Font.Height = -11
    SearchFooter.Font.Name = 'MS Sans Serif'
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
    Version = '8.7.2.4'
  end
  object btnOk: TAdvGlassButton
    Left = 140
    Top = 320
    Width = 250
    Height = 57
    BackColor = 12972990
    Caption = 'Ok'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = #47569#51008' '#44256#46357
    Font.Style = [fsBold]
    ForeColor = clWhite
    GlowColor = 16760205
    ImageIndex = -1
    InnerBorderColor = clBlack
    OuterBorderColor = clWhite
    Picture.Data = {
      89504E470D0A1A0A0000000D49484452000000200000001F08060000008656CF
      8C0000000473424954080808087C086488000000097048597300000DD700000D
      D70142289B780000001974455874536F667477617265007777772E696E6B7363
      6170652E6F72679BEE3C1A000005E8494441544889B5567B54CC7914BF934709
      A387A308A5A432A9E8A42D4B6D0F52722A2AB684D49698F4922C2DEDB61ED1C6
      7AED5ACBA13DEC6265A3635B4A5BA445D63825327AE8814A35D5D44CF3F8EC1F
      EDCC3167DA1EABEE39DF737EE77CEEEFDECFE7FE3EDF3BC30040C3157AABADFD
      A7E9E825F2059D8D2595651B915DF74C2909C0B01CA60FCB26FCF0B646A9548A
      96761E42D2B6D618072F3C434BB598EFE70D4B732262CC8D5852DC29E80200F0
      BBF9785CC7C1D3572F24CEF1FEE5D3036DB6101163D808CC58F711FBE29FD7F8
      0020140B61BADF128CB8316067C480DFCDC7D5C2EC8E79114B8A357C595643AF
      7EA916D329DE9F8B7F634F4E0A28564D7EFCCE05000004DD425886BB96A90CB5
      F1CCF4E61C3BBA29D99088A8A6B596926FEE53C035D53589884875D468D218CB
      140D2981F1CB669A3B5AD8B9984D9FC920228AC98CA74E51A71C67AA31296971
      2211116517E7095F37375C18D2F15B84B914F1F86D00805BE5B90AA3A75835A4
      E51F0100748B4498CF5EF6987693CA904D403FD0766DA457F06CA6FA78124944
      C4BE1AA3809BEBB268F3827022224AB978FC5DE5DBEA48EC8274688CE73945DD
      3EDAAB5C2A9502000EDC4E53529FC7CD0700BC6E6E806598CBF521DD03261B16
      7D575CFE44020075BC7A8CFB7CA242F3D53F05C92E05FCBFDE5845EE06BAB277
      3FF813305CF50D6D675979CE339EA3424414772D813A841D727C9CEA383AE8D9
      7313EE943E10712A9E9E4756E51B79C287AA9F13E69CDFC46B0600E471F39546
      9F72FB1B0080442A817DD4F21272A491EFBFFF4113981660EDB3CED5CF429BA9
      4962A9983667442BE0663AA614B5703311111DF9ED744B7553DD36DC865821A9
      374391B5F5A87E8DE7CB1A6DC3767F269688010087F28F28A9BF559E0B0068E2
      35C332DCF5566F751426306185B98D8389DD336B3B9D52FD35B6417DA99F355E
      73EFC1D02F8C46A88CA0B7ED0DB42B3B59015F69E14DCEC69F101151ECC92F6B
      39DCD2E05E0BC91539D248EB4D6E1C41B71012A904DF67A5775884B9FCADE9C3
      5AA0A4DE69AA9E4F52488DCCD94117362828574FD0C2AB961EF8E10B8ED834D8
      E1F07F4E52F6601CBC30258F734F88F7822FE8C4F61FF7BC33FFCCF926B9E91A
      C87259A14E7FD435BD0100DCADBC0746DC1805027B7252000052A9148B627DCA
      C8D140AD4F02AAEE86B3820E6CA905807DB907A1953819EC8C183477B600E859
      1E6BF647D69B85389C657A9BAD4A4A4F6D0600B1440CABD4F90ACD8DF79A4328
      EED171EAC679DEB435B6ABFAF4121131E684BBDC6FED6843615511466C1D2B2F
      A69D3805270A4F4266B492AAE7D89D9EDA2C147503008EDE39A164BC1B65D900
      001EBF0D561B9714F46B66A3B5F6F1970AAEF345121166EE652915A4583558A6
      CE97AF52593476344173A7AE429ED7195F391E71647B9DAADB0C93FE08A84CD4
      D00E5AF9B187BA4024209E80D7AB5139F54FC8F1C462F24B0FA057AD35444494
      90B5935ABA5AE53963468DA1B4E5078888A8AC862BBDCDB9774370A3E2795F37
      898848A5B5BDED8A6F725835AFBD831E4615D2A773FD89C160F49A7C8973854C
      F75BD2C65F23E9F483B30A5882531C1968E9131111FBD84E6E19B734AABFE644
      FFFE3164B84F9D6A3A79E6094F3B579B5D81D13A250DA51493194F85554503A9
      4186DA33A874EB23521BA946BFE465F2B79FDE135371AEE8E48009C842DDDB64
      BED1A46947D95EEB4D42DC56332F3FC9A084AC9D54D95CD56791CCE0CBE439DB
      83BA84025A10ED75FFD1F1DF6D07C49CA8F71FA3C9FEF3026CD81E65398FEF0A
      052201F6E7A682B96352AF06F538E52D375EDC0F5FBD1EEB656CD59FF17A5D44
      4A802F6BB4D15AFB64B71D8115E5B51568686F44F865B6C23555DD3601DCA697
      008097F5D560853A5E184CF33E09C8139CF5B44D37385C083D145FFFAEAD05A5
      6F9EC2FD941726ECD0C1B705C7E4EADD77ACE192A381C69013909DD11EFA66AC
      50A7FC7D3F1F6DE916891476C2F5BF723A8DD6DAC50DB6F9A008C88EAE9F95FB
      DC882525970BB2BA809E8D671DE1F6887693CAFF21A0700B061A8C24868A6185
      7D94AED6A4F5C26E81A8B4A66C45576655E5A00B11D13FA6795F465FCFF2A700
      00000049454E44AE426082}
    ParentFont = False
    ShineColor = clWhite
    TabOrder = 1
    Version = '1.3.3.1'
    OnClick = btnOkClick
  end
  object BuzzerOffTime: TTimer
    Enabled = False
    Interval = 10000
    OnTimer = BuzzerOffTimeTimer
    Left = 440
    Top = 328
  end
end
