#include "StdAfx.h"
#include "unitil.h"
#include <vector>
#include <string>

#include "../../templ/templ/resource.h"

#include "HtmlgetpicProc.h"
#include "lang.h"
#include "imagedal.h"
#include "chatdal.h"
#include "DataSrv.h"
#include "Htmlexif.h"
#include "HtmlgetMenubar.h"
#include "writedb.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

CHtmlgetpicProc::CHtmlgetpicProc(void)
{
}

CHtmlgetpicProc::~CHtmlgetpicProc(void)
{
}

string CHtmlgetpicProc::get_meta_exif(string url)
{
	map<string, string> attr;
	wstring temp = _u2w(url);
	string album = CDataSrv::Instance()._albumName;

	wstring dDir = (album == "private") ? L"/data2" : L"/data1";

	wstring szPath = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);
	wstring path = szPath + dDir + temp;

	CHtmlexif::Instance().get_exif_value(path, "", attr);

	string MetadataExif = "";

	if (!attr.empty())
	{
		string Exif = "";
		string Exif_Make = "";
		string Exif_Model = "";
		string Exif_DateTime = "";

		map<string, string>::iterator I2;
		for (I2 = attr.begin(); I2 != attr.end(); I2++)
		{
			if (I2->first == "Make")
			{
				Exif_Make = I2->second;
			}
			if (I2->first == "DateTimeOriginal")
			{
				Exif_DateTime = I2->second;
			}
		}

		Exif += Exif_DateTime;
		if (Exif != "")
		{
			Exif += ", ";
		}

		Exif += Exif_Make;
		if (Exif != "")
		{
			MetadataExif = Exif;
		}
	}

	return MetadataExif;
}

long CHtmlgetpicProc::proc_boxshow_htm(string osm, uint32_t from, uint32_t to, string& tpl)
{
	list<IMAGE_DTO> lstOrder;
	list<string>::iterator it;

	list<string> plstImageIds;
	if (osm != "")
	{
		plstImageIds = CDataSrv::Instance()._lstosm;
	}
	else
	{
		CDataSrv::Instance().get_lstImageIds(plstImageIds);
	}

	if (from == 0 && to == 0)
	{
		string ids = "";
		for (it = plstImageIds.begin(); it != plstImageIds.end(); it++)
		{
			ids += *it + ",";
		}
		rtrim(ids, ",");

		Cimagedal::Instance().get_images_all_ids(ids, &plstImageIds, lstOrder);
	}
	else
	{
		list<string> lstOrderIds;
		uint32_t i = 0;
		string ids = "";
		for (it = plstImageIds.begin(); it != plstImageIds.end(); it++)
		{
			i++;
			if (i > to)
			{
				break;
			}
			else if (i <= from)
			{
				continue;
			}
			ids += *it + ",";
			lstOrderIds.push_back(*it);
		}
		rtrim(ids, ",");

		Cimagedal::Instance().get_images_all_ids(ids, &lstOrderIds, lstOrder);
	}

	tpl = "<ul>";

	string url0 = "";
	string templ_mp4 = "";
	string templ = "";
	CLang::Instance().getOneData("viewerjs_image_li_mp4", templ_mp4);
	CLang::Instance().getOneData("viewerjs_image_li_img", templ);

	list<IMAGE_DTO>::iterator iter;
	for (iter = lstOrder.begin(); iter != lstOrder.end(); iter++)
	{
		string tmp = "";
		string url = CDataSrv::Instance().get_image_url(2, *iter);
		url0 = CDataSrv::Instance().get_image_url(100, *iter);

		if (iter->ext == "mp3" || iter->ext == "ogg")
		{
			tmp = templ_mp4;

			uint32_t maxWidth = 576;
			uint32_t maxHeight = 432;
			uint32_t outW = 0;
			uint32_t outH = 0;
			get_imgsize(*iter, maxWidth, maxHeight, outW, outH);

			ReplaceOne(tmp, "{$width}", uint2str(outW));
			ReplaceOne(tmp, "{$height}", uint2str(outH));
			ReplaceOne(tmp, "{$duration}", get_dur_string(iter->duration));
		}
		else if (iter->ext == "mp4" || iter->ext == "webm" || iter->ext == "gif")
		{
			tmp = templ_mp4;

			uint32_t outW = iter->width;
			uint32_t outH = iter->height;

			ReplaceOne(tmp, "{$width}", uint2str(outW));
			ReplaceOne(tmp, "{$height}", uint2str(outH));
			ReplaceOne(tmp, "{$duration}", get_dur_string(iter->duration));
		}
		else
		{
			tmp = templ;

			string duration = "";
			ReplaceOne(tmp, "{$duration}", duration);

			string angle = get_angle(iter->rotation);
			ReplaceOne(tmp, "{$rotate}", angle);
		}
		Replace(tmp, "{$id}", iter->id);
		ReplaceOne(tmp, "{$title}", "");
		ReplaceOne(tmp, "{$url}", url);

		ReplaceOne(tmp, "{$url0}", url0);
		tpl += tmp;
	}
	tpl += " </ul>";

	return S_OK;
}

