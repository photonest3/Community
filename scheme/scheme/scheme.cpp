// scheme.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "scheme.h"
#include "unitil.h"
#include "../../templ/templ/resource.h"
#include "../../photol/photol/photol.h"

#include "Htmlget.h"
#include "Htmlpost.h"
#include "UrlMap.h"
#include "lang.h"
#include "basedal.h"
#include "chatdal.h"
#include "DataSrv.h"
#include "MySoft.h"
#include <combaseapi.h>
#include "imlog.h"
#include "HtmlgetFolder.h"

#include "HtmlgetAdmin.h"
#include "HtmlgetIndexProc.h"
#include "uploadParser.h"

SCHEME_API long mygetfunc(const char* szUrl, char** szResponse, char** szMimeType)
{
	HRESULT hr = S_OK;
	REQUEST_DTO request_dto;
	RESPONSE_DTO response_dto;

	get_request_url(szUrl, request_dto.host, request_dto.szUrl, request_dto.szArgs);

	if (request_dto.szUrl == "c.html")
	{
		string act = "";
		lookitem(request_dto.szArgs, "act=", act);
		if (act == "c.init")
		{
			CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
			//c.html?act=init
			//curl_global_init(CURL_GLOBAL_DEFAULT);

			CUrlMap::Instance().Init_url_map();
			CHtmlgetAdmin::Instance().ParseLangXML();

		}
		else if (act == "c.transl")
		{
			string s2 = "";
			lookitem(request_dto.szArgs, "s2=", s2);
			string::size_type pos0 = request_dto.szArgs.find("s2=");
			if (pos0 != string::npos)
			{
				s2 = request_dto.szArgs.substr(pos0 + 3);
			}

			string szT = "{'" + s2 + "'}";
			CLang::Instance().translate(szT);

			int len = (int)szT.length() + 1;
			*szResponse = (char*)LocalAlloc(LMEM_ZEROINIT, len);
			strcpy_s(*szResponse, len, szT.c_str());
		}
		else if (act == "c.uinit")
		{
			if (CDataSrv::Instance()._uinit == 0)
			{
				CDataSrv::Instance()._uinit = 1;

				if (CDataSrv::Instance()._shutdownEvent != NULL)
				{
					WaitForSingleObject(CDataSrv::Instance()._shutdownEvent, INFINITE);
				}

				CoUninitialize();
			}
		}
		else if (act == "c.backup")
		{
			//c.html?act=backup
			//hr = Cbasedal::Instance().backupDb(CDataSrv::Instance()._albumName);
		}
		else if (act == "c.enckey")
		{
			//c.html?act=c.enckey
			string enckey = "";
			enckey = CUploadParser::Instance().GetKey();

			int len = (int)enckey.length() + 1;
			*szResponse = (char*)LocalAlloc(LMEM_ZEROINIT, len);
			strcpy_s(*szResponse, len, enckey.c_str());
		}
		else if (act == "c.register")
		{
			//c.html?act=register&s1=&s2=
			string enckey = "";
			string s2 = "";
			lookitem(request_dto.szArgs, "s2=", s2);
			if (s2 != "")
			{
				//register
				int mode = MODE_GALLERY;

				string::size_type pos = CDataSrv::Instance()._code.find(s2);
				if (pos != string::npos)
				{
					enckey = "inuse";
				}
				else
				{
					lic_header lic;
					int ret = CMySoft::Instance().Verify(s2, mode, lic);
					if (ret == 0)
					{
						CDataSrv::Instance().set_config_val("wo3", s2);

						if (CDataSrv::Instance().msix == 0)
						{
							string szSubKey = "Software\\OpenCV\\HighGUI\\Windows\\ZK2i3";
							CMySoft::Instance().add_lic(szSubKey, "wo3", s2);
						}

						CDataSrv::Instance()._code = s2;

						enckey = "0";
						CDataSrv::Instance()._ov = "2";
						CDataSrv::Instance()._first = lic.date;
					}
					else if (ret == ERROR_LIC)
					{
						enckey = "error";
					}
					else
					{
						//ERROR_OVERDUE
						enckey = "overdue";
					}
				}
			}

			int len = (int)enckey.length() + 1;
			*szResponse = (char*)LocalAlloc(LMEM_ZEROINIT, len);
			strcpy_s(*szResponse, len, enckey.c_str());
		}
		else if (act == "c.about")
		{
			//c.html?act=c.about
			string lic = "";
			string exp = "";

			if (CDataSrv::Instance()._albumName == "public")
			{
				lic = "{'It is free'}";
				CLang::Instance().translate(lic);
				exp = "";
			}
			else
			{
				if (CDataSrv::Instance()._code != "")
				{
					lic_header lic2;
					CMySoft::Instance().Reg(CDataSrv::Instance()._code, lic2);

					if (lic2.year >= 10)
					{
						lic = "{'Perpetual License'}";
						CLang::Instance().translate(lic);
						exp = "";
					}
					else
					{
						time_t t0 = lic2.date;

						if (lic2.year == 0)
						{
							t0 += 30 * EXP_SPAN;
							lic = "{'30 days trial period'}";
						}
						else
						{
							if (lic2.year == 1)
							{
								lic = "{'1-year license'}";
							}
							else
							{
								lic = "{'3-year license'}";
							}
						}
						CLang::Instance().translate(lic);
						if (lic2.year > 1)
						{
							ReplaceOne(lic, "3", Int2Str(lic2.year));
						}

						struct tm t;
						localtime_s(&t, &t0);
						if (lic2.year != 0)
						{
							t.tm_year += lic2.year;
						}

						char stamped[MAX_PATH] = { 0 };
						if (t.tm_year > 50)
						{
							strftime(stamped, MAX_PATH, "%Y/%m/%d", &t);
						}

						exp = "{'Expiration Date:'}";
						CLang::Instance().translate(exp);
						exp += " ";
						exp += stamped;
					}
				}
				else
				{
					time_t tfirst = CDataSrv::Instance()._first + 30 * EXP_SPAN;

					struct tm t;
					localtime_s(&t, &tfirst);

					char stamped[MAX_PATH] = { 0 };
					if (t.tm_year > 50)
					{
						strftime(stamped, MAX_PATH, "%Y/%m/%d", &t);
					}

					lic = "{'30 days trial period'}";
					CLang::Instance().translate(lic);

					exp = "{'Expiration Date:'}";
					CLang::Instance().translate(exp);
					exp += " ";
					exp += stamped;
				}
			}

			int len = (int)exp.length() + 1;
			*szMimeType = (char*)LocalAlloc(LMEM_ZEROINIT, len);
			strcpy_s(*szMimeType, 100, exp.c_str());

			len = (int)lic.length() + 1;
			*szResponse = (char*)LocalAlloc(LMEM_ZEROINIT, len);
			strcpy_s(*szResponse, len, lic.c_str());
		}
		else if (act == "c.foldersize")
		{
			uint64_t fsize = 0;

			string szPub = "-1";

			long ret = CHtmlgetAdmin::Instance().get_folder_sizes(1, fsize);
			if (ret == 0)
			{
				char buf[100] = { 0 };
				sprintf_s(buf, 100, "%llu", fsize);
				szPub = buf;
			}

			int len = (int)szPub.length() + 1;
			*szResponse = (char*)LocalAlloc(LMEM_ZEROINIT, len);
			strcpy_s(*szResponse, len, szPub.c_str());
		}
		else if (act == "c.paint")
		{
			//#define FROM_EXE 0
			//#define FROM_APP_NAV 1
			//#define FROM_APP_ONE 2
			//#define FROM_APP_VIEWER 3

			request_dto.szUrl = "/lgc_paint.html";
			long ret = CHtmlget::Instance().mygetfunc(request_dto, response_dto);
			if (ret == 0)
			{
				//szPub = LONG642Str(public_size);
				//szPrv = LONG642Str(private_size);
			}
		}
		else if (act == "c.nav")
		{
			string data = "";
			string id = "";
			lookitem(request_dto.szArgs, "id=", id);

			CHtmlgetIndexProc::Instance().get_nav_data(id, data);

			int len = (int)data.length() + 1;
			*szResponse = (char*)LocalAlloc(LMEM_ZEROINIT, len);
			strcpy_s(*szResponse, len, data.c_str());
		}
		else if (act == "c.syn_root")
		{
			string r = "";

			list<ROOT_DTO> lstRoot;
			CDataSrv::Instance().get_roots(lstRoot);
			if (!lstRoot.empty())
			{
				list<ROOT_DTO>::iterator it;
				for (it = lstRoot.begin(); it != lstRoot.end(); it++)
				{
					r += it->id + ";" + it->root_dir0 + "|";
				}
			}

			int len = (int)r.length() + 1;
			*szResponse = (char*)LocalAlloc(LMEM_ZEROINIT, len);
			strcpy_s(*szResponse, len, r.c_str());
		}
		else if (act == "c.reload_img")
		{
			CHtmlgetIndexProc::Instance().post_img_say(request_dto, response_dto);
		}
		else if (act == "c.nb")
		{
			CHtmlgetAdmin::Instance().viewer_nb();
		}
		else if (act == "c.alb")
		{
			string alb = "";
			string viewer = "";
			lookitem(request_dto.szArgs, "alb=", alb);
			lookitem(request_dto.szArgs, "viewer=", viewer);


			wstring p = get_module_path(NULL);
			wstring::size_type pos = p.find(L"/windowsapps/photonest");
			if (pos != wstring::npos)
			{
				CDataSrv::Instance().msix = 1;
			}
			else
			{
				CDataSrv::Instance().msix = 0;
			}

			int bAttr = 0;
			//if (CDataSrv::Instance().msix == 1) 
			{
				CHtmlgetFolder::Instance().CopyAppData(alb, CDataSrv::Instance().msix, bAttr);
			}

			string syn = "";
			CDataSrv::Instance().select_gallery(alb, viewer, act, syn);
			if (alb == "public")
			{
				CUploadParser::Instance().SetKey("123456");
			}

			string enckey = "123456";
			if (CDataSrv::Instance()._albumName == "private")
			{
				//CDataSrv::Instance().get_config_val("enckey", enckey);
			}
			else
			{
				set_key(enckey.c_str());
			}

			//if (bAttr == 1)
			//{
			//	time_t current_time;
			//	time(&current_time);

			//	char buf[100] = { 0 };
			//	sprintf_s(buf, 100, "%llu", (uint64_t)current_time);

			//	CDataSrv::Instance().set_config_val("i3", buf);
			//}

			int len = (int)syn.length() + 1;
			*szResponse = (char*)LocalAlloc(LMEM_ZEROINIT, len);
			strcpy_s(*szResponse, len, syn.c_str());
		}
		else if (act == "c.unins" || act == "c.uninsbefore" || act == "c.uninspass")
		{
			string alb = "";
			lookitem(request_dto.szArgs, "alb=", alb);
			if (alb != "")
			{
				//int ty = 1;
				string syn = "";
				CDataSrv::Instance().select_gallery(alb, "0", act, syn);

				bool b = false;
				if (alb == "public")
				{
					b = true;
					CUploadParser::Instance().SetKey("123456");

					if (act == "c.uninsbefore")
					{
						list<CATEGORY_DTO> lstCategory;
						Cchatdal::Instance().get_categories2("2", lstCategory);;

						if (lstCategory.size() > 5)
						{
							syn = "next";
							int len = (int)syn.length() + 1;
							*szResponse = (char*)LocalAlloc(LMEM_ZEROINIT, len);
							strcpy_s(*szResponse, len, syn.c_str());
							return S_OK;
						}
					}
				}
				else
				{
					string p = "";
					lookitem(request_dto.szArgs, "p=", p);

					list<USER_DTO> lstUsers;
					Cchatdal::Instance().get_users(lstUsers);

					if (act == "c.uninsbefore")
					{
						syn = "next";
						if (lstUsers.empty())
						{
							syn = "ok";
						}

						int len = (int)syn.length() + 1;
						*szResponse = (char*)LocalAlloc(LMEM_ZEROINIT, len);
						strcpy_s(*szResponse, len, syn.c_str());
						return S_OK;
					}

					if (act == "c.uninspass")
					{
						syn = "err";
						list<USER_DTO>::iterator I;
						for (I = lstUsers.begin(); I != lstUsers.end(); I++)
						{
							if (I->username == "admin" && I->password == p)
							{
								syn = "ok";
								break;
							}
						}
						int len = (int)syn.length() + 1;
						*szResponse = (char*)LocalAlloc(LMEM_ZEROINIT, len);
						strcpy_s(*szResponse, len, syn.c_str());
						return S_OK;
					}

					if (act == "c.unins")
					{
						list<USER_DTO>::iterator I;
						for (I = lstUsers.begin(); I != lstUsers.end(); I++)
						{
							if (I->username == "admin" && I->password == p)
							{
								string val = "";
								CDataSrv::Instance().get_config_val("enckey", val);
								CUploadParser::Instance().SetKey(val);
								b = true;
								break;
							}
						}
					}
				}

				syn = "err";
				if (b)
				{
					//string v30 = "";
					//CDataSrv::Instance().get_config_val("wo3", v30);

					CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
					CHtmlgetAdmin::Instance().unins();
					CoUninitialize();
					syn = "ok";
				}

				int len = (int)syn.length() + 1;
				*szResponse = (char*)LocalAlloc(LMEM_ZEROINIT, len);
				strcpy_s(*szResponse, len, syn.c_str());
			}
		}

		return S_OK;
	}

	if (CDataSrv::Instance()._albumName != "public" && CDataSrv::Instance()._albumName != "private")
	{
		return S_FALSE;
	}

	hr = CHtmlget::Instance().mygetfunc(request_dto, response_dto);
	if (hr == S_OK)
	{
		int len = (int)response_dto.szResponse.length() + 1;
		*szResponse = (char*)LocalAlloc(LMEM_ZEROINIT, len);
		strcpy_s(*szResponse, len, response_dto.szResponse.c_str());

		if (response_dto.szContentType == "")
		{
			response_dto.szContentType = "application/x-javascript";
		}

		len = (int)response_dto.szContentType.length() + 1;
		*szMimeType = (char*)LocalAlloc(LMEM_ZEROINIT, len);
		strcpy_s(*szMimeType, len, response_dto.szContentType.c_str());

		//string::size_type pos = request_dto.szUrl.find(".js");
		//string::size_type pos0 = request_dto.szUrl.find(".css");
		//if (pos == string::npos && pos0 == string::npos)
		//{
		//	Cimlog::Instance().LogToFile(response_dto.szResponse);
		//}
	}

	return hr;
}

