
///////////////////////////////////////////////////////////////////////
/// @file Htmlupload.cpp
/// @brief 文件上传处理实现文件
/// @details 实现图片文件上传、EXIF 读取、缩略图生成、数据库记录创建等功能
/// @author PhotoNest Team
/// @date 2024
/// @version 1.0
/// @note 本文件负责处理所有文件上传相关的操作
///////////////////////////////////////////////////////////////////////

﻿#include "StdAfx.h"
#include "../../templ/templ/resource.h"

#include "Htmlupload.h"
#include "Htmlexif.h"
#include "unitil.h"
#include <iomanip>

#include "lang.h"
#include "uploadParser.h"

#include "imagedal.h"
#include "chatdal.h"
#include "wsjosn.h"
#include "Datasrv.h"
#include "writedb.h"
#include "HtmlgetMenubar.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "../../photol/photol/photol.h"

#ifdef _DEBUG
#else
#pragma comment( lib, "../../../PhotoNest/tests/cefclient/Release/bin/photol.lib" ) 
#endif


#define MEDIAINFO_ADVANCED

using namespace rapidjson;

CHtmlupload::CHtmlupload(void)
{
	InitializeCriticalSection(&_cs);
}

CHtmlupload::~CHtmlupload(void)
{
	DeleteCriticalSection(&_cs);
}

long CHtmlupload::set_roots()
{
	EnterCriticalSection(&_cs);

	_lstMydir.clear();
	_lstRoot2.clear();
	list<ROOT_DTO> lst;
	Cchatdal::Instance().get_roots("", "", lst);
album_ = CDataSrv::Instance()._albumName;
	list<ROOT_DTO>::iterator it2;
	for (it2 = lst.begin(); it2 != lst.end(); it2++)
	{
		ROOT_DTO2 dto;
		dto.id = _u2w(it2->id);
		dto.root_dir0 = _u2w(it2->root_dir0);
		dto.root_sizes = it2->root_sizes;
		_lstRoot2.push_back(dto);
	}

	LeaveCriticalSection(&_cs);
	return S_OK;
}

