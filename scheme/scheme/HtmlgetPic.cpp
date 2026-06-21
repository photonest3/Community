#include "StdAfx.h"
#include "unitil.h"
#include <vector>
#include <string>
#include "../../templ/templ/resource.h"

#include "HtmlgetPic.h"
#include "HtmlgetpicProc.h"
#include "HtmlgetIndexProc.h"
#include "HtmlgetMenubar.h"
#include "DataSrv.h"
#include "lang.h"
#include "writedb.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

CHtmlgetPic::CHtmlgetPic(void)
{
}

CHtmlgetPic::~CHtmlgetPic(void)
{
}

long CHtmlgetPic::get_picture_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string act = "";
	string slideshow = "";
	lookitem(request_dto.szArgs, "slideshow=", slideshow);
	lookitem(request_dto.szArgs, "act=", act);

	HRESULT hr = S_OK;

	if (act == "picture.get_data")
	{
		string type = "only";
		lookitem(request_dto.szArgs, "type=", type);
		if (type != "")
		{
			type = "all";
		}
		CHtmlgetpicProc::Instance().get_details(type, request_dto, response_dto);
		return S_OK;
	}
	else if (act == "picture.get_src")
	{
		CHtmlgetpicProc::Instance().get_src(request_dto, response_dto);
		return S_OK;
	}
	else if (act == "picture.get_page")
	{
		string from = "";
		string to = "";
		string page0 = "";
		string osm = "";
		lookitem(request_dto.szArgs, "from=", from);
		lookitem(request_dto.szArgs, "to=", to);
		lookitem(request_dto.szArgs, "page0=", page0);
		lookitem(request_dto.szArgs, "osm=", osm);

		string slider_nav = "";
		string picture_content = "";
		if (page0 == "picture")
		{
			CHtmlgetpicProc::Instance().proc_boxshow_htm(osm, atoi(from.c_str()), atoi(to.c_str()), picture_content);
			response_dto.szContentType = "text/plain; charset=utf-8";
			response_dto.szResponse = picture_content;
		}
		else
		{
			list<string> plstImageIds;
			if (osm != "")
			{

				plstImageIds = CDataSrv::Instance()._lstosm;
			}
			else
			{
				CDataSrv::Instance().get_lstImageIds(plstImageIds);
			}

			CHtmlgetpicProc::Instance().proc_slide_htm(plstImageIds, atoi(from.c_str()), atoi(to.c_str()), slider_nav, picture_content);

			StringBuffer sb;
			Writer<StringBuffer> writer(sb);
			writer.StartObject();
			writer.String("slider_for");
			writer.String(picture_content.c_str());
			writer.String("slider_nav");
			writer.String(slider_nav.c_str());
			writer.EndObject();

			response_dto.szContentType = "text/plain; charset=utf-8";
			response_dto.szResponse = sb.GetString();
		}

		return S_OK;
	}
	else if (act == "picture.add_to_favorites")
	{
		string picid = "";
		lookitem(request_dto.szArgs, "picid=", picid);
		if (picid == "")
		{
			picid = get_picid(request_dto);
		}

		string favorite = "";
		long hr = writedb::Instance().add_favorites("add", CDataSrv::Instance()._login, picid);
		if (hr == 0)
		{
			CDataSrv::Instance().refresh_favorites();
			CLang::Instance().getOneData("actionButtons_favorite_del", favorite);
		}
		else
		{
			CLang::Instance().getOneData("actionButtons_favorite_add", favorite);
		}
		translate(favorite, "", "");

		response_dto.szContentType = "text/html";
		response_dto.szResponse = favorite;
		return S_OK;
	}
	else if (act == "picture.update_comment")
	{
		string picid = "";
		string title = "";
		lookitem(request_dto.szArgs, "picid=", picid);
		lookitem(request_dto.szArgs, "title=", title);
		URLDecode(title, 1);
		if (picid == "")
		{
			picid = get_picid(request_dto);
		}

		writedb::Instance().update_images("comment", picid, title);

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.EndObject();
		response_dto.szResponse = sb.GetString();
		response_dto.szContentType = "text/plain; charset=utf-8";
		return S_OK;
	}
	else if (act == "picture.remove_favorites")
	{
		string picid = "";
		lookitem(request_dto.szArgs, "picid=", picid);
		if (picid == "")
		{
			picid = get_picid(request_dto);
		}

		string favorite = "";
		long hr = writedb::Instance().add_favorites("remove", CDataSrv::Instance()._login, picid);
		if (hr == 0)
		{
			CDataSrv::Instance().refresh_favorites();
			CLang::Instance().getOneData("actionButtons_favorite_add", favorite);
		}
		else
		{
			CLang::Instance().getOneData("actionButtons_favorite_del", favorite);
		}
		translate(favorite, "", "");
		response_dto.szContentType = "text/html";
		response_dto.szResponse = favorite;

		return S_OK;
	}
	else if (act == "picture.set_as_representative")
	{
		string picid = get_picid(request_dto);
		string category = "";
		lookitem(request_dto.szArgs, "category=", category);

		bool b = false;
		long hr = writedb::Instance().update_categories_representative(category, picid);
		if (hr == 0)
		{
			list<CATEGORY_DTO> lst;
			CHtmlgetMenubar::Instance().get_categories(category, lst);

			list<CATEGORY_DTO>::iterator iter2;
			for (iter2 = lst.begin(); iter2 != lst.end(); iter2++)
			{
				if (iter2->id == category && iter2->representative_id == picid)
				{
					b = true;
					break;
				}
			}
		}

		string representative = "";
		if (b)
		{
			CLang::Instance().getOneData("actionButtons_representative_span", representative);
		}
		else
		{
			CLang::Instance().getOneData("actionButtons_representative", representative);
		}

		translate(representative, "", "");
		response_dto.szContentType = "text/html";
		response_dto.szResponse = representative;

		return S_OK;
	}
	else if (act == "picture.set_rotate")
	{
		//viewer.u3.min.js
		string picid = "";
		string ro = "";
		lookitem(request_dto.szArgs, "picid=", picid);
		lookitem(request_dto.szArgs, "ro=", ro);

		if (picid != "")
		{
			int degree = atoi(ro.c_str());
			string rotation = "0";

			int r = degree % 360;
			if (r == 90 || r == -270)
			{
				rotation = "6";
			}
			else if (r == -90 || r == 270)
			{
				rotation = "8";
			}
			else if (r == 180 || r == -180)
			{
				rotation = "3";
			}

			writedb::Instance().update_images("rotate", picid, rotation);

			IMAGE_DTO imageDto;
			imageDto.id = picid;
			imageDto.rotation = get_uint32(rotation.c_str());
			CHtmlgetIndexProc::Instance().update_cache_image(picid, imageDto);
		}

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.EndObject();
		response_dto.szResponse = sb.GetString();
		response_dto.szContentType = "text/plain; charset=utf-8";
	}
	else if (act == "picture.add_to_caddie")
	{
		string op = "";
		lookitem(request_dto.szArgs, "op=", op);
		string picid = get_picid(request_dto);
		long hr = writedb::Instance().add_caddie(CDataSrv::Instance()._login, op, picid);

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.EndObject();
		response_dto.szResponse = sb.GetString();
		response_dto.szContentType = "text/plain; charset=utf-8";
		return S_OK;
	}
	else if (act == "picture.metadata")
	{
		if (CDataSrv::Instance()._metadata == "nometa")
		{
			CDataSrv::Instance()._metadata = "metadata";
		}
		else
		{
			CDataSrv::Instance()._metadata = "nometa";
		}
		CDataSrv::Instance().set_config_val("metadata", CDataSrv::Instance()._metadata);

		CHtmlgetpicProc::Instance().get_details("only", request_dto, response_dto);
		return S_OK;
	}
	else if (act == "picture.fullsize")
	{
		if (CDataSrv::Instance()._vrsize == "3")
		{
			CDataSrv::Instance()._vrsize = "1";
		}
		else
		{
			CDataSrv::Instance()._vrsize = "3";
		}
		CDataSrv::Instance().set_config_val("vrSize", CDataSrv::Instance()._vrsize);

		response_dto.szContentType = "text/html";
		response_dto.szResponse = CDataSrv::Instance()._vrsize;
		return S_OK;
	}
	else if (act == "picture.get_speed")
	{
		CHtmlgetpicProc::Instance().get_speed(request_dto, response_dto);
		return S_OK;
	}

	string tpl = "";
	hr = get_picture_template(request_dto, tpl);

	response_dto.szResponse = tpl;
	response_dto.szContentType = "text/html";

	return S_OK;
}

