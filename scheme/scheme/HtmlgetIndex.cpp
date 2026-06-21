////////////////////////////////////////////////////////////////////
/// @file HtmlgetIndex.cpp
/// @brief 首页（索引页）请求处理实现文件
/// @details 此文件实现了 CHtmlgetIndex 类的所有方法，
///          包括首页 HTML 生成、图片列表查询、分页处理、
///          搜索请求处理、日历视图等功能。
///          是图库首页的核心处理逻辑。
/// @note 使用 RapidJSON 生成 JSON 响应
/// @author PhotoNest Team
/// @date 2024
////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "unitil.h"
#include <vector>
#include <string>
#include <process.h>
#include <thread>
#include <shlobj.h>


#include "../../templ/templ/resource.h"
#include "imlog.h"
#include "lang.h"
#include "imagedal.h"
#include "chatdal.h"
#include "DataSrv.h"

#include "../../photol/photol/photol.h"

#include "HtmlgetIndex.h"
#include "HtmlgetMenubar.h"
#include "HtmlgetIndexAction.h"
#include "HtmlgetIndexProc.h"
#include "HtmlgetCal.h"
#include "uploadParser.h"
#include "HtmlSearchProc.h"
#include "writedb.h"
#include "HtmlgetAdmin.h"
#include "HtmlgetBatch.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

CHtmlgetIndex::CHtmlgetIndex(void)
{
}

CHtmlgetIndex::~CHtmlgetIndex(void)
{
}

