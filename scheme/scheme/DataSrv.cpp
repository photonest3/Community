////////////////////////////////////////////////////////////////////
/// @file DataSrv.cpp
/// @brief 数据服务层实现文件
/// @details 此文件实现了 CDataSrv 类的所有方法，
///          包括缓存管理、配置管理、数据查询等功能。
///          作为数据访问层和服务层之间的桥梁，
///          提供统一的数据服务接口。
/// @note 使用单例模式，通过 CriticalSection 保证线程安全
/// @author PhotoNest Team
/// @date 2024
////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "unitil.h"
#include <vector>
#include <string>
#include <shlobj.h>
#include "SimpleIni.h"

#include "../../templ/templ/resource.h"
#include "../../templ/templ/templ.h"

#include "DataSrv.h"
#include "basedal.h"
#include "chatdal.h"
#include "imagedal.h"
#include "lang.h"
#include "imlog.h"
#include "MySoft.h"
#include "writedb.h"

////////////////////////////////////////////////////////////////////
/// @brief 构造函数
/// @details 初始化所有成员变量为默认值，
///          包括相册名称、图库标题、登录用户、
///          配置参数、缓存状态等。
/// @note 使用 InitializeCriticalSection 初始化临界区，保证线程安全
////////////////////////////////////////////////////////////////////
CDataSrv::CDataSrv(void)
{
	_albumName = "";
	_gallery_title = "";

	_t_masrony_title = 0;

	_ov = "0";
	_day = 0;

	_code = "";
	_mp3_url = "./templ/themes/default/images/mp3.png";
	_mp4_url = "./templ/themes/default/images/mp4.png";
	_orderby = "";

	_display_thumb = "thumb";
	_metadata = "nometa";
	_vrsize = "3";

	_time_zone_bias = 0;
	_readonly = 0;
	_str_readonly = "";

	_maxFileSize = 200;
	_last_posted = "";
	_last_posted2 = "";

	_db_error = 0;
	_acc = "0";
	_ischanged = 0;

	InitializeCriticalSection(&_cs);
}

////////////////////////////////////////////////////////////////////
/// @brief 析构函数
/// @details 释放临界区资源
////////////////////////////////////////////////////////////////////
CDataSrv::~CDataSrv(void)
{
	DeleteCriticalSection(&_cs);
}

////////////////////////////////////////////////////////////////////
/// @brief 清空标签缓存
/// @return S_OK: 成功
/// @note 清空 _lstTag 列表，强制下次重新加载
////////////////////////////////////////////////////////////////////
long CDataSrv::clear_cache_tags()
{
	_lstTag.clear();

	return S_OK;
}

////////////////////////////////////////////////////////////////////
/// @brief 清空图片 ID 缓存
/// @return S_OK: 成功
/// @note 使用临界区保护 _hasCache 变量
////////////////////////////////////////////////////////////////////
long CDataSrv::clear_cache_image_ids()
{
	EnterCriticalSection(&_cs);
	_hasCache = 0;
	LeaveCriticalSection(&_cs);

	return S_OK;
}

////////////////////////////////////////////////////////////////////
/// @brief 刷新收藏列表
/// @return S_OK: 成功
/// @note 清空缓存并从数据库重新加载当前用户的收藏列表
////////////////////////////////////////////////////////////////////
long CDataSrv::refresh_favorites()
{
	_lstFavorite.clear();
	Cchatdal::Instance().get_favorites(_login, _lstFavorite);

	return S_OK;
}

////////////////////////////////////////////////////////////////////
/// @brief 清空滑块过滤器缓存
/// @return S_OK: 成功
/// @note 清空所有滑块相关的值列表（宽度、高度、文件大小等）
////////////////////////////////////////////////////////////////////
long CDataSrv::clear_cache_slider()
{
	EnterCriticalSection(&_cs);
	_sliderValues.widths.clear();
	_sliderValues.heights.clear();
	_sliderValues.filesizes.clear();
	_sliderValues.createdates.clear();
	_sliderValues.postdates.clear();
	_sliderValues.ratios.clear();

	LeaveCriticalSection(&_cs);

	return S_OK;
}

