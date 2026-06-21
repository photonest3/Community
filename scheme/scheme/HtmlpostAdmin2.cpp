
///////////////////////////////////////////////////////////////////////
/// @file HtmlpostAdmin2.cpp
/// @brief 管理界面 POST 请求处理实现文件（辅助文件）
/// @details 实现管理界面的辅助 POST 操作处理，包括文件删除、字段处理等
/// @author PhotoNest Team
/// @date 2024
/// @version 1.0
/// @note 本文件是 HtmlpostAdmin.cpp 的补充文件
///////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include "unitil.h"
#include "../../templ/templ/resource.h"

#include "lang.h"
#include "chatdal.h"
#include "writedb.h"

#include "DataSrv.h"
#include "HtmlgetAdmin.h"
#include "uploadParser.h"
#include "imlog.h"

//private
string  CHtmlgetAdmin::get_field_item(string  szBody, string name, string id)
{
	string val = "";
	string key = name + "-" + id + "=";
	lookitem(szBody, key.c_str(), val);

	return val;
}

long CHtmlgetAdmin::delete_image_files(string type, list<string> _lstFiles, list<string> _lstCoi)
{
	wstring root = L"";
	wstring krf = L"", krf3 = L"";
	wstring szPath = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);
	if (CDataSrv::Instance()._albumName == "private")
	{
		root = szPath + L"/data2";
		krf3 = L".krf";
	}
	else
	{
		root = szPath + L"/data1";
	}
	krf = L".krf";

	map<string, string> ::iterator I3;
	map<string, string> dir_map;

	list<ROOT_DTO> lstRoot;
	CDataSrv::Instance().get_roots(lstRoot);

	list<ROOT_DTO>::iterator it2;

	list<string>::iterator iter;
	for (iter = _lstFiles.begin(); iter != _lstFiles.end(); iter++)
	{
		wstring file = L"";
		file = root + _u2w(*iter);

		if (type == "0")
		{
			string::size_type pos = iter->find("/galleries_");
			if (pos == string::npos)
			{
				wstring dst = file;
				my_DeleteFile(dst);
			}
		}
		else if (type == "all")
		{
			string::size_type pos = iter->find("/galleries_");
			if (pos == string::npos)
			{
				wstring dst = file;
				my_DeleteFile(dst);
			}
			else
			{
				for (it2 = lstRoot.begin(); it2 != lstRoot.end(); it2++)
				{
					string id8 = "/galleries_" + it2->id + "/";
					pos = iter->find(id8);
					if (pos != string::npos)
					{
						string t = *iter;
						ReplaceOne(t, id8, "/");
						wstring dst = _u2w(it2->root_dir0 + t) + krf3;
						my_DeleteFile(dst, 1);
						break;
					}
				}
			}
		}

		wstring::size_type pos = file.find(L"/galleries_");
		if (pos == wstring::npos)
		{
			ReplaceOne(file, L"/_data/", L"/_thumb/");
		}
		else
		{
			ReplaceOne(file, L"/galleries_", L"/_thumb/galleries_");
		}

		ReplaceOne(file, L".webm", L".png");
		ReplaceOne(file, L".mp4", L".png");
		ReplaceOne(file, L".mp3", L".png");
		ReplaceOne(file, L".ogg", L".png");
		ReplaceOne(file, L".gif", L".png");
		wstring dst = file + krf;

		string::size_type n1 = dst.rfind(L"/");
		if (n1 != string::npos)
		{
			wstring temp = dst.substr(0, n1);
			dir_map.insert(map<string, string>::value_type(_w2u(temp), ""));

			wstring::size_type pos = file.find(L"/galleries_");
			if (pos == wstring::npos)
			{
				ReplaceOne(temp, L"/_thumb/", L"/_data/");
			}
			dir_map.insert(map<string, string>::value_type(_w2u(temp), ""));
		}

		my_DeleteFile(dst);

		dst = file + L"_2.png" + krf;
		my_DeleteFile(dst);
	}

	for (iter = _lstCoi.begin(); iter != _lstCoi.end(); iter++)
	{
		wstring dst = root + _u2w(*iter) + krf;
		my_DeleteFile(dst);
	}

	for (I3 = dir_map.begin(); I3 != dir_map.end(); I3++)
	{
		bool b = RemoveDirectory(_u2w(I3->first).c_str());
		if (b)
		{
			string temp = I3->first;

			for (;;)
			{
				string::size_type pos = temp.rfind("/");
				if (pos == string::npos)
				{
					break;
				}
				else
				{
					temp = temp.substr(0, pos);
					if (pos > 3)
					{
						b = RemoveDirectory(_u2w(temp).c_str());
						if (!b)
						{
							//Cimlog::Instance().LogToFile(temp);

						}
					}
				}
			}
		}
	}

	return 0;
}

