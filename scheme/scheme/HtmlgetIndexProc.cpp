////////////////////////////////////////////////////////////////////
/// @file HtmlgetIndexProc.cpp
/// @brief 首页处理逻辑实现文件
/// @details 此文件实现了 CHtmlgetIndexProc 类的所有方法，
///          包括首页数据处理、图片列表查询、分页逻辑、
///          搜索处理、日历数据生成等功能。
///          是首页核心业务逻辑的实现文件。
/// @note 使用临界区（CriticalSection）保证线程安全
/// @author PhotoNest Team
/// @date 2024
////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "unitil.h"
#include <vector>
#include <string>
#include "../../templ/templ/resource.h"
#include "lang.h"
#include "imagedal.h"
#include "chatdal.h"
#include "DataSrv.h"
#include "letter.h"
#include <shellapi.h>
#include "imlog.h"

#include "../../photol/photol/photol.h"

#include "HtmlgetIndexProc.h"
#include "HtmlgetCal.h"
#include "uploadParser.h"
#include "HtmlgetIndexAction.h"
#include "writedb.h"
#include "HtmlgetMenubar.h"

CHtmlgetIndexProc::CHtmlgetIndexProc(void)
{
	InitializeCriticalSection(&_cs);

}

CHtmlgetIndexProc::~CHtmlgetIndexProc(void)
{
	DeleteCriticalSection(&_cs);

}

long CHtmlgetIndexProc::clear_data()
{
	EnterCriticalSection(&_cs);
	_lstImageDto.clear();
	LeaveCriticalSection(&_cs);
	return 0;
}

long CHtmlgetIndexProc::proc_art_edit(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string ids = "";
	string artid = "";
	lookitem(request_dto.szArgs, "ids=", ids);
	lookitem(request_dto.szArgs, "artid=", artid);

	list<IMAGE_DTO> lstOrder;
	uint64_t md5sum = get_md5sum(ids, lstOrder);

	string tpl = "";
	Htmlread(IDR_FILM_BOX_HTML, L"html/_film_box.html", tpl);
	ReplaceOne(tpl, "strawberry_jam", CDataSrv::Instance()._skin);

	string title1 = "";
	string title2 = "";

	list<FILM_DTO> lstfilm;
	Cchatdal::Instance().get_films(artid, md5sum, lstfilm);
	if (!lstfilm.empty())
	{
		FILM_DTO dto = lstfilm.front();
		title1 = dto.title1;
		title2 = dto.title2;
		artid = uint2str(dto.id);
		ids = dto.photos;
	}
	else
	{
		title1 = get_title(CDataSrv::Instance()._q_req);
		list<IMAGE_DTO>::iterator iter;
		for (iter = lstOrder.begin(); iter != lstOrder.end(); iter++)
		{
			title2 = get_time_string(iter->date_creation, "1");
		}
	}
	translate(title1, "", "");
	mk_path(title1);

	translate(title2, "", "");
	mk_path(title2);

	ReplaceOne(tpl, "{$title1}", title1);
	ReplaceOne(tpl, "{$title2}", title2);
	ReplaceOne(tpl, "{$artid}", artid);

	translate(tpl, "", "");

	response_dto.szResponse = tpl;
	response_dto.szContentType = "text/html";

	return S_OK;
}

long CHtmlgetIndexProc::proc_art_film(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string ids = "";
	string reload = "";
	string artid = "";
	lookitem(request_dto.szArgs, "ids=", ids);
	lookitem(request_dto.szArgs, "reload=", reload);
	lookitem(request_dto.szArgs, "artid=", artid);
	rtrim(ids, ",");

	list<IMAGE_DTO> lstOrder;
	uint64_t md5sum = get_md5sum(ids, lstOrder);

	string title1 = "";
	string title2 = "";
	string musci_src = "";

	list<FILM_DTO> lstfilm;
	Cchatdal::Instance().get_films(artid, md5sum, lstfilm);
	if (!lstfilm.empty())
	{
		FILM_DTO dto = lstfilm.front();
		title1 = dto.title1;
		title2 = dto.title2;
		musci_src = dto.music;
		ids = dto.photos;
		artid = uint2str(dto.id);
	}

	string ids0 = "";
	string pics = "";

	uint32_t nb = (uint32_t)lstOrder.size();

	uint32_t total = 0;
	string widths = "";
	string heights = "";

	uint32_t width = 1024;
	uint32_t height = 512;

	list<IMAGE_DTO>::iterator iter;
	for (iter = lstOrder.begin(); iter != lstOrder.end(); iter++)
	{
		get_size(*iter, width, height);

		string url = CDataSrv::Instance().get_image_url(0, *iter);

		pics += "\"" + url + "\",";
		ids0 += iter->id + ",";

		widths += uint2str(width) + ",";
		heights += uint2str(height) + ",";
		total++;

		if (title1.empty())
		{
			title1 = get_title(CDataSrv::Instance()._q_req);
			title2 = get_time_string(iter->date_creation, "1");
		}
	}
	translate(title1, "", "");
	mk_path(title1);

	translate(title2, "", "");
	mk_path(title2);

	rtrim(ids0, ",");
	rtrim(pics, ",");
	rtrim(widths, ",");
	rtrim(heights, ",");

	string tpl = "";
	Htmlread(IDR_ART_FILM_HTML, L"html/_art_film.html", tpl);
	ReplaceOne(tpl, "strawberry_jam", CDataSrv::Instance()._skin);
	ReplaceOne(tpl, "{$skin}", CDataSrv::Instance()._skin);

	ReplaceOne(tpl, "$reload", reload);
	ReplaceOne(tpl, "{$musci_src}", musci_src);
	ReplaceOne(tpl, "$total", uint2str(total));
	ReplaceOne(tpl, "$nb", uint2str(nb));

	ReplaceOne(tpl, "$title1", title1);
	ReplaceOne(tpl, "$title2", title2);
	ReplaceOne(tpl, "$artid", artid);

	ReplaceOne(tpl, "$widths", widths);
	ReplaceOne(tpl, "$heights", heights);

	uint32_t r = CDataSrv::Instance()._readonly;
	ReplaceOne(tpl, "{$readonly}", uint2str(r));
	ReplaceOne(tpl, "{$error_readonly}", CDataSrv::Instance()._str_readonly);

	//2
	string picture_next = "";
	string picture_pre = "";

	CLang::Instance().getOneData("picture_pre3", picture_pre);
	CLang::Instance().getOneData("picture_next3", picture_next);

	ReplaceOne(picture_next, "g='next'", "style=\"display:none;\"");
	ReplaceOne(picture_pre, "g='pre'", "style=\"display:none;\"");

	string calItem = "";
	calItem += picture_pre;
	calItem += picture_next;

	string calendar_nav = "";
	CLang::Instance().getOneData("calendar_nav", calendar_nav);
	ReplaceOne(calendar_nav, "{$calItem}", calItem);

	ReplaceOne(tpl, "{$calendar_nav}", calendar_nav);

	translate(tpl, "", "");

	ReplaceOne(tpl, "$ids", ids0);
	ReplaceOne(tpl, "$pics", pics);

	response_dto.szResponse = tpl;
	response_dto.szContentType = "text/html";

	return S_OK;
}

