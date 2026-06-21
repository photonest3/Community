#pragma once

#include <string>
#include <map>

using namespace std;

typedef map<wstring, string, less<wstring> > MIMETYPES;

class CMimeType
{
public:
	CMimeType(void);
public:
	~CMimeType(void);

	void InitializeMime();
	bool GetMime(wstring ext, string &type);

public:
	MIMETYPES		*_pMimeTypes;

};
