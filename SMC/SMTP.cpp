//---------------------------------------------------------------------------


#pragma hdrstop

#include "SMTP.h"
#include <time.h>
#include <stdio.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)





SMTP::SMTP(void) : host(""), port(25), sock(INVALID_SOCKET), rHandle(NULL)
{
	memset(&sockaddr, 0, sizeof(struct sockaddr_in));
};

SMTP::~SMTP(void)
{
	if ( sock != INVALID_SOCKET )
	{
		closesocket(sock);
        sock = -1;
	}
}

int SMTP::disconnect()
{
	if ( sock != INVALID_SOCKET )
	{
		closesocket(sock);
        sock = -1;
	}
    return 0;
}

int SMTP::connect(string &u, string &p)
{
	memset(&sockaddr, 0, sizeof(struct sockaddr_in));

	if ( host != "" )
	{
		struct hostent *h = gethostbyname(host.c_str());
		sockaddr.sin_family = h->h_addrtype;
		memcpy((char *) &sockaddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
	}
	else
	{
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	}
	sockaddr.sin_port = htons(port);

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}

	user = u;
    password = p;

	// ������������ � ������ �������
	if ( ::connect(sock, (struct sockaddr *) &sockaddr, sizeof(sockaddr_in)) != 0)
		return WSAGetLastError();

	command cmd;
	response rsp;
	int n_bytes;
	string ccmd;

	n_bytes = getResponse(rsp);	// ������ ������ ����� �������
	if ( n_bytes == SOCKET_ERROR )
		return WSAGetLastError();
	if ( rHandle )
        rHandle(rsp.code, rsp.message);

	return 0;	// �������� �����������
}

// �����������
int SMTP::ehlo()
{
	command cmd;
	response rsp;
	int n_bytes;
	string ccmd;

	cmd.set(SMTP_EHLO, user);

	ccmd = cmd.toString();

	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);
	if ( n_bytes == SOCKET_ERROR )
		return WSAGetLastError();

	// �������� �����
	n_bytes = getResponse(rsp);
	if ( n_bytes == SOCKET_ERROR)
		return WSAGetLastError();
	if ( rHandle )
        rHandle(rsp.code, rsp.message);
	if ( !rsp.status )
		return -3;	// �����-�� ������

	// ������ ������� �������������� � ���������
	this->esmtp.clear();
	list<string>::iterator it = rsp.message.begin();
	for ( it = rsp.message.begin();  it != rsp.message.end(); ++it )
		this->esmtp.push_back(*it);

	return 0;
}


// �����������
int SMTP::helo()
{
	command cmd;
	response rsp;
	int n_bytes;
	string ccmd;

	cmd.set(SMTP_HELO);
	ccmd = cmd.toString();

	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);
	if ( n_bytes == SOCKET_ERROR )
		return WSAGetLastError();

	// �������� �����
	n_bytes = getResponse(rsp);
	if ( n_bytes == SOCKET_ERROR)
		return WSAGetLastError();
	if ( rHandle ) rHandle(rsp.code, rsp.message);
	if ( !rsp.status )
		return -3;	// �����-�� ������

	return 0;
}

int SMTP::quit()
{
	command cmd;
	response rsp;
	int n_bytes;
	string ccmd;

	cmd.set(SMTP_QUIT);
	ccmd = cmd.toString();

	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);
	if ( n_bytes == SOCKET_ERROR )
		return WSAGetLastError();

	// �������� �����
	n_bytes = getResponse(rsp);
	if ( n_bytes == SOCKET_ERROR)
		return WSAGetLastError();
	if ( rHandle ) rHandle(rsp.code, rsp.message);
	if ( !rsp.status )
		return -3;	// �����-�� ������

	return 0;
}

int SMTP::vrfy(string &address)
{
	return 0;
}

int SMTP::mail(string &from)
{
	command cmd;
	response rsp;
	int n_bytes;
	string ccmd;

	cmd.set(SMTP_MAIL, string("FROM:")+from);
	ccmd = cmd.toString();

	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);
	if ( n_bytes == SOCKET_ERROR )
		return WSAGetLastError();

	// �������� �����
	n_bytes = getResponse(rsp);
	if ( n_bytes == SOCKET_ERROR)
		return WSAGetLastError();
	if ( rHandle ) rHandle(rsp.code, rsp.message);
	if ( !rsp.status )
		return -3;	// �����-�� ������

	return 0;
}

int SMTP::rcpt(string &to)
{
	command cmd;
	response rsp;
	int n_bytes;
	string ccmd;

	cmd.set(SMTP_RCPT, string("TO:")+to);
	ccmd = cmd.toString();

	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);
	if ( n_bytes == SOCKET_ERROR )
		return WSAGetLastError();

	// �������� �����
	n_bytes = getResponse(rsp);
	if ( n_bytes == SOCKET_ERROR)
		return WSAGetLastError();
	if ( rHandle ) rHandle(rsp.code, rsp.message);
	if ( !rsp.status )
		return -3;	// �����-�� ������

	return 0;
}