long CHtmlgetIndexProc::proc_art_zoom(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string ids = "";
	string artid = "";
	lookitem(request_dto.szArgs, "ids=", ids);
	lookitem(request_dto.szArgs, "artid=", artid);
	rtrim(ids, ",");

	list<IMAGE_DTO> lstOrder;
	uint64_t md5sum = get_md5sum(ids, lstOrder);

	list<FILM_DTO> lstfilm;
	Cchatdal::Instance().get_films(artid, md5sum, lstfilm);
	if (!lstfilm.empty())
	{
		FILM_DTO dto = lstfilm.front();
		ids = dto.photos;
		artid = uint2str(dto.id);
	}

	list<string> lstOrderIds;
	vector<string> v1 = tokenize(ids, ",", true, "");
	int len = (int)v1.size();
	for (int i = 0; i < len; i++)
	{
		lstOrderIds.push_back(v1[i]);
	}

	lstOrder.clear();
	Cimagedal::Instance().get_images_all_ids(ids, &lstOrderIds, lstOrder);

	string picid = v1[0];

	string tpl = "";

	Htmlread(IDR_ART_ZOOM_HTML, L"html/_art_zoom.html", tpl);
	ReplaceOne(tpl, "strawberry_jam", CDataSrv::Instance()._skin);

	ReplaceOne(tpl, "$ids", ids);
	ReplaceOne(tpl, "$picid", picid);
	ReplaceOne(tpl, "$artid", artid);

	uint32_t r = CDataSrv::Instance()._readonly;
	ReplaceOne(tpl, "{$readonly}", uint2str(r));
	ReplaceOne(tpl, "{$error_readonly}", CDataSrv::Instance()._str_readonly);

	string picture_next = "";
	string picture_pre = "";

	CLang::Instance().getOneData("picture_pre", picture_pre);
	CLang::Instance().getOneData("picture_next", picture_next);

	ReplaceOne(picture_next, "g='next'", "style=\"display:none;\"");
	ReplaceOne(picture_pre, "g='pre'", "style=\"display:none;\"");

	string calItem = "";
	calItem += picture_pre;
	calItem += picture_next;

	string calendar_nav = "";
	CLang::Instance().getOneData("calendar_nav", calendar_nav);
	ReplaceOne(calendar_nav, "{$calItem}", calItem);

	ReplaceOne(tpl, "{$calendar_nav}", calendar_nav);

	//3
	string actionButtons = "";
	ReplaceOne(tpl, "{$actionButtons}", actionButtons);
	translate(tpl, "", "");

	string picture_content = "<ul>";

	string url0 = "";
	string templ = "";
	CLang::Instance().getOneData("viewerjs_image_li_img", templ);

	list<IMAGE_DTO>::iterator iter;
	for (iter = lstOrder.begin(); iter != lstOrder.end(); iter++)
	{
		string url = CDataSrv::Instance().get_image_url(0, *iter);
		url0 = CDataSrv::Instance().get_image_url(100, *iter);

		string tmp = templ;

		string duration = "";
		ReplaceOne(tmp, "{$duration}", duration);

		string angle = get_angle(iter->rotation);

		ReplaceOne(tmp, "{$rotate}", angle);
		ReplaceOne(tmp, "{$id}", iter->id);
		ReplaceOne(tmp, "{$id}", iter->id);
		ReplaceOne(tmp, "{$title}", iter->file);

		ReplaceOne(tmp, "{$url}", url);
		ReplaceOne(tmp, "{$url0}", url0);
		picture_content += tmp;
	}

	picture_content += " </ul>";

	ReplaceOne(tpl, "{$picture_content}", picture_content);
	response_dto.szResponse = tpl;
	response_dto.szContentType = "text/html";

	return S_OK;
}

