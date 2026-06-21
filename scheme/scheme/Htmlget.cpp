#include "StdAfx.h"
#include "Htmlget.h"
#include "../../templ/templ/resource.h"
#include "UrlMap.h"
#include "DataSrv.h"
#include "HtmlgetIndex.h"
#include "HtmlgetAdmin.h"
#include "HtmlgetPic.h"
#include "HtmlgetIndexProc.h"
#include "imlog.h"
#include "chatdal.h"
#include "unitil.h"
#include "writedb.h"
#include "HtmlgetViewer.h"


CHtmlget::CHtmlget()
{
	_is_firstrun = false;
}

CHtmlget::~CHtmlget()
{
}

long CHtmlget::mygetfunc(REQUEST_DTO& request_dto, RESPONSE_DTO& response_dto)
{
	UINT res = CUrlMap::Instance().mapUrl2Int(request_dto.szUrl);
	HRESULT hr = S_FALSE;

	switch (res)
	{
	case IDR_VIEW_HTML:		//view.html
	{
		CHtmlgetViewer::Instance().get_viewer_htm(request_dto, response_dto);
		hr = S_OK;
		break;
	}
	case LGC_ACTION_HTML:		//action.html
	{
		CHtmlgetIndexProc::Instance().get_action(request_dto, response_dto);
		hr = S_OK;
		break;
	}
	case LGC_WS_PHP:		//ws.php
	{
		CHtmlgetAdmin::Instance().get_ws_php(request_dto, response_dto);
		hr = S_OK;
		break;
	}
	case IDR_INDEX_JS:		//_index.js
	{
		CHtmlgetIndex::Instance().get_js(IDR_INDEX_JS, L"html/_index.js", response_dto);
		return S_OK;
	}
	case IDR_PICTURE_JS:		//_picture.js
	{
		CHtmlgetIndex::Instance().get_js(IDR_PICTURE_JS, L"html/_picture.js", response_dto);
		return S_OK;
	}
	case IDR_GALLERY_JS:		//_gallery.js
	{
		CHtmlgetIndex::Instance().get_js(IDR_GALLERY_JS, L"html/_gallery.js", response_dto);
		return S_OK;
	}
	case IDR_BATCH_JS:		//_batch.js
	{
		CHtmlgetIndex::Instance().get_js(IDR_BATCH_JS, L"html/_batch.js", response_dto);
		return S_OK;
	}
	case IDR_FIRST_JS:		//_first.js
	{
		CHtmlgetIndex::Instance().get_js(IDR_FIRST_JS, L"html/_first.js", response_dto);
		return S_OK;
	}
	case IDR_VIEW_JS:		//_view.js
	{
		CHtmlgetIndex::Instance().get_js(IDR_VIEW_JS, L"html/_view.js", response_dto);
		return S_OK;
	}
	case IDR_ART_FILM_JS:		//_art_film.js
	{
		CHtmlgetIndex::Instance().get_js(IDR_ART_FILM_JS, L"html/_art_film.js", response_dto);
		return S_OK;
	}
	case IDR_ADMIN_ADD_JS:		//jquery.fileupload-ui.u3.min.js
	{
		CHtmlgetIndex::Instance().get_js(IDR_ADMIN_ADD_JS, L"html/jquery.fileupload-ui.u3.min.js", response_dto);
		return S_OK;
	}
	case IDR_WATERMARK_JS:		//jquery.watermark.js
	{
		CHtmlgetIndex::Instance().get_js(IDR_WATERMARK_JS, L"html/jquery.watermark.js", response_dto);
		return S_OK;
	}
	case IDR_WATERMARK_IMG_JS:		//jquery.watermark_img.js
	{
		CHtmlgetIndex::Instance().get_js(IDR_WATERMARK_IMG_JS, L"html/jquery.watermark_img.js", response_dto);
		return S_OK;
	}
	case IDR_ADMIN_SKIN_CSS:		//admin.skin.css
	{
		CHtmlgetIndex::Instance().get_css(IDR_ADMIN_SKIN_CSS, L"html/admin.skin.css", response_dto);
		return S_OK;
	}
	case IDR_PRV_REGISTER_HTML:		//register.html
	{
		CHtmlgetIndex::Instance().get_register_htm(request_dto, response_dto);
		return S_OK;
	}
	case IDR_DELBOX_HTML:		//delbox.html
	{
		string act = "";
		lookitem(request_dto.szArgs, "act=", act);
		if (act == "index.delimg")
		{
			string idx = "";
			lookitem(request_dto.szArgs, "idx=", idx);
			if (idx != "")
			{
				CHtmlgetViewer::Instance().del_viewer(request_dto, response_dto);
			}
			else
			{
				CHtmlgetIndex::Instance().proc_delbox(request_dto, response_dto);
			}
		}
		else
		{
			CHtmlgetViewer::Instance().get_delbox(request_dto, response_dto);
		}
		return S_OK;
	}
	case IDR_LOCAL_THUMB:		//local_thumb
	{
		CHtmlgetViewer::Instance().get_thumb_file(request_dto, response_dto);
		return S_OK;
	}
	case IDR_INDEX_HTML:		//index.html
	{
		string act = "";
		lookitem(request_dto.szArgs, "act=", act);
		if (act == "last.get_nb")
		{
			CHtmlgetIndex::Instance().get_index_htm(request_dto, response_dto);
			return S_OK;
		}
		break;
	}

	default:
		break;

	}

	if (hr == S_OK)
	{
		return S_OK;
	}

	if (CDataSrv::Instance()._acc != "2")
	{
		if (CDataSrv::Instance()._albumName == "private")
		{
			list<USER_DTO> lstUsers;
			Cchatdal::Instance().get_users(lstUsers);
			CDataSrv::Instance()._acc = "0";
			if (!lstUsers.empty())
			{
				CDataSrv::Instance()._acc = "1";
			}
		}
		else
		{
			CDataSrv::Instance().proc_public_acc();
		}
	}


	if (CDataSrv::Instance()._albumName == "private" && CDataSrv::Instance()._acc == "0")
	{
		_is_firstrun = true;
	}

	if (CDataSrv::Instance()._albumName == "public" && CDataSrv::Instance()._acc == "1")
	{
		_is_firstrun = true;
	}

	if (res == LGC_B_HTML)
	{
		_is_firstrun = false;
	}

	if (_is_firstrun)
	{
		if (CDataSrv::Instance()._albumName == "private" && CDataSrv::Instance()._acc == "0")
		{
			CHtmlgetIndex::Instance().get_register_htm(request_dto, response_dto);
		}
		else
		{
			CHtmlgetIndex::Instance().proc_first_index(request_dto, response_dto);
		}
		return S_OK;
	}

	if (CDataSrv::Instance()._albumName == "private" && CDataSrv::Instance()._acc == "1")
	{
		CHtmlgetIndex::Instance().get_login_htm(request_dto, response_dto);
		return S_OK;
	}

	switch (res)
	{
	case IDR_INDEX_HTML:		//index.html
	{
		CHtmlgetIndex::Instance().get_index_htm(request_dto, response_dto);
		hr = S_OK;
		break;
	}
	case IDR_ADMIN_HTML:		//admin.html
	{
		CHtmlgetAdmin::Instance().get_admin_htm(request_dto, response_dto);
		hr = S_OK;
		break;
	}
	case IDR_PICTURE_HTML:		//picture.html
	{
		CHtmlgetPic::Instance().get_picture_htm(request_dto, response_dto);
		hr = S_OK;
		break;
	}
	case LGC_PAGE_HTML:		//page.html
	{
		CHtmlgetIndex::Instance().get_page_htm(request_dto, response_dto);
		hr = S_OK;
		break;
	}
	case LGC_A_HTML:		//a.html
	{
		CHtmlgetIndex::Instance().get_index_htm(request_dto, response_dto);
		hr = S_OK;
		break;
	}
	case LGC_B_HTML:		//b.html
	{
		CHtmlgetIndex::Instance().get_b_htm(request_dto, response_dto);
		hr = S_OK;
		break;
	}
	case IDR_LAST_HTML:		//last.html
	{
		bool b1 = use_main_url();
		if (b1)
		{
			request_dto.szUrl = "/admin.html";
			request_dto.szArgs = "page=admin.firstrun";
			CHtmlgetIndex::Instance().get_last_htm(request_dto, response_dto);
		}
		else
		{
			string val = "";
			CDataSrv::Instance().ReadPrivate("last", val);

			string szUrl = "";
			string szArgs = "";
			bool b = isurlok(val, szUrl, szArgs);
			if (!b)
			{
				//string::size_type pos = val.find("/admin.html");
				//if (pos != string::npos)
				//{
				//	request_dto.szUrl = "/admin.html";
				//	request_dto.szArgs = "";
				//}
				//else
				{
					request_dto.szUrl = "/index.html";
					request_dto.szArgs = "";
				}
			}
			else
			{
				request_dto.szUrl = szUrl;
				request_dto.szArgs = szArgs;
			}

			time_t current_time;
			time(&current_time);

			HISTORY_DTO historyDto;
			historyDto.lastmodified = static_cast<uint64_t>(current_time);
			historyDto.user_id = CDataSrv::Instance()._login;
			long hr = writedb::Instance().add_history(historyDto);
			if (hr == 8)
			{
				CDataSrv::Instance()._readonly = hr;
			}

			CHtmlgetIndex::Instance().get_last_htm(request_dto, response_dto);
		}
		hr = S_OK;
		break;
	}
	case IDR_FILM_BOX_HTML:		//film_box.html
	{
		CHtmlgetIndexProc::Instance().proc_art_edit(request_dto, response_dto);
		hr = S_OK;
		break;
	}
	case LGC_PAINT:		//lgc_paint.html
	{
		CHtmlgetIndexProc::Instance().show_paint(request_dto, response_dto);
		hr = S_OK;
		break;
	}
	default:
		break;

	}
	if (hr == S_OK)
	{
		return S_OK;
	}

	return S_FALSE;
}

