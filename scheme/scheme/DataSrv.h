#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include <signal.h>
#include <atomic>

//
//struct cmp
//{
//	bool operator()(string const a, string const b) const
//	{
//		if (a != b)
//		{
//			return a < b;
//		}
//		else
//		{
//			return a < b;
//		}
//	}
//};

class CDataSrv : public Singleton<CDataSrv>
{
	friend class Singleton<CDataSrv>;

protected:
	CDataSrv(void);
	~CDataSrv(void);

public:
	long clear_cache_tags();
	long clear_cache_image_ids();

	long refresh_favorites();
	long refresh_gallery_title();
	long clear_cache_slider();

	string get_image_url(int type, IMAGE_DTO dto);
	string get_menubar_tags(int type, string cat_ids, string pic_ids);

	list<TAG_DTO>* get_lstTag();
	long get_lstImageIds(list<string>& lstImageIds);

	long get_sliderValues(SLIDER_VALUES& sliderValues);
	long get_lstImageIds_map(list<string>& lstImageIds);
	long clear_all();
	long get_roots(list<ROOT_DTO>& lstRoot);

	long get_config_val(string id, string& val);
	long set_config_val(string id, string val);
	long get_play_htm(string id, string& tpl);
	long select_gallery(string albumName, string viewer, string act, string& syn);
	void WritePrivate(string key, const char* val);
	void ReadPrivate(string key, string& val);
	void get_language(map<string, string>& lcid, map<string, string>& lang);
	void get_windir(wstring& windir);
	void get_currdir(wstring& szPath, wstring& currdir);
	void get_films(list<FILM_DTO>& lstfilm);
	void proc_public_acc();
	string get_username();

private:
	long init_order();

public:
	SEARCH_REQ _search_req;
	QUERY_REQ _q_req;
	list<FAVORITE_DTO> _lstFavorite;
	list<ORDER_DTO> _lstOrder;
	list<string> _lstosm;

public:
	string _albumName = "";
	string _gallery_title = "";
	string _login = "admin";
	string _metadata = "nometa";
	string _vrsize = "3";
	string _last_posted = "";
	string _last_posted2 = "";
	string _code = "";
	string _mp3_url = "./templ/themes/default/images/mp3.png";
	string _mp4_url = "./templ/themes/default/images/mp4.png";
	string _gif_url = "./templ/themes/default/images/gif.png";
	string _orderby = "";
	string _str_readonly = "";
	string _username = "";
	string _skin = "newspaper";
	string _bg = "";
	string _bgtype = "0";
	string _star = "0";
	string _display_thumb = "thumb";

	string _acc = "0";
	string _ov = "0";

	time_t _first = 0;
	time_t _t_masrony_title = 0;
	uint32_t _readonly = 0;
	uint32_t _maxFileSize = 200;

	long _time_zone_bias = 0;
	int _db_error = 0;
	int _day = 0;
	int _ischanged = 0;
	atomic_int _uinit = 2;
	HANDLE _shutdownEvent = NULL;

	wstring _appDataDir = L"";

	int msix = 0;
private:
	SLIDER_VALUES _sliderValues;
	list<TAG_DTO> _lstTag;
	list<string> _lstImageIds;

	CRITICAL_SECTION _cs;
	int _hasCache = 0;

};