//---------------------------------------------------------------------------

#ifndef base64H
#define base64H
//---------------------------------------------------------------------------

#include <string>

using namespace std;

string base64_encode(unsigned char const *bytes_to_encode, unsigned int len);
string base64_decode(string const &s);

#endif
