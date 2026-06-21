#include "StdAfx.h"
#include "unitil.h"
#include <vector>
#include <string>

#include "../../templ/templ/resource.h"
#include "imagedal.h"
#include "chatdal.h"
#include "DataSrv.h"
#include "lang.h"
#include "HtmlgetAdmin.h"
#include "Blowfish.h"
#include "HtmlgetFolder.h"
#include "Htmlupload.h"
#include "uploadParser.h"
#include "writedb.h"
#include "../../photol/photol/photol.h"
#include "HtmlgetMenubar.h"
#include "HtmlgetIndexProc.h"
#include <process.h>
#include <thread>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

void CHtmlgetAdmin::galleries_addfolder(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string folder = "";
	string::size_type pos = request_dto.szArgs.find("folder=");
	if (pos != string::npos)
	{
		folder = request_dto.szArgs.substr(pos + 7);
	}
	URLDecode2(folder);

	//string firstrun = "";
	//lookitem(request_dto.szArgs, "firstrun=", firstrun);
	if (folder != "")
	{
		_trs = "";

		uint32_t id = 0;
		ROOT_DTO root;
		root.root_dir0 = folder;
		root.root_sizes = 0;
		writedb::Instance().add_root(root, id);

		EnterCriticalSection(&_nbcs);
		_mapSizes.clear();
		LeaveCriticalSection(&_nbcs);

		_beginthreadex(NULL, 0, sb_nb, this, NULL, NULL);
	}

	StringBuffer sb;
	Writer<StringBuffer> writer(sb);
	writer.StartObject();
	writer.String("stat");
	writer.String("ok");
	writer.EndObject();

	response_dto.szResponse = sb.GetString();
	response_dto.szContentType = "text/html";
}

void CHtmlgetAdmin::galleries_remove(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	_trs = "";
	_brisquing++;

	string first = "";
	string rootid = "";
	lookitem(request_dto.szArgs, "first=", first);
	lookitem(request_dto.szBody, "rootid=", rootid);

	string root_dir = "";

	list<ROOT_DTO> lstRoot;
	CDataSrv::Instance().get_roots(lstRoot);

	list<ROOT_DTO>::iterator it2;
	for (it2 = lstRoot.begin(); it2 != lstRoot.end(); it2++)
	{
		if (it2->id == rootid)
		{
			root_dir = it2->root_dir0;
			break;
		}
	}

	list<CATEGORY_DTO> lst;
	CHtmlgetMenubar::Instance().clear();
	CHtmlgetMenubar::Instance().get_categories_order_by_rank(0, lst);

	remove_root(first, lst, rootid, root_dir);

	wstring r_dir0 = _u2w(root_dir);
	EnterCriticalSection(&_nbcs);
	{
		int len0 = (int)r_dir0.size();
		list<FILE_DTO>::iterator it9;

		if (!_lstDbPending.empty())
		{
			for (it9 = _lstDbPending.begin(); it9 != _lstDbPending.end(); )
			{
				if (_wcsnicmp(it9->filename.c_str(), r_dir0.c_str(), len0) == 0)
				{
					it9 = _lstDbPending.erase(it9);
				}
				else
				{
					it9++;
				}
			}
		}

		if (!_lstFsPending.empty())
		{
			for (it9 = _lstFsPending.begin(); it9 != _lstFsPending.end(); )
			{
				if (_wcsnicmp(it9->filename.c_str(), r_dir0.c_str(), len0) == 0)
				{
					it9 = _lstFsPending.erase(it9);
				}
				else
				{
					it9++;
				}
			}
		}

		if (!_lstState.empty())
		{
			list<FOLDER_STATE_DTO>::iterator it3;
			for (it3 = _lstState.begin(); it3 != _lstState.end(); )
			{
				if (_strnicmp(it3->path.c_str(), root_dir.c_str(), len0) == 0)
				{
					it3 = _lstState.erase(it3);
				}
				else
				{
					it3++;
				}
			}
		}
	}
	LeaveCriticalSection(&_nbcs);

	StringBuffer sb;
	Writer<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("stat");
	writer.String("ok");
	writer.EndObject();

	response_dto.szResponse = sb.GetString();
	response_dto.szContentType = "text/plain; charset=utf-8";
}

long CHtmlgetAdmin::remove_root(string first, list<CATEGORY_DTO> lst, string root_id, string root_dir)
{
	wstring root = L"";
	wstring thumb = L"";
	wstring szPath = CDataSrv::Instance()._appDataDir;// get_module_path(NULL);
	if (CDataSrv::Instance()._albumName == "private")
	{
		thumb = szPath + L"/data2";
	}
	else
	{
		thumb = szPath + L"/data1";
	}
	root = _u2w(root_dir);

	thumb += L"/_thumb/galleries_" + _u2w(root_id) + L"/";

	while (!_queDecrypt.empty())
	{
		_queDecrypt.pop();
	}

	while (!_queThumb.empty())
	{
		_queThumb.pop();
	}

	list<wstring>::iterator it;
	if (CDataSrv::Instance()._albumName == "private")
	{
		if (first == "")
		{
			list<wstring> lstDir;
			lstDir.push_back(root);
			CHtmlgetFolder::Instance().GetSubFolders(1, root, lstDir);

			for (it = lstDir.begin(); it != lstDir.end(); it++)
			{
				CHtmlgetFolder::Instance().GetFolderPics(it->c_str(), _queDecrypt);
			}
		}
	}

	_lstDir.clear();
	_lstDir.push_back(thumb);
	CHtmlgetFolder::Instance().GetSubFolders(1, thumb, _lstDir);
	for (it = _lstDir.begin(); it != _lstDir.end(); it++)
	{
		_queThumb.push(*it);
	}

	_progress_len = (int)_queDecrypt.size();
	_progress_len += (int)_queThumb.size();
	_progress_step = 0;

	if (_progress_len > _progress_step)
	{
		for (int i = 0; i < _cpu; i++)
		{
			_beginthreadex(NULL, 0, sb_decrypt, this, NULL, NULL);
		}

		if (first != "")
		{
			for (;;)
			{
				if (_progress_step >= _progress_len)
				{
					break;
				}
				this_thread::sleep_for(chrono::nanoseconds(100));
			}
		}
	}

	if (first != "")
	{
		processok(root_id);
	}

	return 0;
}

