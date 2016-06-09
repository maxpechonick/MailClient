//---------------------------------------------------------------------------

#ifndef POP3H
#define POP3H
//---------------------------------------------------------------------------
#include <winsock2.h>

#include <string>
#include <list>
#include <algorithm>

using namespace std;

#define POP3_SUFF       "\r\n"	// ������� ����� ������

// ������� POP3
#define POP3_APOP       "APOP"
#define POP3_USER       "USER"
#define POP3_PASS       "PASS"
#define POP3_STAT       "STAT"
#define POP3_LIST       "LIST"
#define POP3_QUIT       "QUIT"
#define POP3_RETR       "RETR"
#define POP3_RSET       "RSET"
#define POP3_TOP        "TOP"
#define POP3_DELETE     "DELE"
#define POP3_NOOP       "NOOP"

#define RSP_OK       "+OK"
#define RSP_ERR       "-ERR"

// ������� ��������� ������ �������
typedef void (*rspHandle)(string &message);

// ����� ������ �� ��������� POP3
class POP3
{
	// ������� �������
	class command
	{
	public:
		command() {	};

		void set(string c, string p1 = "", string p2 = "")
		{
			cmd = c;
			param1 = p1;
			param2 = p2;
		}

		string toString()
		{
			string r = cmd;
			if ( param1 != "" )
			{
				r += " ";
				r += param1;
			}
			if ( param2 != "" )
			{
				r += " ";
				r += param2;
			}
			return r + POP3_SUFF;
		}

		string cmd;	// �������
		string param1;	// ���������
		string param2;
	};

	// ����� �������
	class response
	{
	public:
		response(): status(false)
		{};

		~response() {
			data.clear();
		};

		bool status;	// ��������� ������� OK/ERR
		string message;	// ���������
		list<string> data;	// ������������� ������
	};

public:
	string host;
	int port;
	string user;
	string password;
	rspHandle rHandle;

	POP3(void);
	~POP3(void);

	int connect(string &u, string &p);
	int stat(int &a, int &b);
	int top(int m, string &from, string &subject, string &date);
	int retr(int id, list<string> &message);
	int dele(int id);
	int disconnect();
    bool connected();

private:
	struct sockaddr_in sockaddr;	
	SOCKET sock;
	static string suffix;

	bool ready();
	size_t read(char* buff, size_t size);
	bool readChar(char* buffer);
	size_t readLine(string &line);

	int getResponse(response &rsp);
	int getResponseMult(response &rsp);

	static string decodeHeader(string &encoded);
};

#endif