long CHtmlgetIndex::get_index_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	QUERY_REQ req;
	get_req(0, request_dto, req);

	string display = "";
	string act = "";
	lookitem(request_dto.szArgs, "display=", display);
	lookitem(request_dto.szArgs, "act=", act);

	string error_lic = "";
	HRESULT hr = S_OK;
	string tpl = "";
	string thumbnails = "";

	if (act == "index.logout")
	{
		CDataSrv::Instance()._acc = "1";

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.EndObject();
		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		CDataSrv::Instance().WritePrivate("last", "./index.html");
		return S_OK;
	}
	else if (act == "index.profile2")
	{
		CHtmlgetIndexProc::Instance().proc_profile_htm(request_dto, response_dto);
		return S_OK;
	}
	else if (act == "index.skins")
	{
		get_skins_htm(request_dto, response_dto);
		return S_OK;
	}
	else if (act == "index.box")
	{
		string id = "";
		lookitem(request_dto.szArgs, "id=", id);
		CDataSrv::Instance().get_play_htm(id, tpl);

		response_dto.szContentType = "text/html";
		response_dto.szResponse = tpl;
		return S_OK;
	}
	else if (act == "index.slider")
	{
		string szResponse = "";

		CHtmlSearchProc::Instance().proc_slider_req_htm(CDataSrv::Instance()._search_req, szResponse);
		response_dto.szResponse = szResponse;
		response_dto.szContentType = "text/plain; charset=utf-8";
		return S_OK;

	}
	else if (act == "index.addcaddie")
	{
		string ids = "";
		string op = "";
		lookitem(request_dto.szArgs, "ids=", ids);
		lookitem(request_dto.szArgs, "op=", op);

		if (ids == "")
		{
			list<string> plstImageIds;
			CDataSrv::Instance().get_lstImageIds(plstImageIds);

			list<string>::iterator it;
			for (it = plstImageIds.begin(); it != plstImageIds.end(); it++)
			{
				ids += *it + ",";
			}
			rtrim(ids, ",");
		}

		writedb::Instance().add_caddie(CDataSrv::Instance()._login, op, ids);

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.EndObject();
		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}
	else if (act == "remove_from_favorites")
	{
		list<string> plstImageIds;
		CDataSrv::Instance().get_lstImageIds(plstImageIds);

		string ids = "";
		list<string>::iterator it;
		for (it = plstImageIds.begin(); it != plstImageIds.end(); it++)
		{
			ids += *it + ",";
		}
		rtrim(ids, ",");

		writedb::Instance().add_favorites("del", CDataSrv::Instance()._login, ids);
		CDataSrv::Instance().refresh_favorites();

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}
	else if (act == "index.star")
	{
		string star = "";
		lookitem(request_dto.szArgs, "star=", star);
		CDataSrv::Instance()._star = star;

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		writer.StartObject();
		writer.String("stat");
		if (star == "1")
		{
			writer.String("ok");

			writer.String("imgs");
			list<TEMPL_DTO> lstImage;
			CHtmlgetIndexProc::Instance().get_cache(1, lstImage);
			list<TEMPL_DTO>::iterator iter;
			writer.StartArray();
			for (iter = lstImage.begin(); iter != lstImage.end(); iter++)
			{
				writer.StartObject();
				writer.String("id");
				writer.String(iter->id.c_str());
				writer.String("lv");
				writer.String(iter->val.c_str());
				writer.EndObject();
			}
			writer.EndArray();
		}
		else
		{
			writer.String("err");
		}
		writer.String("step");

		string step = "ing";
		if (CHtmlgetAdmin::Instance()._brisque_end == 1)
		{
			step = "end";
		}
		writer.String(step.c_str());

		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}
	else if (act == "index.title")
	{
		string id = "";
		lookitem(request_dto.szArgs, "id=", id);

		string stat = "";
		string imgvisits = "";

		list<IMAGE_DTO> lstImage;
		Cimagedal::Instance().get_images_by_ids(id, lstImage);

		list<IMAGE_DTO>::iterator it;
		for (it = lstImage.begin(); it != lstImage.end(); it++)
		{
			imgvisits = it->name;
			uint32_t q = (it->iscoi == 1) ? it->coi_quality : it->quality;
			if (q > 0)
			{
				imgvisits += "({'Quality'|@translate}: " + float2Str(static_cast<float>(q) / 100.0f, "%.1f") + ")";//+ " {'Visits'|@translate}";
				translate(imgvisits, "", "");
				stat = "ok";
			}
			break;
		}

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("q");
		writer.String(imgvisits.c_str());
		writer.String("stat");
		writer.String(stat.c_str());
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}
	else if (act == "index.searchbox")
	{
		string search_form = "";
		CHtmlSearchProc::Instance().get_search_form(_lstCategory, search_form, req);

		translate(search_form, "", "");
		response_dto.szContentType = "text/html";
		response_dto.szResponse = search_form;

		return S_OK;
	}
	else if (act == "index.monbar")
	{
		string calendarBar_html = "";
		uint32_t nb_images = 0;
		string calendarTitle = "";
		CHtmlgetCal::Instance().get_cal_thumbnails(false, req, CDataSrv::Instance()._search_req, calendarBar_html, nb_images, calendarTitle);

		CHtmlgetIndexProc::Instance().clear_data();
		CDataSrv::Instance()._q_req = req;
		CDataSrv::Instance().clear_cache_image_ids();

		if (nb_images > LIMIT_NB_PHOTOS)
		{
			if (req.q != "" || req.no_tag != "" || req.search == "q" || req.home != "" || req.recent_pics != "")
			{
				nb_images = LIMIT_NB_PHOTOS;
			}
		}

		translate(calendarTitle, "", "");

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String("ok");

		writer.String("nb_images");
		writer.String(uint2str(nb_images).c_str());
		writer.String("title");
		writer.String(calendarTitle.c_str());

		writer.EndObject();
		tpl = sb.GetString();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = tpl;

		return S_OK;
	}
	else if (act == "index.cal")
	{
		if (req.posted_list != "")
		{
			CDataSrv::Instance()._last_posted = req.posted;
		}
		else if (req.created_list != "")
		{
			CDataSrv::Instance()._last_posted2 = req.posted;
		}

		CLang::Instance().getOneData("cal_html", tpl);
		string cal_html = CHtmlgetCal::Instance().get_cal_string(req, CDataSrv::Instance()._search_req);

		ReplaceOne(tpl, "{$cal_html}", cal_html);
		translate(tpl, "", "");

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.String("htm");
		writer.String(tpl.c_str());
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}
	else if (act == "index.weekly")
	{
		string type = "";
		if (req.posted_list != "")
		{
			CDataSrv::Instance()._last_posted = req.posted;
			type = "posted";
		}
		else if (req.created_list != "")
		{
			CDataSrv::Instance()._last_posted2 = req.posted;
			type = "created";
		}

		string year = "";
		lookitem(request_dto.szArgs, "posted=", year);

		CLang::Instance().getOneData("cal_html", tpl);
		string cal_html = CHtmlgetCal::Instance().get_weekly_string(req, CDataSrv::Instance()._search_req, type, year);

		ReplaceOne(tpl, "{$cal_html}", cal_html);
		translate(tpl, "", "");

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.String("htm");
		writer.String(tpl.c_str());
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}
	else if (act == "index.details" || act == "index.ylist" || act == "index.mlist" || req.start != "")
	{
		string url0 = get_up_url(req);
		if (req.posted != "")
		{
			url0 += "&posted=" + req.posted;
			url0 += "&act=" + act + "&cal=" + act;
			ReplaceOne(url0, "start=", "unuse=");
		}

		if (req.posted_list != "")
		{
			CDataSrv::Instance()._last_posted = req.posted;
		}
		else if (req.created_list != "")
		{
			CDataSrv::Instance()._last_posted2 = req.posted;
		}

		CLang::Instance().getOneData("mon_html", tpl);

		uint32_t maxHeight = CHtmlgetIndexAction::Instance().get_maxHeight();
		Replace(tpl, "{$rowHeight}", uint2str(maxHeight));

		CHtmlgetIndexProc::Instance().clear_data();
		CDataSrv::Instance()._q_req = req;
		CDataSrv::Instance().clear_cache_image_ids();

		RESPONSE_DTO dto;
		get_page_htm(request_dto, dto);
		string cal_html = dto.szResponse;

		uint32_t maxPage0 = 0;
		CHtmlgetIndexProc::Instance().get_max_page(req, maxPage0);
		ReplaceOne(tpl, "{$maxPage}", uint2str(maxPage0));

		list<string> plstImageIds;
		CDataSrv::Instance().get_lstImageIds(plstImageIds);

		int curr = atoi(req.start.c_str());
		string navigationBar = get_navigationBar(url0, (int)plstImageIds.size(), curr, PERPAGE_TOTAL);

		string temp = navigationBar;
		Replace(temp, "{$id}", "1");
		ReplaceOne(tpl, "{$navigationBar}", temp);

		temp = navigationBar;
		Replace(temp, "{$id}", "2");
		ReplaceOne(tpl, "{$navigationBar}", temp);


		ReplaceOne(tpl, "{$cal_html}", cal_html);

		translate(tpl, "", "");

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.String("htm");
		writer.String(tpl.c_str());
		writer.String("title");
		writer.String("");
		writer.EndObject();
		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}
	else if (act == "index.face")
	{
		string url0 = get_up_url(req);

		CLang::Instance().getOneData("mon_html", tpl);

		uint32_t maxHeight = CHtmlgetIndexAction::Instance().get_maxHeight();
		Replace(tpl, "{$rowHeight}", uint2str(maxHeight));

		CHtmlgetIndexProc::Instance().clear_data();
		CDataSrv::Instance()._q_req = req;
		CDataSrv::Instance().clear_cache_image_ids();

		RESPONSE_DTO dto;
		get_page_htm(request_dto, dto);
		string cal_html = dto.szResponse;

		uint32_t maxPage0 = 0;
		CHtmlgetIndexProc::Instance().get_max_page(req, maxPage0);
		ReplaceOne(tpl, "{$maxPage}", uint2str(maxPage0));

		list<string> plstImageIds;
		CDataSrv::Instance().get_lstImageIds(plstImageIds);
		uint32_t nb_images = (uint32_t)plstImageIds.size();

		int curr = atoi(req.start.c_str());
		string navigationBar = get_navigationBar(url0, nb_images, curr, PERPAGE_TOTAL);

		string temp = navigationBar;
		Replace(temp, "{$id}", "1");
		ReplaceOne(tpl, "{$navigationBar}", temp);

		temp = navigationBar;
		Replace(temp, "{$id}", "2");
		ReplaceOne(tpl, "{$navigationBar}", temp);
		ReplaceOne(tpl, "{$cal_html}", cal_html);
		translate(tpl, "", "");

		string title = get_title(_lstCategory, "index.html", req, "", nb_images);
		translate(title, "", "");

		string switchBoxSizeUrl = "";
		string switchBoxSortUrl = "";
		string categoryActions = "";
		req.image_order = CDataSrv::Instance()._q_req.image_order;
		categoryActions = CHtmlgetIndexAction::Instance().get_categoryActions(0, _lstCategory, nb_images, req, switchBoxSizeUrl, switchBoxSortUrl);
		translate(categoryActions, "", "");

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.String("htm");
		writer.String(tpl.c_str());
		writer.String("actions");
		writer.String(categoryActions.c_str());
		writer.String("title");
		writer.String(title.c_str());
		writer.EndObject();
		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}
	else if (act == "index.del_art_areyou")
	{
		string areyousure = "";
		CLang::Instance().getOneData("areyousure", areyousure);
		translate(areyousure, "", "");

		response_dto.szContentType = "text/html";
		response_dto.szResponse = areyousure;

		return S_OK;
	}
	else if (act == "index.del_art")
	{
		string artids = "";
		lookitem(request_dto.szArgs, "artids=", artids);
		rtrim(artids, ",");

		writedb::Instance().del_films(artids);

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.EndObject();
		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}
	else if (act == "index.down_box")
	{
		string nb = "";
		string ids = "";
		string down = "";
		lookitem(request_dto.szArgs, "nb=", nb);
		lookitem(request_dto.szArgs, "ids=", ids);
		lookitem(request_dto.szArgs, "down=", down);

		string down_box = "";
		Htmlread(IDR_DOWN_BOX_HTML, L"html/_down_box.html", down_box);
		ReplaceOne(down_box, "strawberry_jam", CDataSrv::Instance()._skin);

		Replace(down_box, "{$total}", nb);
		ReplaceOne(down_box, "{$ids}", ids);
		ReplaceOne(down_box, "{$down}", down);

		wstring downdir = L"";
		string val = "";
		CDataSrv::Instance().ReadPrivate("down_dir", val);
		if (val == "")
		{
			wchar_t buffer[MAX_PATH] = { 0 };
			SHGetSpecialFolderPath(NULL, buffer, CSIDL_MYDOCUMENTS, FALSE);
			downdir = buffer;
			downdir.erase(downdir.find_last_of(L"\\"));
			downdir += L"\\Downloads";
		}
		else
		{
			downdir = _u2w(val);
		}
		Replace(downdir, L"\\", L"/");
		downdir.erase(downdir.find_last_not_of(L"/") + 1);
		//downdir += L"/Export/";

		ReplaceOne(down_box, "{$defdir}", _w2u(downdir));
		ReplaceOne(down_box, "{$defpath}", "Export");

		translate(down_box, "", "");
		response_dto.szContentType = "text/html";
		response_dto.szResponse = down_box;

		return S_OK;
	}
	else if (act == "index.tags_box")
	{
		string ids = "";
		lookitem(request_dto.szArgs, "ids=", ids);

		string tags_box = "";
		Htmlread(IDR_TAGS_BOX_HTML, L"html/_tags_box.html", tags_box);
		ReplaceOne(tags_box, "strawberry_jam", CDataSrv::Instance()._skin);
		if (CDataSrv::Instance()._skin != "dark" && CDataSrv::Instance()._skin != "quartz")
		{
			ReplaceOne(tags_box, "selectize.dark.css", "selectize.clear.css");
		}

		ReplaceOne(tags_box, "{$ids}", ids);

		uint32_t r = CDataSrv::Instance()._readonly;
		ReplaceOne(tags_box, "{$readonly}", uint2str(r));

		translate(tags_box, "", "");
		response_dto.szContentType = "text/html";
		response_dto.szResponse = tags_box;

		return S_OK;
	}
	else if (act == "index.face_box")
	{
		string face_box = "";
		Htmlread(IDR_FACE_BOX_HTML, L"html/_face_box.html", face_box);
		ReplaceOne(face_box, "strawberry_jam", CDataSrv::Instance()._skin);
		if (CDataSrv::Instance()._skin != "dark" && CDataSrv::Instance()._skin != "quartz")
		{
			ReplaceOne(face_box, "selectize.dark.css", "selectize.clear.css");
		}

		uint32_t r = CDataSrv::Instance()._readonly;
		ReplaceOne(face_box, "{$readonly}", uint2str(r));
		ReplaceOne(tpl, "{$error_readonly}", CDataSrv::Instance()._str_readonly);

		translate(face_box, "", "");
		response_dto.szContentType = "text/html";
		response_dto.szResponse = face_box;

		return S_OK;
	}
	else if (act == "index.get_tags")
	{
		string picid = "";
		lookitem(request_dto.szArgs, "picid=", picid);

		list<IMAGE_TAG_DTO> lstImageTag;
		Cimagedal::Instance().get_image_tags(picid, lstImageTag);

		string stat = "ok";
		if (lstImageTag.empty())
		{
			stat = "none";
		}

		string tags = "[";
		list<IMAGE_TAG_DTO>::iterator iter;
		for (iter = lstImageTag.begin(); iter != lstImageTag.end(); iter++)
		{
			tags += "'~~" + iter->tag_id + "~~',";
		}
		rtrim(tags, ",");
		tags += "]";

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String(stat.c_str());
		writer.String("tags");
		writer.String(tags.c_str());
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}
	else if (act == "index.zip")
	{
		string ids = "";
		string fpath = "";
		string order = "";
		lookitem(request_dto.szArgs, "ids=", ids);
		lookitem(request_dto.szArgs, "fpath=", fpath);
		lookitem(request_dto.szArgs, "order=", order);
		URLDecode(fpath, 1);

		string album = CDataSrv::Instance()._albumName;
		rtrim(ids, ",");
		rtrim(fpath, "/");

		wstring wpath = _u2w(fpath);
		CreateMyDirectory(wpath);

		list<IMAGE_DTO> lstImage;
		Cimagedal::Instance().get_images_by_ids(ids, lstImage);

		list<ROOT_DTO> lstRoot;
		CDataSrv::Instance().get_roots(lstRoot);

		int i = 0;
		list<IMAGE_DTO>::iterator iter;
		for (iter = lstImage.begin(); iter != lstImage.end(); iter++)
		{
			i++;
			wstring ori = _u2w(iter->ori);
			if (iter->coi != "")
			{
				ori = _u2w(iter->coi);
			}

			const wchar_t* savefilenameinzip = wcsrchr(ori.c_str(), L'/');
			if (savefilenameinzip != NULL)
			{
				savefilenameinzip = savefilenameinzip + 1;
			}

			wstring strname = savefilenameinzip;
			if (order == "true")
			{
				wchar_t buf[100] = { 0 };
				swprintf_s(buf, 100, L"%d", i);

				strname = wstring(buf) + L"-" + strname;
			}

			string temp0 = ori2path(lstRoot, 1, iter->coi != "" ? iter->coi : iter->ori);
			wstring dest = _u2w(temp0);

			if (album == "private")
			{
				wstring szPath = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);

				wstring wstrinpath = szPath + L"/data2";
				wstring temp = dest + L".krf";
				dest = wstrinpath + L"/_temp/0.tmp";
				CUploadParser::Instance().Decryptfile(temp, dest);
			}

			wstring dst2 = wpath + L"/" + strname;
			CopyFile(dest.c_str(), dst2.c_str(), TRUE);

			if (album == "private")
			{
				my_DeleteFile(dest);
			}
		}

		string stat = "ok";
		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		writer.StartObject();
		writer.String("stat");
		writer.String(stat.c_str());
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}
	else if (act == "index.menubar_categories")
	{
		string cat_id = req.cat_id;
		uint32_t nb_cat_images = 0;
		Cimagedal::Instance().get_nb_cat_images(nb_cat_images);
		string menubar_categories = CHtmlgetMenubar::Instance().get_menubar_categories(cat_id, nb_cat_images, &_lstCategory);

		string stat = "ok";
		if (menubar_categories == "")
		{
			stat = "err";
		}

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String(stat.c_str());

		writer.String("htm");
		writer.String(menubar_categories.c_str());
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}
	else if (act == "index.film")
	{
		CHtmlgetIndexProc::Instance().proc_art_film(request_dto, response_dto);
		return S_OK;
	}
	else if (act == "index.zoom")
	{
		CHtmlgetIndexProc::Instance().proc_art_zoom(request_dto, response_dto);
		return S_OK;
	}
	else if (act == "last.get_nb")
	{
		string ty = "";
		lookitem(request_dto.szArgs, "ty=", ty);

		string nb = "0";

		if (ty == "1")
		{
			uint32_t r = CDataSrv::Instance()._readonly;
			if (r == 0)
			{
				string changed = "";
				writedb::Instance().set_categories_visible(changed);
			}

			_beginthreadex(NULL, 0, sb_nb, this, NULL, NULL);

		}
		else
		{
			for (;;)
			{
				if (CHtmlgetAdmin::Instance()._lstCatNbOk == 1)
				{
					nb = Int2Str(CDataSrv::Instance()._ischanged);
					break;
				}

				this_thread::sleep_for(chrono::milliseconds(100));
			}
		}

		string stat = "ok";
		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String(stat.c_str());
		writer.String("nb");
		writer.String(nb.c_str());
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}
	else
	{
		_lstCategory.clear();
		CHtmlgetMenubar::Instance().clear();
		CHtmlgetMenubar::Instance().get_categories_order_by_rank(1, _lstCategory);
		CHtmlgetMenubar::Instance().proc_categories(&_lstCategory);

		list<CATEGORY_DTO>::iterator iter;
		if (req.cat_id != "")
		{
			bool b = false;
			for (iter = _lstCategory.begin(); iter != _lstCategory.end(); iter++)
			{
				if (iter->id == req.cat_id)
				{
					b = true;
					break;
				}
			}

			if (!b)
			{
				req.cat_id = "";
			}
		}

		if (display == "")
		{
			display = CDataSrv::Instance()._display_thumb;
		}
		else
		{
			CDataSrv::Instance()._display_thumb = display;
			CDataSrv::Instance().set_config_val("display", display);
		}

		CHtmlgetIndexAction::Instance().set_display(display);

		uint32_t nb_images = 0;

		if (req.search == "q")
		{
			if (!request_dto.szBody.empty())
			{
				CHtmlSearchProc::Instance().get_search_req(request_dto, CDataSrv::Instance()._search_req);

				string filter = "";
				lookitem(request_dto.szArgs, "filter=", filter);
				CDataSrv::Instance().set_config_val("filter", filter);

				if (CDataSrv::Instance()._search_req.subcats == "on")
				{
					string cats = "";
					list<CATEGORY_DTO>::iterator iter;

					for (iter = _lstCategory.begin(); iter != _lstCategory.end(); iter++)
					{
						string uppercats = "," + iter->full_id + ",";
						string::size_type pos = uppercats.find("," + CDataSrv::Instance()._search_req.cat + ",");
						if (pos != string::npos)
						{
							cats += iter->id + ",";
						}

					}
					rtrim(cats, ",");
					CDataSrv::Instance()._search_req.allcats = cats;
				}
				CDataSrv::Instance()._search_req.search = "1";

				StringBuffer sb;
				Writer<StringBuffer> writer(sb);

				writer.StartObject();
				writer.String("stat");
				writer.String("ok");
				writer.EndObject();

				response_dto.szContentType = "text/plain; charset=utf-8";
				response_dto.szResponse = sb.GetString();
				return S_OK;
			}
		}
		string menubar_tags = "";
		string menubar = "";

		if (req.search == "1")
		{
			hr = CHtmlSearchProc::Instance().proc_search_htm(_lstCategory, req, "", menubar, tpl);
			translate(tpl, "", "");
			response_dto.szContentType = "text/html";
			response_dto.szResponse = tpl;
			return S_OK;
		}
		else
		{
			hr = get_index_template(_lstCategory, req, "", menubar, tpl);
		}

		if (req.osm == "1")
		{
			string net = "";
			lookitem(request_dto.szArgs, "net=", net);
			if (net == "1")
			{
				tpl = "";
				Htmlread(IDR_NET_HTML, L"html/_net.html", tpl);

				response_dto.szResponse = tpl;
				response_dto.szContentType = "text/html";
				return S_OK;
			}

			string addressPoints = "[";

			list<string> plstImageIds;
			bool b = false;

			if (req.cat_id != "" || req.caddie != "" || req.favorites != "" || req.most_visited != ""
				|| req.list0 != "" || req.recent_pics != ""
				|| req.no_tag != "" || req.longitude != ""
				|| req.posted_list != "" || req.created_list != "" || (req.tags != "" && req.tags != "all")
				|| req.q != "" || req.search == "q")
			{
				CDataSrv::Instance().get_lstImageIds(plstImageIds);
			}
			else
			{
				b = true;
				CDataSrv::Instance().get_lstImageIds_map(plstImageIds);
			}

			string ids = "";

			list<string>::iterator it;
			for (it = plstImageIds.begin(); it != plstImageIds.end(); it++)
			{
				ids += *it + ",";
			}
			rtrim(ids, ",");

			list<IMAGE_DTO> lstImage;
			Cimagedal::Instance().get_images_by_ids(ids, lstImage);

			list<string> lstosm;
			uint32_t nb = 0;
			list<IMAGE_DTO>::iterator iter;
			for (iter = lstImage.begin(); iter != lstImage.end(); iter++)
			{
				if (iter->longitude != 0 || iter->latitude != 0)
				{
					nb++;
					addressPoints += "[";
					addressPoints += float2Str(iter->latitude, "%f") + ",";
					addressPoints += float2Str(iter->longitude, "%f") + ",";

					string temp = iter->name;
					Replace(temp, "\"", "");
					addressPoints += "\"" + temp + "\",";

					string ori = CDataSrv::Instance().get_image_url(0, *iter);
					addressPoints += "\"" + ori + "\",";

					string url = "./picture.html?" + req.args + "&id=" + iter->id;
					addressPoints += "\"" + url + "\",";

					string u2 = CDataSrv::Instance().get_image_url(100, *iter);
					addressPoints += "\"" + u2 + "\",";

					addressPoints += uint2str(iter->width);
					addressPoints += "],";

					lstosm.push_back(iter->id);
				}
			}
			rtrim(addressPoints, ",");
			addressPoints += "]";

			CDataSrv::Instance()._lstosm = lstosm;

			tpl = "";
			Htmlread(IDR_OSM_HTML, L"html/_osm.html", tpl);

			string flag = "";
			lookitem(request_dto.szArgs, "flag=", flag);
			ReplaceOne(tpl, "{$flag}", flag);

			string prev = "./index.html?" + req.args;
			ReplaceOne(prev, "&osm=1", "");
			ReplaceOne(prev, "?osm=1", "");
			Replace(tpl, "{$HOME_PREV}", prev);

			string total = "";
			if (b && plstImageIds.size() == 3000)
			{
				CLang::Instance().getOneData("map_items_top", total);
				total += " " + uint2str(nb);
			}
			else {
				CLang::Instance().getOneData("map_items", total);
				total = uint2str(nb) + " " + total;
			}

			ReplaceOne(tpl, "{$TOTAL}", total);

			translate(tpl, "", "");
			ReplaceOne(tpl, "$addressPoints", addressPoints);

			response_dto.szResponse = tpl;
			response_dto.szContentType = "text/html";
			return S_OK;
		}
		else if (req.art_list == "1")
		{
			ReplaceOne(tpl, "{$navigationBar}", "");
			ReplaceOne(tpl, "{$navigationBar}", "");
			ReplaceOne(tpl, "{$calendarBar_html}", "");

			string thumbnails = CHtmlgetIndexProc::Instance().proc_art_list(request_dto.szArgs);

			ReplaceOne(tpl, "{$THUMBNAILS}", "");
			ReplaceOne(tpl, "{$CATEGORIES}", thumbnails);
		}
		else if (req.tags == "all")
		{
			ReplaceOne(tpl, "{$navigationBar}", "");
			ReplaceOne(tpl, "{$navigationBar}", "");
			ReplaceOne(tpl, "{$calendarBar_html}", "");

			list<TAG_DTO>* ptrlstTag = CDataSrv::Instance().get_lstTag();
			string thumbnails = CHtmlgetIndexProc::Instance().proc_tags_all(request_dto.szArgs, ptrlstTag);

			list<TAG_DTO>::iterator it;
			for (it = ptrlstTag->begin(); it != ptrlstTag->end(); it++)
			{
				nb_images += it->nb_images;
			}

			ReplaceOne(tpl, "{$THUMBNAILS}", "");
			ReplaceOne(tpl, "{$CATEGORIES}", thumbnails);
		}
		else if (req.cat_id != "" || req.favorites != "" || req.caddie != "" || req.most_visited != ""
			|| req.list0 != "" || req.recent_pics != "" || req.face != ""
			|| req.no_tag != "" || req.longitude != ""
			|| req.posted_list != "" || req.created_list != "" || (req.tags != "" && req.tags != "all")
			|| req.q != "" || req.search == "q")
		{

			ORDER_DTO orderDto;
			get_dateType(req, orderDto);
			uint32_t nb = 0;
			CHtmlgetIndexProc::Instance().proc_tumb_htm(_lstCategory, req, orderDto, nb, tpl);
			nb_images = nb;

			if (nb_images == 0)
			{
				list<string> plstImageIds;
				CDataSrv::Instance().get_lstImageIds(plstImageIds);
				nb_images = (uint32_t)plstImageIds.size();
			}

			if (req.tags != "")
			{
				list<TAG_DTO>* ptrlstTag = CDataSrv::Instance().get_lstTag();
				list<TAG_DTO>::iterator it;
				for (it = ptrlstTag->begin(); it != ptrlstTag->end(); it++)
				{
					if (it->id == req.tags)
					{
						nb_images = it->nb_images;
						break;
					}
				}
			}

			if (req.no_tag == "")
			{
				menubar_tags = CDataSrv::Instance().get_menubar_tags(0, "", "");
			}
		}
		else
		{
			ReplaceOne(tpl, "{$navigationBar}", "");
			ReplaceOne(tpl, "{$navigationBar}", "");
			ReplaceOne(tpl, "{$calendarBar_html}", "");

			string cat_ids = "";
			if (req.recent_cats != "")
			{
				Cimagedal::Instance().get_recent_cats(cat_ids);
				thumbnails = CHtmlgetIndexAction::Instance().get_thumbnailCategories("recent_cats", cat_ids, _lstCategory);
			}
			else
			{
				thumbnails = CHtmlgetIndexAction::Instance().get_thumbnailCategories("", "", _lstCategory);
			}

			ReplaceOne(tpl, "{$THUMBNAILS}", "");
			ReplaceOne(tpl, "{$CATEGORIES}", thumbnails);

			if (cat_ids == "")
			{
				cat_ids = req.cat_id;
			}

			menubar_tags = CDataSrv::Instance().get_menubar_tags(1, cat_ids, "");

		}

		ReplaceOne(menubar, "{$menubar_tags}", menubar_tags);
		ReplaceOne(tpl, "{$calendarViews}", "");
		ReplaceOne(tpl, "{$calendarTitle}", "");

		ReplaceOne(tpl, "{$star}", CDataSrv::Instance()._star);
		ReplaceOne(tpl, "{$cat_id}", req.cat_id);

		uint32_t maxHeight = CHtmlgetIndexAction::Instance().get_maxHeight();
		uint32_t maxWidth = CHtmlgetIndexAction::Instance().get_maxWidth();

		ReplaceOne(tpl, "{$maxWidth}", uint2str(maxWidth));
		ReplaceOne(tpl, "{$maxHeight}", uint2str(maxHeight));
		ReplaceOne(tpl, "{$h0}", uint2str(maxHeight));

		string title = get_title(_lstCategory, "index.html", req, "", nb_images);
		ReplaceOne(tpl, "{$title}", title);

		string switchBoxSizeUrl = "";
		string switchBoxSortUrl = "";
		string categoryActions = "";
		if (req.search != "1" && req.osm != "1")
		{
			if (req.list0 == "1")
			{
				req.list0 = CDataSrv::Instance()._q_req.list0;
			}
			req.image_order = CDataSrv::Instance()._q_req.image_order;
			categoryActions = CHtmlgetIndexAction::Instance().get_categoryActions(1, _lstCategory, nb_images, req, switchBoxSizeUrl, switchBoxSortUrl);
		}

		ReplaceOne(tpl, "{$categoryActions}", categoryActions);
		ReplaceOne(tpl, "{$switchBoxSizeUrl}", switchBoxSizeUrl);
		ReplaceOne(tpl, "{$switchBoxSortUrl}", switchBoxSortUrl);

		ReplaceOne(tpl, "{$menubar}", menubar);

		ReplaceOne(tpl, "{$ismsnry}", "0");
		ReplaceOne(tpl, "{$maxPage}", "0");
		ReplaceOne(tpl, "{$art_list}", req.art_list);

		translate(tpl, "", "");
		response_dto.szContentType = "text/html";
		response_dto.szResponse = tpl;
	}
	return S_OK;
}

