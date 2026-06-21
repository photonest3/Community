
///////////////////////////////////////////////////////////////////////
/// @file scheme.cpp
/// @brief 自定义协议处理模块主文件（DLL 导出）
/// @details 实现 DLL 导出函数、GET/POST 请求入口、URL 路由初始化等功能
/// @author PhotoNest Team
/// @date 2024
/// @version 1.0
/// @note 本文件是 PhotoNest 自定义协议处理模块的核心入口
///////////////////////////////////////////////////////////////////////

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

// ------------------------------------------------------------
// DLL 导出：处理 GET 请求的统一入口
// szUrl      : 请求 URL（含 host、path、query string）
// szResponse : 输出响应正文（由调用方负责释放）
// szMimeType : 输出响应 Content-Type（由调用方负责释放）
// 返回值     : S_OK 成功，S_FALSE 失败或无权访问
// ------------------------------------------------------------
// 内部逻辑：
//   1. 解析 URL 拆出 host / path / args；
//   2. 若路径为 c.html，则按 act 参数分发控制命令；
//   3. 否则交由 CHtmlget 处理常规页面/资源请求。
// ------------------------------------------------------------
SCHEME_API long mygetfunc(const char* szUrl, char** szResponse, char** szMimeType)
{
	HRESULT hr = S_OK;
	REQUEST_DTO request_dto;
	RESPONSE_DTO response_dto;

	// 拆分 URL，提取 host、请求路径、查询参数
	get_request_url(szUrl, request_dto.host, request_dto.szUrl, request_dto.szArgs);

	// ============================================================
	// 控制命令入口：c.html?act=xxx
	// ============================================================
	if (request_dto.szUrl == "c.html")
	{
		string act = "";
		lookitem(request_dto.szArgs, "act=", act);

		// --------------------------------------------------------
		// act=c.init：初始化 COM 环境、URL 映射表及语言包
		// --------------------------------------------------------
		if (act == "c.init")
		{
			CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
			//c.html?act=init
			//curl_global_init(CURL_GLOBAL_DEFAULT);

			CUrlMap::Instance().Init_url_map();
			CHtmlgetAdmin::Instance().ParseLangXML();

		}
		// --------------------------------------------------------
		// act=c.transl：前端翻译接口
		// 从参数 s2 提取待翻译文本，包装为 {'text'} 后调用语言包翻译，
		// 将结果写入 szResponse 返回。
		// --------------------------------------------------------
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
		// --------------------------------------------------------
		// act=c.uinit：反初始化（卸载前调用）
		// 等待关机事件完成后释放 COM 环境，确保后台线程安全退出。
		// --------------------------------------------------------
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
		// --------------------------------------------------------
		// act=c.backup：数据库备份（当前已停用）
		// --------------------------------------------------------
		else if (act == "c.backup")
		{
			//c.html?act=backup
			//hr = Cbasedal::Instance().backupDb(CDataSrv::Instance()._albumName);
		}
		// --------------------------------------------------------
		// act=c.enckey：获取当前加密密钥
		// 返回上传解析器内部保存的密钥字符串。
		// --------------------------------------------------------
		else if (act == "c.enckey")
		{
			//c.html?act=c.enckey
			string enckey = "";
			enckey = CUploadParser::Instance().GetKey();

			int len = (int)enckey.length() + 1;
			*szResponse = (char*)LocalAlloc(LMEM_ZEROINIT, len);
			strcpy_s(*szResponse, len, enckey.c_str());
		}
		// --------------------------------------------------------
		// act=c.register：软件注册/授权验证
		// 参数 s2=注册码
		// 返回：
		//   "0"      — 注册成功；
		//   "inuse"  — 该注册码已在当前环境使用；
		//   "error"  — 注册码无效；
		//   "overdue"— 注册码已过期。
		// --------------------------------------------------------
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

				// 若当前已存在相同注册码，直接提示已使用
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
						// 校验通过：写入配置、写入注册表（非 MSIX 模式）、更新全局状态
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
		// --------------------------------------------------------
		// act=c.about：获取授权信息（用于关于对话框）
		// 输出：
		//   szResponse — 许可证类型描述（如永久授权、1 年授权、试用等）；
		//   szMimeType — 过期日期文本（空字符串表示不过期）。
		// --------------------------------------------------------
		else if (act == "c.about")
		{
			//c.html?act=c.about
			string lic = "";
			string exp = "";

			// public 相册为免费版，直接返回固定提示
			if (CDataSrv::Instance()._albumName == "public")
			{
				lic = "{'It is free'}";
				CLang::Instance().translate(lic);
				exp = "";
			}
			else
			{
				// 若已注册，解析注册码中的授权年限与起始日期
				if (CDataSrv::Instance()._code != "")
				{
					lic_header lic2;
					CMySoft::Instance().Reg(CDataSrv::Instance()._code, lic2);

					// year >= 10 视为永久授权
					if (lic2.year >= 10)
					{
						lic = "{'Perpetual License'}";
						CLang::Instance().translate(lic);
						exp = "";
					}
					else
					{
						time_t t0 = lic2.date;

						// year==0 表示试用 30 天
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
						// 将模板中的 "3" 替换为实际年限（若大于 1）
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
					// 未注册：按首次运行日期计算 30 天试用期
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
		// --------------------------------------------------------
		// act=c.foldersize：查询 public 文件夹占用大小
		// 返回字节数字符串，失败返回 "-1"。
		// --------------------------------------------------------
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
		// --------------------------------------------------------
		// act=c.paint：启动画图工具页面
		// 内部重定向到 /lgc_paint.html，由 CHtmlget 渲染返回。
		// --------------------------------------------------------
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
		// --------------------------------------------------------
		// act=c.nav：获取导航栏数据
		// 参数 id=导航节点标识，输出 JSON 格式导航数据。
		// --------------------------------------------------------
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
		// --------------------------------------------------------
		// act=c.syn_root：获取所有同步根目录列表
		// 返回格式：id;root_dir0|id;root_dir0|...
		// --------------------------------------------------------
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
		// --------------------------------------------------------
		// act=c.reload_img：触发图片重新加载/扫描
		// 调用索引处理器刷新图片状态。
		// --------------------------------------------------------
		else if (act == "c.reload_img")
		{
			CHtmlgetIndexProc::Instance().post_img_say(request_dto, response_dto);
		}
		// --------------------------------------------------------
		// act=c.nb：打开查看器（viewer）计数或新窗口
		// --------------------------------------------------------
		else if (act == "c.nb")
		{
			CHtmlgetAdmin::Instance().viewer_nb();
		}
		// --------------------------------------------------------
		// act=c.alb：切换相册（public / private）
		// 同时检测是否运行在 MSIX 容器环境，并复制必要的 AppData；
		// 设置对应加密密钥；最后返回同步状态。
		// --------------------------------------------------------
		else if (act == "c.alb")
		{
			string alb = "";
			string viewer = "";
			lookitem(request_dto.szArgs, "alb=", alb);
			lookitem(request_dto.szArgs, "viewer=", viewer);

			// 检测当前进程是否处于 MSIX 容器（Windows Apps）路径下
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
		// --------------------------------------------------------
		// act=c.unins / c.uninsbefore / c.uninspass：卸载/重置相册
		//  public 相册：直接执行卸载，若分类数大于 5 则先提示 next；
		//  private 相册：需要校验 admin 密码，通过后执行卸载。
		// --------------------------------------------------------
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

					// 卸载前检查：若 public 分类数量超过 5 个，提示需要二次确认（next）
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

					// 卸载前检查：private 无用户时直接通过；有用户时需要密码校验
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

					// 密码校验：遍历用户列表核对 admin + password
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

					// 执行卸载：校验通过后读取加密密钥，调用卸载逻辑
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

	// ============================================================
	// 常规页面/资源请求：交由 CHtmlget 处理
	// 若相册名称未初始化（非 public/private），直接拒绝访问
	// ============================================================
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

// ------------------------------------------------------------
// DLL 导出：处理 POST 请求的统一入口
// szUrl      : 请求 URL
// szBody     : POST 请求体（表单/JSON 数据）
// szFile     : 上传文件路径（宽字符，仅上传类请求有效）
// szResponse : 输出响应正文（由调用方负责释放）
// szMimeType : 输出响应 Content-Type（由调用方负责释放）
// 返回值     : S_OK 成功，S_FALSE 失败或无权访问
// ------------------------------------------------------------
// 流程：解析 URL → 填充 request_dto → 交由 CHtmlpost 处理 →
//       将响应结果写入 szResponse / szMimeType。
// ------------------------------------------------------------
SCHEME_API long mypostfunc(const char* szUrl, const char* szBody, const wchar_t* szFile, char** szResponse, char** szMimeType)
{
	REQUEST_DTO request_dto;
	RESPONSE_DTO response_dto;

	// 拆分 URL 并填充 POST 特有字段（body、上传文件路径）
	get_request_url(szUrl, request_dto.host, request_dto.szUrl, request_dto.szArgs);
	request_dto.szBody = szBody;
	request_dto.szFile = szFile;

	// 相册未初始化时拒绝服务
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