long CHtmlgetIndexProc::update_art_title(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string photos = "";
	string title1 = "";
	string title2 = "";
	string artid = "";
	lookitem(request_dto.szArgs, "photos=", photos);
	lookitem(request_dto.szArgs, "title1=", title1);
	lookitem(request_dto.szArgs, "title2=", title2);
	lookitem(request_dto.szArgs, "artid=", artid);
	URLDecode(title1, 1);
	URLDecode(title2, 1);

	esc_string(title1);
	esc_string(title2);
	trim(title1, " ");

	string music = _w2u(request_dto.szFile);
	Replace(music, "\\", "/");

	list<IMAGE_DTO> lstOrder;
	uint64_t md5sum = get_md5sum(photos, lstOrder);

	uint32_t id = 0;

	time_t current_time;
	time(&current_time);

	response_dto.szResponse = "ok";

	list<FILM_DTO> lstfilm;
	Cchatdal::Instance().get_films(artid, md5sum, lstfilm);
	if (!lstfilm.empty())
	{
		FILM_DTO dto = lstfilm.front();
		if (title1 == dto.title1 && title2 == dto.title2)
		{
			response_dto.szResponse = "not change";
		}
		else
		{
			FILM_DTO film_dto;
			film_dto.id = atoi(artid.c_str());
			film_dto.title1 = title1;
			film_dto.title2 = title2;
			film_dto.md5sum = 0;

			writedb::Instance().update_film(film_dto);
		}
	}
	else
	{
		FILM_DTO film_dto;
		film_dto.id = 0;
		film_dto.md5sum = md5sum;
		film_dto.title1 = title1;
		film_dto.title2 = title2;
		film_dto.photos = photos;
		film_dto.lastmodified = static_cast<uint64_t>(current_time);
		film_dto.type = "film";
		film_dto.music = music;

		writedb::Instance().add_film(film_dto, id);
	}

	response_dto.szContentType = "text/plain; charset=utf-8";

	return S_OK;
}

string CHtmlgetIndexProc::proc_art_list(string szArgs)
{
	string art_li = "";
	CLang::Instance().getOneData("art_li", art_li);
	string style1 = "style=\"width:{$width}px; height:{$height}px;\"";

	uint32_t maxWidth = 240;
	uint32_t maxHeight = 180;

	string art_list = "";
	list<IMAGE_DTO>::iterator iter;

	list<FILM_DTO> lstfilm;
	Cchatdal::Instance().get_films("", 0, lstfilm);
	list<FILM_DTO>::iterator it;
	for (it = lstfilm.begin(); it != lstfilm.end(); it++)
	{
		string artids = it->photos;

		list<string> lstOrderIds;
		vector<string> v1 = tokenize(artids, ",", true, "");
		int len = (int)v1.size();
		for (int i = 0; i < len; i++)
		{
			lstOrderIds.push_back(v1[i]);
		}

		list<IMAGE_DTO> lstOrder;
		Cimagedal::Instance().get_images_all_ids(artids, &lstOrderIds, lstOrder);
		if (!lstOrder.empty())
		{
			string style = style1;

			string temp = art_li;

			Replace(temp, "{$artid}", uint2str(it->id));
			Replace(temp, "{$ids}", it->photos);
			Replace(temp, "{$title1}", (it->title1 != "") ? it->title1 : it->title2);
			ReplaceOne(temp, "{$title2}", it->title2);
			ReplaceOne(temp, "{$duration}", get_dur_string((uint32_t)(lstOrder.size() * 4)));

			for (iter = lstOrder.begin(); iter != lstOrder.end(); iter++)
			{
				string url0 = CDataSrv::Instance().get_image_url(0, *iter);

				uint32_t outW = 0;
				uint32_t outH = 0;
				get_imgsize(*iter, maxWidth, maxHeight, outW, outH);

				ReplaceOne(temp, "{$path}", url0);
				ReplaceOne(temp, "{$image_id}", iter->id);
				ReplaceOne(temp, "{$name}", iter->name);

				ReplaceOne(style, "{$width}", uint2str(outW));
				ReplaceOne(style, "{$height}", uint2str(outH));

				Replace(temp, "g=\"style\"", style);
				break;
			}

			art_list += temp;
		}
	}

	string thumbnails = "";
	Htmlread(IDR_ART_LIST_HTML, L"html/_art_list.html", thumbnails);

	ReplaceOne(thumbnails, "{$art_list}", art_list);

	return thumbnails;
}

