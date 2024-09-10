object FrmChamber: TFrmChamber
  Left = 0
  Top = 0
  Width = 1069
  Height = 560
  TabOrder = 0
  DesignSize = (
    1069
    560)
  object ChamberPanel: TAdvPanel
    Left = 20
    Top = 37
    Width = 1024
    Height = 396
    Anchors = []
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -53
    Font.Name = 'Tahoma'
    Font.Style = []
    Locked = True
    ParentFont = False
    TabOrder = 0
    UseDockManager = True
    Version = '2.7.0.0'
    BorderColor = clBlack
    Caption.Color = clTeal
    Caption.ColorTo = clTeal
    Caption.Flat = True
    Caption.Font.Charset = DEFAULT_CHARSET
    Caption.Font.Color = clYellow
    Caption.Font.Height = -29
    Caption.Font.Name = 'Tahoma'
    Caption.Font.Style = [fsBold]
    Caption.Height = 36
    Caption.Indent = 20
    Caption.ShadeGrain = 34
    Caption.ShadeType = stHShade
    Caption.Text = '<P align="center">CHAMBER A</P>'
    Caption.Visible = True
    ColorTo = clBlack
    DoubleBuffered = True
    FreeOnClose = True
    GradientDirection = gdHorizontal
    StatusBar.Font.Charset = DEFAULT_CHARSET
    StatusBar.Font.Color = clWindowText
    StatusBar.Font.Height = -11
    StatusBar.Font.Name = 'Tahoma'
    StatusBar.Font.Style = []
    Text = '<P align="center"><B></B></P>'
    TextVAlign = tvaCenter
    DesignSize = (
      1024
      396)
    FullHeight = 353
    object CommStatusPanel: TAdvPanel
      Left = 5
      Top = 6
      Width = 356
      Height = 32
      Anchors = []
      Color = clBackground
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -53
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      UseDockManager = True
      Version = '2.7.0.0'
      BorderColor = clBlack
      Caption.Color = clHighlight
      Caption.ColorTo = clNone
      Caption.Font.Charset = DEFAULT_CHARSET
      Caption.Font.Color = clWindowText
      Caption.Font.Height = -53
      Caption.Font.Name = 'Tahoma'
      Caption.Font.Style = []
      Caption.Indent = 0
      DoubleBuffered = True
      StatusBar.Font.Charset = DEFAULT_CHARSET
      StatusBar.Font.Color = clWindowText
      StatusBar.Font.Height = -11
      StatusBar.Font.Name = 'Tahoma'
      StatusBar.Font.Style = []
      Text = ''
      FullHeight = 32
      object MainTempIndicator: TAdvSmoothStatusIndicator
        Left = 1
        Top = 6
        Width = 45
        Height = 24
        Caption = 'Temp.'
        Version = '1.0.1.1'
        Appearance.Fill.Color = clNavy
        Appearance.Fill.ColorMirror = clNone
        Appearance.Fill.ColorMirrorTo = clNone
        Appearance.Fill.GradientType = gtSolid
        Appearance.Fill.GradientMirrorType = gtSolid
        Appearance.Fill.BorderColor = clNavy
        Appearance.Fill.Rounding = 12
        Appearance.Fill.ShadowOffset = 0
        Appearance.Fill.Glow = gmNone
        Appearance.Font.Charset = DEFAULT_CHARSET
        Appearance.Font.Color = clWhite
        Appearance.Font.Height = -11
        Appearance.Font.Name = 'Tahoma'
        Appearance.Font.Style = [fsBold]
        Visible = False
      end
      object TempLimitIndicator: TAdvSmoothStatusIndicator
        Left = 52
        Top = 6
        Width = 45
        Height = 24
        Caption = 'T. Limit'
        Version = '1.0.1.1'
        Appearance.Fill.Color = clNavy
        Appearance.Fill.ColorMirror = clNone
        Appearance.Fill.ColorMirrorTo = clNone
        Appearance.Fill.GradientType = gtSolid
        Appearance.Fill.GradientMirrorType = gtSolid
        Appearance.Fill.BorderColor = clNavy
        Appearance.Fill.Rounding = 12
        Appearance.Fill.ShadowOffset = 0
        Appearance.Fill.Glow = gmNone
        Appearance.Font.Charset = DEFAULT_CHARSET
        Appearance.Font.Color = clWhite
        Appearance.Font.Height = -11
        Appearance.Font.Name = 'Tahoma'
        Appearance.Font.Style = [fsBold]
        Visible = False
      end
      object O2AnalyzerIndicator: TAdvSmoothStatusIndicator
        Left = 150
        Top = 5
        Width = 45
        Height = 24
        Caption = 'O2'
        Version = '1.0.1.1'
        Appearance.Fill.Color = clNavy
        Appearance.Fill.ColorMirror = clNone
        Appearance.Fill.ColorMirrorTo = clNone
        Appearance.Fill.GradientType = gtSolid
        Appearance.Fill.GradientMirrorType = gtSolid
        Appearance.Fill.BorderColor = clNavy
        Appearance.Fill.Rounding = 12
        Appearance.Fill.ShadowOffset = 0
        Appearance.Fill.Glow = gmNone
        Appearance.Font.Charset = DEFAULT_CHARSET
        Appearance.Font.Color = clWhite
        Appearance.Font.Height = -11
        Appearance.Font.Name = 'Tahoma'
        Appearance.Font.Style = [fsBold]
        Visible = False
      end
      object LoggerIndicator: TAdvSmoothStatusIndicator
        Left = 103
        Top = 5
        Width = 45
        Height = 24
        Caption = 'Logger'
        Version = '1.0.1.1'
        Appearance.Fill.Color = clNavy
        Appearance.Fill.ColorMirror = clNone
        Appearance.Fill.ColorMirrorTo = clNone
        Appearance.Fill.GradientType = gtSolid
        Appearance.Fill.GradientMirrorType = gtSolid
        Appearance.Fill.BorderColor = clNavy
        Appearance.Fill.Rounding = 12
        Appearance.Fill.ShadowOffset = 0
        Appearance.Fill.Glow = gmNone
        Appearance.Font.Charset = DEFAULT_CHARSET
        Appearance.Font.Color = clWhite
        Appearance.Font.Height = -11
        Appearance.Font.Name = 'Tahoma'
        Appearance.Font.Style = [fsBold]
        Visible = False
      end
      object DIOIndicator: TAdvSmoothStatusIndicator
        Left = 201
        Top = 5
        Width = 45
        Height = 24
        Caption = 'DIO'
        Version = '1.0.1.1'
        Appearance.Fill.Color = clNavy
        Appearance.Fill.ColorMirror = clNone
        Appearance.Fill.ColorMirrorTo = clNone
        Appearance.Fill.GradientType = gtSolid
        Appearance.Fill.GradientMirrorType = gtSolid
        Appearance.Fill.BorderColor = clNavy
        Appearance.Fill.Rounding = 12
        Appearance.Fill.ShadowOffset = 0
        Appearance.Fill.Glow = gmNone
        Appearance.Font.Charset = DEFAULT_CHARSET
        Appearance.Font.Color = clWhite
        Appearance.Font.Height = -11
        Appearance.Font.Name = 'Tahoma'
        Appearance.Font.Style = [fsBold]
        Visible = False
      end
      object DPIndicator: TAdvSmoothStatusIndicator
        Left = 303
        Top = 5
        Width = 45
        Height = 24
        Caption = 'DP'
        Version = '1.0.1.1'
        Appearance.Fill.Color = clNavy
        Appearance.Fill.ColorMirror = clNone
        Appearance.Fill.ColorMirrorTo = clNone
        Appearance.Fill.GradientType = gtSolid
        Appearance.Fill.GradientMirrorType = gtSolid
        Appearance.Fill.BorderColor = clNavy
        Appearance.Fill.Rounding = 12
        Appearance.Fill.ShadowOffset = 0
        Appearance.Fill.Glow = gmNone
        Appearance.Font.Charset = DEFAULT_CHARSET
        Appearance.Font.Color = clWhite
        Appearance.Font.Height = -11
        Appearance.Font.Name = 'Tahoma'
        Appearance.Font.Style = [fsBold]
        Visible = False
      end
      object DamperMotorIndicator: TAdvSmoothStatusIndicator
        Left = 252
        Top = 5
        Width = 45
        Height = 24
        Caption = 'Motor'
        Version = '1.0.1.1'
        Appearance.Fill.Color = clNavy
        Appearance.Fill.ColorMirror = clNone
        Appearance.Fill.ColorMirrorTo = clNone
        Appearance.Fill.GradientType = gtSolid
        Appearance.Fill.GradientMirrorType = gtSolid
        Appearance.Fill.BorderColor = clNavy
        Appearance.Fill.Rounding = 12
        Appearance.Fill.ShadowOffset = 0
        Appearance.Fill.Glow = gmNone
        Appearance.Font.Charset = DEFAULT_CHARSET
        Appearance.Font.Color = clWhite
        Appearance.Font.Height = -11
        Appearance.Font.Name = 'Tahoma'
        Appearance.Font.Style = [fsBold]
        Visible = False
      end
      object ProgressBar: TAdvSmoothProgressBar
        Left = 148
        Top = 21
        Width = 150
        Height = 17
        Step = 10.000000000000000000
        Maximum = 100.000000000000000000
        Appearance.Shadows = False
        Appearance.BackGroundFill.Color = 16765615
        Appearance.BackGroundFill.ColorTo = 16765615
        Appearance.BackGroundFill.ColorMirror = clNone
        Appearance.BackGroundFill.ColorMirrorTo = clNone
        Appearance.BackGroundFill.GradientType = gtVertical
        Appearance.BackGroundFill.GradientMirrorType = gtSolid
        Appearance.BackGroundFill.BorderColor = clSilver
        Appearance.BackGroundFill.Rounding = 0
        Appearance.BackGroundFill.ShadowOffset = 0
        Appearance.BackGroundFill.Glow = gmNone
        Appearance.ProgressFill.Color = clBlue
        Appearance.ProgressFill.ColorTo = clAqua
        Appearance.ProgressFill.ColorMirror = clAqua
        Appearance.ProgressFill.ColorMirrorTo = clBlue
        Appearance.ProgressFill.GradientType = gtVertical
        Appearance.ProgressFill.GradientMirrorType = gtVertical
        Appearance.ProgressFill.BorderColor = 16765357
        Appearance.ProgressFill.Rounding = 0
        Appearance.ProgressFill.ShadowOffset = 0
        Appearance.ProgressFill.Glow = gmNone
        Appearance.Font.Charset = DEFAULT_CHARSET
        Appearance.Font.Color = clNavy
        Appearance.Font.Height = -13
        Appearance.Font.Name = 'Tahoma'
        Appearance.Font.Style = [fsBold]
        Appearance.ProgressFont.Charset = DEFAULT_CHARSET
        Appearance.ProgressFont.Color = clRed
        Appearance.ProgressFont.Height = -13
        Appearance.ProgressFont.Name = 'Tahoma'
        Appearance.ProgressFont.Style = [fsBold]
        Appearance.ValueFormat = '%.0f%%'
        Appearance.ValueType = vtPercentage
        Appearance.ValueVisible = True
        Version = '1.10.1.2'
        Visible = False
        TMSStyle = 0
      end
    end
    object BuzzerOffButton: TAdvGlassButton
      Left = 921
      Top = 1
      Width = 100
      Height = 32
      BackColor = clNavy
      BackGroundSymbolColor = clLime
      Caption = 'buzzer off'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clBlue
      GlowColor = 16760205
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clLime
      ParentFont = False
      ShineColor = clWhite
      TabOrder = 1
      Version = '1.3.3.1'
      Visible = False
      OnClick = BuzzerOffButtonClick
    end
    object ShowAlarmButton: TAdvGlassButton
      Left = 819
      Top = 1
      Width = 100
      Height = 32
      BackColor = clRed
      BackGroundSymbolColor = clLime
      Caption = 'show alarm'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clRed
      GlowColor = clRed
      ImageIndex = -1
      InnerBorderColor = clBlack
      OuterBorderColor = clLime
      ParentFont = False
      ShineColor = clRed
      TabOrder = 2
      Version = '1.3.3.1'
      Visible = False
      OnClick = ShowAlarmButtonClick
    end
    object MachineStatusPanel: TAdvPanel
      Left = 808
      Top = 39
      Width = 220
      Height = 114
      Color = 3289650
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -53
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      UseDockManager = True
      Version = '2.7.0.0'
      BorderColor = clBlack
      Caption.Color = clHighlight
      Caption.ColorTo = clNone
      Caption.Font.Charset = DEFAULT_CHARSET
      Caption.Font.Color = clHighlightText
      Caption.Font.Height = -16
      Caption.Font.Name = 'Tahoma'
      Caption.Font.Style = [fsBold]
      Caption.Height = 24
      Caption.Indent = 0
      Caption.Text = '<P align="center"><B>Machine Status</B></P>'
      Caption.Visible = True
      DoubleBuffered = True
      StatusBar.Font.Charset = DEFAULT_CHARSET
      StatusBar.Font.Color = clWindowText
      StatusBar.Font.Height = -11
      StatusBar.Font.Name = 'Tahoma'
      StatusBar.Font.Style = []
      Text = ''
      FullHeight = 200
      object O2AnalyzerStatus: TLabeledEdit
        Left = 8
        Top = 47
        Width = 25
        Height = 20
        AutoSelect = False
        AutoSize = False
        BevelEdges = []
        BevelInner = bvNone
        Color = clRed
        Ctl3D = False
        EditLabel.Width = 15
        EditLabel.Height = 11
        EditLabel.Caption = 'O2'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clHighlightText
        EditLabel.Font.Height = -9
        EditLabel.Font.Name = 'Tahoma'
        EditLabel.Font.Style = [fsBold]
        EditLabel.ParentFont = False
        EditLabel.Transparent = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clHighlightText
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpRight
        ParentCtl3D = False
        ParentFont = False
        ReadOnly = True
        TabOrder = 0
      end
      object DoorStatus: TLabeledEdit
        Left = 8
        Top = 25
        Width = 25
        Height = 20
        AutoSelect = False
        AutoSize = False
        BevelEdges = []
        BevelInner = bvNone
        Color = clRed
        Ctl3D = False
        EditLabel.Width = 65
        EditLabel.Height = 11
        EditLabel.Caption = 'Door Opened'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clHighlightText
        EditLabel.Font.Height = -9
        EditLabel.Font.Name = 'Tahoma'
        EditLabel.Font.Style = [fsBold]
        EditLabel.ParentFont = False
        EditLabel.Transparent = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clHighlightText
        Font.Height = -12
        Font.Name = 'Tahoma'
        Font.Style = []
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpRight
        ParentCtl3D = False
        ParentFont = False
        ReadOnly = True
        TabOrder = 1
      end
      object MagazineDetectorStatus: TLabeledEdit
        Left = 8
        Top = 69
        Width = 25
        Height = 20
        AutoSelect = False
        AutoSize = False
        BevelEdges = []
        BevelInner = bvNone
        Color = clRed
        Ctl3D = False
        EditLabel.Width = 47
        EditLabel.Height = 11
        EditLabel.Caption = 'Magazine'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clHighlightText
        EditLabel.Font.Height = -9
        EditLabel.Font.Name = 'Tahoma'
        EditLabel.Font.Style = [fsBold]
        EditLabel.ParentFont = False
        EditLabel.Transparent = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clHighlightText
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpRight
        ParentCtl3D = False
        ParentFont = False
        ReadOnly = True
        TabOrder = 2
      end
      object OnlineStatus: TLabeledEdit
        Left = 8
        Top = 91
        Width = 25
        Height = 20
        AutoSelect = False
        AutoSize = False
        BevelEdges = []
        BevelInner = bvNone
        Color = clRed
        Ctl3D = False
        EditLabel.Width = 47
        EditLabel.Height = 11
        EditLabel.Caption = 'Magazine'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clHighlightText
        EditLabel.Font.Height = -9
        EditLabel.Font.Name = 'Tahoma'
        EditLabel.Font.Style = [fsBold]
        EditLabel.ParentFont = False
        EditLabel.Transparent = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clHighlightText
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ImeName = 'Microsoft Office IME 2007'
        LabelPosition = lpRight
        ParentCtl3D = False
        ParentFont = False
        ReadOnly = True
        TabOrder = 3
        Visible = False
      end
    end
    object TempGraph: TChart
      Left = 2
      Top = 115
      Width = 580
      Height = 244
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      BackWall.Brush.Gradient.Direction = gdBottomTop
      BackWall.Brush.Gradient.EndColor = clWhite
      BackWall.Brush.Gradient.StartColor = 15395562
      BackWall.Brush.Gradient.Visible = True
      BackWall.Transparent = False
      Foot.Font.Color = clBlue
      Foot.Font.Name = 'Verdana'
      Gradient.Direction = gdBottomTop
      Gradient.EndColor = clWhite
      Gradient.MidColor = 15395562
      Gradient.StartColor = 15395562
      Gradient.Visible = True
      LeftWall.Color = 14745599
      Legend.Alignment = laBottom
      Legend.Brush.Gradient.SubGradient.EndColor = clSkyBlue
      Legend.CheckBoxes = True
      Legend.Font.Name = 'Verdana'
      Legend.Font.Style = [fsBold]
      Legend.FontSeriesColor = True
      Legend.Shadow.Transparency = 0
      Legend.Symbol.DefaultPen = False
      Legend.TextSymbolGap = 1
      Legend.Margins.Top = 0
      Legend.Margins.Units = maPercentSize
      MarginBottom = 2
      MarginLeft = 2
      MarginRight = 2
      MarginTop = 5
      RightWall.Color = 14745599
      Title.Font.Name = 'Verdana'
      Title.Text.Strings = (
        'TChart')
      Title.Visible = False
      BottomAxis.Automatic = False
      BottomAxis.AutomaticMaximum = False
      BottomAxis.AutomaticMinimum = False
      BottomAxis.Axis.Color = 4210752
      BottomAxis.DateTimeFormat = 'hh:mm'
      BottomAxis.ExactDateTime = False
      BottomAxis.Grid.Color = 11119017
      BottomAxis.LabelsFormat.Font.Name = 'Verdana'
      BottomAxis.LabelsFormat.Font.Style = [fsBold]
      BottomAxis.Maximum = 0.125000000000000000
      BottomAxis.TicksInner.Color = 11119017
      BottomAxis.Title.Caption = 'Time'
      BottomAxis.Title.Font.Color = clBlue
      BottomAxis.Title.Font.Name = 'Verdana'
      BottomAxis.Title.Font.Style = [fsBold]
      DepthAxis.Automatic = False
      DepthAxis.AutomaticMaximum = False
      DepthAxis.AutomaticMinimum = False
      DepthAxis.Axis.Color = 4210752
      DepthAxis.Grid.Color = 11119017
      DepthAxis.LabelsFormat.Font.Name = 'Verdana'
      DepthAxis.Maximum = 0.500000000000000000
      DepthAxis.Minimum = -0.500000000000000000
      DepthAxis.TicksInner.Color = 11119017
      DepthAxis.Title.Font.Name = 'Verdana'
      DepthTopAxis.Automatic = False
      DepthTopAxis.AutomaticMaximum = False
      DepthTopAxis.AutomaticMinimum = False
      DepthTopAxis.Axis.Color = 4210752
      DepthTopAxis.Grid.Color = 11119017
      DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
      DepthTopAxis.Maximum = 0.500000000000000000
      DepthTopAxis.Minimum = -0.500000000000000000
      DepthTopAxis.TicksInner.Color = 11119017
      DepthTopAxis.Title.Font.Name = 'Verdana'
      LeftAxis.Automatic = False
      LeftAxis.AutomaticMaximum = False
      LeftAxis.AutomaticMinimum = False
      LeftAxis.Axis.Color = 4210752
      LeftAxis.Grid.Color = 11119017
      LeftAxis.LabelsFormat.Font.Name = 'Verdana'
      LeftAxis.LabelsFormat.Font.Style = [fsBold]
      LeftAxis.LabelsSize = 20
      LeftAxis.Maximum = 300.000000000000000000
      LeftAxis.TicksInner.Color = 11119017
      LeftAxis.Title.Caption = 'Temp. ('#8451')'
      LeftAxis.Title.Font.Color = clBlue
      LeftAxis.Title.Font.Name = 'Verdana'
      LeftAxis.Title.Font.Style = [fsBold]
      LeftAxis.TitleSize = 10
      RightAxis.Automatic = False
      RightAxis.AutomaticMaximum = False
      RightAxis.AutomaticMinimum = False
      RightAxis.Axis.Color = 4210752
      RightAxis.ExactDateTime = False
      RightAxis.Grid.Color = 1166591
      RightAxis.Grid.ZZero = True
      RightAxis.Increment = 2.000000000000000000
      RightAxis.LabelsFormat.Font.Name = 'Verdana'
      RightAxis.LabelsFormat.Font.Style = [fsBold]
      RightAxis.LabelsSize = 40
      RightAxis.LogarithmicBase = 20.000000000000000000
      RightAxis.Maximum = 10.000000000000000000
      RightAxis.TicksInner.Color = 11119017
      RightAxis.Title.Caption = 'Vac. (Torr)'
      RightAxis.Title.Font.Color = clBlue
      RightAxis.Title.Font.Name = 'Verdana'
      RightAxis.Title.Font.Style = [fsBold]
      RightAxis.TitleSize = 12
      TopAxis.Automatic = False
      TopAxis.AutomaticMaximum = False
      TopAxis.AutomaticMinimum = False
      TopAxis.Axis.Color = 4210752
      TopAxis.Grid.Color = 11119017
      TopAxis.LabelsFormat.Font.Name = 'Verdana'
      TopAxis.TicksInner.Color = 11119017
      TopAxis.Title.Font.Name = 'Verdana'
      TopAxis.Visible = False
      View3D = False
      BevelOuter = bvNone
      TabOrder = 4
      OnDblClick = TempGraphDblClick
      DefaultCanvas = ''
      PrintMargins = (
        15
        27
        15
        27)
      ColorPaletteIndex = 0
    end
    object ChamberStatusPanel: TAdvPanel
      Left = 1
      Top = 47
      Width = 145
      Height = 76
      Anchors = []
      Color = clCream
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -32
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      Locked = True
      ParentFont = False
      TabOrder = 5
      UseDockManager = True
      OnDblClick = ChamberStatusPanelDblClick
      Version = '2.7.0.0'
      BorderColor = clBlack
      Caption.Color = clHighlight
      Caption.ColorTo = clNone
      Caption.Font.Charset = DEFAULT_CHARSET
      Caption.Font.Color = clWhite
      Caption.Font.Height = -16
      Caption.Font.Name = 'Tahoma'
      Caption.Font.Style = [fsBold]
      Caption.Height = 24
      Caption.Indent = 0
      Caption.Text = '<P align="center">'#52308#48260' '#49345#53468'</P>'
      Caption.Visible = True
      DoubleBuffered = True
      StatusBar.Font.Charset = DEFAULT_CHARSET
      StatusBar.Font.Color = clWhite
      StatusBar.Font.Height = -9
      StatusBar.Font.Name = 'Tahoma'
      StatusBar.Font.Style = [fsBold]
      StatusBar.Text = 'Mode'
      StatusBar.Color = clBlack
      StatusBar.Height = 12
      StatusBar.Visible = True
      Text = '<P align="center"><B>IDLE</B></P>'
      FullHeight = 100
    end
    object RemainTimePanel: TAdvPanel
      Left = 147
      Top = 47
      Width = 145
      Height = 76
      Anchors = []
      Color = clCream
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -32
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      Locked = True
      ParentFont = False
      TabOrder = 6
      UseDockManager = True
      Version = '2.7.0.0'
      BorderColor = clBlack
      Caption.Color = clHighlight
      Caption.ColorTo = clNone
      Caption.Font.Charset = DEFAULT_CHARSET
      Caption.Font.Color = clWhite
      Caption.Font.Height = -16
      Caption.Font.Name = 'Tahoma'
      Caption.Font.Style = [fsBold]
      Caption.Height = 24
      Caption.Indent = 0
      Caption.Text = '<P align="center">'#51092#50668' '#49884#44036'</P>'
      Caption.Visible = True
      DoubleBuffered = True
      StatusBar.Font.Charset = DEFAULT_CHARSET
      StatusBar.Font.Color = clWhite
      StatusBar.Font.Height = -9
      StatusBar.Font.Name = 'Tahoma'
      StatusBar.Font.Style = [fsBold]
      StatusBar.Text = 'Mode'
      StatusBar.Color = clBlack
      StatusBar.Height = 12
      StatusBar.Visible = True
      Text = '<P align="center"><B>00:00</B></P>'
      FullHeight = 100
    end
    object ChamberTempPanel: TAdvPanel
      Left = 292
      Top = 47
      Width = 175
      Height = 76
      Anchors = []
      Color = clCream
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -32
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      Locked = True
      ParentFont = False
      TabOrder = 7
      UseDockManager = True
      Version = '2.7.0.0'
      BorderColor = clBlack
      Caption.Color = clHighlight
      Caption.ColorTo = clNone
      Caption.Font.Charset = DEFAULT_CHARSET
      Caption.Font.Color = clWhite
      Caption.Font.Height = -16
      Caption.Font.Name = 'Tahoma'
      Caption.Font.Style = [fsBold]
      Caption.Height = 24
      Caption.Indent = 0
      Caption.Text = '<P align="center">'#52308#48260' '#50728#46020'</P>'
      Caption.Visible = True
      DoubleBuffered = True
      StatusBar.Font.Charset = DEFAULT_CHARSET
      StatusBar.Font.Color = clWhite
      StatusBar.Font.Height = -9
      StatusBar.Font.Name = 'Tahoma'
      StatusBar.Font.Style = [fsBold]
      StatusBar.Text = 'Mode'
      StatusBar.Color = clBlack
      StatusBar.Height = 12
      StatusBar.Visible = True
      Text = '<P align="center"><B>175.0</B></P>'
      FullHeight = 100
    end
    object DPPanel: TAdvPanel
      Left = 467
      Top = 47
      Width = 115
      Height = 76
      Anchors = []
      Color = clCream
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -32
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      Locked = True
      ParentFont = False
      TabOrder = 8
      UseDockManager = True
      Version = '2.7.0.0'
      BorderColor = clBlack
      Caption.Color = clHighlight
      Caption.ColorTo = clNone
      Caption.Font.Charset = DEFAULT_CHARSET
      Caption.Font.Color = clWhite
      Caption.Font.Height = -16
      Caption.Font.Name = 'Tahoma'
      Caption.Font.Style = [fsBold]
      Caption.Height = 24
      Caption.Indent = 0
      Caption.Text = '<P align="center">Diff. Press</P>'
      Caption.Visible = True
      DoubleBuffered = True
      StatusBar.Font.Charset = DEFAULT_CHARSET
      StatusBar.Font.Color = clWhite
      StatusBar.Font.Height = -9
      StatusBar.Font.Name = 'Tahoma'
      StatusBar.Font.Style = [fsBold]
      StatusBar.Color = clBlack
      StatusBar.Height = 12
      StatusBar.Visible = True
      Text = '<P align="center"><B>28.7</B></P>'
      FullHeight = 100
    end
    object RecipePanel: TAdvPanel
      Left = 582
      Top = 215
      Width = 220
      Height = 175
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -53
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 9
      UseDockManager = True
      Version = '2.7.0.0'
      BorderColor = clBlack
      Caption.Color = clHighlight
      Caption.ColorTo = clNone
      Caption.Font.Charset = DEFAULT_CHARSET
      Caption.Font.Color = clWhite
      Caption.Font.Height = -16
      Caption.Font.Name = 'Tahoma'
      Caption.Font.Style = [fsBold]
      Caption.Height = 24
      Caption.Indent = 0
      Caption.Text = '<P align="center">'#47112#49884#54588'</P>'
      Caption.Visible = True
      DoubleBuffered = True
      StatusBar.Font.Charset = DEFAULT_CHARSET
      StatusBar.Font.Color = clWindowText
      StatusBar.Font.Height = -11
      StatusBar.Font.Name = 'Tahoma'
      StatusBar.Font.Style = []
      Text = ''
      FullHeight = 200
      object RecipeInfoGrid: TAdvStringGrid
        Left = 3
        Top = 57
        Width = 215
        Height = 124
        BevelOuter = bvNone
        ColCount = 2
        DefaultColWidth = 112
        DefaultRowHeight = 24
        DrawingStyle = gdsClassic
        RowCount = 5
        FixedRows = 0
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
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
        ControlLook.DropDownFooter.Font.Name = 'Tahoma'
        ControlLook.DropDownFooter.Font.Style = []
        ControlLook.DropDownFooter.Visible = True
        ControlLook.DropDownFooter.Buttons = <>
        ControlLook.ToggleSwitch.BackgroundBorderWidth = 1.000000000000000000
        ControlLook.ToggleSwitch.ButtonBorderWidth = 1.000000000000000000
        ControlLook.ToggleSwitch.CaptionFont.Charset = DEFAULT_CHARSET
        ControlLook.ToggleSwitch.CaptionFont.Color = clWindowText
        ControlLook.ToggleSwitch.CaptionFont.Height = -11
        ControlLook.ToggleSwitch.CaptionFont.Name = 'Tahoma'
        ControlLook.ToggleSwitch.CaptionFont.Style = []
        ControlLook.ToggleSwitch.Shadow = False
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
        FixedRightCols = 1
        FixedColWidth = 112
        FixedRowHeight = 24
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
        Version = '9.1.1.0'
        RowHeights = (
          24
          24
          24
          24
          24)
      end
      object RecipeComboBox: TAdvComboBox
        Left = 79
        Top = 24
        Width = 138
        Height = 27
        Color = clWindow
        Version = '2.0.0.8'
        Visible = True
        ButtonWidth = 23
        EmptyTextStyle = []
        DropWidth = 0
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ImeName = 'Microsoft Office IME 2007'
        ItemIndex = -1
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -14
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = []
        ParentFont = False
        TabOrder = 1
        OnChange = RecipeComboBoxChange
      end
      object SelectButton: TAdvGlassButton
        Left = 2
        Top = 23
        Width = 76
        Height = 29
        BackColor = clLime
        Caption = 'SELECT'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ForeColor = clWhite
        GlowColor = 16760205
        ImageIndex = -1
        InnerBorderColor = clBlack
        OuterBorderColor = clWhite
        ParentFont = False
        ShineColor = clWhite
        TabOrder = 2
        Version = '1.3.3.1'
        OnClick = SelectButtonClick
      end
    end
    object PatternGraph: TChart
      Left = 666
      Top = 69
      Width = 220
      Height = 149
      AllowPanning = pmHorizontal
      BackWall.Brush.Gradient.Direction = gdBottomTop
      BackWall.Brush.Gradient.EndColor = clWhite
      BackWall.Brush.Gradient.StartColor = 15395562
      BackWall.Brush.Gradient.Visible = True
      BackWall.Color = clBlack
      BackWall.Dark3D = False
      BackWall.Size = 5
      BackWall.Transparent = False
      BottomWall.Dark3D = False
      BottomWall.Size = 5
      BottomWall.Visible = False
      Foot.Font.Color = clBlue
      Foot.Font.Name = 'Verdana'
      Gradient.Direction = gdBottomTop
      Gradient.EndColor = clWhite
      Gradient.MidColor = 15395562
      Gradient.StartColor = 15395562
      Gradient.Visible = True
      LeftWall.Color = 14614266
      LeftWall.Dark3D = False
      LeftWall.Size = 5
      LeftWall.Visible = False
      Legend.Font.Name = 'Verdana'
      Legend.Shadow.Transparency = 0
      Legend.Visible = False
      MarginBottom = 5
      MarginLeft = 5
      MarginRight = 15
      MarginTop = 0
      MarginUnits = muPixels
      RightWall.Color = 14745599
      RightWall.Dark3D = False
      RightWall.Size = 5
      SubFoot.Font.Name = 'Verdana'
      SubTitle.Font.Name = 'Verdana'
      Title.Emboss.SmoothBlur = 34
      Title.Font.Name = 'Verdana'
      Title.Font.Style = [fsBold]
      Title.Font.Emboss.Smooth = False
      Title.Font.Shadow.Color = 12763842
      Title.Font.Shadow.Smooth = False
      Title.Font.Shadow.SmoothBlur = 7
      Title.Font.Shadow.Visible = False
      Title.Shadow.Visible = False
      Title.Text.Strings = (
        'Cure Pattern')
      BottomAxis.Automatic = False
      BottomAxis.AutomaticMaximum = False
      BottomAxis.AutomaticMinimum = False
      BottomAxis.Axis.Color = 4210752
      BottomAxis.ExactDateTime = False
      BottomAxis.Grid.Color = 11119017
      BottomAxis.Increment = 1.000000000000000000
      BottomAxis.LabelsFormat.Font.Color = 10485760
      BottomAxis.LabelsFormat.Font.Name = 'Verdana'
      BottomAxis.LabelsMultiLine = True
      BottomAxis.Maximum = 7.000000000000000000
      BottomAxis.MinorTicks.Visible = False
      BottomAxis.Ticks.Visible = False
      BottomAxis.TicksInner.Color = 11119017
      BottomAxis.TicksInner.Visible = False
      BottomAxis.Title.Font.Name = 'Verdana'
      ClipPoints = False
      DepthAxis.Axis.Color = 4210752
      DepthAxis.Grid.Color = 11119017
      DepthAxis.LabelsFormat.Font.Name = 'Verdana'
      DepthAxis.TicksInner.Color = 11119017
      DepthAxis.Title.Font.Name = 'Verdana'
      DepthTopAxis.Axis.Color = 4210752
      DepthTopAxis.Grid.Color = 11119017
      DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
      DepthTopAxis.TicksInner.Color = 11119017
      DepthTopAxis.Title.Font.Name = 'Verdana'
      Emboss.Smooth = False
      LeftAxis.Automatic = False
      LeftAxis.AutomaticMaximum = False
      LeftAxis.AutomaticMinimum = False
      LeftAxis.Axis.Color = 4210752
      LeftAxis.ExactDateTime = False
      LeftAxis.Grid.Color = 11119017
      LeftAxis.Increment = 100.000000000000000000
      LeftAxis.LabelsFormat.Font.Color = clRed
      LeftAxis.LabelsFormat.Font.Name = #47569#51008' '#44256#46357
      LeftAxis.LabelsFormat.Font.Style = [fsBold]
      LeftAxis.Maximum = 300.000000000000000000
      LeftAxis.MinorTicks.Visible = False
      LeftAxis.Ticks.Visible = False
      LeftAxis.TicksInner.Color = 11119017
      LeftAxis.TicksInner.Visible = False
      LeftAxis.Title.Font.Name = 'Verdana'
      RightAxis.Axis.Color = 4210752
      RightAxis.Grid.Color = 11119017
      RightAxis.LabelsFormat.Font.Name = 'Verdana'
      RightAxis.TicksInner.Color = 11119017
      RightAxis.Title.Font.Name = 'Verdana'
      Shadow.Smooth = False
      Shadow.Visible = False
      TopAxis.Axis.Color = 4210752
      TopAxis.Grid.Color = 11119017
      TopAxis.LabelsFormat.Font.Name = 'Verdana'
      TopAxis.TicksInner.Color = 11119017
      TopAxis.Title.Font.Name = 'Verdana'
      View3D = False
      View3DOptions.Orthogonal = False
      View3DOptions.ZoomText = ztNo
      Zoom.Allow = False
      BevelOuter = bvNone
      Color = clWhite
      Enabled = False
      TabOrder = 10
      Anchors = [akTop, akRight]
      DefaultCanvas = ''
      PrintMargins = (
        15
        29
        15
        29)
      ColorPaletteIndex = 4
      object LineSeries1: TLineSeries
        ColorEachLine = False
        SeriesColor = clRed
        Brush.BackColor = clDefault
        Dark3D = False
        LinePen.Width = 2
        Pointer.Brush.Gradient.EndColor = 10708548
        Pointer.Gradient.EndColor = 10708548
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
        object TSmoothingFunction
          CalcByValue = False
          Period = 1.000000000000000000
          Factor = 8
        end
      end
    end
    object CureStartButton: TAdvGlassButton
      Left = 808
      Top = 282
      Width = 205
      Height = 52
      Anchors = []
      BackColor = clBlue
      BackGroundSymbolColor = clLime
      Caption = #49884#51089
      CornerRadius = 2
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHighlightText
      Font.Height = -21
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clWhite
      GlowColor = clWhite
      ImageIndex = -1
      InnerBorderColor = clSkyBlue
      OuterBorderColor = clSkyBlue
      ParentFont = False
      ShineColor = clBlue
      TabOrder = 11
      Version = '1.3.3.1'
      OnClick = CureStartButtonClick
    end
    object CureStopButton: TAdvGlassButton
      Left = 808
      Top = 340
      Width = 205
      Height = 50
      Hint = 'Switch 2D / 3D'
      Anchors = []
      BackColor = clFuchsia
      BackGroundSymbolColor = clRed
      Caption = #51221#51648
      CornerRadius = 2
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHighlightText
      Font.Height = -21
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clRed
      GlowColor = clRed
      ImageIndex = -1
      InnerBorderColor = clSkyBlue
      OuterBorderColor = clSkyBlue
      ParentFont = False
      ShineColor = clFuchsia
      TabOrder = 12
      Version = '1.3.3.1'
      OnClick = CureStopButtonClick
    end
    object LotInfoButton: TAdvGlassButton
      Left = 808
      Top = 224
      Width = 205
      Height = 52
      Anchors = []
      BackColor = clLime
      BackGroundSymbolColor = clLime
      Caption = 'Lot Info'
      CornerRadius = 2
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clHighlightText
      Font.Height = -21
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ForeColor = clLime
      GlowColor = clLime
      ImageIndex = -1
      InnerBorderColor = clSkyBlue
      OuterBorderColor = clSkyBlue
      ParentFont = False
      ShineColor = clLime
      TabOrder = 13
      Version = '1.3.3.1'
      OnClick = LotInfoButtonClick
    end
    object UserIDEdit: TAdvEdit
      Left = 892
      Top = 183
      Width = 121
      Height = 31
      EmptyTextStyle = []
      LabelCaption = 'User ID'
      LabelPosition = lpLeftCenter
      LabelFont.Charset = DEFAULT_CHARSET
      LabelFont.Color = clHighlightText
      LabelFont.Height = -27
      LabelFont.Name = 'Tahoma'
      LabelFont.Style = []
      Lookup.Font.Charset = DEFAULT_CHARSET
      Lookup.Font.Color = clWindowText
      Lookup.Font.Height = -2691
      Lookup.Font.Name = 'Arial'
      Lookup.Font.Style = []
      Lookup.Separator = ';'
      Color = clWindow
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ImeName = 'Microsoft IME 2010'
      ParentFont = False
      TabOrder = 14
      Text = ''
      Visible = True
      OnKeyDown = UserIDEditKeyDown
      Version = '4.0.5.0'
    end
    object O2Panel: TAdvPanel
      Left = 585
      Top = 47
      Width = 115
      Height = 76
      Anchors = []
      Color = clCream
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -29
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      Locked = True
      ParentFont = False
      TabOrder = 15
      UseDockManager = True
      Version = '2.7.0.0'
      BorderColor = clBlack
      Caption.Color = clHighlight
      Caption.ColorTo = clNone
      Caption.Font.Charset = DEFAULT_CHARSET
      Caption.Font.Color = clWhite
      Caption.Font.Height = -16
      Caption.Font.Name = 'Tahoma'
      Caption.Font.Style = [fsBold]
      Caption.Height = 24
      Caption.Indent = 0
      Caption.Text = '<P align="center">O2 (ppm)</P>'
      Caption.Visible = True
      DoubleBuffered = True
      StatusBar.Font.Charset = DEFAULT_CHARSET
      StatusBar.Font.Color = clWhite
      StatusBar.Font.Height = -9
      StatusBar.Font.Name = 'Tahoma'
      StatusBar.Font.Style = [fsBold]
      StatusBar.Color = clBlack
      StatusBar.Height = 12
      StatusBar.Visible = True
      Text = '<P align="center"><B>28.7</B></P>'
      FullHeight = 100
    end
  end
  object LotInfoPanel: TAdvPanel
    Left = 376
    Top = 208
    Width = 220
    Height = 325
    BevelWidth = 2
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -53
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    UseDockManager = True
    Visible = False
    Version = '2.7.0.0'
    BorderColor = clBlack
    Caption.Color = clHighlight
    Caption.ColorTo = clNone
    Caption.Font.Charset = DEFAULT_CHARSET
    Caption.Font.Color = clWhite
    Caption.Font.Height = -16
    Caption.Font.Name = 'Tahoma'
    Caption.Font.Style = [fsBold]
    Caption.Height = 24
    Caption.Indent = 0
    Caption.Text = '<P align="center">LOT INFO</P>'
    Caption.Visible = True
    DoubleBuffered = True
    StatusBar.Font.Charset = DEFAULT_CHARSET
    StatusBar.Font.Color = clWindowText
    StatusBar.Font.Height = -11
    StatusBar.Font.Name = 'Tahoma'
    StatusBar.Font.Style = []
    Text = ''
    FullHeight = 325
    object LotInfoGrid: TAdvStringGrid
      Left = 1
      Top = 22
      Width = 233
      Height = 298
      ColCount = 2
      DefaultColWidth = 130
      DefaultRowHeight = 25
      DrawingStyle = gdsClassic
      RowCount = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goEditing]
      ParentFont = False
      TabOrder = 0
      OnGetCellColor = LotInfoGridGetCellColor
      OnGetAlignment = LotInfoGridGetAlignment
      OnCanEditCell = LotInfoGridCanEditCell
      OnGetEditorType = LotInfoGridGetEditorType
      OnEditCellDone = LotInfoGridEditCellDone
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
      ControlLook.ToggleSwitch.BackgroundBorderWidth = 1.000000000000000000
      ControlLook.ToggleSwitch.ButtonBorderWidth = 1.000000000000000000
      ControlLook.ToggleSwitch.CaptionFont.Charset = DEFAULT_CHARSET
      ControlLook.ToggleSwitch.CaptionFont.Color = clWindowText
      ControlLook.ToggleSwitch.CaptionFont.Height = -11
      ControlLook.ToggleSwitch.CaptionFont.Name = 'Tahoma'
      ControlLook.ToggleSwitch.CaptionFont.Style = []
      ControlLook.ToggleSwitch.Shadow = False
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
      Version = '9.1.1.0'
      ColWidths = (
        25
        130)
    end
  end
  object ChangeViewModeButton: TAdvSmoothButton
    Left = 880
    Top = 469
    Width = 120
    Height = 35
    Appearance.GlowPercentage = 0
    Appearance.Font.Charset = DEFAULT_CHARSET
    Appearance.Font.Color = clHighlightText
    Appearance.Font.Height = -12
    Appearance.Font.Name = 'Tahoma'
    Appearance.Font.Style = [fsBold]
    Appearance.ShiftDown = 0
    Appearance.SimpleLayout = False
    Appearance.Rounding = 0
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
    Bevel = False
    Caption = 'show full'
    Color = clGreen
    ParentFont = False
    TabOrder = 2
    Version = '2.2.3.1'
    OnClick = ChangeViewModeButtonClick
    TMSStyle = 8
  end
  object LocalButton: TAdvGlassButton
    Left = 681
    Top = 439
    Width = 205
    Height = 52
    Anchors = []
    BackColor = clAqua
    BackGroundSymbolColor = clLime
    Caption = 'LOCAL'
    CornerRadius = 2
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clHighlightText
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ForeColor = clWhite
    GlowColor = clWhite
    ImageIndex = -1
    InnerBorderColor = clSkyBlue
    OuterBorderColor = clSkyBlue
    ParentFont = False
    ShineColor = clAqua
    TabOrder = 3
    Version = '1.3.3.1'
    OnClick = LocalButtonClick
  end
  object CimStatusPanel: TAdvPanel
    AlignWithMargins = True
    Left = 716
    Top = 1
    Width = 307
    Height = 54
    Margins.Left = 0
    Margins.Top = 0
    Margins.Right = 0
    Margins.Bottom = 0
    Color = clCream
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    UseDockManager = True
    Version = '2.7.0.0'
    BorderColor = clBlack
    BorderWidth = 1
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
    Caption.Text = '<P align="center">EQ Title</P>'
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
    Text = '<FONT size="12"><B><P align="center">Online</P></B></FONT>'
    TextVAlign = tvaCenter
    FullHeight = 200
    object CimStatusGridPanel: TGridPanel
      AlignWithMargins = True
      Left = 1
      Top = 1
      Width = 305
      Height = 52
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      Align = alClient
      Caption = 'CimStatusGridPanel'
      ColumnCollection = <
        item
          Value = 33.333333333333340000
        end
        item
          Value = 33.333333333333340000
        end
        item
          Value = 33.333333333333310000
        end>
      ControlCollection = <
        item
          Column = 0
          Control = EzNetOfflineButton
          Row = 0
          RowSpan = 2
        end
        item
          Column = 1
          Control = EzNetLocalButton
          Row = 0
          RowSpan = 2
        end
        item
          Column = 2
          Control = EzNetRemoteButton
          Row = 0
          RowSpan = 2
        end
        item
          Column = 1
          Control = OnlineStaticText
          Row = 2
        end
        item
          Column = 2
          Control = EnabledStaticText
          Row = 2
        end
        item
          Column = 0
          Control = ConnectedStaticText
          Row = 2
        end>
      RowCollection = <
        item
          Value = 33.333333333333340000
        end
        item
          Value = 33.333333333333340000
        end
        item
          Value = 33.333333333333310000
        end>
      ShowCaption = False
      TabOrder = 0
      object EzNetOfflineButton: TAdvGlowButton
        AlignWithMargins = True
        Left = 1
        Top = 1
        Width = 101
        Height = 32
        Margins.Left = 0
        Margins.Top = 0
        Margins.Right = 0
        Margins.Bottom = 1
        Align = alClient
        Caption = 'OFFLINE'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -9
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        DisabledPicture.Data = {
          89504E470D0A1A0A0000000D4948445200000018000000180806000000E0773D
          F8000007E07A5458745261772070726F66696C65207479706520657869660000
          78DAAD595976EBB80EFCC72A7A09244070580EC7737A076FF95DA064C74E3CDD
          DC679D848E2C89000AA8021C9AFFFB77D13F788906A6A029C712A3C32B9450B8
          E24D765FAF79AEC739EFC2FE7D79F9F3B7A7871F3056C12AC7C914CE4FE53C7F
          B93E5E573CE8C1075EBFDD20D76DF876E354CFF3ECF8CE225537DCED2B7FFDAC
          35F25AF3F0AE868830C4C3A9630BBA3C0617364449F66D1147C28FE27DDA47C1
          915D75DD076CD65DC3D17DF1ECC52D1FFC205FFDF2D30FACDD77D8187872C2CA
          DC59F6B92C890B7771E225D8E117272932240B4BE729228184AFB6F8BD6FD9FB
          759FB1F3F0B8943D1EE671CBCB83DE5DF0C9B156778891F7F0DE9FB1825DCC86
          83B7308AFDC66500C4AF1337DD01BE1CD717DD002B40507798331CACAE1D8F68
          EABF724B760208AE53AC477EF9340C35DE5912B0B7C2182F80C0452FEAA37789
          39791F843300AAB09C257003025E95078CE42012814D461E616FDC93FCBE9695
          8FF32815E0A31225019B22156085A0C89F143272A82A0A4C55A326CD5AB45294
          18A2C61853B49AAB4952489A624A29A7926A961CB2E69853CEB9E45AB8084A52
          4B2CA9E4524AADD8B306AA5A7177C515B5366ED242D3165B6AB995563BD2A787
          AE3DF6D4732FBD0E1E32C2D011471A799451A79F48259A61EA8C33CD3CCBAC0B
          B9B66485A52BAEB4F22AAB5E513B51FD71FC016AFE448D3752765DBAA286B329
          D983F623BCF18C1A66408C8307E2C9104042B361E6B20F810D39C3CC15465528
          C348356C8677957C0484617AD6E5AFD87D21F7316E8458BFC38D3F418E0CBAFF
          03724C53BEE1F600B5614CD8376247155A4C9DA0FA6065E58C8F992E6FFE746D
          BDAF890A5A3012C82C1AA9624F88458E4B6729085C1D292BF7A4A3F5D06AAB25
          4EEF47841DD970CCE0AA50E674AB82995301C63A0801ACAB738982922B09A7B4
          F696DC9C435CEE19210DD853A38CEC56A8080518BC6ECB6E57FA7EE26E9531EB
          E94B9F2336AD2D0E57FBE05967614D2116071108C19197C20E768654152908E3
          7A182C6BA631575B73E500F1E09A17905F8B79756FA1D0159AAD15BBA5520699
          3AC527F72372F604434773A90197AD3680E6580E0FEDAAF608E88A9F8570C235
          A803AE2F2BB569B7B5E965DF5E2B1E8DDC8F655B2173A5B156EA006B81DB664C
          414F4BE8B92BEBA11988D6AD21FB8C5942619B7E1A7398726FC8690636DE8698
          A0DF99725A42BF8FCA7D50E8775169437D507B60CF202D9800D48A4F4852D454
          559415F642F6CF3644CE0A680B1580EA840E2217D77E18E03F1E745DE9FB896F
          AB9F4BED8D99014850216B065D194B9B65C70C74599790D415FA9B644380CF87
          3D5FE9FEC403E74FCFF1D8EDBB738FBDA78BFB56FF4F03F081FFF4320087FB37
          DE83880EFF0FEF6F7CA7B7CEBFF5FDC09DFE1EF863A54F80FFC45C7AE9C7CBCC
          B94F1CFA7DE6DCAFF4153DF7EB35FAD84827B7E641C4CBF388B372CB0992DB43
          763D5474E626F4ADA0DAB5350BF7E96C74DBD9A187B3423606A4AD245069A973
          9A92D451F1A7CBB38935CC45354020B1F34044742BC977ADA05722722F2608EF
          C99AD6EF80A6366F81A896AD645CA5A0A030629DD044288D9A28865CA0891045
          68221A9FE8B89AD66C4D8C73FC08179D398376CA38CF6D9A33B801AAC1DD0EB8
          C3E63B1D7393EFF227F98E7A90AF68A3567966C85EF55ADA40705AAF330FF616
          9B90D128619F4364E1877F1A047A1D9DB7D1EB40DE6ACA136CB4A2DA8C8DB292
          6465550CE7ABAFA7A770F9F0D53CB53FEA6D87422F5A17C8F80B2108F7F24CB7
          A268C56F4FB01A7A218FFE228AB79A485FA26865B893465ECB6378D829D0298A
          77C63C9747FFB453A02D8A0F8D791795FBA0D0EFA3721F14FA7D54EE8342BF8F
          CA7D50E865544041A8A0A6A8A03950416854D1DEE7DCD85B9F1A8C8D7193910B
          4688E5E3C1BE291DB2BD4EF21D07F9B2B1B78E0BB53E213732764B975E6677DC
          16A5284794649DAD9575DFE86A13A86F4C460F3D3804109FCFB9BB8CF1893019
          1719DD88CF7AE0F77DF2A58096D3DB8BE857A57F8DC421E19B21D7110C0B85E9
          90B5855DDA4588D3D6E1B875D80CABF3863FEE4CA49F45BF309DFC649D7743CE
          DBE9E8A0ADF7AC456F69EB5B509F21412FA1F883951E40F13029EF903871D8ED
          603CDA417A03C6C758D02F27D11F5800FE138E2712F2582E7FA2411F0E90FCD7
          79F42C52DFE1A54FCCB813B8270C4AEF28F45306A57714FAA9AED04F61F99DAE
          D04F6139E3D4D032818675E49935E5125B52AF48EC16225A9DC8AA2C2EA01DC3
          08102A8DB17BBACBA4818F6CD298D6DE55DFAC3D5B1F0DF3F42773EBABB1955E
          4FF39F0FF3F47A9A7F2CD1630C9FF6F72429F631671DCAA42168713D062B2029
          C8AFD9AD774D19E2F38236BEB306BD67E35764BB13A9B5F30B84C9DBFD762211
          9B88FA11625C4EB4333CAD916B18A8FAB2628EB11587163879D09D93581C9A71
          253FDA8896390EA64DB1EFA46C3A9923250D251EE34982FF39F43D9E74949E70
          3E9AF7142F6D3BFDD558745D35EFE9684F0F81313D30A607C5F480348DDEFEC9
          9019AD426D8640AA9AF85A1AC7546478FA8DE7EE210D52238B4FE8DABCD784D9
          E5F41EF1407B8111E2703F18FC7DBB2FD8DD9DEE5B5E22BE05334718D3C596E3
          084B25865241C96E342455F3DC47A35CDC28D2B243EB84382F0C787B52795518
          3F877034115F73F8EF32FA92D0F47719FDC7D3915881BFA4397AD73FDB539A03
          2BE0C4ECC788C94D9BE24FFBDE35DAFFA1B856B22F5E2BFCF082EEB5A7714005
          BD84B7F41FCF165016D240C9DF00000185694343504943432070726F66696C65
          0000789C7D913D48C35014854F53A52215413B883864A84E1644458A9354B108
          164A5BA155079397FE41938624C5C551702D38F8B358757071D6D5C15510047F
          40DCDC9C145DA4C4FB92428B182F3CDEC779F71CDEBB0F101A15A69A5D1380AA
          59462A1E13B3B95531F00A3F06E04314B31233F5447A3103CFFABAA74EAABB08
          CFF2EEFBB3FA94BCC9009F483CC774C322DE209ED9B474CEFBC421569214E273
          E271832E48FCC875D9E537CE4587059E193232A979E210B158EC60B98359C950
          89A789C38AAA51BE907559E1BCC559ADD458EB9EFC85C1BCB692E63AAD11C4B1
          8404921021A386322AB010A15D23C5448ACE631EFE61C79F24974CAE32183916
          50850AC9F183FFC1EFD99A85A94937291803BA5F6CFB631408EC02CDBA6D7F1F
          DB76F304F03F03575ADB5F6D00D14FD2EB6D2D7C04F46F0317D76D4DDE032E77
          80A1275D322447F2D3120A05E0FD8CBE29070CDE02BD6BEEDC5AE7387D003234
          ABE51BE0E010182B52F6BAC7BB7B3AE7F66F4F6B7E3FE01772D3C5474F5A0000
          0006624B4744000000000000F943BB7F000000097048597300000B1300000B13
          01009A9C180000000774494D4507E5031202041AAD672D450000019B49444154
          48C7EDD5BB6A545114C6F1DF8E632C24285EC1100B1FC107B0482B58686363AD
          D8D8866D2308EB01046B2F8585A5D808762236DA886893A078012D8C681C4D88
          27C7661F1886B99C19186C5CE5D96BADFF5E8BEFDB27E59C57B0826DE36337EE
          44C4152D23E59CBFE11C5691C6E4D7788BE311F1A90DA0833F588D88F76D0A72
          CE55A96B1573CD24661473661C3307740AE440CEB9DB62553F4ACEAE49000BB8
          89AD16F93B7853E4DA1AF00B97F1AA6F82FDA8CA79D50B89886A12C02DBC2872
          6D623E2252CEF914EE62B178A0916A9BDE1F71269582459C28C03D65154FF0A8
          ACEFD914523E8BF38D617EE31E8E9646BDA0C711F17552F5E49C1FE06A072262
          1D4B7D0947CA5AA635613DCE07F57F27FF73405D0CB63165EF0DD46984CC8EE1
          5D51513DE2E91876C984CEA81FC7369EE312BE0F38DFC235DC4077C0F44B7838
          0AB0596E7728225E0E99F27E44BC1E72B68CB534C68DA7711BFB0678A32E6EAF
          FA1EC3A6E74F2CA716965FC041ECC5E19E065D3CC5057C2EDF77F0A100BF44C4
          669AE06DB988EB3D80BA00D74B6398C7C988586BEAFE02B0587AEB3AFCC2B400
          00000049454E44AE426082}
        NotesFont.Charset = DEFAULT_CHARSET
        NotesFont.Color = clWindowText
        NotesFont.Height = -11
        NotesFont.Name = 'Tahoma'
        NotesFont.Style = []
        ParentFont = False
        Picture.Data = {
          89504E470D0A1A0A0000000D4948445200000018000000180806000000E0773D
          F8000009EF7A5458745261772070726F66696C65207479706520657869660000
          78DAAD5A5976EBB80EFCE72A7A0904C071391CCFE91DBCE57781941D3BF1EC67
          2591AF4481240A2C14A86BC6FFFE9DE61F7C9CB3C1381F53C821587C5C76990B
          BE24FBF319C7795F23EBD6DFD3878EBF646EDE609C0567D917A33BEECA71FDD4
          3E9CCF3074E306F95F0FC8B91BBEEC3896E33A5BBE1A510D64AF3EE9E777CE9E
          E61C7B76C505B821EC49ED2ECCC90C1A567849D6630147C4AFC7F7B88E8C23D9
          621B39DB6DB31547A34C4C622739EA860A4D1AD4716ED43046C78323CECC8D65
          5D4B123973132B244E0F9A1C254B97242C8D878838237C1E0BAD7EF3EAAF5142
          CF9DD09409C6088F3C3CCCB306AF1C73360B1F1161F674F80AE362561C48DD28
          FA17CD0008CD0337BF1C7C3ACE1F7301AC0041BFDC9C30C162EB36513DFDC496
          AC0010B4F338EFF8A2D815355E51E2D0B7C7604800810D249E100B91391239E1
          04800A46CEE2B80201F29E3B06C94E24009B843842DF7826D26ACB9EF7752C15
          E0E325480436590AC072CE237EA24B88A1E2C53BEF7DF0D1279F7D3141820B3E
          841083AEB91225BAE8638831A6986349925CF229A49852CAA964CE8225E973C8
          31A79C7329E8B338537CC1D3052D4AA95CA5BAEA6BA8B1A69A6B69089FE69A6F
          A1C5965A6EA57397EEBAEFA1C79E7AEE65D0402899E1861F61C491461E6522D6
          A64C37FD0C33CE34F32C67D40E54FF1C6FA046076ABC90D276F18C1AAEC6A886
          9609529EF18A19106347403C2A02086856CC6C22E7589153CC6C66AC0ACF18A4
          576C3AD9622800423788FDA433763FC8BD8C9B81AF9FE1C6AF206714BAFF0372
          6C86FCC2ED066A5D99B02DC4F62A549F5AC1EA43D47A02EDF76668E66AF51F60
          ACF7CF3163940EAC988D9DE029D8F6A3543D172CB358A4070E1584172BF73046
          678C39F89AE0BDD440B22EB4A29CCF98BF8CEA9B89691491D25DCC4C358F147C
          864B038D3AAB2BB9FA9CC71ABEAF83D3B4B5E7907201785102BCD5A456D794D8
          189831824C683424061FD9FB29E44B6A23B599D1699EC127CCC1A97366C86E06
          5B3815AE2C717F334810EBCBFDB3F456F44B9998C5445CC9F40953EFD3B98A9F
          5C35140C581F5C94F063DD7049BBA7917DCA7058B5E2005A9EB176F55F85CFF4
          9C4A91C136CEA60967797662F5EB976E7DA96D7045B8C42005EBABF4962D7259
          9156855BA1940AC1EBC58F196377D36DA33E11DC314D9BB50F84CA5C6BD47B7A
          3C9229EA99B4AE7AACC58E906C1177B1FA0134499559DBEAE4D4C5D10346BBFA
          4044EE5EECA91F6B8F9E563F669EBA497374DF00AA9F48ADF35DF3E6B6FDF7CD
          9B7786FFC8BCF9DE3BDBBCF9DE3BDBBCF9C03B91F7F21839CDD4417133255A86
          C0EB08FE813B30E4282C4B3A1C34B87F3FA7D6E3EC83666119A65576DA383A10
          49E3BDA8244D7F7C879927E73D42A3431C76B69E86BF5A4065EAEA39AF1D64FA
          07774730482B7382E5474AB372CB327BA758EA41136D488FCFF8C1EE11DD208A
          EDBF39AF9CA392E597FB2EEE9BAB06D7FEDBEE7BD179E6AEF7EE3A4FD9E717F9
          C081E6499311961381EA13379A17FCF8D08DA72833DF85E14F149AEFC2F0C791
          E6D507EF9D4F4898CFE2F86F189BCFE2F8AFFF8D84571FFC7B6EAE076950E41E
          99D6F7E2A1019A8F165A81520656DDBBD4A0006364E95CC3D0A11E5345B1A753
          F56BAAB9203351F705A88534A541DDA108AB28BD6C835C982142F255A1AEB201
          02224375A96C0890560464A5B4867A2C40FA69DD207634E366E9900DD03B5B39
          5437AA9B799C9503BA817DD8BEA11C2EE6691E7A5243EB949617E92B71233E41
          DB7DE5E5E833F8198A6218F1B50B6F4D9132F51064490AD64AD9C35E9E1EE8B7
          222AA4E00D55046DA600B9851A64AED166CFA687097F8C85E3C03CE69FB135B8
          4BE30AEB46C784C88A2BB2C28AAC708CD9C4B9734DF34B4A8425F7E1D3A13AA4
          D791A0CF269CDC265748CF14D4C76C3306064D6F155528183B8C0D5BC24CD912
          065FE490306D6464B2E851DB1E5EEED045D1BE4C6C2FAF5BDB375B60AA2A6B40
          172ACFAC124655C2F03F8412E741287F5A6408F7AA740209AC7482DCBFF9248E
          0258B6D3E14819283A9F2F7C40102C405B2312F801D662633539C96328281B2C
          DC3847542FCD010530C1076BF12BBBE58B7C4E08DCD90D8A14371A9A4A5FBC78
          EEE2B08FD16A0F53B75EEEF580A750D4EC4EA00ECE9290667FDFBCB96DFF7DF3
          E69DE13F326FBEF7CE366FBEF7CE366FBEF7CE366FBEF7CE366FBEF7CE366F3E
          F38E0787A370A400A20625F4D60D927749A932A5860C82C68C4503A3CA8BD49F
          2CDDD1C0445A4B8562C0A5786E5810D1AAA5E2AAA50E2272E7650C76CBCD2E12
          DF5A8DC65218DD6F2DC7269463096B6D8C4984AD8690A18E49A07E6DA89323D8
          0FB3F00D59C1B98469A5D42CD29FE6238CC56C72B795FA26F734D921CD8D8C01
          42B440C41F553298144DC13D7B88A86D0B5FF0BBB929A32E897F5F4F51A6CB0F
          B2B87992DE9FF0E40F939A7B4D5CE5B2B5A3D7A29D45A9F426912A751FCA9FDA
          ECEC5ED56697DCBFAFEBF386DDC79AED2A6D9A5F79F3ACD7EEE4D5DAF9227BB4
          D434ADA27DD1FDA37A557EBD08D46FA0CD0748DF3C9BBF48C38D1BC60344AD55
          AE73E6AD16E68CF415CE7752E635505738990F90BE09B4790F69D83C805C0550
          380A20806DC6F3463710C71DC5FC0272F315E66F2CDA97CFE6AD20BA4B37D999
          CB5DA98F36A506BC7A9145A2CAE76FB6A4CC377B4697E6CD377B4697E6CD377B
          4697E6CDAD4D9DCB3AECD532CCDCABC37A69D187DCA9685901419FFBDF3A0C79
          EBA46B17FC1FE85A8A17C224A29C35BFBDF6695499572079C5B8B91FB3EF1937
          3FD67769D57C1D35427C44092D83E7B8A5A09BDE50144D5F6A0646C5585B0CD7
          F4636EF00FC0D17211C5738D8C4A569280872AEBB67B8A10141173E8C0ED4446
          5A7D99F80A8FDE4888BFEBC863F5DFA923C3285492ABC396DCA3EB5DBC679F82
          EEE5F75C86A3107A73D9B768209B6C0B849A1EB20C019762C66371F452438B89
          12BCE65158729ED5EAAB58C846E420EA43770032720F7AA7584DE9A3A0EE5FAB
          003572AA36EF5590E05B2E52B3BEADD0773235BA501D572728D12B4DC9D06C1A
          F65310E9C69F3603AA97EFA9165F2AE9322DFA8AE8FCD6A365DA2F3D16FCBA50
          6D8B95F4E50214E35E4570ADAEA266AA2EA37012E01099414AEAC8C7007EED98
          C8D07D82E37DC858DB04D91D303B0463BEFB5E4419BE669E0020E91B075F2193
          E1AB1A724036A340A46FC305D196AD4F33588D50385B4D37389412C2614720C0
          C54898F5059B4ADA5873A39067896D8742F5DC5DCE472830F96020F2572CA480
          286A7685C28CA5F333F287C09E435F48C6092AE15DD424EE7E44F283030BC1ED
          30D6E1A9BD39C1E0AE3CB4835E3B844D0D5D1C6E5DA50273A3B78F56AFB9A715
          C3B95CD9D54A7BB270CD837D9357372116539BB7CBBC5BDB1028F5CC51EB95BE
          E2A670AB6B8B4B2A6697CA20DDE3D2FF42C15CB082B10AE3F64D5EDA0EE675CF
          0F3F46378E409E6BDDAB03D6B6D1687058CFE63F8B82788BC09825EC00000185
          694343504943432070726F66696C650000789C7D913D48C35014854F53A52215
          413B883864A84E1644458A9354B108164A5BA155079397FE41938624C5C55170
          2D38F8B358757071D6D5C15510047F40DCDC9C145DA4C4FB92428B182F3CDEC7
          79F71CDEBB0F101A15A69A5D1380AA59462A1E13B3B95531F00A3F06E04314B3
          1233F5447A3103CFFABAA74EAABB08CFF2EEFBB3FA94BCC9009F483CC774C322
          DE209ED9B474CEFBC421569214E273E271832E48FCC875D9E537CE4587059E19
          3232A979E210B158EC60B98359C95089A789C38AAA51BE907559E1BCC559ADD4
          58EB9EFC85C1BCB692E63AAD11C4B18404921021A386322AB010A15D23C5448A
          CE631EFE61C79F24974CAE3218391650850AC9F183FFC1EFD99A85A949372918
          03BA5F6CFB631408EC02CDBA6D7F1FDB76F304F03F03575ADB5F6D00D14FD2EB
          6D2D7C04F46F0317D76D4DDE032E7780A1275D322447F2D3120A05E0FD8CBE29
          070CDE02BD6BEEDC5AE7387D003234ABE51BE0E010182B52F6BAC7BB7B3AE7F6
          6F4F6B7E3FE01772D3C5474F5A00000006624B4744000000000000F943BB7F00
          0000097048597300000B1300000B1301009A9C180000000774494D4507E50312
          02062FC9E28BE40000019A4944415448C7EDD53D6B145114C6F1DFACAB161214
          5FC1905BF811FC001669050B6D6C52EBD8D8066C04BF812053FA5258588A8D60
          2762A38D883609CAA8A085118DAB09713216392BEBB22FB30B8B8DA79C7BEEF9
          DF73789E3399A25CC632B68C8FDDB82D4F97358C4C517EC139AC201B935FE30D
          923C7D680268E31756E4A96CF4A4A2ACE25EA368FDE96446D132E39839A01D90
          838AB2D36054DF2267D7248039DCC06683FC6DBC0EB93606FCC025BCECEBE000
          AA38AFFE82E4A99A047013CF43AEDDD8234F99A23C853B980F0F74A5DAA4F67B
          9CC9E2C23C4E04706F8CE2311EC6F89E4E21E5B338DF35CC4FDCC5B128D40B7A
          244F9F27964F51DEC7951D409ED6B0D0977034C632AD09EB713EA8FF3BF99F03
          EA30D8FA94B5D751672364761C6F4345F588D5D11AFA33A33DEAC7B18567B888
          AF03CE377115D7D119D0FD021E8C026CC4EB0ECBD38B215DDE93A75743CE16B1
          9A8D71E369DCC2FE01DEA8C3ED55DF32ECD6FC8EC5AC81E5E77008FB70A4A740
          074FB0848FF17D1BEF02F8499E36B20976CB055CEB01D4015C8BC23B5B9893F2
          B4DABDF61BCEA17017D8E860350000000049454E44AE426082}
        Rounded = True
        TabOrder = 0
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
      object EzNetLocalButton: TAdvGlowButton
        AlignWithMargins = True
        Left = 102
        Top = 1
        Width = 101
        Height = 32
        Margins.Left = 0
        Margins.Top = 0
        Margins.Right = 0
        Margins.Bottom = 1
        Align = alClient
        Caption = 'LOCAL'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -9
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        DisabledPicture.Data = {
          89504E470D0A1A0A0000000D4948445200000018000000180806000000E0773D
          F8000007E07A5458745261772070726F66696C65207479706520657869660000
          78DAAD595976EBB80EFCC72A7A09244070580EC7737A076FF95DA064C74E3CDD
          DC679D848E2C89000AA8021C9AFFFB77D13F788906A6A029C712A3C32B9450B8
          E24D765FAF79AEC739EFC2FE7D79F9F3B7A7871F3056C12AC7C914CE4FE53C7F
          B93E5E573CE8C1075EBFDD20D76DF876E354CFF3ECF8CE225537DCED2B7FFDAC
          35F25AF3F0AE868830C4C3A9630BBA3C0617364449F66D1147C28FE27DDA47C1
          915D75DD076CD65DC3D17DF1ECC52D1FFC205FFDF2D30FACDD77D8187872C2CA
          DC59F6B92C890B7771E225D8E117272932240B4BE729228184AFB6F8BD6FD9FB
          759FB1F3F0B8943D1EE671CBCB83DE5DF0C9B156778891F7F0DE9FB1825DCC86
          83B7308AFDC66500C4AF1337DD01BE1CD717DD002B40507798331CACAE1D8F68
          EABF724B760208AE53AC477EF9340C35DE5912B0B7C2182F80C0452FEAA37789
          39791F843300AAB09C257003025E95078CE42012814D461E616FDC93FCBE9695
          8FF32815E0A31225019B22156085A0C89F143272A82A0A4C55A326CD5AB45294
          18A2C61853B49AAB4952489A624A29A7926A961CB2E69853CEB9E45AB8084A52
          4B2CA9E4524AADD8B306AA5A7177C515B5366ED242D3165B6AB995563BD2A787
          AE3DF6D4732FBD0E1E32C2D011471A799451A79F48259A61EA8C33CD3CCBAC0B
          B9B66485A52BAEB4F22AAB5E513B51FD71FC016AFE448D3752765DBAA286B329
          D983F623BCF18C1A66408C8307E2C9104042B361E6B20F810D39C3CC15465528
          C348356C8677957C0484617AD6E5AFD87D21F7316E8458BFC38D3F418E0CBAFF
          03724C53BEE1F600B5614CD8376247155A4C9DA0FA6065E58C8F992E6FFE746D
          BDAF890A5A3012C82C1AA9624F88458E4B6729085C1D292BF7A4A3F5D06AAB25
          4EEF47841DD970CCE0AA50E674AB82995301C63A0801ACAB738982922B09A7B4
          F696DC9C435CEE19210DD853A38CEC56A8080518BC6ECB6E57FA7EE26E9531EB
          E94B9F2336AD2D0E57FBE05967614D2116071108C19197C20E768654152908E3
          7A182C6BA631575B73E500F1E09A17905F8B79756FA1D0159AAD15BBA5520699
          3AC527F72372F604434773A90197AD3680E6580E0FEDAAF608E88A9F8570C235
          A803AE2F2BB569B7B5E965DF5E2B1E8DDC8F655B2173A5B156EA006B81DB664C
          414F4BE8B92BEBA11988D6AD21FB8C5942619B7E1A7398726FC8690636DE8698
          A0DF99725A42BF8FCA7D50E8775169437D507B60CF202D9800D48A4F4852D454
          559415F642F6CF3644CE0A680B1580EA840E2217D77E18E03F1E745DE9FB896F
          AB9F4BED8D99014850216B065D194B9B65C70C74599790D415FA9B644380CF87
          3D5FE9FEC403E74FCFF1D8EDBB738FBDA78BFB56FF4F03F081FFF4320087FB37
          DE83880EFF0FEF6F7CA7B7CEBFF5FDC09DFE1EF863A54F80FFC45C7AE9C7CBCC
          B94F1CFA7DE6DCAFF4153DF7EB35FAD84827B7E641C4CBF388B372CB0992DB43
          763D5474E626F4ADA0DAB5350BF7E96C74DBD9A187B3423606A4AD245069A973
          9A92D451F1A7CBB38935CC45354020B1F34044742BC977ADA05722722F2608EF
          C99AD6EF80A6366F81A896AD645CA5A0A030629DD044288D9A28865CA0891045
          68221A9FE8B89AD66C4D8C73FC08179D398376CA38CF6D9A33B801AAC1DD0EB8
          C3E63B1D7393EFF227F98E7A90AF68A3567966C85EF55ADA40705AAF330FF616
          9B90D128619F4364E1877F1A047A1D9DB7D1EB40DE6ACA136CB4A2DA8C8DB292
          6465550CE7ABAFA7A770F9F0D53CB53FEA6D87422F5A17C8F80B2108F7F24CB7
          A268C56F4FB01A7A218FFE228AB79A485FA26865B893465ECB6378D829D0298A
          77C63C9747FFB453A02D8A0F8D791795FBA0D0EFA3721F14FA7D54EE8342BF8F
          CA7D50E865544041A8A0A6A8A03950416854D1DEE7DCD85B9F1A8C8D7193910B
          4688E5E3C1BE291DB2BD4EF21D07F9B2B1B78E0BB53E213732764B975E6677DC
          16A5284794649DAD9575DFE86A13A86F4C460F3D3804109FCFB9BB8CF1893019
          1719DD88CF7AE0F77DF2A58096D3DB8BE857A57F8DC421E19B21D7110C0B85E9
          90B5855DDA4588D3D6E1B875D80CABF3863FEE4CA49F45BF309DFC649D7743CE
          DBE9E8A0ADF7AC456F69EB5B509F21412FA1F883951E40F13029EF903871D8ED
          603CDA417A03C6C758D02F27D11F5800FE138E2712F2582E7FA2411F0E90FCD7
          79F42C52DFE1A54FCCB813B8270C4AEF28F45306A57714FAA9AED04F61F99DAE
          D04F6139E3D4D032818675E49935E5125B52AF48EC16225A9DC8AA2C2EA01DC3
          08102A8DB17BBACBA4818F6CD298D6DE55DFAC3D5B1F0DF3F42773EBABB1955E
          4FF39F0FF3F47A9A7F2CD1630C9FF6F72429F631671DCAA42168713D062B2029
          C8AFD9AD774D19E2F38236BEB306BD67E35764BB13A9B5F30B84C9DBFD762211
          9B88FA11625C4EB4333CAD916B18A8FAB2628EB11587163879D09D93581C9A71
          253FDA8896390EA64DB1EFA46C3A9923250D251EE34982FF39F43D9E74949E70
          3E9AF7142F6D3BFDD558745D35EFE9684F0F81313D30A607C5F480348DDEFEC9
          9019AD426D8640AA9AF85A1AC7546478FA8DE7EE210D52238B4FE8DABCD784D9
          E5F41EF1407B8111E2703F18FC7DBB2FD8DD9DEE5B5E22BE05334718D3C596E3
          084B25865241C96E342455F3DC47A35CDC28D2B243EB84382F0C787B52795518
          3F877034115F73F8EF32FA92D0F47719FDC7D3915881BFA4397AD73FDB539A03
          2BE0C4ECC788C94D9BE24FFBDE35DAFFA1B856B22F5E2BFCF082EEB5A7714005
          BD84B7F41FCF165016D240C9DF00000185694343504943432070726F66696C65
          0000789C7D913D48C35014854F53A52215413B883864A84E1644458A9354B108
          164A5BA155079397FE41938624C5C551702D38F8B358757071D6D5C15510047F
          40DCDC9C145DA4C4FB92428B182F3CDEC779F71CDEBB0F101A15A69A5D1380AA
          59462A1E13B3B95531F00A3F06E04314B31233F5447A3103CFFABAA74EAABB08
          CFF2EEFBB3FA94BCC9009F483CC774C322DE209ED9B474CEFBC421569214E273
          E271832E48FCC875D9E537CE4587059E193232A979E210B158EC60B98359C950
          89A789C38AAA51BE907559E1BCC559ADD458EB9EFC85C1BCB692E63AAD11C4B1
          8404921021A386322AB010A15D23C5448ACE631EFE61C79F24974CAE32183916
          50850AC9F183FFC1EFD99A85A94937291803BA5F6CFB631408EC02CDBA6D7F1F
          DB76F304F03F03575ADB5F6D00D14FD2EB6D2D7C04F46F0317D76D4DDE032E77
          80A1275D322447F2D3120A05E0FD8CBE29070CDE02BD6BEEDC5AE7387D003234
          ABE51BE0E010182B52F6BAC7BB7B3AE7F66F4F6B7E3FE01772D3C5474F5A0000
          0006624B4744000000000000F943BB7F000000097048597300000B1300000B13
          01009A9C180000000774494D4507E5031202041AAD672D450000019B49444154
          48C7EDD5BB6A545114C6F1DF8E632C24285EC1100B1FC107B0482B58686363AD
          D8D8866D2308EB01046B2F8585A5D808762236DA886893A078012D8C681C4D88
          27C7661F1886B99C19186C5CE5D96BADFF5E8BEFDB27E59C57B0826DE36337EE
          44C4152D23E59CBFE11C5691C6E4D7788BE311F1A90DA0833F588D88F76D0A72
          CE55A96B1573CD24661473661C3307740AE440CEB9DB62553F4ACEAE49000BB8
          89AD16F93B7853E4DA1AF00B97F1AA6F82FDA8CA79D50B89886A12C02DBC2872
          6D623E2252CEF914EE62B178A0916A9BDE1F71269582459C28C03D65154FF0A8
          ACEFD914523E8BF38D617EE31E8E9646BDA0C711F17552F5E49C1FE06A072262
          1D4B7D0947CA5AA635613DCE07F57F27FF73405D0CB63165EF0DD46984CC8EE1
          5D51513DE2E91876C984CEA81FC7369EE312BE0F38DFC235DC4077C0F44B7838
          0AB0596E7728225E0E99F27E44BC1E72B68CB534C68DA7711BFB0678A32E6EAF
          FA1EC3A6E74F2CA716965FC041ECC5E19E065D3CC5057C2EDF77F0A100BF44C4
          669AE06DB988EB3D80BA00D74B6398C7C988586BEAFE02B0587AEB3AFCC2B400
          00000049454E44AE426082}
        NotesFont.Charset = DEFAULT_CHARSET
        NotesFont.Color = clWindowText
        NotesFont.Height = -11
        NotesFont.Name = 'Tahoma'
        NotesFont.Style = []
        ParentFont = False
        Picture.Data = {
          89504E470D0A1A0A0000000D4948445200000018000000180806000000E0773D
          F800000A257A5458745261772070726F66696C65207479706520657869660000
          78DAC5596D72E33A0EFC8F53EC11089220C8E3F0B3EADD608FBF0D4A7612C7B1
          959957B5D678E888120576834083A2F9DF7F16FD079FE8355214CDA9A4E4F089
          25165FF123BB8FCF3CDBE31CBBB8FFBF7DF8FC9FE96987471BD086E3A4C6B337
          9CE76FD7A77B8B819E74B03CDC10EE8FF19F1FACF53CEF9DFF625160765F3EF9
          E3BBD6C86BCD63763526C0908E491D8FA0DB30B8B001A5B06F4B38145FC16FDD
          47C1915D759DA31BAEBB86A37361CFC12D8E3C882B2F9E3CD076EEB031FAE915
          ADF7DD877D2E07F5C5F7008B43B48397D750C20839F8D0FD0C21440AFE6E0BEF
          E796FDBCCE194F1E8C4B3D6330C62D2F0F7A77C19563ADEE801110CE1B273E08
          F6DE786083D1D0675C0642789DBCC906F876DC3FF489D8000665C39C31C1EADA
          314413FEF0ADB01D20E03A417BF817EB30D6FCF69288670B8CE1000A5CE2209C
          D8A9F7CA1C83CF20A8C2721FA26F608045FC80913E8690C04D861FE1D9B84779
          5FEBC51FE7B154C08F841414DC945041568C02FFD198E1435582441149A292A5
          48A514524C9252D2646BAE6AD0A8A24955B316AD39E49825A7AC39E7926BF125
          60494A49454B2EA5D48A67D648552AEEAEB8A2D6E65B68B1494B4D5B6EA5D50E
          F7E9B14B4F5D7BEEA5D7E1471871C84843471E65D4C913AE44334E9969EACCB3
          CCBAE06B2BACB864A5A52BAFB2EA9DB593D56FC72F58E39335BF99B2EBF4CE1A
          CEAADA407B08B63823C61918F391C1B81A0370686F9CB9CC317A63CE3873C563
          5588879162DC0C76953881C238D9CBE23B771FCC5DE68D80F53BDEFC15E6C8A8
          FB1798F334C3036F4F581B1609FB66EC58853B0206AC3EDC99ABCF75551A6189
          3AFBC3230EFEBA9D084829ACE58935B7355BB610E94A6D4D2B6C8F2D82F62169
          4AD232630120027A5D8E8E31BD303A2FA9337519BE8854A1DE78F4053653F703
          48242E23546D63F0D4826415A5AE9939B52A926B5C485AC142A2AD6144D2DC93
          2B13F148AB686802602481D3125B49D2CA7260670F8DEF4A2AE19C7F0BA1E43D
          3100F7A9A5C713BB5D928FBB668B6B4807A5F00B71B348EA2DF4D9E0A62DD83C
          7113FCCC2BB5C2D3005816010B885BF60B084E40B78EF3D3D7BC18D9A867E404
          3B53E70CD6464EC0DBB75EA9CB0C1E9E519BA105BE531F0D80C1D995FD0460A9
          8E24C3CD81B48635049AD7D43C86AEB17CB3E1BA08D31B4B5AB619165D2DA6B6
          607DD2559162C69C0D5635B5EB72EC4A710F506650DCABBDDA5F8B25EAEA3E81
          BEFDC4C50D1D6D0AFECCD526936B38C776C7C8F469681B38EE81CE7191DD6DE4
          656AE0D3C8E7B808307B645884B1E9C9E0DFCCBE62355D31FB8AD574C5EC2B56
          FFEB608F747835C65AAD87325F3B202C79E8A7070F75BD5B185A3E4CB7720E70
          3A59D59FEB2CC7C5EEC942B3B5F67EA14D6D3BE45822C93BE6B46C7F0D27DB5F
          E1F2E8A79F2F582E045EB0B8E795C3F488583A1047CFB58FE58264758F70F43A
          047E60577728BD8107020E780EF0800C7DC6EE7BF757E060FB8FD0D10FD85D87
          EEC48536305F71BB777EE97B0219BCFA0E1A7D43EDCF40EB74DDE55E7B1C5D77
          B9D7B091E176F5C69F5AC39DDEFBEC3597A5F73E7B0D7DBAC1FFE7E97F6778A1
          8514EF2B52BCC5A4621F877C8752C0523CB2795EB10EF9100B8F5AA13456882D
          A8DAA221AA86DE91CFA0C3836B1919DAC33F14855283AC81FCA92DF962AABEF7
          DA9B47D2976EF51D147CE3309B749A83A1143AC883C6C2BF09D13615D1D1EF61
          ED3BAAC47152DBFB4C4F67493FE287C91C9935869D597710466E1D676E3599E2
          2008908961EE2253040C4B3A34C216BC90293C6704073A5051F424DB50C9D344
          5DC90208DC40E4869899A5A92E941A09ACD9C3AD1AFC66572FD3168AC3A5B650
          E45C287A2C14A9663014C1F6B30935225B0DC4ADCE07D611E467EA01AAAAA1E6
          742166C03BA51788AAD2511C435499A62A9251859A56C3EF2A8D50338C02391A
          7241758A9E58338A9DBE40E7B9CE05F32CEFFC897EEB804F66BCE74BCBA67BC6
          8DC7CEDD8705E5E74740E3750434E8E23EA1E16FC9901058DED900DF7CAF54E8
          5DCE87413791058BB1381C8A135CDAF2DC1E75137164D47DD271A6E2D63A251C
          9EFF59C2DD045C5E580ED0CC913F8D4BF98D36BC220D6D64FA36F483C9572DA6
          77265FB598DE997CD562FA5B906F16D3DF827C1B99FE16E49BC5F487201F61A2
          C4502B2A0E145FE43B7C18C11FC517BC1D718267DF51AD94A1694D37B75875D1
          028DA0B4335BB3B7AA1071442C42238E24A186E20841A4205C2A0B22489C28D2
          530B332B0A74B1885EAB0E94D9CA3C911021645BDF58210160D1E438B0D8688B
          E0AC7BB1C996C06D4BE0235BF9D210E150864D14F7E3568EBA7C96A305E5E8E2
          8514D509110ED5BBEFC848336A486744EE016B398E86921459A958AA619900DC
          77DE660287B345118C1F847E0CAB1389FAD68167EDFB968F0AF01E6F7CDED293
          0E190D21AC198DBCB5D180B93BF43B90C0CFFBE97E81A24C5CD04E28EEC672EB
          884D75C574C7BC4D1D9391A98E3F91D157894EB60188D9E78F5BCB4873C78523
          95054FBB9F7F3EC08F03FDDCBEF4AA6EFBD92FDD6E146404B87CD4A6FD583AB0
          7370B17DD34DE52A4BB3D2458A0E8E5F504CBFE2F805C574727830FC9CDF4F7D
          8FD4220E9CD4D2256E2F504B97B8BDD0D203B727713863D41DCCC657BD375AE9
          CEEB73562F934ACF59FD7D4BAF2FF86B8B348C17A13E3F5127F4599E18AE4007
          F13365C4FA5D40AD91A04F17AE551E47E87FBAB1401656C3FA3909D90E3B02E3
          402087D40A8FE3DEB742E8892E0A2FD2D08FE3D26B83AFDB4BDF0C86F7DB1E7E
          8148DB7B8F50C728BBD4F61EFDDEC646CDCE0535C8DEE5ABE6B048221011337A
          E413AC4CB7F37DE580443672B3D244622B15B7AC23F1D59A585D9FA81BC69C79
          A44FECD19F6C836D6DE9EF1B78569FFCFF45C437D9437FABD46E16D313857245
          A07C0B28F44D9F5C9627E54B34A457E1F055A2DACCF2389885C3D1416D39A82D
          F0632D5ABC6681CFA490AC7A8CE2FD2A3DF6195092C1395357706E0598CF8885
          93934EB25AAFA3E8C442772AD037B52DC15DAB87968A15498209686828601B7C
          1F40827BF3750720112EECF5666454D9D19D156C14997B3F1A252ECF946D7CB1
          D73528FCEC3BCCA1CFCD8A81E97DADA2E84FF61E7C43D45F23D684E77601109C
          28A4944AF428735D81A0633CD4361F2652C3CB623D60395B012E28C06DA7808A
          BD24D8724F76010EB4AD002FF3DC194A7DF5D95FED20E199308F9C9FA63727DC
          C2158F702B399B7D3187628233EFF71FB9DA6658A92D9A4F85DEFCC80B0E0E47
          43B087FF2A9911A0D383C2A407ED4187F67EA3DDDEA9E9A61DA5ABD18EE1C6F7
          02837EF5AEA1D8C60D1E569605AB86515BEE804A7A235FD71040B44C40A1102F
          5F42FA936834642080F9AC00043C4D27C500A15E8D3094D2DD0190ADC021B16D
          538CC7B92956AC266EE5DC144B08AEF6FAD28C6B1BAC593A30DAAF407C635FCB
          88F60A44CDD9D22FF620BE39E4B5FAFEA749D385FA3E9B1BEB1C860AC78CC9CE
          244910C10A96A8BD26631EB461999AF60B194C0DA9486CB7AC69E8A3CB4E1DF8
          2A3CA8184EA3E0E9FF039E750DF9ABB5D50F0000018569434350494343207072
          6F66696C650000789C7D913D48C35014854F53A52215413B883864A84E164445
          8A9354B108164A5BA155079397FE41938624C5C551702D38F8B358757071D6D5
          C15510047F40DCDC9C145DA4C4FB92428B182F3CDEC779F71CDEBB0F101A15A6
          9A5D1380AA59462A1E13B3B95531F00A3F06E04314B31233F5447A3103CFFABA
          A74EAABB08CFF2EEFBB3FA94BCC9009F483CC774C322DE209ED9B474CEFBC421
          569214E273E271832E48FCC875D9E537CE4587059E193232A979E210B158EC60
          B98359C95089A789C38AAA51BE907559E1BCC559ADD458EB9EFC85C1BCB692E6
          3AAD11C4B18404921021A386322AB010A15D23C5448ACE631EFE61C79F24974C
          AE3218391650850AC9F183FFC1EFD99A85A94937291803BA5F6CFB631408EC02
          CDBA6D7F1FDB76F304F03F03575ADB5F6D00D14FD2EB6D2D7C04F46F0317D76D
          4DDE032E7780A1275D322447F2D3120A05E0FD8CBE29070CDE02BD6BEEDC5AE7
          387D003234ABE51BE0E010182B52F6BAC7BB7B3AE7F66F4F6B7E3FE01772D3C5
          474F5A00000006624B4744000000000000F943BB7F000000097048597300000B
          1300000B1301009A9C180000000774494D4507E50312020016C0BDA46A000001
          964944415448C7EDD53D6B145114C6F1DFC4351612145FC1905BF811FC001669
          E55A686363ADD8D8066C04BF8160ED4B6161E9D808762236DA886893A08C0A5A
          18D1184D889BB13903CBB22FB30B8B8DA79C7BCEF9DF73789E3B45A55CC10A76
          8C8FBDB89BE4AB5A465129BFE13C56518CC9AFF10E29C99FDA003AF883D52457
          6D0A2A6537EA5AC55C338919C59C19C7CC019D801CAA949B2D56F52372F64C02
          58C02D6CB7C8DFC5DB906B6BC02F5CC1EBBE090EA21BE7DD5E4892BB93006EE3
          65C8B589F9241795F234EE61313CD048B54DEF8F385B44C1224E06705FACE229
          1EC7FA9E4F21E573B8D018E637EEE37834EA053D49F2D749D553291FE25A0792
          BC8EA5BE8463B196694D588FF341FDDFC9FF1C5087C136A6ECBD81BA1821B313
          781F2AAA473C1DC32E59A033EAC7B18317B88CEF03CEB7711D37B13960FA253C
          1A05D88ADB1D49F2AB21533E48F29B2167CB582BC6B8F10CEEE0C0006FD4E1F6
          6EDF63D8F4FC89E5A285E5177018FB71B4A7C1269EE1223EC7F75D7C08E09724
          6F1513BC2D9770A3075007703D1AC33C4E2579ADA9FB0B83C37087FF47620A00
          00000049454E44AE426082}
        Rounded = True
        TabOrder = 1
        OnClick = EzNetLocalButtonClick
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
      object EzNetRemoteButton: TAdvGlowButton
        AlignWithMargins = True
        Left = 203
        Top = 1
        Width = 101
        Height = 32
        Margins.Left = 0
        Margins.Top = 0
        Margins.Right = 0
        Margins.Bottom = 1
        Align = alClient
        Caption = 'REMOTE'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -9
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        DisabledPicture.Data = {
          89504E470D0A1A0A0000000D4948445200000018000000180806000000E0773D
          F8000007E07A5458745261772070726F66696C65207479706520657869660000
          78DAAD595976EBB80EFCC72A7A09244070580EC7737A076FF95DA064C74E3CDD
          DC679D848E2C89000AA8021C9AFFFB77D13F788906A6A029C712A3C32B9450B8
          E24D765FAF79AEC739EFC2FE7D79F9F3B7A7871F3056C12AC7C914CE4FE53C7F
          B93E5E573CE8C1075EBFDD20D76DF876E354CFF3ECF8CE225537DCED2B7FFDAC
          35F25AF3F0AE868830C4C3A9630BBA3C0617364449F66D1147C28FE27DDA47C1
          915D75DD076CD65DC3D17DF1ECC52D1FFC205FFDF2D30FACDD77D8187872C2CA
          DC59F6B92C890B7771E225D8E117272932240B4BE729228184AFB6F8BD6FD9FB
          759FB1F3F0B8943D1EE671CBCB83DE5DF0C9B156778891F7F0DE9FB1825DCC86
          83B7308AFDC66500C4AF1337DD01BE1CD717DD002B40507798331CACAE1D8F68
          EABF724B760208AE53AC477EF9340C35DE5912B0B7C2182F80C0452FEAA37789
          39791F843300AAB09C257003025E95078CE42012814D461E616FDC93FCBE9695
          8FF32815E0A31225019B22156085A0C89F143272A82A0A4C55A326CD5AB45294
          18A2C61853B49AAB4952489A624A29A7926A961CB2E69853CEB9E45AB8084A52
          4B2CA9E4524AADD8B306AA5A7177C515B5366ED242D3165B6AB995563BD2A787
          AE3DF6D4732FBD0E1E32C2D011471A799451A79F48259A61EA8C33CD3CCBAC0B
          B9B66485A52BAEB4F22AAB5E513B51FD71FC016AFE448D3752765DBAA286B329
          D983F623BCF18C1A66408C8307E2C9104042B361E6B20F810D39C3CC15465528
          C348356C8677957C0484617AD6E5AFD87D21F7316E8458BFC38D3F418E0CBAFF
          03724C53BEE1F600B5614CD8376247155A4C9DA0FA6065E58C8F992E6FFE746D
          BDAF890A5A3012C82C1AA9624F88458E4B6729085C1D292BF7A4A3F5D06AAB25
          4EEF47841DD970CCE0AA50E674AB82995301C63A0801ACAB738982922B09A7B4
          F696DC9C435CEE19210DD853A38CEC56A8080518BC6ECB6E57FA7EE26E9531EB
          E94B9F2336AD2D0E57FBE05967614D2116071108C19197C20E768654152908E3
          7A182C6BA631575B73E500F1E09A17905F8B79756FA1D0159AAD15BBA5520699
          3AC527F72372F604434773A90197AD3680E6580E0FEDAAF608E88A9F8570C235
          A803AE2F2BB569B7B5E965DF5E2B1E8DDC8F655B2173A5B156EA006B81DB664C
          414F4BE8B92BEBA11988D6AD21FB8C5942619B7E1A7398726FC8690636DE8698
          A0DF99725A42BF8FCA7D50E8775169437D507B60CF202D9800D48A4F4852D454
          559415F642F6CF3644CE0A680B1580EA840E2217D77E18E03F1E745DE9FB896F
          AB9F4BED8D99014850216B065D194B9B65C70C74599790D415FA9B644380CF87
          3D5FE9FEC403E74FCFF1D8EDBB738FBDA78BFB56FF4F03F081FFF4320087FB37
          DE83880EFF0FEF6F7CA7B7CEBFF5FDC09DFE1EF863A54F80FFC45C7AE9C7CBCC
          B94F1CFA7DE6DCAFF4153DF7EB35FAD84827B7E641C4CBF388B372CB0992DB43
          763D5474E626F4ADA0DAB5350BF7E96C74DBD9A187B3423606A4AD245069A973
          9A92D451F1A7CBB38935CC45354020B1F34044742BC977ADA05722722F2608EF
          C99AD6EF80A6366F81A896AD645CA5A0A030629DD044288D9A28865CA0891045
          68221A9FE8B89AD66C4D8C73FC08179D398376CA38CF6D9A33B801AAC1DD0EB8
          C3E63B1D7393EFF227F98E7A90AF68A3567966C85EF55ADA40705AAF330FF616
          9B90D128619F4364E1877F1A047A1D9DB7D1EB40DE6ACA136CB4A2DA8C8DB292
          6465550CE7ABAFA7A770F9F0D53CB53FEA6D87422F5A17C8F80B2108F7F24CB7
          A268C56F4FB01A7A218FFE228AB79A485FA26865B893465ECB6378D829D0298A
          77C63C9747FFB453A02D8A0F8D791795FBA0D0EFA3721F14FA7D54EE8342BF8F
          CA7D50E865544041A8A0A6A8A03950416854D1DEE7DCD85B9F1A8C8D7193910B
          4688E5E3C1BE291DB2BD4EF21D07F9B2B1B78E0BB53E213732764B975E6677DC
          16A5284794649DAD9575DFE86A13A86F4C460F3D3804109FCFB9BB8CF1893019
          1719DD88CF7AE0F77DF2A58096D3DB8BE857A57F8DC421E19B21D7110C0B85E9
          90B5855DDA4588D3D6E1B875D80CABF3863FEE4CA49F45BF309DFC649D7743CE
          DBE9E8A0ADF7AC456F69EB5B509F21412FA1F883951E40F13029EF903871D8ED
          603CDA417A03C6C758D02F27D11F5800FE138E2712F2582E7FA2411F0E90FCD7
          79F42C52DFE1A54FCCB813B8270C4AEF28F45306A57714FAA9AED04F61F99DAE
          D04F6139E3D4D032818675E49935E5125B52AF48EC16225A9DC8AA2C2EA01DC3
          08102A8DB17BBACBA4818F6CD298D6DE55DFAC3D5B1F0DF3F42773EBABB1955E
          4FF39F0FF3F47A9A7F2CD1630C9FF6F72429F631671DCAA42168713D062B2029
          C8AFD9AD774D19E2F38236BEB306BD67E35764BB13A9B5F30B84C9DBFD762211
          9B88FA11625C4EB4333CAD916B18A8FAB2628EB11587163879D09D93581C9A71
          253FDA8896390EA64DB1EFA46C3A9923250D251EE34982FF39F43D9E74949E70
          3E9AF7142F6D3BFDD558745D35EFE9684F0F81313D30A607C5F480348DDEFEC9
          9019AD426D8640AA9AF85A1AC7546478FA8DE7EE210D52238B4FE8DABCD784D9
          E5F41EF1407B8111E2703F18FC7DBB2FD8DD9DEE5B5E22BE05334718D3C596E3
          084B25865241C96E342455F3DC47A35CDC28D2B243EB84382F0C787B52795518
          3F877034115F73F8EF32FA92D0F47719FDC7D3915881BFA4397AD73FDB539A03
          2BE0C4ECC788C94D9BE24FFBDE35DAFFA1B856B22F5E2BFCF082EEB5A7714005
          BD84B7F41FCF165016D240C9DF00000185694343504943432070726F66696C65
          0000789C7D913D48C35014854F53A52215413B883864A84E1644458A9354B108
          164A5BA155079397FE41938624C5C551702D38F8B358757071D6D5C15510047F
          40DCDC9C145DA4C4FB92428B182F3CDEC779F71CDEBB0F101A15A69A5D1380AA
          59462A1E13B3B95531F00A3F06E04314B31233F5447A3103CFFABAA74EAABB08
          CFF2EEFBB3FA94BCC9009F483CC774C322DE209ED9B474CEFBC421569214E273
          E271832E48FCC875D9E537CE4587059E193232A979E210B158EC60B98359C950
          89A789C38AAA51BE907559E1BCC559ADD458EB9EFC85C1BCB692E63AAD11C4B1
          8404921021A386322AB010A15D23C5448ACE631EFE61C79F24974CAE32183916
          50850AC9F183FFC1EFD99A85A94937291803BA5F6CFB631408EC02CDBA6D7F1F
          DB76F304F03F03575ADB5F6D00D14FD2EB6D2D7C04F46F0317D76D4DDE032E77
          80A1275D322447F2D3120A05E0FD8CBE29070CDE02BD6BEEDC5AE7387D003234
          ABE51BE0E010182B52F6BAC7BB7B3AE7F66F4F6B7E3FE01772D3C5474F5A0000
          0006624B4744000000000000F943BB7F000000097048597300000B1300000B13
          01009A9C180000000774494D4507E5031202041AAD672D450000019B49444154
          48C7EDD5BB6A545114C6F1DF8E632C24285EC1100B1FC107B0482B58686363AD
          D8D8866D2308EB01046B2F8585A5D808762236DA886893A078012D8C681C4D88
          27C7661F1886B99C19186C5CE5D96BADFF5E8BEFDB27E59C57B0826DE36337EE
          44C4152D23E59CBFE11C5691C6E4D7788BE311F1A90DA0833F588D88F76D0A72
          CE55A96B1573CD24661473661C3307740AE440CEB9DB62553F4ACEAE49000BB8
          89AD16F93B7853E4DA1AF00B97F1AA6F82FDA8CA79D50B89886A12C02DBC2872
          6D623E2252CEF914EE62B178A0916A9BDE1F71269582459C28C03D65154FF0A8
          ACEFD914523E8BF38D617EE31E8E9646BDA0C711F17552F5E49C1FE06A072262
          1D4B7D0947CA5AA635613DCE07F57F27FF73405D0CB63165EF0DD46984CC8EE1
          5D51513DE2E91876C984CEA81FC7369EE312BE0F38DFC235DC4077C0F44B7838
          0AB0596E7728225E0E99F27E44BC1E72B68CB534C68DA7711BFB0678A32E6EAF
          FA1EC3A6E74F2CA716965FC041ECC5E19E065D3CC5057C2EDF77F0A100BF44C4
          669AE06DB988EB3D80BA00D74B6398C7C988586BEAFE02B0587AEB3AFCC2B400
          00000049454E44AE426082}
        NotesFont.Charset = DEFAULT_CHARSET
        NotesFont.Color = clWindowText
        NotesFont.Height = -11
        NotesFont.Name = 'Tahoma'
        NotesFont.Style = []
        ParentFont = False
        Picture.Data = {
          89504E470D0A1A0A0000000D4948445200000018000000180806000000E0773D
          F800000006624B474400FF00FF00FFA0BDA793000000097048597300000B1300
          000B1301009A9C180000000774494D4507E10905062E1FA98088AF000001A049
          44415448C7EDD53D6B544114C6F1DF5D572D6450E21B1832857E033F80455AC1
          C23436D68A8D6D4AC16F2058FB525ADB682B2282168A689310B92A686144C795
          84B8B916991BE2B27BF7EEC262E329EF9C39FF39E73ECF4C2194CB58C696F1B1
          1FF7A4785DCB2884F21B96B082624C7E853544297E6A03E8E23756A458B63A52
          28FB795FABE8EC7632A3E89871CC1CD0CD9039A1ECB518D58F9CB36F1240C06D
          6CB6C8DFC6BB2CD7D6805FB88637031D1C413FAFF7FF82A4D89F0470072FB35C
          EB3820C54228CFE13EE6B3076AA9B6A9FD11178ABC611EA733F0601EC5133CCA
          E37B3685942FE2520D98C32B9CCC856AD0632C49F1EBC4F209E5193CEF0A2529
          AE636120E1441ECBB426AC767C906263C27F27FF5340950D96A6AC9D50150D32
          3B85F7594555C3D5D119F998D16D7A38B6F00257F17DC8FA266EE0167A43BA5F
          C0C326C0463EDD3129BE1ED1E50329BE1DB1B688D5628C1BCFE32E0E0FF14695
          DDDE1FB80CEB9A3FB1D8F40F489150061CC5211CDF53A087A7B88CCFF9FB363E
          64E017296E3403B0EBF4505EC1CD3D802A03D773E19D5B98B3525CADCBFC01B8
          B97333261040360000000049454E44AE426082}
        Rounded = True
        TabOrder = 2
        OnClick = EzNetRemoteButtonClick
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
      object OnlineStaticText: TPanel
        Left = 102
        Top = 34
        Width = 101
        Height = 17
        Align = alClient
        Caption = 'Online'
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -8
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentBackground = False
        ParentFont = False
        ParentShowHint = False
        ShowHint = False
        TabOrder = 3
      end
      object EnabledStaticText: TPanel
        Left = 203
        Top = 34
        Width = 101
        Height = 17
        Align = alClient
        Caption = 'Enabled'
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -8
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentBackground = False
        ParentFont = False
        ParentShowHint = False
        ShowHint = False
        TabOrder = 4
        OnDblClick = EnabledStaticTextDblClick
      end
      object ConnectedStaticText: TPanel
        Left = 1
        Top = 34
        Width = 101
        Height = 17
        Align = alClient
        Caption = 'Connected'
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -8
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentBackground = False
        ParentFont = False
        ParentShowHint = False
        ShowHint = False
        TabOrder = 5
      end
    end
  end
  object Refresh: TTimer
    OnTimer = RefreshTimer
    Left = 56
    Top = 512
  end
  object GrabData: TTimer
    Interval = 2000
    OnTimer = GrabDataTimer
    Left = 144
    Top = 512
  end
  object AlarmCheck: TTimer
    Interval = 100
    OnTimer = AlarmCheckTimer
    Left = 240
    Top = 512
  end
  object SwitchToLotInfoPanel: TTimer
    Enabled = False
    Interval = 50
    OnTimer = SwitchToLotInfoPanelTimer
    Left = 352
    Top = 512
  end
  object CureStartAlarmTimer: TTimer
    Interval = 10000
    OnTimer = CureStartAlarmTimerTimer
    Left = 536
    Top = 511
  end
  object UpdateUI: TTimer
    Interval = 100
    OnTimer = UpdateUITimer
    Left = 280
    Top = 456
  end
end
