#include "StdAfx.h"
#include "unitil.h"
#include <vector>
#include <string>
#include "../../templ/templ/resource.h"
#include "../../photol/photol/photol.h"

#include "lang.h"
#include "imagedal.h"
#include "chatdal.h"
#include "wsjosn.h"
#include <process.h>
#include <thread>

#include "DataSrv.h"
#include "HtmlgetAdmin.h"
#include "HtmlgetAdminProc.h"
#include "HtmlgetIndexProc.h"
#include "HtmlgetCal.h"
#include "HtmlgetFolder.h"
#include "Htmlupload.h"
#include "HtmlgetBatch.h"
#include "Htmlexif.h"

#include "imlog.h"

#include "HtmlgetMenubar.h"
#include "HtmlSearchProc.h"
#include "writedb.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

CHtmlgetAdmin::CHtmlgetAdmin(void)
{
	_progress_step = 1;
	_progress_len = 100;
	_progress_len3 = 0;
	_progress_step3 = 0;

	_nb_gg2_thread = 0;
	_progress_cancel = 0;
	_crc = 0;
	_trs = "";
	_lstCatNbOk = 0;
	_cpu = thread::hardware_concurrency();
	if (_cpu == 0 || _cpu > 20)
	{
		_cpu = 10;
	}
	else
	{
		_cpu += 1;
	}

	InitializeCriticalSection(&_nbcs);
	InitializeCriticalSection(&_cs0);
}

CHtmlgetAdmin::~CHtmlgetAdmin(void)
{
	DeleteCriticalSection(&_nbcs);
	DeleteCriticalSection(&_cs0);
}