long CHtmlgetPic::get_picture_template(const REQUEST_DTO request_dto, string& tpl)
{
	QUERY_REQ req;
	get_req(0, request_dto, req);

	string picid = "";
	string slideshow = "";
	string idx0 = "";
	string osm = "";
	lookitem(request_dto.szArgs, "idx=", idx0);
	lookitem(request_dto.szArgs, "id=", picid);
	lookitem(request_dto.szArgs, "slideshow=", slideshow);
	lookitem(request_dto.szArgs, "osm=", osm);

	assert(picid != "");
	if (picid == "")
	{
		picid = "0";
	}
	list<string> plstImageIds;
	if (osm != "")
	{

		plstImageIds = CDataSrv::Instance()._lstosm;
	}
	else
	{
		CDataSrv::Instance().get_lstImageIds(plstImageIds);
	}

	int idx = 0;
	BOOL bfav = false;
	long ret = get_curr_images(plstImageIds, idx0, picid, idx, bfav);
	
	uint32_t nb_images = (uint32_t)plstImageIds.size();

	Htmlread(IDR_PICTURE_HTML, L"html/_picture.html", tpl);
	ReplaceOne(tpl, "strawberry_jam", CDataSrv::Instance()._skin);
	ReplaceOne(tpl, "{$skin}", CDataSrv::Instance()._skin);
	ReplaceOne(tpl, "{$album}", CDataSrv::Instance()._albumName);

	string op = ".75";

	ReplaceOne(tpl, "{$skinOpacity}", op);
	ReplaceOne(tpl, "{$bgtype}", CDataSrv::Instance()._bgtype);
	ReplaceOne(tpl, "{$bg}", CDataSrv::Instance()._bg);

	if (CDataSrv::Instance()._skin != "dark" && CDataSrv::Instance()._skin != "quartz" && CDataSrv::Instance()._skin != "newspaper" && CDataSrv::Instance()._skin != "strawberry_jam")
	{
		ReplaceOne(tpl, "selectize.dark.css", "selectize.clear.css");
	}


	ReplaceOne(tpl, "{$ov}", CDataSrv::Instance()._ov);
	ReplaceOne(tpl, "{$metadata}", CDataSrv::Instance()._metadata);
	ReplaceOne(tpl, "{$osm}", osm);
	ReplaceOne(tpl, "{$vrsize}", CDataSrv::Instance()._vrsize);
	ReplaceOne(tpl, "{$PERPAGE_TOTAL}", Int2Str(PERPAGE_TOTAL));


	string error_lic = "";
	if (CDataSrv::Instance()._ov == "1")
	{
		CLang::Instance().getOneData("error_lic", error_lic);

		ReplaceOne(error_lic, "999", "");
	}
	ReplaceOne(tpl, "{$error_lic}", error_lic);

	uint32_t r = CDataSrv::Instance()._readonly;
	ReplaceOne(tpl, "{$readonly}", uint2str(r));
	ReplaceOne(tpl, "{$error_readonly}", CDataSrv::Instance()._str_readonly);

	uint32_t page_nb = idx / PERPAGE_TOTAL;
	if (idx0 != "")
	{
		int t = atoi(idx0.c_str());
		ldiv_t div1 = ldiv(t, PERPAGE_TOTAL);
		page_nb = div1.quot;
	}

	uint32_t from = 0;
	uint32_t to = 3 * PERPAGE_TOTAL;
	to = PERPAGE_TOTAL;
	if (nb_images <= to)
	{
		to = 0;
		ReplaceOne(tpl, "{$nbPage}", "0");
		ReplaceOne(tpl, "{$total}", uint2str(nb_images));
	}
	else
	{
		if (page_nb > 0)
		{
			from = page_nb * PERPAGE_TOTAL;
			to = page_nb * PERPAGE_TOTAL + PERPAGE_TOTAL;
		}
		if (nb_images <= to)
		{
			to = nb_images;
		}
		ReplaceOne(tpl, "{$nbPage}", uint2str(page_nb));
		ReplaceOne(tpl, "{$total}", uint2str(to));
	}

	ReplaceOne(tpl, "{$init}", Int2Str(idx));
	ReplaceOne(tpl, "{$slideshow}", slideshow);
	ReplaceOne(tpl, "{$category}", req.cat_id);

	//1
	if (page_nb > 0)
	{
		req.args += "&start=" + uint2str(page_nb * PERPAGE_TOTAL);
	}
	list<CATEGORY_DTO> lst;
	CHtmlgetMenubar::Instance().get_categories(req.cat_id, lst);


	string title = get_title(lst, "picture.html", req, picid, nb_images);
	ReplaceOne(tpl, "{$title}", title);

	//2
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
	{
		string actionButtons_cmdMeta = "";
		CLang::Instance().getOneData("actionButtons_cmdMeta", actionButtons_cmdMeta);

		string actionButtons_properties = "";
		CLang::Instance().getOneData("actionButtons_properties", actionButtons_properties);

		string actionButtons_favorite = "";
		if (bfav)
		{
			CLang::Instance().getOneData("actionButtons_favorite_del", actionButtons_favorite);
		}
		else
		{
			CLang::Instance().getOneData("actionButtons_favorite_add", actionButtons_favorite);
		}

		string actionButtons_representative = "";
		CLang::Instance().getOneData("actionButtons_representative", actionButtons_representative);

		string actionButtons_editphoto = "";
		CLang::Instance().getOneData("actionButtons_editphoto", actionButtons_editphoto);

		string actionButtons_cmdCaddie = "";
		CLang::Instance().getOneData("actionButtons_cmdCaddie", actionButtons_cmdCaddie);

		actionButtons_properties = "<span id=\"cmdproperties\" >" + actionButtons_properties + "</span>";
		actionButtons_favorite = "<span id=\"cmdfavorite\" >" + actionButtons_favorite + "</span>";

		actionButtons_representative = "<span id=\"cmdSetRepresentative\" >" + actionButtons_representative + "</span>";
		actionButtons_editphoto = "<span id=\"cmdedit\" >" + actionButtons_editphoto + "</span>";
		actionButtons_cmdCaddie = "<span id=\"cmdCaddie\" >" + actionButtons_cmdCaddie + "</span>";
		actionButtons_cmdMeta = "<span id=\"cmdMeta\" >" + actionButtons_cmdMeta + "</span>";

		actionButtons += actionButtons_editphoto;

		actionButtons += actionButtons_favorite;
		actionButtons += actionButtons_cmdCaddie;

		if (req.cat_id != "")
		{
			actionButtons += actionButtons_representative;
		}
		actionButtons += actionButtons_properties;
		actionButtons += actionButtons_cmdMeta;
	}
	ReplaceOne(tpl, "{$actionButtons}", actionButtons);

	translate(tpl, "", "");
	//4
	string picture_content = "";
	CHtmlgetpicProc::Instance().proc_boxshow_htm(osm, from, to, picture_content);
	ReplaceOne(tpl, "{$picture_content}", picture_content);

	return S_OK;
}