long CHtmlgetIndexProc::proc_profile_htm(REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string tpl = "";
	if (CDataSrv::Instance()._albumName == "private")
	{
		Htmlread(IDR_PRV_PROFILE2_HTML, L"html/_prv_profile2.html", tpl);
	}
	else {
		Htmlread(IDR_PRV_PROFILE_HTML, L"html/_prv_profile.html", tpl);
	}

	if (CDataSrv::Instance()._skin != "dark" && CDataSrv::Instance()._skin != "quartz")
	{
		ReplaceOne(tpl, "selectize.dark.css", "selectize.clear.css");
	}

	ReplaceOne(tpl, "strawberry_jam", CDataSrv::Instance()._skin);

	ReplaceOne(tpl, "{$ov}", CDataSrv::Instance()._ov);

	uint32_t r = CDataSrv::Instance()._readonly;
	ReplaceOne(tpl, "{$readonly}", uint2str(r));
	ReplaceOne(tpl, "{$error_readonly}", CDataSrv::Instance()._str_readonly);

	string uername = "";
	string hint = "";
	string enckey = "";

	if (CDataSrv::Instance()._albumName == "private")
	{
		list<USER_DTO> lstUsers;
		Cchatdal::Instance().get_users(lstUsers);

		list<USER_DTO>::iterator I;
		for (I = lstUsers.begin(); I != lstUsers.end(); I++)
		{
			uername = I->username;
			hint = I->hint;
			CDataSrv::Instance().get_config_val("enckey", enckey);
		}
	}
	ReplaceOne(tpl, "{$uername}", uername);
	ReplaceOne(tpl, "{$hint}", hint);
	ReplaceOne(tpl, "{$enckey}", enckey);

	string all_option = "";
	string option = "<option value=\"{$id}\">{$name}</option>";
	string lang0 = CLang::Instance()._language;

	map<string, string> lcid;
	map<string, string> lang;
	CDataSrv::Instance().get_language(lcid, lang);

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
	ReplaceOne(tpl, "{$all_option}", all_option);
	translate(tpl, "", "");

	response_dto.szResponse = tpl;
	response_dto.szContentType = "text/html";

	return S_OK;
}

string CHtmlgetIndexProc::proc_tags_all(string szArgs, list<TAG_DTO>* lstTag)
{
	string thumbnails = "";

	list<TAG_DTO>::iterator it;

	string display_mode = "";
	lookitem(szArgs, "display_mode=", display_mode);
	if (display_mode == "letters")
	{
		map<string, list<TAG_DTO>> mapTag;
		map<string, list<TAG_DTO>>::iterator iter;

		for (it = lstTag->begin(); it != lstTag->end(); it++)
		{
			wstring temp = _u2w(it->name).substr(0, 1);
			string letter = GetFirstLetter(temp);

			transform(letter.begin(), letter.end(), letter.begin(), ::toupper);

			iter = mapTag.find(letter);
			if (iter != mapTag.end())
			{
				iter->second.push_back(*it);
			}
			else
			{
				list<TAG_DTO> lst;
				lst.push_back(*it);
				mapTag.insert(map<string, list<TAG_DTO>>::value_type(letter, lst));
			}
		}

		string tags_letters = "";
		CLang::Instance().getOneData("tags_letters", tags_letters);
		string tags_letters_tr_span = "";
		string tags_letters_tr = "";
		CLang::Instance().getOneData("tags_letters_tr_span", tags_letters_tr_span);
		CLang::Instance().getOneData("tags_letters_tr", tags_letters_tr);
		for (iter = mapTag.begin(); iter != mapTag.end(); iter++)
		{
			string tr_all = "";
			string temp0 = tags_letters;
			ReplaceOne(temp0, "{$letter}", iter->first);

			for (it = iter->second.begin(); it != iter->second.end(); it++)
			{
				string temp = tags_letters_tr;
				if (it->nb_images == 0)
				{
					temp = tags_letters_tr_span;
				}
				Replace(temp, "{$id}", it->id);
				Replace(temp, "{$name}", it->name);

				string str_photo = get_photo_string(it->nb_images);

				ReplaceOne(temp, "{$str_photo}", str_photo);
				tr_all += temp;
			}

			ReplaceOne(temp0, "{$tags_letters_tr}", tr_all);

			thumbnails += temp0;
		}
	}
	else
	{
		string all_tags = "";
		string cloud_tags_a = "";
		string  cloud_tags_span = "";
		CLang::Instance().getOneData("cloud_tags_a", cloud_tags_a);
		CLang::Instance().getOneData("cloud_tags_span", cloud_tags_span);
		for (it = lstTag->begin(); it != lstTag->end(); it++)
		{
			string temp = cloud_tags_a;
			if (it->nb_images == 0) {
				temp = cloud_tags_span;
			}
			Replace(temp, "{$id}", it->id);
			Replace(temp, "{$name}", it->name);

			string str_photo = get_photo_string(it->nb_images);

			ReplaceOne(temp, "{$nb}", uint2str(it->nb_images));
			ReplaceOne(temp, "{$str_photo}", str_photo);
			all_tags += temp + ",";
		}
		rtrim(all_tags, ",");

		string cloud_tags = "";
		CLang::Instance().getOneData("cloud_tags", cloud_tags);
		ReplaceOne(cloud_tags, "{$cloud_tags}", all_tags);
		thumbnails = cloud_tags;
	}

	if (display_mode != "letters")
	{
		thumbnails = " <div id=\"fullTagCloud\">" + thumbnails;
		thumbnails += "</div>";
	}

	return thumbnails;
}