UINT __stdcall CHtmlgetIndex::sb_nb(LPVOID pParam)
{
	CHtmlgetIndex* pThis = (CHtmlgetIndex*)pParam;
	CHtmlgetAdmin::Instance().nb_proc();
	CHtmlgetAdmin::Instance().galleries_brisque(0);

	return 0;
}

long CHtmlgetIndex::get_login_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string tpl = "";
	Htmlread(IDR_PRV_LOGIN_HTML, L"html/_prv_login.html", tpl);
	ReplaceOne(tpl, "strawberry_jam", CDataSrv::Instance()._skin);

	string hint = "";
	list<USER_DTO> lstUsers;
	Cchatdal::Instance().get_users(lstUsers);
	list<USER_DTO>::iterator I;
	for (I = lstUsers.begin(); I != lstUsers.end(); I++)
	{
		hint = I->hint;
		break;
	}
	Replace(tpl, "{$hint}", hint);
	ReplaceOne(tpl, "{$gallery_title}", CDataSrv::Instance()._gallery_title);
	translate(tpl, "", "");

	response_dto.szResponse = tpl;
	response_dto.szContentType = "text/html";
	return S_OK;
}

long CHtmlgetIndex::get_register_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string tpl = "";
	Htmlread(IDR_PRV_REGISTER_HTML, L"html/_prv_registr.html", tpl);

	ReplaceOne(tpl, "{$ov}", CDataSrv::Instance()._ov);
	translate(tpl, "", "");

	map<string, string> lcid;
	map<string, string> lang;
	CDataSrv::Instance().get_language(lcid, lang);

	string all_option = "";
	string option = "<option value=\"{$id}\">{$name}</option>";
	string lang0 = CLang::Instance()._language;

	map<string, string>::iterator I2;
	for (I2 = lang.begin(); I2 != lang.end(); I2++)
	{
		string temp = option;
		if (I2->first == lang0)
		{
			temp = "<option value=\"{$id}\" selected=\"selected\">{$name}</option>";
		}
		ReplaceOne(temp, "{$id}", I2->first);
		ReplaceOne(temp, "{$name}", I2->second);

		all_option += temp;
	}
	translate(all_option, "", "");
	ReplaceOne(tpl, "{$all_option}", all_option);

	response_dto.szResponse = tpl;
	response_dto.szContentType = "text/html";
	return S_OK;
}

