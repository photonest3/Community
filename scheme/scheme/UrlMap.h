#pragma once
#include "stmdef.h"
#include "Singleton.h"

#define LGC_PAGE_HTML					1510
#define LGC_ACTION_HTML                 1520
#define LGC_UPLOAD_HTML                 1530
#define LGC_WS_PHP                      1540
#define LGC_A_HTML                      1550
#define LGC_B_HTML                      1560
#define LGC_PAINT		                1570
#define IDR_LOCAL_THUMB		            1580

class CUrlMap : public Singleton<CUrlMap>
{
	friend class Singleton<CUrlMap>;
protected:

	CUrlMap(void);
	~CUrlMap(void);

public:
	UINT mapUrl2Int(string url);
	long Init_url_map();

private:
	REQ_URL_MAP * _pUrl_map;
};