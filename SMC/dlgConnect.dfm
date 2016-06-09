object OKBottomDlg: TOKBottomDlg
  Left = 256
  Top = 253
  BorderStyle = bsDialog
  Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1089#1077#1088#1074#1077#1088#1072
  ClientHeight = 347
  ClientWidth = 433
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object OKBtn: TButton
    Left = 319
    Top = 20
    Width = 98
    Height = 25
    Caption = #1055#1086#1076#1082#1083#1102#1095#1080#1090#1100#1089#1103
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object CancelBtn: TButton
    Left = 319
    Top = 52
    Width = 98
    Height = 25
    Cancel = True
    Caption = #1054#1090#1084#1077#1085#1072
    ModalResult = 2
    TabOrder = 1
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 297
    Height = 105
    Caption = #1057#1077#1088#1074#1077#1088' '#1074#1093#1086#1076#1103#1097#1077#1081' '#1087#1086#1095#1090#1099' (POP3)'
    TabOrder = 2
    object Label1: TLabel
      Left = 24
      Top = 32
      Width = 24
      Height = 13
      Caption = #1061#1086#1089#1090
    end
    object Label2: TLabel
      Left = 24
      Top = 72
      Width = 25
      Height = 13
      Caption = #1055#1086#1088#1090
    end
    object EditHostPOP3: TEdit
      Left = 64
      Top = 32
      Width = 193
      Height = 21
      TabOrder = 0
    end
    object UpDownPOP3: TUpDown
      Left = 121
      Top = 72
      Width = 16
      Height = 21
      Associate = EditPortPOP3
      Min = 1
      Max = 32000
      Position = 110
      TabOrder = 1
      Wrap = False
    end
    object EditPortPOP3: TEdit
      Left = 64
      Top = 72
      Width = 57
      Height = 21
      TabOrder = 2
      Text = '110'
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 120
    Width = 297
    Height = 105
    Caption = #1057#1077#1088#1074#1077#1088' '#1080#1089#1093#1086#1076#1103#1097#1077#1081' '#1087#1086#1095#1090#1099' (SMTP)'
    TabOrder = 3
    object Label3: TLabel
      Left = 24
      Top = 32
      Width = 24
      Height = 13
      Caption = #1061#1086#1089#1090
    end
    object Label4: TLabel
      Left = 24
      Top = 72
      Width = 25
      Height = 13
      Caption = #1055#1086#1088#1090
    end
    object EditHostSMTP: TEdit
      Left = 64
      Top = 32
      Width = 193
      Height = 21
      TabOrder = 0
    end
    object UpDownSMTP: TUpDown
      Left = 121
      Top = 72
      Width = 16
      Height = 21
      Associate = EditPortSMTP
      Min = 1
      Max = 32000
      Position = 25
      TabOrder = 1
      Wrap = False
    end
    object EditPortSMTP: TEdit
      Left = 64
      Top = 72
      Width = 57
      Height = 21
      TabOrder = 2
      Text = '25'
    end
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 232
    Width = 297
    Height = 105
    Caption = #1048#1076#1077#1085#1090#1080#1092#1080#1082#1072#1094#1080#1103
    TabOrder = 4
    object Label5: TLabel
      Left = 24
      Top = 32
      Width = 73
      Height = 13
      Caption = #1055#1086#1083#1100#1079#1086#1074#1072#1090#1077#1083#1100
    end
    object Label6: TLabel
      Left = 24
      Top = 64
      Width = 38
      Height = 13
      Caption = #1055#1072#1088#1086#1083#1100
    end
    object EditUser: TEdit
      Left = 112
      Top = 32
      Width = 153
      Height = 21
      TabOrder = 0
    end
    object EditPassword: TEdit
      Left = 112
      Top = 64
      Width = 153
      Height = 21
      TabOrder = 1
    end
  end
end