long CHtmlgetpicProc::proc_slide_htm(list<string> plstImageIds, uint32_t from, uint32_t to, string& slider_nav, string& picture_content)
{
	list<IMAGE_DTO> lstOrder;
	list<string>::iterator it;

	if (from == 0 && to == 0)
	{
		string ids = "";
		for (it = plstImageIds.begin(); it != plstImageIds.end(); it++)
		{
			ids += *it + ",";
		}
		rtrim(ids, ",");

		Cimagedal::Instance().get_images_all_ids(ids, &plstImageIds, lstOrder);
	}
	else
	{
		list<string> lstOrderIds;
		uint32_t i = 0;
		string ids = "";
		for (it = plstImageIds.begin(); it != plstImageIds.end(); it++)
		{
			i++;
			if (i > to)
			{
				break;
			}
			else if (i <= from)
			{
				continue;
			}
			ids += *it + ",";
			lstOrderIds.push_back(*it);
		}
		rtrim(ids, ",");

		Cimagedal::Instance().get_images_all_ids(ids, &lstOrderIds, lstOrder);
	}

	slider_nav = "";
	picture_content = "";
	string templ = "";
	string slick_btn = "";
	CLang::Instance().getOneData("slide_image_li_img", templ);
	CLang::Instance().getOneData("slick_btn", slick_btn);

	uint32_t i = 0;
	list<IMAGE_DTO>::iterator iter;
	for (iter = lstOrder.begin(); iter != lstOrder.end(); iter++)
	{
		i++;
		string tmp = templ;
		string url = CDataSrv::Instance().get_image_url(0, *iter);

		string idx0 = uint2str(i + from);
		ReplaceOne(tmp, "{$idx}", idx0);
		ReplaceOne(tmp, "{$id}", iter->id);
		ReplaceOne(tmp, "{$title}", iter->name);
		ReplaceOne(tmp, "{$url}", url);

		if (iter->ext == "mp3" || iter->ext == "mp4" || iter->ext == "webm" || iter->ext == "ogg")
		{
			ReplaceOne(tmp, "imgbg", "");
		}

		picture_content += tmp;
		slider_nav += slick_btn;
	}

	return S_OK;
}

