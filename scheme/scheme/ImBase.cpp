#include "StdAfx.h"

#include "../../templ/templ/resource.h"
#include "../../templ/templ/templ.h"

#include <shobjidl.h>
#include <shellapi.h>
#include <assert.h>

#include "ImBase.h"
#include "unitil.h"
#include "lang.h"
#include "DataSrv.h"

#pragma comment( lib, "../../../PhotoNest/tests/cefclient/Release/bin/templ.lib" ) 

CImBase::CImBase(void)
{
}

CImBase::~CImBase(void)
{
}

long CImBase::Htmlread(UINT cr_id, wstring filename, string& szhtml)
{
	szhtml = "";
	char* szResponse = 0;
	int ret = Read_htm(cr_id, filename.c_str(), &szResponse);
	if (ret == 0)
	{
		szhtml = szResponse;
		LocalFree(szResponse);
	}

	return 0;
}

bool CImBase::translate(string& strText, string intVal, string szVal)
{
	return CLang::Instance().translate(strText, intVal, szVal);
}

string CImBase::get_title(list<CATEGORY_DTO> lst, string szUrl, const QUERY_REQ req, string picid, uint32_t nb_images)
{
	string cal = "";
	string week = "";
	lookitem(req.args, "cal=", cal);
	lookitem(req.args, "week=", week);

	int len = 0;
	if (req.posted != "")
	{
		vector<string> v1 = tokenize(req.posted, "-", true, "");
		len = (int)v1.size();
	}

	string sub_title_a = "";
	CLang::Instance().getOneData("sub_title_a", sub_title_a);

	if (szUrl == "picture.html")
	{
		CLang::Instance().getOneData("sub_title_a_pic", sub_title_a);
	}

	string title = sub_title_a;
	ReplaceOne(title, "{$url}", "index.html");
	ReplaceOne(title, "{$name}", "{'Home'|@translate}");

	string nb = "";
	if (nb_images >= 0)
	{
		nb = "<span class=\"badge nb_items\">" + uint2str(nb_images) + "</span>";
	}

	if (req.cat_id != "")
	{
		CATEGORY_DTO curr;
		string full_id = "";
		list<CATEGORY_DTO>::iterator iter;
		for (iter = lst.begin(); iter != lst.end(); iter++)
		{
			if (iter->id == req.cat_id)
			{
				curr = *iter;
			}

			vector<string> v1 = tokenize(iter->full_id, ",", true, "");
			int len = (int)v1.size();
			for (int i = 0; i < len; i++)
			{
				if (v1[i] == req.cat_id)
				{
					full_id = iter->full_id;
					break;
				}
			}
		}

		string szT0 = " / </span> " + sub_title_a;
		ReplaceOne(szT0, "{$url}", "index.html?category={$id}");

		string sub_title = "";

		vector<string> v1 = tokenize(full_id, ",", true, "");
		int len = (int)v1.size();
		for (int i = 0; i < len; i++)
		{
			for (iter = lst.begin(); iter != lst.end(); iter++)
			{
				if (v1[i] == iter->id)
				{
					string temp = szT0;
					if (v1[i] == req.cat_id)
					{
						temp = " / " + sub_title_a;
						ReplaceOne(temp, "{$url}", "index.html?category={$id}&{$arg_ext}");
					}

					ReplaceOne(temp, "{$id}", iter->id);
					ReplaceOne(temp, "{$name}", iter->name);
					sub_title += temp;
				}
			}

			if (v1[i] == req.cat_id)
			{
				break;
			}
		}

		if (picid == "")
		{
			if (req.posted_list != "" || req.created_list != "")
			{
				sub_title += "<span class=\"badge nb_items\">" + uint2str(nb_images) + "</span>";
			}
			else
			{
				sub_title += "<span class=\"badge nb_items\">" + uint2str(curr.nb_images) + "</span>";
			}
		}

		title += sub_title;
	}
	else
	{
		if (req.favorites != "")
		{
			string szT = " / " + sub_title_a;
			ReplaceOne(szT, "{$url}", "index.html?favorites=1&{$arg_ext}");
			ReplaceOne(szT, "{$name}", "{'Favorites'|@translate}");

			title += szT;
			if (picid == "")
			{
				title += nb;
			}
		}
		else if (req.caddie != "")
		{
			string szT = " / " + sub_title_a;
			ReplaceOne(szT, "{$url}", "index.html?caddie=1&{$arg_ext}");
			ReplaceOne(szT, "{$name}", "{'Caddie'|@translate}");

			title += szT;
			if (picid == "")
			{
				title += nb;
			}
		}
		else if (req.most_visited != "")
		{
			string szT = " / " + sub_title_a;
			ReplaceOne(szT, "{$url}", "index.html?most_visited=1&{$arg_ext}");
			ReplaceOne(szT, "{$name}", "{'Most visited'|@translate}");

			title += szT;
			if (picid == "")
			{
				title += nb;
			}
		}
		else if (req.recent_pics != "")
		{
			string szT = " / " + sub_title_a;
			ReplaceOne(szT, "{$url}", "index.html?recent_pics=1&{$arg_ext}");
			ReplaceOne(szT, "{$name}", "{'Recent photos'|@translate}");

			title += szT;
			if (picid == "")
			{
				if (nb_images >= LIMIT_NB_PHOTOS)
				{
					if ((cal == "calendar" && len == 2) || (cal == "weekly" && week == ""))
					{
					}
					else
					{
						nb = "<span class=\"badge nb_items\">" + uint2str(nb_images) + "</span>";
					}
				}
				title += nb;
			}
		}
		else if (req.no_tag != "")
		{
			string szT = " / " + sub_title_a;
			ReplaceOne(szT, "{$url}", "index.html?no_tag=1&{$arg_ext}");
			ReplaceOne(szT, "{$name}", "{'With no tag'|@translate}");

			title += szT;
			if (picid == "")
			{
				if (nb_images >= LIMIT_NB_PHOTOS)
				{
					if ((cal == "calendar" && len == 2) || (cal == "weekly" && week == ""))
					{
					}
					else
					{
						nb = "<span class=\"badge nb_items\">" + uint2str(nb_images) + "</span>";
					}
				}
				title += nb;
			}
		}
		else if (req.list0 != "")
		{
			string url = "index.html?list=1&{$arg_ext}";
			string szT = " / " + sub_title_a;
			if (szUrl == "picture.html")
			{
				url = "index.html?list=" + req.list0 + "&{$arg_ext}";
			}

			ReplaceOne(szT, "{$url}", url);
			ReplaceOne(szT, "{$name}", "{'Random photos'|@translate}");

			title += szT;
			if (picid == "")
			{
				title += nb;
			}
		}
		else if (req.recent_cats != "")
		{
			string szT = " / " + sub_title_a;
			ReplaceOne(szT, "{$url}", "index.html?recent_cats=1&{$arg_ext}");
			ReplaceOne(szT, "{$name}", "{'Recent albums'|@translate}");
			title += szT;
			if (nb_images > 0)
			{
				if (picid == "")
				{
					title += nb;
				}
			}
		}
		else if (req.face != "")
		{
			string szT = " / " + sub_title_a;
			ReplaceOne(szT, "{$url}", "index.html?face=" + req.face);
			ReplaceOne(szT, "{$name}", "{'Face Classification'|@translate}");
			title += szT;
			if (nb_images > 0)
			{
				if (picid == "")
				{
					title += nb;
				}
			}
		}
		else if (req.home != "")
		{
			string szT = "";
			title += szT;
			if (nb_images > 0)
			{
				if (picid == "")
				{
					title += nb;
				}
			}
		}
		else if (req.tags != "")
		{
			//&display_mode=letters
			string szT = " / " + sub_title_a;
			ReplaceOne(szT, "{$url}", "index.html?tags=all&display_mode=letters");
			ReplaceOne(szT, "{$name}", "{'Tags'|@translate}");

			title += szT;
			list<TAG_DTO>::iterator it;
			list<TAG_DTO>* lstTag = CDataSrv::Instance().get_lstTag();
			if (req.tags == "all")
			{
				if (picid == "")
				{
					//nb = "<span class=\"badge nb_items\">" + uint2str((uint32_t)lstTag->size()) + "</span>";
					title += nb;
				}
			}
			else
			{
				for (it = lstTag->begin(); it != lstTag->end(); it++)
				{
					if (it->id == req.tags)
					{
						string szT0 = sub_title_a;
						ReplaceOne(szT0, "{$url}", "index.html?tags={$tags}&{$arg_ext}");

						ReplaceOne(szT0, "{$tags}", req.tags);
						ReplaceOne(szT0, "{$name}", it->name);
						title += " / " + szT0;
						break;
					}
				}
				if (picid == "")
				{
					title += nb;
				}
			}
		}
		else if (req.art_list != "")
		{
			string szT = " / " + sub_title_a;
			ReplaceOne(szT, "{$url}", "index.html?art_list=1");
			ReplaceOne(szT, "{$name}", "{'My creation'|@translate}");

			title += szT;
			if (picid == "")
			{
				list<FILM_DTO> lstfilm;
				CDataSrv::Instance().get_films(lstfilm);
				nb = "<span class=\"badge nb_items\">" + uint2str((uint32_t)lstfilm.size()) + "</span>";
				title += nb;
			}
		}
		else if (req.q != "")
		{
			string szT = " / " + sub_title_a;
			ReplaceOne(szT, "{$url}", "index.html?q={$q}&{$arg_ext}");
			ReplaceOne(szT, "{$name}", "{'Search results'|@translate}");

			ReplaceOne(szT, "{$q}", req.q);
			title += szT;
			if (picid == "")
			{
				if (nb_images >= LIMIT_NB_PHOTOS)
				{
					if ((cal == "calendar" && len == 2) || (cal == "weekly" && week == ""))
					{
					}
					else
					{
						nb = "<span class=\"badge nb_items\">" + uint2str(nb_images) + "</span>";
					}
				}
				title += nb;
			}
		}
		else if (req.search != "")
		{
			string szT = " / " + sub_title_a;

			if (req.search == "q")
			{
				if (szUrl != "picture.html")
				{
					string sub_title_a3 = "";
					CLang::Instance().getOneData("sub_title_a3", sub_title_a3);
					szT = " / " + sub_title_a3;
				}
				ReplaceOne(szT, "{$url}", "index.html?search={$search}&{$arg_ext}");
			}
			else
			{
				ReplaceOne(szT, "{$url}", "index.html?search={$search}&{$arg_ext}");
			}
			ReplaceOne(szT, "{$name}", "{'Search'|@translate}");
			ReplaceOne(szT, "{$search}", req.search);
			title += szT;

			if (nb_images >= LIMIT_NB_PHOTOS)
			{
				if ((cal == "calendar" && len == 2) || (cal == "weekly" && week == ""))
				{
				}
				else
				{
					nb = "<span class=\"badge nb_items\">" + uint2str(nb_images) + "</span>";
				}
			}

			if (req.search == "q")
			{
				if (picid == "")
				{
					title += nb;
				}
			}
		}
		else if (req.posted_list != "" || req.created_list != "")
		{
			title = sub_title_a;
			ReplaceOne(title, "{$url}", "index.html?home=1&{$arg_ext}");
			ReplaceOne(title, "{$name}", "{'Home'|@translate}");

			if (picid == "")
			{
				if (nb_images >= LIMIT_NB_PHOTOS)
				{
					if ((cal == "calendar" && len == 2) || (cal == "weekly" && week == ""))
					{
					}
					else if (req.home != "")
					{
						nb = "<span class=\"badge nb_items\">" + uint2str(nb_images) + "</span>";
					}
				}
				title += nb;
			}
		}
		else
		{
			string profile = "";
			lookitem(req.args, "profile=", profile);
			if (profile != "")
			{
				if (req.szBody == "")
				{
					string szT = " / {'Profile'|@translate}";
					title += szT;
				}
			}
		}
	}

	string arg = "";

	if (szUrl == "picture.html")
	{
		if (cal != "")
		{
			string week = "";
			lookitem(req.args, "week=", week);
			if (week != "")
			{
				arg += "&week=" + week;
			}

			if (cal == "details")
			{
				if (req.posted.size() > 4)
				{
					cal = "calendar";
				}
				else
				{
					cal = "weekly";
				}
			}

			arg += "&cal=" + cal;
		}

		string start = "";
		lookitem(req.args, "start=", start);
		if (start != "")
		{
			arg += "&start=" + start;
		}

		if (req.posted != "")
		{
			arg += "&posted=" + req.posted;
		}

		if (req.posted_list != "")
		{
			arg += "&posted_list=1";
		}

		if (req.created_list != "")
		{
			arg += "&created_list=1";
		}
	}
	Replace(title, "&{$arg_ext}", arg);

	if (szUrl != "picture.html")
	{
		title = "<h2 id=\"titleh2\" >" + title + "</h2>";

	}

	return title;
}

