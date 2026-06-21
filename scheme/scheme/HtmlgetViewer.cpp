#include "StdAfx.h"
#include "unitil.h"
#include <vector>
#include <string>
#include "../../templ/templ/resource.h"
#include "../../photol/photol/photol.h"

#include "imlog.h"
#include "lang.h"
#include "HtmlgetViewer.h"
#include "HtmlgetFolder.h"

#include "DataSrv.h"
#include "chatdal.h"
#include "writedb.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

CHtmlgetViewer::CHtmlgetViewer(void)
{
}

CHtmlgetViewer::~CHtmlgetViewer(void)
{
}

long CHtmlgetViewer::get_viewer_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string his = "";
	lookitem(request_dto.szArgs, "his=", his);
	if (his == "view.clear")
	{
		writedb::Instance().del_log_viewer("", "");

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

	string path = "";
	string::size_type pos = request_dto.szArgs.find("path=");
	if (pos != string::npos)
	{
		path = request_dto.szArgs.substr(pos + 5);
		path = Replace(path, "%2B", " ");
	}

	string fname = "";
	pos = request_dto.szArgs.find("fname=");
	if (pos != string::npos)
	{
		fname = request_dto.szArgs.substr(pos + 6);
	}

	wstring path0 = request_dto.szFile;
	Replace(path0, L"\\", L"/");

	if (path0.empty())
	{
		string path2 = (path == "") ? fname : path;
		URLDecode2(path2);

		//import
		Replace(path2, ">", "#");

		if (!path2.empty())
		{
			path0 = _u2w(path2);
		}
	}

	string tpl = "";

	int idx = 0;
	//int exist = 0;
	string hsidata = "";
	string items = "";

	get_viewer_items(path0, items, idx);
	string index = Int2Str(idx);

	if (request_dto.szFile.empty() && his == "" && fname == "")
	{
		Htmlread(IDR_VIEW_HTML, L"html/_view.html", tpl);
		ReplaceOne(tpl, "{$album}", CDataSrv::Instance()._albumName);

		uint32_t r = CDataSrv::Instance()._readonly;
		ReplaceOne(tpl, "{$readonly}", uint2str(r));
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

		if (!path.empty())
		{
			ReplaceOne(tpl, "{$idx0}", index);
		}
		else {
			ReplaceOne(tpl, "{$idx0}", "-1");
		}

		set_viewer_data(items, hsidata, tpl);
		translate(tpl, "", "");
	}
	else
	{
		set_viewer_data("", hsidata, tpl);
		translate(hsidata, "", "");

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		writer.StartObject();
		writer.String("idx");
		writer.String(index.c_str());
		writer.String("htm");
		writer.String(items.c_str());
		writer.String("his");
		writer.String(hsidata.c_str());
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();
		return S_OK;
	}

	response_dto.szResponse = tpl;
	response_dto.szContentType = "text/html";

	return S_OK;
}


