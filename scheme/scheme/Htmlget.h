#pragma once

#include "stmdef.h"
#include "Singleton.h"

class CHtmlget : public Singleton<CHtmlget>
{
	friend class Singleton<CHtmlget>;
protected:
	CHtmlget(void);
	~CHtmlget(void);

public:
	long mygetfunc(REQUEST_DTO &request_dto, RESPONSE_DTO &response_dto);

private:
	bool isurlok(string val, string& szUrl, string& szArgs);
	bool use_main_url();
	bool _is_firstrun = false;

};