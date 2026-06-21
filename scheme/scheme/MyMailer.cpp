#include "StdAfx.h"
//#include "MyMailer.h"
#include <iostream>
#include <process.h>

#include "unitil.h"
#include "../../curl-7.65.3/include/curl/curl.h"
#include "Blowfish.h"
#include "chatdal.h"


#pragma comment( lib, "Crypt32.lib" ) 
#pragma comment( lib, "wldap32.lib" ) 
#pragma comment(lib,"Ws2_32.lib")

#ifdef _DEBUG
#pragma comment( lib, "../../libs/debug/libcurld.lib" ) 
#else
#pragma comment( lib, "../../curl-7.65.3/build/Win32/libcurl.lib" ) 
#endif



static string m_payload_text = "";

struct upload_status {
	int lines_read;
};

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
	struct upload_status *upload_ctx = (struct upload_status *)userp;

	if ((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
		return 0;
	}

	if (!m_payload_text.empty()) {
		size_t len = m_payload_text.length();
		memcpy(ptr, m_payload_text.c_str(), len);
		upload_ctx->lines_read++;
		m_payload_text = "";
		return len;
	}

	return 0;
}

CMyMailer::CMyMailer(void)
{
	string ids = "'smtp.servr','smtp.sender','smtp.username','smtp.password'";
	list<CONFIG_DTO> lstConfig;
	Cchatdal::Instance().get_configs(ids, lstConfig);

	list<CONFIG_DTO>::iterator iter;
	for (iter = lstConfig.begin(); iter != lstConfig.end(); iter++)
	{
		if (iter->param == "smtp.servr")
		{
			_smtp.smtpservr = iter->value;
		}
		//else if (iter->param == "smtp.sender")
		//{
		//	_smtp.sender = iter->value;
		//}
		else if (iter->param == "smtp.username")
		{
			_smtp.username = iter->value;
		}
		else if (iter->param == "smtp.password")
		{
			//string tbPassword = decrypt_pwd(iter->value);
			_smtp.password = iter->value;
		}
	}
}

CMyMailer::~CMyMailer(void)
{
}

long CMyMailer::SendMail(string recvName, string recvMail, string content, string &Result)
{
	if (_smtp.smtpservr == "")
	{
		Result = "smtp";
		return 0;
	}
	//string sender = _smtp.sender;
	string smtpservr = _smtp.smtpservr;
	string authname = "", password = "";
	//if (_smtp.auth_type == "1")
	{
		authname = _smtp.username;
		password = _smtp.password;
		password = decrypt_pwd(password);
	}

	Replace(content,"%FROMADDR%", authname);
	Replace(content,"%FROMNAME%", "Encryption Galleries");

	long ret = SendMail(authname, password, recvName, recvMail, content, Result);
	return ret;
}