long CHtmlgetIndexProc::get_max_page(QUERY_REQ req, uint32_t& maxPage)
{
	int curr = 0;

	EnterCriticalSection(&_cs);
	uint32_t nb = (uint32_t)_lstImageDto.size();
	LeaveCriticalSection(&_cs);

	list<string> plstImageIds;
	CDataSrv::Instance().get_lstImageIds(plstImageIds);
	uint32_t nb_images = (uint32_t)plstImageIds.size();

	uint32_t pageTotal = PERPAGE_TOTAL;
	if (nb_images - curr < pageTotal)
	{
		pageTotal = nb_images - curr;
	}

	div_t div_result = div(pageTotal - nb, PERPAGE_NUM);

	if (div_result.rem != 0)
	{
		div_result.quot += 1;
	}
	maxPage = div_result.quot;
	return 0;
}

long CHtmlgetIndexProc::proc_tumb_htm(list<CATEGORY_DTO> lst, QUERY_REQ req0, ORDER_DTO orderDto, uint32_t& nb_images, string& tpl)
{
	QUERY_REQ _req = req0;

	string calendarBar_html = "";

	string thumbnailCategories = "";
	string thumbnails = "";
	string calendarTitle = "";
	string cal_html = "";

	list<IMAGE_DTO> lstDto;


	if (req0.posted_list != "" || req0.created_list != "")
	{
		EnterCriticalSection(&_cs);
		lstDto = _lstImageDto;
		LeaveCriticalSection(&_cs);

		CDataSrv::Instance()._q_req = req0;
		string calendarViews = "";

		CHtmlgetCal::Instance().get_cal_thumbnails(true, req0, CDataSrv::Instance()._search_req, calendarBar_html, nb_images, calendarTitle);

		string cal = "";
		lookitem(req0.args, "cal=", cal);

		calendarViews = CHtmlgetCal::Instance().get_calendarViews(req0, cal, "");
		cal_html = "0";
		ReplaceOne(tpl, "{$calendarViews}", calendarViews);
	}
	else
	{
		calendarTitle = orderDto.title;

		CDataSrv::Instance()._q_req = _req;
		CDataSrv::Instance().clear_cache_image_ids();

		get_init_page(_req, lstDto);

		EnterCriticalSection(&_cs);
		_lstImageDto = lstDto;
		LeaveCriticalSection(&_cs);


		if (req0.list0 == "1")
		{
			string ids = "";
			list<IMAGE_DTO>::iterator it;
			for (it = lstDto.begin(); it != lstDto.end(); it++)
			{
				ids += it->id + ",";
			}
			rtrim(ids, ",");

			req0.list0 = ids;
			_req.list0 = ids;

			CDataSrv::Instance()._q_req = req0;
		}
	}

	if (nb_images > LIMIT_NB_PHOTOS)
	{
		if (req0.q != "" || req0.no_tag != "" || req0.search == "q" || req0.home != "" || req0.recent_pics != "")
		{
			nb_images = LIMIT_NB_PHOTOS;
		}
	}

	//1
	string navigationBar = "";
	if (_req.posted_list == "" && _req.created_list == "")
	{
		list<string> plstImageIds;
		CDataSrv::Instance().get_lstImageIds(plstImageIds);
		uint32_t nb_images = (uint32_t)plstImageIds.size();

		int curr = atoi(_req.start.c_str());
		string url0 = get_up_url(_req);
		navigationBar = get_navigationBar(url0, nb_images, curr, PERPAGE_TOTAL);
	}

	string temp = navigationBar;
	Replace(temp, "{$id}", "1");
	ReplaceOne(tpl, "{$navigationBar}", temp);

	temp = navigationBar;
	Replace(temp, "{$id}", "2");
	ReplaceOne(tpl, "{$navigationBar}", temp);

	//2
	if (_req.posted_list != "" || _req.created_list != "")
	{
		ReplaceOne(tpl, "{$calendarTitle}", calendarTitle);
	}
	else
	{
		if (!lstDto.empty())
		{
			ReplaceOne(tpl, "{$calendarTitle}", calendarTitle);
		}
		else
		{
			ReplaceOne(tpl, "{$calendarTitle}", "");
		}
	}

	if (cal_html == "")
	{
		if (_req.q != "")
		{
			if (lstDto.empty())
			{
				string search_results = "";
				CLang::Instance().getOneData("search_results", search_results);
				ReplaceOne(search_results, "{$q}", _req.q);
				thumbnailCategories = search_results;
			}
		}
		else if (_req.cat_id != "")
		{
			if (_req.posted_list != "" || _req.created_list != "")
			{
			}
			else
			{
				thumbnailCategories += CHtmlgetIndexAction::Instance().get_thumbnailCategories("", _req.cat_id, lst);
			}
		}

		thumbnails = CHtmlgetIndexAction::Instance().get_thumbnails(_req, lstDto, orderDto, lst);
	}

	ReplaceOne(tpl, "{$calendarBar_html}", calendarBar_html);

	uint32_t maxPage0 = 0;
	get_max_page(req0, maxPage0);

	ReplaceOne(tpl, "{$ismsnry}", "1");
	ReplaceOne(tpl, "{$maxPage}", uint2str(maxPage0));

	if (thumbnailCategories == "")
	{
		ReplaceOne(tpl, "max-height:1024px;", "display:none;");
	}
	else
	{
		ReplaceOne(tpl, "max-height:1024px;", "");
	}

	ReplaceOne(tpl, "{$CATEGORIES}", thumbnailCategories);
	ReplaceOne(tpl, "{$THUMBNAILS}", thumbnails);

	return 0;
}

