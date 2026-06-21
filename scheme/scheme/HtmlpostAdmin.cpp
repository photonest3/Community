
///////////////////////////////////////////////////////////////////////
/// @file HtmlpostAdmin.cpp
/// @brief 管理界面 POST 请求处理实现文件（主文件）
/// @details 实现管理界面的各类 POST 操作处理，包括分类管理、图片操作等
/// @author PhotoNest Team
/// @date 2024
/// @version 1.0
/// @note 本文件是管理界面 POST 请求处理的主要实现文件
///////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include "unitil.h"
#include "../../templ/templ/resource.h"

#include "lang.h"
#include "imagedal.h"
#include "chatdal.h"
#include "wsjosn.h"

#include "DataSrv.h"
#include "HtmlgetAdmin.h"
#include "HtmlgetAdminProc.h"
#include "HtmlgetIndexProc.h"
#include "HtmlgetBatch.h"
#include "Htmlupload.h"
#include "HtmlgetMenubar.h"
#include "HtmlgetViewer.h"
#include "HtmlgetFolder.h"
#include "writedb.h"
#include <process.h>
#include <thread>
#include "../../photol/photol/photol.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

long CHtmlgetAdmin::post_admin_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string page = "";
	lookitem(request_dto.szArgs, "page=", page);

	string tpl = "";
	if (page == "admin.cat_list")
	{
		string act = "";
		lookitem(request_dto.szArgs, "act=", act);

		string cat_list_order = "";
		if (act == "admin.autoorder")
		{
			//order_by=name+DESC&recursive=on&submitAutoOrder=
			string parent_id = "";
			string order_by = "";
			string recursive = "";
			lookitem(request_dto.szArgs, "parent_id=", parent_id);
			lookitem(request_dto.szBody, "order_by=", order_by);
			lookitem(request_dto.szBody, "recursive=", recursive);
			URLDecode(order_by, 1);

			string catOrder = request_dto.szBody;
			//URLDecode(catOrder, 1);

			string myid = parent_id;
			if (myid == "")
			{
				myid = "0";
			}
			CDataSrv::Instance().set_config_val(myid, catOrder);


			list<string> lstCatIds;

			list<CATEGORY_DTO> lstCategory;
			CHtmlgetMenubar::Instance().get_order_categories(order_by, lstCategory);

			list<CATEGORY_DTO>::iterator iter;
			for (iter = lstCategory.begin(); iter != lstCategory.end(); iter++)
			{
				if (parent_id == "")
				{
					if (recursive == "on")
					{
						lstCatIds.push_back(iter->id);
					}
					else
					{
						if (iter->id == iter->full_id)
						{
							lstCatIds.push_back(iter->id);
						}
					}
				}
				else
				{
					string uppercats = "," + iter->full_id + ",";
					string::size_type pos = uppercats.find("," + parent_id + ",");
					if (pos != string::npos)
					{
						if (recursive == "on")
						{
							lstCatIds.push_back(iter->id);
						}
						else
						{
							pos = uppercats.find("," + parent_id + "," + iter->id + ",");
							if (pos != string::npos)
							{
								lstCatIds.push_back(iter->id);
							}
						}
					}
				}
			}

			writedb::Instance().set_order_categories(lstCatIds);

		}
		else if (act == "admin.manualorder")
		{
			list<string> lstCatIds;
			//submitManualOrder=%E4%BF%9D%E5%AD%98%E6%89%8B%E5%8A%A8%E6%8E%92%E5%BA%8F&catOrd%5B5%5D=0&catOrd%5B2%5D=1&catOrd%5B6%5D=2
			string szBody = request_dto.szBody;
			URLDecode(szBody, 0);
			vector<string> v1 = tokenize(szBody, "&", true, "");
			int len = (int)v1.size();
			for (int i = 0; i < len; i++)
			{
				string::size_type pos = v1[i].find("_");
				if (pos > 0)
				{
					string::size_type pos2 = v1[i].find("=");
					if (pos2 > pos + 1)
					{
						string val = v1[i].substr(pos + 1, pos2 - pos - 1);
						lstCatIds.push_back(val);
					}
				}
			}

			writedb::Instance().set_order_categories(lstCatIds);
		}

		string parent_id = "";
		lookitem(request_dto.szArgs, "parent_id=", parent_id);

		list<CATEGORY_DTO> lst;
		CHtmlgetMenubar::Instance().clear();
		CHtmlgetMenubar::Instance().get_categories_order_by_rank(1, lst);
		CHtmlgetMenubar::Instance().proc_categories(&lst);

		string order_by = "";
		string recursive = "";
		string titlename = "";
		if (act == "admin.categoryui")
		{
			string myid = parent_id;
			if (myid == "")
			{
				myid = "0";
			}

			string catOrder = "";
			CDataSrv::Instance().get_config_val(myid, catOrder);
			lookitem(catOrder, "order_by=", order_by);
			lookitem(catOrder, "recursive=", recursive);
			URLDecode(order_by, 1);

			CHtmlgetAdminProc::Instance().get_category_title(lst, "0", parent_id, "", titlename);
			translate(titlename, "", "");
		}
		else
		{
			CLang::Instance().getOneData("cat_list_order", cat_list_order);
			translate(cat_list_order, "", "");
		}

		string categoryUl = "";
		CHtmlgetAdminProc::Instance().get_categoryUl(lst, parent_id, categoryUl);
		translate(categoryUl, "", "");

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.String("order_by");
		writer.String(order_by.c_str());
		writer.String("recursive");
		writer.String(recursive.c_str());
		writer.String("title");
		writer.String(titlename.c_str());
		writer.String("htm");
		writer.String(cat_list_order.c_str());
		writer.String("ui");
		writer.String(categoryUl.c_str());
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}
	else if (page == "admin.tags")
	{
		string mode = "";
		string selectAction = "";
		lookitem(request_dto.szArgs, "mode=", mode);
		lookitem(request_dto.szBody, "selectAction=", selectAction);

		string tag_infos = "";
		string edit_list = "";

		if (mode == "edit" || mode == "duplicate")
		{
			list<TAG_DTO> lstAllTag;
			Cimagedal::Instance().get_tags("", lstAllTag);
			list<TAG_DTO>::iterator iter;

			//edit_list: 9, 10
			//	tag_name - 9 : vvvvvvv
			//	tag_name - 10 : 5555555
			//	edit_submit :

			lookitem(request_dto.szBody, "edit_list=", edit_list);
			URLDecode(edit_list, 0);

			TAG_DTO dto;
			list<TAG_DTO> lstTag;
			vector<string> v1 = tokenize(edit_list, ",", true, "");
			int len = (int)v1.size();
			for (int i = 0; i < len; i++)
			{
				dto.id = v1[i];
				dto.name = get_field_item(request_dto.szBody, "tag_name", v1[i]);
				dto.nb_images = 0;
				URLDecode(dto.name, 1);

				Replace(dto.name, ",", " ");
				esc_string(dto.name);
				trim(dto.name, " ");
				if (dto.name != "")
				{
					bool b = false;
					for (iter = lstAllTag.begin(); iter != lstAllTag.end(); iter++)
					{
						if (iter->name == dto.name)
						{
							b = true;
							break;
						}
					}

					if (!b)
					{
						for (iter = lstTag.begin(); iter != lstTag.end(); iter++)
						{
							if (iter->name == dto.name)
							{
								b = true;
								break;
							}
						}
					}

					if (!b)
					{
						lstTag.push_back(dto);
					}
				}
			}

			HRESULT hr = 100;
			if (!lstTag.empty())
			{
				hr = writedb::Instance().update_tags(mode, lstTag);
			}

			if (mode == "duplicate")
			{
				if (hr == S_OK)
				{
					CLang::Instance().getOneData("tag_infos", tag_infos);
				}
				else if (hr == 100)
				{
					CLang::Instance().getOneData("tag_error", tag_infos);
				}

				translate(tag_infos, "", "");
				Replace(tag_infos, "\\", "");
				ReplaceOne(tag_infos, "%s", dto.name);
			}


		}
		else if (mode == "merge")
		{
			string destination_tag = "";
			string merge_list = "";
			lookitem(request_dto.szBody, "destination_tag=", destination_tag);
			lookitem(request_dto.szBody, "merge_list=", merge_list);
			URLDecode(merge_list, 1);

			writedb::Instance().merge_tags(destination_tag, merge_list);
		}
		else if (mode == "delete")
		{
			string tags = "";
			string names = "";
			lookitem(request_dto.szBody, "tags=", tags);
			lookitem(request_dto.szBody, "names=", names);
			URLDecode(tags, 0);
			URLDecode(names, 1);

			writedb::Instance().delete_tags(tags);

			CLang::Instance().getOneData("tag_del_infos", tag_infos);
			translate(tag_infos, "", "");
			Replace(tag_infos, "\\", "");
			ReplaceOne(tag_infos, "%s", names);


			//tags[]: 3
			//selectAction: delete
		}
		else if (mode == "orphan")
		{
			writedb::Instance().delete_tags("");

		}
		else
		{
			string name = "";
			lookitem(request_dto.szBody, "add_tag=", name);
			URLDecode(name, 1);

			Replace(name, ",", " ");
			esc_string(name);
			trim(name, " ");
			if (name != "")
			{
				time_t current_time;
				time(&current_time);

				TAG_DTO dto;
				dto.id = "";
				dto.name = name;
				dto.lastmodified = static_cast<uint64_t>(current_time);
				dto.nb_images = 0;

				uint32_t id = 0;
				long hr = writedb::Instance().add_tag(dto, id);
				if (hr == S_OK)
				{
					CLang::Instance().getOneData("tag_infos", tag_infos);
				}
				else if (hr == 100)
				{
					CLang::Instance().getOneData("tag_error", tag_infos);
				}
				translate(tag_infos, "", "");
				Replace(tag_infos, "\\", "");
				ReplaceOne(tag_infos, "%s", name);
			}
		}

		CDataSrv::Instance().clear_cache_tags();

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.String("htm");
		writer.String(tag_infos.c_str());
		writer.String("tags");
		writer.String(edit_list.c_str());
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}
	else if (page == "admin.batch")
	{
		string mode = "";
		lookitem(request_dto.szArgs, "mode=", mode);

		if (mode == "global")
		{
			string selection = "";
			string setSelected = "";
			string selectAction = "";
			string element_ids = "";

			lookitem_arr(request_dto.szBody, "selection=", selection);
			lookitem(request_dto.szBody, "setSelected=", setSelected);
			lookitem(request_dto.szBody, "selectAction=", selectAction);
			lookitem(request_dto.szBody, "element_ids=", element_ids);
			URLDecode(element_ids, 0);

			//all
			if (setSelected == "on")
			{
				selection = element_ids;
			}

		}
		else if (mode == "unit")
		{
			//string start = "";
			//string display = "";
			//lookitem(request_dto.szArgs, "start=", start);
			//lookitem(request_dto.szArgs, "display=", display);

			//int curr = atoi(start.c_str());
			//int per = atoi(display.c_str());
			//if (per == 0)
			//{
			//	display = "5";
			//	per = 5;
			//}

			string element_ids = "";
			lookitem(request_dto.szBody, "element_ids=", element_ids);
			URLDecode(element_ids, 0);

			list<IMAGE_DTO> lstImage;
			list<IMAGE_TAG_DTO> lstImageTag;
			string selIds = "";

			vector<string> v1 = tokenize(element_ids, ",", true, "");
			int len = (int)v1.size();
			for (int i = 0; i < len; i++)
			{
				IMAGE_DTO dto;
				dto.id = v1[i];
				dto.name = get_field_item(request_dto.szBody, "name", v1[i]);
				dto.author = get_field_item(request_dto.szBody, "author", v1[i]);
				dto.comment = get_field_item(request_dto.szBody, "description", v1[i]);

				string cr = get_field_item(request_dto.szBody, "date_creation", v1[i]);
				dto.date_creation_day = get_uint64((char*)cr.c_str());

				URLDecode(dto.name, 1);
				URLDecode(dto.author, 1);
				URLDecode(dto.comment, 1);

				esc_string(dto.name);
				esc_string(dto.author);
				Replace(dto.author, ",", " ");

				trim(dto.name, " ");
				trim(dto.author, " ");

				lstImage.push_back(dto);
				selIds += v1[i] + ",";

				string tags = "";
				string key = "tags-" + v1[i] + "=";
				lookitem_arr(request_dto.szBody, key.c_str(), tags);
				if (tags != "")
				{
					URLDecode(tags, 1);

					//	add_tags[] : 11,~~1~~
					string val = "";
					get_tagids(tags, val);

					vector<string> v0 = tokenize(val, ",", true, "");
					int len0 = (int)v0.size();
					for (int j = 0; j < len0; j++)
					{
						IMAGE_TAG_DTO item;
						item.image_id = v1[i];
						item.tag_id = v0[j];

						lstImageTag.push_back(item);
					}
				}
			}

			if (!lstImage.empty())
			{
				writedb::Instance().update_unit_images(lstImage, CDataSrv::Instance()._time_zone_bias);
			}

			rtrim(selIds, ",");
			writedb::Instance().add_image_tags(selIds, lstImageTag);

			StringBuffer sb;
			Writer<StringBuffer> writer(sb);

			writer.StartObject();
			writer.String("stat");
			writer.String("ok");
			writer.EndObject();

			response_dto.szContentType = "text/plain; charset=utf-8";
			response_dto.szResponse = sb.GetString();

			return S_OK;
		}

		CDataSrv::Instance().clear_cache_tags();
		CHtmlgetIndexProc::Instance().clear_data();

		CHtmlgetBatch::Instance().proc_batch_htm(request_dto, "", tpl);
	}
	else if (page == "admin.photo")
	{
		//./admin.html?page=admin.photo&photo
		PHOTO_DTO photo_dto;
		lookitem(request_dto.szArgs, "photo=", photo_dto.id);
		lookitem(request_dto.szArgs, "category=", photo_dto.categories);

		PHOTO_DTO dto2;
		Cimagedal::Instance().get_photo(photo_dto.id, dto2);

		string date_creation = "";
		lookitem(request_dto.szBody, "creation=", date_creation);

		photo_dto.date_creation = get_uint64((char*)date_creation.c_str());

		lookitem(request_dto.szBody, "name=", photo_dto.name);
		lookitem(request_dto.szBody, "author=", photo_dto.author);
		lookitem(request_dto.szBody, "description=", photo_dto.description);
		URLDecode(photo_dto.name, 1);
		URLDecode(photo_dto.author, 1);
		URLDecode(photo_dto.description, 1);

		esc_string(photo_dto.author);
		Replace(photo_dto.author, ",", " ");


		bool isAdd = false;
		lookitem_arr(request_dto.szBody, "associate=", photo_dto.associate);

		//if (dto2.associate != photo_dto.associate)
		{
			string associate = "";
			rtrim(photo_dto.associate, ",");
			vector<string> v1 = tokenize(photo_dto.associate, ",", true, "");
			int len = (int)v1.size();
			for (int i = 0; i < len; i++)
			{
				string::size_type pos = v1[i].find("~~");
				if (pos != string::npos)
				{
					string tt = v1[i];
					associate += trim(tt, "~~") + ",";
				}
				else
				{
					time_t current_time;
					time(&current_time);

					CATEGORY_DTO catDto;
					catDto.name = v1[i];
					catDto.full_id = "0";
					catDto.lastmodified = static_cast<uint64_t>(current_time);

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
						associate += uint2str(cid) + ",";
					}
					isAdd = true;

					_trs = "";
					EnterCriticalSection(&_nbcs);
					_mapSizes.clear();
					LeaveCriticalSection(&_nbcs);

					_beginthreadex(NULL, 0, sb_nb, this, NULL, NULL);

				}
			}

			rtrim(associate, ",");
			photo_dto.associate = associate;

		}

		string tags = "";
		lookitem_arr(request_dto.szBody, "tags=", tags);
		URLDecode(tags, 1);

		string val = "";
		get_tagids(tags, val);

		photo_dto.tags = val;
		photo_dto.storage_category_id = dto2.storage_category_id;
		photo_dto.storage_category = dto2.storage_category;

		string stat = "";
		string msg = "";

		long hr = writedb::Instance().save_photo(photo_dto, CDataSrv::Instance()._time_zone_bias);
		if (hr == 0)
		{
			bool b = false;
			if (dto2.name != photo_dto.name)
			{
				b = true;
			}

			if (dto2.associate != photo_dto.associate
				|| dto2.categories != photo_dto.categories)
			{
				b = true;
				CDataSrv::Instance().clear_cache_image_ids();
			}

			if (dto2.tags != photo_dto.tags)
			{
				b = true;
				CDataSrv::Instance().clear_cache_image_ids();
				CDataSrv::Instance().clear_cache_tags();
			}

			if (b)
			{
				CHtmlgetIndexProc::Instance().clear_data();
			}

			stat = "ok";
			if (isAdd)
			{
				CDataSrv::Instance().clear_cache_image_ids();
				CHtmlgetIndexProc::Instance().clear_data();
				stat = "add";
			}
		}
		else
		{
			stat = "err";
			msg = Cimagedal::Instance().get_sqlite_error_string(hr);
		}

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String(stat.c_str());

		writer.String("msg");
		writer.String(msg.c_str());
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}
	else if (page == "admin.album")
	{
		string cat_infos = "";
		string properties = "";
		lookitem(request_dto.szArgs, "properties=", properties);
		//category=35&page=admin.album&properties=1
		int retVal = 0;

		if (properties != "")
		{
			CATEGORY_DTO cat_dto;
			lookitem(request_dto.szArgs, "category=", cat_dto.id);

			lookitem(request_dto.szBody, "name=", cat_dto.name);
			lookitem(request_dto.szBody, "parent=", cat_dto.full_id);
			lookitem(request_dto.szBody, "comment=", cat_dto.comment);
			URLDecode(cat_dto.name, 1);
			URLDecode(cat_dto.full_id, 1);
			URLDecode(cat_dto.comment, 1);

			esc_string(cat_dto.name);
			trim(cat_dto.name, " ");
			if (cat_dto.name != "")
			{
				writedb::Instance().update_categories(cat_dto.id, cat_dto);
				retVal = CHtmlgetAdminProc::Instance().move_categories(cat_dto.id, cat_dto.full_id);
				if (retVal > 0)
				{
					CDataSrv::Instance().clear_cache_image_ids();
					CHtmlgetIndexProc::Instance().clear_data();
					CLang::Instance().getOneData("cat_infos", cat_infos);
				}
				else
				{
					CLang::Instance().getOneData("cat_parent_infos", cat_infos);
				}
			}
		}
		else
		{
			string cat_id = "";
			lookitem(request_dto.szArgs, "category=", cat_id);
			string rank_of_image = request_dto.szBody;
			URLDecode(rank_of_image, 0);

			string order = "";
			string image_order_choice = "";
			lookitem(rank_of_image, "image_order_choice=", image_order_choice);

			if (image_order_choice == "default")
			{
				order = "default";
			}
			else if (image_order_choice == "rank")
			{
				list<IMAGE_CATEGORY_DTO> lstRank;
				vector<string> v1 = tokenize(rank_of_image, "&", true, "");
				uint32_t len = (uint32_t)v1.size();
				for (uint32_t i = 0; i < len; i++)
				{
					string id = "";
					string::size_type pos1 = v1[i].find("rank_");
					if (pos1 == 0)
					{
						string::size_type pos2 = v1[i].find("=");
						if (pos2 > pos1)
						{
							id = v1[i].substr(pos1 + 5, pos2 - pos1 - 5);

							IMAGE_CATEGORY_DTO dto;
							dto.image_id = id;
							dto.category_id = cat_id;
							dto.rank = uint2str(i + 1);

							lstRank.push_back(dto);
						}
					}
				}
				writedb::Instance().update_images_rank(lstRank);

				order = "b.rank asc";

			}
			else if (image_order_choice == "user_define")
			{
				string image_order = "";
				//image_order_choice=user_define&image_order%5B%5D=file+DESC&image_order%5B%5D=file+DESC&image_order%5B%5D=rating_score+DESC&rank_25=20&submit=
				lookitem_arr(rank_of_image, "image_order[]=", image_order);

				string newVal = "";
				vector<string> v1 = tokenize(image_order, ",", true, "");
				int len = (int)v1.size();
				for (int i = 0; i < len; i++)
				{
					newVal += v1[i] + ",";
				}
				trim(newVal, ",");


				order = newVal;
			}

			writedb::Instance().update_categories_imageorder(cat_id, order);

			CDataSrv::Instance().clear_cache_image_ids();
			CHtmlgetIndexProc::Instance().clear_data();

			CLang::Instance().getOneData("cat_infos", cat_infos);

			//submit=Submit&rank_1000=40&rank_994=100&rank_993=110&rank_1002=20&rank_1001=30&rank_999=50&rank_998=60&rank_996=70&rank_997=80&rank_995=90&image_order_choice=rank&image_order%5B%5D=&image_order%5B%5D=&image_order%5B%5D=
			//rank_of_image[9]: 10
			//	rank_of_image[10] : 20
			//	rank_of_image[11] : 30
			//	rank_of_image[12] : 40
			//	rank_of_image[13] : 50
			//	rank_of_image[15] : 60
			//	rank_of_image[16] : 70
		}
		translate(cat_infos, "", "");

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String("ok");

		writer.String("mov");
		writer.String((retVal == 0) ? "0" : "1");

		writer.String("htm");
		writer.String(cat_infos.c_str());
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}
	else if (page == "admin.config")
	{
		list<CONFIG_DTO> lstConfig;
		CONFIG_DTO gallery_title;
		gallery_title.param = "gallery_title";
		lookitem(request_dto.szBody, "gallery_title=", gallery_title.value);
		URLDecode(gallery_title.value, 1);
		esc_string(gallery_title.value);

		if (gallery_title.value == CDataSrv::Instance()._gallery_title)
		{
			gallery_title.value = "";
		}

		lstConfig.push_back(gallery_title);

		//CONFIG_DTO maxFileSize_dto;
		//maxFileSize_dto.param = "maxFileSize";
		//lookitem(request_dto.szBody, "maxFileSize=", maxFileSize_dto.value);

		//lstConfig.push_back(maxFileSize_dto);

		CONFIG_DTO order_by;
		order_by.param = "order_by";
		lookitem_arr(request_dto.szBody, "order_by=", order_by.value);
		URLDecode(order_by.value, 1);

		string newVal = "";
		vector<string> v1 = tokenize(order_by.value, ",", true, "");
		int len = (int)v1.size();
		for (int i = 0; i < len; i++)
		{
			newVal += v1[i] + ",";
		}
		trim(newVal, ",");

		order_by.value = newVal;
		if (order_by.value != "")
		{
			order_by.value = order_by.value;
		}

		lstConfig.push_back(order_by);

		string config_infos = "";
		HRESULT hr = writedb::Instance().add_configs(lstConfig);
		if (hr == S_OK)
		{
			CDataSrv::Instance().refresh_gallery_title();
			CLang::Instance().getOneData("batch_save_infos", config_infos);
		}

		translate(config_infos, "", "");

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.String("htm");
		writer.String(config_infos.c_str());
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		CHtmlgetIndexProc::Instance().clear_data();
		return S_OK;
	}
	else if (page == "admin.maxfilesize")
	{
		string val = "";
		lookitem(request_dto.szArgs, "size=", val);

		if (val != "")
		{
			CDataSrv::Instance().set_config_val("maxFileSize", val);
			CDataSrv::Instance()._maxFileSize = get_uint32(val.c_str());
		}

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		CHtmlgetIndexProc::Instance().clear_data();
		return S_OK;
	}

	if (page != "admin.batch" && page != "admin.album" && page != "admin.photo")
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

