//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <Registry.hpp>

#include "Main.h"
#include "dlgConnect.h"
#include "dlgView.h"
#include "Letter.h"
#include "base64.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

void rspLogSMTP(string &code, list<string> &message)
{
	list<string>::iterator it = message.begin();
    Form1->Log(("Сервер: " + code+": "+*it).c_str());

	for ( ++it;  it != message.end(); ++it )
        Form1->Log(("Сервер: " + *it).c_str());
}

void rspLog(string &message)
{
    Form1->Log(("Сервер: " + message).c_str());

}

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    // Инициализация работы с сокетами
    WSAData wsaData;
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
	{
        StatusBarMain->SimpleText ="WSAStartup failed!";
	}
    portPOP3 = 110;
    portSMTP = 25;

    clientPOP3.rHandle = rspLog;
    clientSMTP.rHandle = rspLogSMTP;

    RestoreSetting();
}

//---------------------------------------------------------------------------
void __fastcall TForm1::ToolButtonConnectClick(TObject *Sender)
{
    TOKBottomDlg *dlg = new TOKBottomDlg(this);

    dlg->EditHostPOP3->Text = hostPOP3.c_str();
    dlg->EditHostSMTP->Text = AnsiString(hostSMTP.c_str());
    dlg->EditPortPOP3->Text = IntToStr(portPOP3);
    dlg->EditPortSMTP->Text = IntToStr(portSMTP);

    dlg->EditUser->Text = user.c_str();
    dlg->EditPassword->Text = password.c_str();

    int rc = dlg->ShowModal();
    if ( rc == mrOk )
    {
        hostPOP3 = dlg->EditHostPOP3->Text.c_str();
        hostSMTP = dlg->EditHostSMTP->Text.c_str();
        portPOP3 = StrToInt(dlg->EditPortPOP3->Text);
        portSMTP = StrToInt(dlg->EditPortSMTP->Text);
        user = dlg->EditUser->Text.c_str();
        password = dlg->EditPassword->Text.c_str();

    }

    dlg->Close();
    delete dlg;

    if ( rc == mrOk )
    {
        SaveSetting();
        clientPOP3.rHandle = rspLog;

        if ( clientPOP3.connected() ) {
            clientPOP3.disconnect();
        }
        ReadEMailList();
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButtonRefreshClick(TObject *Sender)
{
    ReadEMailList();
}
//---------------------------------------------------------------------------

// Чтение параметров из реестра
void TForm1::RestoreSetting()
{
    TRegistry *regKey = new TRegistry();

    bool key = regKey->OpenKey("Simple Mail Client", false);
    if (!key)
    {
        delete regKey;
        return;
    }

    try
    {
        hostPOP3 = regKey->ReadString("hostPOP3").c_str();
        hostSMTP = regKey->ReadString("hostSMTP").c_str();
        portPOP3 = regKey->ReadInteger("portPOP3");
        portSMTP = regKey->ReadInteger("portSMTP");
        user = regKey->ReadString("user").c_str();
        password = regKey->ReadString("password").c_str();
    }
    catch( Exception &ex )
    {
    };

    delete regKey;
}
//---------------------------------------------------------------------------

void TForm1::SaveSetting()
{
    TRegistry *regKey = new TRegistry();

    bool key = regKey->OpenKey("Simple Mail Client", true);
    if (!key)
    {
        delete regKey;
        return;
    }

    try
    {
        regKey->WriteString("hostPOP3", hostPOP3.c_str());
        regKey->WriteString("hostSMTP", hostSMTP.c_str());
        regKey->WriteInteger("portPOP3", portPOP3);
        regKey->WriteInteger("portSMTP", portSMTP);

        regKey->WriteString("user", user.c_str());
        regKey->WriteString("password", password.c_str());
    }
    catch( Exception &ex )
    {
    };

    delete regKey;
}
//---------------------------------------------------------------------------

void TForm1::ReadEMailList()
{
    if ( !clientPOP3.connected() )
    {
        // Сначала подключение
        clientPOP3.host = hostPOP3;
        clientPOP3.port = portPOP3;
    	int rc = clientPOP3.connect(user, password);
        if ( rc < 0 )
        {
            Log("Logon error");
            ToolButtonRefresh->Enabled = false;
            ToolButtonNew->Enabled = false;
            return;
        }
        else
        {
            Log("Logon OK");
            ToolButtonRefresh->Enabled = true;
            ToolButtonNew->Enabled = true;
        }
    }

    StatusBarMain->SimpleText = "Загрузка почты...";

    // Запрос списка
	string from, subj, date;
	int m = 0;
	int s = 0;
	clientPOP3.stat(m, s);

    ListViewMail->Clear();
    ListViewMail->Cursor = crHourGlass;

	for ( int i = 0; i < m; i++ )
	{
		clientPOP3.top(i+1, from, subj, date);

        ListViewMail->Items->Add();
        TListItem *Item = ListViewMail->Items->Item[ListViewMail->Items->Count-1];
        Item->Caption = from.c_str();
        Item->SubItems->Add(subj.c_str());
        Item->SubItems->Add(date.c_str());
	}
    ListViewMail->Cursor = crDefault;
    StatusBarMain->SimpleText = "Почта загружена";
}

void TForm1::Log(const char *line)
{
    MemoLog->Lines->Add(line);
    SendMessage(MemoLog->Handle, EM_LINESCROLL, 0, MemoLog->Lines->Count);
}

int ColumnToSort = 0;

void __fastcall TForm1::ListViewMailColumnClick(TObject *Sender, TListColumn *Column)
{
    ColumnToSort = Column->Index;
    ((TCustomListView *)Sender)->AlphaSort();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListViewMailCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{
    if (ColumnToSort == 0)
        CompareText(Item1->Caption, Item2->Caption);
    else
        Compare = CompareText(Item1->SubItems->Strings[ColumnToSort-1], Item2->SubItems->Strings[ColumnToSort-1]);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::TimerRefreshTimer(TObject *Sender)
{
    //    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
	WSACleanup();
}
//---------------------------------------------------------------------------



void __fastcall TForm1::ToolButtonNewClick(TObject *Sender)
{
    TForm2 *f2 = new TForm2(this);
    string to;
    string subject;
    list<string> message;
    int rc = mrCancel;

    try
    {
    	rc = f2->ShowModal();
        to = string(f2->EditTo->Text.c_str());
        subject = string(f2->EditSubject->Text.c_str());

        for ( int i = 0; i < f2->mlText->Lines->Count; i++ )
            message.push_back(f2->mlText->Lines->Strings[i].c_str());
    }
    __finally
    {
    	delete f2;
    }
    if ( rc != mrOk ) return;

    // Попытка послать письмо

    clientSMTP.host = hostSMTP;
    clientSMTP.port = portSMTP;
	rc = clientSMTP.connect(user, password);
	if ( rc < 0 )
	{
		Log("Ошибка подключения к SMTP-серверу");
		return;
	}

	rc = clientSMTP.ehlo();
	if ( rc < 0 )
	{
		Log("Ошибка команды EHLO");
		return;
	}

    // Смотрим, какой вариант аутентификации на сервере
	list<string>::iterator it;
    bool login = false;
	for ( it = clientSMTP.esmtp.begin();  it != clientSMTP.esmtp.end(); ++it )
    {
        if ( it->substr(0, 4) == string("AUTH") )
        {
            if ( it->substr(5, 5) == string("LOGIN") )
            login = true;
        }
    }

    if ( login )
    {
    	rc = clientSMTP.cmnd("AUTH LOGIN");
	    if ( rc < 0 )
    	{
	    	Log("Ошибка команды EHLO");
		    return;
        }

        // Логин
    	string encoded_login = base64_encode(
            reinterpret_cast<const unsigned char*>(clientSMTP.user.c_str()),
            clientSMTP.user.size());
    	rc = clientSMTP.cmnd(encoded_login);
	    if ( rc < 0 )
    	{
	    	Log("Неверный логин");
		    return;
        }

        // Пароль
    	string encoded_password = base64_encode(
            reinterpret_cast<const unsigned char*>(clientSMTP.password.c_str()),
            clientSMTP.password.size());
    	rc = clientSMTP.cmnd(encoded_password);
	    if ( rc < 0 )
    	{
	    	Log("Неверный пароль");
		    return;
        }
	}

    // Адрес для возврата
	rc = clientSMTP.mail(user);
	if ( rc < 0 )
	{
		Log("SMTP error");
		return;
	}

    // Адрес получателя
	rc = clientSMTP.rcpt(to);
	if ( rc < 0 )
	{
		Log("SMTP error");
		return;
	}

    list<string> attach;
	rc = clientSMTP.data(
        user, to,
        subject, message, attach);
	if ( rc < 0 )
	{
		Log("SMTP error");
		return;
	}

    // Завершение передачи - команда QUIT
    clientSMTP.quit();
    clientSMTP.disconnect();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListViewMailDblClick(TObject *Sender)
{
    // Открыть выделенное письмо
    TListItem *item = ListViewMail->Selected;
    if ( item == NULL ) return;

    list<string> message;
    int id = item->Index + 1;
    clientPOP3.retr(id, message);

    TForm4 *f4 = new TForm4(this);
    string to;
    string subject;
    int rc = mrCancel;

    // Чтение сообщения
    f4->mlText->Clear();
    list<string>::iterator it;
    bool r = false;
    for ( it = message.begin(); it != message.end(); ++it )
    {
       f4->mlText->Lines->Add(AnsiString(it->c_str()));
    }


    try
    {
    	rc = f4->ShowModal();
        if ( rc ==  mrOk )
        {
            clientPOP3.dele(id);
        }
    }
    __finally
    {
    	delete f4;
    }

}
//---------------------------------------------------------------------------