long CHtmlgetIndexProc::update_cache_image(string id, IMAGE_DTO imageDto)
{
	EnterCriticalSection(&_cs);

	list<IMAGE_DTO>::iterator iter;
	for (iter = _lstImageDto.begin(); iter != _lstImageDto.end(); iter++)
	{
		if (iter->id == imageDto.id)
		{
			iter->rotation = imageDto.rotation;
			if (id != "")
			{
				iter->iscoi = imageDto.iscoi;
				iter->coi = imageDto.coi;
				iter->width = imageDto.width;
				iter->height = imageDto.height;
			}
			break;
		}
	}
	LeaveCriticalSection(&_cs);

	return 0;
}

long CHtmlgetIndexProc::push_cache_images(list<IMAGE_DTO> lstImage)
{
	EnterCriticalSection(&_cs);
	list<IMAGE_DTO>::iterator iter;
	for (iter = lstImage.begin(); iter != lstImage.end(); iter++)
	{
		_lstImageDto.push_back(*iter);
	}
	LeaveCriticalSection(&_cs);
	return 0;
}

string CHtmlgetIndexProc::get_banner()
{
	string pagebanner = "";
	CLang::Instance().getOneData("pagebanner", pagebanner);
	ReplaceOne(pagebanner, "{$gallery_title}", CDataSrv::Instance()._gallery_title);
	return pagebanner;
}

long CHtmlgetIndexProc::get_init_page(QUERY_REQ req, list<IMAGE_DTO>& lstDto)
{
	string st = req.start;
	int start = atoi(st.c_str());

	//Cimlog::Instance().LogToFile("get_init_page\t" + Int2Str(start) + " - " + Int2Str(start + PERPAGE_NUM));

	string ids = "";
	int i = 0;
	list<string> plstImageIds;
	CDataSrv::Instance().get_lstImageIds(plstImageIds);
	list<string>::iterator it;
	for (it = plstImageIds.begin(); it != plstImageIds.end(); it++)
	{
		if (i >= start + PERPAGE_NUM)
		{
			break;
		}
		if (i >= start && i < start + PERPAGE_NUM)
		{
			ids += *it + ",";
		}
		i++;
	}
	rtrim(ids, ",");

	Cimagedal::Instance().get_images_by_ids(ids, lstDto);

	return 0;
}

string CHtmlgetIndexProc::get_title(const QUERY_REQ req)
{
	string title = "";

	if (req.cat_id != "")
	{
		list<CATEGORY_DTO> lst;
		CHtmlgetMenubar::Instance().get_categories(req.cat_id, lst);

		list<CATEGORY_DTO>::iterator iter;
		for (iter = lst.begin(); iter != lst.end(); iter++)
		{
			if (iter->id == req.cat_id)
			{
				title = iter->name;
				break;
			}
		}
	}
	else
	{
		if (req.favorites != "")
		{
			title = "{'Favorites'|@translate}";
		}
		else if (req.caddie != "")
		{
			title = "{'Caddie'|@translate}";
		}
		else if (req.most_visited != "")
		{
			title = "{'Most visited'|@translate}";
		}
		else if (req.recent_pics != "")
		{
			title = "{'Recent photos'|@translate}";
		}
		else if (req.no_tag != "")
		{
			title = "{'With no tag'|@translate}";
		}
		else if (req.list0 != "")
		{
			title = "{'Random photos'|@translate}";
		}
		else if (req.recent_cats != "")
		{
			title = "{'Recent albums'|@translate}";
		}
		else if (req.home != "")
		{
		}
		else if (req.tags != "")
		{
			title = "{'Tags'|@translate}";

			if (req.tags != "all")
			{
				list<TAG_DTO>::iterator it;
				list<TAG_DTO>* lstTag = CDataSrv::Instance().get_lstTag();
				for (it = lstTag->begin(); it != lstTag->end(); it++)
				{
					if (it->id == req.tags)
					{
						title = it->name;
						break;
					}
				}
			}
		}
		else if (req.q != "")
		{
			title = "{'Search results'|@translate}";
		}
		else if (req.search != "")
		{
			title = "{'Search'|@translate}";
		}

		if (title == "" && req.posted != "")
		{
			title += req.posted;
		}

	}
	return title;
}

long CHtmlgetIndexProc::get_size(IMAGE_DTO dto, uint32_t& w, uint32_t& h)
{
	uint32_t width = dto.width;
	uint32_t height = dto.height;

	double maxWidth = 1280;
	double maxHeight = 720;

	uint32_t outW = 0;
	uint32_t outH = 0;

	if (width < maxWidth && height < maxHeight) {
		outW = width;
		outH = height;
	}
	else
	{
		double f0 = maxWidth / maxHeight;
		double f1 = (double)width / (double)height;

		if (f0 <= f1)
		{
			outW = (uint32_t)maxWidth;
			outH = (uint32_t)(maxWidth / f1);
		}
		else
		{
			outW = (uint32_t)(maxHeight * f1);
			outH = (uint32_t)maxHeight;
		}
	}
	w = outW;
	h = outH;
	return 0;
}