long CHtmlupload::post_upload_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	HRESULT hr;

	string type = "";
	string cat_id = "";
	string fol = "";
	lookitem(request_dto.szArgs, "type=", type);
	lookitem(request_dto.szArgs, "category=", cat_id);
	lookitem(request_dto.szArgs, "fol=", fol);
	//string album = CDataSrv::Instance()._albumName;

	wstring szFile = request_dto.szFile;
	if (szFile == L"")
	{
		string file = "";
		string::size_type pos = request_dto.szBody.find("file=");
		if (pos != string::npos)
		{
			file = request_dto.szBody.substr(pos + 5);
		}

		if (file != "")
		{
			szFile = _u2w(file);
		}
	}

	Replace(szFile, L"\\", L"/");
	wstring szDir = L"";
	wstring szName = L"";
	wstring ext = L"";
	splitFile(szFile, szDir, szName, ext);

	transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

	time_t date_creation = 0;

	DWORD dw9 = 0;
	IMAGE_DTO imageDto;
	struct _stat32  info;
	int result = _wstat32(szFile.c_str(), &info);
	if (result == 0)
	{
		imageDto.date_creation_day = 0;
		imageDto.lastmodified = static_cast<uint64_t>(info.st_mtime);
		imageDto.date_creation = static_cast<uint64_t>(info.st_mtime);

		double d = info.st_size;
		d = d / 1024.0f;

		imageDto.quality = 0;
		imageDto.filesize = static_cast<uint32_t>(d * 10 / 1024.0f + 0.5f);

		if (imageDto.filesize == 0)
		{
			imageDto.filesize = 1;
		}

		if (ext == L"krf")
		{
			szFile = szFile.substr(0, szFile.length() - 4);
		}

		dw9 = (DWORD)info.st_size;
		if (album_ == "private" && ext != L"krf")
		{
			div_t div_result = div(dw9, 8);
			if (div_result.rem != 0)
			{
				dw9 += 8 - div_result.rem;
			}
			dw9 += 8;
		}
		imageDto.md5sum = dw9;
	}

	wstring newszFile = szFile;
	wstring dstkrf = szFile + L".krf";

	string existId = "0";

	imageDto.file = _w2u(szName + L"." + ext);

	if (type == "add")
	{
		EnterCriticalSection(&_cs);
		{
			string id5 = cat_id + "," + uint64str(imageDto.md5sum);
			list<string>::iterator it;
			for (it = _fileSet.begin(); it != _fileSet.end(); it++)
			{
				if (*it == id5)
				{
					existId = "1";
					break;
				}
			}

			if (existId == "0")
			{
				_fileSet.push_back(id5);
			}
		}
		LeaveCriticalSection(&_cs);

		if (existId == "0")
		{
			Cimagedal::Instance().exist_image(cat_id, imageDto.file, imageDto.md5sum, existId);
		}
	}
	else if (type == "fromviewer")
	{
		Cimagedal::Instance().exist_image(cat_id, imageDto.file, imageDto.md5sum, existId);
	}
	//else if (album_ == "private" && ext != L"krf")
	//{
	//	Cimagedal::Instance().exist_image(cat_id, imageDto.file, imageDto.md5sum, existId);
	//}

	int retCode = 0;

	bool iskrf = false;
	string msg = "";
	if (ext == L"krf")
	{
		iskrf = true;
		szDir = L"";
		szName = L"";
		ext = L"";
		splitFile(szFile, szDir, szName, ext);
		transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

		if (existId == "0")
		{
			wstring src = szFile + wstring(L".krf");
			int ret = CUploadParser::Instance().Decryptfile(src, szFile);
			if (ret == 0)
			{
				my_DeleteFile(src);
			}
			else
			{
				my_DeleteFile(szFile);
				msg = "{'Unable to decrypt krf file, key mismatch'|@translate}";
				retCode = 30;
			}
		}
	}

	/*
	if (existId != "0" && album_ == "private")
	{
		WIN32_FIND_DATA ffd;
		HANDLE hFind;
		dstkrf = szFile + L".krf";
		hFind = FindFirstFile(dstkrf.c_str(), &ffd);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			FindClose(hFind);
			wstring temp = L"";
			wstring ext0 = L"";
			string::size_type n1 = szFile.rfind(L".");
			if (n1 != string::npos)
			{
				temp = szFile.substr(0, n1);
				ext0 = szFile.substr(n1);
			}
			for (int i = 1; i < 100; i++)
			{
				wchar_t uid[MAX_PATH] = { 0 };
				swprintf_s(uid, MAX_PATH, L"%d", i);
				dstkrf = temp + L"-" + uid + ext0 + L".krf";
				hFind = FindFirstFile(dstkrf.c_str(), &ffd);
				if (hFind == INVALID_HANDLE_VALUE)
				{
					szName = szName + L"-" + uid;
					break;
				}
				else
				{
					FindClose(hFind);
				}
			}
		}

		string::size_type n1 = dstkrf.rfind(L".");
		if (n1 != string::npos)
		{
			newszFile = dstkrf.substr(0, n1);
		}
		imageDto.md5sum = dw9;
	}
	*/

	string err2 = "";
	if (type == "add" || type == "fromviewer")
	{
		if (existId != "0")
		{
			msg = "{'Duplicate file error.'|@translate}";
			retCode = 20;
		}

		if (imageDto.filesize > CDataSrv::Instance()._maxFileSize * 10)
		{
			CLang::Instance().getOneData("toolarge", msg);

			err2 = imageDto.file;
			err2 += ", " + uint2str(imageDto.filesize / 10) + "MB <br/>";

			string maxfilesize = "";
			CLang::Instance().getOneData("maxfilesize", maxfilesize);
			translate(maxfilesize, "", "");
			maxfilesize += ": " + uint2str(CDataSrv::Instance()._maxFileSize) + "MB";
			err2 += maxfilesize;
		}
	}

	if (msg != "")
	{
		if (fol == "")
		{
			translate(msg, "", "");

			StringBuffer sb;
			Writer<StringBuffer> writer(sb);

			if (iskrf)
			{
				szFile += L".krf";
			}

			writer.StartObject();
			writer.String("stat");
			writer.String("error");
			writer.String("message");
			writer.String(msg.c_str());
			writer.String("category");
			writer.String(cat_id.c_str());
			writer.String("file");
			writer.String(_w2u(szFile).c_str());
			writer.String("err2");
			writer.String(err2.c_str());

			writer.EndObject();
			response_dto.szContentType = "text/plain; charset=utf-8";
			response_dto.szResponse = sb.GetString();
		}
		return retCode;
	}

	uint32_t width = 0;
	uint32_t height = 0;
	uint32_t duration = 0;
	float longitude = 0;
	float latitude = 0;

	uint32_t orientation = 0;
	long mp4rotate = 1;

	wstring cover = L"";
	wstring fullname = L"";
	wstring upload_url = L"";
	wstring toname = L"";

	EnterCriticalSection(&_cs);
	CreateDirectoryFile(cat_id, type, newszFile, fullname, toname, upload_url);
	LeaveCriticalSection(&_cs);

	if (type == "add" || type == "fromviewer")
	{
		wstring fol = upload_url;
		if (fol.substr(0, 1) == L"/")
		{
			fol = fol.substr(1);
		}

		wstring az = L"";
		wstring::size_type pos = fol.find(L"/");
		if (pos != wstring::npos)
		{
			az = fol.substr(0, pos + 1);
		}

		ReplaceOne(fullname, L"/_thumb/", L"/_thumb/" + az);
	}

	string url = "";
	if (ext == L"mp3")
	{
		width = 144;
		height = 144;
		imageDto.width = width;
		imageDto.height = height;
		imageDto.duration = 0;
		imageDto.quality = 1;

		int ret = get_cover(szFile, szName, fullname, upload_url, url, imageDto);
		if (url == "")
		{
			url = CDataSrv::Instance()._mp3_url;
		}

		if (album_ == "private")
		{
			Encrypt_szFile(szFile, dstkrf);
		}
		duration = imageDto.duration;
	}
	else if (ext == L"mp4" || ext == L"webm" || ext == L"ogg")
	{
		wstring szImagePathName = fullname;

		if (ext == L"webm")
		{
			ReplaceOne(szImagePathName, L".webm", L".png");
		}
		else if (ext == L"ogg")
		{
			ReplaceOne(szImagePathName, L".ogg", L".png");
		}
		else
		{
			ReplaceOne(szImagePathName, L".mp4", L".png");
		}
		/*
				wstring src = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);
				if (album_ == "private")
				{
					src += L"/data2/_temp/";
				}
				else
				{
					src += L"/data1/_temp/";
				}
				src += _u2w(cat_id) + szName + L".png";
		*/
		//HRESULT ret = capture_mp4(szFile.c_str(), src.c_str(), width, height, duration);
		HRESULT ret = capture_mp4(szFile.c_str(), szImagePathName.c_str(), width, height, duration);
		if (ret == 0)
		{
			//CUploadParser::Instance().Encryptfile(src, szImagePathName + L".krf");
			//my_DeleteFile(src);

			//wstring dst2 = src + L"_2.png";
			//CUploadParser::Instance().Encryptfile(dst2, szImagePathName + L"_2.png.krf");
			//my_DeleteFile(dst2);
		}

		imageDto.quality = 1;
		imageDto.duration = duration;
		if (width != 0 && height != 0)
		{
			imageDto.width = width;
			imageDto.height = height;
		}
		else
		{
			imageDto.width = 144;
			imageDto.height = 144;
			imageDto.duration = 0;
		}

		if (album_ == "private")
		{
			Encrypt_szFile(szFile, dstkrf);
		}

		if (ret == 0)
		{
			string temp = _w2u(upload_url);
			url = _data2_thumb(temp);

			url = album_ + url;
			if (ext == L"webm")
			{
				ReplaceOne(url, ".webm", ".png");
			}
			else if (ext == L"ogg")
			{
				ReplaceOne(url, ".ogg", ".png");
			}
			else
			{
				ReplaceOne(url, ".mp4", ".png");
			}
		}
		else
		{
			if (ext == L"ogg")
			{
				url = CDataSrv::Instance()._mp3_url;
			}
			else
			{
				url = CDataSrv::Instance()._mp4_url;
				imageDto.representative_ext = url;
			}
		}
	}
	else if (ext == L"gif")
	{
		wstring szImagePathName = fullname;
		ReplaceOne(szImagePathName, L".gif", L".png");
		/*
				wstring src = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);
				if (album_ == "private")
				{
					src += L"/data2/_temp/";
				}
				else
				{
					src += L"/data1/_temp/";
				}
				src += _u2w(cat_id) + szName + L".png";
		*/
		imageDto.quality = 1;

		int ret = capture_gif(szFile.c_str(), szImagePathName.c_str(), width, height);
		if (ret == 0)
		{
			//CUploadParser::Instance().Encryptfile(src, szImagePathName + L".krf");
			//my_DeleteFile(src);

			//wstring dst2 = src + L"_2.png";
			//CUploadParser::Instance().Encryptfile(dst2, szImagePathName + L"_2.png.krf");
			//my_DeleteFile(dst2);

			imageDto.width = width;
			imageDto.height = height;
			imageDto.duration = 0;

			if (album_ == "private")
			{
				Encrypt_szFile(szFile, dstkrf);
			}
		}
		//else {
		//	ret = 0;
		//	string temp = _w2u(upload_url);
		//	url = _data2_thumb(temp);
		//	url = album_ + url;

		//	UPLOAD_DATA data;
		//	data.srcfile = szFile;
		//	data.width = 0;
		//	data.fullname = src;
		//	data.height = 0;
		//	data.krf = dstkrf;
		//	data.orientation = orientation;

		//	ProcThumbnailFunc(data, imageDto);

		//}

		if (ret != 0)
		{
			url = CDataSrv::Instance()._gif_url;
			imageDto.representative_ext = url;

			imageDto.width = 0;
			imageDto.height = 0;
		}
		else
		{
			string temp = _w2u(upload_url);
			url = _data2_thumb(temp);

			url = album_ + url;
			ReplaceOne(url, ".gif", ".png");
		}
	}
	else
	{
		string temp = _w2u(upload_url);
		url = _data2_thumb(temp);
		url = album_ + url;

		UPLOAD_DATA data;
		data.srcfile = szFile;
		data.width = 0;
		data.fullname = fullname;
		data.height = 0;
		data.krf = dstkrf;
		data.orientation = orientation;


		ProcThumbnailFunc(data, imageDto);

		EXIFInfo result;
		CHtmlexif::Instance().get_exif(szFile, result);

		longitude = static_cast<float>(result.GeoLocation.Longitude);
		latitude = static_cast<float>(result.GeoLocation.Latitude);

		char ref1 = result.GeoLocation.LonComponents.direction;
		char ref2 = result.GeoLocation.LonComponents.direction;
		orientation = result.Orientation;

		uint64_t timestamp = 0;
		if (result.DateTimeOriginal != "")
		{
			timestamp = convertDateTimeOriginalToUint64(result.DateTimeOriginal);
		}
		if (result.DateTimeDigitized != "" && timestamp == 0)
		{
			timestamp = convertDateTimeOriginalToUint64(result.DateTimeDigitized);
		}
		if (result.SubSecTimeOriginal != "" && timestamp == 0)
		{
			timestamp = convertDateTimeOriginalToUint64(result.SubSecTimeOriginal);
		}

		if (timestamp != 0)
		{
			date_creation = timestamp;
		}
	}


	//1
	time_t current_time;
	time(&current_time);

	imageDto.date_available = static_cast<uint64_t>(current_time);
	imageDto.name = _w2u(szName);
	imageDto.author = "";

	imageDto.date_available_day = 0;
	imageDto.ori = _w2u(upload_url);
	imageDto.storage_category_id = "";

	imageDto.added_by = CDataSrv::Instance().get_username();

	imageDto.orientation = orientation;
	imageDto.ext = _w2u(ext);
	imageDto.duration = duration;
	imageDto.longitude = longitude;
	imageDto.latitude = latitude;

	msg = "";
	err2 = imageDto.file;

	if (cat_id == "" || imageDto.width == 0 || imageDto.height == 0)
	{
		msg = "{'Filetype not allowed'|@translate}";
		retCode = 30;
	}

	if ((imageDto.width > 0 && imageDto.width <= 32) || (imageDto.height > 0 && imageDto.height <= 32))
	{
		CLang::Instance().getOneData("toosmall", msg);
		retCode = 40;
	}

	if (date_creation > 0)
	{
		imageDto.date_creation = static_cast<uint64_t>(date_creation);
	}

	if (imageDto.height > 0)
	{
		float ratio = static_cast<float>(imageDto.width) / static_cast<float>(imageDto.height);
		imageDto.ratio = floor(ratio * 100) / 100;
	}
	else
	{
		imageDto.ratio = 1;
	}

	if (type == "add" || type == "fromviewer")
	{
		if (msg == "")
		{
			CopyFile(szFile.c_str(), toname.c_str(), TRUE);
		}
	}

	uint32_t id = 0;
	if (msg != "")
	{
		id = 1;
		if (type == "")
		{
			imageDto.md5sum = dw9;

			MD5_DTO mdto;
			mdto.pic_id = "";
			mdto.lastmodified = imageDto.lastmodified;
			mdto.cat_id = "";
			mdto.path = imageDto.file;
			mdto.md5sum = imageDto.md5sum;
			mdto.msg = msg;
			hr = writedb::Instance().add_imgerr(cat_id, mdto, id);
		}
	}
	else
	{
		hr = writedb::Instance().add_image(cat_id, imageDto, CDataSrv::Instance()._time_zone_bias, id);
	}

	if (id == 0)
	{
		msg = "{'Filetype not allowed'|@translate}";
		retCode = 30;
	}

	if (msg != "")
	{
		if (fol == "")
		{
			translate(msg, "", "");
			string err = "error";

			StringBuffer sb;
			Writer<StringBuffer> writer(sb);
			writer.StartObject();
			writer.String("stat");
			writer.String(err.c_str());
			writer.String("message");
			writer.String(msg.c_str());
			writer.String("category");
			writer.String(cat_id.c_str());
			writer.String("file");
			writer.String(_w2u(szFile).c_str());
			writer.String("err2");
			writer.String(err2.c_str());
			writer.EndObject();
			response_dto.szContentType = "text/plain; charset=utf-8";
			response_dto.szResponse = sb.GetString();
		}
		return retCode;
	}

	if (fol == "")
	{
		RetImageDto dto2;
		dto2.image_id_ = uint2str(id);
		dto2.src_ = url;

		dto2.ori_ = url;
		dto2.name_ = _w2u(szName);
		dto2.fullname_ = _w2u(szFile);
		dto2.category_ = cat_id;
		dto2.duration_ = get_dur_string(duration);
		dto2.ext_ = _w2u(ext);

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.String("category");
		writer.String(cat_id.c_str());
		writer.String("result");
		dto2.Serialize(writer);
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();
	}

	return 0;
}