long CHtmlgetAdmin::unins()
{
	wstring root = L"";
	wstring thumb = L"";
	wstring szPath = CDataSrv::Instance()._appDataDir;// get_module_path(NULL);
	if (CDataSrv::Instance()._albumName == "private")
	{
		thumb = szPath + L"/data2";
	}
	else
	{
		thumb = szPath + L"/data1";
	}

	while (!_queDecrypt.empty())
	{
		_queDecrypt.pop();
	}

	while (!_queThumb.empty())
	{
		_queThumb.pop();
	}

	_lstDir.clear();

	list<ROOT_DTO>::iterator it2;
	list<ROOT_DTO> lstRoot;
	CDataSrv::Instance().get_roots(lstRoot);
	for (it2 = lstRoot.begin(); it2 != lstRoot.end(); it2++)
	{
		if (!it2->root_dir0.empty())
		{
			root = _u2w(it2->root_dir0);

			list<wstring>::iterator it;
			if (CDataSrv::Instance()._albumName == "private")
			{
				list<wstring> lstDir;
				lstDir.push_back(root);
				CHtmlgetFolder::Instance().GetSubFolders(1, root, lstDir);

				for (it = lstDir.begin(); it != lstDir.end(); it++)
				{
					CHtmlgetFolder::Instance().GetFolderPics(it->c_str(), _queDecrypt);
				}
			}

			wstring t3 = thumb + L"/_thumb/galleries_" + _u2w(it2->id) + L"/";
			_lstDir.push_back(t3);
			CHtmlgetFolder::Instance().GetSubFolders(1, t3, _lstDir);
			for (it = _lstDir.begin(); it != _lstDir.end(); it++)
			{
				_queThumb.push(*it);
			}
		}
	}

	_progress_len = (int)_queDecrypt.size();
	_progress_len += (int)_queThumb.size();
	_progress_step = 0;

	if (_progress_len > _progress_step)
	{
		for (int i = 0; i < _cpu; i++)
		{
			_beginthreadex(NULL, 0, sb_decrypt, this, NULL, NULL);
		}

		for (;;)
		{
			if (_progress_step >= _progress_len)
			{
				break;
			}
			this_thread::sleep_for(chrono::nanoseconds(100));
		}
	}

	list<wstring>::reverse_iterator it0;
	for (it0 = _lstDir.rbegin(); it0 != _lstDir.rend(); it0++)
	{
		RemoveDirectory(it0->c_str());
	}

	writedb::Instance().unins();

	writedb::Instance().del_users();
	CDataSrv::Instance().set_config_val("last_import", "");
	if (CDataSrv::Instance()._albumName == "private")
	{
		CDataSrv::Instance().set_config_val("enckey", "");
	}

	return 0;
}

void CHtmlgetAdmin::galleries_restore(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	_trs = "";
	string stat = "err";

	list<string> fol;
	get_restore(fol);

	if (!fol.empty())
	{
		list<string>::iterator it;
		for (it = fol.begin(); it != fol.end(); it++)
		{
			string folder = *it;

			uint32_t id = 0;
			ROOT_DTO root;
			root.root_dir0 = folder;
			root.root_sizes = 0;
			writedb::Instance().add_root(root, id);
		}
		stat = "ok";

		EnterCriticalSection(&_nbcs);
		_mapSizes.clear();
		LeaveCriticalSection(&_nbcs);

		_beginthreadex(NULL, 0, sb_nb, this, NULL, NULL);
	}

	StringBuffer sb;
	Writer<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("stat");
	writer.String(stat.c_str());

	writer.EndObject();

	response_dto.szResponse = sb.GetString();
	response_dto.szContentType = "text/html";
}

void CHtmlgetAdmin::categories_refresh(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	_trs = "";

	//string first = "";
	//lookitem(request_dto.szArgs, "first=", first);
	{
		EnterCriticalSection(&_nbcs);
		_mapSizes.clear();
		LeaveCriticalSection(&_nbcs);

		_beginthreadex(NULL, 0, sb_nb, this, NULL, NULL);
	}

	StringBuffer sb;
	Writer<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("stat");
	writer.String("ok");
	writer.EndObject();

	response_dto.szResponse = sb.GetString();
	response_dto.szContentType = "text/plain; charset=utf-8";
}

