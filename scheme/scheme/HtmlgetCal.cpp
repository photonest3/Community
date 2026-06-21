////////////////////////////////////////////////////////////////////
/// @file HtmlgetCal.cpp
/// @brief 日历视图请求处理实现文件
/// @details 此文件实现了 CHtmlgetCal 类的所有方法，
///          包括日历视图 HTML 生成、按日期浏览图片、
///          日历缩略图显示等功能。
///          是日历视图和按日期浏览的核心处理逻辑。
/// @note 支持按年、月、日浏览图片
/// @author PhotoNest Team
/// @date 2024
////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "unitil.h"
#include "../../templ/templ/resource.h"
#include "lang.h"
#include "imagedal.h"
#include "DataSrv.h"
#include "HtmlgetCal.h"

CHtmlgetCal::CHtmlgetCal(void)
{
	_outsql = "";
}

CHtmlgetCal::~CHtmlgetCal(void)
{
}

long CHtmlgetCal::get_cal_thumbnails(bool bfirst, QUERY_REQ req, SEARCH_REQ search_req, string &calendarBar_html, uint32_t &nb, string &calendarTitle)
{
	string cal = "";
	lookitem(req.args, "cal=", cal);

	uint32_t tm_year = 0;
	uint32_t tm_mon = 0;
	uint32_t tm_mday = 0;

	if (bfirst)
	{
		string _last_posted = CDataSrv::Instance()._last_posted2;
		if (req.posted_list != "")
		{
			_last_posted = CDataSrv::Instance()._last_posted;
		}
		if (req.posted != _last_posted && _last_posted != "")
		{
			set_req(_last_posted, req);
		}
	}

	if (req.posted != "")
	{
		vector<string> v1 = tokenize(req.posted, "-", true, "");
		int len = (int)v1.size();
		for (int i = 0; i < len; i++)
		{
			if (i == 0)
			{
				tm_year = atoi(v1[0].c_str());
			}
			if (i == 1)
			{
				tm_mon = atoi(v1[1].c_str());
			}
			if (i == 2)
			{
				tm_mday = atoi(v1[2].c_str());
			}
		}
	}

	string type = "";
	if (req.posted_list != "")
	{
		type = "posted";
	}
	else if (req.created_list != "")
	{
		type = "created";
	}

	QUERY_REQ reqTemp = req;
	reqTemp.date_start = "0";
	reqTemp.date_end = "0";

	string sql = "";
	Cimagedal::Instance().get_image_cal_sql(reqTemp, search_req, sql);
	if (_outsql != sql)
	{
		_outsql = sql;
		_mapCal.clear();
		Cimagedal::Instance().get_image_cal(sql, _mapCal);
	}

	if (_mapCal.empty())
	{
		return 1;
	}

	map<uint32_t, uint32_t >::iterator it;
	list<CAL_DTO>::iterator iter;
	bool isin = false;

	if (cal == "weekly" || cal == "ylist")
	{
		list<CAL_DTO > lstCalYear;
		for (it = _mapCal.begin(); it != _mapCal.end(); it++)
		{
			time_t t0 = it->first; // date_available_day, date_creation_day is local
			struct tm t;
			gmtime_s(&t, &t0);

			bool b = false;
			for (iter = lstCalYear.begin(); iter != lstCalYear.end(); iter++)
			{
				if (iter->year == t.tm_year + 1900)
				{
					iter->nb_year_images += it->second;
					b = true;
				}
			}
			if (!b)
			{
				CAL_DTO dto;
				dto.year = t.tm_year + 1900;
				dto.month = t.tm_mon + 1;
				dto.day = t.tm_mday;
				dto.nb_month_images = it->second;
				dto.nb_day_images = it->second;
				dto.nb_year_images = it->second;

				lstCalYear.push_back(dto);

				if (dto.year == tm_year)
				{
					isin = true;
				}
			}
		}

		if (!isin)
		{
			CAL_DTO dto = lstCalYear.back();
			tm_year = dto.year;
			nb = dto.nb_year_images;
		}
		else
		{
			for (iter = lstCalYear.begin(); iter != lstCalYear.end(); iter++)
			{
				if (iter->year == tm_year)
				{
					nb = iter->nb_year_images;
					break;
				}
			}
		}

		if (bfirst)
		{
			string timel = "";
			string timel_li = "";
			string timel_all = "";
			CLang::Instance().getOneData("timel", timel);
			CLang::Instance().getOneData("timel_li", timel_li);

			int mon = 0;
			for (iter = lstCalYear.begin(); iter != lstCalYear.end(); iter++)
			{
				string selected = "";
				if (tm_year == iter->year)
				{
					selected = "class=\"selected\"";
				}

				if (iter->year != mon)
				{
					string tem = timel_li;
					mon = iter->year;
					string url = get_cal_url(req, type, cal, uint2str(iter->year));

					if (selected != "" && req.start != "")
					{
						url += "&start=" + req.start;
					}

					ReplaceOne(tem, "{$url}", url);
					ReplaceOne(tem, "{$class}", selected);

					ReplaceOne(tem, "{$year}", uint2str(iter->year));
					ReplaceOne(tem, "{$nb}", uint2str(iter->nb_year_images));
					timel_all += tem;
				}
			}

			ReplaceOne(timel, "{$timel_li}", timel_all);
			calendarBar_html = timel;
		}
	}
	else
	{
		list<CAL_DTO > lstCalMon;
		for (it = _mapCal.begin(); it != _mapCal.end(); it++)
		{
			time_t t0 = it->first; // date_available_day, date_creation_day is local
			struct tm t;
			gmtime_s(&t, &t0);

			bool b = false;
			for (iter = lstCalMon.begin(); iter != lstCalMon.end(); iter++)
			{
				if (iter->year == t.tm_year + 1900 && iter->month == t.tm_mon + 1)
				{
					iter->nb_month_images += it->second;
					b = true;
				}
			}
			if (!b)
			{
				CAL_DTO dto;
				dto.year = t.tm_year + 1900;
				dto.month = t.tm_mon + 1;
				dto.day = t.tm_mday;
				dto.nb_month_images = it->second;
				dto.nb_day_images = it->second;
				dto.nb_year_images = it->second;

				lstCalMon.push_back(dto);

				if (dto.year == tm_year)
				{
					isin = true;
				}
			}
		}

		if (!isin)
		{
			CAL_DTO dto = lstCalMon.back();
			tm_year = dto.year;
			tm_mon = dto.month;
			nb = dto.nb_month_images;
		}
		else if (tm_mday == 0)
		{
			for (iter = lstCalMon.begin(); iter != lstCalMon.end(); iter++)
			{
				if (iter->year == tm_year && iter->month == tm_mon)
				{
					nb = iter->nb_month_images;
					break;
				}
			}
		}
		else
		{
			for (it = _mapCal.begin(); it != _mapCal.end(); it++)
			{
				time_t t0 = it->first; // date_available_day, date_creation_day is local
				struct tm t;
				gmtime_s(&t, &t0);

				if (t.tm_year + 1900 == tm_year && t.tm_mon + 1 == tm_mon && t.tm_mday == tm_mday)
				{
					nb = it->second;
					break;
				}
			}
		}

		if (bfirst)
		{
			string timel = "";
			string timel_li = "";
			string timel_all = "";
			CLang::Instance().getOneData("timel", timel);
			CLang::Instance().getOneData("timel_li2", timel_li);

			bool b = false;
			int temp_year = 0;
			for (iter = lstCalMon.begin(); iter != lstCalMon.end(); iter++)
			{
				string selected = "";
				if (!b)
				{
					if (iter->year == tm_year)
					{
						if (tm_mon != 0)
						{
							if (tm_mon == iter->month)
							{
								selected = "class=\"selected\"";
								b = true;
							}
						}
						else
						{
							selected = "class=\"selected\"";
							b = true;
						}
					}
				}

				string tem = timel_li;
				string url = get_cal_url(req, type, cal, uint2str(iter->year) + "-" + uint2str(iter->month));

				if (selected != "" && req.start != "")
				{
					url += "&start=" + req.start;
				}

				ReplaceOne(tem, "{$url}", url);
				ReplaceOne(tem, "{$class}", selected);

				string mon = "{'month" + uint2str(iter->month) + "'|@translate} ";

				if (temp_year != iter->year)
				{
					temp_year = iter->year;
					ReplaceOne(tem, "{$year}", uint2str(iter->year) + "<br/>" + mon);
				}
				else
				{
					ReplaceOne(tem, "{$year}", "&nbsp;<br/>" + mon);
				}
				ReplaceOne(tem, "{$nb}", uint2str(iter->nb_month_images));
				timel_all += tem;
			}

			ReplaceOne(timel, "{$timel_li}", timel_all);
			calendarBar_html = timel;
		}
	}

	calendarTitle = get_calendar_title(req, cal, uint2str(tm_year), uint2str(tm_mon), uint2str(tm_mday));
	translate(calendarTitle, "", "");

	return 0;
}