uint64_t CHtmlgetIndexProc::get_md5sum(string artids, list<IMAGE_DTO>& lstOrder)
{
	list<string> lstOrderIds;
	vector<string> v1 = tokenize(artids, ",", true, "");
	int len = (int)v1.size();
	for (int i = 0; i < len; i++)
	{
		lstOrderIds.push_back(v1[i]);
	}

	Cimagedal::Instance().get_images_all_ids(artids, &lstOrderIds, lstOrder);

	uint64_t md5sum = 0;
	{
		lstOrderIds.sort();
		string tmp = "";
		list<string>::iterator it;
		for (it = lstOrderIds.begin(); it != lstOrderIds.end(); it++)
		{
			tmp += *it + ",";
		}
		rtrim(tmp, ",");

		md5sum = murmur3_32((unsigned char*)tmp.c_str(), (int)tmp.size(), 123456);
	}

	return md5sum;
}

long CHtmlgetIndexProc::get_nav_data(string id, string& data)
{
	string szPath = _w2u(CDataSrv::Instance()._appDataDir);//get_module_path(NULL));

	list<ROOT_DTO> lstRoot;
	CDataSrv::Instance().get_roots(lstRoot);

	string alb = CDataSrv::Instance()._albumName;
	list<IMAGE_DTO>::iterator iter;

	string dDir = (alb == "private") ? "/data2" : "/data1";
	if (id != "0")
	{
		list<IMAGE_DTO> tempDto;
		Cimagedal::Instance().get_images_by_ids(id, tempDto);
		for (iter = tempDto.begin(); iter != tempDto.end(); iter++)
		{
			string tm = get_time_string(iter->date_creation, "d");
			string ori = ori2path(lstRoot, 1, iter->ori);

			string newfile = szPath + dDir + iter->ori + "_coi.png";
			newfile += ".krf";

			_data2_thumb(newfile);

			data += iter->id + "|";
			data += newfile + "|";
			data += ori;
			if (alb == "private")
			{
				data += ".krf";
			}

			data += "|" + tm;
			data += "|";
			data += uint2str(iter->rotation);
			data += "\r\n";
		}
	}
	else
	{
		list<IMAGE_DTO> lstDto;

		EnterCriticalSection(&_cs);
		lstDto = _lstImageDto;
		LeaveCriticalSection(&_cs);

		for (iter = lstDto.begin(); iter != lstDto.end(); iter++)
		{
			if (iter->ext == "mp3" || iter->ext == "mp4" || iter->ext == "gif" || iter->ext == "webm" || iter->ext == "ogg")
			{
			}
			else
			{
				string tm = get_time_string(iter->date_creation, "%c");
				string ori = ori2path(lstRoot, 1, iter->ori);

				string newfile = szPath + dDir + iter->ori + "_coi.png";
				newfile += ".krf";

				_data2_thumb(newfile);

				data += iter->id + "|";
				data += newfile + "|";
				data += ori;
				if (alb == "private")
				{
					data += ".krf";
				}
				data += "|" + tm;
				data += "|";
				data += uint2str(iter->rotation);
				data += "\r\n";
			}
		}
	}

	return 0;
}

long CHtmlgetIndexProc::post_img_say(REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string id = "";
	string w = "";
	string h = "";
	string q = "";

	lookitem(request_dto.szArgs, "id=", id);
	lookitem(request_dto.szArgs, "w=", w);
	lookitem(request_dto.szArgs, "h=", h);
	lookitem(request_dto.szArgs, "q=", q);

	list<ROOT_DTO> lstRoot;
	CDataSrv::Instance().get_roots(lstRoot);

	IMAGE_DTO curr;
	list<IMAGE_DTO> lstImageDto;
	Cimagedal::Instance().get_images_by_ids(id, lstImageDto);

	if (!lstImageDto.empty())
	{
		curr = lstImageDto.front();

		wstring dest = _u2w(ori2path(lstRoot, 1, curr.ori));
		if (CDataSrv::Instance()._albumName == "private")
		{
			dest += L".krf";
		}

		curr.coi_w = atoi(w.c_str());
		curr.coi_h = atoi(h.c_str());
		curr.rotation = 0;
		curr.coi = curr.ori + "_coi.png";
		curr.iscoi = 1;
		curr.coi_quality = atoi(q.c_str());

		struct _stat32  info;
		int result = _wstat32(dest.c_str(), &info);
		if (result == 0)
		{
			curr.coi_lastmodified = static_cast<uint64_t>(info.st_mtime);

			double d = info.st_size;
			d = d / 1024.0f;
			curr.coi_filesize = static_cast<uint32_t>(d * 10 / 1024.0f + 0.5f);
			if (curr.coi_filesize == 0)
			{
				curr.coi_filesize = 1;
			}
		}

		writedb::Instance().update_coi(curr);
		update_cache_image("", curr);
	}

	response_dto.szResponse = "ok";
	response_dto.szContentType = "text/plain; charset=utf-8";

	return 0;
}