void CHtmlgetAdmin::galleries_init(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	if (_trs != "")
	{
		response_dto.szResponse = _trs;
		response_dto.szContentType = "text/plain; charset=utf-8";
		return;
	}

	list<CATEGORY_DTO>::iterator iter;
	list<CATEGORY_DTO> lstCategory;
	CHtmlgetMenubar::Instance().clear();
	CHtmlgetMenubar::Instance().get_categories_order_by_root_dir(0, lstCategory);

	wstring szPath = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);
	string album = CDataSrv::Instance()._albumName;

	wstring dDir = (album == "private") ? L"/data2" : L"/data1";
	wstring path0 = szPath + dDir;

	list<ROOT_DTO>::iterator it0;
	list<ROOT_DTO> lstRoot;
	Cchatdal::Instance().get_roots2(lstRoot);

	string method = "";
	lookitem(request_dto.szArgs, "method=", method);

	uint32_t idx3 = 0;

	_cats_map.clear();
	_lstInitDto.clear();

	_lstState.clear();
	_lstFsPending.clear();

	list<TR_DTO> _lstTR;

	for (int i = 0; i < 100; i++)
	{
		while (!_queState[i].empty())
		{
			_queState[i].pop();
		}
	}

	string folder_tr = "";
	CLang::Instance().getOneData("folder_tr", folder_tr);

	string coo_a = "";
	string coo_span = "";
	CLang::Instance().getOneData("coo_a", coo_a);
	CLang::Instance().getOneData("coo_span", coo_span);

	string folder_tr_sp = folder_tr;
	string folder_td = "";
	CLang::Instance().getOneData("folder_td_a", folder_td);
	ReplaceOne(folder_tr, "{$folder_td}", folder_td);

	string folder_td_sp = "";
	CLang::Instance().getOneData("folder_td_span", folder_td_sp);
	ReplaceOne(folder_tr_sp, "{$folder_td}", folder_td_sp);

	string remove_enc_title = "";
	if (method == "galleries.init0")
	{
		CLang::Instance().getOneData("remove_enc_title", remove_enc_title);
	}
	else
	{
		if (CDataSrv::Instance()._albumName == "private")
		{
			CLang::Instance().getOneData("remove_enc_title1", remove_enc_title);
		}
		else
		{
			CLang::Instance().getOneData("remove_enc_title", remove_enc_title);
		}
	}

	string operate_folder_remove = "";
	CLang::Instance().getOneData("operate_folder_remove", operate_folder_remove);
	ReplaceOne(operate_folder_remove, "{$title}", remove_enc_title);
	translate(operate_folder_remove, "", "");

	string operate_folder_remove2 = "";
	CLang::Instance().getOneData("operate_folder_remove2", operate_folder_remove2);

	string dir_files = "";
	CLang::Instance().getOneData("dir_files", dir_files);
	translate(dir_files, "", "");

	string pending_files = "";
	CLang::Instance().getOneData("pending_files", pending_files);
	translate(pending_files, "", "");

	map<wstring, uint32_t>::iterator I2;
	for (it0 = lstRoot.begin(); it0 != lstRoot.end(); it0++)
	{
		bool b = false;
		wstring szPath = _u2w(it0->root_dir0);
		for (I2 = _mapSizes.begin(); I2 != _mapSizes.end(); I2++)
		{
			if (_wcsnicmp(I2->first.c_str(), szPath.c_str(), szPath.size()) == 0)
			{
				b = true;
				break;
			}
		}
		if (!b)
		{
			CHtmlgetFolder::Instance().GetSubFolderNb(szPath, _mapSizes);
		}
	}

	list<CATEGORY_DTO> lstCat = lstCategory;
	uint32_t i = 0;

	string synroot = "";
	for (it0 = lstRoot.begin(); it0 != lstRoot.end(); it0++)
	{
		idx3 = 0;

		synroot += it0->root_dir0 + "|";

		i++;
		string zzid = uint2str(i);

		wstring szPath = _u2w(it0->root_dir0);
		string szPath2 = it0->root_dir0;
		string root = it0->root_dir0;


		string alb = "1";
		DWORD dwAttr = GetFileAttributes(szPath.c_str());
		if (dwAttr != -1)
		{
			// found something; better not be a folder....
			if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
			{
				alb = "";
			}
		}

		list<string>::iterator it;
		list<string> lstDir;

		wstring szpath = szPath + L"/";
		for (I2 = _mapSizes.begin(); I2 != _mapSizes.end(); I2++)
		{
			wstring dir = I2->first + L"/";
			if (_wcsnicmp(dir.c_str(), szpath.c_str(), szpath.size()) == 0)
			{
				lstDir.push_back(_w2u(I2->first));
			}
		}

		list<FOLDER_STATE_DTO>::iterator it3;
		list<FILE_DTO>::iterator it5;

		vector<string> v0 = tokenize(szPath2, "/", true, "");
		int offset = (int)v0.size();

		for (it = lstDir.begin(); it != lstDir.end(); it++)
		{
			string fname = *it;
			string dispDirf = fname;

			vector<string> v1 = tokenize(fname, "/", true, "");
			uint32_t nb1 = (uint32_t)(v1.size() - offset + 1);

			string id5 = "";
			string fullname = "";
			bool b5 = false;
			for (iter = lstCat.begin(); iter != lstCat.end(); iter++)
			{
				if (_stricmp(it->c_str(), iter->dir0.c_str()) == 0)
				{
					fullname = iter->name;
					id5 = iter->id;
					lstCat.erase(iter);
					b5 = true;
					break;
				}
			}

			TR_DTO dto;
			{
				if (b5)
				{
					string temp = folder_tr;
					if (_stricmp(it->c_str(), it0->root_dir0.c_str()) == 0)
					{
						ReplaceOne(temp, "{$coo}", coo_a);
					}
					else
					{
						ReplaceOne(temp, "{$coo}", "");
					}

					Replace(temp, "{$id}", id5);
					Replace(temp, "treecol", "col" + uint2str(nb1));

					ReplaceOne(temp, "{$root}", root);
					ReplaceOne(temp, "{$folder}", fullname);
					if (szPath2 != "")
					{
						dto.id = "";
						string operate_folder = operate_folder_remove;
						ReplaceOne(operate_folder, "{$rootid}", it0->id);
						ReplaceOne(temp, "{$operate_folder}", operate_folder);

						ReplaceOne(temp, "{$mytd}", "mytd00");
						ReplaceOne(temp, "{$zz}", "root_tr");
						ReplaceOne(temp, "{$img}", "<img src=\"./templ/themes/default/images/loader.gif\" />");
					}
					else
					{
						dto.id = it0->id;
						string operate_folder = operate_folder_remove2;
						ReplaceOne(operate_folder, "{$dir}", dispDirf);
						ReplaceOne(temp, "{$operate_folder}", operate_folder);

						ReplaceOne(temp, "{$mytd}", "mytd11");
						ReplaceOne(temp, "{$zz}", "hide_tr zz" + zzid);
						ReplaceOne(temp, "{$img}", "");
					}
					ReplaceOne(temp, "{$dir}", it0->root_dir0);
					ReplaceOne(temp, "{$rootid}", it0->id);

					string t3 = it0->root_dir0;
					my_tolower(t3);
					string::size_type pos0 = t3.find(_w2u(path0));
					if (pos0 != string::npos)
					{
						t3 = "." + it0->root_dir0.substr(path0.size());
					}
					else
					{
						t3 = it0->root_dir0;
					}
					ReplaceOne(temp, "{$dir3}", t3);
					ReplaceOne(temp, "{$zzid}", zzid);
					ReplaceOne(temp, "{$trid}", string("trid") + id5);
					ReplaceOne(temp, "{$rootid}", it0->id);

					dto.tr = temp;
					_lstTR.push_back(dto);
				}
				else
				{
					char buf[MAX_PATH] = { 0 };
					strcpy_s(buf, MAX_PATH, fname.c_str());

					string name = "";
					char* p = strrchr(buf, '/');
					if (p != NULL)
					{
						name = p + 1;
					}

					idx3++;
					string cat_id = "i" + it0->id + uint2str(idx3);

					string temp = folder_tr_sp;
					if (_stricmp(it->c_str(), it0->root_dir0.c_str()) == 0)
					{
						ReplaceOne(temp, "{$coo}", coo_a);
					}
					else
					{
						ReplaceOne(temp, "{$coo}", "");
					}

					Replace(temp, "{$id}", cat_id);
					Replace(temp, "treecol", "col" + uint2str(nb1));

					ReplaceOne(temp, "{$root}", root);
					ReplaceOne(temp, "{$folder}", name);
					if (szPath2 != "")
					{
						dto.id = "";
						string operate_folder = operate_folder_remove;
						ReplaceOne(operate_folder, "{$rootid}", it0->id);
						ReplaceOne(temp, "{$operate_folder}", operate_folder);

						ReplaceOne(temp, "{$mytd}", "mytd00");
						ReplaceOne(temp, "{$zz}", "root_tr");
						ReplaceOne(temp, "{$img}", "<img src=\"./templ/themes/default/images/loader.gif\" />");
					}
					else
					{
						dto.id = it0->id;
						string operate_folder = operate_folder_remove2;
						ReplaceOne(operate_folder, "{$dir}", dispDirf);
						ReplaceOne(temp, "{$operate_folder}", operate_folder);

						ReplaceOne(temp, "{$mytd}", "mytd11");
						ReplaceOne(temp, "{$zz}", "hide_tr zz" + zzid);
						ReplaceOne(temp, "{$img}", "");
					}

					ReplaceOne(temp, "{$rootid}", it0->id);
					ReplaceOne(temp, "{$dir}", it0->root_dir0);
					string t3 = it0->root_dir0;
					my_tolower(t3);
					string::size_type pos0 = t3.find(_w2u(path0));
					if (pos0 != string::npos)
					{
						t3 = "." + it0->root_dir0.substr(path0.size());
					}
					else
					{
						t3 = it0->root_dir0;
					}
					ReplaceOne(temp, "{$dir3}", t3);
					ReplaceOne(temp, "{$zzid}", zzid);
					ReplaceOne(temp, "{$trid}", string("trid") + cat_id);
					ReplaceOne(temp, "{$rootid}", it0->id);

					dto.tr = temp;
					_lstTR.push_back(dto);

					FOLDER_STATE_DTO statedto;
					statedto.root_id = it0->id;
					statedto.id = cat_id;
					statedto.path = *it;
					statedto.uploaded = dir_files;
					statedto.pending = pending_files;
					_lstInitDto.push_back(statedto);
				}

				if (szPath2 != "")
				{
					szPath2 = "";
				}
			}
		}
	}

	rtrim(synroot, "|");

	if (method == "galleries.init")
	{
		list<string> fol;
		get_restore(fol);

		if (!fol.empty())
		{
			string folder_tr3 = "";
			CLang::Instance().getOneData("folder_tr3", folder_tr3);

			wstring szPath = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);
			wstring dDir = (album == "private") ? L"/data2" : L"/data1";
			wstring path0 = szPath + dDir + L"/_data";
			ReplaceOne(folder_tr3, "{$root}", _w2u(path0));

			if (album == "private")
			{
				ReplaceOne(folder_tr3, "visible", "hidden");
			}

			translate(folder_tr3, "", "");

			TR_DTO dto;
			dto.id = "";
			dto.tr = folder_tr3;
			_lstTR.push_back(dto);
		}
	}

	StringBuffer sb;
	Writer<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("stat");
	writer.String("ok");
	writer.String("synroot");
	writer.String(synroot.c_str());

	writer.String("cats");
	writer.StartArray();
	if (!_lstTR.empty())
	{
		list<TR_DTO> ::iterator I2;
		for (I2 = _lstTR.begin(); I2 != _lstTR.end(); I2++)
		{
			writer.StartObject();
			writer.String("id");
			writer.String(I2->id.c_str());
			writer.String("tr");
			writer.String(I2->tr.c_str());
			writer.EndObject();
		}
	}
	writer.EndArray();
	writer.EndObject();

	response_dto.szResponse = sb.GetString();
	_trs = response_dto.szResponse;
	response_dto.szContentType = "text/plain; charset=utf-8";
}

