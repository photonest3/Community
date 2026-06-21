#include "StdAfx.h"
#include "../../templ/templ/resource.h"
#include "UrlMap.h"

CUrlMap::CUrlMap(void)
{
	_pUrl_map = new REQ_URL_MAP();
}

CUrlMap::~CUrlMap(void)
{
	_pUrl_map->clear();
	delete _pUrl_map;
}


UINT CUrlMap::mapUrl2Int(string url)
{
	UINT res = 0;

	REQ_URL_MAP::iterator iter = _pUrl_map->find(url);
	if (iter != _pUrl_map->end())
	{
		res = iter->second;
	}
	return res;
}

long CUrlMap::Init_url_map()
{
	_pUrl_map->clear();
	_pUrl_map->insert(REQ_URL_MAP::value_type("/index.html", IDR_INDEX_HTML));

	_pUrl_map->insert(REQ_URL_MAP::value_type("/admin.html", IDR_ADMIN_HTML));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/picture.html", IDR_PICTURE_HTML));

	_pUrl_map->insert(REQ_URL_MAP::value_type("/art_film.html", IDR_ART_FILM_HTML));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/art_zoom.html", IDR_ART_ZOOM_HTML));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/film_box.html", IDR_FILM_BOX_HTML));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/view.html", IDR_VIEW_HTML));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/delbox.html", IDR_DELBOX_HTML));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/register.html", IDR_PRV_REGISTER_HTML));

	_pUrl_map->insert(REQ_URL_MAP::value_type("/page.html", LGC_PAGE_HTML));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/a.html", LGC_A_HTML));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/b.html", LGC_B_HTML));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/last.html", IDR_LAST_HTML));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/upload.html", LGC_UPLOAD_HTML));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/ws.php", LGC_WS_PHP));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/action.html", LGC_ACTION_HTML));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/lgc_paint.html", LGC_PAINT));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/local_thumb", IDR_LOCAL_THUMB));

	_pUrl_map->insert(REQ_URL_MAP::value_type("/_index.js", IDR_INDEX_JS));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/_picture.js", IDR_PICTURE_JS));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/_gallery.js", IDR_GALLERY_JS));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/_batch.js", IDR_BATCH_JS));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/_first.js", IDR_FIRST_JS));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/_view.js", IDR_VIEW_JS));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/_art_film.js", IDR_ART_FILM_JS));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/jquery.fileupload-ui.u3.min.js", IDR_ADMIN_ADD_JS));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/jquery.watermark.js", IDR_WATERMARK_JS));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/jquery.watermark_img.js", IDR_WATERMARK_IMG_JS));
	_pUrl_map->insert(REQ_URL_MAP::value_type("/admin.skin.css", IDR_ADMIN_SKIN_CSS));

	return 0;
}