string CHtmlgetCal::get_cal_url(QUERY_REQ req, string type, string cal, string year)
{
	string url = get_up_url(req);
	if (cal != "")
	{
		url += "&cal=" + cal;
	}
	if (year != "")
	{
		url += "&posted=" + year;
	}
	return url;
}

string CHtmlgetCal::get_calendarViews(QUERY_REQ req, string cal, string year)
{
	string type = req.posted_list != "" ? "posted" : "created";
	string url = get_cal_url(req, type, "", year);

	string calendarViews = "";
	CLang::Instance().getOneData("calendarViews", calendarViews);
	Replace(calendarViews, "{$url}", url);

	if (cal == "calendar")
	{
		ReplaceOne(calendarViews, "g='mlist'", "style=\"visibility:hidden\"");
		ReplaceOne(calendarViews, "g='calendar'", "");
		ReplaceOne(calendarViews, "g='weekly'", "style=\"visibility:hidden\"");
		ReplaceOne(calendarViews, "g='ylist'", "style=\"visibility:hidden\"");
		ReplaceOne(calendarViews, "{$title}", "{'Monthly calendar'|@translate}");
		ReplaceOne(calendarViews, "switchboxcalendar", "switchbox1");
	}
	else if (cal == "weekly")
	{
		ReplaceOne(calendarViews, "g='mlist'", "style=\"visibility:hidden\"");
		ReplaceOne(calendarViews, "g='calendar'", "style=\"visibility:hidden\"");
		ReplaceOne(calendarViews, "g='weekly'", "");
		ReplaceOne(calendarViews, "g='ylist'", "style=\"visibility:hidden\"");
		ReplaceOne(calendarViews, "{$title}", "{'Weekly list'|@translate}");
		ReplaceOne(calendarViews, "switchboxweekly", "switchbox1");
	}
	else if (cal == "mlist")
	{
		ReplaceOne(calendarViews, "g='mlist'", "");
		ReplaceOne(calendarViews, "g='calendar'", "style=\"visibility:hidden\"");
		ReplaceOne(calendarViews, "g='weekly'", "style=\"visibility:hidden\"");
		ReplaceOne(calendarViews, "g='ylist'", "style=\"visibility:hidden\"");
		ReplaceOne(calendarViews, "{$title}", "{'Monthly list'|@translate}");
		ReplaceOne(calendarViews, "switchboxmlist", "switchbox1");
	}
	else
	{
		ReplaceOne(calendarViews, "g='mlist'", "style=\"visibility:hidden\"");
		ReplaceOne(calendarViews, "g='calendar'", "style=\"visibility:hidden\"");
		ReplaceOne(calendarViews, "g='weekly'", "style=\"visibility:hidden\"");
		ReplaceOne(calendarViews, "g='ylist'", "");
		ReplaceOne(calendarViews, "{$title}", "{'All'|@translate}");
		ReplaceOne(calendarViews, "switchboxylist", "switchbox1");
	}

	return calendarViews;
}

