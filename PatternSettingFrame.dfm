object FrmPatternSetting: TFrmPatternSetting
  Left = 0
  Top = 0
  Width = 1119
  Height = 889
  TabOrder = 0
  object EditorPanel: TAdvPanel
    Left = 1
    Top = 191
    Width = 768
    Height = 478
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    UseDockManager = True
    OnClick = EditorPanelClick
    Version = '2.6.3.4'
    BorderColor = clBlack
    Caption.Color = clHighlight
    Caption.ColorTo = clNone
    Caption.CloseColor = clTeal
    Caption.CloseButtonColor = clBlack
    Caption.Font.Charset = DEFAULT_CHARSET
    Caption.Font.Color = clWindowText
    Caption.Font.Height = -11
    Caption.Font.Name = 'Tahoma'
    Caption.Font.Style = []
    Caption.Indent = 0
    Caption.Text = 
      '<FONT  size="10" face="Tahoma" color="#FFFFFF"><P align="center"' +
      '><B>2. Edit Pattern Informations</B></P></FONT>'
    Caption.Visible = True
    DoubleBuffered = True
    StatusBar.Font.Charset = DEFAULT_CHARSET
    StatusBar.Font.Color = clWindowText
    StatusBar.Font.Height = -11
    StatusBar.Font.Name = 'Tahoma'
    StatusBar.Font.Style = []
    Text = ''
    FullHeight = 485
    object EventInfoGrid: TAdvStringGrid
      Left = 324
      Top = 256
      Width = 444
      Height = 222
      ColCount = 4
      DoubleBuffered = True
      DrawingStyle = gdsClassic
      FixedCols = 0
      RowCount = 4
      FixedRows = 2
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
      ParentDoubleBuffered = False
      ParentFont = False
      TabOrder = 0
      OnDrawCell = EventInfoGridDrawCell
      GridLineColor = 15855083
      GridFixedLineColor = 13745060
      OnGetAlignment = EventInfoGridGetAlignment
      OnClickCell = EventInfoGridClickCell
      HintColor = clBtnHighlight
      ActiveCellFont.Charset = DEFAULT_CHARSET
      ActiveCellFont.Color = clWindowText
      ActiveCellFont.Height = -11
      ActiveCellFont.Name = 'Tahoma'
      ActiveCellFont.Style = [fsBold]
      ActiveCellColor = 10344697
      ActiveCellColorTo = 6210033
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
      MouseActions.RowSelect = True
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
      ShowSelection = False
      UIStyle = tsOffice2007Luna
      VAlignment = vtaCenter
      Version = '8.7.2.6'
    end
    object SegNoPanel: TAdvPanel
      Left = 0
      Top = 20
      Width = 769
      Height = 36
      Color = clInfoBk
      TabOrder = 1
      UseDockManager = True
      Version = '2.6.3.4'
      BorderColor = clBlack
      Caption.Color = clHighlight
      Caption.ColorTo = clNone
      Caption.Font.Charset = DEFAULT_CHARSET
      Caption.Font.Color = clWindowText
      Caption.Font.Height = -11
      Caption.Font.Name = 'Tahoma'
      Caption.Font.Style = []
      Caption.Indent = 0
      DoubleBuffered = True
      StatusBar.Font.Charset = DEFAULT_CHARSET
      StatusBar.Font.Color = clWindowText
      StatusBar.Font.Height = -11
      StatusBar.Font.Name = 'Tahoma'
      StatusBar.Font.Style = []
      Text = 
        '<P align="center"><B><FONT size="12" face="Tahoma">Not Loaded</F' +
        'ONT></B></P>'
      TextVAlign = tvaCenter
      FullHeight = 200
    end
    object SegInfoGrid: TAdvStringGrid
      Left = -1
      Top = 256
      Width = 325
      Height = 222
      ColCount = 4
      DoubleBuffered = True
      DrawingStyle = gdsClassic
      FixedCols = 0
      RowCount = 6
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentDoubleBuffered = False
      ParentFont = False
      TabOrder = 4
      GridLineColor = 15855083
      GridFixedLineColor = 13745060
      OnGetAlignment = SegInfoGridGetAlignment
      HintColor = clBtnHighlight
      ActiveCellFont.Charset = DEFAULT_CHARSET
      ActiveCellFont.Color = clWindowText
      ActiveCellFont.Height = -11
      ActiveCellFont.Name = 'Tahoma'
      ActiveCellFont.Style = [fsBold]
      ActiveCellColor = 10344697
      ActiveCellColorTo = 6210033
      Bands.SecondaryColor = clCream
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
      FixedRowHeight = 22
      FixedFont.Charset = DEFAULT_CHARSET
      FixedFont.Color = clBlack
      FixedFont.Height = -11
      FixedFont.Name = 'Tahoma'
      FixedFont.Style = [fsBold]
      FloatFormat = '%.1f'
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
      ScrollWidth = 21
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
      ShowSelection = False
      UIStyle = tsOffice2007Luna
      VAlignment = vtaCenter
      Version = '8.7.2.6'
    end
    object PrevSegButton: TAdvGlassButton
      Left = 1
      Top = 19
      Width = 72
      Height = 36
      BackColor = clCream
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clWhite
      GlowColor = 16760205
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clWhite
      Picture.Data = {
        89504E470D0A1A0A0000000D4948445200000020000000200806000000737A7A
        F40000000467414D410000B18E7CFB5193000000206348524D0000870F00008C
        0F0000FD520000814000007D790000E98B00003CE5000019CC733C857700000A
        2F694343504943432050726F66696C65000048C79D96775454D71687CFBD777A
        A1CD30D2197A932E3080F42E201D045118660618CA00C30C4D6C88A840441111
        014590A08001A3A148AC88622128A8600F48105062308AA8A86446D64A7C7979
        EFE5E5F7C7BDDFDA67EF73F7D97B9FB52E00244F1F2E2F059602209927E0077A
        38D3578547D0B1FD0006788001A6003059E9A9BE41EEC140242F37177ABAC809
        FC8BDE0C0148FCBE65E8E94FA783FF4FD2AC54BE0000C85FC4E66C4E3A4BC4F9
        224ECA14A48AED3322A6C6248A194689992F4A50C472628E5BE4A59F7D16D951
        CCEC641E5BC4E29C53D9C96C31F788787B86902362C447C405195C4EA6886F8B
        58334998CC15F15B716C3287990E008A24B60B38AC78119B8898C40F0E7411F1
        720070A4B82F38E60B1670B204E243B9A4A466F3B971F102BA2E4B8F6E6A6DCD
        A07B723293380281A13F9395C8E4B3E92E29C9A94C5E36008B67FE2C19716DE9
        A2225B9A5A5B5A1A9A19997E51A8FFBAF83725EEED22BD0AF8DC3388D6F787ED
        AFFC52EA0060CC8A6AB3EB0F5BCC7E003AB6022077FF0F9BE6210024457D6BBF
        F1C57968E279891708526D8C8D3333338DB81C9691B8A0BFEB7F3AFC0D7DF13D
        23F176BF9787EECA89650A93047471DD58294929423E3D3D95C9E2D00DFF3CC4
        FF38F0AFF3581AC889E5F0393C5144A868CAB8BC3851BB796CAE809BC2A37379
        FFA989FF30EC4F5A9C6B9128F59F0035CA0848DDA002E4E73E80A21001127950
        DCF5DFFBE6830F05E29B17A63AB138F79F05FDFBAE7089F891CE8DFB1CE71218
        4C6709F9198B6BE26B09D08000240115C80315A001748121300356C016380237
        B002F88160100ED602168807C9800F32412ED80C0A4011D805F6824A5003EA41
        236801274007380D2E80CBE03AB809EE800760048C83E76006BC01F310046121
        324481E42155480B3280CC2006640FB9413E5020140E454371100F1242B9D016
        A8082A852AA15AA811FA163A055D80AE4203D03D68149A827E85DEC3084C82A9
        B032AC0D1BC30CD809F68683E135701C9C06E7C0F9F04EB802AE838FC1EDF005
        F83A7C071E819FC3B3084088080D51430C1106E282F82111482CC24736208548
        395287B4205D482F720B1941A69177280C8A82A2A30C51B6284F54088A854A43
        6D4015A32A514751EDA81ED42DD4286A06F5094D462BA10DD036682FF42A741C
        3A135D802E4737A0DBD097D077D0E3E837180C8686D1C158613C31E19804CC3A
        4C31E600A615731E338019C3CC62B15879AC01D60EEB87656205D802EC7EEC31
        EC39EC20761CFB1647C4A9E2CC70EEB8081C0F97872BC735E1CEE2067113B879
        BC145E0B6F83F7C3B3F1D9F8127C3DBE0B7F033F8E9F27481374087684604202
        6133A182D042B844784878452412D589D6C4002297B88958413C4EBC421C25BE
        23C990F4492EA4489290B4937484749E748FF48A4C266B931DC91164017927B9
        917C91FC98FC5682226124E125C196D8285125D12E3128F142122FA925E924B9
        563247B25CF2A4E40DC96929BC94B6948B14536A835495D429A961A959698AB4
        A9B49F74B274B17493F455E94919AC8CB68C9B0C5B265FE6B0CC4599310A42D1
        A0B85058942D947ACA25CA381543D5A17A5113A845D46FA8FDD4195919D965B2
        A1B259B255B267644768084D9BE6454BA295D04ED08668EF97282F715AC259B2
        6349CB92C12573728A728E721CB942B956B93B72EFE5E9F26EF289F2BBE53BE4
        1F29A014F415021432150E2A5C529856A42ADA2AB2140B154F28DE578295F495
        0295D6291D56EA539A555651F6504E55DEAF7C51795A85A6E2A892A052A67256
        654A95A26AAFCA552D533DA7FA8C2E4B77A227D12BE83DF4193525354F35A15A
        AD5ABFDABCBA8E7A887A9E7AABFA230D8206432356A34CA35B63465355D35733
        57B359F3BE165E8BA115AFB54FAB576B4E5B473B4C7B9B7687F6A48E9C8E974E
        8E4EB3CE435DB2AE836E9A6E9DEE6D3D8C1E432F51EF80DE4D7D58DF423F5EBF
        4AFF86016C6069C035386030B014BDD47A296F69DDD2614392A193618661B3E1
        A811CDC8C728CFA8C3E885B1A67184F16EE35EE34F2616264926F5260F4C654C
        5798E6997699FE6AA66FC632AB32BB6D4E367737DF68DE69FE7299C132CEB283
        CBEE5A502C7C2DB659745B7CB4B4B2E45BB6584E59695A455B555B0D33A80C7F
        4631E38A35DADAD97AA3F569EB77369636029B1336BFD81ADA26DA36D94E2ED7
        59CE595EBF7CCC4EDD8E69576B37624FB78FB63F643FE2A0E6C074A87378E2A8
        E1C8766C709C70D2734A703AE6F4C2D9C499EFDCE63CE762E3B2DEE5BC2BE2EA
        E15AE8DAEF26E316E256E9F6D85DDD3DCEBDD97DC6C3C2639DC7794FB4A7B7E7
        6ECF612F652F9657A3D7CC0AAB15EB57F47893BC83BC2BBD9FF8E8FBF07DBA7C
        61DF15BE7B7C1FAED45AC95BD9E107FCBCFCF6F83DF2D7F14FF3FF3E0013E01F
        5015F034D0343037B03788121415D414F426D839B824F841886E8830A43B5432
        3432B431742ECC35AC346C6495F1AAF5ABAE872B8473C33B23B011A1110D11B3
        ABDD56EF5D3D1E6911591039B446674DD69AAB6B15D626AD3D132519C58C3A19
        8D8E0E8B6E8AFEC0F463D6316763BC62AA6366582EAC7DACE76C4776197B8A63
        C729E54CC4DAC596C64EC6D9C5ED899B8A77882F8F9FE6BA702BB92F133C136A
        12E612FD128F242E248525B526E392A3934FF1647889BC9E149594AC94815483
        D482D491349BB4BD69337C6F7E433A94BE26BD534015FD4CF50975855B85A319
        F61955196F3343334F664967F1B2FAB2F5B377644FE4B8E77CBD0EB58EB5AE3B
        572D7773EEE87AA7F5B51BA00D311BBA376A6CCCDF38BEC963D3D1CD84CD899B
        7FC833C92BCD7BBD256C4B57BE72FEA6FCB1AD1E5B9B0B240AF805C3DB6CB7D5
        6C476DE76EEFDF61BE63FF8E4F85ECC26B452645E5451F8A59C5D7BE32FDAAE2
        AB859DB13BFB4B2C4B0EEEC2ECE2ED1ADAEDB0FB68A974694EE9D81EDF3DED65
        F4B2C2B2D77BA3F65E2D5F565EB38FB04FB86FA4C2A7A273BFE6FE5DFB3F54C6
        57DEA972AE6AAD56AADE513D77807D60F0A0E3C1961AE59AA29AF787B887EED6
        7AD4B6D769D7951FC61CCE38FCB43EB4BEF76BC6D78D0D0A0D450D1F8FF08E8C
        1C0D3CDAD368D5D8D8A4D454D20C370B9BA78E451EBBF98DEB379D2D862DB5AD
        B4D6A2E3E0B8F0F8B36FA3BF1D3AE17DA2FB24E364CB775ADF55B751DA0ADBA1
        F6ECF6998EF88E91CEF0CE81532B4E7577D976B57D6FF4FD91D36AA7ABCEC89E
        29394B389B7F76E15CCEB9D9F3A9E7A72FC45D18EB8EEA7E7071D5C5DB3D013D
        FD97BC2F5DB9EC7EF962AF53EFB92B76574E5FB5B97AEA1AE35AC775CBEBED7D
        167D6D3F58FCD0D66FD9DF7EC3EA46E74DEB9B5D03CB07CE0E3A0C5EB8E57AEB
        F26DAFDBD7EFACBC333014327477387278E42EFBEEE4BDA47B2FEF67DC9F7FB0
        E921FA61E123A947E58F951ED7FDA8F763EB88E5C89951D7D1BE27414F1E8CB1
        C69EFF94FED387F1FCA7E4A7E513AA138D936693A7A7DCA76E3E5BFD6CFC79EA
        F3F9E9829FA57FAE7EA1FBE2BB5F1C7FE99B593533FE92FF72E1D7E257F2AF8E
        BC5EF6BA7BD67FF6F19BE437F373856FE5DF1E7DC778D7FB3EECFDC47CE607EC
        878A8F7A1FBB3E797F7AB890BCB0F01BF784F3FB3704291E0000000970485973
        00000EC300000EC301C76FA8640000045C494441545847C5575D689B55187EBF
        343F4D5BB7B673B51D635E8CC9108673B0B1C9D0CF0B2FBCF142C52B2F2A4CB1
        5BBB0EC7B07752A83FACAE6C66505C6B5B6128CCEDC61F544414FF060565C260
        2052D7AE2C69D6A6B5A64DCC97E6F3794ECED77D49937649207EF0F6A4E77BCF
        799EF779DFF39EC4B06D5BFECFC75309B8F154FB77B44AF6289B0081BB4263A6
        04FDB55527E080477D72DE686E6EA92A0107FCE65CE4929191CEB225D4AC4B5A
        AFC0DF1B35FF9A8F7C6965EC172A89DC597BCF041CF0E9B9F04F99A4FDB4A444
        BC19919A0A59DC1301827722F2DBB1C8B5E4B271389D14C98040265D050259F0
        11333C17F923B9247BD3005EB100BE922580A1A2C758AF1129F0732366341699
        4A2C1B3B5200B700CEDEE581F64DDBDAE44AA867C20E47A64A65617F33F624D7
        142540F063673F3063F333D144C268F9979123E2348C4F0D92E7F589DCD7DA36
        1E0CC8351F3E734E0C902BC4464FD6611C78BDFB60E6D2B9478A12C8820F9B0B
        73D18544CA6864E404B6604EE73600E4850A5482630D8C73B4FC87737CEFF58A
        B46E6F93A1DE9E89C5D17776D26F0D59053E3064FE3D7F672969198D6948BEC2
        7CA3E215636D5C483236E6D378EFA4C7D26962AA568D730CC2499F8B610E0182
        1F3DF3BEB9B418B5506CF5195D6C06738E455E46A2CDE3488D77684860783745
        6A9DDB1800CC861950D12DD22A018277BC3B685AF1D84ADAF2F8986F15B5BE2C
        3DF054797724C76745C2B59BA30E97E498FE8781E467481150E0FD836620B540
        B96BD68033B7F4D339CE195D697108141AA91CE709E8965D9D02FFF35DBFBDDA
        177A747B834878596429219264EE29AB4B85528FDAAA3F90FD50AED62F723F30
        86FB7A26A2EE224C5D0EEDFBE4ED537F4EC7E1101009E048F958D9CC37A567B5
        53F2328D7B386BB9A73B0FAB6A843FECDFF551DF6B376610FDB67A91064DC48F
        A3E3D5B957645CE6D40347125606FF4246057C58EBD3A970D4C93905B317071E
        BED87BE27A18247640AA4DF8AA518B0D9DC524A20A511322A092163ECAA05C10
        632173BF73A7724D1F887D7C76CFC81BC77F9F442D3C08129B41C2CF945009DD
        70D87C1438E6EAA154539DC816F8B640B942F6809EDF4A3FF8BB6FD0A2ADB8EE
        B9CE5F8FBC797EDFEE4D22B74166918D84E759D3E7F163A42D20B8552B857FB3
        9BE79D35D543F027C0357078B1BB67627224DB09D7BD8C7CCF768C77BC35B87F
        6FA3C82D4DC2D27D81C5B419553DD6DF3B9B5A8EC7F28FD77A2726128F2FA6AE
        0CEEDF90807278E695AB5DA72F1C3CD0243289DAF887DF03D81981D80C0267D0
        D7677434E51CD38217977B23FBD30B8742A75EBE3A3E2FB21379DE02B9830066
        D105790396835AEC2E28B697FDD9D063A193477EFE6556E42116140AA98115CF
        53510D02C4B0BF183E1C3AF1D28FDFDE11D9856A6E05897AA850E0F62D89D286
        29C849C757A38F87BADB7FF83C823E111469840A953E2511504A7C3DF604495C
        9E860AD5AA81FC281589AEF6EF47A7B2ADB59267DD3EB0D1C6EA87297E1BF2A4
        6CE45BEC7D4504CA0575AFFB0F44C0AC4F049A4E960000000049454E44AE4260
        82}
      ParentFont = False
      ShineColor = clWhite
      TabOrder = 2
      Version = '1.3.3.1'
      OnClick = PrevSegButtonClick
    end
    object NextSegButton: TAdvGlassButton
      Left = 694
      Top = 19
      Width = 72
      Height = 36
      BackColor = clCream
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clWhite
      GlowColor = 16760205
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clWhite
      Picture.Data = {
        89504E470D0A1A0A0000000D4948445200000020000000200806000000737A7A
        F400000A41694343504943432050726F66696C65000078019D96775453D91687
        CFBD37BDD012222025F41A7A0920D23B48150451894980500286842676440546
        141129566454C0014787226345140B838262D709F21050C6C1514445E5DD8C6B
        09EFAD35F3DE9AFDC759DFD9E7B7D7D967EF7DD7BA0050FC8204C27458018034
        A15814EEEBC15C1213CBC4F7021810010E5801C0E166660447F84402D4FCBD3D
        9999A848C6B3F6EE2E8064BBDB2CBF502673D6FF7F912237432406000A45D536
        3C7E2617E5029453B3C51932FF04CAF495293286313216A109A2AC22E3C4AF6C
        F6A7E62BBBC9989726E4A11A59CE19BC349E8CBB50DE9A25E1A38C04A15C9825
        E067A37C0765BD54499A00E5F728D3D3F89C4C003014995FCCE726A16C893245
        1419EE89F202000894C439BC720E8BF939689E0078A667E48A04894962A611D7
        9869E5E8C866FAF1B353F962312B94C34DE188784CCFF4B40C8E301780AF6F96
        450125596D996891EDAD1CEDED59D6E668F9BFD9DF1E7E53FD3DC87AFB55F126
        ECCF9E418C9E59DF6CECAC2FBD1600F6245A9B1DB3BE955500B46D0640E5E1AC
        4FEF2000F20500B4DE9CF31E866C5E92C4E20C270B8BECEC6C73019F6B2E2BE8
        37FB9F826FCABF8639F799CBEEFB563BA6173F81234915336545E5A6A7A64B44
        CCCC0C0E97CF64FDF710FFE3C03969CDC9C32C9C9FC017F185E85551E8940984
        8968BB853C8158902E640A847FD5E17F18362707197E9D6B1468755F007D8539
        50B84907C86F3D00432303246E3F7A027DEB5B10310AC8BEBC68AD91AF738F32
        7AFEE7FA1F0B5C8A6EE14C412253E6F60C8F647225A22C19A3DF846CC1021290
        0774A00A34812E30022C600D1C80337003DE2000848048100396032E48026940
        04B2413ED8000A4131D80176836A7000D4817AD0044E823670065C0457C00D70
        0B0C8047400A86C14B3001DE81690882F01015A241AA9016A40F9942D6101B5A
        0879434150381403C5438990109240F9D026A8182A83AAA143503DF423741ABA
        085D83FAA007D0203406FD017D84119802D3610DD800B680D9B03B1C0847C2CB
        E04478159C0717C0DBE14AB8163E0EB7C217E11BF0002C855FC2930840C80803
        D14658081BF144429058240111216B9122A402A9459A900EA41BB98D489171E4
        030687A161981816C619E387598CE1625661D6624A30D5986398564C17E63666
        103381F982A562D5B1A65827AC3F760936119B8D2DC456608F605BB097B103D8
        61EC3B1C0EC7C019E21C707EB8185C326E35AE04B70FD78CBB80EBC30DE126F1
        78BC2ADE14EF820FC173F0627C21BE0A7F1C7F1EDF8F1FC6BF2790095A046B82
        0F219620246C2454101A08E708FD8411C2345181A84F7422861079C45C6229B1
        8ED841BC491C264E93144986241752242999B48154496A225D263D26BD2193C9
        3A6447721859405E4FAE249F205F250F923F50942826144F4A1C4542D94E394A
        B94079407943A5520DA86ED458AA98BA9D5A4FBD447D4A7D2F47933397F397E3
        C9AD93AB916B95EB977B254F94D79777975F2E9F275F217F4AFEA6FCB80251C1
        40C15381A3B056A146E1B4C23D8549459AA2956288629A62896283E235C55125
        BC928192B7124FA940E9B0D225A5211A42D3A579D2B8B44DB43ADA65DA301D47
        37A4FBD393E9C5F41FE8BDF4096525655BE528E51CE51AE5B3CA5206C23060F8
        335219A58C938CBB8C8FF334E6B9CFE3CFDB36AF695EFFBC2995F92A6E2A7C95
        2295669501958FAA4C556FD514D59DAA6DAA4FD4306A266A616AD96AFBD52EAB
        8DCFA7CF779ECF9D5F34FFE4FC87EAB0BA897AB8FA6AF5C3EA3DEA931A9A1ABE
        1A191A551A9734C635199A6E9AC99AE59AE734C7B4685A0BB5045AE55AE7B55E
        309599EECC546625B38B39A1ADAEEDA72DD13EA4DDAB3DAD63A8B35867A34EB3
        CE135D922E5B3741B75CB75377424F4B2F582F5FAF51EFA13E519FAD9FA4BF47
        BF5B7FCAC0D020DA608B419BC1A8A18AA1BF619E61A3E16323AA91ABD12AA35A
        A33BC63863B6718AF13EE35B26B0899D4992498DC94D53D8D4DE5460BACFB4CF
        0C6BE6682634AB35BBC7A2B0DC5959AC46D6A039C33CC87CA3799BF92B0B3D8B
        588B9D16DD165F2CED2C532DEB2C1F59295905586DB4EAB0FAC3DAC49A6B5D63
        7DC7866AE363B3CEA6DDE6B5ADA92DDF76BFED7D3B9A5DB0DD16BB4EBBCFF60E
        F622FB26FB31073D877887BD0EF7D8747628BB847DD511EBE8E1B8CEF18CE307
        277B27B1D349A7DF9D59CE29CE0DCEA30B0C17F017D42D1872D171E1B81C7291
        2E642E8C5F7870A1D455DB95E35AEBFACC4DD78DE776C46DC4DDD83DD9FDB8FB
        2B0F4B0F91478BC794A793E71ACF0B5E8897AF579157AFB792F762EF6AEFA73E
        3A3E893E8D3E13BE76BEAB7D2FF861FD02FD76FADDF3D7F0E7FAD7FB4F043804
        AC09E80AA404460456073E0B320912057504C3C101C1BB821F2FD25F245CD416
        0242FC4376853C09350C5D15FA73182E2C34AC26EC79B855787E7877042D6245
        4443C4BB488FC8D2C8478B8D164B167746C947C545D5474D457B4597454B9758
        2C59B3E4468C5A8C20A63D161F1B157B247672A9F7D2DD4B87E3ECE20AE3EE2E
        335C96B3ECDA72B5E5A9CBCFAE905FC159712A1E1B1F1DDF10FF8913C2A9E54C
        AEF45FB977E504D793BB87FB92E7C62BE78DF15DF865FC91049784B284D14497
        C45D896349AE491549E3024F41B5E075B25FF281E4A9949094A32933A9D1A9CD
        6984B4F8B4D34225618AB02B5D333D27BD2FC334A33043BACA69D5EE5513A240
        D1914C28735966BB988EFE4CF5488C249B2583590BB36AB2DE6747659FCA51CC
        11E6F4E49AE46ECB1DC9F3C9FB7E356635777567BE76FE86FCC135EE6B0EAD85
        D6AE5CDBB94E775DC1BAE1F5BEEB8F6D206D48D9F0CB46CB8D651BDF6E8ADED4
        51A051B0BE6068B3EFE6C642B94251E1BD2DCE5B0E6CC56C156CEDDD66B3AD6A
        DB97225ED1F562CBE28AE24F25DC92EBDF597D57F9DDCCF684EDBDA5F6A5FB77
        E0760877DCDDE9BAF3589962595ED9D0AEE05DADE5CCF2A2F2B7BB57ECBE5661
        5B71600F698F648FB432A8B2BD4AAF6A47D5A7EAA4EA811A8F9AE6BDEA7BB7ED
        9DDAC7DBD7BFDF6D7FD3018D03C5073E1E141CBC7FC8F7506BAD416DC561DCE1
        ACC3CFEBA2EABABF677F5F7F44ED48F191CF478547A5C7C28F75D53BD4D737A8
        379436C28D92C6B1E371C76FFDE0F5437B13ABE95033A3B9F804382139F1E2C7
        F81FEF9E0C3CD9798A7DAAE927FD9FF6B6D05A8A5AA1D6DCD689B6A436697B4C
        7BDFE980D39D1DCE1D2D3F9BFF7CF48CF6999AB3CA674BCF91CE159C9B399F77
        7EF242C685F18B8917873A57743EBAB4E4D29DAEB0AEDECB8197AF5EF1B972A9
        DBBDFBFC5597AB67AE395D3B7D9D7DBDED86FD8DD61EBB9E965FEC7E69E9B5EF
        6DBDE970B3FD96E3AD8EBE057DE7FA5DFB2FDEF6BA7DE58EFF9D1B038B06FAEE
        2EBE7BFF5EDC3DE97DDEFDD107A90F5E3FCC7A38FD68FD63ECE3A2270A4F2A9E
        AA3FADFDD5F8D766A9BDF4ECA0D760CFB388678F86B8432FFF95F9AF4FC305CF
        A9CF2B46B446EA47AD47CF8CF98CDD7AB1F4C5F0CB8C97D3E385BF29FEB6F795
        D1AB9F7E77FBBD6762C9C4F06BD1EB993F4ADEA8BE39FAD6F66DE764E8E4D377
        69EFA6A78ADEABBE3FF681FDA1FB63F4C791E9EC4FF84F959F8D3F777C09FCF2
        78266D66E6DFF784F3FBA5F226220000046A494441545809C5965F685B551CC7
        7FF7E64FB334B469DA658994AA6393090E8B6251E7445151D1F960B78963B076
        6B9582A9652A2808E28332361CDD9A5A98827D1044415F7DF0612AC581E21808
        0A2B7DE9433B3AE7D62D6D9ADEA6D7EFF7F49EB364666D92FBE08193F3E79EF3
        FB7E7EBFF33BF7C6725D57FECF62FB11B75E7AED9CF54CCF593F367C016CDF9C
        486646C69FF003E10BC0C1E9F576882888677B7EAA2712BE008A50BCE488EC6D
        07C4A9F1C7AD3A207C01D0E36B0098CE8BBC98F2209EEBFDB99648F802E0FD59
        5841140A22938B224F6D06C4F017BBAD17FA26AA85F005C023C8AF8AE4108539
        405C5C1079B40D109F7CBECBDAD3FF4B3510FE0120BE8428E49745AE2C894CE5
        44BA5A0071E2B347784D378208EA05E1EE81DF52B158931EAFD7C26961DD148D
        352D220C05D4A2F73E7300B182879D71401C3FF370E8E5815F9DEFC6BA6E67CF
        00A49B9B135F9E3AB6354F8330462314292B9867D8E1B0F2FA32C4E6500B9858
        F500F8CCC146D61D7067E0E3B187A2DD6FFCBEF86DF6C1325BDEC00020923289
        6CBE82B3A4472BA8DA68E946EA10A20801867E1903F6750478A6DC7715737462
        5B4CA4EFA3EC03B157062FE4BE3EDD596A8B7D0340A314BF8C6CA6610713953E
        13CA51FCB0A5906ED9517DCE13C8129947CB48DC191539FCE1E9FB13AF0EFDF1
        CF57C33BB1C494B224CC43987509E1D0FD5B5B3E23A0F69EA08C16BD6524D852
        9491E19AEB38A26924667A93C8C10F86EF6B3B78F44FA38E8E01203D5FAD7AB3
        83CD152B0C1B514F5C1D8107A0203CA065D828003887C8CEE08A6E01C481F74F
        DE9B3EF4CEA48630008C1FCF91E15321F45A3DAEB52D0242E7068109F23740DA
        9113FBDE3BB12DBC37739E106539C0F05D9F9B559E93BE520E68F28D5A0B39C0
        12808BC1008E064AC5064C44D00FC5C599BF31CFE70600DAEADAF1281809B6AC
        1B964A8B204E789B10A8362002500A04A45808C703636F0FFCE8FE30FE246D1B
        000E94A8B74979708B7142B270098BBAA65843311D2DEE63A568A97830B4EA84
        626DA14FDF7ADD88D3860150E2185908970D253546BFB4D814C204F3812D43A6
        AE22060AC65B1CD010B415824858161A9B928DA347FBCBC4B9DC00D03B9207B1
        41912371B4575CA88BF696498634110BE2E8AE01718CAA420E71DA8A84DC6BCD
        2DC9F8E850DF7FC4B1F426406E219F83B133CDE934E795776B9D9BBF84A412B3
        DB4156E70BD279E3D26C1723A6DF84386715458A3744DCB978CB96E4E8D0918A
        E23467E97FC5F5FCAFB3D2A98EEECCB1AD576766651561600443100E87F9A172
        A793895447F6CDC3B715278039029D959CACB634F4BC3B85F35DCB1B8833F41C
        87A37231D99ABA273BB8BE38750C40B5A2A5EB98A3362CD8ECA032FC9146B990
        4EA43AB383BDEB7AAEEDF0B2D45D987C0A005E07F1828944DD893B6A10A7B06F
        80155A00801DB1BE6F6F4D3F3652A5E71467F105B0660289675BDFDCDD927ABE
        5671DF00BC96AE2DD9BB5A53FB47323D559DB986362DAF61BD55F6F49FCBFC05
        034F1F3A5BAF0DF31E30443574F4BBA39E2BAC657C0168237EDA7F01CDB279B3
        037C69F50000000049454E44AE426082}
      ParentFont = False
      ShineColor = clWhite
      TabOrder = 3
      Version = '1.3.3.1'
      OnClick = NextSegButtonClick
    end
    object TempGraph: TChart
      AlignWithMargins = True
      Left = 0
      Top = 56
      Width = 767
      Height = 202
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      Foot.Font.Name = 'Tahoma'
      Foot.Font.Style = [fsBold]
      Legend.Font.Name = 'Tahoma'
      Legend.Font.Style = [fsBold]
      Legend.LegendStyle = lsSeries
      Legend.Title.Font.Name = 'Tahoma'
      SubFoot.Font.Name = 'Tahoma'
      SubFoot.Font.Style = [fsBold]
      SubTitle.Font.Name = 'Tahoma'
      SubTitle.Font.Style = [fsBold]
      Title.Font.Name = 'Tahoma'
      Title.Font.Style = [fsBold]
      Title.Text.Strings = (
        'TChart')
      Title.Visible = False
      BottomAxis.LabelsFormat.Font.Name = 'Tahoma'
      BottomAxis.LabelsFormat.Font.Style = [fsBold]
      BottomAxis.Title.Font.Name = 'Tahoma'
      BottomAxis.Title.Font.Style = [fsBold]
      DepthAxis.LabelsFormat.Font.Name = 'Tahoma'
      DepthAxis.LabelsFormat.Font.Style = [fsBold]
      DepthAxis.Title.Font.Name = 'Tahoma'
      DepthAxis.Title.Font.Style = [fsBold]
      DepthTopAxis.LabelsFormat.Font.Name = 'Tahoma'
      DepthTopAxis.LabelsFormat.Font.Style = [fsBold]
      DepthTopAxis.Title.Font.Name = 'Tahoma'
      DepthTopAxis.Title.Font.Style = [fsBold]
      LeftAxis.LabelsFormat.Font.Name = 'Tahoma'
      LeftAxis.LabelsFormat.Font.Style = [fsBold]
      LeftAxis.Title.Font.Name = 'Tahoma'
      LeftAxis.Title.Font.Style = [fsBold]
      RightAxis.LabelsFormat.Font.Name = 'Tahoma'
      RightAxis.LabelsFormat.Font.Style = [fsBold]
      RightAxis.Title.Font.Name = 'Tahoma'
      RightAxis.Title.Font.Style = [fsBold]
      TopAxis.LabelsFormat.Font.Name = 'Tahoma'
      TopAxis.LabelsFormat.Font.Style = [fsBold]
      TopAxis.Title.Font.Name = 'Tahoma'
      TopAxis.Title.Font.Style = [fsBold]
      TopAxis.Title.Font.Emboss.Visible = True
      View3D = False
      OnBeforeDrawAxes = TempGraphBeforeDrawAxes
      OnBeforeDrawSeries = TempGraphBeforeDrawSeries
      TabOrder = 5
      OnClick = TempGraphClick
      DefaultCanvas = 'TGDIPlusCanvas'
      ColorPaletteIndex = 13
    end
  end
  object PatternEditor: TAdvToolPanel
    Left = 768
    Top = 191
    Width = 239
    Height = 478
    AllowDocking = False
    BackgroundTransparent = False
    BackGroundPosition = bpTopLeft
    BorderWidth = 1
    Button3D = False
    HoverButtonColor = 16576740
    HoverButtonColorTo = 16576740
    DownButtonColor = 16573128
    DownButtonColorTo = 16573128
    CaptionButton = False
    Color = clBtnFace
    ColorTo = clBtnFace
    GradientDirection = gdVertical
    DockDots = False
    CanSize = False
    Caption = 'Pattern Editor'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    Font.Quality = fqClearTypeNatural
    CaptionGradientDirection = gdHorizontal
    FocusCaptionFontColor = clBlack
    FocusCaptionColor = 16573128
    FocusCaptionColorTo = 16573128
    NoFocusCaptionFontColor = clBlack
    NoFocusCaptionColor = 15066597
    NoFocusCaptionColorTo = 15066597
    CloseHint = 'Close panel'
    LockHint = 'Lock panel'
    UnlockHint = 'Unlock panel'
    Sections = <
      item
        Autosize = False
        Caption = 'Pattern Information'
        Draw = True
        SectionItems = <>
        Height = 151
        CaptionImageIndex = -1
      end
      item
        Autosize = False
        Caption = 'Segment Information'
        Draw = True
        SectionItems = <>
        Height = 150
        CaptionImageIndex = -1
      end
      item
        Autosize = False
        Caption = 'Event Information'
        Draw = True
        SectionItems = <>
        Height = 279
        CaptionImageIndex = -1
      end>
    SectionLayout.CaptionColor = clNavy
    SectionLayout.CaptionColorTo = clNavy
    SectionLayout.CaptionFontColor = clWhite
    SectionLayout.CaptionRounded = False
    SectionLayout.Corners = scRectangle
    SectionLayout.BackGroundColor = clWhite
    SectionLayout.BackGroundColorTo = clWhite
    SectionLayout.BorderColor = clBackground
    SectionLayout.BorderWidth = 1
    SectionLayout.CaptionGradientDir = gdVertical
    SectionLayout.BackGroundGradientDir = gdVertical
    SectionLayout.Indent = 0
    SectionLayout.Spacing = 0
    SectionLayout.ItemFontColor = clNavy
    SectionLayout.ItemHoverTextColor = clBlack
    SectionLayout.ItemHoverUnderline = True
    SectionLayout.UnderLineCaption = False
    ShowCaption = False
    ShowCaptionBorder = False
    ShowClose = False
    ShowLock = False
    Style = esCustom
    Version = '1.8.5.0'
    object TimeEdit: TAdvEdit
      Tag = 100
      Left = 135
      Top = 171
      Width = 100
      Height = 23
      Cursor = crIBeam
      EditType = etNumeric
      EmptyTextStyle = []
      MinValue = 1
      MaxValue = 5000
      MinFloatValue = 1.000000000000000000
      MaxFloatValue = 5000.000000000000000000
      LabelCaption = 'Time'
      LabelPosition = lpLeftCenter
      LabelMargin = 8
      LabelTransparent = True
      LabelAlwaysEnabled = True
      LabelFont.Charset = DEFAULT_CHARSET
      LabelFont.Color = clWindowText
      LabelFont.Height = -13
      LabelFont.Name = 'Tahoma'
      LabelFont.Style = []
      Lookup.Font.Charset = DEFAULT_CHARSET
      Lookup.Font.Color = clWindowText
      Lookup.Font.Height = -60
      Lookup.Font.Name = 'Arial'
      Lookup.Font.Style = []
      Lookup.Font.Quality = fqClearTypeNatural
      Lookup.Separator = ';'
      Color = clWindow
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Font.Quality = fqClearTypeNatural
      HideSelection = False
      ImeName = 'Microsoft Office IME 2007'
      ParentFont = False
      TabOrder = 0
      Text = '30'
      Visible = True
      OnEnter = OnEditEnter
      OnExit = OnEditExit
      Version = '4.0.4.2'
    end
    object SPEdit: TAdvEdit
      Tag = 100
      Left = 135
      Top = 196
      Width = 100
      Height = 23
      Cursor = crIBeam
      EditType = etNumeric
      EmptyTextStyle = []
      MaxValue = 350
      MaxFloatValue = 350.000000000000000000
      LabelCaption = 'SP'
      LabelPosition = lpLeftCenter
      LabelMargin = 8
      LabelTransparent = True
      LabelAlwaysEnabled = True
      LabelFont.Charset = DEFAULT_CHARSET
      LabelFont.Color = clWindowText
      LabelFont.Height = -13
      LabelFont.Name = 'Tahoma'
      LabelFont.Style = []
      Lookup.Font.Charset = DEFAULT_CHARSET
      Lookup.Font.Color = clWindowText
      Lookup.Font.Height = -60
      Lookup.Font.Name = 'Arial'
      Lookup.Font.Style = []
      Lookup.Font.Quality = fqClearTypeNatural
      Lookup.Separator = ';'
      Color = clWindow
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Font.Quality = fqClearTypeNatural
      HideSelection = False
      ImeName = 'Microsoft Office IME 2007'
      ParentFont = False
      TabOrder = 1
      Text = '175'
      Visible = True
      OnEnter = OnEditEnter
      OnExit = OnEditExit
      Version = '4.0.4.2'
    end
    object EventTypeComboBox: TAdvComboBox
      Left = 135
      Top = 321
      Width = 100
      Height = 23
      Color = clWindow
      Version = '2.0.0.6'
      Visible = True
      ButtonWidth = 23
      EmptyTextStyle = []
      DropWidth = 0
      Enabled = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Font.Quality = fqClearTypeNatural
      ImeName = 'Microsoft Office IME 2007'
      ItemIndex = -1
      Items.Strings = (
        'PV HIGH'
        'PV LOW'
        'COOLING'
        'PV HIGH CLEAR'
        'PV LOW CLEAR'
        'VAC. VALVE'
        'PURGE'
        'AIR')
      LabelCaption = 'Event Type'
      LabelPosition = lpLeftCenter
      LabelMargin = 8
      LabelTransparent = True
      LabelAlwaysEnabled = True
      LabelFont.Charset = DEFAULT_CHARSET
      LabelFont.Color = clWindowText
      LabelFont.Height = -13
      LabelFont.Name = 'Tahoma'
      LabelFont.Style = []
      ParentFont = False
      TabOrder = 2
      Text = 'PV HIGH'
      OnChange = EventTypeComboBoxChange
    end
    object OnEdit: TAdvEdit
      Tag = 100
      Left = 135
      Top = 346
      Width = 100
      Height = 23
      Cursor = crIBeam
      EditType = etNumeric
      EmptyTextStyle = []
      MaxValue = 5000
      MaxFloatValue = 5000.000000000000000000
      LabelCaption = 'On'
      LabelPosition = lpLeftCenter
      LabelMargin = 8
      LabelTransparent = True
      LabelAlwaysEnabled = True
      LabelFont.Charset = DEFAULT_CHARSET
      LabelFont.Color = clWindowText
      LabelFont.Height = -13
      LabelFont.Name = 'Tahoma'
      LabelFont.Style = []
      Lookup.Font.Charset = DEFAULT_CHARSET
      Lookup.Font.Color = clWindowText
      Lookup.Font.Height = -60
      Lookup.Font.Name = 'Arial'
      Lookup.Font.Style = []
      Lookup.Font.Quality = fqClearTypeNatural
      Lookup.Separator = ';'
      Color = clWindow
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Font.Quality = fqClearTypeNatural
      HideSelection = False
      ImeName = 'Microsoft Office IME 2007'
      ParentFont = False
      TabOrder = 3
      Text = '0'
      Visible = True
      OnEnter = OnEditEnter
      OnExit = OnEditExit
      Version = '4.0.4.2'
    end
    object STCComboBox: TAdvComboBox
      Left = 135
      Top = 20
      Width = 100
      Height = 23
      ParentCustomHint = False
      Color = clWindow
      Version = '2.0.0.6'
      Visible = True
      BiDiMode = bdLeftToRight
      ButtonWidth = 23
      EmptyTextStyle = []
      Ctl3D = True
      DropWidth = 0
      Enabled = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Font.Quality = fqClearTypeNatural
      ImeName = 'Microsoft Office IME 2007'
      ItemIndex = 0
      Items.Strings = (
        ' SSP Start'
        ' PV Start(Rate)'
        ' PV Start(Time)')
      LabelCaption = 'STC'
      LabelPosition = lpLeftCenter
      LabelMargin = 8
      LabelTransparent = True
      LabelAlwaysEnabled = True
      LabelFont.Charset = DEFAULT_CHARSET
      LabelFont.Color = clWindowText
      LabelFont.Height = -13
      LabelFont.Name = 'Tahoma'
      LabelFont.Style = []
      ParentBiDiMode = False
      ParentCtl3D = False
      ParentFont = False
      ParentShowHint = False
      ShowHint = False
      TabOrder = 4
      Text = ' SSP Start'
    end
    object OffEdit: TAdvEdit
      Tag = 100
      Left = 135
      Top = 371
      Width = 100
      Height = 23
      Cursor = crIBeam
      EditType = etNumeric
      EmptyTextStyle = []
      MaxValue = 5000
      MaxFloatValue = 5000.000000000000000000
      LabelCaption = 'Off'
      LabelPosition = lpLeftCenter
      LabelMargin = 8
      LabelTransparent = True
      LabelAlwaysEnabled = True
      LabelFont.Charset = DEFAULT_CHARSET
      LabelFont.Color = clWindowText
      LabelFont.Height = -13
      LabelFont.Name = 'Tahoma'
      LabelFont.Style = []
      Lookup.Font.Charset = DEFAULT_CHARSET
      Lookup.Font.Color = clWindowText
      Lookup.Font.Height = -60
      Lookup.Font.Name = 'Arial'
      Lookup.Font.Style = []
      Lookup.Font.Quality = fqClearTypeNatural
      Lookup.Separator = ';'
      Color = clWindow
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Font.Quality = fqClearTypeNatural
      HideSelection = False
      ImeName = 'Microsoft Office IME 2007'
      ParentFont = False
      TabOrder = 5
      Text = '0'
      Visible = False
      OnEnter = OnEditEnter
      OnExit = OnEditExit
      Version = '4.0.4.2'
    end
    object SSPEdit: TAdvEdit
      Tag = 100
      Left = 135
      Top = 45
      Width = 100
      Height = 23
      Cursor = crIBeam
      EditType = etNumeric
      EmptyTextStyle = []
      FocusBorder = True
      FocusBorderColor = clHighlight
      FocusColor = clHighlight
      FocusFontColor = clHighlightText
      FocusLabel = True
      MaxValue = 350
      MaxFloatValue = 350.000000000000000000
      LabelCaption = 'SSP'
      LabelPosition = lpLeftCenter
      LabelMargin = 8
      LabelTransparent = True
      LabelAlwaysEnabled = True
      LabelFont.Charset = DEFAULT_CHARSET
      LabelFont.Color = clWindowText
      LabelFont.Height = -13
      LabelFont.Name = 'Tahoma'
      LabelFont.Style = []
      Lookup.Font.Charset = DEFAULT_CHARSET
      Lookup.Font.Color = clWindowText
      Lookup.Font.Height = -60
      Lookup.Font.Name = 'Arial'
      Lookup.Font.Style = []
      Lookup.Font.Quality = fqClearTypeNatural
      Lookup.Separator = ';'
      Color = clWindow
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Font.Quality = fqClearTypeNatural
      HideSelection = False
      ImeName = 'Microsoft Office IME 2007'
      ParentFont = False
      TabOrder = 6
      Text = '0'
      Visible = True
      OnEnter = OnEditEnter
      OnExit = OnEditExit
      Version = '4.0.4.2'
    end
    object AddSegmentButton: TAdvGlassButton
      Left = 6
      Top = 270
      Width = 75
      Height = 30
      BackColor = clTeal
      Caption = 'Add'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clWhite
      GlowColor = 16760205
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clWhite
      ParentFont = False
      ShineColor = clWhite
      TabOrder = 7
      Version = '1.3.3.1'
      OnClick = AddSegmentButtonClick
    end
    object AddEventButton: TAdvGlassButton
      Left = 6
      Top = 396
      Width = 75
      Height = 31
      BackColor = clTeal
      Caption = 'Add'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clWhite
      GlowColor = 16760205
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clWhite
      ParentFont = False
      ShineColor = clWhite
      TabOrder = 8
      Version = '1.3.3.1'
      OnClick = AddEventButtonClick
    end
    object UpdateSegmentButton: TAdvGlassButton
      Left = 83
      Top = 270
      Width = 75
      Height = 30
      BackColor = clTeal
      Caption = 'Update'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clWhite
      GlowColor = 16760205
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clWhite
      ParentFont = False
      ShineColor = clWhite
      TabOrder = 9
      Version = '1.3.3.1'
      OnClick = UpdateSegmentButtonClick
    end
    object DeleteSegmentButton: TAdvGlassButton
      Left = 160
      Top = 270
      Width = 75
      Height = 30
      BackColor = clTeal
      Caption = 'Delete'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clWhite
      GlowColor = 16760205
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clWhite
      ParentFont = False
      ShineColor = clWhite
      TabOrder = 10
      Version = '1.3.3.1'
      OnClick = DeleteSegmentButtonClick
    end
    object UpdateEventButton: TAdvGlassButton
      Left = 83
      Top = 396
      Width = 75
      Height = 31
      BackColor = clTeal
      Caption = 'Update'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clWhite
      GlowColor = 16760205
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clWhite
      ParentFont = False
      ShineColor = clWhite
      TabOrder = 11
      Version = '1.3.3.1'
      OnClick = UpdateEventButtonClick
    end
    object DeleteEventButton: TAdvGlassButton
      Left = 160
      Top = 396
      Width = 75
      Height = 31
      BackColor = clTeal
      Caption = 'Delete'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clWhite
      GlowColor = 16760205
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clWhite
      ParentFont = False
      ShineColor = clWhite
      TabOrder = 12
      Version = '1.3.3.1'
      OnClick = DeleteEventButtonClick
    end
    object DamperEdit: TAdvEdit
      Tag = 100
      Left = 135
      Top = 221
      Width = 100
      Height = 23
      Cursor = crIBeam
      EditType = etNumeric
      EmptyTextStyle = []
      MaxValue = 100
      MaxFloatValue = 100.000000000000000000
      LabelCaption = 'Damper Open (%)'
      LabelPosition = lpLeftCenter
      LabelMargin = 8
      LabelTransparent = True
      LabelAlwaysEnabled = True
      LabelFont.Charset = DEFAULT_CHARSET
      LabelFont.Color = clWindowText
      LabelFont.Height = -13
      LabelFont.Name = 'Tahoma'
      LabelFont.Style = []
      Lookup.Font.Charset = DEFAULT_CHARSET
      Lookup.Font.Color = clWindowText
      Lookup.Font.Height = -60
      Lookup.Font.Name = 'Arial'
      Lookup.Font.Style = []
      Lookup.Font.Quality = fqClearTypeNatural
      Lookup.Separator = ';'
      Color = clWindow
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Font.Quality = fqClearTypeNatural
      HideSelection = False
      ImeName = 'Microsoft Office IME 2007'
      ParentFont = False
      TabOrder = 13
      Text = '0'
      Visible = True
      OnEnter = OnEditEnter
      OnExit = OnEditExit
      Version = '4.0.4.2'
    end
    object UpdateSTCButton: TAdvGlassButton
      Left = 160
      Top = 120
      Width = 75
      Height = 30
      BackColor = clTeal
      Caption = 'Update'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clWhite
      GlowColor = 16760205
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clWhite
      ParentFont = False
      ShineColor = clWhite
      TabOrder = 14
      Version = '1.3.3.1'
      OnClick = UpdateSTCButtonClick
    end
    object InverterEdit: TAdvEdit
      Tag = 200
      Left = 135
      Top = 246
      Width = 100
      Height = 23
      Cursor = crIBeam
      EditType = etFloat
      EmptyTextStyle = []
      MinValue = 10
      MaxValue = 60
      MinFloatValue = 10.000000000000000000
      MaxFloatValue = 60.000000000000000000
      Precision = 2
      LabelCaption = 'Inverter Freq. (Hz)'
      LabelPosition = lpLeftCenter
      LabelMargin = 8
      LabelTransparent = True
      LabelAlwaysEnabled = True
      LabelFont.Charset = DEFAULT_CHARSET
      LabelFont.Color = clWindowText
      LabelFont.Height = -13
      LabelFont.Name = 'Tahoma'
      LabelFont.Style = []
      Lookup.Font.Charset = DEFAULT_CHARSET
      Lookup.Font.Color = clWindowText
      Lookup.Font.Height = -60
      Lookup.Font.Name = 'Arial'
      Lookup.Font.Style = []
      Lookup.Font.Quality = fqClearTypeNatural
      Lookup.Separator = ';'
      Color = clWindow
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Font.Quality = fqClearTypeNatural
      HideSelection = False
      ImeName = 'Microsoft Office IME 2007'
      ParentFont = False
      TabOrder = 15
      Text = '30.00'
      Visible = True
      OnEnter = OnEditEnter
      OnExit = OnEditExit
      Version = '4.0.4.2'
    end
    object UseLocalModeCheckBox: TCheckBox
      Left = 114
      Top = 76
      Width = 121
      Height = 17
      Alignment = taLeftJustify
      Caption = 'Use Local Mode'
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = []
      Font.Quality = fqClearTypeNatural
      ParentColor = False
      ParentFont = False
      TabOrder = 16
    end
    object LocalTempEdit: TAdvEdit
      Tag = 100
      Left = 135
      Top = 95
      Width = 100
      Height = 24
      Cursor = crIBeam
      EditType = etNumeric
      EmptyTextStyle = []
      FlatLineColor = 11250603
      FocusBorder = True
      FocusBorderColor = clHighlight
      FocusColor = clWindow
      FocusFontColor = 3881787
      FocusLabel = True
      MaxValue = 350
      MaxFloatValue = 350.000000000000000000
      LabelCaption = 'Local Temp.'
      LabelPosition = lpLeftCenter
      LabelMargin = 8
      LabelTransparent = True
      LabelAlwaysEnabled = True
      LabelFont.Charset = DEFAULT_CHARSET
      LabelFont.Color = clWindowText
      LabelFont.Height = -13
      LabelFont.Name = 'Tahoma'
      LabelFont.Style = []
      Lookup.Font.Charset = DEFAULT_CHARSET
      Lookup.Font.Color = clWindowText
      Lookup.Font.Height = -11
      Lookup.Font.Name = 'Arial'
      Lookup.Font.Style = []
      Lookup.Font.Quality = fqClearTypeNatural
      Lookup.Separator = ';'
      Color = clWindow
      HideSelection = False
      ImeName = 'Microsoft Office IME 2007'
      TabOrder = 17
      Text = '0'
      Visible = True
      OnEnter = OnEditEnter
      OnExit = OnEditExit
      Version = '4.0.4.2'
    end
  end
  object PatternRepositoryPanel: TAdvPanel
    Left = 1
    Top = 1
    Width = 515
    Height = 190
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    UseDockManager = True
    Version = '2.6.3.4'
    BorderColor = clBlack
    Caption.Color = clHighlight
    Caption.ColorTo = clNone
    Caption.Font.Charset = DEFAULT_CHARSET
    Caption.Font.Color = clWhite
    Caption.Font.Height = -13
    Caption.Font.Name = 'Tahoma'
    Caption.Font.Style = [fsBold]
    Caption.Indent = 0
    Caption.Text = '<P align="center">1. Select Pattern Repository</P>'
    Caption.Visible = True
    DoubleBuffered = True
    StatusBar.Font.Charset = DEFAULT_CHARSET
    StatusBar.Font.Color = clWindowText
    StatusBar.Font.Height = -11
    StatusBar.Font.Name = 'Tahoma'
    StatusBar.Font.Style = []
    Text = ''
    FullHeight = 200
    object BtnPattern1: TAdvSmoothToggleButton
      Left = 4
      Top = 23
      Width = 40
      Height = 40
      ColorDown = clYellow
      BorderColor = clLime
      BorderInnerColor = clLime
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '1'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 0
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ShowFocus = False
      ParentFont = False
      TabOrder = 0
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern10: TAdvSmoothToggleButton
      Left = 364
      Top = 23
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '10'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern11: TAdvSmoothToggleButton
      Left = 4
      Top = 63
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '11'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern12: TAdvSmoothToggleButton
      Left = 44
      Top = 63
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '12'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 3
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern13: TAdvSmoothToggleButton
      Left = 84
      Top = 63
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '13'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 0
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 4
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern14: TAdvSmoothToggleButton
      Left = 124
      Top = 63
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '14'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 5
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern15: TAdvSmoothToggleButton
      Left = 164
      Top = 63
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '15'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 6
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern16: TAdvSmoothToggleButton
      Left = 204
      Top = 63
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '16'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 7
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern17: TAdvSmoothToggleButton
      Left = 244
      Top = 63
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '17'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 8
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern18: TAdvSmoothToggleButton
      Left = 284
      Top = 63
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '18'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 9
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern19: TAdvSmoothToggleButton
      Left = 324
      Top = 63
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '19'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 0
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 10
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern2: TAdvSmoothToggleButton
      Left = 44
      Top = 23
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BorderInnerColor = clFuchsia
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '2'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 11
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern20: TAdvSmoothToggleButton
      Left = 364
      Top = 63
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '20'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 12
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern21: TAdvSmoothToggleButton
      Left = 4
      Top = 103
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '21'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 13
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern22: TAdvSmoothToggleButton
      Left = 44
      Top = 103
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '22'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 14
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern23: TAdvSmoothToggleButton
      Left = 84
      Top = 103
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '23'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 15
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern24: TAdvSmoothToggleButton
      Left = 124
      Top = 103
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '24'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 16
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern25: TAdvSmoothToggleButton
      Left = 164
      Top = 103
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '25'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 0
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 17
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern26: TAdvSmoothToggleButton
      Left = 204
      Top = 103
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '26'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 18
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern27: TAdvSmoothToggleButton
      Left = 244
      Top = 103
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '27'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 19
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern28: TAdvSmoothToggleButton
      Left = 284
      Top = 103
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '28'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 20
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern29: TAdvSmoothToggleButton
      Left = 324
      Top = 103
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '29'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 21
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern3: TAdvSmoothToggleButton
      Left = 84
      Top = 23
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '3'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 22
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern30: TAdvSmoothToggleButton
      Left = 364
      Top = 103
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '30'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 23
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern4: TAdvSmoothToggleButton
      Left = 124
      Top = 23
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '4'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 24
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern5: TAdvSmoothToggleButton
      Left = 164
      Top = 23
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '5'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 25
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern6: TAdvSmoothToggleButton
      Left = 204
      Top = 23
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '6'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 26
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern7: TAdvSmoothToggleButton
      Left = 244
      Top = 23
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '7'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 0
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 27
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern8: TAdvSmoothToggleButton
      Left = 284
      Top = 23
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '8'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 28
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPattern9: TAdvSmoothToggleButton
      Left = 324
      Top = 23
      Width = 40
      Height = 40
      ColorDown = clMenuHighlight
      BevelWidth = 2
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -16
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = '9'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 29
      OnClick = BtnPattern1Click
      TMSStyle = 0
    end
    object BtnPtnDelete: TAdvGlassButton
      Left = 224
      Top = 147
      Width = 100
      Height = 38
      BackColor = clRed
      Caption = 'Delete'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clWhite
      GlowColor = 16760205
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clWhite
      ParentFont = False
      ShineColor = clWhite
      TabOrder = 30
      Version = '1.3.3.1'
      OnClick = BtnPtnDeleteClick
    end
    object BtnPtnSave: TAdvGlassButton
      Left = 84
      Top = 147
      Width = 100
      Height = 38
      BackColor = clBlue
      Caption = 'Save'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clWhite
      GlowColor = 16760205
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clWhite
      ParentFont = False
      ShineColor = clWhite
      TabOrder = 31
      Version = '1.3.3.1'
      OnClick = BtnPtnSaveClick
    end
    object AdvSmoothToggleButton1: TAdvSmoothToggleButton
      Left = 419
      Top = 23
      Width = 90
      Height = 40
      Color = clLime
      ColorDisabled = clLime
      ColorDown = clMenuHighlight
      BorderInnerColor = clMenuHighlight
      BevelWidth = 1
      BevelColor = clSkyBlue
      BevelColorDisabled = clSkyBlue
      BevelColorDown = clSkyBlue
      Enabled = False
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -15
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = 'Existed'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 32
      TMSStyle = 0
    end
    object AdvSmoothToggleButton2: TAdvSmoothToggleButton
      Left = 419
      Top = 63
      Width = 90
      Height = 40
      Color = clLime
      ColorDisabled = clYellow
      ColorDown = clYellow
      BorderInnerColor = clMenuHighlight
      BevelWidth = 1
      BevelColor = clSkyBlue
      BevelColorDisabled = clSkyBlue
      BevelColorDown = clSkyBlue
      Enabled = False
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -15
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = 'Selected'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 33
      TMSStyle = 0
    end
    object AdvSmoothToggleButton3: TAdvSmoothToggleButton
      Left = 419
      Top = 103
      Width = 90
      Height = 40
      ColorDisabled = clSilver
      ColorDown = clMenuHighlight
      BevelWidth = 1
      BevelColorDisabled = clWhite
      Enabled = False
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -15
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.SimpleLayout = False
      Caption = 'Empty'
      Version = '1.7.2.3'
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 8
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      ParentFont = False
      TabOrder = 34
      TMSStyle = 0
    end
  end
  object RegisterRecipePanel: TAdvPanel
    Left = 517
    Top = 1
    Width = 489
    Height = 190
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    UseDockManager = True
    Version = '2.6.3.4'
    BorderColor = clBlack
    Caption.Color = clHighlight
    Caption.ColorTo = clNone
    Caption.Font.Charset = DEFAULT_CHARSET
    Caption.Font.Color = clWhite
    Caption.Font.Height = -13
    Caption.Font.Name = 'Tahoma'
    Caption.Font.Style = [fsBold]
    Caption.Indent = 0
    Caption.Text = 
      '<FONT color="#FFFFFF"><P align="center">3. Register Recipe</P></' +
      'FONT>'
    Caption.Visible = True
    DoubleBuffered = True
    StatusBar.Font.Charset = DEFAULT_CHARSET
    StatusBar.Font.Color = clWindowText
    StatusBar.Font.Height = -11
    StatusBar.Font.Name = 'Tahoma'
    StatusBar.Font.Style = []
    Text = ''
    FullHeight = 200
    object GrdRecipeEdit: TAdvStringGrid
      Left = 0
      Top = 20
      Width = 488
      Height = 109
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
      ParentDoubleBuffered = False
      ParentFont = False
      TabOrder = 0
      OnDrawCell = GrdRecipeEditDrawCell
      GridLineColor = 15855083
      GridFixedLineColor = 13745060
      OnClickCell = GrdRecipeEditClickCell
      OnDblClickCell = GrdRecipeEditDblClickCell
      HintColor = clBtnHighlight
      ActiveCellFont.Charset = DEFAULT_CHARSET
      ActiveCellFont.Color = clWindowText
      ActiveCellFont.Height = -11
      ActiveCellFont.Name = 'Tahoma'
      ActiveCellFont.Style = [fsBold]
      ActiveCellColor = 10344697
      ActiveCellColorTo = 6210033
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
      FixedRowHeight = 22
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
      ScrollWidth = 21
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
      ShowSelection = False
      UIStyle = tsOffice2007Luna
      VAlignment = vtaCenter
      Version = '8.7.2.6'
    end
    object BtnRecipeDelete: TAdvGlassButton
      Left = 386
      Top = 134
      Width = 76
      Height = 51
      BackColor = clRed
      Caption = 'Delete'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clWhite
      GlowColor = 16760205
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clWhite
      ParentFont = False
      ShineColor = clWhite
      TabOrder = 1
      Version = '1.3.3.1'
      OnClick = BtnRecipeDeleteClick
    end
    object BtnRecipeAdd: TAdvGlassButton
      Left = 304
      Top = 134
      Width = 76
      Height = 51
      BackColor = clRed
      Caption = 'Add'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
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
      OnClick = BtnRecipeAddClick
    end
    object RecipeNameEdit: TAdvEdit
      Tag = 3
      Left = 136
      Top = 134
      Width = 107
      Height = 24
      EmptyText = 'Input name.'
      EmptyTextStyle = []
      LabelCaption = 'Recipe Name'
      LabelPosition = lpLeftCenter
      LabelMargin = 8
      LabelTransparent = True
      LabelAlwaysEnabled = True
      LabelFont.Charset = DEFAULT_CHARSET
      LabelFont.Color = clWindowText
      LabelFont.Height = -16
      LabelFont.Name = 'Tahoma'
      LabelFont.Style = []
      Lookup.Font.Charset = DEFAULT_CHARSET
      Lookup.Font.Color = clWindowText
      Lookup.Font.Height = -60
      Lookup.Font.Name = 'Arial'
      Lookup.Font.Style = []
      Lookup.Separator = ';'
      Color = clWindow
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Font.Quality = fqClearTypeNatural
      ImeName = 'Microsoft Office IME 2007'
      ParentFont = False
      TabOrder = 3
      Text = ''
      Visible = True
      Version = '4.0.4.2'
    end
    object SelectPatternComboBox: TAdvComboBox
      Left = 136
      Top = 161
      Width = 107
      Height = 24
      Color = clWindow
      Version = '2.0.0.6'
      Visible = True
      ButtonWidth = 23
      EmptyText = 'Select pattern.'
      EmptyTextStyle = []
      DropWidth = 0
      Enabled = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      Font.Quality = fqClearTypeNatural
      ImeName = 'Microsoft Office IME 2007'
      ItemIndex = -1
      LabelCaption = 'Pattern'
      LabelPosition = lpLeftCenter
      LabelMargin = 8
      LabelTransparent = True
      LabelAlwaysEnabled = True
      LabelFont.Charset = DEFAULT_CHARSET
      LabelFont.Color = clWindowText
      LabelFont.Height = -16
      LabelFont.Name = 'Tahoma'
      LabelFont.Style = []
      ParentFont = False
      TabOrder = 4
      OnEnter = SelectPatternComboBoxEnter
    end
  end
end
