//---------------------------------------------------------------------------


#pragma hdrstop

#include "QuotedPrintable.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

#define SKIP '\202'

const char hexmap[] = {
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
      0 ,    1 ,    2 ,    3 ,    4 ,    5 ,    6 ,    7 ,
      8 ,    9 ,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,    10,    11,    12,    13,    14,    15,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
     SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP
};


string qp_decode(string const &str)
{
//char* CQPUtils::Decode(char *input)
  char *input = new char[str.length()+1];
  strcpy(input, str.c_str());

  char *s = input;
  char *finalresult = (char*)calloc(strlen(input) + sizeof(char), sizeof(char));
  char *result = finalresult;
  while (*s != '\0')
  {
    if (*s == '=')
    {
      for (int i = 0; i < 3; i++)
      {
        if (s[i] == '\0')
        {
          return string(finalresult);
        }
      }
      char mid[3];
      s++;
      bool ok = true;
      for (int i = 0; i < 2; i++)
      {
        if (hexmap[s[i]] == SKIP)
        {
          ok = false;
          if (s[i] == '\r' && s[i + 1] == '\n')
          {
            s += 2;
            break;
          }
          else
          {
            ;
          }
        }
        mid[i] = s[i];
      }

      if (ok)
      {
        s += 2;
        int m = hexmap[mid[0]];
        m <<= 4;
        m |= hexmap[mid[1]];
        *(result++) = m;
      }
    }
    else
    {
      if (*s != '\0') *(result++) = *(s++);
    }
  }

  delete []input;
  return string(finalresult);
}
