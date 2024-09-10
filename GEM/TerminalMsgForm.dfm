object FmTerminalMsg: TFmTerminalMsg
  Left = 0
  Top = 0
  BorderStyle = bsSingle
  Caption = 'FmTerminalMsg'
  ClientHeight = 117
  ClientWidth = 616
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Visible = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object mmMsg: TMemo
    Left = 0
    Top = 0
    Width = 616
    Height = 117
    Align = alClient
    Alignment = taCenter
    BevelInner = bvNone
    BevelOuter = bvNone
    Color = 33023
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlack
    Font.Height = -24
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ImeName = #54620#44397#50612' '#51077#47141' '#49884#49828#53596' (IME 2000)'
    Lines.Strings = (
      'Terminal Service1')
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 0
    OnClick = mmMsgClick
    ExplicitWidth = 594
    ExplicitHeight = 42
  end
end