long CHtmlgetpicProc::get_details(string type, const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string name = "";
	string favorite = "";
	string caddie = "";
	string representative = "";

	string idx = "";
	string picid = "";
	string category = "";
	string exif = "";
	string osm = "";
	lookitem(request_dto.szArgs, "idx=", idx);
	lookitem(request_dto.szArgs, "category=", category);
	lookitem(request_dto.szArgs, "picid=", picid);
	lookitem(request_dto.szArgs, "exif=", exif);
	lookitem(request_dto.szArgs, "osm=", osm);

	int index = atoi(idx.c_str());

	list<string>::iterator it;
	list<string> plstImageIds;
	if (osm != "")
	{
		plstImageIds = CDataSrv::Instance()._lstosm;
	}
	else
	{
		CDataSrv::Instance().get_lstImageIds(plstImageIds);
	}

	if (picid != "")
	{
	}
	else if (picid == "")
	{
		int i = 0;
		for (it = plstImageIds.begin(); it != plstImageIds.end(); it++)
		{
			if (index == i)
			{
				picid = *it;
				break;
			}
			i++;
		}
	}

	IMAGE_DTO curr;

	list<IMAGE_DTO> lstImageDto;
	Cimagedal::Instance().get_images_by_ids(picid, lstImageDto);

	list<IMAGE_DTO>::iterator iter;
	for (iter = lstImageDto.begin(); iter != lstImageDto.end(); iter++)
	{
		if (picid == iter->id)
		{
			curr = *iter;
		}
	}

	if (curr.id == "")
	{
		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		writer.StartObject();
		writer.String("stat");
		writer.String("error");
		writer.EndObject();

		response_dto.szContentType = "text/plain; charset=utf-8";
		response_dto.szResponse = sb.GetString();

		return S_OK;
	}

	name = curr.name;

	char buf[MAX_PATH] = { 0 };
	sprintf_s(buf, "%d &times %d ", curr.width, curr.height);

	string pic_details = curr.file;

	{
		string temp = "";
		float n = static_cast<float>(curr.filesize) / 10.0f;
		temp = float2Str(n, "%.1f") + "MB";

		bool b = false;
		if (curr.ext == "mp3" || curr.ext == "ogg")
		{
			if (curr.representative_ext == "")
			{
				b = true;
			}
		}
		else if (curr.ext == "mp4" || curr.ext == "webm")
		{
			if (curr.representative_ext != "")
			{
				b = true;
			}
		}

		if (b)
		{
			pic_details += " (" + temp + ") ";
		}
		else
		{
			pic_details += " (" + string(buf) + ", " + temp + ") ";
		}

	}

	list<CATEGORY_DTO> lst;
	CHtmlgetMenubar::Instance().get_categories_order_by_root_dir(0, lst);

	list<CATEGORY_DTO>::iterator iter2;
	if (type == "all")
	{
		bool bfav = false;

		list<FAVORITE_DTO>::iterator I;
		for (I = CDataSrv::Instance()._lstFavorite.begin(); I != CDataSrv::Instance()._lstFavorite.end(); I++)
		{
			if (I->image_id == curr.id)
			{
				bfav = true;
				break;
			}
		}

		if (bfav)
		{
			CLang::Instance().getOneData("actionButtons_favorite_del", favorite);
		}
		else
		{
			CLang::Instance().getOneData("actionButtons_favorite_add", favorite);
		}

		CLang::Instance().getOneData("actionButtons_cmdCaddie", caddie);
		list<CADDIE_DTO> lstCaddie;
		Cchatdal::Instance().get_caddies(picid, lstCaddie);
		if (!lstCaddie.empty())
		{
			ReplaceOne(caddie, "pn-icon-caddie-add", "pn-icon-caddie-del");
		}

		bool b = false;
		for (iter2 = lst.begin(); iter2 != lst.end(); iter2++)
		{
			if (iter2->id == category && iter2->representative_id == picid)
			{
				b = true;
				break;
			}
		}

		if (b)
		{
			CLang::Instance().getOneData("actionButtons_representative_span", representative);
		}
		else
		{
			CLang::Instance().getOneData("actionButtons_representative", representative);
		}
	}

	string add_tags = "";
	string cat_name = "";
	{
		string tags = "";
		string cats = "";
		Cimagedal::Instance().get_image_properties(curr.id, cats, tags);

		list<TAG_DTO>* ptrlstTag = CDataSrv::Instance().get_lstTag();
		list<TAG_DTO>::iterator it2;
		for (it2 = ptrlstTag->begin(); it2 != ptrlstTag->end(); it2++)
		{
			string::size_type pos = tags.find("," + it2->id + ",");
			if (pos != string::npos)
			{
				add_tags += "'~~" + it2->id + "~~',";
			}
		}

		vector<string> v1 = tokenize(cats, ",", true, "");
		for (int i = 0; i < v1.size(); i++)
		{
			for (iter2 = lst.begin(); iter2 != lst.end(); iter2++)
			{
				string::size_type pos = cats.find("," + iter2->id + ",");
				if (v1[i] == iter2->id)
				{
					cat_name += iter2->name + ",";
					break;
				}
			}
		}

		if (cat_name != "")
		{
			rtrim(cat_name, ",");
		}
	}

	string MetadataExif = "";
	string exif_val = "";
	{
		if (exif == "details")
		{
			get_all_exif(curr, cat_name, MetadataExif, exif_val);
			if (MetadataExif == "")
			{
				pic_details += ", " + get_time_string(curr.date_creation, "%c");
			}
			else
			{
				pic_details += ", " + MetadataExif;
			}
		}
		else if (exif == "title")
		{
			MetadataExif = get_meta_exif(curr.ori);
			if (MetadataExif == "")
			{
				pic_details += ", " + get_time_string(curr.date_creation, "%c");
			}
			else
			{
				pic_details += ", " + MetadataExif;
			}
		}
		else
		{
			pic_details += ", " + get_time_string(curr.date_creation, "%c");

		}
	}

	translate(pic_details, "", "");
	translate(favorite, "", "");
	translate(caddie, "", "");
	translate(representative, "", "");
	translate(exif_val, "", "");

	if (add_tags != "")
	{
		rtrim(add_tags, ",");
		add_tags = "[" + add_tags + "]";
	}

	string angle = get_angle(curr.rotation);

	string changed = "0";

	StringBuffer sb;
	Writer<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("stat");
	writer.String("ok");

	writer.String("result");
	writer.StartObject();
	writer.String("id");
	writer.String(picid.c_str());
	writer.String("name");
	writer.String(name.c_str());
	writer.String("album");
	writer.String(cat_name.c_str());
	writer.String("favorite");
	writer.String(favorite.c_str());
	writer.String("caddie");
	writer.String(caddie.c_str());
	writer.String("width");
	writer.String(uint2str(curr.width).c_str());
	writer.String("height");
	writer.String(uint2str(curr.height).c_str());
	writer.String("ext");
	writer.String(curr.ext.c_str());
	writer.String("comment");
	writer.String(curr.comment.c_str());
	writer.String("rotate");
	writer.String(angle.c_str());
	writer.String("representative");
	writer.String(representative.c_str());
	writer.String("detail");
	writer.String(pic_details.c_str());
	writer.String("changed");
	writer.String(changed.c_str());
	writer.String("exif");
	writer.String(exif_val.c_str());
	writer.String("tags");
	writer.String(add_tags.c_str());
	writer.String("coi");
	writer.String(curr.coi.c_str());
	writer.EndObject();
	writer.EndObject();

	response_dto.szContentType = "text/plain; charset=utf-8";
	response_dto.szResponse = sb.GetString();

	if (type == "all")
	{
		writedb::Instance().update_images_hit(picid);
	}

	return 0;
}