string CHtmlgetPic::get_picid(const REQUEST_DTO request_dto)
{
	string picid = "0";
	string idx = "";
	lookitem(request_dto.szArgs, "idx=", idx);
	int index = atoi(idx.c_str());

	int i = 0;
	list<string> plstImageIds;
	CDataSrv::Instance().get_lstImageIds(plstImageIds);
	list<string>::iterator it;
	for (it = plstImageIds.begin(); it != plstImageIds.end(); it++)
	{
		if (index == i)
		{
			picid = *it;
			break;
		}
		i++;
	}
	return picid;
}

long CHtmlgetPic::get_curr_images(list<string> plstImageIds, string idx0, string& picid, int& idx, BOOL& bfav)
{
	long hr = 1;
	list<string>::iterator it;

	idx = 0;

	if (idx0 != "")
	{
		int t = atoi(idx0.c_str());
		for (it = plstImageIds.begin(); it != plstImageIds.end(); it++)
		{
			if (idx == t)
			{
				picid = *it;
				hr = 0;
				break;
			}
			idx++;
		}
	}
	else
	{
		for (it = plstImageIds.begin(); it != plstImageIds.end(); it++)
		{
			if (picid == *it)
			{
				hr = 0;
				break;
			}
			idx++;
		}
	}

	bfav = false;

	list<FAVORITE_DTO>::iterator I;
	for (I = CDataSrv::Instance()._lstFavorite.begin(); I != CDataSrv::Instance()._lstFavorite.end(); I++)
	{
		if (I->image_id == picid)
		{
			bfav = true;
			break;
		}
	}
	return hr;
}