////////////////////////////////////////////////////////////////////
/// @brief 获取滑块过滤器的值列表
/// @param[out] sliderValues 输出参数，滑块过滤器的值列表
/// @return S_OK: 成功; 其他: 错误码
/// @note 使用缓存机制，如果已加载则直接返回缓存数据
////////////////////////////////////////////////////////////////////
long CDataSrv::get_sliderValues(SLIDER_VALUES& sliderValues)
{
	EnterCriticalSection(&_cs);
	if (_sliderValues.createdates.empty())
	{
		Cimagedal::Instance().get_images_slider_values(_sliderValues);
	}
	LeaveCriticalSection(&_cs);
	sliderValues = _sliderValues;

	return S_OK;
}

////////////////////////////////////////////////////////////////////
/// @brief 刷新图库标题和配置
/// @return S_OK: 成功; 其他: 错误码
/// @details 从数据库加载图库配置参数，包括标题、显示模式、
///          时区偏移、最大文件大小、皮肤、背景等。
/// @note 此函数在初始化和配置更改时调用
////////////////////////////////////////////////////////////////////
long CDataSrv::refresh_gallery_title()
{
	_gallery_title = "";
	_display_thumb = "thumb";
	_maxFileSize = 200;
	_time_zone_bias = 0;
	_code = "";
	_ov = "0";
	_first = 0;

	string sel_bg = "";
	string custom_bg = "";

	string ids = "'gallery_title','display','time_zone_bias','maxFileSize', 'metadata','fullsize','skin','sel_bg','custom_bg', 'bgtype'";
	list<CONFIG_DTO> lstConfig;
	Cchatdal::Instance().get_configs(ids, lstConfig);

	list<CONFIG_DTO>::iterator iter;
	for (iter = lstConfig.begin(); iter != lstConfig.end(); iter++)
	{
		if (iter->param == "gallery_title")
		{
			_gallery_title = iter->value;
		}
		else if (iter->param == "display")
		{
			_display_thumb = iter->value;
		}
		else if (iter->param == "maxFileSize")
		{
			_maxFileSize = get_uint32(iter->value.c_str());
		}
		else if (iter->param == "metadata")
		{
			_metadata = iter->value;
		}
		else if (iter->param == "vrSize")
		{
			_vrsize = iter->value;
		}
		else if (iter->param == "time_zone_bias")
		{
			_time_zone_bias = atol(iter->value.c_str());
		}
		else if (iter->param == "skin")
		{
			_skin = iter->value;
		}
		else if (iter->param == "sel_bg")
		{
			sel_bg = iter->value;
		}
		else if (iter->param == "bgtype")
		{
			_bgtype = iter->value;
		}
		else if (iter->param == "custom_bg")
		{
			custom_bg = iter->value;
		}
	}

	if (sel_bg == "-1")
	{
		_bg = "";
	}
	else if (sel_bg == "0")
	{
		_bg = "./background/default.jpg";

	}
	else
	{
		if (custom_bg != "")
		{
			list<IMAGE_DTO> lstImage;
			Cimagedal::Instance().get_images_by_ids(custom_bg, lstImage);
			if (!lstImage.empty())
			{
				_bg = get_image_url(2, lstImage.front());
			}
		}
	}

	if (_gallery_title == "")
	{
		_gallery_title = "{'My Gallery'|@translate}";
		if (_albumName == "private")
		{
			//_gallery_title = "{'Encrypted Photos'|@translate}";
		}
		CLang::Instance().translate(_gallery_title, "", "");
	}

	if (_albumName == "private")
	{
		int mode = MODE_GALLERY;

		string v3 = "";

		string szSubKey = "Software\\OpenCV\\HighGUI\\Windows\\ZK2i3";
		if (msix == 0)
		{
			string v31 = "";
			CMySoft::Instance().get_lic(szSubKey, v31);
			v3 = v31;
		}

		if (v3 == "")
		{
			string v30 = "";
			get_config_val("wo3", v30);
			v3 = v30;
		}

		time_t current_time;
		time(&current_time);

		lic_header lic;
		int ret = CMySoft::Instance().Verify(v3, mode, lic);
		if (ret == 0)
		{
			_code = v3;
			_ov = "2";
			_first = lic.date;

			int day = abs(static_cast<int>(current_time - lic.date));

			if (lic.year == 0)
			{
				if (day > 30 * EXP_SPAN)
				{
					_ov = "1";
				}
				else
				{
					_ov = "0";
				}
			}
			else if (day > 365 * EXP_SPAN * lic.year)
			{
				_ov = "1";
			}
		}
		else if (ret == ERROR_LIC)
		{
			lic.version = 20;
			lic.mode = mode;
			lic.num = 1;
			lic.chksum = 0;
			lic.ov = 0;
			lic.year = 0;
			lic.date = current_time;

			string sLic = "";
			CMySoft::Instance().Grn(lic, sLic);
			_code = sLic;
			_ov = "0";

			set_config_val("wo3", _code);

			if (msix == 0)
			{
				CMySoft::Instance().add_lic(szSubKey, "wo3", _code);
			}
			_first = lic.date;
		}
		else
		{
			//ERROR_OVERDUE
			_code = v3;
			_ov = "1";
			_first = lic.date;
		}
	}

	string orderby = "";
	Cimagedal::Instance().refresh_orderby(orderby);

	_orderby = "";

	vector<string> v1 = tokenize(orderby, ",", true, "");
	int len = (int)v1.size();
	for (int i = 0; i < len; i++)
	{
		_orderby = v1[i];
		break;
	}

	init_order();

	return S_OK;
}

