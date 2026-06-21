#pragma once
#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

class CHtmlgetCal : public CImBase, public Singleton<CHtmlgetCal>
{
	friend class Singleton<CHtmlgetCal>;
protected:
	CHtmlgetCal(void);
	~CHtmlgetCal(void);

public:
	string get_calendarViews(QUERY_REQ req, string cal, string year);
	string get_cal_string(QUERY_REQ req, SEARCH_REQ search_req);
	string get_cal_url(QUERY_REQ req, string type, string cal, string year);
	string get_calendar_title(QUERY_REQ req, string cal, string tm_year, string tm_mon, string tm_mday);
	string get_weekly_string(QUERY_REQ req, SEARCH_REQ search_req, string type, string year);
	long get_cal_thumbnails(bool bfirst, QUERY_REQ req, SEARCH_REQ search_req, string &calendarBar_html, uint32_t &nb, string &calendarTitle);
	string _outsql = "";

private:
	SYSTEMTIME time_t_to_systemtime(time_t t);
	string get_cal(QUERY_REQ req, SEARCH_REQ search_req, int week_starts_on);
	string get_weekly(QUERY_REQ req, SEARCH_REQ search_req, string type, int week_starts_on, string year);
	long get_calMonth_thead(int &week_starts_on, string &calMonth_thead);

private:
	map<uint32_t, uint32_t > _mapCal;
};