long CImBase::get_req(int type, const REQUEST_DTO request_dto, QUERY_REQ& req)
{
	req.imageids = "";
	req.args = request_dto.szArgs;
	req.szBody = request_dto.szBody;

	lookitem(request_dto.szArgs, "category=", req.cat_id);
	lookitem(request_dto.szArgs, "page_nb=", req.page_nb);
	lookitem(request_dto.szArgs, "favorites=", req.favorites);
	lookitem(request_dto.szArgs, "caddie=", req.caddie);
	lookitem(request_dto.szArgs, "most_visited=", req.most_visited);
	lookitem(request_dto.szArgs, "recent_pics=", req.recent_pics);
	lookitem(request_dto.szArgs, "home=", req.home);
	lookitem(request_dto.szArgs, "no_tag=", req.no_tag);
	lookitem(request_dto.szArgs, "art_list=", req.art_list);
	lookitem(request_dto.szArgs, "longitude=", req.longitude);
	lookitem(request_dto.szArgs, "latitude=", req.latitude);
	lookitem(request_dto.szArgs, "list=", req.list0);
	lookitem(request_dto.szArgs, "posted_list=", req.posted_list);
	lookitem(request_dto.szArgs, "created_list=", req.created_list);
	lookitem(request_dto.szArgs, "posted=", req.posted);
	lookitem(request_dto.szArgs, "recent_cats=", req.recent_cats);
	lookitem(request_dto.szArgs, "tags=", req.tags);
	lookitem(request_dto.szArgs, "osm=", req.osm);
	lookitem(request_dto.szArgs, "q=", req.q);
	lookitem(request_dto.szArgs, "search=", req.search);
	lookitem(request_dto.szArgs, "start=", req.start);
	lookitem(request_dto.szArgs, "image_order=", req.image_order);
	lookitem(request_dto.szArgs, "face=", req.face);

	rtrim(req.face, ",");

	if (type == 0)
	{
		URLDecode(req.q, 0);
	}
	esc_string(req.q);

	set_req(req.posted, req);

	if (req.page_nb.empty())
	{
		req.page_nb = "0";
	}

	return 0;
}

