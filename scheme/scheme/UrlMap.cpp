#include "StdAfx.h"
#include "../../templ/templ/resource.h"
#include "UrlMap.h"

////////////////////////////////////////////////////////////////////
/// @brief 构造函数
/// @details 创建 URL 映射表对象
////////////////////////////////////////////////////////////////////
CUrlMap::CUrlMap(void)
{
	_pUrl_map = new REQ_URL_MAP();
}

////////////////////////////////////////////////////////////////////
/// @brief 析构函数
/// @details 清空并释放 URL 映射表对象
////////////////////////////////////////////////////////////////////
CUrlMap::~CUrlMap(void)
{
	_pUrl_map->clear();
	delete _pUrl_map;
}

////////////////////////////////////////////////////////////////////
/// @brief 将 URL 映射到资源 ID
/// @param[in] url 请求的 URL 路径（如 "/index.html"）
/// @return 资源 ID（如 IDR_INDEX_HTML），如果未找到映射则返回 0
/// @note 此函数是 URL 路由的核心，根据 URL 返回对应的资源 ID
////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////
/// @brief 初始化 URL 映射表
/// @return 0: 成功
/// @details 将所有的 URL 路径映射到对应的资源 ID，
///          包括 HTML 页面、JS 文件、CSS 文件等。
/// @note 此函数在服务启动时调用，初始化完成后 URL 路由即可工作
////////////////////////////////////////////////////////////////////
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
