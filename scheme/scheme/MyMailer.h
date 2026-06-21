#pragma once

#include "stmdef.h"

class CMyMailer 
{
public:
	CMyMailer(void);
	~CMyMailer(void);

public:

	long SendMail(string recvName, string recvMail, string content, string &Result);

private:
	long SendMail(string username, string password, string recvName, string recvMail, string content, string &Result);
	string decrypt_pwd(string opass);

private:
	string GetHttpDate();

	SMTP _smtp;

};
