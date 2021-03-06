//---------------------------------------------------------------------------


#pragma hdrstop

#include "POP3.h"
#include "base64.h"
#include "QuotedPrintable.h"

#include <stdio.h>

#include <string.h>
#include <SysUtils.hpp>


//---------------------------------------------------------------------------

#pragma package(smart_init)


POP3::POP3(void): host(""), port(110), sock(INVALID_SOCKET), rHandle(NULL)
{
	memset(&sockaddr, 0, sizeof(struct sockaddr_in));
};

POP3::~POP3(void)
{
	if ( sock != INVALID_SOCKET )
	{
		closesocket(sock);
        sock = -1;
	}
}

bool POP3::connected()
{
    return (sock != INVALID_SOCKET);
}

int POP3::connect(string &u, string &p)
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
        sock = INVALID_SOCKET;
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
        rHandle(rsp.message);
	if ( !rsp.status )	// ������ ������ �����������
		return -1;

	cmd.set(POP3_USER, user);	// �������� ������� USER
	ccmd = cmd.toString();

	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);
	if ( n_bytes == SOCKET_ERROR )
		return WSAGetLastError();

	// �������� �����
	n_bytes = getResponse(rsp);
	if ( n_bytes == SOCKET_ERROR )
		return WSAGetLastError();
	if ( rHandle ) rHandle(rsp.message);
	if ( !rsp.status )
		return -2;	// �������� ��� ������������

	cmd.set(POP3_PASS, password);	// �������� ������� PASS
	ccmd = cmd.toString();

	n_bytes = ::send (sock, ccmd.data(), ccmd.size(), 0);
	if ( n_bytes == SOCKET_ERROR )
		return WSAGetLastError();

	n_bytes = getResponse(rsp);	// �������� �����
	if ( n_bytes == SOCKET_ERROR )
		return WSAGetLastError();
	if ( rHandle ) rHandle(rsp.message);
	if ( !rsp.status )
		return -3;	// �������� ������

	return 0;	// �������� �����������
}

// �������� �� ����������� ������
bool POP3::ready()
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
size_t POP3::read(char* buff, size_t size)
{
	if (!ready()) return -1;
	return ::recv(sock, buff, size, 0);
}

// ������ �����
bool POP3::readChar(char* buffer)
{
	return (read(buffer, 1) > 0);
}

// ������ �� ����� ������
size_t POP3::readLine(string &line)
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

// ������ ������������� ������
int POP3::getResponse(response &rsp)
{
	string buffer;
	int rc = readLine(buffer);
	if ( rc <= 0 ) return SOCKET_ERROR;

	if (buffer[0] == '+')
	{
		rsp.status = true;
		buffer.erase(0, 4); // ������� +OK
	}
	else
	{
		rsp.status = false;
		buffer.erase(0, 5); // ������� -ERR 
	}

	rsp.message = buffer;
	rsp.data.clear();
	return rc;
}

// ������ �������������� ������
int POP3::getResponseMult(response &rsp)
{
	string buffer;
	int rc = readLine(buffer);
	if ( rc < 0 ) return SOCKET_ERROR;

	if (buffer[0] == '+')
	{
		rsp.status = true;
		buffer.erase(0, 4); // ������� +OK
	}
	else
	{
		rsp.status = false;
		buffer.erase(0, 5); // ������� -ERR 
	}

	rsp.message = buffer;
	rsp.data.clear();
	rsp.data.push_back(buffer);

	while ( buffer != "." ) 
	{
		int rc = readLine(buffer);
		if ( rc < 0 ) break;
		rsp.data.push_back(buffer);
	} 
	return rc;
}

// ���������� �����
int POP3::stat(int &a, int &b)
{
	command cmd;
	response rsp;
	int n_bytes;
	string ccmd;

	cmd.set(POP3_STAT);
	ccmd = cmd.toString();

	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);
	if ( n_bytes == SOCKET_ERROR )
		return WSAGetLastError();

	// �������� �����
	n_bytes = getResponse(rsp);
	if ( n_bytes == SOCKET_ERROR)
		return WSAGetLastError();
	if ( rHandle ) rHandle(rsp.message);
	if ( !rsp.status )
		return -3;	// �����-�� ������

	// �������� ������ �� ������
	sscanf(rsp.message.c_str(), "%d %d", &a, &b);

	return 0;
}

// ��������� ���������
int POP3::top(int m, string &from, string &subject, string &date)
{
	command cmd;
	response rsp;
	char b[10];  
	int n_bytes;
	string ccmd;

	itoa(m, b, 10);
	cmd.set(POP3_TOP, string(b), "0");
	ccmd = cmd.toString();

	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);
	if ( n_bytes == SOCKET_ERROR)
		return WSAGetLastError();

	// �������� ����� �� �����
	getResponseMult(rsp);
	if ( n_bytes == SOCKET_ERROR )
		return WSAGetLastError();
	if ( !rsp.status )
		return -3;	// �����-�� ������

	from.clear(); subject.clear(); date.clear();

	list<string>::iterator it;
	for ( it = rsp.data.begin(); it != rsp.data.end(); ++it )
	{
		if ( !(it->compare(0, 5, "From:")) ) 
			from = decodeHeader(*it);
		else if ( !(it->compare(0, 8, "Subject:")) )
			subject = decodeHeader(*it);
		else if ( !(it->compare(0, 5, "Date:")) ) 
			date = decodeHeader(*it);
	}
	return 0;
}