SCHEME_API long mypostfunc(const char* szUrl, const char* szBody, const wchar_t* szFile, char** szResponse, char** szMimeType)
{
	REQUEST_DTO request_dto;
	RESPONSE_DTO response_dto;

	get_request_url(szUrl, request_dto.host, request_dto.szUrl, request_dto.szArgs);
	request_dto.szBody = szBody;
	request_dto.szFile = szFile;

	if (CDataSrv::Instance()._albumName != "public" && CDataSrv::Instance()._albumName != "private")
	{
		return S_FALSE;
	}

	HRESULT hr = CHtmlpost::Instance().mypostfunc(request_dto, response_dto);
	if (hr == S_OK)
	{
		int len = (int)response_dto.szResponse.length() + 1;
		*szResponse = (char*)LocalAlloc(LMEM_ZEROINIT, len);
		strcpy_s(*szResponse, len, response_dto.szResponse.c_str());

		if (response_dto.szContentType == "")
		{
			response_dto.szContentType = "application/x-javascript";
		}

		len = (int)response_dto.szContentType.length() + 1;
		*szMimeType = (char*)LocalAlloc(LMEM_ZEROINIT, len);
		strcpy_s(*szMimeType, len, response_dto.szContentType.c_str());
		//Cimlog::Instance().LogToFile(response_dto.szResponse);
	}

	return hr;
}
