#pragma once


#include <string>
#include <map>
#include "Singleton.h"

#include "Blowfish.h"

using namespace std;

class CUploadParser: public Singleton<CUploadParser>
{
	friend class Singleton<CUploadParser>;
protected:
	CUploadParser(void);
	~CUploadParser(void);

public:
	void SetKey(string enckey);
	string GetKey();
	int Decryptfile(wstring src, wstring Decryptname);
	int Encryptfile(wstring src, wstring Encryptname);

private:
	string _enckey;
	CBlowfish  * _pBF;
};