long CHtmlgetIndex::get_b_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string nb = "";
	string err30 = "";
	string err40 = "";
	lookitem(request_dto.szArgs, "nb=", nb);
	lookitem(request_dto.szArgs, "err30=", err30);
	lookitem(request_dto.szArgs, "err40=", err40);

	string tpl = "";
	Htmlread(IDR_ADMIN_FIRST_OK, L"html/_admin_first_ok.html", tpl);

	ReplaceOne(tpl, "{$nb}", nb);
	ReplaceOne(tpl, "{$err30}", err30);
	ReplaceOne(tpl, "{$err40}", err40);

	translate(tpl, "", nb);

	response_dto.szResponse = tpl;
	response_dto.szContentType = "text/html";
	return S_OK;
}

long CHtmlgetIndex::get_js(UINT id, wstring js, RESPONSE_DTO& response_dto)
{
	string tpl = "";
	Htmlread(id, js, tpl);
	response_dto.szResponse = tpl;
	response_dto.szContentType = "application/x-javascript";
	return S_OK;
}

long CHtmlgetIndex::get_css(UINT id, wstring css, RESPONSE_DTO& response_dto)
{
	string tpl = "";
	Htmlread(id, css, tpl);

	string skin = CDataSrv::Instance()._skin;

	string background = "#e7ffe7";
	string cboxC = "#bebebe";
	string cboxChover = "#fff";

	if (skin == "avocado")
	{
		background = "#e7ffe7";
		cboxC = "#bbb";
		cboxChover = "#74bf04";
	}
	else if (skin == "blueberry")
	{
		background = "#2e4756";
		cboxC = "#bbb";
		cboxChover = "#93c2d6";
	}
	else if (skin == "cafe_latte")
	{
		background = "#f4f4f4";
		cboxC = "#bbb";
		cboxChover = "#C0B283;";
	}
	else if (skin == "dark")
	{
		background = "#141414";
		cboxC = "#bbb";
		cboxChover = "#fff";
	}
	else if (skin == "glacier")
	{
		background = "#C1C7D9";
		cboxC = "#bbb";
		cboxChover = "#fff";
	}
	else if (skin == "neon_orange")
	{
		background = "#fff";
		cboxC = "#bbb";
		cboxChover = "#ff7700";
	}
	else if (skin == "neon_pink")
	{
		background = "#fff";
		cboxC = "#bbb";
		cboxChover = "#FF2F63";
	}
	else if (skin == "newspaper")
	{
		background = "#fff";
		cboxC = "#bbb";
		cboxChover = "#fff";
	}
	else if (skin == "quartz")
	{
		background = "#1E1F23";
		cboxC = "#bbb";
		cboxChover = "#fff";
	}
	else if (skin == "splash")
	{
		background = "#fff";
		cboxC = "#bbb";
		cboxChover = "#fff;";
	}
	else if (skin == "strawberry_jam")
	{
		background = "#fff";
		cboxC = "#bbb";
		cboxChover = "#BF0D08";
	}
	else if (skin == "swimming_pool")
	{
		background = "#f1f5f8";
		cboxC = "#bbb";
		cboxChover = "#7ca2bc";
	}

	ReplaceOne(tpl, "#fff", cboxChover);
	ReplaceOne(tpl, "#bebebe", cboxC);
	ReplaceOne(tpl, "#e7ffe7", background);

	response_dto.szResponse = tpl;
	response_dto.szContentType = "text/css";
	return S_OK;
}