//private
void CHtmlupload::mk_dir(wstring path, wstring src)
{
	wchar_t szBuf[MAX_PATH] = { 0 };

	wstring::size_type pre_index = 0, index = 0, len = 0;
	while ((index = src.find_first_of(L"/", pre_index)) != wstring::npos)
	{
		if ((len = index - pre_index) != 0)
		{
			wstring fol = src.substr(pre_index, len);
			if (fol.substr(0, 1) == L"/")
			{
				fol = fol.substr(1);
			}
			path += L"/" + fol;

			memset(szBuf, 0, sizeof(wchar_t) * MAX_PATH);
			DWORD dwSize = GetFullPathName(path.c_str(), MAX_PATH, szBuf, NULL);
			CreateMyDirectory(szBuf);

		}
		pre_index = index + 1;
	}

}

long CHtmlupload::set_category(string cat_id)
{
	list<CATEGORY_DTO> lst;
	CHtmlgetMenubar::Instance().get_categories(cat_id, lst);

	CATEGORY_DTO cur = lst.front();

	CATEGORY_DTO2 dto2;
	dto2.id = cur.id;
	dto2.root_id = _u2w(cur.root_id);
	dto2.dir0 = _u2w(cur.dir0);
	dto2.root_dir0 = _u2w(cur.root_dir0);

	my_tolower(dto2.dir0);
	my_tolower(dto2.root_dir0);

	_lstCategory.clear();
	_lstCategory.push_back(dto2);

	return 0;
}