long CHtmlgetViewer::set_viewer_data(string items, string& history_file, string& tpl)
{
	list<string> lstFolders;
	list<LOG_VIEWER_DTO> lstViewers;
	string log_viewer_his = "";

	Cchatdal::Instance().get_log_folder(lstFolders);

	uint32_t r = CDataSrv::Instance()._readonly;
	if (r == 0)
	{
		Cchatdal::Instance().get_log_viewer("", "", "", lstViewers);
	}

	string history_file_tr = "";
	string history_folder = "";
	CLang::Instance().getOneData("history_file_tr", history_file_tr);
	CLang::Instance().getOneData("history_folder", history_folder);

	uint32_t i = 0;
	list<string>::iterator it;
	list<LOG_VIEWER_DTO>::iterator iter;
	for (it = lstFolders.begin(); it != lstFolders.end(); it++)
	{
		string currfolder = "";

		for (iter = lstViewers.begin(); iter != lstViewers.end(); iter++)
		{
			if (*it == iter->folder)
			{
				string szFile = iter->folder + "/" + iter->file;
				wstring wfile = _u2w(szFile);

				WIN32_FIND_DATA ffd;
				HANDLE hFind = FindFirstFile(wfile.c_str(), &ffd);
				if (hFind != INVALID_HANDLE_VALUE)
				{
					i++;
					string tem = history_file_tr;
					if (currfolder != iter->folder)
					{
						currfolder = iter->folder;
						ReplaceOne(tem, "{$history_folder}", history_folder);
					}
					else
					{
						ReplaceOne(tem, "{$history_folder}", "");
					}

					ReplaceOne(tem, "{$path}", iter->folder + "/" + iter->file);
					ReplaceOne(tem, "{$name}", uint2str(i) + ":" + iter->file);
					ReplaceOne(tem, "{$folder}", iter->folder + "/" + iter->file);
					ReplaceOne(tem, "{$title}", iter->file);
					ReplaceOne(tem, "{$title0}", iter->folder);

					log_viewer_his += tem;
				}
			}
		}
	}
	string history_file_clean = "";
	CLang::Instance().getOneData("history_file_clean", history_file_clean);

	string hisempty = "1";
	if (log_viewer_his != "")
	{
		hisempty = "0";
		log_viewer_his += history_file_clean;
	}

	history_file = "";
	CLang::Instance().getOneData("history_file", history_file);
	ReplaceOne(history_file, "{$history_file_tr}", log_viewer_his);

	ReplaceOne(tpl, "{$items}", items);
	ReplaceOne(tpl, "{$log_viewer_his}", history_file);

	ReplaceOne(tpl, "{$hisempty}", hisempty);
	return 0;
}

long CHtmlgetViewer::get_viewer_items(wstring path0, string& items, int& idx)
{
	wstring curr_path = L"";
	wstring curr_name = L"";

	wstring wtemp = path0;
	Replace(wtemp, L"\\", L"/");

	wchar_t buf[MAX_PATH] = { 0 };
	wcscpy_s(buf, MAX_PATH, wtemp.c_str());

	wchar_t* p = wcsrchr(buf, L'/');
	if (p != NULL)
	{
		curr_name = p + 1;
		p[0] = 0;

		curr_path = buf;
	}

	items = "<ul class=\"pictures\" style=\"display:none;\" >";

	_strPath = curr_path;
	_lstffd.clear();
	CHtmlgetFolder::Instance().GetFolderPics(0, _strPath, _lstffd);
	uint32_t len = (uint32_t)_lstffd.size();
	if (len > 0)
	{
		string templ = "";
		string templ_mp4 = "";
		CLang::Instance().getOneData("viewerjs_image_li_mp4", templ_mp4);
		CLang::Instance().getOneData("viewerjs_image_li_img", templ);

		std::wstring dstP = L"";

		wchar_t* szDocument;
		if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &szDocument)))
		{
			dstP = szDocument;

			wstring p = get_module_path(NULL);
			wstring::size_type pos = p.find(L"/windowsapps/photonest");
			if (pos != wstring::npos)
			{
				dstP += L"\\PhotoNest_App";
			}
			else
			{
				dstP += L"\\PhotoNest";
			}
			CreateDirectory(dstP.c_str(), NULL);

			dstP += L"\\Viewer";
			CreateDirectory(dstP.c_str(), NULL);
			dstP += L"\\thumb";
			CreateDirectory(dstP.c_str(), NULL);

			CoTaskMemFree(szDocument);
		}
		string ttt = _w2u(_strPath);
		uint32_t md5 = murmur3_32((unsigned char*)ttt.c_str(), (int)ttt.size(), 123456);

		dstP += L"\\" + _u2w(uint2str(md5));
		CreateDirectory(dstP.c_str(), NULL);

		if (dstP == L"")
		{
			dstP = get_module_path(NULL);
		}
		_dstP = dstP;

		uint32_t r = CDataSrv::Instance()._readonly;

		uint32_t i = 0;
		idx = -1;

		list<WIN32_FIND_DATA>::iterator it0;
		for (it0 = _lstffd.begin(); it0 != _lstffd.end(); it0++)
		{
			if (it0->cFileName == curr_name)
			{
				idx = i;
				if (r == 0)
				{
					time_t current_time;
					time(&current_time);

					LOG_VIEWER_DTO viewerDto;
					viewerDto.folder = _w2u(curr_path);
					viewerDto.file = _w2u(curr_name);
					viewerDto.lastmodified = static_cast<uint64_t>(current_time);
					writedb::Instance().add_log_viewer(viewerDto);
				}
			}

			string url = _w2u(L"./local_file?f=" + _strPath + L"/" + it0->cFileName);
			string url0 = _w2u(L"./local_thumb?f=" + _strPath + L"/" + it0->cFileName);


			wstring fname = it0->cFileName;
			wstring szDir = L"";
			wstring szName = L"";
			wstring ext = L"";
			splitFile(fname, szDir, szName, ext);
			transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

			string tmp = templ;
			if (ext == L"ogg" || ext == L"webm" || ext == L"mp3" || ext == L"mp4")
			{
				tmp = templ_mp4;
				ReplaceOne(tmp, "{$width}", "100");
				ReplaceOne(tmp, "{$height}", "100");
			}

			ReplaceOne(tmp, "{$duration}", "");

			Replace(tmp, "{$id}", uint2str(i));
			ReplaceOne(tmp, "{$rotate}", "0");
			ReplaceOne(tmp, "{$title}", _w2u(it0->cFileName));
			ReplaceOne(tmp, "{$url}", url);
			ReplaceOne(tmp, "{$url0}", url0);

			items += tmp;
			i++;
		}
	}

	items += "</ul>";

	return 0;
}