long CHtmlgetIndexProc::show_paint(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string hwnd = "";
	string close = "";
	string id = "";
	string type = "";
	lookitem(request_dto.szArgs, "hwnd=", hwnd);
	lookitem(request_dto.szArgs, "id=", id);
	lookitem(request_dto.szArgs, "close=", close);
	lookitem(request_dto.szArgs, "type=", type);

	if (close == "1")
	{
	}
	else
	{
		string usr = CDataSrv::Instance()._code;
		if (CDataSrv::Instance()._albumName == "private")
		{
			usr = "1";
		}
		else
		{
			usr = "0";
		}

		wchar_t buf[MAX_PATH] = { 0 };
		long lSize = ::GetModuleFileName(NULL, buf, MAX_PATH);

		wchar_t* p = wcsrchr(buf, L'\\');
		if (p != NULL)
		{
			p[0] = 0;
		}
		wstring szPath = buf;
		szPath += L"/paint.exe";
		Replace(szPath, L"\\", L"/");

		string lang = CLang::Instance()._language;
		string enckey = CUploadParser::Instance().GetKey();

		HDC hDC = CreateCompatibleDC(0);
		int pixelsy = GetDeviceCaps(hDC, LOGPIXELSY);
		DeleteDC(hDC);

		string args = hwnd + " " + type + " " + Int2Str(pixelsy) + " " + id + " " + lang + " " + usr + " " + enckey + " " + CDataSrv::Instance()._ov;
		ShellExecute(NULL, L"open", szPath.c_str(), _u2w(args).c_str(), NULL, SW_SHOWNORMAL);
	}
	//DWORD hr = ::GetLastError();

	return 0;
}

long CHtmlgetIndexProc::get_action(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string download = " ";
	lookitem(request_dto.szArgs, "download=", download);

	if (download == "1")
	{
		string id = "";
		lookitem(request_dto.szArgs, "id=", id);

		string url = "";
		string fname = "";

		list<IMAGE_DTO> lstImageDto;
		Cimagedal::Instance().get_images_by_ids(id, lstImageDto);

		if (!lstImageDto.empty())
		{
			list<ROOT_DTO> lstRoot;
			CDataSrv::Instance().get_roots(lstRoot);

			url = ori2path(lstRoot, 1, lstImageDto.front().ori);
			fname = lstImageDto.front().file;
		}

		response_dto.szResponse = url;
		response_dto.szContentType = "filename:" + fname;
		//Content-Disposition: attachment; filename="77094b36acaf2eddcccc86e3831001e9390193ff.jpg";

	}
	else if (download == "action.u3down")
	{
		string path = "";
		//download=2&path=./local_file?F:\exif\&%E4%BD%A9%20%27%27+0#%20%E7%BD%97%20-%20Copy.jpg
		string::size_type pos = request_dto.szArgs.find("f=");
		if (pos != string::npos)
		{
			path = request_dto.szArgs.substr(pos + 2);
		}

		string curr_name = "";
		char* p = strrchr((char*)path.c_str(), '/');
		if (p != NULL)
		{
			curr_name = p + 1;
		}

		response_dto.szResponse = "./local_file?f=" + path;
		response_dto.szContentType = "filename:" + curr_name;
	}
	return 0;
}

void CHtmlgetIndexProc::mk_path(string& title1)
{
	Replace(title1, "\"", "");
	Replace(title1, "'", "");
	Replace(title1, "?", "");
	Replace(title1, ":", "");
	Replace(title1, "/", "");
	Replace(title1, "\\", "");
	Replace(title1, "*", "");
	Replace(title1, "|", "");
}

long CHtmlgetIndexProc::get_cache(int ty, list<TEMPL_DTO>& lstDto)
{
	int i = 0;
	list<IMAGE_DTO>::iterator iter;

	EnterCriticalSection(&_cs);
	if (ty == 0)
	{
		for (iter = _lstImageDto.begin(); iter != _lstImageDto.end(); iter++)
		{
			if (iter->quality == 0 &&
				iter->ext != "mp3" && iter->ext != "mp4" && iter->ext != "ogg" && iter->ext != "webm" && iter->ext != "gif")
			{
				TEMPL_DTO dto;
				dto.id = iter->id;
				dto.val = iter->ori;
				lstDto.push_back(dto);

				i++;
				if (i > 5)
				{
					break;
				}
			}
		}
	}
	else
	{
		for (iter = _lstImageDto.begin(); iter != _lstImageDto.end(); iter++)
		{
			if (iter->quality > 0)
			{
				TEMPL_DTO dto;
				dto.id = iter->id;
				dto.val = float2Str(static_cast<float>(iter->quality) / 100.0f, "%.1f");
				lstDto.push_back(dto);
			}
		}
	}
	LeaveCriticalSection(&_cs);
	return 0;
}

long CHtmlgetIndexProc::update_cache(list<TEMPL_DTO> lstQuality)
{
	list<TEMPL_DTO>::iterator it;
	list<IMAGE_DTO>::iterator iter;

	EnterCriticalSection(&_cs);
	for (it = lstQuality.begin(); it != lstQuality.end(); it++)
	{
		for (iter = _lstImageDto.begin(); iter != _lstImageDto.end(); iter++)
		{
			if (it->id == iter->id)
			{
				iter->quality = get_uint32(it->val.c_str());
				break;
			}
		}
	}
	LeaveCriticalSection(&_cs);

	return 0;
}