int POP3::dele(int id)
{
	command cmd;
	response rsp;
	int n_bytes;
	string ccmd;
    char buff[10];

    itoa(id, buff, 10);
	cmd.set(POP3_DELETE, string(buff));
	ccmd = cmd.toString();

	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);
	if ( n_bytes == SOCKET_ERROR )
		return WSAGetLastError();

	// �������� �����
	n_bytes = getResponse(rsp);
	if ( n_bytes == SOCKET_ERROR)
		return WSAGetLastError();
	if ( rHandle ) rHandle(rsp.message);
	if ( !rsp.status )
		return -3;	// �����-�� ������

	// �������� ������ �� ������
	return 0;
}

int POP3::retr(int id, list<string> &message)
{
	command cmd;
	response rsp;
	int n_bytes;
	string ccmd;
    char buff[10];

    itoa(id, buff, 10);
	cmd.set(POP3_RETR, string(buff));
	ccmd = cmd.toString();

	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);
	if ( n_bytes == SOCKET_ERROR )
		return WSAGetLastError();

	// �������� �����
	n_bytes = getResponseMult(rsp);
	if ( n_bytes == SOCKET_ERROR)
		return WSAGetLastError();
	if ( rHandle ) rHandle(rsp.message);
	if ( !rsp.status )
		return -3;	// �����-�� ������

	// �������� ������ �� ������
    list<string>::iterator it;
    for ( it = rsp.data.begin(); it != rsp.data.end(); ++it )
        message.push_back(*it);
	return 0;
}

// ������� ������������� ���������

string POP3::decodeHeader(string &encoded)
{
	string enc = encoded;

	int start = 0;
	int end = 0;

	end = enc.find(":", start);
	if ( end < 0 )
		return encoded;

	start = end;
	end = enc.find("=?", start);
	if ( end < 0 )
		return enc.substr(start+1);
	start = end+2;

	// ������� ��������
	string cp;
	end = enc.find("?", start);
	cp = enc.substr(start, end-start);
	transform(cp.begin(), cp.end(), cp.begin(), ::toupper);

	// ����� �����������
	string em;
	start = end+1;
	end = enc.find("?", start);
	em = enc.substr(start, end-start);
	transform(em.begin(), em.end(), em.begin(), ::toupper);

	// ���� ���������
	string mess;
	start = end+1;
	end = enc.find("?=", start);
	mess = enc.substr(start, end-start);

	// �������������� �������
	string rest;
	start = end+2;
	rest = enc.substr(start);

	// �������������
	string dec;
	if ( em == "B" )
		dec = base64_decode(mess+string("?="));
	else if ( em == "Q" )
		dec = qp_decode(mess);
	else
		dec = mess;

    int cpn = 20866;
    AnsiString as = AnsiString(dec.c_str());

    if ( cp == "UTF-8" )
    {
        // ���� ������� ������������� �� Borland
        as = Utf8ToAnsi(as);
        dec = as.c_str();
    }
	else
	{
        // ���������� ����� ������� ��������
        if ( cp == "KOI8-R" )
            cpn = 20866;
        else if ( cp == "KOI8-U" )
            cpn = 21866;
        else if ( cp == "UTF-16" )
            cpn = 1200;
        else if ( cp == "ISO-8859-5" )
            cpn = 28595;
        else if ( cp == "WINDOWS-1251" )
            cpn = 1251;

        int b = MultiByteToWideChar(cpn, 0, as.c_str(), -1, NULL, 0);
        wchar_t* temp = new wchar_t[b];
        MultiByteToWideChar(cpn, 0, as.c_str(), -1, temp, b);

        char* temp2 = new char[as.Length()+1];

        WideCharToMultiByte(1251, 0, temp, -1, temp2, as.Length(), NULL, NULL);
        temp2[as.Length()] = 0;
        dec = AnsiString(temp2).c_str();
        delete []temp;
        delete []temp2;
    }

	return dec + rest;
}

int POP3::disconnect()
{
	// ������� ����������
	command cmd;
	response rsp;
	int n_bytes;
	string ccmd;

	cmd.set(POP3_QUIT);
	ccmd = cmd.toString();

	n_bytes = ::send(sock, ccmd.data(), ccmd.size(), 0);
	if ( n_bytes == SOCKET_ERROR)
		return WSAGetLastError();

	// �������� �����
	n_bytes = getResponse(rsp);
	if ( n_bytes == SOCKET_ERROR)
		return WSAGetLastError();
	if ( rHandle ) rHandle(rsp.message);

	return 0;
}

string POP3::suffix = POP3_SUFF;
