object TCardInputDlg: TTCardInputDlg
  Left = 227
  Top = 108
  BorderStyle = bsDialog
  Caption = 'Chamber Select Barcode'
  ClientHeight = 169
  ClientWidth = 506
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object NoticeLabel: TLabel
    Left = 104
    Top = 96
    Width = 5
    Height = 19
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object ChamberBacodeEdit: TAdvEdit
    Left = 107
    Top = 32
    Width = 369
    Height = 27
    EmptyTextStyle = []
    LabelCaption = 'Barcode'
    LabelPosition = lpLeftCenter
    LabelFont.Charset = DEFAULT_CHARSET
    LabelFont.Color = clWindowText
    LabelFont.Height = -16
    LabelFont.Name = 'Tahoma'
    LabelFont.Style = [fsBold]
    Lookup.Font.Charset = DEFAULT_CHARSET
    Lookup.Font.Color = clWindowText
    Lookup.Font.Height = -11
    Lookup.Font.Name = 'Arial'
    Lookup.Font.Style = []
    Lookup.Separator = ';'
    Color = clWindow
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = ''
    Visible = True
    OnKeyDown = ChamberSelectEditKeyDown
    Version = '4.0.3.6'
  end
  object CancelButton: TAdvGlassButton
    Left = 342
    Top = 112
    Width = 134
    Height = 38
    BackColor = clBlue
    Caption = 'Cancel'
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
    Version = '1.3.3.0'
    OnClick = CancelButtonClick
  end
end