long CHtmlgetpicProc::get_src(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string idx = "";
	lookitem(request_dto.szArgs, "idx=", idx);
	int index = atoi(idx.c_str());

	string picid = "0";
	int i = 0;
	list<string> plstImageIds;
	CDataSrv::Instance().get_lstImageIds(plstImageIds);

	list<string>::iterator it;
	for (it = plstImageIds.begin(); it != plstImageIds.end(); it++)
	{
		if (index == i)
		{
			picid = *it;
			break;
		}
		i++;
	}

	string changed = "0";
	string angle = "0";
	string url = "";
	string stat = "ok";

	if (picid == "0")
	{
		stat = "error";
	}
	else
	{
		IMAGE_DTO curr;
		list<IMAGE_DTO> lstImageDto;
		Cimagedal::Instance().get_images_by_ids(picid, lstImageDto);
		list<IMAGE_DTO>::iterator iter;
		for (iter = lstImageDto.begin(); iter != lstImageDto.end(); iter++)
		{
			if (picid == iter->id)
			{
				curr = *iter;
				break;
			}
		}

		angle = get_angle(curr.rotation);
		url = CDataSrv::Instance().get_image_url(2, curr);
		//url += "?mk=" + uint64str(curr.lastmodified);
	}

	StringBuffer sb;
	Writer<StringBuffer> writer(sb);
	writer.StartObject();
	writer.String("stat");
	writer.String(stat.c_str());

	writer.String("result");
	writer.StartObject();
	writer.String("id");
	writer.String(picid.c_str());
	writer.String("src");
	writer.String(url.c_str());
	writer.String("rotate");
	writer.String(angle.c_str());
	writer.String("changed");
	writer.String(changed.c_str());
	writer.EndObject();
	writer.EndObject();

	response_dto.szContentType = "text/plain; charset=utf-8";
	response_dto.szResponse = sb.GetString();

	return 0;
}