//private
BOOL CHtmlupload::CreateDirectoryFile(string cat_id, string type, wstring szFile, wstring& fullname, wstring& toname, wstring& upload_url)
{
	wstring folder = L"";

	wchar_t drive[_MAX_DRIVE];
	wchar_t direc[_MAX_DIR];
	wchar_t fname[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];

	_wsplitpath_s(szFile.c_str(), drive, direc, fname, ext);

	wstring szDir = direc;
	wstring szPath = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);
	//string album = CDataSrv::Instance()._albumName;

	wstring dDir = (album_ == "private") ? L"/data2" : L"/data1";
	wstring path0 = szPath + dDir + L"/_thumb";

	wchar_t szBuf[MAX_PATH] = { 0 };

	list<wstring>::iterator it;

	bool b = false;
	for (it = _lstMydir.begin(); it != _lstMydir.end(); it++)
	{
		if (_wcsicmp(it->c_str(), path0.c_str()) == 0)
		{
			b = true;
			break;
		}
	}
	if (!b)
	{
		_lstMydir.push_back(path0);
		DWORD dwSize = GetFullPathName(path0.c_str(), MAX_PATH, szBuf, NULL);
		CreateMyDirectory(szBuf);
	}

	path0 = szPath + dDir + L"/_temp";

	b = false;
	for (it = _lstMydir.begin(); it != _lstMydir.end(); it++)
	{
		if (_wcsicmp(it->c_str(), path0.c_str()) == 0)
		{
			b = true;
			break;
		}
	}
	if (!b)
	{
		_lstMydir.push_back(path0);
		memset(szBuf, 0, sizeof(wchar_t) * MAX_PATH);
		DWORD dwSize = GetFullPathName(path0.c_str(), MAX_PATH, szBuf, NULL);
		CreateMyDirectory(szBuf);
	}

	wstring src = direc;

	wstring fpath = L"";
	wstring synroot = L"";

	if (type == "")
	{
		wstring t0 = direc;
		t0 += L"/";

		list<ROOT_DTO2>::iterator it2;
		for (it2 = _lstRoot2.begin(); it2 != _lstRoot2.end(); it2++)
		{
			wstring id8 = L"/galleries_" + it2->id + L"/";
			wstring root = it2->root_dir0.substr(2) + L"/";

			if (_wcsnicmp(t0.c_str(), root.c_str(), root.size()) == 0)
			{
				fpath = id8;
				synroot = root;
				break;
			}
		}
	}
	else
	{
		wstring src2 = L"";
		wstring src3 = L"";

		if (_lstCategory.empty())
		{
			set_category(cat_id);
		}

		CATEGORY_DTO2 cur = _lstCategory.front();
		if (cur.id != cat_id)
		{
			set_category(cat_id);
			cur = _lstCategory.front();
		}

		{
			wstring tt = cur.dir0;
			wstring root = cur.root_dir0.substr(2);

			string::size_type pos0 = tt.find(root);
			if (pos0 != string::npos)
			{
				tt = tt.substr(pos0 + root.length());
			}
			src = tt + L"/";
			src2 = cur.dir0 + L"/";

			wstring tt3 = cur.dir0;
			wstring tmp = szPath + dDir + L"/_data";
			tt3 = ReplaceOne(tt3, tmp, L"");

			src3 = tt3 + L"/";
		}

		wstring t2 = L"galleries_" + cur.root_id;
		wstring path = szPath + dDir + L"/_thumb/" + t2;

		b = false;
		for (it = _lstMydir.begin(); it != _lstMydir.end(); it++)
		{
			if (_wcsicmp(it->c_str(), path.c_str()) == 0)
			{
				b = true;
				break;
			}
		}
		if (!b)
		{
			memset(szBuf, 0, sizeof(wchar_t) * MAX_PATH);
			DWORD dwSize = GetFullPathName(path.c_str(), MAX_PATH, szBuf, NULL);
			CreateMyDirectory(szBuf);
			_lstMydir.push_back(path);
		}

		b = false;
		for (it = _lstMydir.begin(); it != _lstMydir.end(); it++)
		{
			if (_wcsnicmp(it->c_str(), src.c_str(), src.size()) == 0)
			{
				b = true;
				break;
			}
		}
		if (!b)
		{
			_lstMydir.push_back(src);
			mk_dir(path, src);

			wstring path2 = szPath + dDir + L"/_data/";
			mk_dir(path2, src3);
		}

		fullname = szPath + dDir + L"/_thumb" + src + fname + ext;

		toname = src2 + fname + ext;
		src = L"/" + t2 + src;
		upload_url = src;
		upload_url = upload_url + fname + ext;

		//E:/voy/102/PhotoNest/tests/cefclient/Release/data1/_thumb/aaaaaa/bbbbbbbbbbbbbb/IMG_20230706_151100.jpg
		///galleries_2/aaaaaa/bbbbbbbbbbbbbb/IMG_20230706_151100.jpg
	}

	if (fpath != L"")
	{
		wstring path = szPath + dDir + L"/_thumb";
		src = fpath + src.substr(synroot.size());

		b = false;
		for (it = _lstMydir.begin(); it != _lstMydir.end(); it++)
		{
			if (_wcsnicmp(it->c_str(), src.c_str(), src.size()) == 0)
			{
				b = true;
				break;
			}
		}
		if (!b)
		{
			_lstMydir.push_back(src);
			mk_dir(path, src);

		}

		fullname = szPath + dDir + L"/_thumb" + src + fname + ext;

		upload_url = src;
		upload_url = upload_url + fname + ext;
	}

	return 0;
}