list<TAG_DTO>* CDataSrv::get_lstTag()
{
	if (_lstTag.empty())
	{
		Cimagedal::Instance().get_tags("0", _lstTag);
	}
	return &_lstTag;
}

long CDataSrv::get_lstImageIds(list<string>& lstImageIds)
{
	EnterCriticalSection(&_cs);
	if (_hasCache == 0)
	{
		_lstImageIds.clear();
		if (_q_req.search == "q")
		{
			Cimagedal::Instance().get_image_ids(_q_req, _search_req, _lstImageIds);
		}
		else
		{
			Cimagedal::Instance().get_image_ids(_q_req, _lstImageIds);
		}
		_hasCache = 1;
	}
	lstImageIds = _lstImageIds;


	LeaveCriticalSection(&_cs);

	return 0;
}

long CDataSrv::get_lstImageIds_map(list<string>& lstImageIds)
{
	Cimagedal::Instance().Cimagedal::Instance().get_images_all_map(lstImageIds);

	return S_OK;
}

string CDataSrv::get_menubar_tags(int type, string cat_ids, string pic_ids)
{
	get_lstTag();

	string menubar_tags = "";
	string image_ids = "";

	if (pic_ids != "")
	{
		image_ids = pic_ids;
	}
	else
	{
		if (type == 0)
		{
			image_ids = "0";
		}
	}

	if (image_ids == "" && type == 0)
	{
		return menubar_tags;
	}

	list<IMAGE_TAG_DTO> lstImageTag;
	Cimagedal::Instance().get_menubar_tags(image_ids, cat_ids, lstImageTag);
	if (!lstImageTag.empty())
	{
		string tags_item = "";
		string menubar_tags_span = "";
		CLang::Instance().getOneData("menubar_tags_span", menubar_tags_span);

		int i = 0;
		list<TAG_DTO>::iterator it;
		list<IMAGE_TAG_DTO>::iterator iter;
		for (iter = lstImageTag.begin(); iter != lstImageTag.end(); iter++)
		{
			for (it = _lstTag.begin(); it != _lstTag.end(); it++)
			{
				if (iter->tag_id == it->id && it->nb_images > 0)
				{
					i++;
					string title = it->name + "(" + uint2str(it->nb_images) + ")";
					string temp = menubar_tags_span;
					ReplaceOne(temp, "{$id}", it->id);
					ReplaceOne(temp, "{$name}", title);

					ReplaceOne(temp, "{$title}", title);
					tags_item += temp;
					if (i > 20)
					{
						break;
					}
				}
			}
		}

		int nb = 0;
		for (it = _lstTag.begin(); it != _lstTag.end(); it++)
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

		if (tags_item != "")
		{
			string menubar_tags_all = "";
			CLang::Instance().getOneData("menubar_tags_all", menubar_tags_all);

			ReplaceOne(menubar_tags_all, "nbTags", nbTags);



			tags_item += menubar_tags_all;

			CLang::Instance().getOneData("menubar_tags", menubar_tags);
			ReplaceOne(menubar_tags, "{$menubar_tags_span}", tags_item);

		}
	}
	return menubar_tags;
}