long CImBase::set_req(string posted, QUERY_REQ& req)
{
	if (posted != "")
	{
		req.posted = posted;

		struct tm t_last;
		t_last.tm_mon = 0;
		t_last.tm_mday = 1;

		vector<string> v1 = tokenize(posted, "-", true, "");
		int len = (int)v1.size();
		for (int i = 0; i < len; i++)
		{
			if (i == 0)
			{
				t_last.tm_year = atoi(v1[0].c_str()) - 1900;
			}
			if (i == 1)
			{
				t_last.tm_mon = atoi(v1[1].c_str()) - 1;
			}
			if (i == 2)
			{
				t_last.tm_mday = atoi(v1[2].c_str());
			}
		}

		t_last.tm_hour = 0;
		t_last.tm_min = 0;
		t_last.tm_sec = 0;
		t_last.tm_isdst = 0;

		time_t t0 = _mkgmtime(&t_last);// +CDataSrv::Instance()._time_zone_bias * 60;

		int day = getdays(t_last.tm_year + 1900, t_last.tm_mon + 1);
		time_t t1 = t0 + day * 24 * 3600;
		if (len == 1)
		{
			day = getdays(t_last.tm_year + 1900, 2) + 365 - 28;
			t1 = t0 + day * 24 * 3600;
		}
		else if (len == 3)
		{
			t1 = t0 + 24 * 3600;
		}

		//useto date_available_day, date_creation_day

		req.date_start = uint64str(static_cast<uint64_t>(t0));
		req.date_end = uint64str(static_cast<uint64_t>(t1));

	}
	return 0;
}