long CHtmlgetAdmin::get_admin_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string tpl = "";
	CHtmlgetCal::Instance()._outsql = "";

	string page = "";
	lookitem(request_dto.szArgs, "page=", page);

	if (page == "admin.storage")
	{
		string stat = "err";
		string htm = "";

		string res = " ";
		uint64_t fsize = 0;

		long ret = get_folder_sizes(0, fsize);
		if (fsize != 0)
		{
			float d = fsize / 1024.0f;
			if (d > 1024.0f)
			{
				d = d / 1024.0f;
				res = float2Str(d, "%.2f") + "GB";
			}
			else
			{
				res = float2Str(d, "%.1f") + "MB";
			}
			stat = "ok";
			htm = res;
		}

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String(stat.c_str());

		writer.String("htm");
		writer.String(htm.c_str());
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();
		return S_OK;
	}
	else if (page == "admin.box")
	{
		string id = "";
		lookitem(request_dto.szArgs, "id=", id);

		CDataSrv::Instance().get_play_htm(id, tpl);
		response_dto.szContentType = "text/html";
		response_dto.szResponse = tpl;
		return S_OK;
	}
	else if (page == "admin.rank")
	{
		string cat_id = "";
		string idx = "";
		lookitem(request_dto.szArgs, "cat_id=", cat_id);
		lookitem(request_dto.szArgs, "idx=", idx);

		list<IMAGE_DTO> lstImageDto;
		Cimagedal::Instance().get_images_by_cats(cat_id, atoi(idx.c_str()), lstImageDto);

		string rank_of_image = "";
		CLang::Instance().getOneData("rank_of_image", rank_of_image);

		int ind = atoi(idx.c_str());
		uint32_t i = 10 + ind * PERPAGE_NUM * 10;
		string thumbnails = "";
		list<IMAGE_DTO>::iterator iter;
		for (iter = lstImageDto.begin(); iter != lstImageDto.end(); iter++)
		{
			i += 10;
			string temp = rank_of_image;

			string url0 = CDataSrv::Instance().get_image_url(1, *iter);
			ReplaceOne(temp, "{$url}", url0);
			ReplaceOne(temp, "{$id}", iter->id);
			ReplaceOne(temp, "{$rank}", uint2str(i));
			Replace(temp, "{$name}", iter->name);
			thumbnails += temp;
		}
		response_dto.szContentType = "text/html";
		response_dto.szResponse = thumbnails;
		return S_OK;
	}
	else if (page == "admin.galleries")
	{
		string trs = "";
		Htmlread(IDR_ADMIN_GALLERIES_HTML, L"html/_admin_galleries.html", tpl);
		ReplaceOne(tpl, "{$ov}", CDataSrv::Instance()._ov);

		string pen_title = "";
		string pen_title2 = "";
		CLang::Instance().getOneData("pen_title", pen_title);
		CLang::Instance().getOneData("pen_title2", pen_title2);

		ReplaceOne(tpl, "{$pen_title}", pen_title);
		ReplaceOne(tpl, "{$pen_title2}", pen_title2);

		string does_not_move = "";
		if (CDataSrv::Instance()._albumName == "private")
		{
			CLang::Instance().getOneData("does_not_move2", does_not_move);
		}
		else
		{
			CLang::Instance().getOneData("does_not_move", does_not_move);
		}
		ReplaceOne(tpl, "{$does_not_move}", does_not_move);


		ReplaceOne(tpl, "{$album}", CDataSrv::Instance()._albumName);
		ReplaceOne(tpl, "o='style'", "");

		wstring szPath = L"";
		wstring currdir = L"";
		CDataSrv::Instance().get_currdir(szPath, currdir);
		ReplaceOne(tpl, "{$appdir}", _w2u(szPath));
		ReplaceOne(tpl, "$currdir", _w2u(currdir));

		wstring windir = L"";
		CDataSrv::Instance().get_windir(currdir);
		ReplaceOne(tpl, "$windir", _w2u(windir));

		string upload_file_types = "";
		CLang::Instance().getOneData("upload_file_types", upload_file_types);

		string uploadWarnings = "";
		CLang::Instance().getOneData("uploadWarnings", uploadWarnings);
		translate(uploadWarnings, "", "");
		ReplaceOne(uploadWarnings, "%s", upload_file_types);
		ReplaceOne(tpl, "{$uploadWarnings}", uploadWarnings);

	}
	else if (page == "admin.firstrun")
	{
		string reg = "";
		lookitem(request_dto.szArgs, "reg=", reg);

		string trs = "";
		Htmlread(IDR_ADMIN_FIRST_HTML, L"html/_admin_first.html", tpl);

		std::wstring dst = L"";

		wstring p = get_module_path(NULL);
		wstring::size_type pos = p.find(L"/windowsapps/photonest");
		if (pos != wstring::npos)
		{
			wchar_t* szDocument;
			if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &szDocument)))
			{
				dst = szDocument;
				dst += L"\\PhotoNest_App";
				dst += L"\\Nest1";
				CoTaskMemFree(szDocument);
			}
		}
		else
		{
			dst = p;
		}
		dst += L"\\Sample";

		Replace(dst, L"\\", L"/");
		my_tolower(dst);

		ReplaceOne(tpl, "{$sample}", _w2u(dst));

		list<ROOT_DTO> lstRoot;
		CDataSrv::Instance().get_roots(lstRoot);

		if (reg == "1")
		{
			ReplaceOne(tpl, "contents", "none");
		}
		string nbroot = "1";
		if (lstRoot.empty())
		{
			nbroot = "0";
			ReplaceOne(tpl, "contents", "");
		}
		else
		{
			ReplaceOne(tpl, "contents", "none");
		}
		ReplaceOne(tpl, "{$nbroot}", nbroot);

		ReplaceOne(tpl, "{$ov}", CDataSrv::Instance()._ov);
		ReplaceOne(tpl, "{$reg}", reg);

		uint32_t r = CDataSrv::Instance()._readonly;
		ReplaceOne(tpl, "{$readonly}", uint2str(r));
		ReplaceOne(tpl, "{$error_readonly}", CDataSrv::Instance()._str_readonly);

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
		ReplaceOne(tpl, "{$error}", Int2Str(CDataSrv::Instance()._db_error));
		ReplaceOne(tpl, "{$album}", CDataSrv::Instance()._albumName);

		wstring sample = CDataSrv::Instance()._appDataDir + L"/galleries";//get_module_path(NULL) + L"/galleries";

		ReplaceOne(tpl, "{$sample}", _w2u(sample));

		wstring szPath = L"";
		wstring currdir = L"";
		CDataSrv::Instance().get_currdir(szPath, currdir);
		ReplaceOne(tpl, "{$appdir}", _w2u(szPath));
		ReplaceOne(tpl, "$currdir", _w2u(currdir));

		wstring windir = L"";
		CDataSrv::Instance().get_windir(currdir);
		ReplaceOne(tpl, "$windir", _w2u(windir));

		string upload_file_types = "";
		CLang::Instance().getOneData("upload_file_types", upload_file_types);

		string uploadWarnings = "";
		CLang::Instance().getOneData("uploadWarnings", uploadWarnings);
		translate(uploadWarnings, "", "");
		ReplaceOne(uploadWarnings, "%s", upload_file_types);
		ReplaceOne(tpl, "{$uploadWarnings}", uploadWarnings);

	}
	else if (page == "admin.photos_add")
	{
		string album = "";
		lookitem(request_dto.szArgs, "album=", album);

		Htmlread(IDR_ADMIN_ADD_HTML, L"html/_admin_add.html", tpl);
		ReplaceOne(tpl, "{$ov}", CDataSrv::Instance()._ov);
		ReplaceOne(tpl, "o='style'", "");

		string pending_span = "";
		CLang::Instance().getOneData("pending_span", pending_span);
		{
			if (CDataSrv::Instance()._ischanged != 0)
			{
				ReplaceOne(pending_span, "#f00;", "");
			}
			else
			{
				ReplaceOne(pending_span, "#f00;", "display:none;");
			}
		}
		ReplaceOne(tpl, "{$pending_span}", pending_span);


		wstring szPath = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);

		CHtmlupload::Instance().set_roots();

		string type = "1";
		if (CDataSrv::Instance()._albumName == "private")
		{
			type = "2";
			szPath += L"/data2";
			ReplaceOne(tpl, "o='tabsheet'", "style=\"display:none;\"");
		}
		else
		{
			szPath += L"/data1";
		}
		ReplaceOne(tpl, "{$type}", type);
		ReplaceOne(tpl, "{$path}", _w2u(szPath));

		ReplaceOne(tpl, "{$maxFileSize}", uint2str(CDataSrv::Instance()._maxFileSize));
		ReplaceOne(tpl, "o='uploadform'", "");
		ReplaceOne(tpl, "o='droptext'", "");
		ReplaceOne(tpl, "o='title'", "");
		ReplaceOne(tpl, "o='title'", "");
		ReplaceOne(tpl, "o='tabsheet'", "");

		string upload_file_types = "";
		CLang::Instance().getOneData("upload_file_types", upload_file_types);

		ReplaceOne(tpl, "{$upload_file_types}", upload_file_types);

		string uploadWarnings = "";
		CLang::Instance().getOneData("uploadWarnings", uploadWarnings);
		translate(uploadWarnings, "", "");
		ReplaceOne(uploadWarnings, "%s", upload_file_types);
		ReplaceOne(tpl, "{$uploadWarnings}", uploadWarnings);

		ReplaceOne(tpl, "{$file_exts}", upload_file_types);

		if (album != "")
		{
			album = "&quot;" + album + "&quot;";
		}
		ReplaceOne(tpl, "{$selected_category}", album);

	}
	else if (page == "admin.tags")
	{
		CHtmlgetAdminProc::Instance().proc_tag_htm(request_dto, tpl);
	}
	else if (page == "admin.batch")
	{
		CHtmlgetBatch::Instance().proc_batch_htm(request_dto, "", tpl);
	}
	else if (page == "admin.batch_picture_data")
	{
		string picture_data = CHtmlgetBatch::Instance()._picture_data;
		translate(picture_data, "", "");

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.String("htm");
		writer.String(picture_data.c_str());

		writer.String("ids");
		writer.String("all");
		//writer.String(CHtmlgetBatch::Instance()._element_ids.c_str());

		writer.EndObject();

		response_dto.szResponse = sb.GetString();
		response_dto.szContentType = "text/plain; charset=utf-8";
		return 0;
	}
	else if (page == "admin.config")
	{
		CHtmlgetAdminProc::Instance().proc_config_htm(page, request_dto, "", tpl);
	}
	else if (page == "admin.album")
	{
		CHtmlgetAdminProc::Instance().proc_properties_htm(page, request_dto, "", tpl);
	}
	else if (page == "admin.cat_name")
	{
		CHtmlgetAdminProc::Instance().proc_cat_name_htm(request_dto, "", tpl);
	}
	else if (page == "admin.cat_list")
	{
		CHtmlgetAdminProc::Instance().proc_cat_list_htm(request_dto, "", tpl);
	}
	else if (page == "admin.stats")
	{
		CHtmlgetAdminProc::Instance().proc_stats_htm(request_dto, "", tpl);
	}
	else if (page == "admin.photo")
	{
		CHtmlgetAdminProc::Instance().proc_photo_htm(page, request_dto, "", tpl);
	}
	else if (page == "admin.remove_enc_box")
	{
		string rootid = "";
		lookitem(request_dto.szArgs, "rootid=", rootid);

		string folder = "";

		list<ROOT_DTO> lstRoot;
		CDataSrv::Instance().get_roots(lstRoot);

		list<ROOT_DTO>::iterator it2;
		for (it2 = lstRoot.begin(); it2 != lstRoot.end(); it2++)
		{
			if (it2->id == rootid)
			{
				folder = it2->root_dir0;
				break;
			}
		}

		list<CATEGORY_DTO> lst;
		CHtmlgetMenubar::Instance().get_categories_order_by_rank(0, lst);
		string albName = "";
		list<CATEGORY_DTO>::iterator iter;
		for (iter = lst.begin(); iter != lst.end(); iter++)
		{
			if (iter->root_dir0 == folder)
			{
				albName = iter->name;
				break;
			}
		}

		if (albName == "")
		{
			string::size_type n1 = folder.rfind("/");
			if (n1 != string::npos)
			{
				albName = folder.substr(n1 + 1);
			}
			else
			{
				albName = folder;
			}
		}

		string remove_enc_box = "";
		Htmlread(IDR_REMOVE_ENC_BOX_HTML, L"html/_remove_enc_box.html", remove_enc_box);
		ReplaceOne(remove_enc_box, "{$rootid}", rootid);
		ReplaceOne(remove_enc_box, "{$album}", CDataSrv::Instance()._albumName);
		ReplaceOne(remove_enc_box, "{$folder}", albName);

		_progress_step = 1;
		_progress_len = 100;

		string remove_enc_title = "";
		if (CDataSrv::Instance()._albumName == "private")
		{
			CLang::Instance().getOneData("remove_enc_title1", remove_enc_title);
		}
		else
		{
			CLang::Instance().getOneData("remove_enc_title", remove_enc_title);
		}
		ReplaceOne(remove_enc_box, "{$remove_enc_title}", remove_enc_title);

		translate(remove_enc_box, "", "");
		response_dto.szContentType = "text/html";
		response_dto.szResponse = remove_enc_box;

		return S_OK;
	}
	else
	{
		Htmlread(IDR_ADMIN_HTML, L"html/_admin.html", tpl);
		ReplaceOne(tpl, "{$ov}", CDataSrv::Instance()._ov);
		ReplaceOne(tpl, "{$albumName}", CDataSrv::Instance()._albumName);

		time_t current_time;
		time(&current_time);
		struct tm tm0;
		localtime_s(&tm0, &current_time);
		string yearmonth = "year=" + Int2Str(tm0.tm_year + 1900);
		ReplaceOne(tpl, "{yearmonth}", yearmonth);

		string error_lic = "";
		if (CDataSrv::Instance()._ov == "1")
		{
			CLang::Instance().getOneData("error_lic", error_lic);
			ReplaceOne(error_lic, "999", "");
		}
		ReplaceOne(tpl, "{$error_lic}", error_lic);

		if (CDataSrv::Instance()._ischanged == 1)
		{
			ReplaceOne(tpl, "#f00;", "");
		}
		else
		{
			ReplaceOne(tpl, "#f00;", "#f00; display:none;");
		}


		DASHBOARD_DTO dashboard_dto;
		Cchatdal::Instance().get_dashboard(dashboard_dto);

		string NB_PHOTOS = uint2str(dashboard_dto.nb_photos);
		ReplaceOne(tpl, "{$NB_PHOTOS}", NB_PHOTOS);

		string NB_ALBUMS = uint2str(dashboard_dto.nb_albums);
		ReplaceOne(tpl, "{$NB_ALBUMS}", NB_ALBUMS);

		string NB_TAGS = uint2str(dashboard_dto.nb_tags);
		ReplaceOne(tpl, "{$NB_TAGS}", NB_TAGS);

		string tag_associations = "";
		CLang::Instance().getOneData("tag_associations", tag_associations);
		translate(tag_associations, uint2str(dashboard_dto.nb_tag_images), "");
		ReplaceOne(tpl, "{$tag_associations}", tag_associations);

		string NB_COMMENTS = uint2str(dashboard_dto.nb_comments);
		ReplaceOne(tpl, "{$NB_COMMENTS}", NB_COMMENTS);

		string NB_VIEWS = uint2str(dashboard_dto.nb_views);
		ReplaceOne(tpl, "{$NB_VIEWS}", NB_VIEWS);

		string first_added_age = "";
		uint64_t tt = dashboard_dto.nb_first_add;
		if (tt != 0)
		{
			CLang::Instance().getOneData("first_added_age", first_added_age);

			string added_age = CHtmlgetAdminProc::Instance().get_ago_string(tt);
			ReplaceOne(first_added_age, "{$added_age}", added_age);

			translate(first_added_age, "", "");
			ReplaceOne(first_added_age, "%s", get_time_string(tt, "0"));

			ReplaceOne(first_added_age, "{yearmonth}", yearmonth);
		}
		ReplaceOne(tpl, "{$first_added_age}", first_added_age);
	}

	if (page != "admin.batch" && page != "admin.album" && page != "admin.photo" && page != "admin.cat_list")
	{
		uint32_t nbCaddie = 0;
		uint32_t nbDuplicates = 0;
		uint32_t nbOrphan = 0;
		CHtmlgetAdminProc::Instance().proc_head_htm(page, nbCaddie, nbDuplicates, nbOrphan, tpl);
	}

	translate(tpl, "", "");

	response_dto.szContentType = "text/html";
	response_dto.szResponse = tpl;

	return S_OK;
}