string CDataSrv::get_image_url(int type, IMAGE_DTO dto)
{
	my_tolower(dto.ext);
	string url = "";

	//galleries/casting.mp4
	//_add/aaa/casting.mp4

	//mp3, mp4 play
	if (type == 3)
	{
		url = dto.ori;
		string::size_type pos = url.find("/galleries_");
		if (pos == string::npos)
		{
			url = _albumName + url;
		}
		return url;
	}
	else
	{
		url = dto.ori;
		if (type == 0 || type == 2 || type == 100 || type == 9)
		{
			//in use
			if (dto.coi != "")
			{
				url = (dto.iscoi == 1) ? dto.coi : dto.ori;
			}
		}

		if (dto.ext == "mp3")
		{
			if (dto.representative_ext == "")
			{
				url = _mp3_url;
				if (type == 100)
				{
					ReplaceOne(url, ".png", ".jpg");
				}
				return url;
			}
			ReplaceOne(url, ".mp3", ".png");
		}
		else if (dto.ext == "ogg")
		{
			if (dto.representative_ext == "")
			{
				url = _mp3_url;
				if (type == 100)
				{
					ReplaceOne(url, ".png", ".jpg");
				}
				return url;
			}
			ReplaceOne(url, ".ogg", ".png");
		}
		else if (dto.ext == "mp4")
		{
			if (dto.representative_ext != "")
			{
				url = _mp4_url;
				if (type == 100)
				{
					ReplaceOne(url, ".png", ".jpg");
				}
				return url;
			}
			ReplaceOne(url, ".mp4", ".png");
		}
		else if (dto.ext == "webm")
		{
			if (dto.representative_ext != "")
			{
				url = _mp4_url;
				if (type == 100)
				{
					ReplaceOne(url, ".png", ".jpg");
				}
				return url;
			}
			ReplaceOne(url, ".webm", ".png");
		}
		else  if (dto.ext == "gif")
		{
			if (dto.representative_ext != "")
			{
				url = _gif_url;
				if (type == 100)
				{
					ReplaceOne(url, ".png", ".jpg");
				}
				return url;
			}
			ReplaceOne(url, ".gif", ".png");
		}

		if (type == 0) //0://in use small
		{
			_data2_thumb(url);
			url = _albumName + url;
		}
		else if (type == 1) //1:ori small
		{
			_data2_thumb(url);
			url = _albumName + url;
		}
		else if (type == 100) //100:min small
		{
			_data2_thumb(url);
			url = _albumName + url;
			url += "_2.png";
		}
		else if (type == 9) //100:film
		{
			_data2_thumb(url);
			url = _albumName + url;
			url += "_3.jpg";
		}
		else if (type == 2)//2:coi or ori big
		{
			if (dto.ext == "mp3" || dto.ext == "mp4" || dto.ext == "webm" || dto.ext == "ogg")
			{
				_data2_thumb(url);
				url = _albumName + url;
			}
			else if (dto.ext == "gif")
			{
				_data2_thumb(url);
				url = _albumName + url;
			}
			else if (dto.width > 6000 || dto.height > 6000)
			{
				_data2_thumb(url);
				url = _albumName + url;
			}
			else
			{
				string::size_type pos = url.find("/galleries_");
				if (pos == string::npos)
				{
					url = _albumName + url;
				}
			}
		}
	}

	return url;
}

long CDataSrv::clear_all()
{
	clear_cache_image_ids();
	clear_cache_tags();
	clear_cache_slider();
	return S_OK;
}

long CDataSrv::get_roots(list<ROOT_DTO>& lstRoot)
{
	Cchatdal::Instance().get_roots("", "", lstRoot);
	return S_OK;
}

string CDataSrv::get_username()
{
	if (_username == "")
	{
		wchar_t buf[1024] = { 0 };
		DWORD dw = 1024;
		GetUserName(buf, &dw);
		_username = _w2u(buf);
	}
	return _username;
}