int CImBase::getdays(int year, int month)
{
	switch (month) {
	case 12:
		return 31;
	case 11:
		return 30;
	case 10:
		return 31;
	case  9:
		return 30;
	case  8:
		return 31;
	case  7:
		return 31;
	case  6:
		return 30;
	case  5:
		return 31;
	case  4:
		return 30;
	case  3:
		return 31;
	case  2:
		if ((year % 400 == 0) || (year % 4 == 0) && (year % 100 != 0))
			return 29;
		else
			return 28;
	case  1:
		return 31;
	default:
		return 31;
	}
}

string CImBase::get_picurl(const QUERY_REQ req)
{
	string clickurl = "./picture.html?category=" + req.cat_id;
	if (req.favorites != "")
	{
		clickurl = "./picture.html?favorites=1";
	}
	else if (req.caddie != "")
	{
		clickurl = "./picture.html?caddie=1";
	}
	else if (req.most_visited != "")
	{
		clickurl = "./picture.html?most_visited=1";
	}
	else if (req.recent_pics != "")
	{
		clickurl = "./picture.html?recent_pics=1";
	}
	else if (req.no_tag != "")
	{
		clickurl = "./picture.html?no_tag=1";
	}
	else if (req.longitude != "")
	{
		clickurl = "./picture.html?longitude=" + req.longitude + "&latitude=" + req.latitude;
	}
	else if (req.list0 != "")
	{
		clickurl = "./picture.html?list=" + req.list0;
	}
	else if (req.tags != "")
	{
		clickurl = "./picture.html?tags=" + req.tags;
	}
	else if (req.osm != "")
	{
		clickurl = "./picture.html?osm=" + req.osm;
	}
	else if (req.q != "")
	{
		clickurl = "./picture.html?q=" + req.q;
	}
	else if (req.search != "")
	{
		clickurl = "./picture.html?search=" + req.search;
	}

	if (req.posted_list != "")
	{
		clickurl += "&posted_list=1";
	}
	else if (req.created_list != "")
	{
		clickurl += "&created_list=1";
	}
	if (req.recent_cats != "")
	{
		clickurl += "&recent_cats=1";
	}

	string cal = "";
	string week = "";
	lookitem(req.args, "cal=", cal);
	lookitem(req.args, "week=", week);
	if (cal != "")
	{
		clickurl += "&cal=" + cal;
	}
	if (week != "")
	{
		clickurl += "&week=" + week;
	}

	return clickurl;
}