void CHtmlgetAdmin::galleries_pending(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string catid = "";
	string rootid = "";
	string first = "";
	string idx0 = "";
	lookitem(request_dto.szArgs, "catid=", catid);
	lookitem(request_dto.szArgs, "rootid=", rootid);
	lookitem(request_dto.szArgs, "first=", first);
	lookitem(request_dto.szArgs, "idx0=", idx0);
	int idx = atoi(idx0.c_str());

	list<FOLDER_STATE_DTO>::iterator it;

	for (it = _lstInitDto.begin(); it != _lstInitDto.end(); it++)
	{
		if (it->root_id == rootid)
		{
			_queState[idx].push(*it);
		}
	}

	if (!_queState[idx].empty())
	{
		_nb_init0_thread[idx] = 0;

		for (int i = 0; i < _cpu; i++)
		{
			IDX_DTO* idxdto = new IDX_DTO;
			idxdto->idx = idx;
			idxdto->pParam = (LPVOID)this;

			_nb_init0_thread[idx]++;
			_beginthreadex(NULL, 0, sb_init, (LPVOID)idxdto, NULL, NULL);
		}

		for (;;)
		{
			if (_nb_init0_thread[idx] == 0)
			{
				break;
			}
			this_thread::sleep_for(chrono::nanoseconds(100));
		}
	}

	list<CAT_NB_DTO>lstNb2;
	list<FOLDER_STATE_DTO> lstfol;

	EnterCriticalSection(&_nbcs);
	for (it = _lstState.begin(); it != _lstState.end(); it++)
	{
		if (it->root_id == rootid)
		{
			lstfol.push_back(*it);
		}
	}

	if (first == "")
	{
		lstNb2 = _lstCatNb;
	}
	LeaveCriticalSection(&_nbcs);

	if (first == "")
	{
		string dir_files = "";
		CLang::Instance().getOneData("dir_files", dir_files);
		translate(dir_files, "", "");

		string pending_files = "";
		CLang::Instance().getOneData("pending_files", pending_files);
		translate(pending_files, "", "");

		list<CAT_NB_DTO>::iterator it0;
		for (it0 = lstNb2.begin(); it0 != lstNb2.end(); it0++)
		{
			if (it0->root_id == rootid)
			{
				int nbExist = it0->nb_db_pics;
				int nb0 = it0->nb_folder_pics - it0->nb_db_pics - it0->nb_err_pics;

				string nbdis = uint2str(nb0);
				if (nb0 < 0)
				{
					nbdis = "0";
				}

				string state = "";
				if (it0->nb_changed == 0)
				{
					state = "0";
				}
				else
				{
					state = Int2Str(nb0);
				}

				FOLDER_STATE_DTO state_dto;

				state_dto.id = it0->id;
				state_dto.uploaded = dir_files;
				state_dto.pending = pending_files;
				state_dto.root_id = it0->root_id;
				state_dto.state = state;
				state_dto.alb = "";
				state_dto.path = _w2u(it0->dir0);

				ReplaceOne(state_dto.uploaded, "nb1", uint2str(nbExist));
				ReplaceOne(state_dto.pending, "nb0", nbdis);

				lstfol.push_back(state_dto);
			}
		}
	}

	StringBuffer sb;
	Writer<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("stat");
	writer.String("ok");
	writer.String("catid");
	writer.String(catid.c_str());
	writer.String("result");
	writer.StartArray();

	list<FOLDER_STATE_DTO>::iterator it2;
	for (it2 = lstfol.begin(); it2 != lstfol.end(); it2++)
	{
		writer.StartObject();
		writer.String("id");
		writer.String(it2->id.c_str());
		writer.String("uploaded");
		writer.String(it2->uploaded.c_str());
		writer.String("pending");
		writer.String(it2->pending.c_str());
		writer.String("state");
		writer.String(it2->state.c_str());
		writer.String("root_id");
		writer.String(it2->root_id.c_str());
		writer.String("alb");
		writer.String(it2->alb.c_str());
		writer.String("path");
		writer.String(it2->path.c_str());
		writer.EndObject();
	}
	writer.EndArray();
	writer.EndObject();

	response_dto.szResponse = sb.GetString();
	response_dto.szContentType = "text/plain; charset=utf-8";
}

void CHtmlgetAdmin::galleries_pendingok(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	StringBuffer sb;
	Writer<StringBuffer> writer(sb);

	string stat = Int2Str(CDataSrv::Instance()._ischanged);

	int v = 0;
	list<CAT_NB_DTO>::iterator it0;
	for (it0 = _lstCatNb.begin(); it0 != _lstCatNb.end(); it0++)
	{
		if (it0->nb_changed < 0 && it0->nb_db_pics + it0->nb_err_pics > it0->nb_folder_pics)
		{
			v += it0->nb_db_pics + it0->nb_err_pics - it0->nb_folder_pics;
		}
	}
	string deletes = Int2Str(v);

	writer.StartObject();
	writer.String("stat");
	writer.String(stat.c_str());
	writer.String("deletes");
	writer.String(deletes.c_str());
	writer.EndObject();

	response_dto.szResponse = sb.GetString();
	response_dto.szContentType = "text/plain; charset=utf-8";
}