long CHtmlgetAdmin::get_tagids(string tags, string& val)
{
	val = "";
	vector<string> v0 = tokenize(tags, ",", true, "");
	int len0 = (int)v0.size();
	for (int j = 0; j < len0; j++)
	{
		string tag = v0[j];
		string temp = tag;
		trim(temp, "~~");

		if (tag == "~~" + temp + "~~")
		{
			val += temp + ",";
		}
		else
		{
			time_t current_time;
			time(&current_time);

			esc_string(tag);
			trim(tag, " ");
			if (tag != "")
			{
				TAG_DTO dto;
				dto.id = "";
				dto.name = tag;
				dto.lastmodified = static_cast<uint64_t>(current_time);
				dto.nb_images = 0;

				uint32_t id = 0;
				writedb::Instance().add_tag(dto, id);
				if (id != 0)
				{
					val += uint2str(id) + ",";
				}
			}
		}
	}
	rtrim(val, ",");

	return 0;
}

long CHtmlgetAdmin::get_fullname(string full_id, list<CATEGORY_DTO>* ptrlstCategory, string& fullname)
{
	fullname = "";
	vector<string> v1 = tokenize(full_id, ",", true, "");
	int len = (int)v1.size();

	list<CATEGORY_DTO>::iterator it;

	for (int i = 0; i < len; i++)
	{
		for (it = ptrlstCategory->begin(); it != ptrlstCategory->end(); it++)
		{
			if (it->id == trim(v1[i], " "))
			{
				fullname += it->name + "/";
			}
		}
	}

	rtrim(fullname, "/");

	return 0;
}

long CHtmlgetAdmin::proc_login(const REQUEST_DTO request_dto)
{
	USER_DTO users_dto;
	string hint = "";

	lookitem(request_dto.szBody, "password=", users_dto.password);
	URLDecode(users_dto.password, 0);

	users_dto.username = "admin";

	bool b = false;
	list<USER_DTO> lstUsers;
	Cchatdal::Instance().get_users(lstUsers);
	list<USER_DTO>::iterator I;
	for (I = lstUsers.begin(); I != lstUsers.end(); I++)
	{
		hint = I->hint;

		if (I->username == users_dto.username && I->password == users_dto.password)
		{
			b = true;
			break;
		}
	}

	if (b)
	{
		string val = "";
		CDataSrv::Instance().get_config_val("enckey", val);
		CUploadParser::Instance().SetKey(val);

		return S_OK;
	}
	return S_FALSE;

}

long CHtmlgetAdmin::proc_registr(const REQUEST_DTO request_dto)
{
	string password_conf = "";
	USER_DTO users_dto;

	lookitem(request_dto.szBody, "password=", users_dto.password);
	lookitem(request_dto.szBody, "password_conf=", password_conf);
	lookitem(request_dto.szBody, "hint=", users_dto.hint);

	URLDecode(users_dto.password, 0);
	URLDecode(password_conf, 0);
	URLDecode(users_dto.hint, 1);
	users_dto.username = "admin";

	bool b = false;
	string enckey = "";
	string temp = users_dto.username + users_dto.password + "photonest";
	uint64_t md5sum = murmur3_32((unsigned char*)temp.c_str(), (int)temp.size(), 123456);
	enckey = uint64str(md5sum);

	writedb::Instance().add_users(users_dto);
	CDataSrv::Instance().set_config_val("enckey", enckey);
	CUploadParser::Instance().SetKey(enckey);
	CDataSrv::Instance().set_config_val("language", CLang::Instance()._language);
	CDataSrv::Instance().WritePrivate("language", CLang::Instance()._language.c_str());

	CLang::Instance().refresh_language();

	return S_OK;
}

long CHtmlgetAdmin::ParseLangXML()
{
	CLang::Instance().reset("templ");

	string szhtml = "";
	Htmlread(IDR_COMM_XML, L"/html/comm.xml", szhtml);
	CLang::Instance().ParseLangXML("templ", szhtml);

	Htmlread(IDR_INDEX_XML, L"/html/index.xml", szhtml);
	CLang::Instance().ParseLangXML("templ", szhtml);

	Htmlread(IDR_PICTURE_XML, L"/html/picture.xml", szhtml);
	CLang::Instance().ParseLangXML("templ", szhtml);

	Htmlread(IDR_ADMIN_XML, L"/html/admin.xml", szhtml);
	CLang::Instance().ParseLangXML("templ", szhtml);

	return 0;
}