string CImBase::get_up_url(const QUERY_REQ req)
{
	string clickurl = "";
	if (req.favorites != "")
	{
		clickurl = "./index.html?favorites=1";
	}
	else if (req.caddie != "")
	{
		clickurl = "./index.html?caddie=1";
	}
	else if (req.most_visited != "")
	{
		clickurl = "./index.html?most_visited=1";
	}
	else if (req.recent_pics != "")
	{
		clickurl = "./index.html?recent_pics=1";
	}
	else if (req.no_tag != "")
	{
		clickurl = "./index.html?no_tag=1";
	}
	else if (req.longitude != "")
	{
		clickurl = "./index.html?longitude=" + req.longitude + "&latitude=" + req.latitude;
	}
	else if (req.list0 != "")
	{
		clickurl = "./index.html?list=" + req.list0;
	}
	else if (req.tags != "")
	{
		clickurl = "./index.html?tags=" + req.tags;
	}
	else if (req.q != "")
	{
		clickurl = "./index.html?q=" + req.q;
	}
	else if (req.search != "")
	{
		clickurl = "./index.html?search=" + req.search;
	}
	else if (req.face != "")
	{
		clickurl = "./index.html?face=" + req.face;
	}

	if (req.cat_id != "")
	{
		if (clickurl == "")
		{
			clickurl = "./index.html?category=" + req.cat_id;
		}
		else
		{
			clickurl += "&category=" + req.cat_id;
		}
	}

	if (req.osm != "")
	{
		if (clickurl == "")
		{
			clickurl = "./index.html?osm=" + req.osm;
		}
		else
		{
			clickurl += "&osm=" + req.osm;
		}
	}
	if (req.posted_list != "")
	{
		if (clickurl == "")
		{
			clickurl = "./index.html?home=1&posted_list=1";
		}
		else
		{
			clickurl += "&posted_list=1";
		}
	}
	else if (req.created_list != "")
	{
		if (clickurl == "")
		{
			clickurl = "./index.html?home=1&created_list=1";
		}
		else
		{
			clickurl += "&created_list=1";
		}
	}

	if (req.recent_cats != "")
	{
		clickurl += "&recent_cats=1";
	}

	return clickurl;
}

