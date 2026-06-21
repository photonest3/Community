#pragma once
#include <string>
#include "Singleton.h"

using namespace std;

typedef struct _lic_header
{
	unsigned char version;
	unsigned char mode;
	unsigned short num;
	unsigned short chksum;
	unsigned char ov;
	unsigned char year;
	unsigned long long date;
} lic_header;

#define MODE_GALLERY 10
//#define MODE_ENCRYPTED 20
#define MODE_CUTOUT 30

#define ERROR_SIGN 4
#define ERROR_LIC 5
#define ERROR_OVERDUE 6

class CMySoft : public Singleton<CMySoft>
{
	friend class Singleton<CMySoft>;

protected:
	CMySoft(void);
	~CMySoft(void);

public:
	int Verify(string s2, int mode, lic_header &lic);

	long add_lic(string szSubKey, string key, string val);
	void get_lic(string szSubKey, string & v3);

	int Reg(string s2, lic_header &lic);
	BOOL Grn(lic_header lic, string& sLic);

private:
	void ScrubBlob(void *b, DWORD cb);


};
