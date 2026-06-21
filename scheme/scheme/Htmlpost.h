#pragma once

#include "stmdef.h"
#include "Singleton.h"

class CHtmlpost : public Singleton<CHtmlpost> 
{
	friend class Singleton<CHtmlpost>;
protected:
	CHtmlpost(void);
	~CHtmlpost(void);

public:
	long mypostfunc(REQUEST_DTO request_dto, RESPONSE_DTO &response_dto);
};