long CImBase::get_imgsize(IMAGE_DTO dto, uint32_t maxWidth, uint32_t maxHeight, uint32_t& outW, uint32_t& outH)
{
	uint32_t width = (dto.iscoi == 1) ? dto.coi_w : dto.width;
	uint32_t height = (dto.iscoi == 1) ? dto.coi_h : dto.height;

	if (width < maxWidth && height < maxHeight) {
		outW = width;
		outH = height;
	}
	else
	{
		double f0 = (double)maxWidth / (double)maxHeight;
		double f1 = (double)width / (double)height;

		if (f0 <= f1)
		{
			outW = maxWidth;
			outH = (uint32_t)(maxWidth / f1);
		}
		else
		{
			outW = (uint32_t)(maxHeight * f1);
			outH = maxHeight;
		}
	}

	return 0;
}

long CImBase::get_flex_size(IMAGE_DTO dto, uint32_t maxWidth, uint32_t maxHeight, uint32_t& outW, uint32_t& outH)
{
	uint32_t width = (dto.iscoi == 1) ? dto.coi_w : dto.width;
	uint32_t height = (dto.iscoi == 1) ? dto.coi_h : dto.height;

	if (height < maxHeight) {

		outW = width;
		outH = height;
	}
	else
	{
		outW = maxHeight * width / height;
		outH = maxHeight;
	}
	if (dto.ext == "mp4" || dto.ext == "webm")
	{
		if (dto.representative_ext != "")
		{
			outW = 144;
			outH = 144;
		}
	}

	return 0;
}