string  CHtmlgetCal::get_cal_string(QUERY_REQ req, SEARCH_REQ search_req)
{
	int week_starts_on = 6;
	string calMonth_thead = "";
	get_calMonth_thead(week_starts_on, calMonth_thead);

	string startweek = "";

	string tab = "<table class=\"calMonth\">";
	tab += calMonth_thead;
	string tr = get_cal(req, search_req, week_starts_on);
	tab += tr;
	tab += "</table>";
	return tab;
}

SYSTEMTIME CHtmlgetCal::time_t_to_systemtime(time_t t)
{
	tm temptm;
	gmtime_s(&temptm, &t); // t is local

	SYSTEMTIME st = { static_cast<WORD>(1900 + temptm.tm_year),
		static_cast<WORD>(1 + temptm.tm_mon),
		static_cast<WORD>(temptm.tm_wday),
		static_cast<WORD>(temptm.tm_mday),
		static_cast<WORD>(temptm.tm_hour),
		static_cast<WORD>(temptm.tm_min),
		static_cast<WORD>(temptm.tm_sec),
		0 };
	return st;
}

string CHtmlgetCal::get_cal(QUERY_REQ req, SEARCH_REQ search_req, int week_starts_on)
{
	string type = "";
	if (req.posted_list != "")
	{
		type = "posted";
	}
	else if (req.created_list != "")
	{
		type = "created";
	}

	struct tm t_last; // is local
	t_last.tm_year = 0;
	t_last.tm_mon = 0;

	t_last.tm_mday = 1;

	t_last.tm_hour = 0;
	t_last.tm_min = 0;
	t_last.tm_sec = 0;
	t_last.tm_isdst = 0;

	vector<string> v1 = tokenize(req.posted, "-", true, "");
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

	time_t tt = _mkgmtime(&t_last);

	list<DAY_IMAGES_DTO> lstDay;
	list<DAY_IMAGES_DTO>::iterator iter;

	QUERY_REQ reqTemp = req;
	reqTemp.date_start = "";
	reqTemp.date_end = "";

	Cimagedal::Instance().get_cal_images(reqTemp, search_req, lstDay);

	list<IMAGE_DTO> lstImage;
	list<IMAGE_DTO>::iterator I;
	{
		string ids = "";
		for (iter = lstDay.begin(); iter != lstDay.end(); iter++)
		{
			ids += iter->image_id + ",";
		}
		rtrim(ids, ",");

		Cimagedal::Instance().get_images_by_ids(ids, lstImage);
	}

	map<uint64_t, IMAGE_DTO> mapDay;
	for (iter = lstDay.begin(); iter != lstDay.end(); iter++)
	{
		IMAGE_DTO dto;
		for (I = lstImage.begin(); I != lstImage.end(); I++)
		{
			if (iter->image_id == I->id)
			{
				dto = *I;
				dto.hit = iter->nb_images;
				break;
			}
		}

		mapDay.insert(map<uint64_t, IMAGE_DTO>::value_type(iter->day2, dto));
	}

	int i, j;
	int time_table[6][7];

	SYSTEMTIME sys = time_t_to_systemtime(tt);
	int days = getdays(sys.wYear, sys.wMonth);
	int Pdays = getdays(sys.wYear, sys.wMonth - 1);

	int day_count = 1;
	int FirstDayWeekAsMonth = sys.wDayOfWeek - (sys.wDay - 1) % 7;
	if (FirstDayWeekAsMonth < 1)
		FirstDayWeekAsMonth += 7;

	string tem = "";
	CLang::Instance().getOneData("calBackDate", tem);

	string cal = "<tr>";

	int needday = FirstDayWeekAsMonth;
	if (week_starts_on != 6)
	{
		needday = FirstDayWeekAsMonth - 1;
	}

	if (needday == 7)
	{
		needday = 0;
	}

	int nb = 6;
	if (needday == 0)
	{
		nb = 5;
	}
	bool b = false;

	for (i = 0; i < nb; i++)
	{
		for (j = 0; j < 7; j++)
		{
			if (needday-- > 0)
			{
				time_table[i][j] = Pdays - needday;

				cal += "<td ></td>";

				if (j == 6)
				{
					cal += "</tr><tr>";
				}
			}
			else
			{
				time_table[i][j] = day_count++;

				if (day_count > days + 1)
				{
					if (j == 0)
					{
						b = true;
					}
					if (!b)
					{
						cal += "<td></td>";
					}
				}
				else
				{
					struct tm curr;
					curr.tm_year = sys.wYear - 1900;
					curr.tm_mon = sys.wMonth - 1;
					curr.tm_mday = time_table[i][j];
					curr.tm_hour = 0;
					curr.tm_min = 0;
					curr.tm_sec = 0;
					curr.tm_isdst = 0;

					time_t t0 = _mkgmtime(&curr);

					char buf[1024] = { 0 };

					uint32_t nb_images = 0;
					IMAGE_DTO image_dto;
					HRESULT hr = S_FALSE;

					map<uint64_t, IMAGE_DTO>::iterator I = mapDay.find(static_cast<uint64_t>(t0));
					if (I != mapDay.end())
					{
						nb_images = I->second.hit;
						image_dto = I->second;
						hr = S_OK;
					}

					if (hr == S_OK)
					{
						string td = "<td class=\"calDayCellFull\">{$calBackDate}</td>";

						string calBackDate = tem;

						ReplaceOne(calBackDate, "{$date}", Int2Str(time_table[i][j]));
						ReplaceOne(calBackDate, "{$date}", Int2Str(time_table[i][j]));

						string url0 = CDataSrv::Instance().get_image_url(0, image_dto);
						ReplaceOne(calBackDate, "{$src}", url0);
						ReplaceOne(calBackDate, "{$name}", image_dto.file);
						ReplaceOne(calBackDate, "{$photo}", get_photo_string(nb_images));

						string url = get_cal_url(req, type, "calendar", Int2Str(sys.wYear) + "-" + Int2Str(sys.wMonth) + "-" + Int2Str(time_table[i][j]));
						ReplaceOne(calBackDate, "{$url}", url + "&mod=0");
						ReplaceOne(td, "{$calBackDate}", calBackDate);

						cal += td;
					}
					else
					{
						sprintf_s(buf, 1024, "<td class=\"calDayCellEmpty\">%d</td>", time_table[i][j]);
						cal += buf;
					}
				}
				if (j == 6) {
					cal += "</tr><tr>";
				}
			}
		}
	}

	if (cal.size() > 10)
	{
		cal = cal.substr(0, cal.size() - 4);
	}

	return cal;
}

