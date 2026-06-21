#pragma once

#include "ImBase.h"
#include "stmdef.h"
#include "Singleton.h"
#include "exif.h"

class CHtmlexif : public CImBase, public Singleton<CHtmlexif>
{
	friend class Singleton<CHtmlexif>;
protected:
	CHtmlexif(void);
	~CHtmlexif(void);

public:
	long get_exif_value(wstring filename, string type, map<string, string> &attr);
	long get_exif(wstring szSrc, EXIFInfo &result);

};