long CDataSrv::get_config_val(string id, string& val)
{
	string ids = "'" + id + "'";
	list<CONFIG_DTO> lstConfig;
	Cchatdal::Instance().get_configs(ids, lstConfig);

	list<CONFIG_DTO>::iterator iter;
	for (iter = lstConfig.begin(); iter != lstConfig.end(); iter++)
	{
		if (iter->param == id)
		{
			val = iter->value;
			break;
		}
	}
	return S_OK;
}

long CDataSrv::set_config_val(string id, string val)
{
	if (_readonly == 0)
	{
		list<CONFIG_DTO> lstConfig;
		CONFIG_DTO displayDto;
		displayDto.param = id;
		displayDto.value = val;

		lstConfig.push_back(displayDto);
		writedb::Instance().add_configs(lstConfig);
	}

	return S_OK;
}

long CDataSrv::get_play_htm(string id, string& tpl)
{
	list<IMAGE_DTO> lstImage;
	Cimagedal::Instance().get_images_by_ids(id, lstImage);

	string ext = "";
	string url = "";
	if (!lstImage.empty())
	{
		IMAGE_DTO dto = lstImage.front();
		ext = dto.ext;
		url = get_image_url(3, dto);
	}

	if (ext == "mp3" || ext == "ogg")
	{
		CLang::Instance().getOneData("play_box_aud", tpl);
	}
	else if (ext == "mp4" || ext == "webm")
	{
		CLang::Instance().getOneData("play_box_vid", tpl);
	}
	else if (ext == "gif")
	{
		CLang::Instance().getOneData("play_box_gif", tpl);
	}

	Replace(tpl, "{$src}", url);

	return S_OK;
}

long CDataSrv::select_gallery(string albumName0, string viewer, string act, string& syn)
{
	string alb = albumName0;
	if (alb == "public")
	{
		_acc = "2";
	}
	else if (alb == "private")
	{
		_acc = "0";
		if (viewer == "1")
		{
			_acc = "2";
		}
	}
	_albumName = alb;
	wstring logdir = get_appdata_path(alb);

	/*0829
	wstring sample = get_module_path(NULL);
	if (alb == "private")
	{
	sample += L"\\data2\\_db\\archive.ini";
	}
	else
	{
	sample += L"\\data1\\_db\\archive.ini";
	}
	//E:\\oo1.iso\\E_\\voy
	//_readonly = 8;

	DWORD dwAttr = GetFileAttributes(sample.c_str());
	if (dwAttr != -1)
	{
	// found something; better not be a folder....
	if (dwAttr & FILE_ATTRIBUTE_READONLY)
	{
	Cbasedal::Instance()._appDataDir = logdir;
	Cbasedal::Instance().copy2appData(alb);
	_readonly = 8;

	CLang::Instance().getOneData("error_readonly", _str_readonly);
	string dv2 = "";
	wstring dv = sample.substr(0, 2) + L"\\";
	UINT uType = GetDriveType(dv.c_str());
	if (uType == DRIVE_CDROM)
	{
	dv2 = "(" + _w2u(sample.substr(0, 2)) + ") CDROM";
	}
	ReplaceOne(_str_readonly, "{$Drive}", dv2);
	}
	}
	*/
	Cbasedal::Instance()._appDataDir = logdir;
	_appDataDir = logdir;

	int res = Cbasedal::Instance().refresh_db(alb, logdir);
	if (res != 0)
	{
		_db_error = res;
		return 1;
	}

	Cimagedal::Instance().set_alb(alb);
	Cchatdal::Instance().set_alb(alb);

	sqlite3* pDb = Cbasedal::Instance().getDB();
	Cimagedal::Instance().setDb(pDb);
	Cchatdal::Instance().setDb(pDb);

	if (act == "c.alb")
	{
		map<string, string> lcid;
		map<string, string> lang;
		get_language(lcid, lang);

		string val = "";
		ReadPrivate("language", val);
		if (val == "")
		{
			get_config_val("language", val);
		}

		if (val != "")
		{
			CLang::Instance()._language = val;
		}
		else
		{

			if (msix == 0)
			{
				wstring subKey = L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\PhotoNest Home_is1";
				if (alb == "private")
				{
					subKey = L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\PhotoNest Encryption_is1";
				}
				HKEY hkey;
				long ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, subKey.c_str(), 0, KEY_QUERY_VALUE, &hkey);
				if (ret == ERROR_SUCCESS)
				{
					wchar_t buf[100] = { 0 };// | KEY_WOW64_64KEY

					DWORD dwType = REG_SZ;
					DWORD dwSize = sizeof(buf);

					RegQueryValueEx(hkey, L"Inno Setup: Language", NULL, &dwType, (BYTE*)buf, &dwSize);
					CLang::Instance()._language = _w2u(buf);

					RegCloseKey(hkey);
				}
			}

			if (CLang::Instance()._language == "")
			{
				LCID lcd = GetUserDefaultUILanguage();
				char buf[100] = { 0 };
				sprintf_s(buf, 100, "0x%04X", lcd);
				map<string, string>::iterator I = lcid.find(buf);
				if (I != lcid.end())
				{
					CLang::Instance()._language = I->second;
				}
				else
				{
					CLang::Instance()._language = "en";
				}
			}
		}

		map<string, string>::iterator I = lang.find(CLang::Instance()._language);
		if (I == lang.end())
		{
			CLang::Instance()._language = "en";
		}

		//if (val == "")
		{
			Cbasedal::Instance().upgrade();

			CLang::Instance().refresh_language();
			refresh_gallery_title();

			syn = "";
			list<ROOT_DTO> lstRoot;
			get_roots(lstRoot);
			if (lstRoot.empty())
			{
				if (alb == "public")
				{
					_acc = "1";
				}
			}
			else
			{
				list<ROOT_DTO>::iterator it;
				for (it = lstRoot.begin(); it != lstRoot.end(); it++)
				{
					syn += it->id + ";" + it->root_dir0 + "|";
				}
			}

			TIME_ZONE_INFORMATION   tzi;
			GetSystemTime(&tzi.StandardDate);
			GetTimeZoneInformation(&tzi);
			if (_time_zone_bias != tzi.Bias)
			{
				_time_zone_bias = tzi.Bias;
				writedb::Instance().update_images_byzone(tzi.Bias);

				clear_all();
			}

			_albumName = alb;
			_login = "admin";

			clear_cache_tags();
			clear_cache_image_ids();
			refresh_favorites();
		}
	}
	return S_OK;
}

