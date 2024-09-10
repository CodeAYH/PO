object FrmSystemSetting: TFrmSystemSetting
  Left = 0
  Top = 0
  Width = 1007
  Height = 699
  TabOrder = 0
  object SystemSettingPanel: TAdvPanel
    Left = 0
    Top = 0
    Width = 1007
    Height = 699
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    UseDockManager = True
    Version = '2.6.3.2'
    BorderColor = clBlack
    Caption.Color = clHighlight
    Caption.ColorTo = clNone
    Caption.Font.Charset = DEFAULT_CHARSET
    Caption.Font.Color = clHighlightText
    Caption.Font.Height = -27
    Caption.Font.Name = 'Tahoma'
    Caption.Font.Style = []
    Caption.Height = 28
    Caption.Indent = 0
    Caption.Text = 
      '<P align="center"><B><FONT color="#FFFFFF"  size="16" face="Taho' +
      'ma">System Setting</FONT></B></P>'
    Caption.TopIndent = -9
    Caption.Visible = True
    DoubleBuffered = True
    StatusBar.Font.Charset = DEFAULT_CHARSET
    StatusBar.Font.Color = clWindowText
    StatusBar.Font.Height = -11
    StatusBar.Font.Name = 'Tahoma'
    StatusBar.Font.Style = []
    Text = ''
    FullHeight = 674
    object BtnSystemSave: TAdvGlassButton
      Left = 789
      Top = 634
      Width = 100
      Height = 55
      BackColor = clBlue
      Caption = 'Save'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clWhite
      GlowColor = 16760205
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clWhite
      ParentFont = False
      ShineColor = clWhite
      TabOrder = 0
      Version = '1.3.3.0'
      OnClick = BtnSystemSaveClick
    end
    object BtnSystemCancel: TAdvGlassButton
      Left = 895
      Top = 634
      Width = 100
      Height = 55
      BackColor = clGreen
      Caption = 'Cancel'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
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
      Version = '1.3.3.0'
      OnClick = BtnSystemCancelClick
    end
    object AdvPanel2: TAdvPanel
      Left = -5
      Top = 247
      Width = 504
      Height = 204
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      UseDockManager = True
      Version = '2.6.3.2'
      BorderColor = clSilver
      BorderWidth = 1
      Caption.Color = clSkyBlue
      Caption.ColorTo = clNone
      Caption.Font.Charset = DEFAULT_CHARSET
      Caption.Font.Color = clWindowText
      Caption.Font.Height = -13
      Caption.Font.Name = 'Tahoma'
      Caption.Font.Style = [fsBold]
      Caption.Height = 18
      Caption.Indent = 0
      Caption.Text = '<FONT  size="10"  face="Tahoma"><B>COM Port</B></FONT>'
      Caption.Visible = True
      DoubleBuffered = True
      StatusBar.Font.Charset = DEFAULT_CHARSET
      StatusBar.Font.Color = clWindowText
      StatusBar.Font.Height = -11
      StatusBar.Font.Name = 'Tahoma'
      StatusBar.Font.Style = []
      Text = ''
      FullHeight = 200
      object LoggerPortComboBox: TAdvComboBox
        Left = 400
        Top = 61
        Width = 90
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        Items.Strings = (
          'COM1'
          'COM2'
          'COM3'
          'COM4')
        LabelCaption = 'Temp. Logger Port'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object TempLimitPortComboBox: TAdvComboBox
        Left = 400
        Top = 40
        Width = 90
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        Items.Strings = (
          'COM1'
          'COM2'
          'COM3'
          'COM4')
        LabelCaption = 'Chamber Temp. Limit  Port'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
      end
      object MainTempControllerPortComboBox: TAdvComboBox
        Left = 400
        Top = 19
        Width = 90
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        Items.Strings = (
          'COM1'
          'COM2'
          'COM3'
          'COM4')
        LabelCaption = 'Main Temp. Cotroller  Port'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
      end
      object DIOPortComboBox: TAdvComboBox
        Left = 400
        Top = 82
        Width = 90
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        Items.Strings = (
          'COM1'
          'COM2'
          'COM3'
          'COM4')
        LabelCaption = 'DIO Port'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 3
      end
      object DamperMotorPortComboBox: TAdvComboBox
        Left = 144
        Top = 50
        Width = 90
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        Items.Strings = (
          'COM1'
          'COM2'
          'COM3'
          'COM4')
        LabelCaption = 'Damper Motor Port'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 4
      end
      object DPPortComboBox: TAdvComboBox
        Left = 144
        Top = 23
        Width = 90
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        Items.Strings = (
          'COM1'
          'COM2'
          'COM3'
          'COM4')
        LabelCaption = 'Diff. Pressure Port'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 5
      end
      object ChamberAO2PortComboBox: TAdvComboBox
        Left = 400
        Top = 114
        Width = 90
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        Items.Strings = (
          'COM1'
          'COM2'
          'COM3'
          'COM4')
        LabelCaption = 'Chamber A O2 Analyzer Port'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 6
      end
      object ChamberBO2PortComboBox: TAdvComboBox
        Left = 400
        Top = 135
        Width = 90
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        Items.Strings = (
          'COM1'
          'COM2'
          'COM3'
          'COM4')
        LabelCaption = 'Chamber B O2 Analyzer Port'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 7
      end
      object InverterPortComboBox: TAdvComboBox
        Left = 144
        Top = 71
        Width = 90
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        Items.Strings = (
          'COM1'
          'COM2'
          'COM3'
          'COM4')
        LabelCaption = 'Inverter Port'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 8
      end
      object ChamberCO2PortComboBox: TAdvComboBox
        Left = 400
        Top = 156
        Width = 90
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        Items.Strings = (
          'COM1'
          'COM2'
          'COM3'
          'COM4')
        LabelCaption = 'Chamber C O2 Analyzer Port'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 9
      end
      object ChamberDO2PortComboBox: TAdvComboBox
        Left = 400
        Top = 177
        Width = 90
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        Items.Strings = (
          'COM1'
          'COM2'
          'COM3'
          'COM4')
        LabelCaption = 'Chamber D O2 Analyzer Port'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 10
      end
      object RfidReaderAPortComboBox: TAdvComboBox
        Left = 144
        Top = 94
        Width = 90
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        Items.Strings = (
          'COM1'
          'COM2'
          'COM3'
          'COM4')
        LabelCaption = 'RFID Reader A  Port'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 11
      end
    end
    object MainTempCtrlPanel: TAdvPanel
      Left = 505
      Top = 27
      Width = 501
      Height = 532
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      UseDockManager = True
      Visible = False
      Version = '2.6.3.2'
      BorderColor = clSilver
      BorderWidth = 1
      Caption.Color = clSkyBlue
      Caption.ColorTo = clNone
      Caption.Font.Charset = DEFAULT_CHARSET
      Caption.Font.Color = clWindowText
      Caption.Font.Height = -13
      Caption.Font.Name = 'Tahoma'
      Caption.Font.Style = [fsBold]
      Caption.Height = 18
      Caption.Indent = 0
      Caption.Text = '<FONT  size="10"  face="Tahoma"><B>Factory Settings</B></FONT>'
      Caption.Visible = True
      DoubleBuffered = True
      StatusBar.Font.Charset = DEFAULT_CHARSET
      StatusBar.Font.Color = clWindowText
      StatusBar.Font.Height = -11
      StatusBar.Font.Name = 'Tahoma'
      StatusBar.Font.Style = []
      Text = ''
      FullHeight = 200
      object ChamberCountComboBox: TAdvComboBox
        Left = 280
        Top = 17
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          '1'
          '2'
          '4')
        LabelCaption = 'Chamber Count'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        Text = '1'
      end
      object DiffPressureTypeComboBox: TAdvComboBox
        Left = 280
        Top = 105
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Normal Type'
          'Communication Type')
        LabelCaption = 'Diff. Pressure Type'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        Text = 'Normal Type'
      end
      object UseO2AnalyzerComboBox: TAdvComboBox
        Left = 280
        Top = 83
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use O2 Analyzer'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
        Text = 'Not Use'
      end
      object DamperControlTypeComboBox: TAdvComboBox
        Left = 280
        Top = 149
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        Items.Strings = (
          'Cylinder'
          'Moons Step Motor'
          'Ezi Step Motor')
        LabelCaption = 'Damper Control Type'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 3
        Text = 'Cylinder'
      end
      object UseMagazineDetectorComboBox: TAdvComboBox
        Left = 280
        Top = 215
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use Magazine Detector'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 4
        Text = 'Not Use'
      end
      object UseWaterFlowDetectorComboBox: TAdvComboBox
        Left = 280
        Top = 237
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use Water Flow Detector'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 5
        Text = 'Not Use'
      end
      object UseAirPressureDetectorComboBox: TAdvComboBox
        Left = 280
        Top = 259
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use Air Pressure Detector'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 6
        Text = 'Not Use'
      end
      object UseElectricPanelDoorDetectorComboBox: TAdvComboBox
        Left = 280
        Top = 281
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use Electric Panel Door Detector'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 7
        Text = 'Not Use'
      end
      object UseN2FlowDetectorComboBox: TAdvComboBox
        Left = 280
        Top = 303
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use N2 Flow Detector'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 8
        Text = 'Not Use'
      end
      object DIOTypeComboBox: TAdvComboBox
        Left = 280
        Top = 61
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'ICPDAS'
          'OVEN BOARD')
        LabelCaption = 'DIO Type'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 9
        Text = 'ICPDAS'
      end
      object UseInverterComboBox: TAdvComboBox
        Left = 280
        Top = 127
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use Inverter'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 10
        Text = 'Not Use'
      end
      object UseN2FlowControlComboBox: TAdvComboBox
        Left = 280
        Top = 193
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use N2 Flow Control'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 11
        Text = 'Not Use'
      end
      object LoggerCountComboBox: TAdvComboBox
        Left = 280
        Top = 39
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        Items.Strings = (
          '1'
          '2')
        LabelCaption = 'Logger Count'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 12
        Text = '1'
      end
      object UseN2PressureDetectorComboBox: TAdvComboBox
        Left = 280
        Top = 325
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use N2 Pressure Detector'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 13
        Text = 'Not Use'
      end
      object UseHeaterOverloadDetectorComboBox: TAdvComboBox
        Left = 280
        Top = 347
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use Heater Overload Detector'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 14
        Text = 'Not Use'
      end
      object UseDiffPressureDetectorComboBox: TAdvComboBox
        Left = 280
        Top = 369
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use Diff. Pressure Detector'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 15
        Text = 'Not Use'
      end
      object UseSSROverTempDetectorComboBox: TAdvComboBox
        Left = 280
        Top = 391
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use SSR Over Temp.  Detector'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 16
        Text = 'Not Use'
      end
      object UseChamberDoorTempInterlockComboBox: TAdvComboBox
        Left = 280
        Top = 413
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use Chamber Door Temp. Interlock'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 17
        Text = 'Not Use'
      end
      object UseGroundAlarmDetectorComboBox: TAdvComboBox
        Left = 280
        Top = 435
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use Ground Alarm Detector'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 18
        Text = 'Not Use'
      end
      object UseBlowMotorLowCurrentDetectorComboBox: TAdvComboBox
        Left = 280
        Top = 457
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use Blow Motor Low Current Detector'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 19
        Text = 'Not Use'
      end
      object UseDoorLockCylinderComboBox: TAdvComboBox
        Left = 280
        Top = 171
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use Door Lock Cylinder'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 20
        Text = 'Not Use'
      end
      object UseTowerLampControlComboBox: TAdvComboBox
        Left = 280
        Top = 479
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use Tower Lamp Control'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 21
        Text = 'Not Use'
      end
      object UseLoggerDataSaveComboBox: TAdvComboBox
        Left = 280
        Top = 501
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use Logger Data Save'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 22
        Text = 'Not Use'
      end
    end
    object AdvPanel3: TAdvPanel
      Left = -5
      Top = 22
      Width = 504
      Height = 219
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      UseDockManager = True
      Version = '2.6.3.2'
      BorderColor = clSilver
      BorderWidth = 1
      Caption.Color = clSkyBlue
      Caption.ColorTo = clNone
      Caption.Font.Charset = DEFAULT_CHARSET
      Caption.Font.Color = clWindowText
      Caption.Font.Height = -13
      Caption.Font.Name = 'Tahoma'
      Caption.Font.Style = [fsBold]
      Caption.Height = 18
      Caption.Indent = 0
      Caption.Text = '<FONT  size="10"  face="Tahoma"><B>General</B></FONT>'
      Caption.Visible = True
      DoubleBuffered = True
      StatusBar.Font.Charset = DEFAULT_CHARSET
      StatusBar.Font.Color = clWindowText
      StatusBar.Font.Height = -11
      StatusBar.Font.Name = 'Tahoma'
      StatusBar.Font.Style = []
      Text = ''
      FullHeight = 220
      object LanguageComboBox: TAdvComboBox
        Left = 280
        Top = 17
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        LabelCaption = 'Language'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object JobEndTimerEdit: TAdvEdit
        Left = 280
        Top = 37
        Width = 210
        Height = 20
        EmptyTextStyle = []
        FlatLineColor = 11250603
        FocusColor = clWindow
        FocusFontColor = 3881787
        LabelCaption = 'Job End Timer (.sec)'
        LabelPosition = lpLeftCenter
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        Lookup.Font.Charset = DEFAULT_CHARSET
        Lookup.Font.Color = clWindowText
        Lookup.Font.Height = -33
        Lookup.Font.Name = 'Arial'
        Lookup.Font.Style = []
        Lookup.Separator = ';'
        AutoSize = False
        Color = clWindow
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 1
        Text = ''
        Visible = True
        Version = '4.0.3.6'
      end
      object DoorLockTempEdit: TAdvEdit
        Left = 280
        Top = 76
        Width = 210
        Height = 20
        EmptyTextStyle = []
        FlatLineColor = 11250603
        FocusColor = clWindow
        FocusFontColor = 3881787
        LabelCaption = 'Door Lock Temp'
        LabelPosition = lpLeftCenter
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        Lookup.Font.Charset = DEFAULT_CHARSET
        Lookup.Font.Color = clWindowText
        Lookup.Font.Height = -33
        Lookup.Font.Name = 'Arial'
        Lookup.Font.Style = []
        Lookup.Separator = ';'
        AutoSize = False
        Color = clWindow
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 2
        Text = ''
        Visible = True
        Version = '4.0.3.6'
      end
      object UseDoorLockTempControlComboBox: TAdvComboBox
        Left = 280
        Top = 56
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use Door Lock Temp Control'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 3
        Text = 'Not Use'
      end
      object UsePasswordWhenIdleCoolingComboBox: TAdvComboBox
        Left = 280
        Top = 95
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use Password When Idle Cooling Stop'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 4
        Text = 'Not Use'
      end
      object WaterCoolingExuastTimeEdit: TAdvEdit
        Left = 280
        Top = 115
        Width = 210
        Height = 19
        EmptyTextStyle = []
        FlatLineColor = 11250603
        FocusColor = clWindow
        FocusFontColor = 3881787
        LabelCaption = 'Water Cooling Exuast Time (sec.)'
        LabelPosition = lpLeftCenter
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        Lookup.Font.Charset = DEFAULT_CHARSET
        Lookup.Font.Color = clWindowText
        Lookup.Font.Height = -33
        Lookup.Font.Name = 'Arial'
        Lookup.Font.Style = []
        Lookup.Separator = ';'
        AutoSize = False
        Color = clWindow
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 5
        Text = ''
        Visible = True
        Version = '4.0.3.6'
      end
      object UseLocalAlarmComboBox: TAdvComboBox
        Left = 280
        Top = 133
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Use Local Mode N2 Alarm'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 6
        Text = 'Not Use'
      end
      object DraBgSPComboBox: TAdvComboBox
        Left = 280
        Top = 153
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'Not Use'
          'Use')
        LabelCaption = 'Draw SP Background in chart'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 7
        Text = 'Not Use'
      end
      object CureStartAlarmTimeEdit: TAdvEdit
        Left = 280
        Top = 173
        Width = 210
        Height = 19
        EmptyTextStyle = []
        FlatLineColor = 11250603
        FocusColor = clWindow
        FocusFontColor = 3881787
        LabelCaption = 'Cure Start Alarm Time (sec.) [0 = Not Use]'
        LabelPosition = lpLeftCenter
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        Lookup.Font.Charset = DEFAULT_CHARSET
        Lookup.Font.Color = clWindowText
        Lookup.Font.Height = -33
        Lookup.Font.Name = 'Arial'
        Lookup.Font.Style = []
        Lookup.Separator = ';'
        AutoSize = False
        Color = clWindow
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 8
        Text = ''
        Visible = True
        Version = '4.0.3.6'
      end
      object ModeChangeTimerEdit: TAdvEdit
        Left = 280
        Top = 191
        Width = 210
        Height = 19
        EmptyTextStyle = []
        FlatLineColor = 11250603
        FocusColor = clWindow
        FocusFontColor = 3881787
        LabelCaption = 'Auto Mode Change Timer (sec.) [0 = Not Use]'
        LabelPosition = lpLeftCenter
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        Lookup.Font.Charset = DEFAULT_CHARSET
        Lookup.Font.Color = clWindowText
        Lookup.Font.Height = -33
        Lookup.Font.Name = 'Arial'
        Lookup.Font.Style = []
        Lookup.Separator = ';'
        AutoSize = False
        Color = clWindow
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 9
        Text = ''
        Visible = True
        Version = '4.0.3.6'
      end
    end
    object AdvPanel1: TAdvPanel
      Left = 1
      Top = 457
      Width = 504
      Height = 192
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
      UseDockManager = True
      Version = '2.6.3.2'
      BorderColor = clSilver
      BorderWidth = 1
      Caption.Color = clSkyBlue
      Caption.ColorTo = clNone
      Caption.Font.Charset = DEFAULT_CHARSET
      Caption.Font.Color = clWindowText
      Caption.Font.Height = -13
      Caption.Font.Name = 'Tahoma'
      Caption.Font.Style = [fsBold]
      Caption.Height = 18
      Caption.Indent = 0
      Caption.Text = '<FONT  size="10"  face="Tahoma"><B>Automation</B></FONT>'
      Caption.Visible = True
      DoubleBuffered = True
      StatusBar.Font.Charset = DEFAULT_CHARSET
      StatusBar.Font.Color = clWindowText
      StatusBar.Font.Height = -11
      StatusBar.Font.Name = 'Tahoma'
      StatusBar.Font.Style = []
      Text = ''
      FullHeight = 200
      object AutomationKindComboBox: TAdvComboBox
        Left = 279
        Top = 38
        Width = 210
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = 0
        Items.Strings = (
          'COM1'
          'COM2'
          'COM3'
          'COM4')
        LabelCaption = 'Automation Kind'
        LabelPosition = lpLeftCenter
        LabelMargin = 8
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        Text = 'COM1'
        OnChange = AutomationKindComboBoxChange
      end
      object UseAutomationCheckBox: TCheckBox
        Left = 363
        Top = 18
        Width = 135
        Height = 17
        ParentCustomHint = False
        Alignment = taLeftJustify
        BiDiMode = bdLeftToRight
        Caption = 'Use Automation'
        Color = clBtnFace
        Ctl3D = True
        DoubleBuffered = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentBiDiMode = False
        ParentColor = False
        ParentCtl3D = False
        ParentDoubleBuffered = False
        ParentFont = False
        ParentShowHint = False
        ShowHint = False
        TabOrder = 1
        OnClick = UseAutomationCheckBoxClick
      end
      object OFilmMES_DeviceID: TAdvEdit
        Left = 279
        Top = 62
        Width = 210
        Height = 20
        EmptyTextStyle = []
        LabelCaption = 'DEVICE ID'
        LabelPosition = lpLeftCenter
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        Lookup.Font.Charset = DEFAULT_CHARSET
        Lookup.Font.Color = clWindowText
        Lookup.Font.Height = -33
        Lookup.Font.Name = 'Arial'
        Lookup.Font.Style = []
        Lookup.Separator = ';'
        AutoSize = False
        Color = clWindow
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 2
        Text = ''
        Visible = True
        Version = '4.0.3.6'
      end
      object OFilmMES_IP: TAdvEdit
        Left = 279
        Top = 82
        Width = 210
        Height = 20
        EmptyTextStyle = []
        LabelCaption = 'FTP_IP'
        LabelPosition = lpLeftCenter
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        Lookup.Font.Charset = DEFAULT_CHARSET
        Lookup.Font.Color = clWindowText
        Lookup.Font.Height = -43
        Lookup.Font.Name = 'Arial'
        Lookup.Font.Style = []
        Lookup.Separator = ';'
        AutoSize = False
        Color = clWindow
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 3
        Text = ''
        Visible = True
        Version = '4.0.3.6'
      end
      object OFilmMES_PORT: TAdvEdit
        Left = 279
        Top = 102
        Width = 210
        Height = 20
        EmptyTextStyle = []
        LabelCaption = 'FTP_Port'
        LabelPosition = lpLeftCenter
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        Lookup.Font.Charset = DEFAULT_CHARSET
        Lookup.Font.Color = clWindowText
        Lookup.Font.Height = -52
        Lookup.Font.Name = 'Arial'
        Lookup.Font.Style = []
        Lookup.Separator = ';'
        AutoSize = False
        Color = clWindow
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 4
        Text = ''
        Visible = True
        Version = '4.0.3.6'
      end
      object OFilmMES_ID: TAdvEdit
        Left = 279
        Top = 122
        Width = 210
        Height = 20
        EmptyTextStyle = []
        LabelCaption = 'FTP_ID'
        LabelPosition = lpLeftCenter
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        Lookup.Font.Charset = DEFAULT_CHARSET
        Lookup.Font.Color = clWindowText
        Lookup.Font.Height = -65
        Lookup.Font.Name = 'Arial'
        Lookup.Font.Style = []
        Lookup.Separator = ';'
        AutoSize = False
        Color = clWindow
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 5
        Text = ''
        Visible = True
        Version = '4.0.3.6'
      end
      object OFilmMES_FOLDER: TAdvEdit
        Left = 279
        Top = 162
        Width = 210
        Height = 20
        EmptyTextStyle = []
        LabelCaption = 'FTP_FOLDER'
        LabelPosition = lpLeftCenter
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        Lookup.Font.Charset = DEFAULT_CHARSET
        Lookup.Font.Color = clWindowText
        Lookup.Font.Height = -103
        Lookup.Font.Name = 'Arial'
        Lookup.Font.Style = []
        Lookup.Separator = ';'
        AutoSize = False
        Color = clWindow
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 6
        Text = ''
        Visible = True
        Version = '4.0.3.6'
      end
      object OFilmMES_PW: TAdvEdit
        Left = 279
        Top = 142
        Width = 210
        Height = 20
        EmptyTextStyle = []
        LabelCaption = 'FTP_PW'
        LabelPosition = lpLeftCenter
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        Lookup.Font.Charset = DEFAULT_CHARSET
        Lookup.Font.Color = clWindowText
        Lookup.Font.Height = -83
        Lookup.Font.Name = 'Arial'
        Lookup.Font.Style = []
        Lookup.Separator = ';'
        AutoSize = False
        Color = clWindow
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 7
        Text = ''
        Visible = True
        Version = '4.0.3.6'
      end
      object ChamberAHanaPortComboBox: TAdvComboBox
        Left = 4
        Top = 46
        Width = 137
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        Items.Strings = (
          'COM1'
          'COM2'
          'COM3'
          'COM4')
        LabelCaption = 'Chamber A Hana Port'
        LabelPosition = lpTopLeft
        LabelMargin = 1
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 8
      end
      object ChamberBHanaPortComboBox: TAdvComboBox
        Left = 4
        Top = 82
        Width = 137
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        Items.Strings = (
          'COM1'
          'COM2'
          'COM3'
          'COM4')
        LabelCaption = 'Chamber B Hana Port'
        LabelPosition = lpTopLeft
        LabelMargin = 1
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 9
      end
      object ChamberCHanaPortComboBox: TAdvComboBox
        Left = 4
        Top = 118
        Width = 137
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        Items.Strings = (
          'COM1'
          'COM2'
          'COM3'
          'COM4')
        LabelCaption = 'Chamber C Hana Port'
        LabelPosition = lpTopLeft
        LabelMargin = 1
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 10
      end
      object ChamberDHanaPortComboBox: TAdvComboBox
        Left = 4
        Top = 154
        Width = 137
        Height = 21
        Color = clWindow
        Version = '2.0.0.6'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        Items.Strings = (
          'COM1'
          'COM2'
          'COM3'
          'COM4')
        LabelCaption = 'Chamber D Hana Port'
        LabelPosition = lpTopLeft
        LabelMargin = 1
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        ParentFont = False
        TabOrder = 11
      end
      object UseCureEnableFromLocalCheckBox: TCheckBox
        Left = 2
        Top = 18
        Width = 205
        Height = 17
        ParentCustomHint = False
        Alignment = taLeftJustify
        BiDiMode = bdLeftToRight
        Caption = 'Use Cure Enable From Local State'
        Color = clBtnFace
        Ctl3D = True
        DoubleBuffered = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentBiDiMode = False
        ParentColor = False
        ParentCtl3D = False
        ParentDoubleBuffered = False
        ParentFont = False
        ParentShowHint = False
        ShowHint = False
        TabOrder = 12
        OnMouseDown = UseCureEnableFromLocalCheckBoxMouseDown
      end
      object RunRemoteCheckBox: TCheckBox
        Left = 215
        Top = 17
        Width = 134
        Height = 17
        ParentCustomHint = False
        Alignment = taLeftJustify
        BiDiMode = bdLeftToRight
        Caption = 'Remote State Check'
        Color = clBtnFace
        Ctl3D = True
        DoubleBuffered = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentBiDiMode = False
        ParentColor = False
        ParentCtl3D = False
        ParentDoubleBuffered = False
        ParentFont = False
        ParentShowHint = False
        ShowHint = False
        TabOrder = 13
      end
    end
    object AdvPanel4: TAdvPanel
      Left = 0
      Top = 641
      Width = 504
      Height = 58
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 6
      UseDockManager = True
      Version = '2.6.3.2'
      BorderColor = clSilver
      BorderWidth = 1
      Caption.Color = clSkyBlue
      Caption.ColorTo = clNone
      Caption.Font.Charset = DEFAULT_CHARSET
      Caption.Font.Color = clWindowText
      Caption.Font.Height = -13
      Caption.Font.Name = 'Tahoma'
      Caption.Font.Style = [fsBold]
      Caption.Height = 18
      Caption.Indent = 0
      Caption.Text = '<FONT  size="10"  face="Tahoma"><B>Screen Saver</B></FONT>'
      Caption.Visible = True
      DoubleBuffered = True
      StatusBar.Font.Charset = DEFAULT_CHARSET
      StatusBar.Font.Color = clWindowText
      StatusBar.Font.Height = -11
      StatusBar.Font.Name = 'Tahoma'
      StatusBar.Font.Style = []
      Text = ''
      FullHeight = 60
      object UseScreenSaverCheckBox: TCheckBox
        Left = 355
        Top = 16
        Width = 135
        Height = 17
        ParentCustomHint = False
        Alignment = taLeftJustify
        BiDiMode = bdLeftToRight
        Caption = 'Use ScreenSaver'
        Color = clBtnFace
        Ctl3D = True
        DoubleBuffered = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentBiDiMode = False
        ParentColor = False
        ParentCtl3D = False
        ParentDoubleBuffered = False
        ParentFont = False
        ParentShowHint = False
        ShowHint = False
        TabOrder = 0
        OnClick = UseScreenSaverCheckBoxClick
      end
      object ScreenSaverCheckTime: TAdvEdit
        Left = 280
        Top = 33
        Width = 210
        Height = 20
        EmptyTextStyle = []
        LabelCaption = 'ScreenSaver Check Time (.sec)'
        LabelPosition = lpLeftCenter
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        Lookup.Font.Charset = DEFAULT_CHARSET
        Lookup.Font.Color = clWindowText
        Lookup.Font.Height = -33
        Lookup.Font.Name = 'Arial'
        Lookup.Font.Style = []
        Lookup.Separator = ';'
        AutoSize = False
        Color = clWindow
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft IME 2010'
        ParentFont = False
        TabOrder = 1
        Text = ''
        Visible = True
        Version = '4.0.3.6'
      end
    end
    object TowerLampSettingButton: TAdvGlassButton
      Left = 532
      Top = 634
      Width = 197
      Height = 55
      BackColor = 12972990
      Caption = 'Tower Lamp'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clWhite
      GlowColor = 16760205
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clBlack
      Picture.Data = {
        424D360800000000000036040000280000002000000020000000010008000000
        0000000400000000000000000000000000000000000000000000000080000080
        000000808000800000008000800080800000C0C0C000C0DCC000F0CAA6000020
        400000206000002080000020A0000020C0000020E00000400000004020000040
        400000406000004080000040A0000040C0000040E00000600000006020000060
        400000606000006080000060A0000060C0000060E00000800000008020000080
        400000806000008080000080A0000080C0000080E00000A0000000A0200000A0
        400000A0600000A0800000A0A00000A0C00000A0E00000C0000000C0200000C0
        400000C0600000C0800000C0A00000C0C00000C0E00000E0000000E0200000E0
        400000E0600000E0800000E0A00000E0C00000E0E00040000000400020004000
        400040006000400080004000A0004000C0004000E00040200000402020004020
        400040206000402080004020A0004020C0004020E00040400000404020004040
        400040406000404080004040A0004040C0004040E00040600000406020004060
        400040606000406080004060A0004060C0004060E00040800000408020004080
        400040806000408080004080A0004080C0004080E00040A0000040A0200040A0
        400040A0600040A0800040A0A00040A0C00040A0E00040C0000040C0200040C0
        400040C0600040C0800040C0A00040C0C00040C0E00040E0000040E0200040E0
        400040E0600040E0800040E0A00040E0C00040E0E00080000000800020008000
        400080006000800080008000A0008000C0008000E00080200000802020008020
        400080206000802080008020A0008020C0008020E00080400000804020008040
        400080406000804080008040A0008040C0008040E00080600000806020008060
        400080606000806080008060A0008060C0008060E00080800000808020008080
        400080806000808080008080A0008080C0008080E00080A0000080A0200080A0
        400080A0600080A0800080A0A00080A0C00080A0E00080C0000080C0200080C0
        400080C0600080C0800080C0A00080C0C00080C0E00080E0000080E0200080E0
        400080E0600080E0800080E0A00080E0C00080E0E000C0000000C0002000C000
        4000C0006000C0008000C000A000C000C000C000E000C0200000C0202000C020
        4000C0206000C0208000C020A000C020C000C020E000C0400000C0402000C040
        4000C0406000C0408000C040A000C040C000C040E000C0600000C0602000C060
        4000C0606000C0608000C060A000C060C000C060E000C0800000C0802000C080
        4000C0806000C0808000C080A000C080C000C080E000C0A00000C0A02000C0A0
        4000C0A06000C0A08000C0A0A000C0A0C000C0A0E000C0C00000C0C02000C0C0
        4000C0C06000C0C08000C0C0A000F0FBFF00A4A0A000808080000000FF0000FF
        000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFA4A4A4A4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFA4FFFFA4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFA4FFFFA4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFA4FFFFA4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFA4FFFFA4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA4A4
        A4A4A4A4A4A4A4A4A4A4A4A4A4A4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA471
        71717171717171717171717171A4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA471
        71717171717171717171717171A4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA471
        71717171717171717171717171A4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA471
        71717171717171717171717171A4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA471
        71717171717171717171717171A4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA471
        71717171717171717171717171A4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA4A4
        A4A4A4A4A4A4A4A4A4A4A4A4A4A4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA4FB
        FBFBFBFBFBFBFBFBFBFBFBFBFBA4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA4FB
        FBFBFBFBFBFBFBFBFBFBFBFBFBA4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA4FB
        FBFBFBFBFBFBFBFBFBFBFBFBFBA4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA4FB
        FBFBFBFBFBFBFBFBFBFBFBFBFBA4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA4FB
        FBFBFBFBFBFBFBFBFBFBFBFBFBA4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA4FB
        FBFBFBFBFBFBFBFBFBFBFBFBFBA4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA4A4
        A4A4A4A4A4A4A4A4A4A4A4A4A4A4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA44F
        4F4F4F4F4F4F4F4F4F4F4F4F4FA4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA44F
        4F4F4F4F4F4F4F4F4F4F4F4F4FA4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA44F
        4F4F4F4F4F4F4F4F4F4F4F4F4FA4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA44F
        4F4F4F4F4F4F4F4F4F4F4F4F4FA4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA44F
        4F4F4F4F4F4F4F4F4F4F4F4F4FA4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA44F
        4F4F4F4F4F4F4F4F4F4F4F4F4FA4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA4A4
        A4A4A4A4A4A4A4A4A4A4A4A4A4A4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
      ParentFont = False
      ShineColor = clWhite
      TabOrder = 7
      Version = '1.3.3.0'
      OnClick = TowerLampSettingButtonClick
    end
    object EtcPanel: TAdvPanel
      Left = 505
      Top = 559
      Width = 501
      Height = 58
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 8
      UseDockManager = True
      Version = '2.6.3.2'
      BorderColor = clSilver
      BorderWidth = 1
      Caption.Color = clSkyBlue
      Caption.ColorTo = clNone
      Caption.Font.Charset = DEFAULT_CHARSET
      Caption.Font.Color = clWindowText
      Caption.Font.Height = -13
      Caption.Font.Name = 'Tahoma'
      Caption.Font.Style = [fsBold]
      Caption.GradientDirection = gdVertical
      Caption.Height = 18
      Caption.Indent = 0
      Caption.ShadeLight = 255
      Caption.Text = '<FONT  size="10"  face="Tahoma"><B>ETC</B></FONT>'
      Caption.Visible = True
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
      Text = ''
      FullHeight = 60
      object UseCsvDataSaveCheckBox: TCheckBox
        Left = 355
        Top = 16
        Width = 135
        Height = 17
        ParentCustomHint = False
        Alignment = taLeftJustify
        BiDiMode = bdLeftToRight
        Caption = 'Use CSV Data Save'
        Color = clBtnFace
        Ctl3D = True
        DoubleBuffered = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentBiDiMode = False
        ParentColor = False
        ParentCtl3D = False
        ParentDoubleBuffered = False
        ParentFont = False
        ParentShowHint = False
        ShowHint = False
        TabOrder = 0
        OnClick = UseCsvDataSaveCheckBoxClick
      end
      object MachineIdEdit: TAdvEdit
        Left = 280
        Top = 33
        Width = 210
        Height = 20
        EmptyTextStyle = []
        FlatLineColor = 11250603
        FocusColor = clWindow
        FocusFontColor = 3881787
        LabelCaption = 'Machine ID'
        LabelPosition = lpLeftCenter
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = [fsBold]
        Lookup.Font.Charset = DEFAULT_CHARSET
        Lookup.Font.Color = clWindowText
        Lookup.Font.Height = -11
        Lookup.Font.Name = 'Tahoma'
        Lookup.Font.Style = []
        Lookup.Separator = ';'
        AutoSize = False
        Color = clWindow
        ImeName = 'Microsoft IME 2010'
        TabOrder = 1
        Text = ''
        Visible = True
        Version = '4.0.3.6'
      end
    end
  end
end