void CHtmlgetAdmin::galleries_clear(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	_brisquing++;

	map<string, string, less<string> > mapIds;
	map<string, string, less<string> >::iterator it9;
	{
		string ids = "";
		string files = "";
		lookitem(request_dto.szBody, "ids=", ids);
		lookitem(request_dto.szBody, "files=", files);
		URLDecode(files, 0);

		int i = 0;
		vector<string> vv0 = tokenize(ids, ",", true, "");
		vector<string> vv1 = tokenize(files, ",", true, "");
		int len = (int)vv1.size();

		for (i = 0; i < len; i++)
		{
			mapIds.insert(map<string, string>::value_type(vv1[i], vv0[i]));
		}
	}

	list<ROOT_DTO> lstRoot;
	CDataSrv::Instance().get_roots(lstRoot);
	list<ROOT_DTO>::iterator it2;
	{
		string val = "";
		CDataSrv::Instance().get_config_val("last_import", val);

		time_t current_time;
		time(&current_time);
		val = uint64str(static_cast<uint64_t>(current_time)) + "," + val;
		CDataSrv::Instance().set_config_val("last_import", val);

		list<CATEGORY_DTO>::iterator iter;
		list<CATEGORY_DTO> lst;
		CHtmlgetMenubar::Instance().clear();
		CHtmlgetMenubar::Instance().get_categories_order_by_rank(0, lst);

		wstring szPath = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);

		list<CATEGORY_DTO> lstCat;
		for (it9 = mapIds.begin(); it9 != mapIds.end(); it9++)
		{
			string fname = it9->first;
			string tt2 = fname + "/";

			string root_id = "";

			for (it2 = lstRoot.begin(); it2 != lstRoot.end(); it2++)
			{
				string t9 = it2->root_dir0 + "/";
				if (_strnicmp(tt2.c_str(), t9.c_str(), t9.size()) == 0)
				{
					root_id = it2->id;
					break;
				}
			}

			char buf[MAX_PATH] = { 0 };
			strcpy_s(buf, MAX_PATH, fname.c_str());

			string name = "";
			char* p = strrchr(buf, '/');
			if (p != NULL)
			{
				name = p + 1;
			}

			time_t current_time;
			time(&current_time);

			CATEGORY_DTO category;
			category.name = name;
			category.full_id = "0";
			category.lastmodified = static_cast<uint64_t>(current_time);
			category.dir0 = fname;

			string t9 = category.dir0;
			my_tolower(t9);
			string::size_type pos0 = t9.find(_w2u(szPath));
			if (pos0 != string::npos)
			{
				category.dir0 = "." + category.dir0.substr(szPath.size());
			}
			category.root_id = root_id;
			lstCat.push_back(category);
		}

		if (!lstCat.empty())
		{
			writedb::Instance().add_category(lstCat);

			if (CDataSrv::Instance()._albumName != "private")
			{
				CDataSrv::Instance()._acc = "2";
			}
		}

		//import
		list<CATEGORY_DTO> lstCategory;
		CHtmlgetMenubar::Instance().clear();
		CHtmlgetMenubar::Instance().get_order_categories("a.dir", lstCategory);
		CHtmlupload::Instance().set_roots();

		for (it9 = mapIds.begin(); it9 != mapIds.end(); it9++)
		{
			for (iter = lstCategory.begin(); iter != lstCategory.end(); iter++)
			{
				if (_stricmp(it9->first.c_str(), iter->dir0.c_str()) == 0)
				{
					_cats_map.insert(map<string, string>::value_type(it9->second, iter->id));
					it9->second = iter->id;
					break;
				}
			}
		}

		list<TEMPL_DTO> map_full_id;
		for (it9 = mapIds.begin(); it9 != mapIds.end(); it9++)
		{
			char buf[MAX_PATH] = { 0 };
			strcpy_s(buf, MAX_PATH, it9->first.c_str());

			string updir = "";
			char* p = strrchr(buf, '/');
			if (p != NULL)
			{
				p[0] = 0;
				updir = buf;
			}

			for (iter = lstCategory.begin(); iter != lstCategory.end(); iter++)
			{
				if (_stricmp(updir.c_str(), iter->dir0.c_str()) == 0)
				{
					TEMPL_DTO dto;
					dto.id = it9->second;
					dto.val = iter->id;
					map_full_id.push_back(dto);
					break;
				}
			}
		}
		writedb::Instance().update_full_id(map_full_id);
	}

	StringBuffer sb;
	Writer<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("stat");
	writer.String("ok");

	writer.String("cats");
	writer.StartArray();
	if (!_cats_map.empty())
	{
		map<string, string> ::iterator I2;
		for (I2 = _cats_map.begin(); I2 != _cats_map.end(); I2++)
		{
			writer.StartObject();
			writer.String("id");
			writer.String(I2->first.c_str());
			writer.String("newid");
			writer.String(I2->second.c_str());
			writer.EndObject();
		}
	}
	writer.EndArray();
	writer.EndObject();

	response_dto.szResponse = sb.GetString();
	response_dto.szContentType = "text/plain; charset=utf-8";

	while (!_queUpload.empty())
	{
		_queUpload.pop();
	}

	list<FILE_DTO > ::iterator it;

	for (it = _lstDbPending.begin(); it != _lstDbPending.end(); it++)
	{
		_queUpload.push(*it);
	}

	if (!_lstFsPending.empty())
	{
		map<string, string>::iterator z;
		for (it = _lstFsPending.begin(); it != _lstFsPending.end(); it++)
		{
			if (!_cats_map.empty())
			{
				z = _cats_map.find(it->cat_id);
				if (z != _cats_map.end())
				{
					it->cat_id = z->second.c_str();
				}
			}

			_queUpload.push(*it);
		}
	}
}

void CHtmlgetAdmin::galleries_categories(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	//upload begin
	string clr = "";
	lookitem(request_dto.szArgs, "clr=", clr);

	EnterCriticalSection(&_nbcs);
	{
		if (_del_ids != "")
		{
			rtrim(_del_ids, ",");
			list<string> _lstFiles;
			list<string> _lstCoi;
			Cimagedal::Instance().get_files(_del_ids, "", _lstFiles, _lstCoi);
			long hr = writedb::Instance().update_images("delete", _del_ids, "");
			if (hr == 0)
			{
				writedb::Instance().update_last_import();
				delete_image_files("syn", _lstFiles, _lstCoi);
			}
		}
		_progress_len3 = 0;
	}
	LeaveCriticalSection(&_nbcs);

	if (clr == "1")
	{
		EnterCriticalSection(&_nbcs);
		{
			if (_del_ids != "")
			{
				_del_ids = "";
				CDataSrv::Instance().clear_all();
				CHtmlgetIndexProc::Instance().clear_data();
				CHtmlgetMenubar::Instance().clear();
			}
			_progress_len3 = 0;
		}
		LeaveCriticalSection(&_nbcs);
	}
	else
	{
		EnterCriticalSection(&_nbcs);
		{
			_del_ids = "";
		}
		LeaveCriticalSection(&_nbcs);

		_nb_gg2_thread = 0;
		_progress_step3 = 0;
		_progress_cancel = 0;
		_nb_upload_ok = 0;
		_nb_upload_err30 = 0;
		_nb_upload_err40 = 0;

		_progress_len3 = (int)_queUpload.size();

		for (int i = 0; i < _cpu; i++)
		{
			_nb_gg2_thread++;
			_beginthreadex(NULL, 0, sb_upload, this, NULL, NULL);
		}
	}

	string val = "ok";
	StringBuffer sb;
	Writer<StringBuffer> writer(sb);
	writer.StartObject();

	writer.String("stat");
	writer.String(val.c_str());
	writer.EndObject();

	response_dto.szResponse = sb.GetString();
	response_dto.szContentType = "text/plain; charset=utf-8";
}