void CHtmlupload::ProcThumbnailFunc(UPLOAD_DATA data, IMAGE_DTO& imageDto)
{
	wstring szImagePathName = data.fullname;

	wchar_t src[MAX_PATH] = { 0 };
	DWORD dwSize = GetFullPathName(data.srcfile.c_str(), MAX_PATH, src, NULL);

	wchar_t dst[MAX_PATH] = { 0 };
	dwSize = GetFullPathName(szImagePathName.c_str(), MAX_PATH, dst, NULL);

	int ftype = 0;

	wchar_t dst2[MAX_PATH] = { 0 };
	szImagePathName += L"_2.png";
	dwSize = GetFullPathName(szImagePathName.c_str(), MAX_PATH, dst2, NULL);

	wstring szDir = L"";
	wstring szName = L"";
	wstring ext = L"";
	splitFile(src, szDir, szName, ext);
	transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

	string ext2 = "." + _w2u(ext);
	uint32_t level = 0;

	int ret = resize_img(ext2.c_str(), src, dst, imageDto.width, imageDto.height, level);
	if (ret == 0)
	{
		if (imageDto.width != 0 && imageDto.height != 0)
		{
			imageDto.quality = level;

			//wstring krf = dst;
			//krf += L".krf";
			//CUploadParser::Instance().Encryptfile(dst, krf.c_str());
			//my_DeleteFile(dst);

			//wstring krf2 = dst2;
			//krf2 += L".krf";
			//CUploadParser::Instance().Encryptfile(dst2, krf2.c_str());
			//my_DeleteFile(dst2);

			if (album_ == "private")
			{
				Encrypt_szFile(src, data.krf);
			}
		}
	}
}