long CHtmlgetAdmin::post_ws_php(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string method = "";
	lookitem(request_dto.szArgs, "method=", method);
	if (method == "")
	{
		lookitem(request_dto.szBody, "method=", method);
	}

	if (method == "ws.menubartags")
	{
		string menubar_tags = "";
		string src = "";
		lookitem(request_dto.szBody, "src=", src);
		URLDecode(src, 0);

		string::size_type  pos = src.find("?");
		if (pos != string::npos)
		{
			src = src.substr(pos + 1);
		}

		string no_tag = "";
		string cal = "";
		string week = "";
		string details = "";
		lookitem(src, "cal=", cal);
		lookitem(src, "week=", week);
		lookitem(src, "details=", details);
		lookitem(src, "no_tag=", no_tag);

		if (no_tag == "")
		{
			if ((cal == "calendar" && details == "") || (cal == "weekly" && week == ""))
			{
				QUERY_REQ req_temp;

				REQUEST_DTO dto;
				dto.szUrl = "";
				dto.szArgs = src;
				dto.szBody = "";
				dto.szFile = L"";
				dto.host = "";
				get_req(1, dto, req_temp);

				list <string> lstImageIds;
				if (req_temp.search == "q")
				{
					Cimagedal::Instance().get_image_ids(req_temp, CDataSrv::Instance()._search_req, lstImageIds);
				}
				else
				{
					Cimagedal::Instance().get_image_ids(req_temp, lstImageIds);
				}

				string image_ids = "";
				list<string>::iterator iter;
				for (iter = lstImageIds.begin(); iter != lstImageIds.end(); iter++)
				{
					image_ids += *iter + ",";
				}
				rtrim(image_ids, ",");

				if (image_ids != "")
				{
					menubar_tags = CDataSrv::Instance().get_menubar_tags(0, "", image_ids);
				}
			}
			else
			{
				menubar_tags = CDataSrv::Instance().get_menubar_tags(0, "", "");
			}
		}

		translate(menubar_tags, "", "");

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.String("htm");
		writer.String(menubar_tags.c_str());
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();
		return S_OK;
	}
	else if (method == "categories.add")
	{
		//parent: 0
		//	name : open1
		string parent = "";
		string name = "";
		lookitem(request_dto.szBody, "parent=", parent);
		lookitem(request_dto.szBody, "name=", name);
		URLDecode(name, 1);

		uint32_t id = 0;
		esc_string(name);
		trim(name, " ");
		if (name != "")
		{
			time_t current_time;
			time(&current_time);

			CATEGORY_DTO category;
			category.name = name;
			category.full_id = parent;
			category.addby = 0;

			category.lastmodified = static_cast<uint64_t>(current_time);
			if (category.full_id == "")
			{
				category.full_id = "0";
			}

			bool b = false;
			uint32_t id9 = 0;
			string parentDir = "";

			list<CATEGORY_DTO> lst;
			CHtmlgetMenubar::Instance().get_categories_order_by_rank(0, lst);

			list<CATEGORY_DTO>::iterator iter;
			for (iter = lst.begin(); iter != lst.end(); iter++)
			{
				if (iter->id == parent)
				{
					parentDir = iter->dir0;
					id9 = atoi(iter->root_id.c_str());
				}
			}

			for (iter = lst.begin(); iter != lst.end(); iter++)
			{
				if (iter->name == category.name)
				{
					if (category.full_id == "0")
					{
						if (iter->id == iter->full_id)
						{
							b = true;
							break;
						}
					}
					else
					{
						string tmp = "," + category.full_id + ",";
						string uppercats = "," + iter->full_id + ",";

						string::size_type pos = uppercats.find(tmp);
						if (pos != string::npos)
						{
							b = true;
							break;
						}
					}
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

				if (parentDir != "")
				{
					my_tolower(parentDir);

					path0 = _u2w(parentDir) + L"/" + _u2w(category.name);
					path1 = _u2w(parentDir) + L"/" + _u2w(category.name);

					ReplaceOne(path1, szPath, L".");

				}
				else
				{
					path0 += L"/" + _u2w(category.name);
					path1 += L"/" + _u2w(category.name);

					ROOT_DTO root;
					root.root_dir0 = _w2u(path1);
					root.root_sizes = 0;
					writedb::Instance().add_root(root, id9);

					_trs = "";
					EnterCriticalSection(&_nbcs);
					_mapSizes.clear();
					LeaveCriticalSection(&_nbcs);

					_beginthreadex(NULL, 0, sb_nb, this, NULL, NULL);
				}

				dwSize = GetFullPathName(path0.c_str(), MAX_PATH, szBuf, NULL);
				CreateDirectory(szBuf, NULL);

				category.root_id = uint2str(id9);
				category.dir0 = _w2u(path1);

				writedb::Instance().add_category(category, id);
			}
		}

		if (id != 0)
		{
			InfoDto infoDto;
			infoDto.id_ = id;
			infoDto.info_ = name;

			StringBuffer sb;
			Writer<StringBuffer> writer(sb);

			writer.StartObject();
			writer.String("stat");
			writer.String("ok");

			writer.String("result");
			infoDto.Serialize(writer);
			writer.EndObject();

			response_dto.szContentType = "text/plain; charset=utf-8";
			response_dto.szResponse = sb.GetString();

			CDataSrv::Instance()._acc = "2";
		}
		else
		{
			StringBuffer sb;
			Writer<StringBuffer> writer(sb);

			writer.StartObject();
			writer.String("stat");
			writer.String("err");
			writer.EndObject();

			response_dto.szContentType = "text/plain; charset=utf-8";
			response_dto.szResponse = sb.GetString();
		}

		return 0;
	}
	else if (method == "categories.refreshRepresentative")
	{
		string category_id = "";
		string representative = "";
		lookitem(request_dto.szArgs, "representative=", representative);
		lookitem(request_dto.szBody, "category_id=", category_id);

		string picid = "";
		string src = "";
		string src0 = "";
		string ori = "";
		string url = "";
		string ext = "";
		string duration = "";

		IMAGE_DTO dto;
		long hr = 1;
		Cimagedal::Instance().get_rand_images(category_id, representative, dto);
		if (dto.id != "")
		{
			ext = dto.ext;
			picid = dto.id;
			ori = CDataSrv::Instance().get_image_url(1, dto);
			src = CDataSrv::Instance().get_image_url(0, dto);
			url = "./admin.html?page=admin.box&id=" + dto.id;
			src0 = "./admin.html?page=admin.photo&photo=" + dto.id;
			duration = get_dur_string(dto.duration);

			hr = 0;
		}

		if (picid != "")
		{
			hr = writedb::Instance().update_categories_representative(category_id, picid);
		}

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		if (hr == 0)
		{
			writer.StartObject();
			writer.String("stat");
			writer.String("ok");
			//photo
			writer.String("result");
			writer.StartObject();
			writer.String("ext");
			writer.String(ext.c_str());
			writer.String("photo");
			writer.String(picid.c_str());
			writer.String("duration");
			writer.String(duration.c_str());
			writer.String("src");
			writer.String(src.c_str());
			writer.String("src0");
			writer.String(src0.c_str());
			writer.String("ori");
			writer.String(ori.c_str());
			writer.String("url");
			writer.String(url.c_str());
			writer.EndObject();
			writer.EndObject();
		}
		else
		{
			writer.StartObject();
			writer.String("stat");
			writer.String("error");
			writer.EndObject();
		}

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return 0;
	}
	else if (method == "ws.associated")
	{
		long hr = 0;
		map<string, string> cats_map;

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		list<CATEGORY_DTO> lst;
		CHtmlgetMenubar::Instance().get_order_categories("0", lst);

		list<CATEGORY_DTO>::iterator iter;
		for (iter = lst.begin(); iter != lst.end(); iter++)
		{
			string uppercats = iter->full_id;
			string fullname = "";
			get_fullname(uppercats, &lst, fullname);

			if (fullname == "")
			{
				fullname = iter->name;
			}

			cats_map.insert(map<string, string>::value_type(iter->id, fullname));
		}


		if (!cats_map.empty())
		{
			writer.StartObject();
			writer.String("stat");
			writer.String("ok");
			//photo
			writer.String("result");
			writer.StartArray();

			map<string, string>::iterator Im;
			for (Im = cats_map.begin(); Im != cats_map.end(); Im++)
			{
				writer.StartObject();
				writer.String("id");
				writer.String(Im->first.c_str());
				writer.String("name");
				writer.String(Im->second.c_str());
				writer.EndObject();
			}
			writer.EndArray();
			writer.EndObject();
		}
		else
		{
			writer.StartObject();
			writer.String("stat");
			writer.String("error");
			writer.EndObject();
		}

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return 0;
	}
	else if (method == "batch.associate")
	{
		string photos = "";
		string associate = "";
		lookitem(request_dto.szBody, "photos=", photos);
		lookitem(request_dto.szBody, "associate=", associate);
		//associate: 35
		if (photos == "all")
		{
			photos = CHtmlgetBatch::Instance()._element_ids;
		}

		if (associate != "")
		{
			writedb::Instance().update_images("associate", photos, associate);
		}
	}
	else if (method == "batch.move")
	{
		string associate = "";
		string photos = "";
		lookitem(request_dto.szBody, "photos=", photos);
		lookitem(request_dto.szBody, "associate=", associate);
		if (photos == "all")
		{
			photos = CHtmlgetBatch::Instance()._element_ids;
		}

		if (associate != "")
		{
			writedb::Instance().update_images("move", photos, associate);
		}
		//	selectAction : move
		//	associate : 35
	}
	else if (method == "batch.duplicates")
	{
		string stat = "ok";
		//duplicates: 35

		list<string> lstImageIds;
		Cimagedal::Instance().get_duplicates(lstImageIds);
		if (!lstImageIds.empty())
		{
			string ids = "";
			list<string>::iterator iter;

			for (iter = lstImageIds.begin(); iter != lstImageIds.end(); iter++)
			{
				ids += *iter + ",";
			}
			rtrim(ids, ",");
			writedb::Instance().update_images("duplicates", ids, "");
		}
		else
		{
			stat = "err";
		}

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String(stat.c_str());
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}
	else if (method == "batch.add_tags" || method == "batch.append_tags")
	{
		string photos = "";
		string tags = "";
		lookitem(request_dto.szBody, "photos=", photos);
		lookitem_arr(request_dto.szBody, "add_tags=", tags);
		URLDecode(tags, 1);
		//	add_tags[] : 11,~~1~~

		if (photos == "all")
		{
			photos = CHtmlgetBatch::Instance()._element_ids;
		}

		string val = "";
		get_tagids(tags, val);

		if (val != "" && photos != "")
		{
			writedb::Instance().update_images("add_tags", photos, val);
		}

	}
	else if (method == "batch.add_tags2")
	{
		string tags = "";
		lookitem_arr(request_dto.szBody, "add_tags=", tags);
		URLDecode(tags, 1);

		time_t current_time;
		time(&current_time);

		TAG_DTO dto;
		dto.id = "";
		dto.name = tags;
		dto.lastmodified = static_cast<uint64_t>(current_time);
		dto.nb_images = 0;

		uint32_t id = 0;
		writedb::Instance().add_tag(dto, id);

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.String("id");
		writer.String(Int2Str(id).c_str());
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return 0;

	}
	else if (method == "batch.update_tags")
	{
		string photos = "";
		string tags = "";
		lookitem(request_dto.szBody, "photos=", photos);
		lookitem_arr(request_dto.szBody, "add_tags=", tags);
		URLDecode(tags, 1);
		//	add_tags[] : 11,~~1~~

		if (photos == "all")
		{
			photos = CHtmlgetBatch::Instance()._element_ids;
		}
		string val = "";
		get_tagids(tags, val);

		writedb::Instance().update_images("update_tags", photos, val);

	}
	else if (method == "batch.del_tags")
	{
		string photos = "";
		string tags = "";
		lookitem(request_dto.szBody, "photos=", photos);
		lookitem_arr(request_dto.szBody, "del_tags=", tags);
		URLDecode(tags, 1);

		if (photos == "all")
		{
			photos = CHtmlgetBatch::Instance()._element_ids;
		}
		//	add_tags[] : ~~9~~
		if (tags != "")
		{
			writedb::Instance().update_images("del_tags", photos, tags);
		}
	}
	else if (method == "batch.author")
	{

		string photos = "";
		string author = "";
		string remove_author = "";
		lookitem(request_dto.szBody, "photos=", photos);
		lookitem(request_dto.szBody, "author=", author);
		lookitem(request_dto.szBody, "remove_author=", remove_author);
		URLDecode(author, 1);
		esc_string(author);
		Replace(author, ",", " ");
		trim(author, " ");

		if (photos == "all")
		{
			photos = CHtmlgetBatch::Instance()._element_ids;
		}

		if (remove_author == "on")
		{
			author = "";
			writedb::Instance().update_images("author", photos, author);
		}
		else
		{
			if (author != "")
			{
				writedb::Instance().update_images("author", photos, author);
			}
		}
		//	author : vvvvvvvvvvvv
		//	remove_author : on

	}
	else if (method == "batch.title")
	{
		string photos = "";
		string title = "";
		string remove_title = "";
		lookitem(request_dto.szBody, "photos=", photos);
		lookitem(request_dto.szBody, "title=", title);
		lookitem(request_dto.szBody, "remove_title=", remove_title);
		URLDecode(title, 1);
		esc_string(title);
		//	title : bbbbbb
		//	remove_title : on
		trim(title, " ");

		if (photos == "all")
		{
			photos = CHtmlgetBatch::Instance()._element_ids;
		}

		if (remove_title == "on")
		{
			title = "";
			writedb::Instance().update_images("title", photos, title);
		}
		else
		{
			if (title != "")
			{
				writedb::Instance().update_images("title", photos, title);
			}
		}
	}
	else if (method == "batch.add_to_caddie")
	{
		string photos = "";
		lookitem(request_dto.szBody, "photos=", photos);
		if (photos == "all")
		{
			photos = CHtmlgetBatch::Instance()._element_ids;
		}
		writedb::Instance().add_caddie(CDataSrv::Instance()._login, "add", photos);
	}
	else if (method == "galleries.categories")
	{
		galleries_categories(request_dto, response_dto);
		return S_OK;
	}
	else if (method == "galleries.change")
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

		EnterCriticalSection(&_nbcs);
		bool b = folder_ischange();
		LeaveCriticalSection(&_nbcs);

		string stat = "err";
		if (b)
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
	else if (method == "galleries.remove")
	{
		if (CDataSrv::Instance()._uinit == 0)
		{
			CDataSrv::Instance()._uinit = 1;

			if (CDataSrv::Instance()._shutdownEvent != NULL)
			{
				WaitForSingleObject(CDataSrv::Instance()._shutdownEvent, INFINITE);
			}
			_predict = 0;
			_nbTrain = 0;
		}

		galleries_remove(request_dto, response_dto);
		return S_OK;
	}
	else if (method == "galleries.before")
	{
		string val = "";
		CDataSrv::Instance().get_config_val("last_import", val);

		time_t current_time;
		time(&current_time);
		val = uint64str(static_cast<uint64_t>(current_time)) + "," + val;
		CDataSrv::Instance().set_config_val("last_import", val);

		CHtmlupload::Instance().set_roots();

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
	else if (method == "galleries.clear")
	{
		EnterCriticalSection(&_nbcs);
		galleries_clear(request_dto, response_dto);
		LeaveCriticalSection(&_nbcs);
		return S_OK;
	}
	else if (method == "ws.progress")
	{
		if (_progress_step >= _progress_len)
		{
			string root_id = "";
			lookitem(request_dto.szArgs, "root_id=", root_id);

			processok(root_id);

			while (!_queUpload.empty())
			{
				_queUpload.pop();
			}

			EnterCriticalSection(&_nbcs);
			_mapSizes.clear();
			nb_proc();
			LeaveCriticalSection(&_nbcs);
		}

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.String("step");
		writer.String(uint2str(_progress_step).c_str());
		writer.String("len");
		writer.String(uint2str(_progress_len).c_str());
		writer.EndObject();

		response_dto.szResponse = sb.GetString();
		response_dto.szContentType = "text/plain; charset=utf-8";
		return S_OK;
	}
	else if (method == "ws.progress3")
	{
		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.String("step");
		writer.String(uint2str(_progress_step3).c_str());
		writer.String("len");
		writer.String(uint2str(_progress_len3).c_str());
		writer.String("real");
		string real = "";
		if (_nb_gg2_thread == 0)
		{
			real = "ok";
		}
		writer.String(real.c_str());
		writer.EndObject();

		response_dto.szResponse = sb.GetString();
		response_dto.szContentType = "text/plain; charset=utf-8";
		return S_OK;
	}
	else if (method == "ws.cancel3")
	{
		_progress_cancel = 1;
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
	else if (method == "ws.profile2_ok")
	{
		string stat = "err";
		if (CDataSrv::Instance()._albumName == "private")
		{
			string password = "";
			string use_new_pwd = "";
			string passwordConf = "";
			string hint = "";

			lookitem(request_dto.szBody, "password=", password);
			lookitem(request_dto.szBody, "hint=", hint);
			lookitem(request_dto.szBody, "use_new_pwd=", use_new_pwd);
			lookitem(request_dto.szBody, "passwordConf=", passwordConf);
			URLDecode(password, 0);
			URLDecode(hint, 1);
			URLDecode(use_new_pwd, 0);
			URLDecode(passwordConf, 0);

			trim(password, " ");
			trim(use_new_pwd, " ");
			trim(passwordConf, " ");
			trim(hint, " ");


			bool b = false;
			list<USER_DTO> lstUsers;
			Cchatdal::Instance().get_users(lstUsers);
			list<USER_DTO>::iterator I;
			for (I = lstUsers.begin(); I != lstUsers.end(); I++)
			{
				if (I->username == "admin" && I->password == password)
				{
					b = true;
					break;
				}
			}

			//password
			if (use_new_pwd != "" && use_new_pwd == passwordConf)
			{
				if (b)
				{
					USER_DTO users_dto;
					users_dto.username = "admin";
					users_dto.password = use_new_pwd;
					users_dto.hint = hint;
					writedb::Instance().updte_users(users_dto);
					stat = "ok";
				}
			}
			else
			{
				if (password == "")
				{
					USER_DTO users_dto;
					users_dto.username = "admin";
					users_dto.password = "";
					users_dto.hint = hint;
					writedb::Instance().updte_users(users_dto);

					stat = "ok";
				}
			}
		}
		else
		{
			stat = "ok";
		}

		if (stat == "ok")
		{
			string language = "";
			lookitem(request_dto.szBody, "language=", language);
			if (language != "" && language != CLang::Instance()._language)
			{
				CDataSrv::Instance().set_config_val("language", language);
				CDataSrv::Instance().WritePrivate("language", language.c_str());
				CLang::Instance()._language = language;
				CLang::Instance().refresh_language();

				CDataSrv::Instance().refresh_gallery_title();
			}
		}

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String(stat.c_str());

		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();
		return S_OK;
	}
	else if (method == "ws.skins")
	{
		string stat = "ok";
		string skin = "";

		lookitem(request_dto.szBody, "skin=", skin);

		CDataSrv::Instance().set_config_val("skin", skin);
		CDataSrv::Instance().WritePrivate("skin", skin.c_str());
		CDataSrv::Instance()._skin = skin;

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String(stat.c_str());

		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();
		return S_OK;
	}
	else if (method == "ws.bg")
	{
		string stat = "err";
		string id = "";
		string bgtype = "";

		lookitem(request_dto.szArgs, "id=", id);
		lookitem(request_dto.szArgs, "bgtype=", bgtype);

		string bg = "";
		if (bgtype != "")
		{
			CDataSrv::Instance().set_config_val("bgtype", bgtype);
			CDataSrv::Instance()._bgtype = bgtype;
			stat = "ok";
		}
		else
		{
			if (id == "-1")
			{
				stat = "ok";
				CDataSrv::Instance()._bg = "";

			}
			else if (id == "-2")
			{
				stat = "ok";
				string custom_bg = "";
				CDataSrv::Instance().get_config_val("custom_bg", custom_bg);

				list<IMAGE_DTO> lstImage;
				Cimagedal::Instance().get_images_by_ids(custom_bg, lstImage);
				if (!lstImage.empty())
				{
					CDataSrv::Instance()._bg = CDataSrv::Instance().get_image_url(0, lstImage.front());

				}
				bg = CDataSrv::Instance()._bg;

			}
			else if (id == "0")
			{
				stat = "ok";
				bg = "./background/default.jpg";
				CDataSrv::Instance()._bg = bg;
			}
			else
			{
				CDataSrv::Instance().set_config_val("custom_bg", id);
				list<IMAGE_DTO> lstImage;
				Cimagedal::Instance().get_images_by_ids(id, lstImage);
				if (!lstImage.empty())
				{
					stat = "ok";
					bg = CDataSrv::Instance().get_image_url(0, lstImage.front());
					CDataSrv::Instance()._bg = bg;

				}
				id = "-2";
			}
			CDataSrv::Instance().set_config_val("sel_bg", id);
		}

		string op = ".75";

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String(stat.c_str());
		writer.String("skin");
		writer.String(CDataSrv::Instance()._skin.c_str());
		writer.String("bg");
		writer.String(bg.c_str());
		writer.String("opacity");
		writer.String(op.c_str());

		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();
		return S_OK;
	}
	else if (method == "ws.register_ok")
	{
		string stat = "ok";
		HRESULT hr = proc_registr(request_dto);
		if (hr == S_FALSE)
		{
			stat = "err";
		}
		else
		{
			CDataSrv::Instance()._acc = "2";

			string enckey = "123456";
			CDataSrv::Instance().get_config_val("enckey", enckey);
			set_key(enckey.c_str());
		}

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String(stat.c_str());

		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}
	else if (method == "ws.login_ok")
	{
		string stat = "ok";
		HRESULT hr = proc_login(request_dto);
		if (hr == S_FALSE)
		{
			stat = "err";
		}
		else
		{
			CDataSrv::Instance()._acc = "2";

			string enckey = "123456";
			CDataSrv::Instance().get_config_val("enckey", enckey);
			set_key(enckey.c_str());
		}

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String(stat.c_str());

		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();
		return S_OK;
	}
	else if (method == "ws.update_art")
	{
		CHtmlgetIndexProc::Instance().update_art_title(request_dto, response_dto);
		return S_OK;
	}
	else if (method == "ws.viewer")
	{
		CHtmlgetViewer::Instance().get_viewer_htm(request_dto, response_dto);
		return S_OK;
	}
	else if (method == "ws.progressface")
	{
		string ty = "";
		lookitem(request_dto.szArgs, "ty=", ty);
		if (ty == "0" || ty == "3")
		{
			_faceIds = "";
		}

		int nb = 0;
		if (ty == "3")
		{
			wstring path0 = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);

			string alb = CDataSrv::Instance()._albumName;
			if (alb == "private")
			{
				path0 += L"\\data2\\_train\\";
			}
			else
			{
				path0 += L"\\data1\\_train\\";
			}

			list<string> lstId;
			Cimagedal::Instance().get_images_face(lstId);
			nb = (int)lstId.size();
			if (nb == 0)
			{
				wstring path1 = path0 + L"_temp\\";
				wchar_t szBuf[MAX_PATH] = { 0 };
				DWORD dwSize = GetFullPathName(path1.c_str(), MAX_PATH, szBuf, NULL);

				list<WIN32_FIND_DATA > vecffd;
				CHtmlgetFolder::Instance().GetFolderPics(1, szBuf, vecffd);
				nb = (int)vecffd.size();
			}

			list<FACE_DTO> tempDto;
			Cimagedal::Instance().get_face("", "", tempDto);
			if (tempDto.empty())
			{
				wstring dst = path0 + L"_model\\face_model.yml";
				my_DeleteFile(dst);
			}
		}

		string single = "";
		string group = "";

		list<FACE_DTO> tempDto;

		if (_nbTrain >= _predict)
		{
			Cimagedal::Instance().get_face("", "", tempDto);
		}
		else
		{
			Cimagedal::Instance().get_face("", _faceIds, tempDto);
		}

		if (!tempDto.empty())
		{
			string path = "data1/_train/_face/";
			if (CDataSrv::Instance()._albumName == "private")
			{
				path = "data2/_train/_face/";
			}

			string face_li = "";
			CLang::Instance().getOneData("face_li", face_li);

			int i = 0;
			list<FACE_DTO>::iterator iter;
			for (iter = tempDto.begin(); iter != tempDto.end(); iter++)
			{
				_faceIds += iter->id + ",";

				string temp = face_li;

				string name2 = path + iter->name;
				ReplaceOne(temp, "{$name}", name2);
				Replace(temp, "{$id}", iter->id);
				ReplaceOne(temp, "{$merge_id}", iter->merge_id);

				if (_nbTrain >= _predict)
				{
					ReplaceOne(temp, "{$title}", Int2Str(iter->nb));
					if (i < 9)
					{
						single += temp;
					}
					else
					{
						group += temp;
					}
				}
				else
				{
					ReplaceOne(temp, "{$title}", "");
					if (iter->nb > 1)
					{
						single += temp;
					}
					else
					{
						group += temp;
					}
				}
				i++;
			}
			translate(single, "", "");
			translate(group, "", "");
		}

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.String("step");
		writer.String(uint2str(_nbTrain).c_str());
		writer.String("len");
		writer.String(uint2str(_predict).c_str());
		writer.String("single");
		writer.String(single.c_str());
		writer.String("group");
		writer.String(group.c_str());
		writer.String("nb");
		writer.String(uint2str(nb).c_str());
		writer.EndObject();

		response_dto.szResponse = sb.GetString();
		response_dto.szContentType = "text/plain; charset=utf-8";
		return S_OK;
	}
	else if (method == "ws.showmerge")
	{
		string ids = "";
		lookitem(request_dto.szArgs, "ids=", ids);

		string path = "data1/_train/_face/";
		if (CDataSrv::Instance()._albumName == "private")
		{
			path = "data2/_train/_face/";
		}

		list<FACE_DTO> tempDto;
		Cimagedal::Instance().get_face(ids, "", tempDto);

		string label = "";
		string tag_merge_form_label = "";
		CLang::Instance().getOneData("face_merge_form_label", tag_merge_form_label);

		int i = 0;
		list<FACE_DTO>::iterator iter;
		for (iter = tempDto.begin(); iter != tempDto.end(); iter++)
		{
			string temp = tag_merge_form_label;
			Replace(temp, "{$id}", iter->id);
			Replace(temp, "{$name}", path + iter->name);
			if (i == 0)
			{
				Replace(temp, "g='checked'", "checked=\"checked\"");
			}
			else
			{
				Replace(temp, "g='checked'", "");
			}

			label += temp;
			i++;
		}
		translate(label, "", "");

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.String("face");
		writer.String(label.c_str());
		writer.EndObject();

		response_dto.szResponse = sb.GetString();
		response_dto.szContentType = "text/plain; charset=utf-8";
		return S_OK;
	}
	else if (method == "ws.merge")
	{
		string destination_tag = "";
		string merge_list = "";
		lookitem(request_dto.szBody, "destination_tag=", destination_tag);
		lookitem(request_dto.szBody, "merge_list=", merge_list);
		URLDecode(merge_list, 1);

		writedb::Instance().merge_face(destination_tag, merge_list);

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
	else if (method == "ws.removeface")
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

		string ids = "";
		lookitem(request_dto.szArgs, "ids=", ids);
		list<string> lstFile;
		uint32_t nb = 0;
		writedb::Instance().delete_face(ids, lstFile, nb);

		if (nb == 0)
		{
			list<string>::iterator iter;
			for (iter = lstFile.begin(); iter != lstFile.end(); iter++)
			{
				wstring dst = path0 + L"_face/" + _u2w(iter->c_str());
				if (alb == "private")
				{
					dst += L".krf";
				}
				my_DeleteFile(dst);
			}

			wstring dst = path0 + L"_model/face_model.yml";
			my_DeleteFile(dst);
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
	else if (method == "ws.predict")
	{
		if (_predict == 0)
		{
			list<FACE_DTO> tempDto;
			Cimagedal::Instance().get_face("", "", tempDto);
			list<FACE_DTO>::iterator iter;
			for (iter = tempDto.begin(); iter != tempDto.end(); iter++)
			{
				_faceIds += iter->id + ",";
			}

			_lstImageId.clear();
			Cimagedal::Instance().get_images_face(_lstImageId);
			_predict = (int)_lstImageId.size();
			_nbTrain = 0;

			if (_predict == 0)
			{
				wstring path0 = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);
				wchar_t szBuf[MAX_PATH] = { 0 };
				DWORD dwSize = GetFullPathName(path0.c_str(), MAX_PATH, szBuf, NULL);
				wstring tmp = wstring(szBuf) + L"\\_train\\_temp";

				map<string, list<string>> mapFiles;
				get_group_face(tmp, mapFiles);

				_predict = (int)mapFiles.size();
			}

			if (_predict > 0)
			{
				_beginthreadex(NULL, 0, sb_predict, this, NULL, NULL);
			}
		}

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String("ok");

		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();
		return S_OK;
	}

	CDataSrv::Instance().clear_cache_tags();
	CHtmlgetIndexProc::Instance().clear_data();

	if (method == "batch.append_tags")
	{
		string menubar_tags = CDataSrv::Instance().get_menubar_tags(0, "", "");
		translate(menubar_tags, "", "");

		StringBuffer sb;
		Writer<StringBuffer> writer(sb);

		writer.StartObject();
		writer.String("stat");
		writer.String("ok");

		writer.String("htm");
		writer.String(menubar_tags.c_str());
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();
		return S_OK;
	}

	response_dto.szResponse = "ok";
	response_dto.szContentType = "text/plain; charset=utf-8";
	return S_OK;
}

UINT __stdcall CHtmlgetAdmin::sb_predict(LPVOID pParam)
{
	CHtmlgetAdmin* pThis = (CHtmlgetAdmin*)pParam;
	pThis->predict_proc();
	return 0;
}

void CHtmlgetAdmin::predict_proc()
{
	CDataSrv::Instance()._uinit = 0;

	if (CDataSrv::Instance()._shutdownEvent == NULL)
	{
		CDataSrv::Instance()._shutdownEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	}

	ResetEvent(CDataSrv::Instance()._shutdownEvent);


	wstring path0 = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);
	string enckey = "";
	string alb = CDataSrv::Instance()._albumName;
	if (alb == "private")
	{
		string val = "";
		CDataSrv::Instance().get_config_val("enckey", val);
		enckey = val;
		path0 += L"\\data2";
	}
	else
	{
		path0 += L"\\data1";
	}

	wchar_t szBuf[MAX_PATH] = { 0 };
	DWORD dwSize = GetFullPathName(path0.c_str(), MAX_PATH, szBuf, NULL);
	CreateDirectory(szBuf, NULL);

	path0 = wstring(szBuf) + L"\\_train";
	CreateDirectory(path0.c_str(), NULL);

	path0 = wstring(szBuf) + L"\\_train\\_face";
	CreateDirectory(path0.c_str(), NULL);

	path0 = wstring(szBuf) + L"\\_train\\_model";
	CreateDirectory(path0.c_str(), NULL);

	wstring tmp = wstring(szBuf) + L"\\_train\\_temp";
	CreateDirectory(tmp.c_str(), NULL);
	tmp += L"\\";
	string strPath = _w2u(tmp);

	list<ROOT_DTO> lstRoot;
	CDataSrv::Instance().get_roots(lstRoot);

	list<string>::iterator it;

	_predict = (int)_lstImageId.size();
	_nbTrain = 0;

	list<FACE_DTO> lstout;
	Cimagedal::Instance().get_face("all", "", lstout);

	for (;;)
	{
		if (CDataSrv::Instance()._uinit == 1)
		{
			break;
		}

		this_thread::sleep_for(chrono::nanoseconds(10));

		int i = 0;
		string ids = "";

		for (it = _lstImageId.begin(); it != _lstImageId.end(); )
		{
			i++;
			if (i < 6)
			{
				ids += *it + ",";
				it = _lstImageId.erase(it);
			}
			else
			{
				it++;
				break;
			}
		}

		if (ids != "")
		{
			string trainlist_txt = "";
			list<IMAGE_DTO> tempDto;
			list<IMAGE_DTO>::iterator iter;

			Cimagedal::Instance().get_images_by_ids(ids, tempDto);
			for (iter = tempDto.begin(); iter != tempDto.end(); iter++)
			{
				string ori = ori2path(lstRoot, 1, iter->ori);

				trainlist_txt += iter->id + "|";
				trainlist_txt += ori;
				if (alb == "private")
				{
					trainlist_txt += ".krf";
				}
				trainlist_txt += "\r\n";
			}

			char* mlabels = NULL;
			int ret = predict(1, enckey.c_str(), trainlist_txt.c_str(), &mlabels);
			if (ret == 1)
			{
				break;
			}

			list<FACE_DTO> lstface;
			if (mlabels != NULL)
			{
				vector<string> v1 = tokenize(mlabels, "\r\n", true, "");

				int len = (int)v1.size();
				for (int i = 0; i < len; i++)
				{
					vector<string> v2 = tokenize(v1[i], "|", true, "");

					FACE_DTO dto;
					dto.id = v2[0];
					dto.image_id = v2[1];
					dto.name = v2[2];
					dto.al = v2[3];
					dto.dict = v2[4];

					lstface.push_back(dto);
				}
				free(mlabels);
			}
			writedb::Instance().add_face(ids, lstface, lstout);

			if (_nbTrain + (int)tempDto.size() < _predict)
			{
				_nbTrain += (int)tempDto.size();
			}
		}
		else
		{
			break;
		}
	}

	if (CDataSrv::Instance()._uinit == 1)
	{
		SetEvent(CDataSrv::Instance()._shutdownEvent);
		return;
	}

	map<string, list<string>> mapFiles;
	get_group_face(tmp, mapFiles);

	map<string, list<string>> ::iterator it2;

	bool b1 = false;
	if (_predict == 0)
	{
		_predict = (int)mapFiles.size();
		_nbTrain = 0;
		b1 = true;
	}

	for (;;)
	{
		if (CDataSrv::Instance()._uinit == 1)
		{
			break;
		}

		this_thread::sleep_for(chrono::nanoseconds(10));

		string trainlist_txt = "";
		for (it2 = mapFiles.begin(); it2 != mapFiles.end(); it2++)
		{
			for (it = it2->second.begin(); it != it2->second.end(); it++)
			{
				string oo = *it;
				string::size_type pos = oo.find(".");
				if (pos != string::npos)
				{
					oo = oo.substr(0, pos);
				}
				trainlist_txt += oo + "|";
				trainlist_txt += strPath + *it;
				trainlist_txt += "\r\n";
			}

			mapFiles.erase(it2);
			break;
		}

		if (trainlist_txt != "")
		{
			char* mlabels = NULL;
			int ret = predict(0, enckey.c_str(), trainlist_txt.c_str(), &mlabels);
			if (ret == 1)
			{
				break;
			}

			list<FACE_DTO> lstface;
			if (mlabels != NULL)
			{
				vector<string> v1 = tokenize(mlabels, "\r\n", true, "");

				int len = (int)v1.size();
				for (int i = 0; i < len; i++)
				{
					vector<string> v2 = tokenize(v1[i], "|", true, "");

					FACE_DTO dto;
					dto.id = v2[0];
					dto.image_id = v2[1];
					dto.name = v2[2];
					dto.al = v2[3];
					dto.dict = v2[4];

					lstface.push_back(dto);
				}
				LocalFree(mlabels);
			}
			writedb::Instance().add_face("", lstface, lstout);

			vector<string> v1 = tokenize(trainlist_txt, "\r\n", true, "");
			int len = (int)v1.size();
			for (int i = 0; i < len; i++)
			{
				vector<string> v2 = tokenize(v1[i], "|", true, "");
				if ((int)v2.size() == 2)
				{
					wstring  dst = _u2w(v2[1]);
					my_DeleteFile(dst);
				}
			}

			if (b1)
			{
				_nbTrain++;
			}
		}
		else
		{
			_predict = 0;
			break;
		}
	}

	SetEvent(CDataSrv::Instance()._shutdownEvent);
}

void CHtmlgetAdmin::get_group_face(wstring strPath, map<string, list<string>>& mapFiles)
{
	map<string, list<string>> ::iterator it;

	list<WIN32_FIND_DATA > vecffd;
	CHtmlgetFolder::Instance().GetFolderPics(1, strPath, vecffd);
	list<WIN32_FIND_DATA>::iterator it0;

	for (it0 = vecffd.begin(); it0 != vecffd.end(); it0++)
	{
		string oo = _w2u(it0->cFileName);
		string::size_type pos = oo.find(".");
		if (pos != string::npos)
		{
			string temp = oo.substr(0, pos - 1);

			it = mapFiles.find(temp);
			if (it != mapFiles.end())
			{
				it->second.push_back(oo);
			}
			else
			{
				list<string> lstFiles;
				lstFiles.push_back(oo);
				mapFiles.insert(map<string, list<string>> ::value_type(temp, lstFiles));
			}
		}
	}

}