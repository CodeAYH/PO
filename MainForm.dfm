object FmMain: TFmMain
  Left = 0
  Top = 0
  Anchors = []
  Caption = 'FmMain'
  ClientHeight = 729
  ClientWidth = 1009
  Color = clBtnFace
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  GlassFrame.Enabled = True
  GlassFrame.SheetOfGlass = True
  OldCreateOrder = False
  Position = poScreenCenter
  OnClick = FormClick
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnShow = FormShow
  DesignSize = (
    1009
    729)
  PixelsPerInch = 96
  TextHeight = 13
  object BtnOperatorLogin: TAdvGlassButton
    Left = 225
    Top = 80
    Width = 570
    Height = 90
    Anchors = []
    AntiAlias = aaAntiAlias
    BackColor = clYellow
    Caption = 'BtnOperatorLogin'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -48
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
    Version = '1.3.3.1'
    OnClick = BtnOperatorLoginClick
  end
  object BtnEngineerLogin: TAdvGlassButton
    Left = 225
    Top = 190
    Width = 570
    Height = 90
    Anchors = []
    AntiAlias = aaAntiAlias
    BackColor = clRed
    Caption = 'BtnOperatorLogin'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -48
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
    OnClick = BtnEngineerLoginClick
  end
  object BtnExit: TAdvGlassButton
    Left = 225
    Top = 568
    Width = 570
    Height = 90
    Anchors = []
    AntiAlias = aaAntiAlias
    BackColor = clBlue
    Caption = 'BtnOperatorLogin'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -48
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
    OnClick = BtnExitClick
  end
  object ClickCancel: TTimer
    Interval = 3000
    OnTimer = ClickCancelTimer
    Left = 904
    Top = 216
  end
  object Keyboard: TAdvSmoothPopupTouchKeyBoard
    AutoCompletion.Font.Charset = DEFAULT_CHARSET
    AutoCompletion.Font.Color = clWhite
    AutoCompletion.Font.Height = -19
    AutoCompletion.Font.Name = 'Tahoma'
    AutoCompletion.Font.Style = []
    AutoCompletion.Color = clBlack
    AutoHide = True
    KeyboardType = ktQWERTY
    Fill.Color = clWhite
    Fill.ColorTo = clWhite
    Fill.ColorMirror = clNone
    Fill.ColorMirrorTo = clNone
    Fill.GradientType = gtVertical
    Fill.GradientMirrorType = gtSolid
    Fill.BorderColor = 13027014
    Fill.Rounding = 0
    Fill.ShadowOffset = 0
    Fill.Glow = gmNone
    Keys = <
      item
        KeyCombination = True
        ShiftKeyCombination = True
        Caption = '`'
        ShiftCaption = '~'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 1
        Y = 1
        SubKeys = <>
      end
      item
        Caption = '1'
        ShiftCaption = '!'
        AltGrCaption = '|'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 41
        Y = 1
        SubKeys = <>
      end
      item
        Caption = '2'
        ShiftCaption = '@'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 81
        Y = 1
        SubKeys = <>
      end
      item
        Caption = '3'
        ShiftCaption = '#'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 121
        Y = 1
        SubKeys = <>
      end
      item
        Caption = '4'
        ShiftCaption = '$'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 161
        Y = 1
        SubKeys = <>
      end
      item
        Caption = '5'
        ShiftCaption = '%'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 201
        Y = 1
        SubKeys = <>
      end
      item
        ShiftKeyCombination = True
        Caption = '6'
        ShiftCaption = '^'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 241
        Y = 1
        SubKeys = <>
      end
      item
        Caption = '7'
        ShiftCaption = '&'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 281
        Y = 1
        SubKeys = <>
      end
      item
        Caption = '8'
        ShiftCaption = '*'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 321
        Y = 1
        SubKeys = <>
      end
      item
        Caption = '9'
        ShiftCaption = '('
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 361
        Y = 1
        SubKeys = <>
      end
      item
        Caption = '0'
        ShiftCaption = ')'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 401
        Y = 1
        SubKeys = <>
      end
      item
        Caption = '-'
        ShiftCaption = '_'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 441
        Y = 1
        SubKeys = <>
      end
      item
        Caption = '='
        ShiftCaption = '+'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 481
        Y = 1
        SubKeys = <>
      end
      item
        Caption = '\'
        ShiftCaption = '|'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 521
        Y = 1
        SubKeys = <>
      end
      item
        Caption = 'Back'
        KeyValue = 8
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skBackSpace
        Color = 16185078
        ColorDown = 16573128
        X = 561
        Y = 1
        SubKeys = <>
      end
      item
        Caption = '->'
        ShiftCaption = '<-'
        KeyValue = 9
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skTab
        Color = 16185078
        ColorDown = 16573128
        X = 1
        Y = 41
        Width = 60
        SubKeys = <>
      end
      item
        Caption = 'Q'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 61
        Y = 41
        SubKeys = <>
      end
      item
        Caption = 'W'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 101
        Y = 41
        SubKeys = <>
      end
      item
        Caption = 'E'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 141
        Y = 41
        SubKeys = <
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end>
      end
      item
        Caption = 'R'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 181
        Y = 41
        SubKeys = <>
      end
      item
        Caption = 'T'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 221
        Y = 41
        SubKeys = <>
      end
      item
        Caption = 'Y'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 261
        Y = 41
        SubKeys = <
          item
            Caption = #63735
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end>
      end
      item
        Caption = 'U'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 301
        Y = 41
        SubKeys = <
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end>
      end
      item
        Caption = 'I'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 341
        Y = 41
        SubKeys = <
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end>
      end
      item
        Caption = 'O'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 381
        Y = 41
        SubKeys = <
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end>
      end
      item
        Caption = 'P'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 421
        Y = 41
        SubKeys = <>
      end
      item
        Caption = '['
        ShiftCaption = '{'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 461
        Y = 41
        SubKeys = <>
      end
      item
        Caption = ']'
        ShiftCaption = '}'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 501
        Y = 41
        SubKeys = <>
      end
      item
        Caption = 'Enter'
        KeyValue = 13
        ShiftKeyValue = 0
        AltGrKeyValue = 0
        SpecialKey = skReturn
        Color = 16185078
        ColorDown = 16573128
        X = 551
        Y = 41
        Height = 80
        Width = 50
        SubKeys = <>
      end
      item
        Caption = 'Caps Lock'
        KeyValue = 20
        ShiftKeyValue = 0
        AltGrKeyValue = 0
        SpecialKey = skCaps
        Color = 16185078
        ColorDown = 16573128
        X = 1
        Y = 81
        Width = 70
        SubKeys = <>
      end
      item
        Caption = 'A'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 71
        Y = 81
        SubKeys = <
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end>
      end
      item
        Caption = 'S'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 111
        Y = 81
        SubKeys = <>
      end
      item
        Caption = 'D'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 151
        Y = 81
        SubKeys = <>
      end
      item
        Caption = 'F'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 191
        Y = 81
        SubKeys = <>
      end
      item
        Caption = 'G'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 231
        Y = 81
        SubKeys = <>
      end
      item
        Caption = 'H'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 271
        Y = 81
        SubKeys = <>
      end
      item
        Caption = 'J'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 311
        Y = 81
        SubKeys = <>
      end
      item
        Caption = 'K'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 351
        Y = 81
        SubKeys = <>
      end
      item
        Caption = 'L'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 391
        Y = 81
        SubKeys = <>
      end
      item
        Caption = ';'
        ShiftCaption = ':'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 431
        Y = 81
        SubKeys = <>
      end
      item
        Caption = ','
        ShiftCaption = '"'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 471
        Y = 81
        SubKeys = <>
      end
      item
        Caption = 'Shift'
        KeyValue = 160
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skShift
        Color = 16185078
        ColorDown = 16573128
        X = 1
        Y = 121
        Width = 80
        SubKeys = <>
      end
      item
        Caption = 'Z'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 81
        Y = 121
        SubKeys = <>
      end
      item
        Caption = 'X'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 121
        Y = 121
        SubKeys = <>
      end
      item
        Caption = 'C'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 161
        Y = 121
        SubKeys = <
          item
            Caption = '?'
          end>
      end
      item
        Caption = 'V'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 201
        Y = 121
        SubKeys = <>
      end
      item
        Caption = 'B'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 241
        Y = 121
        SubKeys = <>
      end
      item
        Caption = 'N'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 281
        Y = 121
        SubKeys = <
          item
            Caption = '?'
          end
          item
            Caption = '?'
          end>
      end
      item
        Caption = 'M'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 321
        Y = 121
        SubKeys = <>
      end
      item
        Caption = ','
        ShiftCaption = '<'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 361
        Y = 121
        SubKeys = <>
      end
      item
        Caption = '.'
        ShiftCaption = '>'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 401
        Y = 121
        SubKeys = <>
      end
      item
        Caption = '/'
        ShiftCaption = '?'
        KeyValue = -1
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skNone
        Color = 16185078
        ColorDown = 16573128
        X = 441
        Y = 121
        SubKeys = <>
      end
      item
        Caption = 'Shift'
        KeyValue = 161
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skShift
        Color = 16185078
        ColorDown = 16573128
        X = 481
        Y = 121
        Width = 110
        SubKeys = <>
      end
      item
        Caption = 'Ctrl'
        KeyValue = 162
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skCtrl
        Color = 16185078
        ColorDown = 16573128
        X = 1
        Y = 161
        Width = 60
        SubKeys = <>
      end
      item
        Caption = 'Win'
        KeyValue = 91
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skWin
        Color = 16185078
        ColorDown = 16573128
        X = 61
        Y = 161
        SubKeys = <>
      end
      item
        Caption = 'Alt'
        KeyValue = 18
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skAlt
        Color = 16185078
        ColorDown = 16573128
        X = 101
        Y = 161
        Width = 60
        SubKeys = <>
      end
      item
        KeyValue = 32
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skSpaceBar
        Color = 16185078
        ColorDown = 16573128
        X = 161
        Y = 161
        Width = 240
        SubKeys = <>
      end
      item
        Caption = 'Alt Gr'
        KeyValue = 0
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skAltGr
        Color = 16185078
        ColorDown = 16573128
        X = 401
        Y = 161
        Width = 60
        SubKeys = <>
      end
      item
        Caption = 'Win'
        KeyValue = 92
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skWin
        Color = 16185078
        ColorDown = 16573128
        X = 461
        Y = 161
        SubKeys = <>
      end
      item
        Caption = 'Menu'
        KeyValue = 93
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skApp
        Color = 16185078
        ColorDown = 16573128
        X = 501
        Y = 161
        SubKeys = <>
      end
      item
        Caption = '->'
        KeyValue = 39
        ShiftKeyValue = -1
        AltGrKeyValue = -1
        SpecialKey = skRight
        Color = 16185078
        ColorDown = 16573128
        X = 541
        Y = 161
        Width = 60
        SubKeys = <>
      end>
    SmallFont.Charset = DEFAULT_CHARSET
    SmallFont.Color = clWindowText
    SmallFont.Height = -9
    SmallFont.Name = 'Tahoma'
    SmallFont.Style = []
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Left = 904
    Top = 72
  end
  object ClearOutput: TTimer
    Enabled = False
    Interval = 3000
    Left = 904
    Top = 288
  end
  object AutoModeChange: TTimer
    Enabled = False
    Interval = 300000
    OnTimer = AutoModeTimer
    Left = 904
    Top = 352
  end
end
