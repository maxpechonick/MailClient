//---------------------------------------------------------------------------


#pragma hdrstop

#include "Log.h"

void __fastcall rspLogSMTP(string &code, list<string> &message)
{
	list<string>::iterator it = message.begin();
    Form1->Log(("������: " + code+": "+*it).c_str());

	for ( ++it;  it != message.end(); ++it )
        Form1->Log(("������: " + *it).c_str());
}

void __fastcall rspLog(string &message)
{
    Form1->Log(("������: " + message).c_str());
}


//---------------------------------------------------------------------------

#pragma package(smart_init)
 