void CDataSrv::WritePrivate(string key, const char* val)
{
	CSimpleIniA ini;
	ini.SetUnicode(true);

	wstring iniPath = get_appdata_path(_albumName);
	iniPath += L"\\prof.ini";

	ini.LoadFile(iniPath.c_str());
	ini.SetValue("section", key.c_str(), val);
	ini.SaveFile(iniPath.c_str());
}

void CDataSrv::ReadPrivate(string key, string& val)
{
	val = "";

	CSimpleIniA ini;
	ini.SetUnicode(true);

	wstring iniPath = get_appdata_path(_albumName);
	iniPath += L"\\prof.ini";

	ini.LoadFile(iniPath.c_str());

	const char* pv1 = ini.GetValue("section", key.c_str());
	if (pv1 != nullptr)
	{
		val = pv1;
	}
}

void CDataSrv::get_language(map<string, string>& lcid, map<string, string>& lang)
{
	string szhtml = "";
	char* szResponse = 0;
	int ret = Read_htm(IDR_LANG_TXT, L"html/_lang.txt", &szResponse);
	if (ret == 0)
	{
		szhtml = szResponse;
		LocalFree(szResponse);
	}

	vector<string> v1 = tokenize(szhtml, "\n", true, "");

	int len = (int)v1.size();
	for (int i = 2; i < len; i++)
	{
		vector<string> v2 = tokenize(v1[i], ",", true, "");
		lcid.insert(map<string, string>::value_type(v2[1], v2[0]));
	}

	vector<string> lang_id = tokenize(v1[0], ",", true, "");
	vector<string> lang_name = tokenize(v1[1], ",", true, "");

	len = (int)lang_id.size();
	for (int i = 0; i < len; i++)
	{
		lang.insert(map<string, string>::value_type(lang_id[i], lang_name[i]));
	}
}

