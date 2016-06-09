//----------------------------------------------------------------------------
#ifndef dlgConnectH
#define dlgConnectH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
#include <ComCtrls.hpp>
//----------------------------------------------------------------------------
class TOKBottomDlg : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
    TGroupBox *GroupBox1;
    TEdit *EditHostPOP3;
    TLabel *Label1;
    TLabel *Label2;
    TUpDown *UpDownPOP3;
    TEdit *EditPortPOP3;
    TGroupBox *GroupBox2;
    TLabel *Label3;
    TLabel *Label4;
    TEdit *EditHostSMTP;
    TUpDown *UpDownSMTP;
    TEdit *EditPortSMTP;
    TGroupBox *GroupBox3;
    TLabel *Label5;
    TLabel *Label6;
    TEdit *EditUser;
    TEdit *EditPassword;
private:
public:
	virtual __fastcall TOKBottomDlg(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TOKBottomDlg *OKBottomDlg;
//----------------------------------------------------------------------------
#endif    
