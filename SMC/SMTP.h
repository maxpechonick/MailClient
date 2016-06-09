//---------------------------------------------------------------------------

#ifndef SMTPH
#define SMTPH

#include <winsock2.h>

#include <string>
#include <list>
#include <algorithm>

using namespace std;

#define SMTP_SUFF       "\r\n"	// ������� ����� ������
#define SMTP_FINE       "."	// ����� ���������

// ������� SMTP
#define SMTP_EHLO       "EHLO"
#define SMTP_HELO       "HELO"
#define SMTP_MAIL       "MAIL"
#define SMTP_RCPT       "RCPT"
#define SMTP_DATA       "DATA"
#define SMTP_QUIT       "QUIT"
#define SMTP_HELP       "HELP"
#define SMTP_VRFY       "VRFY"
#define SMTP_NOOP       "NOOP"
#define SMTP_RSET       "RSET"
#define SMTP_SEND       "SEND"
#define SMTP_TURM       "TURM"


typedef void (*rspHandleSMTP)(string &code, list<string> &message);

// ����� ������ �� ��������� SMTP
class SMTP
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
			return r + SMTP_SUFF;
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
			message.clear();
		};

		bool status;	// ������������� ������ - ������ ��� ������
		string code;	// ��� ������
		list<string> message;	// ���������
	};

public:
	string host;	// ���� �������
	int port;		// ���� �������
	list<string> esmtp;	// ��������� ���������� SMTP, ������� ������ 

	string user;
	string password;

	rspHandleSMTP rHandle;

	SMTP(void);
	~SMTP(void);

	int connect(string &u, string &p);
	int disconnect();
	int ehlo();
	int helo();
	int quit();
	int vrfy(string &address);
	int mail(string &from);
	int rcpt(string &to);
	int data(string &from, string &to, string &subject, list<string> &message, list<string> &attach);
	int rset();
	int cmnd(string cmdt);

private:
	struct sockaddr_in sockaddr;	
	SOCKET sock;
	static string suffix;

	bool ready();
	size_t read(char* buff, size_t size);
	bool readChar(char* buffer);
	size_t readLine(string &line);

	int getResponse(response &rsp);
};


//---------------------------------------------------------------------------
#endif