void CDataSrv::get_windir(wstring& windir)
{
	wchar_t buffer[MAX_PATH] = { 0 };
	SHGetSpecialFolderPath(NULL, buffer, CSIDL_WINDOWS, FALSE);
	windir += L"\"" + wstring(buffer) + L"\",";
	Replace(windir, L"\\", L"/");

	//windir += L"\":/users/",";
	windir += L"\":/Program Files\",";

	wstring	szPath = get_module_path(NULL);
	windir += L"\"" + szPath + L"/templ\",";
	windir += L"\"" + szPath + L"/extension\",";
	//windir += L"\"" + szPath + L"/data1/_db\",";
	//windir += L"\"" + szPath + L"/data2/_db\",";
	//windir += L"\"" + szPath + L"/data1/_temp\",";
	//windir += L"\"" + szPath + L"/data2/_temp\",";
	windir += L"\"" + szPath + L"/data1/_thumb\",";
	windir += L"\"" + szPath + L"/data2/_thumb\"";
}

void CDataSrv::get_currdir(wstring& szPath, wstring& currdir)
{
	szPath = get_module_path(NULL);
	currdir += L"\"" + szPath + L"/data1\",";
	currdir += L"\"" + szPath + L"/data2\",";
	currdir += L"\"" + szPath + L"/data1/_data\",";
	currdir += L"\"" + szPath + L"/data2/_data\",";
	currdir += L"\"" + szPath + L"/bin\"";
}

void CDataSrv::get_films(list<FILM_DTO>& lstfilm)
{
	Cchatdal::Instance().get_films("", 0, lstfilm);
}

void CDataSrv::proc_public_acc()
{
	if (_albumName == "public")
	{
		_acc = "2";

		list<ROOT_DTO> lstRoot;
		get_roots(lstRoot);
		if (lstRoot.empty())
		{
			_acc = "1";
		}
	}
}

long CDataSrv::init_order()
{
	_lstOrder.clear();

	ORDER_DTO dto;

	dto.id = "1";
	dto.order = "file ASC";
	dto.type = "";
	CLang::Instance().getOneData("title_file_AZ", dto.title);
	_lstOrder.push_back(dto);

	dto.id = "2";
	dto.order = "file DESC";
	dto.type = "";
	CLang::Instance().getOneData("title_file_ZA", dto.title);
	_lstOrder.push_back(dto);

	dto.id = "3";
	dto.order = "a.name ASC";
	dto.type = "";
	CLang::Instance().getOneData("title_title_AZ", dto.title);
	_lstOrder.push_back(dto);

	dto.id = "4";
	dto.order = "a.name DESC";
	dto.type = "";
	CLang::Instance().getOneData("title_title_ZA", dto.title);
	_lstOrder.push_back(dto);

	dto.id = "5";
	dto.order = "date_creation DESC";
	dto.type = "date_creation";
	CLang::Instance().getOneData("title_created_NO", dto.title);
	_lstOrder.push_back(dto);

	dto.id = "6";
	dto.order = "date_creation ASC";
	dto.type = "date_creation";
	CLang::Instance().getOneData("title_created_ON", dto.title);
	_lstOrder.push_back(dto);

	dto.id = "7";
	dto.order = "date_available DESC";
	dto.type = "date_available";
	CLang::Instance().getOneData("title_posted_NO", dto.title);
	_lstOrder.push_back(dto);

	dto.id = "8";
	dto.order = "date_available ASC";
	dto.type = "date_available";
	CLang::Instance().getOneData("title_posted_ON", dto.title);
	_lstOrder.push_back(dto);

	dto.id = "9";
	dto.order = "hit DESC";
	dto.type = "";
	CLang::Instance().getOneData("title_visits_HL", dto.title);
	_lstOrder.push_back(dto);

	dto.id = "10";
	dto.order = "hit ASC";
	dto.type = "";
	CLang::Instance().getOneData("title_visits_LH", dto.title);
	_lstOrder.push_back(dto);

	dto.id = "0";
	dto.order = "b.rank asc";
	dto.type = "";
	CLang::Instance().getOneData("title_manual", dto.title);
	_lstOrder.push_back(dto);

	return 0;

}