long CHtmlgetpicProc::get_speed(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string btn = "";
	string speed = "";
	lookitem(request_dto.szArgs, "btn=", btn);
	lookitem(request_dto.szArgs, "speed=", speed);

	int _speed = atoi(speed.c_str());

	string actionButtons_slideshow_start = "";
	string actionButtons_slideshow_inc = "";
	string actionButtons_slideshow_dec = "";

	CLang::Instance().getOneData("actionButtons_slideshow_dec", actionButtons_slideshow_dec);
	CLang::Instance().getOneData("actionButtons_slideshow_inc", actionButtons_slideshow_inc);

	if (btn == "stop")
	{
		CLang::Instance().getOneData("actionButtons_slideshow_start", actionButtons_slideshow_start);
		CLang::Instance().getOneData("actionButtons_slideshow_inc_span", actionButtons_slideshow_inc);
		CLang::Instance().getOneData("actionButtons_slideshow_dec_span", actionButtons_slideshow_dec);
	}
	else
	{
		CLang::Instance().getOneData("actionButtons_slideshow_stop", actionButtons_slideshow_start);

		if (_speed <= 1000)
		{
			CLang::Instance().getOneData("actionButtons_slideshow_inc_span", actionButtons_slideshow_inc);
		}
		else if (_speed >= 5000)
		{
			CLang::Instance().getOneData("actionButtons_slideshow_dec_span", actionButtons_slideshow_dec);
		}
	}

	translate(actionButtons_slideshow_start, "", "");
	translate(actionButtons_slideshow_dec, "", "");
	translate(actionButtons_slideshow_inc, "", "");

	StringBuffer sb;
	Writer<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("stat");
	writer.String("ok");

	writer.String("result");
	writer.StartObject();
	writer.String("cmdslideshow");
	writer.String(actionButtons_slideshow_start.c_str());
	writer.String("cmdinc");
	writer.String(actionButtons_slideshow_inc.c_str());
	writer.String("cmddec");
	writer.String(actionButtons_slideshow_dec.c_str());
	writer.String("speed");
	writer.String(Int2Str(_speed).c_str());

	writer.EndObject();
	writer.EndObject();

	response_dto.szContentType = "text/html";
	response_dto.szResponse = sb.GetString();
	return 0;
}