string CImBase::get_time_string(uint64_t tt, string fmt)
{
	string month = "{'month%d'|@translate}";

	time_t current_time;
	time(&current_time);

	int64_t uuu = tt - current_time;
	uuu = uuu / 60;
	if (tt < 137000000 || uuu > 60 * 24 * 36500)
	{
		return "";
	}

	time_t t0 = tt;
	struct tm t;
	localtime_s(&t, &t0);//tt is gmt
	if (t.tm_year == -1 || t.tm_mon < 0 || t.tm_mon >= 12)
	{
		return "";
	}

	char stamped[MAX_PATH] = { 0 };

	if (fmt == "%c")
	{
		strftime(stamped, MAX_PATH, "%Y:%m:%d %H:%M:%S", &t);
		return stamped;
	}
	else if (fmt == "d")
	{
		strftime(stamped, MAX_PATH, "%Y:%m:%d", &t);
		return stamped;
	}

	string sLongDate = get_sLongDate();

	ReplaceOne(sLongDate, "yy", Int2Str(t.tm_year + 1900));
	ReplaceOne(sLongDate, "d", Int2Str(t.tm_mday));

	string lang = CLang::Instance()._language;
	if (lang.find("zh") != string::npos)
	{
		ReplaceOne(sLongDate, "m", Int2Str(t.tm_mon + 1));
	}
	else if (lang.find("ja") != string::npos)
	{
		ReplaceOne(sLongDate, "m", Int2Str(t.tm_mon + 1));
	}
	else if (lang.find("es") != string::npos)
	{
		sprintf_s(stamped, MAX_PATH, month.c_str(), t.tm_mon + 1);
		ReplaceOne(sLongDate, "MM", stamped);
	}
	else if (lang.find("fr") != string::npos)
	{
		sprintf_s(stamped, MAX_PATH, month.c_str(), t.tm_mon + 1);
		ReplaceOne(sLongDate, "MM", stamped);
	}
	else if (lang.find("ru") != string::npos)
	{
		sprintf_s(stamped, MAX_PATH, month.c_str(), t.tm_mon + 1);
		ReplaceOne(sLongDate, "MM", stamped);
	}
	else
	{
		sprintf_s(stamped, MAX_PATH, month.c_str(), t.tm_mon + 1);
		ReplaceOne(sLongDate, "MM", stamped);
	}

	return sLongDate;
}

string  CImBase::get_navigationBar(string url, int total, int curr, int per, int css)
{
	//if (CDataSrv::Instance()._ov == 1)
	//{
	//	return "";
	//}

	if (per == 0)
	{
		return "";
	}

	string navigationBar = "";
	if (total <= per)
	{
		return navigationBar;
	}

	int pages = 0;
	div_t div_result = div(total, per);
	if (div_result.rem != 0)
	{
		div_result.quot += 1;
	}
	pages = div_result.quot;

	if (curr <= 0) {
		curr = 1;
	}
	else if (curr > pages)
	{
		curr = pages;
	}

	CLang::Instance().getOneData("jquery_page", navigationBar);
	Replace(navigationBar, "{$pages}", Int2Str(pages));
	ReplaceOne(navigationBar, "{$URL}", url);
	ReplaceOne(navigationBar, "{$curr}", Int2Str(curr));
	ReplaceOne(navigationBar, "{$PERPAGE_TOTAL}", Int2Str(PERPAGE_TOTAL));

	if (css == 1)
	{
		ReplaceOne(navigationBar, "dark-theme", "light-theme");
	}

	return navigationBar;
}

string CImBase::get_photo_string(uint32_t nb)
{
	string str_photo = "";
	if (nb > 1)
	{
		CLang::Instance().getOneData("str_photos", str_photo);
	}
	else
	{
		CLang::Instance().getOneData("str_photo", str_photo);
	}
	translate(str_photo, uint2str(nb), "");

	return str_photo;
}

uint32_t CImBase::murmur3_32(const uint8_t* key, int len, uint32_t seed) {
	uint32_t h = seed;
	if (len > 3) {
		const uint32_t* key_x4 = (const uint32_t*)key;
		int i = len >> 2;
		do {
			uint32_t k = *key_x4++;
			k *= 0xcc9e2d51;
			k = (k << 15) | (k >> 17);
			k *= 0x1b873593;
			h ^= k;
			h = (h << 13) | (h >> 19);
			h = (h * 5) + 0xe6546b64;
		} while (--i);
		key = (const uint8_t*)key_x4;
	}
	if (len & 3) {
		int i = len & 3;
		uint32_t k = 0;
		key = &key[i - 1];
		do {
			k <<= 8;
			k |= *key--;
		} while (--i);
		k *= 0xcc9e2d51;
		k = (k << 15) | (k >> 17);
		k *= 0x1b873593;
		h ^= k;
	}
	h ^= len;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;

	return h;
}

