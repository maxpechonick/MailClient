//---------------------------------------------------------------------------

#ifndef LetterH
#define LetterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TEdit *EditTo;
    TLabel *Label3;
    TEdit *EditSubject;
    TButton *ButtonSend;
    TMemo *mlText;
    TLabel *Label2;
    TOpenDialog *OpenDialogAttach;
    TEdit *EditAttach;
    TSpeedButton *SpeedButtonOpen;
    void __fastcall ButtonSendClick(TObject *Sender);
    void __fastcall SpeedButtonOpenClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