long CHtmlgetpicProc::get_all_exif(IMAGE_DTO curr, string cat_name, string& metadata, string& all)
{
	all = "";
	map<string, string> attr;

	float size = (curr.iscoi == 1) ? static_cast<float>(curr.coi_filesize) / 10.0f : static_cast<float>(curr.filesize) / 10.0f;
	attr.insert(map<string, string>::value_type("FileSize", float2Str(size, "%.1f") + " MB"));

	bool b = false;
	if (curr.ext == "mp3" || curr.ext == "ogg")
	{
		if (curr.representative_ext == "")
		{
			b = true;
		}
	}
	else if (curr.ext == "mp4" || curr.ext == "webm")
	{
		if (curr.representative_ext != "")
		{
			b = true;
		}
	}

	if (!b)
	{
		if (curr.iscoi == 0)
		{
			attr.insert(map<string, string>::value_type("Dimensions", uint2str(curr.width) + " &times " + uint2str(curr.height)));
		}
		else
		{
			attr.insert(map<string, string>::value_type("Dimensions", uint2str(curr.coi_w) + " &times " + uint2str(curr.coi_h)));
		}
	}

	attr.insert(map<string, string>::value_type("FileDate", get_time_string(curr.date_creation, "%c")));
	if (curr.iscoi == 0)
	{
		attr.insert(map<string, string>::value_type("Fname", curr.file));
	}
	else
	{
		attr.insert(map<string, string>::value_type("Fname", curr.file + "_coi"));
	}

	long q = (curr.iscoi == 1) ? curr.coi_quality : curr.quality;
	if (q > 0)
	{
		attr.insert(map<string, string>::value_type("quality", float2Str(static_cast<float>(q) / 100.0f, "%.1f")));
	}
	string album = CDataSrv::Instance()._albumName;

	wstring temp = _u2w(curr.ori);
	wstring path = L"";

	wstring::size_type pos = temp.find(L"/galleries_");
	if (pos == wstring::npos)
	{
		wstring dDir = (album == "private") ? L"/data2" : L"/data1";

		wstring szPath = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);
		path = szPath + dDir + temp;
	}
	else
	{
		string t = _w2u(temp);
		list<ROOT_DTO> lstRoot;
		CDataSrv::Instance().get_roots(lstRoot);

		list<ROOT_DTO>::iterator it2;
		for (it2 = lstRoot.begin(); it2 != lstRoot.end(); it2++)
		{
			string id8 = "/galleries_" + it2->id + "/";
			string::size_type pos = t.find(id8);
			if (pos != string::npos)
			{
				ReplaceOne(t, id8, "/");
				path = _u2w(it2->root_dir0 + t);
			}
		}
	}

	string fpath = _w2u(path);
	attr.insert(map<string, string>::value_type("FilePath", fpath));

	if (cat_name != "")
	{
		attr.insert(map<string, string>::value_type("Album", cat_name));
	}

	if (curr.iscoi == 0)
	{
		CHtmlexif::Instance().get_exif_value(path, "all", attr);
	}

	map<string, string>::iterator I2;

	string details_key = "";
	string details_val = "";
	CLang::Instance().getOneData("details_key", details_key);
	CLang::Instance().getOneData("details_val", details_val);

	vector<string> v0 = tokenize(details_key, ",", true, "");
	vector<string> v1 = tokenize(details_val, ",", true, "");

	string details_tr = "";
	CLang::Instance().getOneData("details_tr", details_tr);

	string details_desc = "";
	CLang::Instance().getOneData("details_desc", details_desc);

	string details_album = "";
	CLang::Instance().getOneData("details_album", details_album);

	string loc = "";
	for (I2 = attr.begin(); I2 != attr.end(); I2++)
	{
		if (I2->first == "FilePath")
		{
			loc = I2->second;
			break;
		}
	}
	if (album == "private")
	{
		loc += ".krf";
	}

	int len = (int)v0.size();
	int i = 0;
	for (i = 0; i < len; i++)
	{
		for (I2 = attr.begin(); I2 != attr.end(); I2++)
		{
			if (I2->first == v0[i])
			{
				string temp0 = details_tr;
				ReplaceOne(temp0, "{$name}", v1[i]);
				if (I2->first == "Fname")
				{
					string details_td_loc = "";
					CLang::Instance().getOneData("details_td_loc", details_td_loc);

					ReplaceOne(details_td_loc, "{$value}", I2->second);
					ReplaceOne(details_td_loc, "{$title}", loc);
					ReplaceOne(temp0, "{$value}", details_td_loc);
					all += temp0;
				}
				else if (I2->first == "Dimensions")
				{
					ReplaceOne(details_desc, "{$comment}", curr.comment);
					if (attr.size() > 8)
					{
						ReplaceOne(details_desc, "opacity100", "opacity:1");
					}
					else {
						ReplaceOne(details_desc, "opacity100", "opacity:.7");
					}

					ReplaceOne(temp0, "{$value}", I2->second);
					all += temp0;
					all += details_desc;
				}
				else if (I2->first == "Album")
				{
					string details_td_loc = details_album;
					ReplaceOne(details_td_loc, "{$value}", I2->second);
					ReplaceOne(details_td_loc, "{$cat_id}", curr.storage_category_id);
					ReplaceOne(temp0, "{$value}", details_td_loc);
					all += temp0;
				}
				else
				{
					if (I2->second == "QCAM-AA")
					{
						ReplaceOne(temp0, "{$value}", I2->second);

					}
					ReplaceOne(temp0, "{$value}", I2->second);
					all += temp0;
				}
				break;
			}
		}
	}
	all += "</table></td></tr> ";

	string Exif_Make = "";
	string Exif_DateTime = "";

	for (I2 = attr.begin(); I2 != attr.end(); I2++)
	{
		if (I2->first == "Make")
		{
			Exif_Make = I2->second;
		}
		if (I2->first == "DateTimeOriginal")
		{
			Exif_DateTime = I2->second;
		}
	}

	metadata += Exif_DateTime;
	if (metadata != "" && Exif_Make != "")
	{
		metadata += ", ";
		metadata += Exif_Make;
	}

	return 0;
}