int SMTP::data(
    string &from,
    string &to,
    string &subject,
    list<string> &message,
    list<string> &attach)
{
	command cmd;
	response rsp;
	int n_bytes;
	string ccmd;

	cmd.set(SMTP_DATA);
	ccmd = cmd.toString();

	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);
	if ( n_bytes == SOCKET_ERROR )
		return WSAGetLastError();

	// �������� �����
	n_bytes = getResponse(rsp);
	if ( n_bytes == SOCKET_ERROR )
		return WSAGetLastError();
	if ( rHandle )
        rHandle(rsp.code, rsp.message);
	if ( !rsp.status )
		return -3;	// �����-�� ������


    // ��������� � ���������� �������� ���������
    cmd.set(string("From: ") + from);
	ccmd = cmd.toString();
	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);

    cmd.set(string("To: ") + to);
	ccmd = cmd.toString();
	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);

    cmd.set(string("Subject: ") + subject);
	ccmd = cmd.toString();
	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);

    // ���� ���������
	char month[][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	time_t rawtime;
	struct tm* timeinfo;
    char buff[50];

    if(time(&rawtime) > 0)
		timeinfo = localtime(&rawtime);
    sprintf(buff,"Date: %02d %s %d %02d:%02d:%02d",
        timeinfo->tm_mday,
        month[timeinfo->tm_mon],
        timeinfo->tm_year+1900,
        timeinfo->tm_hour,
        timeinfo->tm_min,
        timeinfo->tm_sec);

    cmd.set(string("Date: ") + string(buff));
	ccmd = cmd.toString();
	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);

    cmd.set(string("MIME-Version: 1.0"));
	ccmd = cmd.toString();
	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);

    if ( attach.size() == 0)
    {
        cmd.set(string("Content-Type: text/plain; charset=US-ASCII:"));
        ccmd = cmd.toString();
        n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);
    }
    else
    {
        cmd.set(string("Content-Type: multipart/mixed: boundary=\"smc boundary\""));
        ccmd = cmd.toString();
        n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);

        cmd.set(SMTP_SUFF);
        ccmd = cmd.toString();
        n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);

        cmd.set(string("--smc boundary"));
        ccmd = cmd.toString();
        n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);

        cmd.set(string("Content-Type: text/plain; charset=US-ASCII:"));
        ccmd = cmd.toString();
        n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);
    }

    cmd.set(SMTP_SUFF);
	ccmd = cmd.toString();
	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);

    // �������� ��������� ���������
    list<string>::iterator it;
    for ( it = message.begin(); it != message.end(); ++it )
    {
        cmd.set(*it);
        ccmd = cmd.toString();
        n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);
    }

    // ����� ���������
	cmd.set(SMTP_FINE);
	ccmd = cmd.toString();

	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);
	if ( n_bytes == SOCKET_ERROR )
		return WSAGetLastError();

	n_bytes = getResponse(rsp);
	if ( n_bytes == SOCKET_ERROR)
		return WSAGetLastError();
	if ( rHandle ) rHandle(rsp.code, rsp.message);
	if ( !rsp.status )
		return -3;	// �����-�� ������

	return 0;
}

// ������������ �������
int SMTP::cmnd(string cmdt)
{
	command cmd;
	response rsp;
	int n_bytes;
	string ccmd;

	cmd.set(cmdt);
	ccmd = cmd.toString();

	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);
	if ( n_bytes == SOCKET_ERROR )
		return WSAGetLastError();

	// �������� �����
	n_bytes = getResponse(rsp);
	if ( n_bytes == SOCKET_ERROR)
		return WSAGetLastError();
	if ( rHandle ) rHandle(rsp.code, rsp.message);
	if ( !rsp.status )
		return -3;	// �����-�� ������

	return 0;
}

int SMTP::rset()
{
	return 0;
}

// �������� �� ����������� ������
bool SMTP::ready()
{
	fd_set recieveFd;
	struct timeval timeout;

	FD_ZERO(&recieveFd);
	FD_SET(sock, &recieveFd);

	// �������� �������� 10 ������
	timeout.tv_sec = 1000000;
	timeout.tv_usec = 0;

	int rc = select(sock + 1, &recieveFd, NULL, NULL, &timeout);

	return (rc > 0);
}

// ������ ����� �������� �����
size_t SMTP::read(char* buff, size_t size)
{
	if (!ready()) return -1;
	return ::recv(sock, buff, size, 0);
}

// ������ �����
bool SMTP::readChar(char* buffer)
{
	return (read(buffer, 1) > 0);
}

// ������ �� ����� ������
size_t SMTP::readLine(string &line)
{
	char buffer[2] = "";
	line = "";
	size_t bytesRead = 0;
	int rc;

	rc = readChar(&(buffer[1]));
	if ( rc )
	{
		do
		{
			line += buffer[0]; 
			buffer[0] = buffer[1];
			bytesRead++;
		}
		while (readChar(&(buffer[1])) &&
			!(buffer[0] == '\r' && buffer[1] == '\n'));
		line.erase(0, 1);
	}
	return bytesRead;
}

// ������ �������������� ������
int SMTP::getResponse(response &rsp)
{
	string buffer;
	char delim;

	int rc = readLine(buffer);
	if ( rc < 0 ) return SOCKET_ERROR;

	// ��������� 3-������� ��� ������
	rsp.code = buffer.substr(0, 3);
	delim = buffer[3];
	buffer.erase(0, 4); 
	
	rsp.message.clear();
	rsp.message.push_back(buffer);

	// ������������� ������
	rsp.status = (rsp.code[0] == '2' || rsp.code[0] == '3'); 

	// ������ �� ������, ��� ��� ���������� �������� � �� �������
	while ( delim != ' ' ) 
	{
		int rc = readLine(buffer);
		if ( rc < 0 ) break;

		delim = buffer[3];
		buffer.erase(0, 4); 
		rsp.message.push_back(buffer);
	} 
	return rc;
}