void CHtmlgetViewer::splitFile(wstring file_name, wstring& szDir, wstring& szName, wstring& szExt)
{
	wchar_t drive[_MAX_DRIVE];
	wchar_t direc[_MAX_DIR];
	wchar_t fname[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];

	_wsplitpath_s(file_name.c_str(), drive, direc, fname, ext);
	szName = fname;
	szDir = direc;

	if (wcslen(ext) > 0)
	{
		szExt = ext + 1;
	}
}

long CHtmlgetViewer::get_delbox(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string nb = "";
	string ids = "";
	string type = "";
	string idx = "";
	string artid = "";
	lookitem(request_dto.szArgs, "nb=", nb);
	lookitem(request_dto.szArgs, "ids=", ids);
	lookitem(request_dto.szArgs, "type=", type);
	lookitem(request_dto.szArgs, "idx=", idx);
	lookitem(request_dto.szArgs, "artid=", artid);

	string del_box = "";
	Htmlread(IDR_DELBOX_HTML, L"html/_del_box.html", del_box);
	if (type == "9")
	{
		Htmlread(IDR_DELBOX2_HTML, L"html/_del_box2.html", del_box);
	}
	else if (type == "0")
	{
		//ReplaceOne(del_box, "strawberry_jam", "newspaper");
	}
	else
	{
		ReplaceOne(del_box, "strawberry_jam", CDataSrv::Instance()._skin);
	}

	ReplaceOne(del_box, "{$total}", nb);
	ReplaceOne(del_box, "{$ids}", ids);
	ReplaceOne(del_box, "{$idx}", idx);
	ReplaceOne(del_box, "{$type}", type);
	ReplaceOne(del_box, "{$artid}", artid);

	wstring szFile = L"";
	if (type == "0")
	{
		int index = atoi(idx.c_str());

		int i = 0;
		list<WIN32_FIND_DATA>::iterator it0;
		for (it0 = _lstffd.begin(); it0 != _lstffd.end(); it0++)
		{
			if (index == i)
			{
				szFile = it0->cFileName;
				break;
			}
			i++;
		}
	}
	ReplaceOne(del_box, "{$filename}", _w2u(szFile));

	translate(del_box, "", "");
	response_dto.szContentType = "text/html";
	response_dto.szResponse = del_box;

	return S_OK;
}