void CHtmlgetAdmin::upload_complete(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string fileupload = "";
	string cancel = "";
	string cat_id = "";
	lookitem(request_dto.szArgs, "fileupload=", fileupload);
	lookitem(request_dto.szArgs, "cancel=", cancel);
	lookitem(request_dto.szArgs, "id=", cat_id);
	rtrim(cat_id, ",");

	//if (cancel == "1" || cancel == "2")
	//{
	//	_progress_cancel = 1;
	//}

	list<CATEGORY_DTO> lstCategory;
	list<CATEGORY_DTO>::iterator it;
	CHtmlgetMenubar::Instance().get_categories(cat_id, lstCategory);

	EnterCriticalSection(&_nbcs);
	{
		CDataSrv::Instance().clear_all();
		CHtmlgetIndexProc::Instance().clear_data();
		CHtmlgetMenubar::Instance().clear();
		writedb::Instance().set_representative_picture_id("");

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.String("nb");
		writer.String(Int2Str(_nb_upload_ok).c_str());
		writer.String("err30");
		writer.String(Int2Str(_nb_upload_err30).c_str());
		writer.String("err40");
		writer.String(Int2Str(_nb_upload_err40).c_str());

		if (cat_id != "")
		{
			writer.String("result");
			writer.StartArray();
			for (it = lstCategory.begin(); it != lstCategory.end(); it++)
			{
				if (it->nb_images != 0)
				{
					writer.StartObject();
					writer.String("cat_id");
					writer.String(it->id.c_str());
					writer.String("nb_photos");
					writer.String(uint2str(it->nb_images).c_str());
					writer.String("label");
					writer.String(it->name.c_str());
					writer.EndObject();
				}
			}
			writer.EndArray();
		}
		writer.EndObject();

		response_dto.szResponse = sb.GetString();
		response_dto.szContentType = "text/plain; charset=utf-8";
	}
	LeaveCriticalSection(&_nbcs);

	_trs = "";
	if (fileupload == "") {
		EnterCriticalSection(&_nbcs);
		_mapSizes.clear();
		LeaveCriticalSection(&_nbcs);
		_beginthreadex(NULL, 0, sb_nb, this, NULL, NULL);
	}
	else
	{
		list<CAT_NB_DTO> lstCatNb;
		Cchatdal::Instance().get_cat_nb(cat_id, lstCatNb);
		list<CAT_NB_DTO>::iterator it0;
		map<wstring, uint32_t>::iterator I2;

		EnterCriticalSection(&_nbcs);
		{
			folder_ischange();

			for (it0 = lstCatNb.begin(); it0 != lstCatNb.end(); it0++)
			{
				for (I2 = _mapSizes.begin(); I2 != _mapSizes.end(); I2++)
				{
					if (_wcsicmp(it0->dir0.c_str(), I2->first.c_str()) == 0)
					{
						it0->nb_folder_files = I2->second;
						it0->nb_folder_pics = it0->nb_db_pics;
						_lstCatNb.push_back(*it0);
					}
				}
			}
		}

		LeaveCriticalSection(&_nbcs);

	}

	galleries_brisque(_brisquing);
}

void CHtmlgetAdmin::galleries_brisque(int idx)
{
	IDX_DTO* ptr = new IDX_DTO;
	ptr->pParam = (LPVOID)this;
	ptr->idx = idx;

	HANDLE h = (HANDLE)_beginthreadex(NULL, 0, sb_brisque, (LPVOID)ptr, 0, 0);
	if (h != 0)
	{
		SetThreadPriority(h, THREAD_PRIORITY_IDLE);
		CloseHandle(h);
	}
}

UINT __stdcall CHtmlgetAdmin::sb_brisque(LPVOID pParam)
{
	IDX_DTO* ptr = (IDX_DTO*)pParam;
	int idx = ptr->idx;
	CHtmlgetAdmin* pThis = (CHtmlgetAdmin*)ptr->pParam;

	for (;;)
	{
		this_thread::sleep_for(chrono::milliseconds(1000));

		if (pThis->_brisque_end == 1)
		{
			break;
		}
	}

	pThis->brisque(idx);
	delete ptr;

	return 0;
}

long CHtmlgetAdmin::brisque(int idx)
{
	_brisque_end = 0;

	string enckey = "";
	wstring krf = L"";
	if (CDataSrv::Instance()._albumName == "private")
	{
		krf = L".krf";
		CDataSrv::Instance().get_config_val("enckey", enckey);
	}

	list<ROOT_DTO> lstRoot;
	CDataSrv::Instance().get_roots(lstRoot);
	for (;;)
	{
		this_thread::sleep_for(chrono::milliseconds(10));

		if (_brisquing > idx)
		{
			break;
		}

		list<TEMPL_DTO> lstImage;
		CHtmlgetIndexProc::Instance().get_cache(0, lstImage);
		if (lstImage.empty())
		{
			Cimagedal::Instance().get_images_quality("0", lstImage);
		}

		if (lstImage.empty())
		{
			break;
		}

		list<TEMPL_DTO> lstQuality;

		list<TEMPL_DTO>::iterator iter;
		for (iter = lstImage.begin(); iter != lstImage.end(); iter++)
		{
			this_thread::sleep_for(chrono::milliseconds(10));

			wstring src = _u2w(ori2path(lstRoot, 1, iter->val)) + krf;
			uint32_t level = 0;
			int ret = 0;

			ret = img_brisque(src.c_str(), enckey.c_str(), level);
			if (ret == 0)
			{
				if (level == 0)
				{
					level = 1;
				}

				TEMPL_DTO dto;
				dto.id = iter->id;
				dto.val = uint2str(level);
				lstQuality.push_back(dto);
			}
		}

		if (!lstQuality.empty())
		{
			writedb::Instance().update_quality(lstQuality);
			CHtmlgetIndexProc::Instance().update_cache(lstQuality);
		}
	}
	_brisque_end = 1;

	return 0;
}

bool CHtmlgetAdmin::folder_ischange()
{
	bool b = false;
	map<wstring, uint32_t> mapSizes;
	map<wstring, uint32_t>::iterator I2;
	list<ROOT_DTO> lstRoot;
	list<ROOT_DTO>::iterator I;
	Cchatdal::Instance().get_roots2(lstRoot);

	for (I = lstRoot.begin(); I != lstRoot.end(); I++)
	{
		CHtmlgetFolder::Instance().GetSubFolderNb(_u2w(I->root_dir0), mapSizes);
	}

	uint32_t nb1 = 0;
	for (I2 = _mapSizes.begin(); I2 != _mapSizes.end(); I2++)
	{
		nb1 += I2->second;
	}

	uint32_t nb2 = 0;
	for (I2 = mapSizes.begin(); I2 != mapSizes.end(); I2++)
	{
		nb2 += I2->second;
	}

	if (nb1 != nb2)
	{
		_trs = "";
		_mapSizes = mapSizes;
		b = true;
	}
	return b;
}

UINT __stdcall CHtmlgetAdmin::sb_nb(LPVOID pParam)
{
	CHtmlgetAdmin* pThis = (CHtmlgetAdmin*)pParam;
	pThis->nb_proc();
	return 0;
}