long CMyMailer::SendMail(string username, string password, string recvName, string recvMail, string content, string &Result)
{
	if ("mail.yourcompany.com" == _smtp.smtpservr || _smtp.smtpservr.empty())
	{
		Result = "Outgoing mail server (SMTP) infomation error";
		return 1;
	}

	string smtpservr = "smtp://" + _smtp.smtpservr;

	Result = "Successful";

	string sender = username;
	sender = "<" + sender + ">";
	recvMail = "<" + recvMail + ">";


	//		"Date: Mon, 29 Nov 2010 21:54:29 +1100\r\n",
	//		"To:  TO \r\n",
	//		"From:  FROM (Example User)\r\n",
	//		"Cc: CC(Another example User)\r\n",
	//		"Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@"
	//		"rfcpedant.example.org>\r\n",
	//		"Subject: SMTP example message\r\n",
	//		"\r\n", /* empty line to divide headers from body, see RFC5322 */
	//		"The body of the message starts here.\r\n",
	//		"\r\n",
	//		"It could be a lot of lines, could be MIME encoded, whatever.\r\n",
	//		"Check RFC5322.\r\n"


	string date = "Date: " + GetHttpDate() + "\r\n";
	string to = "To: " + recvMail + "\r\n";
	string from = "From: " + sender + "(Encryption Galleries)\r\n";
	//string msgid = "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@rfcpedant.example.org>\r\n";
	string body = content + "\r\n";

	m_payload_text = "";

	m_payload_text = date;
	m_payload_text += to;
	m_payload_text += from;
	//payload_text += msgid;
	m_payload_text += "Subject: ACCOUNT INFORMATION\r\n";
	m_payload_text += "\r\n";
	m_payload_text += body;
	m_payload_text += "\r\n";

	CURL *curl;
	CURLcode res = CURLE_OK;
	struct curl_slist *recipients = NULL;
	struct upload_status upload_ctx;

	upload_ctx.lines_read = 0;

	curl = curl_easy_init();
	if (curl) {
		/* This is the URL for your mailserver */
		//curl_easy_setopt(curl, CURLOPT_URL, "smtp://mail.example.com");

		curl_easy_setopt(curl, CURLOPT_URL, smtpservr.c_str());

		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3L);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, (long)1);

		/* Note that this option isn't strictly required, omitting it will result
		* in libcurl sending the MAIL FROM command with empty sender data. All
		* autoresponses should have an empty reverse-path, and should be directed
		* to the address in the reverse-path which triggered them. Otherwise,
		* they could cause an endless loop. See RFC 5321 Section 4.5.5 for more
		* details.
		*/

		string user = username + ":" + password;
		curl_easy_setopt(curl, CURLOPT_USERPWD, user.c_str());// "username:userpassword");

		curl_easy_setopt(curl, CURLOPT_MAIL_FROM, sender.c_str());

		/* Add two recipients, in this particular case they correspond to the
		* To: and Cc: addressees in the header, but they could be any kind of
		* recipient. */

		//recipients = curl_slist_append(recipients, TO);
		recipients = curl_slist_append(recipients, recvMail.c_str());

		//recipients = curl_slist_append(recipients, CC);
		curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

		/* We're using a callback function to specify the payload (the headers and
		* body of the message). You could just use the CURLOPT_READDATA option to
		* specify a FILE pointer to read from. */
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
		curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

		/* Send the message */
		res = curl_easy_perform(curl);

		/* Check for errors */
		if (res != CURLE_OK)
		{
			//fprintf(stderr, "curl_easy_perform() failed: %s\n",
			//	curl_easy_strerror(res));

			Result = curl_easy_strerror(res);
		}

		/* Free the list of recipients */
		curl_slist_free_all(recipients);

		/* curl won't send the QUIT command until you call cleanup, so you should
		* be able to re-use this connection for additional messages (setting
		* CURLOPT_MAIL_FROM and CURLOPT_MAIL_RCPT as required, and calling
		* curl_easy_perform() again. It may not be a good idea to keep the
		* connection open for a very long time though (more than a few minutes
		* may result in the server timing out the connection), and you do want to
		* clean up in the end.
		*/
		curl_easy_cleanup(curl);
	}

	//Result = Replace(Result, "'", "\\'");

	return res;
}

string CMyMailer::GetHttpDate()
{
	static const char day_names[7][4] =
	{ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	static const char month_names[12][4] =
	{ "Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	SYSTEMTIME st;
	GetSystemTime(&st);

	string ret = "Sun, 06 Nov 1994 08:49:37 GMT";

	char *buf = (char*)malloc(1024);
	if (buf != NULL)
	{
		memset(buf, 0, 1024);

		// Format: "Sun, 06 Nov 1994 08:49:37 GMT" 
		sprintf_s(buf, 1024,
			"%s, %02u %s %4u %02u:%02u:%02u GMT",
			day_names[st.wDayOfWeek],
			st.wDay,
			month_names[st.wMonth - 1],
			st.wYear,
			st.wHour,
			st.wMinute,
			st.wSecond
			);

		ret = buf;
		free(buf);
	}
	return ret;
}

string CMyMailer::decrypt_pwd(string opass)
{
	string pass = "";
	CBlowfish  * m_pBF = new CBlowfish();
	if (m_pBF != NULL)
	{
		m_pBF->SetPassword(BF_PASSWORD);

		unsigned char buf3[2048];
		memset(buf3, 0, 2048);
		hexchange(buf3, opass.c_str());

		m_pBF->Decrypt((void*)buf3, opass.length());

		pass = (char*)buf3;

		delete m_pBF;
		m_pBF = NULL;
	}

	return pass;
}