long CHtmlgetIndex::get_page_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	QUERY_REQ req = CDataSrv::Instance()._q_req;

	string page_nb = "";
	string st = "";
	lookitem(request_dto.szArgs, "page_nb=", page_nb);
	lookitem(request_dto.szArgs, "start=", st);

	req.page_nb = page_nb;

	int start = atoi(st.c_str());
	if (st == "")
	{
		start = atoi(req.start.c_str());
	}
	int np = atoi(page_nb.c_str());

	start = (start - 1) * PERPAGE_TOTAL;
	if (start < 0)
	{
		start = 0;
	}
	//Cimlog::Instance().LogToFile("get_page_htm\t" + request_dto.szArgs);
	//Cimlog::Instance().LogToFile("get_page_htm\t" + Int2Str(start + np * PERPAGE_NUM) + " - " + Int2Str(start + np * PERPAGE_NUM + PERPAGE_NUM));

	string ids = "";
	int i = 0;
	list<string> plstImageIds;
	CDataSrv::Instance().get_lstImageIds(plstImageIds);

	list<string>::iterator it;
	for (it = plstImageIds.begin(); it != plstImageIds.end(); it++)
	{
		if (i > np * PERPAGE_NUM + PERPAGE_NUM + start)
		{
			break;
		}
		else if (i >= start + np * PERPAGE_NUM && i < start + np * PERPAGE_NUM + PERPAGE_NUM)
		{
			ids += *it + ",";
		}
		i++;
	}
	rtrim(ids, ",");

	list<IMAGE_DTO> lstImage;
	Cimagedal::Instance().get_images_by_ids(ids, lstImage);

	CHtmlgetIndexProc::Instance().push_cache_images(lstImage);

	ORDER_DTO orderDto;
	get_dateType(req, orderDto);

	string thumbnails = CHtmlgetIndexAction::Instance().get_thumbnails(req, lstImage, orderDto, _lstCategory);
	translate(thumbnails, "", "");

	if (thumbnails == "")
	{
		thumbnails = " ";
	}

	response_dto.szContentType = "text/html";
	response_dto.szResponse = thumbnails;

	return S_OK;
}