void CHtmlgetAdmin::nb_proc()
{
	if (_crc == 0 && CDataSrv::Instance()._albumName == "private")
	{
		CBlowfish m_BF;
		string val = "";
		CDataSrv::Instance().get_config_val("enckey", val);
		m_BF.SetPassword((char*)val.c_str());
		_crc = m_BF._crc;
	}

	EnterCriticalSection(&_nbcs);

	bool b9 = true;
	if (_queUpload.empty())
	{
		b9 = folder_ischange();
	}

	if (b9)
	{
		_lstCatNbOk = 0;
		_lstCatNb.clear();
		_lstmd5.clear();
		_lstDbPending.clear();

		while (!_queCatNb2.empty())
		{
			_queCatNb2.pop();
		}

		Cchatdal::Instance().get_cat_nb("", _lstCatNb);

		string catids = "";
		map<wstring, uint32_t>::iterator I2;
		list<CAT_NB_DTO>::iterator it;
		for (it = _lstCatNb.begin(); it != _lstCatNb.end(); it++)
		{
			for (I2 = _mapSizes.begin(); I2 != _mapSizes.end(); I2++)
			{
				if (_wcsicmp(I2->first.c_str(), it->dir0.c_str()) == 0)
				{
					it->nb_folder_files = I2->second;
					break;
				}
			}
			if (it->nb_db_files != it->nb_folder_files && it->nb_folder_files != 0)
			{
				_queCatNb2.push(*it);
				catids += it->id + ",";
			}
		}

		if (catids != "")
		{
			rtrim(catids, ",");
			Cchatdal::Instance().get_image_md5(catids, _lstmd5);
		}

		if (!_queUpload.empty())
		{
			while (!_queUpload.empty())
			{
				FILE_DTO dto;
				dto = _queUpload.front();
				_queUpload.pop();
				_lstDbPending.push_back(dto);
			}

			list<FILE_DTO> lstTemp = _lstDbPending;

			list<FILE_DTO > ::iterator it3;
			list<CAT_NB_DTO>::iterator it5;
			for (it5 = _lstCatNb.begin(); it5 != _lstCatNb.end(); it5++)
			{
				it5->nb_folder_pics = 0;

				for (it3 = lstTemp.begin(); it3 != lstTemp.end(); )
				{
					if (it5->id == it3->cat_id)
					{
						it5->nb_folder_pics++;
						it3 = lstTemp.erase(it3);
					}
					else
					{
						it3++;
					}
				}

				it5->nb_folder_pics += it5->nb_db_pics - it5->nb_err_pics;
				it5->nb_changed = it5->nb_folder_pics - it5->nb_db_pics - it5->nb_err_pics;
			}
		}
		else
		{
			_nb_thread = 0;

			for (int i = 0; i < _cpu; i++)
			{
				_nb_thread++;
				_beginthreadex(NULL, 0, proc_nb_, this, NULL, NULL);
			}

			for (;;)
			{
				if (_nb_thread == 0)
				{
					break;
				}
				this_thread::sleep_for(chrono::nanoseconds(100));
			}
		}

		list<CAT_NB_DTO> lstOk;
		for (it = _lstCatNb.begin(); it != _lstCatNb.end(); it++)
		{
			if (it->nb_db_pics + it->nb_err_pics == it->nb_folder_pics && it->nb_db_files != it->nb_folder_files)
			{
				lstOk.push_back(*it);
			}

		}
		if (!lstOk.empty())
		{
			writedb::Instance().update_cat_nb(lstOk);
		}

		int _is_changed = 0;
		for (it = _lstCatNb.begin(); it != _lstCatNb.end(); it++)
		{
			if (it->nb_changed < 0)
			{
				_is_changed = -1;
			}
			if (it->nb_changed > 0)
			{
				_is_changed = 1;
				break;
			}
		}

		for (I2 = _mapSizes.begin(); I2 != _mapSizes.end(); I2++)
		{
			bool b = false;
			for (it = _lstCatNb.begin(); it != _lstCatNb.end(); it++)
			{
				if (_wcsicmp(I2->first.c_str(), it->dir0.c_str()) == 0)
				{
					b = true;
				}
			}

			if (!b)
			{
				_is_changed = 1;
				CAT_NB_DTO dto;
				dto.nb_folder_files = I2->second;
				_lstCatNb.push_back(dto);
			}
		}

		CDataSrv::Instance()._ischanged = _is_changed;
		_lstCatNbOk = 1;
	}
	LeaveCriticalSection(&_nbcs);
}

UINT __stdcall CHtmlgetAdmin::proc_nb_(LPVOID pParam)
{
	CHtmlgetAdmin* pThis = (CHtmlgetAdmin*)pParam;
	pThis->proc_nb_ok();
	return 0;
}

void CHtmlgetAdmin::proc_nb_ok()
{
	for (;;)
	{
		this_thread::sleep_for(chrono::nanoseconds(100));

		CAT_NB_DTO dto;

		EnterCriticalSection(&_cs0);
		{
			if (!_queCatNb2.empty())
			{
				dto = _queCatNb2.front();
				_queCatNb2.pop();
			}
		}
		LeaveCriticalSection(&_cs0);

		if (dto.id == "")
		{
			break;
		}

		list<MD5_DTO> lst2;
		list<MD5_DTO>::iterator I;

		MAP_MD5::iterator it0 = _lstmd5.find(dto.id);
		if (it0 != _lstmd5.end())
		{
			lst2 = it0->second;
		}

		wstring t0 = dto.dir0;
		list<FILE_DTO> lstFile;
		list<FILE_DTO>::iterator it;

		CHtmlgetFolder::Instance().GetFolderPics(dto.id, t0, _crc, lstFile);

		int len0 = (int)lstFile.size();
		list<FILE_DTO> lstTemp;

		for (it = lstFile.begin(); it != lstFile.end(); it++)
		{
			bool b = false;
			for (I = lst2.begin(); I != lst2.end(); I++)
			{
				if (I->md5sum == it->dwSize && _wcsnicmp(it->file.c_str(), _u2w(I->path).c_str(), I->path.size()) == 0)
				{
					b = true;
					lst2.erase(I);
					break;
				}
			}

			if (!b)
			{
				lstTemp.push_back(*it);
			}
		}

		EnterCriticalSection(&_cs0);
		{
			for (I = lst2.begin(); I != lst2.end(); I++)
			{
				_del_ids += I->pic_id + ",";
			}

			for (it = lstTemp.begin(); it != lstTemp.end(); it++)
			{
				_lstDbPending.push_back(*it);
			}

			list<CAT_NB_DTO>::iterator it5;
			for (it5 = _lstCatNb.begin(); it5 != _lstCatNb.end(); it5++)
			{
				if (it5->id == dto.id)
				{
					it5->nb_folder_pics = len0;
					it5->nb_changed = it5->nb_folder_pics - it5->nb_db_pics - it5->nb_err_pics;
				}
			}
		}
		LeaveCriticalSection(&_cs0);
	}
	_nb_thread--;
}

UINT __stdcall CHtmlgetAdmin::sb_init(LPVOID pParam)
{
	IDX_DTO* ptr = (IDX_DTO*)pParam;
	CHtmlgetAdmin* pThis = (CHtmlgetAdmin*)ptr->pParam;

	IDX_DTO dto;
	dto.idx = ptr->idx;
	pThis->init_proc(dto);

	delete ptr;
	return 0;
}

void CHtmlgetAdmin::init_proc(IDX_DTO idxdto)
{
	int idx = idxdto.idx;

	for (;;)
	{
		this_thread::sleep_for(chrono::nanoseconds(100));

		FOLDER_STATE_DTO dto;

		EnterCriticalSection(&_cs0);
		{
			if (!_queState[idx].empty())
			{
				dto = _queState[idx].front();
				_queState[idx].pop();
			}
		}
		LeaveCriticalSection(&_cs0);

		if (dto.id == "")
		{
			break;
		}

		list<FILE_DTO> lstFile;
		list<FILE_DTO>::iterator it7;

		CHtmlgetFolder::Instance().GetFolderPics(dto.id, _u2w(dto.path), _crc, lstFile);
		int nb0 = (int)lstFile.size();

		EnterCriticalSection(&_cs0);
		{
			if (!lstFile.empty())
			{
				for (it7 = lstFile.begin(); it7 != lstFile.end(); it7++)
				{
					_lstFsPending.push_back(*it7);
				}
			}

			dto.state = Int2Str(nb0);
			dto.alb = "";

			ReplaceOne(dto.uploaded, "nb1", "0");
			ReplaceOne(dto.pending, "nb0", uint2str(nb0));

			_lstState.push_back(dto);
		}
		LeaveCriticalSection(&_cs0);

	}
	_nb_init0_thread[idx]--;
}