void CHtmlupload::splitFile(wstring file_name, wstring& szDir, wstring& szName, wstring& szExt)
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

BOOL CHtmlupload::CreateMyDirectory(wstring path)
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

long CHtmlupload::get_cover(wstring szFile, wstring szName, wstring fullname, wstring upload_url, string& url, IMAGE_DTO& imageDto)
{
	/*
		wstring dst = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);
		if (album_ == "private")
		{
			dst += L"/data2/_temp/";
		}
		else
		{
			dst += L"/data1/_temp/";
		}
		dst += szName + L".png";
	*/
	wstring szImagePathName = fullname;
	ReplaceOne(szImagePathName, L".mp3", L".png");

	bool cover = false;
	long duration = 0;
	int ret = mp3_cover(0, szFile.c_str(), szImagePathName.c_str(), duration, cover);
	if (ret == 0)
	{
		imageDto.duration = duration;

		if (cover)
		{
			//wstring szImagePathName = fullname;
			//ReplaceOne(szImagePathName, L".mp3", L".png");

			//CUploadParser::Instance().Encryptfile(dst, szImagePathName + L".krf");
			//my_DeleteFile(dst);

			//wstring dst2 = dst + L"_2.png";
			//CUploadParser::Instance().Encryptfile(dst2, szImagePathName + L"_2.png.krf");
			//my_DeleteFile(dst2);

			string temp = _w2u(upload_url);
			url = _data2_thumb(temp);

			url = album_ + url;
			ReplaceOne(url, ".mp3", ".png");
			imageDto.representative_ext = url;
		}
	}
	return ret;
}

long CHtmlupload::Encrypt_szFile(wstring szFile, wstring krf)
{
	//WIN32_FIND_DATA ffd;
	//HANDLE hFind;

	CUploadParser::Instance().Encryptfile(szFile, krf.c_str());

	//hFind = FindFirstFile(szFile.c_str(), &ffd);
	//if (hFind != INVALID_HANDLE_VALUE)
	//{
	//	FindClose(hFind);
	//	HANDLE hWrite = CreateFile(krf.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
	//		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	//	if (hWrite != INVALID_HANDLE_VALUE)
	//	{
	//		::SetFileTime(hWrite, &ffd.ftCreationTime, &ffd.ftLastAccessTime, &ffd.ftLastWriteTime);
	//		CloseHandle(hWrite);
	//	}
	//}
	my_DeleteFile(szFile);

	return 0;
}

uint64_t CHtmlupload::convertDateTimeOriginalToUint64(const std::string& dateTimeOriginal)
{
	istringstream ss(dateTimeOriginal);
	tm time_info = {};

	ss >> get_time(&time_info, "%Y:%m:%d %H:%M:%S");

	time_t timestamp = mktime(&time_info);
	if (timestamp == -1) {
		return 0;
	}

	return static_cast<uint64_t>(timestamp);
}