long CHtmlgetIndex::get_skins_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string tpl = "";
	Htmlread(IDR_SKIN_HTML, L"html/_skins.html", tpl);
	ReplaceOne(tpl, "strawberry_jam", CDataSrv::Instance()._skin);
	ReplaceOne(tpl, "{$skin}", CDataSrv::Instance()._skin);

	string sel_bg = "";
	string custom_bg = "";
	string custom_path = "";
	CDataSrv::Instance().get_config_val("sel_bg", sel_bg);
	CDataSrv::Instance().get_config_val("custom_bg", custom_bg);

	list<IMAGE_DTO> lstImage;
	Cimagedal::Instance().get_images_by_ids(custom_bg, lstImage);
	if (!lstImage.empty())
	{
		custom_bg = CDataSrv::Instance().get_image_url(2, lstImage.front());
		list<ROOT_DTO> lstRoot;
		CDataSrv::Instance().get_roots(lstRoot);

		custom_path = ori2path(lstRoot, 1, lstImage.front().ori);
		if (CDataSrv::Instance()._albumName == "private")
		{
			custom_path += ".krf";
		}
	}

	if (sel_bg != "0" && sel_bg != "-2")
	{
		sel_bg = "-1";
	}

	ReplaceOne(tpl, "{$sel_bg}", sel_bg);
	ReplaceOne(tpl, "{$custom_bg}", custom_bg);
	ReplaceOne(tpl, "{$custom_path}", custom_path);

	if (CDataSrv::Instance()._bgtype == "1")
	{
		ReplaceOne(tpl, "o='header'", "checked=\"checked\"");
	}
	else
	{
		ReplaceOne(tpl, "o='header'", "");
	}

	uint32_t r = CDataSrv::Instance()._readonly;
	ReplaceOne(tpl, "{$readonly}", uint2str(r));
	translate(tpl, "", "");

	response_dto.szContentType = "text/html";
	response_dto.szResponse = tpl;

	return S_OK;
}

