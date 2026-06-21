#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"


class CHtmlgetPic : public CImBase, public Singleton<CHtmlgetPic>
{
	friend class Singleton<CHtmlgetPic>;
protected:
	CHtmlgetPic(void);
	~CHtmlgetPic(void);

public:
	long get_picture_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);

private:
	long get_picture_template(const REQUEST_DTO request_dto, string& tpl);
	string get_picid(const REQUEST_DTO request_dto);
	long get_curr_images(list<string> plstImageIds, string idx0, string& picid, int& idx, BOOL& bfav);

};