string CImBase::get_dur_string(uint32_t duration)
{
	string dur = "";
	if (duration != 0)
	{
		ldiv_t div0 = ldiv(duration, 3600);
		int hour = div0.quot;
		int rem = div0.rem;

		ldiv_t div1 = ldiv(rem, 60);
		int mi = div1.quot;
		int sec = div1.rem;

		char buf[100] = { 0 };
		if (hour > 0)
		{
			sprintf_s(buf, 100, "%02d:%02d:%02d", hour, mi, sec);
		}
		else
		{
			sprintf_s(buf, 100, "%02d:%02d", mi, sec);
		}
		dur = buf;

	}
	return dur;
}

string CImBase::get_sLongDate()
{
	string sLongDate = "";
	CLang::Instance().getOneData("sLongDate", sLongDate);
	translate(sLongDate, "", "");

	string lang = CLang::Instance()._language;
	LCID lcd = GetUserDefaultUILanguage();

	if ((lang.find("en") != string::npos && lcd == 0x0409) || sLongDate == "")
	{
		sLongDate = "MM d, yy";
	}
	return sLongDate;
}

string CImBase::get_sYearMonth()
{
	string sYearMonth = "";
	CLang::Instance().getOneData("sYearMonth", sYearMonth);
	translate(sYearMonth, "", "");

	string lang = CLang::Instance()._language;
	LCID lcd = GetUserDefaultUILanguage();

	if ((lang.find("en") != string::npos && lcd == 0x0409) || sYearMonth == "")
	{
		sYearMonth = "MM, yy";
	}
	return sYearMonth;
}

BOOL CImBase::my_DeleteFile(wstring dst, int ty)
{
	wchar_t src[MAX_PATH] = { 0 };
	DWORD dwSize = GetFullPathName(dst.c_str(), MAX_PATH, src, NULL);

	SetFileAttributes(src, FILE_ATTRIBUTE_NORMAL);
	if (ty == 0)
	{
		DeleteFile(src);
	}
	else
	{
		SHFILEOPSTRUCT op = { 0 };
		op.wFunc = FO_DELETE;
		op.pTo = NULL;
		op.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION;
		op.pFrom = src;

		int ret = SHFileOperation(&op);
		if (ret != 0)
		{
			DWORD dw = GetLastError();
			char buf[100] = { 0 };
			sprintf_s(buf, 100, "%d", dw);
		}
	}
	return TRUE;

}

long CImBase::esc_string(string& name)
{
	Replace(name, "\"", "");
	//Replace(name, "'", "");
	//Replace(name, ":", "");
	//Replace(name, "/", "");
	//Replace(name, "\\", "");
	//Replace(name, "*", "");
	//Replace(name, "|", "");
	//Replace(name, "%", "");
	return 0;
}

string CImBase::ori2path(list<ROOT_DTO> lstRoot, int type, string ori)
{
	wstring sz = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);

	string szPath = _w2u(sz);
	string album = CDataSrv::Instance()._albumName;
	string dDir = (album == "private") ? "/data2" : "/data1";

	string strinpath = szPath + dDir;

	if (type == 0)
	{
		return strinpath + ori;
	}

	string dest = "";
	string src = ori;
	string::size_type pos = src.find("/galleries_");
	if (pos == string::npos)
	{
		dest = strinpath + src;
		ReplaceOne(dest, "/_thumb/", "/_data/");
	}
	else
	{
		list<ROOT_DTO>::iterator it2;
		for (it2 = lstRoot.begin(); it2 != lstRoot.end(); it2++)
		{
			string id8 = "/galleries_" + it2->id + "/";
			string::size_type pos0 = src.find(id8);
			if (pos0 != string::npos)
			{
				ReplaceOne(src, id8, "/");
				dest = it2->root_dir0 + src;
				break;
			}
		}
	}
	return dest;
}

string CImBase::get_angle(uint32_t rotation)
{
	string angle = "0";
	//if (iter->iscoi == 0)
	{
		if (rotation == 6)
		{
			angle = "90";
		}
		else if (rotation == 8)
		{
			angle = "-90";
		}
		else if (rotation == 3)
		{
			angle = "180";
		}
	}
	return angle;
}