long CHtmlgetIndex::get_index_template(list<CATEGORY_DTO> lst, const QUERY_REQ req, string num_tags, string& menubar, string& tpl)
{
	string cal = "";
	string last = "";
	lookitem(req.args, "cal=", cal);
	lookitem(req.args, "last=", last);

	Htmlread(IDR_INDEX_HTML, L"html/_index.html", tpl);
	ReplaceOne(tpl, "strawberry_jam", CDataSrv::Instance()._skin);
	ReplaceOne(tpl, "{$last}", last);

	if (CDataSrv::Instance()._skin != "dark" && CDataSrv::Instance()._skin != "quartz")
	{
		ReplaceOne(tpl, "selectize.dark.css", "selectize.clear.css");
	}

	string filter = "";
	CDataSrv::Instance().get_config_val("filter", filter);
	ReplaceOne(tpl, "{$filters}", filter);

	string op = ".75";

	ReplaceOne(tpl, "{$skinOpacity}", op);
	ReplaceOne(tpl, "{$bgtype}", CDataSrv::Instance()._bgtype);

	ReplaceOne(tpl, "{$bg}", CDataSrv::Instance()._bg);
	ReplaceOne(tpl, "{$skin}", CDataSrv::Instance()._skin);


	ReplaceOne(tpl, "{$ov}", CDataSrv::Instance()._ov);
	ReplaceOne(tpl, "{$album}", CDataSrv::Instance()._albumName);

	uint32_t r = CDataSrv::Instance()._readonly;
	ReplaceOne(tpl, "{$readonly}", uint2str(r));
	ReplaceOne(tpl, "{$error_readonly}", CDataSrv::Instance()._str_readonly);

	int day = 0;
	string error_lic = "";
	if (CDataSrv::Instance()._ov == "1")
	{
		CLang::Instance().getOneData("error_lic", error_lic);
		ReplaceOne(error_lic, "999", "");
	}
	else if (CDataSrv::Instance()._ov == "0")
	{
		if (CDataSrv::Instance()._first != 0)
		{
			time_t current_time;
			time(&current_time);
			day = static_cast<int>(current_time - CDataSrv::Instance()._first) / (24 * 60 * 60);

			if (day > 23 && CDataSrv::Instance()._day == 0)
			{
				time_t tfirst = CDataSrv::Instance()._first + 30 * EXP_SPAN;
				struct tm t;
				localtime_s(&t, &tfirst);

				char stamped[MAX_PATH] = { 0 };
				strftime(stamped, MAX_PATH, "%Y/%m/%d", &t);

				CLang::Instance().getOneData("day_lic", error_lic);
				ReplaceOne(error_lic, "999", stamped);
				CDataSrv::Instance()._day = 1;
			}
		}
	}

	if (req.search != "")
	{
		ReplaceOne(tpl, "{qval}", "{'Search'|@translate}...");
	}

	if (req.q != "")
	{
		ReplaceOne(tpl, "{qval}", req.q);
	}
	else
	{
		ReplaceOne(tpl, "{qval}", "{'Search'|@translate}...");
	}

	ReplaceOne(tpl, "{$error_lic}", error_lic);
	ReplaceOne(tpl, "{$day}", Int2Str(day));

	//2
	string pagebanner = "";
	CLang::Instance().getOneData("pagebanner", pagebanner);
	ReplaceOne(pagebanner, "{$gallery_title}", CDataSrv::Instance()._gallery_title);

	ReplaceOne(tpl, "{$page_banner}", pagebanner);

	string admin_url = "./admin.html";

	admin_url = "./admin.html?page=admin.galleries";
	ReplaceOne(tpl, "{$admin_url}", admin_url);

	//4
	menubar = "";
	{
		//1
		string menubar_categories = "";
		CLang::Instance().getOneData("menubar_categories", menubar_categories);
		//2
		string menubar_tags = "{$menubar_tags}";

		//3
		string menubar_specials = "";
		CLang::Instance().getOneData("menubar_specials", menubar_specials);

		int nb = 0;
		list<TAG_DTO>::iterator it;
		list<TAG_DTO>* ptrlstTag = CDataSrv::Instance().get_lstTag();
		for (it = ptrlstTag->begin(); it != ptrlstTag->end(); it++)
		{
			if (it->nb_images > 0)
			{
				nb++;
			}
		}
		string nbTags = "";
		if (nb > 0)
		{
			nbTags = " (" + Int2Str(nb) + ")";
		}
		ReplaceOne(menubar_specials, "nbTags", nbTags);


		//4
		string menubar_menu = "";
		CLang::Instance().getOneData("menubar_menu", menubar_menu);

		//5
		string menubar_identification = "";
		CLang::Instance().getOneData("menubar_identification", menubar_identification);

		if (CDataSrv::Instance()._albumName == "public")
		{
			ReplaceOne(menubar_identification, "{'Settings'|@translate}", "{'Language'|@translate}");
		}

		ReplaceOne(menubar_identification, "{$USERNAME}", CDataSrv::Instance().get_username());

		string pag = lst.empty() ? "page=admin.photos_add" : "page=1";
		ReplaceOne(menubar_identification, "{$page}", pag);


		string style = "style=\"display:none;\"";
		if (CDataSrv::Instance()._ischanged != 0)
		{
			style = "";
		}
		ReplaceOne(menubar_identification, "o='style'", style);

		menubar += menubar_categories;
		menubar += menubar_tags;
		menubar += menubar_specials;
		menubar += menubar_menu;
		menubar += menubar_identification;
	}
	return S_OK;
}

