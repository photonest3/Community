#include "StdAfx.h"
#include "../../templ/templ/resource.h"
#include "Htmlpost.h"

#include "UrlMap.h"
#include "Htmlupload.h"
#include "HtmlgetAdmin.h"
#include "HtmlgetIndex.h"

CHtmlpost::CHtmlpost(void)
{
}

CHtmlpost::~CHtmlpost(void)
{
}

long CHtmlpost::mypostfunc(REQUEST_DTO request_dto, RESPONSE_DTO &response_dto)
{
	UINT res = CUrlMap::Instance().mapUrl2Int(request_dto.szUrl);
	HRESULT hr = S_FALSE;

	switch (res)
	{
	case LGC_UPLOAD_HTML:		//upload.html
	{
		CHtmlupload::Instance().post_upload_htm(request_dto, response_dto);
		hr = S_OK;
		break;
	}
	case LGC_WS_PHP:		//ws.php
	{
		CHtmlgetAdmin::Instance().post_ws_php(request_dto, response_dto);
		hr = S_OK;
		break;
	}
	case IDR_ADMIN_HTML:		//ws.php
	{
		CHtmlgetAdmin::Instance().post_admin_htm(request_dto, response_dto);
		hr = S_OK;
		break;
	}
	case IDR_INDEX_HTML:		//index.html
	{
		//searchform
		CHtmlgetIndex::Instance().get_index_htm(request_dto, response_dto);
		hr = S_OK;
		break;
	}
	default:
		break;
	}

	return S_OK;
}