long CHtmlgetAdmin::get_ws_php(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string method = "";
	lookitem(request_dto.szArgs, "method=", method);

	if (method == "categories.getAdminList")
	{
		string type = "";
		lookitem(request_dto.szArgs, "type=", type);

		wstring szPath = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);
		string album = CDataSrv::Instance()._albumName;

		wstring dDir = (album == "private") ? L"/data2" : L"/data1";
		wstring path0 = szPath + dDir + L"/_data";

		list<CATEGORY_DTO> lst2;
		list<CATEGORY_DTO> lstCategory;

		list<CATEGORY_DTO>::iterator iter;
		if (type == "2")
		{
			CHtmlgetMenubar::Instance().get_categories_order_by_root_dir(1, lstCategory);
			CHtmlgetMenubar::Instance().proc_categories(&lstCategory);
			for (iter = lstCategory.begin(); iter != lstCategory.end(); iter++)
			{
				if (iter->nb_images == 0 && iter->sub_images == 0)
				{
					continue;
				}
				lst2.push_back(*iter);
			}
		}
		else
		{
			CHtmlgetMenubar::Instance().get_categories_order_by_root_dir(0, lstCategory);
			lst2 = lstCategory;
		}

		vector<CateGoryDto> categories;

		for (iter = lst2.begin(); iter != lst2.end(); iter++)
		{
			CateGoryDto gateGoryDto;
			gateGoryDto.id_ = iter->id;
			gateGoryDto.name_ = iter->name;
			gateGoryDto.root_dir_ = iter->root_dir0;
			gateGoryDto.full_id_ = iter->full_id;

			string fullname = "";
			get_fullname(gateGoryDto.full_id_, &lst2, fullname);

			if (fullname == "")
			{
				fullname = iter->name;
			}

			gateGoryDto.dir_ = iter->dir0;
			gateGoryDto.fullname_ = rtrim(fullname, "/");

			if (type == "1")
			{
				string t9 = iter->dir0;
				my_tolower(t9);
				string::size_type pos0 = t9.find(_w2u(path0));
				if (pos0 != string::npos)
				{
					categories.push_back(gateGoryDto);
				}
			}
			else
			{
				categories.push_back(gateGoryDto);
			}
		}

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.String("result");
		writer.StartObject();
		writer.String("categories");

		writer.StartArray();
		for (std::vector<CateGoryDto>::const_iterator I = categories.begin(); I != categories.end(); ++I)
		{
			I->Serialize(writer);
		}
		writer.EndArray();
		writer.EndObject();

		writer.EndObject();

		response_dto.szResponse = sb.GetString();
		response_dto.szContentType = "text/plain; charset=utf-8";
		return 0;
	}
	else if (method == "tags.getAdminList")
	{
		list<TAG_DTO> lstTag;
		list<TAG_DTO>::iterator iter;
		Cimagedal::Instance().get_tags("", lstTag);

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String("ok");

		writer.String("result");
		writer.StartObject();
		writer.String("tags");

		writer.StartArray();
		for (iter = lstTag.begin(); iter != lstTag.end(); iter++)
		{
			writer.StartObject();
			writer.String("id");
			writer.String(iter->id.c_str());
			writer.String("name");
			writer.String(iter->name.c_str());
			writer.String("nb_images");
			writer.String(uint2str(iter->nb_images).c_str());
			writer.EndObject();
		}
		writer.EndArray();
		writer.EndObject();

		writer.EndObject();

		response_dto.szResponse = sb.GetString();
		response_dto.szContentType = "text/plain; charset=utf-8";

		return S_OK;
	}
	else if (method == "upload.brisque")
	{
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
	else if (method == "upload.complete")
	{
		upload_complete(request_dto, response_dto);
		return S_OK;
	}
	else if (method == "categories.refresh")
	{
		categories_refresh(request_dto, response_dto);
		return S_OK;
	}
	else if (method == "categories.get_nb")
	{
		string ty = "";
		lookitem(request_dto.szArgs, "ty=", ty);

		string nb = "0";
		string stat = "err";
		if (ty == "1")
		{
			uint32_t r = CDataSrv::Instance()._readonly;
			if (r == 0)
			{
				string changed = "";
				writedb::Instance().set_categories_visible(changed);
				if (changed != "")
				{
					CDataSrv::Instance().clear_all();
					CHtmlgetIndexProc::Instance().clear_data();
					CHtmlgetMenubar::Instance().clear();
				}
			}

			_beginthreadex(NULL, 0, sb_nb, this, NULL, NULL);
		}
		else
		{
			if (_lstCatNbOk == 1)
			{
				stat = "ok";
				nb = Int2Str(CDataSrv::Instance()._ischanged);
			}
		}

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
	else if (method == "galleries.init0" || method == "galleries.init")
	{
		EnterCriticalSection(&_nbcs);

		galleries_init(request_dto, response_dto);
		LeaveCriticalSection(&_nbcs);
		return S_OK;
	}
	else if (method == "galleries.restore")
	{
		galleries_restore(request_dto, response_dto);
		return S_OK;
	}
	else if (method == "galleries.addfolder")
	{
		galleries_addfolder(request_dto, response_dto);

		time_t current_time;
		time(&current_time);

		HISTORY_DTO historyDto;
		historyDto.lastmodified = static_cast<uint64_t>(current_time);
		historyDto.id = "1";
		historyDto.user_id = CDataSrv::Instance()._login;
		long hr = writedb::Instance().add_history(historyDto);

		return S_OK;
	}
	//else if (method == "galleries.pwd")
	//{
	//	list<USER_DTO> lstUsers;
	//	Cchatdal::Instance().get_users(lstUsers);

	//	response_dto.szResponse = "0";
	//	if (!lstUsers.empty())
	//	{
	//		CDataSrv::Instance()._acc = "2";
	//		response_dto.szResponse = "1";
	//	}
	//	response_dto.szContentType = "text/plain; charset=utf-8";

	//}
	else if (method == "galleries.pending")
	{
		galleries_pending(request_dto, response_dto);
		return S_OK;
	}
	else if (method == "galleries.pendingok")
	{
		EnterCriticalSection(&_nbcs);
		galleries_pendingok(request_dto, response_dto);
		LeaveCriticalSection(&_nbcs);
		return S_OK;
	}
	else if (method == "galleries.modify")
	{
		string id = "";
		lookitem(request_dto.szArgs, "id=", id);

		string fullname = "";
		list<CATEGORY_DTO> lstCategory;
		CHtmlgetMenubar::Instance().get_categories(id, lstCategory);

		list<CATEGORY_DTO>::iterator iter;
		for (iter = lstCategory.begin(); iter != lstCategory.end(); iter++)
		{
			if (id == iter->id)
			{
				fullname = iter->name;
				break;
			}
		}

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.String("text");
		writer.String(fullname.c_str());
		writer.EndObject();

		response_dto.szResponse = sb.GetString();
		response_dto.szContentType = "text/plain; charset=utf-8";
		return S_OK;
	}
	else if (method == "galleries.language")
	{
		string language = "";
		lookitem(request_dto.szArgs, "language=", language);

		if (language != "" && language != CLang::Instance()._language)
		{
			CDataSrv::Instance().set_config_val("language", language);
			CLang::Instance()._language = language;
			CLang::Instance().refresh_language();
			CDataSrv::Instance().WritePrivate("language", language.c_str());

			CDataSrv::Instance().refresh_gallery_title();
		}
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
	else if (method == "empty.caddie")
	{
		string stat = "err";
		uint32_t nb = 0;
		writedb::Instance().clear_caddie(CDataSrv::Instance()._login, nb);
		if (nb > 0)
		{
			stat = "ok";
		}

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String(stat.c_str());
		writer.EndObject();

		response_dto.szResponse = sb.GetString();
		response_dto.szContentType = "text/plain; charset=utf-8";
		return S_OK;
	}
	else if (method == "delete.orphans")
	{
		string stat = "err";

		list<string> _lstFiles;
		list<string> _lstCoi;
		Cimagedal::Instance().get_files("", "", _lstFiles, _lstCoi);
		if (!_lstFiles.empty())
		{
			long hr = writedb::Instance().delete_orphans();
			if (hr == 0)
			{
				delete_image_files("0", _lstFiles, _lstCoi);
				writedb::Instance().update_last_import();
			}
			CDataSrv::Instance().clear_cache_slider();
			stat = "ok";
		}

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String(stat.c_str());
		writer.EndObject();

		response_dto.szResponse = sb.GetString();
		response_dto.szContentType = "text/plain; charset=utf-8";
		return S_OK;
	}
	else if (method == "batch.slider")
	{
		string szResponse = "";

		CHtmlSearchProc::Instance().proc_slider_req_htm(CHtmlgetBatch::Instance()._f_req, szResponse);
		response_dto.szResponse = szResponse;
		response_dto.szContentType = "text/plain; charset=utf-8";
		return S_OK;
	}
	else if (method == "viewer.u3.rotate")
	{
		//viewer.u3.min.js, only viewer
		get_viewer_u3_degree(request_dto, response_dto);
		return S_OK;
	}
	else if (method == "viewer.addto")
	{
		addbyViewer(request_dto, response_dto);

		time_t current_time;
		time(&current_time);

		HISTORY_DTO historyDto;
		historyDto.lastmodified = static_cast<uint64_t>(current_time);
		historyDto.id = "1";
		historyDto.user_id = CDataSrv::Instance()._login;
		long hr = writedb::Instance().add_history(historyDto);


		return S_OK;
	}

	return S_OK;
}

long CHtmlgetAdmin::get_folder_sizes(int ty, uint64_t& fsize)
{
	string album = CDataSrv::Instance()._albumName;

	list<ROOT_DTO> lstRoot;
	if (ty == 1)
	{
		CDataSrv::Instance().get_roots(lstRoot);
	}
	CHtmlgetFolder::Instance().GetFolderSize(lstRoot, album, fsize);

	return 0;
}

long CHtmlgetAdmin::get_viewer_u3_degree(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string file = "";
	lookitem(request_dto.szArgs, "src=", file);
	URLDecode2(file);

	string transform = "0";

	EXIFInfo result;
	CHtmlexif::Instance().get_exif(_u2w(file), result);

	int Orientation = result.Orientation;
	if (Orientation == 6)
	{
		transform = "90";
	}
	else if (Orientation == 8)
	{
		transform = "-90";
	}
	else if (Orientation == 3)
	{
		transform = "180";
	}

	response_dto.szResponse = transform;
	response_dto.szContentType = "text/html";

	return S_OK;
}

long CHtmlgetAdmin::addbyViewer(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string category = "";
	string src = "";
	lookitem(request_dto.szArgs, "category=", category);
	lookitem(request_dto.szArgs, "src=", src);
	URLDecode(category, 1);
	URLDecode2(src);
	//	add_tags[] : 11,~~1~~

	string id = "";
	string temp = category;
	trim(temp, "~~");
	trim(temp, " ");

	if (category == "~~" + temp + "~~")
	{
		id = temp;
	}
	else
	{
		time_t current_time;
		time(&current_time);

		CATEGORY_DTO catDto;
		catDto.name = category;
		catDto.addby = 1;
		catDto.full_id = "0";
		catDto.lastmodified = static_cast<uint64_t>(current_time);

		bool b = false;
		list<CATEGORY_DTO> lst;
		CHtmlgetMenubar::Instance().get_categories_order_by_rank(0, lst);

		list<CATEGORY_DTO>::iterator iter;
		for (iter = lst.begin(); iter != lst.end(); iter++)
		{
			if (iter->addby == 1)
			{
				id = iter->id;
				b = true;
				break;
			}
		}
		if (!b)
		{
			wstring szPath = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);
			string album = CDataSrv::Instance()._albumName;

			wstring dDir = (album == "private") ? L"/data2" : L"/data1";
			wstring path0 = szPath + dDir + L"/_data";
			wstring path1 = L"." + dDir + L"/_data";

			wchar_t szBuf[MAX_PATH] = { 0 };
			DWORD dwSize = GetFullPathName(path0.c_str(), MAX_PATH, szBuf, NULL);
			CreateDirectory(szBuf, NULL);

			path0 += L"/" + _u2w(catDto.name);
			path1 += L"/" + _u2w(catDto.name);

			uint32_t id9 = 0;
			ROOT_DTO root;
			root.root_dir0 = _w2u(path1);
			root.root_sizes = 0;
			writedb::Instance().add_root(root, id9);

			dwSize = GetFullPathName(path0.c_str(), MAX_PATH, szBuf, NULL);
			CreateDirectory(szBuf, NULL);

			catDto.root_id = uint2str(id9);
			catDto.dir0 = _w2u(path1);

			uint32_t cid = 0;
			writedb::Instance().add_category(catDto, cid);
			if (cid != 0)
			{
				id = uint2str(cid);
			}
		}
	}

	time_t current_time;
	time(&current_time);

	CHtmlupload::Instance().set_roots();

	CDataSrv::Instance()._acc = "2";

	REQUEST_DTO request_dto2 = request_dto;
	request_dto2.szFile = _u2w(src);
	request_dto2.szArgs = "type=fromviewer&category=" + id;

	long hr = CHtmlupload::Instance().post_upload_htm(request_dto2, response_dto);
	if (hr == 0)
	{
		writedb::Instance().set_representative_picture_id("");

		string val = "";
		CDataSrv::Instance().get_config_val("last_import", val);

		val = uint64str(static_cast<uint64_t>(current_time)) + "," + val;
		CDataSrv::Instance().set_config_val("last_import", val);
	}


	CHtmlgetIndexProc::Instance().clear_data();

	return S_OK;
}

long CHtmlgetAdmin::get_restore(list<string>& fol)
{
	wstring szPath = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);
	string album = CDataSrv::Instance()._albumName;

	wstring dDir = (album == "private") ? L"/data2" : L"/data1";
	wstring path0 = szPath + dDir + L"/_data";

	list<wstring> lstDir;
	CHtmlgetFolder::Instance().GetSubFolders(0, path0, lstDir);

	list<ROOT_DTO> lstRoot;
	CDataSrv::Instance().get_roots(lstRoot);

	wstring s0 = szPath;

	list<wstring>::iterator it;
	for (it = lstDir.begin(); it != lstDir.end(); it++)
	{
		bool b = false;

		list<WIN32_FIND_DATA > vecffd;
		CHtmlgetFolder::Instance().GetFolderPics(0, *it, vecffd);
		if (vecffd.empty())
		{
			b = true;
		}
		else
		{
			list<ROOT_DTO>::iterator it2;
			for (it2 = lstRoot.begin(); it2 != lstRoot.end(); it2++)
			{
				if (_wcsnicmp(it->c_str(), _u2w(it2->root_dir0).c_str(), it2->root_dir0.size()) == 0)
				{
					b = true;
					break;
				}
			}
		}

		if (!b)
		{
			wstring s = *it;
			my_tolower(s);

			wstring s1 = *it;

			wstring::size_type ndx = s.find(s0);
			if (ndx != wstring::npos)
			{
				s1 = L"." + s1.substr(s0.size());
			}
			fol.push_back(_w2u(s1));
		}
	}
	return 0;
}