UINT __stdcall CHtmlgetAdmin::sb_upload(LPVOID pParam)
{
	CHtmlgetAdmin* pThis = (CHtmlgetAdmin*)pParam;
	pThis->upload();

	return 0;
}

void CHtmlgetAdmin::upload()
{
	for (;;)
	{
		if (_progress_cancel == 1)
		{
			break;
		}

		this_thread::sleep_for(chrono::nanoseconds(100));

		FILE_DTO dto;

		EnterCriticalSection(&_cs0);
		{
			if (!_queUpload.empty())
			{
				dto = _queUpload.front();
				_queUpload.pop();
			}
		}
		LeaveCriticalSection(&_cs0);

		if (dto.cat_id == "")
		{
			break;
		}

		REQUEST_DTO request_dto2;
		request_dto2.szFile = dto.filename;
		request_dto2.szArgs = "fol=1&category=" + dto.cat_id;

		RESPONSE_DTO response_dto;
		HRESULT hr = CHtmlupload::Instance().post_upload_htm(request_dto2, response_dto);
		if (hr == 0)
		{
			_nb_upload_ok++;
		}
		else if (hr == 40)
		{
			_nb_upload_err40++;
		}
		else
		{
			_nb_upload_err30++;
		}

		_progress_step3++;
	}
	_nb_gg2_thread--;
}

void CHtmlgetAdmin::update_catnb(map<string, uint32_t> mapCat)
{
	list<CAT_NB_DTO>::iterator it0;
	map<string, uint32_t>::iterator it;

	EnterCriticalSection(&_nbcs);

	for (it = mapCat.begin(); it != mapCat.end(); it++)
	{
		for (it0 = _lstCatNb.begin(); it0 != _lstCatNb.end(); it0++)
		{
			if (it0->id == it->first)
			{
				it0->nb_db_pics -= it->second;
				it0->nb_folder_pics -= it->second;
				it0->nb_db_files -= it->second;
				it0->nb_folder_files -= it->second;
			}
		}
	}
	folder_ischange();
	LeaveCriticalSection(&_nbcs);
}

UINT __stdcall CHtmlgetAdmin::sb_decrypt(LPVOID pParam)
{
	CHtmlgetAdmin* pThis = (CHtmlgetAdmin*)pParam;
	pThis->decrypt();
	return 0;
}

void CHtmlgetAdmin::decrypt()
{
	for (;;)
	{
		this_thread::sleep_for(chrono::nanoseconds(100));

		wstring filename0 = L"";
		wstring filename = L"";

		EnterCriticalSection(&_cs0);
		{
			if (!_queDecrypt.empty())
			{
				filename = _queDecrypt.front();
				_queDecrypt.pop();
			}
			else
			{
				if (!_queThumb.empty())
				{
					filename0 = _queThumb.front();
					_queThumb.pop();
				}
			}
		}
		LeaveCriticalSection(&_cs0);

		if (filename0 == L"" && filename == L"")
		{
			break;
		}

		if (filename0 != L"")
		{
			CHtmlgetFolder::Instance().DelFolderFiles(filename0);
			RemoveDirectory(filename0.c_str());
		}
		else
		{
			wstring temp = filename.substr(0, filename.size() - 4);

			int ret = CUploadParser::Instance().Decryptfile(filename, temp);
			if (ret == 0)
			{
				my_DeleteFile(filename.c_str());
			}
		}
		_progress_step++;
	}
}

void CHtmlgetAdmin::processok(string root_id)
{
	list<CATEGORY_DTO>::iterator iter;
	list<CATEGORY_DTO> lst;
	CHtmlgetMenubar::Instance().clear();
	CHtmlgetMenubar::Instance().get_categories_order_by_root_dir(0, lst);

	string ids = "";
	for (iter = lst.begin(); iter != lst.end(); iter++)
	{
		if (iter->root_id == root_id)
		{
			ids += iter->id + ",";
		}
	}

	if (ids != "")
	{
		rtrim(ids, ",");
		long hr = writedb::Instance().update_images("delete_cat", ids, "");
	}

	list<wstring>::reverse_iterator it0;
	for (it0 = _lstDir.rbegin(); it0 != _lstDir.rend(); it0++)
	{
		RemoveDirectory(it0->c_str());
	}

	writedb::Instance().del_root(root_id);
	writedb::Instance().update_last_import();

	CDataSrv::Instance().clear_all();
	CHtmlgetIndexProc::Instance().clear_data();

	list<ROOT_DTO> lstRoot;
	Cchatdal::Instance().get_roots("", "", lstRoot);
	if (lstRoot.empty())
	{
		wstring path0 = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);
		string alb = CDataSrv::Instance()._albumName;
		if (alb == "private")
		{
			path0 += L"/data2/_train/";
		}
		else
		{
			path0 += L"/data1/_train/";
		}

		uint32_t nb = 0;
		list<string> lstFile;
		writedb::Instance().delete_face("", lstFile, nb);

		//if (!lstFile.empty())
		{
			wstring dst = path0 + L"_model/face_model.yml";
			my_DeleteFile(dst);

			dst = path0 + L"_face/";
			wchar_t szBuf2[MAX_PATH] = { 0 };
			DWORD dwSize = GetFullPathName(dst.c_str(), MAX_PATH, szBuf2, NULL);

			list<WIN32_FIND_DATA>::iterator it0;
			list<WIN32_FIND_DATA > vecffd;
			CHtmlgetFolder::Instance().GetFolderPics(1, szBuf2, vecffd);

			for (it0 = vecffd.begin(); it0 != vecffd.end(); it0++)
			{
				wstring txt = dst + it0->cFileName;
				my_DeleteFile(txt);
			}

			dst = path0 + L"_temp/";
			wchar_t szBuf[MAX_PATH] = { 0 };
			dwSize = GetFullPathName(dst.c_str(), MAX_PATH, szBuf, NULL);

			vecffd.clear();
			CHtmlgetFolder::Instance().GetFolderPics(1, szBuf, vecffd);

			for (it0 = vecffd.begin(); it0 != vecffd.end(); it0++)
			{
				wstring txt = dst + it0->cFileName;
				my_DeleteFile(txt);
			}
		}



		_nb_upload_ok = 0;
		_nb_upload_err30 = 0;
		_nb_upload_err40 = 0;

		while (!_queUpload.empty())
		{
			_queUpload.pop();
		}

		_lstDbPending.clear();
		_lstFsPending.clear();
		_queCatNb2;

		_lstState.clear();


		_del_ids = "";
		_lstmd5.clear();
		_lstCatNb.clear();
		_lstInitDto.clear();

		while (!_queDecrypt.empty())
		{
			_queDecrypt.pop();
		}

		while (!_queThumb.empty())
		{
			_queThumb.pop();
		}
		_lstDir.clear();

		CDataSrv::Instance()._ischanged = 0;
	}
}

void CHtmlgetAdmin::viewer_nb()
{
	EnterCriticalSection(&_nbcs);
	_mapSizes.clear();
	LeaveCriticalSection(&_nbcs);

	_beginthreadex(NULL, 0, sb_nb, this, NULL, NULL);
}