bool CHtmlget::isurlok(string val, string& szUrl, string& szArgs)
{
	string host = "";
	get_request_url(val.c_str(), host, szUrl, szArgs);

	bool b = false;
	const char* urls[] = { ".pc/index.html",NULL };
	const char** filename = urls;

	while (*filename)
	{
		string::size_type pos = val.find(*filename);
		if (pos != string::npos)
		{
			b = true;
			break;
		}
		filename++;
	}

	if (b)
	{
		QUERY_REQ req;
		lookitem(szArgs, "category=", req.cat_id);
		lookitem(szArgs, "page_nb=", req.page_nb);
		lookitem(szArgs, "favorites=", req.favorites);
		lookitem(szArgs, "caddie=", req.caddie);
		lookitem(szArgs, "most_visited=", req.most_visited);
		lookitem(szArgs, "recent_pics=", req.recent_pics);
		lookitem(szArgs, "home=", req.home);
		lookitem(szArgs, "no_tag=", req.no_tag);
		lookitem(szArgs, "art_list=", req.art_list);
		lookitem(szArgs, "longitude=", req.longitude);
		lookitem(szArgs, "latitude=", req.latitude);
		lookitem(szArgs, "list=", req.list0);
		lookitem(szArgs, "posted_list=", req.posted_list);
		lookitem(szArgs, "created_list=", req.created_list);
		lookitem(szArgs, "posted=", req.posted);
		lookitem(szArgs, "recent_cats=", req.recent_cats);
		lookitem(szArgs, "tags=", req.tags);
		lookitem(szArgs, "osm=", req.osm);
		lookitem(szArgs, "q=", req.q);
		lookitem(szArgs, "search=", req.search);
		lookitem(szArgs, "start=", req.start);
		lookitem(szArgs, "image_order=", req.image_order);
		lookitem(szArgs, "face=", req.face);

		if (req.favorites != "" || req.caddie != "" || req.most_visited != "" || req.recent_pics != "" || req.cat_id != "" ||
			req.list0 != "" || req.posted_list != "" || req.created_list != "" || req.recent_cats != "")
		{
			return true;
		}
	}
	return false;
}

bool CHtmlget::use_main_url()
{
	if (CDataSrv::Instance()._albumName == "private")
	{
		if (CDataSrv::Instance()._acc != "2")
		{
			list<USER_DTO> lstUsers;
			Cchatdal::Instance().get_users(lstUsers);
			CDataSrv::Instance()._acc = "0";
			if (!lstUsers.empty())
			{
				CDataSrv::Instance()._acc = "1";
			}
		}
	}

	bool b1 = false;
	list<ROOT_DTO> lstRoot;
	CDataSrv::Instance().get_roots(lstRoot);

	if (lstRoot.empty())
	{
		b1 = true;
		if (CDataSrv::Instance()._albumName == "public")
		{
			b1 = false;
		}
	}

	if (CDataSrv::Instance()._albumName == "private" && CDataSrv::Instance()._acc == "0")
	{
		b1 = true;
	}

	return b1;
}
