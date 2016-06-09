//---------------------------------------------------------------------------

#ifndef LogH
#define LogH
//---------------------------------------------------------------------------

#include <string>
#include <list>

using namespace std;

void __fastcall rspLogSMTP(string &code, list<string> &message);
void __fastcall rspLog(string &message);

#endif