string CHtmlgetCal::get_calendar_title(QUERY_REQ req, string cal, string tm_year, string tm_mon, string tm_mday)
{
	string calendarTitle = "";
	if (req.posted_list != "")
	{
		CLang::Instance().getOneData("calendarTitlePosted", calendarTitle);
	}
	else
	{
		CLang::Instance().getOneData("calendarTitleCreation", calendarTitle);
	}

	if (cal == "weekly")
	{
		string title = tm_year;

		string week = "";
		lookitem(req.args, "week=", week);
		if (week != "")
		{
			title = +"{'day" + week + "'|@translate} " + tm_year;
		}

		ReplaceOne(calendarTitle, "{$title}", title);

	}
	else if (cal == "ylist")
	{
		string title = " " + tm_year;
		ReplaceOne(calendarTitle, "{$title}", title);
	}
	else if (cal == "mlist")
	{
		string sYearMonth = get_sYearMonth();
		string mm = "{'month" + tm_mon + "'|@translate}";
		ReplaceOne(sYearMonth, "yy", tm_year);
		ReplaceOne(sYearMonth, "m", tm_mon);
		ReplaceOne(sYearMonth, "MM", mm);

		ReplaceOne(calendarTitle, "{$title}", sYearMonth);
	}
	else if (cal == "calendar")
	{
		string title = "";
		if (tm_mday != "0")
		{
			string sLongDate = get_sLongDate();
			string mm = "{'month" + tm_mon + "'|@translate}";
			ReplaceOne(sLongDate, "yy", tm_year);
			ReplaceOne(sLongDate, "d", tm_mday);
			ReplaceOne(sLongDate, "m", tm_mon);
			ReplaceOne(sLongDate, "MM", mm);
			title = sLongDate;
		}
		else
		{
			string sYearMonth = get_sYearMonth();
			string mm = "{'month" + tm_mon + "'|@translate}";
			ReplaceOne(sYearMonth, "yy", tm_year);
			ReplaceOne(sYearMonth, "m", tm_mon);
			ReplaceOne(sYearMonth, "MM", mm);
			title = sYearMonth;
		}

		ReplaceOne(calendarTitle, "{$title}", title);
	}
	return calendarTitle;
}

