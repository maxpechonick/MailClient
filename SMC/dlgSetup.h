//---------------------------------------------------------------------------

#ifndef dlgSetupH
#define dlgSetupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
    TButton *ButtonSave;
    TButton *ButtonCancel;
    TLabel *Label1;
    TEdit *EditDir;
    TOpenDialog *OpenDialog1;
private:	// User declarations
public:		// User declarations
    __fastcall TForm3(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