void CHtmlgetIndex::proc_first_index(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	REQUEST_DTO request_dto2 = request_dto;
	request_dto2.szUrl = "admin.html";
	request_dto2.szArgs = request_dto.szArgs;

	string::size_type pos = request_dto.szArgs.find("admin.firstrun");
	if (pos == string::npos)
	{
		request_dto2.szArgs = request_dto.szArgs + "&page=admin.firstrun";
	}
	CHtmlgetAdmin::Instance().get_admin_htm(request_dto2, response_dto);
}

BOOL CHtmlgetIndex::CreateMyDirectory(wstring path)
{
	wchar_t szbuf[MAX_PATH] = { 0 };
	DWORD dwSize = GetFullPathName(path.c_str(), MAX_PATH, szbuf, NULL);

	BOOL b = FALSE;
	DWORD dwAttr = GetFileAttributes(szbuf);
	if (dwAttr != -1)
	{
		// found something; better not be a folder....
		if (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY))
		{
			b = CreateDirectory(szbuf, NULL);
		}
	}
	else
	{
		b = CreateDirectory(szbuf, NULL);
	}

	return b;
}

long CHtmlgetIndex::get_last_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string tpl = "";
	Htmlread(IDR_LAST_HTML, L"html/_last.html", tpl);
	ReplaceOne(tpl, "strawberry_jam", CDataSrv::Instance()._skin);

	string newurl = request_dto.szUrl;
	if (request_dto.szArgs != "")
	{
		newurl = newurl + "?" + request_dto.szArgs;

		string::size_type pos = request_dto.szArgs.find("last=");
		if (pos == string::npos)
		{
			newurl += "&last=1";
		}
	}
	else
	{
		newurl = newurl + "?last=1";
	}
	ReplaceOne(tpl, "newurl", newurl);

	uint32_t r = CDataSrv::Instance()._readonly;
	ReplaceOne(tpl, "{$readonly}", uint2str(r));
	ReplaceOne(tpl, "{$error_readonly}", CDataSrv::Instance()._str_readonly);

	translate(tpl, "", "");

	response_dto.szContentType = "text/html";
	response_dto.szResponse = tpl;

	return 0;
}

long CHtmlgetIndex::get_dateType(QUERY_REQ req, ORDER_DTO& orderDto)
{
	list<ORDER_DTO> order = CDataSrv::Instance()._lstOrder;
	list<ORDER_DTO>::iterator it;

	string reqOrder = req.image_order;

	if (req.posted_list != "" || req.created_list != "")
	{
		reqOrder = "998";
	}

	if (reqOrder == "")
	{
		bool b = false;
		string image_order0 = CDataSrv::Instance()._orderby;
		if (req.cat_id != "")
		{
			list<CATEGORY_DTO>::iterator iter;
			for (iter = _lstCategory.begin(); iter != _lstCategory.end(); iter++)
			{
				if (iter->id == req.cat_id)
				{
					if (iter->imageorder != "")
					{
						image_order0 = iter->imageorder;
						b = true;
					}
					break;
				}
			}
		}
		else if (req.search == "q")
		{
			if (CDataSrv::Instance()._search_req.date_type == "date_available")
			{
				image_order0 = "date_available DESC";
			}
			else if (CDataSrv::Instance()._search_req.date_type == "date_creation")
			{
				image_order0 = "date_creation DESC";
			}
		}
		else if (req.most_visited != "")
		{
			image_order0 = "";
		}
		else if (req.recent_pics != "")
		{
			image_order0 = "date_available DESC";
		}

		for (it = order.begin(); it != order.end(); it++)
		{
			if (it->order == image_order0)
			{
				orderDto = *it;
				break;
			}
		}

		if (b)
		{
			if (image_order0 != "b.rank asc")
			{
				string temp = "";
				CLang::Instance().getOneData("title_automatic", temp);
				Replace(orderDto.title, "h2", "span");
				Replace(orderDto.title, "calendarTitle", "");
				ReplaceOne(temp, "sortOrder", orderDto.title);
				orderDto.title = temp;
			}
		}
	}
	else
	{
		for (it = order.begin(); it != order.end(); it++)
		{
			if (it->id == reqOrder)
			{
				orderDto = *it;
				break;
			}
		}
	}
	return 0;
}

long CHtmlgetIndex::proc_delbox(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string ids = "";
	string idx = "";
	string type = "";
	string artid = "";
	lookitem(request_dto.szArgs, "ids=", ids);
	lookitem(request_dto.szArgs, "idx=", idx);
	lookitem(request_dto.szArgs, "type=", type);
	lookitem(request_dto.szArgs, "artid=", artid);

	string url = "";
	if (ids != "")
	{
		if (type == "7")
		{
			if (artid != "")
			{
				list<IMAGE_DTO> lstOrder;
				uint64_t md5sum = CHtmlgetIndexProc::Instance().get_md5sum(ids, lstOrder);

				FILM_DTO film_dto;
				film_dto.id = atoi(artid.c_str());
				film_dto.md5sum = md5sum;
				film_dto.photos = ids;
				writedb::Instance().update_film(film_dto);
			}
		}
		else
		{
			if (ids == "all")
			{
				ids = CHtmlgetBatch::Instance()._element_ids;
			}

			list<string> _lstFiles;
			list<string> _lstCoi;
			Cimagedal::Instance().get_files(ids, "", _lstFiles, _lstCoi);

			map<string, uint32_t> mapCat;
			Cimagedal::Instance().get_images_group(ids, mapCat);

			HRESULT hr = writedb::Instance().update_images("delete", ids, "");
			if (hr == 0)
			{
				writedb::Instance().update_last_import();
				CHtmlgetAdmin::Instance().delete_image_files("all", _lstFiles, _lstCoi);

				CDataSrv::Instance().clear_all();
				CHtmlgetIndexProc::Instance().clear_data();
				CHtmlgetMenubar::Instance().clear();
				CHtmlgetCal::Instance()._outsql = "";

				CHtmlgetAdmin::Instance().update_catnb(mapCat);
			}
		}
	}

	StringBuffer sb;
	Writer<StringBuffer> writer(sb);
	writer.StartObject();
	writer.String("stat");
	writer.String("ok");
	writer.String("path");
	writer.String(url.c_str());
	writer.String("ids");
	writer.String(ids.c_str());
	writer.String("artid");
	writer.String(artid.c_str());
	writer.EndObject();

	response_dto.szContentType = "text/plain; charset=utf-8";
	response_dto.szResponse = sb.GetString();

	return S_OK;
}