string  CHtmlgetCal::get_weekly_string(QUERY_REQ req, SEARCH_REQ search_req, string type, string year)
{
	int week_starts_on = 6;
	string calMonth_thead = "";
	get_calMonth_thead(week_starts_on, calMonth_thead);

	string tab = "<br/><br/><table class=\"calMonth\">";
	tab += calMonth_thead;
	string tr = get_weekly(req, search_req, type, week_starts_on, year);

	tab += tr;
	tab += "</table>";
	return tab;
}

string CHtmlgetCal::get_weekly(QUERY_REQ req, SEARCH_REQ search_req, string type, int week_starts_on, string year)
{
	//%w day of week 0-6 with sunday==0 
	list<WEEK_IMAGES_DTO> lstDay;
	list<WEEK_IMAGES_DTO>::iterator iter;
	Cimagedal::Instance().get_weekly_images(req, search_req, lstDay);

	list<IMAGE_DTO> lstImage;
	list<IMAGE_DTO>::iterator I;
	{
		string ids = "";
		for (iter = lstDay.begin(); iter != lstDay.end(); iter++)
		{
			ids += iter->image_id + ",";
		}
		rtrim(ids, ",");

		Cimagedal::Instance().get_images_by_ids(ids, lstImage);
	}

	map<uint64_t, IMAGE_DTO> mapDay;
	for (iter = lstDay.begin(); iter != lstDay.end(); iter++)
	{
		IMAGE_DTO dto;
		for (I = lstImage.begin(); I != lstImage.end(); I++)
		{
			if (iter->image_id == I->id)
			{
				dto = *I;
				dto.hit = iter->nb_images;
				break;
			}
		}

		mapDay.insert(map<uint64_t, IMAGE_DTO>::value_type(iter->day2, dto));
	}

	string tem = "";
	CLang::Instance().getOneData("calBackDate", tem);

	string cal = "<tr>";
	for (int j = 0; j < 7; j++)
	{
		char buf[1024] = { 0 };

		uint32_t nb_images = 0;
		IMAGE_DTO image_dto;
		HRESULT hr = S_FALSE;

		int k = j;
		if (week_starts_on != 6)
		{
			if (j == 6)
			{
				k = 0;
			}
			else
			{
				k = j + 1;
			}
		}

		map<uint64_t, IMAGE_DTO>::iterator I = mapDay.find(k);
		if (I != mapDay.end())
		{
			nb_images = I->second.hit;
			image_dto = I->second;
			hr = S_OK;
		}

		if (hr == S_OK)
		{
			string td = "<td class=\"calDayCellFull\">{$calBackDate}</td>";
			string calBackDate = tem;
			ReplaceOne(calBackDate, "{$date}", "");
			ReplaceOne(calBackDate, "{$date}", "");

			string url0 = CDataSrv::Instance().get_image_url(0, image_dto);
			ReplaceOne(calBackDate, "{$src}", url0);
			ReplaceOne(calBackDate, "{$name}", image_dto.file);
			ReplaceOne(calBackDate, "{$photo}", get_photo_string(nb_images));

			string url = get_up_url(req);
			url += "&cal=weekly&posted=" + year + "&week=" + Int2Str(k);

			ReplaceOne(calBackDate, "{$url}", url);
			ReplaceOne(td, "{$calBackDate}", calBackDate);

			cal += td;
		}
		else
		{
			sprintf_s(buf, 1024, "<td class=\"calDayCellEmpty\">%s</td>", "");
			cal += buf;
		}

	}
	cal += "</tr>";

	return cal;
}

long CHtmlgetCal::get_calMonth_thead(int &week_starts_on, string &calMonth_thead)
{
	//first day of week specifier, 0 - 6, 0 = Monday, 6 = Sunday
	week_starts_on = 6;
	calMonth_thead = "";

	wchar_t buf[MAX_PATH] = { 0 };
	int ret = GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IFIRSTDAYOFWEEK, buf, MAX_PATH);
	if (ret > 0)
	{
		week_starts_on = _wtoi(buf);
	}

	if (week_starts_on == 6)
	{
		CLang::Instance().getOneData("calMonth_thead7", calMonth_thead);
	}
	else
	{
		CLang::Instance().getOneData("calMonth_thead1", calMonth_thead);
	}
	return 0;
}