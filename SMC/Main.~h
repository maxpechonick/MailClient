//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ActnCtrls.hpp>
#include <ActnMan.hpp>
#include <ImgList.hpp>
#include <ExtCtrls.hpp>

#include <string>
#include <list>

using namespace std;

#include "POP3.h"
#include "SMTP.h"



//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TListView *ListViewMail;
    TStatusBar *StatusBarMain;
    TImageList *ImageListAction;
    TActionToolBar *ActionToolBar1;
    TToolBar *ToolBarMain;
    TToolButton *ToolButtonConnect;
    TToolButton *ToolButton2;
    TToolButton *ToolButtonRefresh;
    TToolButton *ToolButtonNew;
    TSplitter *SplitterMain;
    TMemo *MemoLog;
    TTimer *TimerRefresh;
    TToolButton *ToolButton1;
    void __fastcall ToolButtonConnectClick(TObject *Sender);
    void __fastcall ListViewMailColumnClick(TObject *Sender,
          TListColumn *Column);
    void __fastcall ListViewMailCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
    void __fastcall ToolButtonRefreshClick(TObject *Sender);
    void __fastcall TimerRefreshTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall ToolButtonNewClick(TObject *Sender);
    void __fastcall ListViewMailDblClick(TObject *Sender);

private:	// User declarations
    // Клиенты
    POP3 clientPOP3;
    SMTP clientSMTP;

    // Параметры серверов
    string hostPOP3;
    string hostSMTP;
    string user;
    string password;
    int portPOP3;
    int portSMTP;

    void RestoreSetting();
    void SaveSetting();
    void ReadEMailList();

public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
    void Log(const char *line);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;

extern void rspLogSMTP(string &code, list<string> &message);
extern void rspLog(string &message);

//---------------------------------------------------------------------------
#endif