long CHtmlgetViewer::del_viewer(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
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
	if (idx != "")
	{
		//viewer
		int index = atoi(idx.c_str());
		int len = (int)_lstffd.size();

		int next = index + 1;
		if (len == 1)
		{
			next = -1;
		}
		else if (index == len - 1)
		{
			next = 0;
		}

		int i = 0;
		list<WIN32_FIND_DATA>::iterator it0;
		for (it0 = _lstffd.begin(); it0 != _lstffd.end(); it0++)
		{
			if (next == i)
			{
				url = _w2u(_strPath + L"/" + it0->cFileName);
			}

			if (index == i)
			{
				wstring curr = _strPath + L"/" + it0->cFileName;

				my_DeleteFile(curr, 1);
				writedb::Instance().del_log_viewer(_w2u(_strPath), _w2u(it0->cFileName));
			}
			i++;
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

long CHtmlgetViewer::get_thumb_file(REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string szFile = "";
	string::size_type pos = request_dto.szArgs.find("f=");
	if (pos != string::npos)
	{
		szFile = request_dto.szArgs.substr(pos + 2);
		URLDecode2(szFile);
	}
	wstring src = _u2w(szFile);
	Replace(src, L"\\", L"/");

	wstring szDir = L"";
	wstring szName = L"";
	wstring ext = L"";

	wchar_t drive[_MAX_DRIVE];
	wchar_t direc[_MAX_DIR];
	wchar_t fname[_MAX_FNAME];
	wchar_t szExt[_MAX_EXT];

	_wsplitpath_s(src.c_str(), drive, direc, fname, szExt);
	szName = fname;
	szDir = direc;

	if (wcslen(szExt) > 0)
	{
		ext = szExt + 1;
	}

	transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

	wstring dstP = _dstP + L"\\";
	wstring dstName = szName + L"." + ext;

	if (ext == L"mp4")
	{
		ReplaceOne(dstName, L".mp4", L".png");
	}
	else if (ext == L"gif")
	{
		ReplaceOne(dstName, L".gif", L".png");
	}
	else if (ext == L"mp3")
	{
		ReplaceOne(dstName, L".mp3", L".png");
	}
	else if (ext == L"webm")
	{
		ReplaceOne(dstName, L".webm", L".png");
	}
	else if (ext == L"ogg")
	{
		ReplaceOne(dstName, L".ogg", L".png");
	}
	wstring dst = dstP + dstName;

	BOOL b = FALSE;
	DWORD dwAttr = GetFileAttributes(dst.c_str());
	if (dwAttr != -1)
	{
		// found something; better not be a folder....
		if (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY))
		{
			b = TRUE;
		}
	}
	wstring sample = get_module_path(NULL);

	wstring url0 = dstP + dstName;
	uint32_t outW = 100;
	uint32_t outH = 100;

	int ret = 0;

	if (ext == L"ogg")
	{
		if (!b)
		{
			ret = capture_ot(0, dst.c_str());
		}
	}
	else if (ext == L"webm")
	{
		if (!b)
		{
			ret = capture_ot(1, dst.c_str());
		}
	}
	else if (ext == L"mp3")
	{
		if (!b)
		{
			bool cover = false;
			long duration = 0;
			ret = mp3_cover(2, src.c_str(), dst.c_str(), duration, cover);
			if (ret != 0)
			{
				ret = capture_ot(0, dst.c_str());
			}
		}
		else
		{
			img_size(dst.c_str(), outW, outH);
		}
	}
	else if (ext == L"mp4")
	{
		if (!b)
		{
			uint32_t duration = 0;
			ret = capture_mp4(src.c_str(), dst.c_str(), outW, outH, duration);
			if (ret != 0)
			{
				ret = capture_ot(1, dst.c_str());
			}
		}
		else
		{
			img_size(dst.c_str(), outW, outH);
		}
	}
	else if (ext == L"gif")
	{
		if (!b)
		{
			ret = capture_gif(src.c_str(), dst.c_str(), outW, outH);
			if (ret != 0)
			{
				ret = capture_ot(2, dst.c_str());
			}
		}
	}
	else
	{
		if (!b)
		{
			string ext2 = "." + _w2u(ext);
			uint32_t level = 9;
			ret = resize_img(ext2.c_str(), src.c_str(), dst.c_str(), outW, outH, level);
			if (ret != 0)
			{
				ret = capture_ot(2, dst.c_str());
			}
		}
	}

	if (ret != 0)
	{
		url0 = sample + L"\\templ\\themes\\default\\images\\jpg.jpg";
	}

	response_dto.szResponse = _w2u(url0);
	response_dto.szContentType = "0";
	return 0;
}
