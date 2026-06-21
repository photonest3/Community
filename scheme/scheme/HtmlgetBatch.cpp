#include "StdAfx.h"
#include "unitil.h"
#include <vector>
#include <string>

#include "../../templ/templ/resource.h"
#include "lang.h"
#include "imagedal.h"
#include "DataSrv.h"
#include "HtmlgetBatch.h"
#include "HtmlgetMenubar.h"
#include "HtmlgetAdminProc.h"
#include "HtmlSearchProc.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;


CHtmlgetBatch::CHtmlgetBatch(void)
{
	_szArgs = "";
}

CHtmlgetBatch::~CHtmlgetBatch(void)
{
}


long CHtmlgetBatch::proc_batch_htm(REQUEST_DTO request_dto, string batch_infos, string& tpl)
{
	string from = "";
	string mode = "";
	string nohead = "";
	lookitem(request_dto.szArgs, "from=", from);
	lookitem(request_dto.szArgs, "mode=", mode);
	lookitem(request_dto.szArgs, "nohead=", nohead);

	string start = "";
	string display = "";
	lookitem(request_dto.szArgs, "start=", start);
	lookitem(request_dto.szArgs, "display=", display);

	if (start == "")
	{
		start = "1";
	}

	string _batch_body = "";

	SEARCH_REQ dto;
	if (from != "")
	{
		_szArgs = request_dto.szArgs;
		_batch_body = "";
		_f_req = dto;
	}
	else
	{
		if (request_dto.szBody == "")
		{
			request_dto.szArgs = _szArgs;
		}
	}

	string filter = "";
	string tag = "";
	string album = "";
	lookitem(request_dto.szArgs, "filter=", filter);
	lookitem(request_dto.szArgs, "tag=", tag);
	lookitem(request_dto.szArgs, "album=", album);

	//page=admin.batch&filter=album-1
	if (mode == "")
	{
		mode = "global";
	}

	int curr = atoi(start.c_str());
	int per = atoi(display.c_str());
	if (mode == "global")
	{
		if (per == 0)
		{
			display = "20";
			per = 20;

			string val = "";
			CDataSrv::Instance().get_config_val("global_display", val);
			if (val != "")
			{
				display = val;
				per = atoi(display.c_str());
			}
		}
		else
		{
			CDataSrv::Instance().set_config_val("global_display", display);
		}
	}
	else
	{
		if (per == 0)
		{
			display = "5";
			per = 5;

			string val = "";
			CDataSrv::Instance().get_config_val("unit_display", val);
			if (val != "")
			{
				display = val;
				per = atoi(display.c_str());
			}
		}
		else
		{
			CDataSrv::Instance().set_config_val("unit_display", display);
		}
	}
	list<CATEGORY_DTO> lst;
	CHtmlgetMenubar::Instance().clear();
	CHtmlgetMenubar::Instance().get_categories_order_by_root_dir(1, lst);
	CHtmlgetMenubar::Instance().proc_categories(&lst);

	uint32_t nbCaddie = 0;
	uint32_t nbDuplicates = 0;
	uint32_t nbOrphan = 0;

	if (mode == "global")
	{
		if (request_dto.szBody != "")
		{
			_szArgs = request_dto.szArgs;
			_f_req = dto;
			_batch_body = request_dto.szBody;
		}

		if (filter == "caddie" || filter == "no_album" || filter == "last_import" || filter == "tag" || filter == "album")
		{
			_batch_body = "";
			_f_req = dto;
		}

		Htmlread(IDR_ADMIN_BATCH_HTML, L"html/_admin_batch.html", tpl);
		ReplaceOne(tpl, "{$ov}", CDataSrv::Instance()._ov);
		ReplaceOne(tpl, "{$album}", CDataSrv::Instance()._albumName);

		CHtmlgetAdminProc::Instance().proc_head_htm("admin.batch", nbCaddie, nbDuplicates, nbOrphan, tpl);

		string style = "";
		if (nohead != "")
		{
			style = "style=\"display:none;\"";
			ReplaceOne(tpl, "id=\"content\"", "");
		}
		Replace(tpl, "o='style'", style);

		string slider_dimension = "";
		string slider_file = "";

		if (_batch_body != "")
		{
			CHtmlSearchProc::Instance().get_search_req(request_dto, _f_req);
			_f_req.search = "0";
		}

		if (tag != "")
		{
			_f_req.tags = tag;
			_f_req.tag_mode = "OR";
			_f_req.filter_tags_use = "on";
		}
		else if (album != "")
		{
			_f_req.cat = album;
			_f_req.filter_category_use = "on";

		}

		proc_filter_req_htm(lst, filter, tpl);
	}

	list<IMAGE_DTO> lstImageDto;
	if (_f_req.subcats == "on")
	{
		string cats = "";
		list<CATEGORY_DTO>::iterator iter;

		for (iter = lst.begin(); iter != lst.end(); iter++)
		{
			string uppercats = "," + iter->full_id + ",";
			string::size_type pos = uppercats.find("," + _f_req.cat + ",");
			if (pos != string::npos)
			{
				cats += iter->id + ",";
			}

		}
		rtrim(cats, ",");
		_f_req.allcats = cats;
	}

	Cimagedal::Instance().get_filter_ids(_f_req);
	Cimagedal::Instance().get_images_by_ids("0", lstImageDto);

	int len = (int)lstImageDto.size();
	if (curr == len)
	{
		curr -= per;
		if (curr <= 0)
		{
			curr = 1;
		}
		start = Int2Str(curr);
	}

	string url0 = "./admin.html?page=admin.batch&mode=" + mode + "&display=" + display;
	string navigationBar = get_navigationBar(url0, len, curr, per, 1);

	uint32_t maxWidth = 144;
	uint32_t maxHeight = 144;

	list<IMAGE_DTO>::iterator iter;

	if (mode == "global")
	{
		string element_ids = "";
		for (iter = lstImageDto.begin(); iter != lstImageDto.end(); iter++)
		{
			element_ids += iter->id + ",";
		}
		rtrim(element_ids, ",");

		string beatch_selectedMessage = "";
		string nb_thumbs_page = Int2Str(per);
		string nb_thumbs_set = "0";
		string beatch_thumbnail = "";

		uint32_t nb = 0;;
		Cimagedal::Instance().get_image_tags_count("0", nb);

		string option_del_tags = "";
		if (nb > 0)
		{
			CLang::Instance().getOneData("option_del_tags", option_del_tags);
		}
		ReplaceOne(tpl, "{$option_del_tags}", option_del_tags);

		string selectSet = "";
		string navigationBar_global = "";

		string tt = get_navigationBar_global("global", per, navigationBar, navigationBar_global);
		if (len > per)
		{
			CLang::Instance().getOneData("selectSet", selectSet);
		}
		else
		{
			navigationBar_global = "<p>" + tt + " {'photos per page'|@translate}</p>";
		}

		_picture_data = "";
		_element_ids = "";

		list<ROOT_DTO> lstRoot;
		CDataSrv::Instance().get_roots(lstRoot);

		if (!lstImageDto.empty())
		{
			CLang::Instance().getOneData("picture_data1", _picture_data);
			nb_thumbs_set = uint2str((uint32_t)lstImageDto.size());
			CLang::Instance().getOneData("beatch_selectedMessage", beatch_selectedMessage);
			translate(beatch_selectedMessage, uint2str((uint32_t)lstImageDto.size()), "");

			string selection = "";
			if (request_dto.szBody != "")
			{
				string setSelected = "";
				string element_ids0 = "";
				lookitem_arr(request_dto.szBody, "selection=", selection);
				lookitem(request_dto.szBody, "setSelected=", setSelected);
				lookitem(request_dto.szBody, "element_ids=", element_ids0);
				URLDecode(element_ids0, 0);

				//all
				if (setSelected == "on")
				{
					selection = element_ids0;
				}
				selection = "," + selection + ",";
			}

			string style1 = "style=\"width:{$width}px; height:{$height}px;display:inline-block;\"";
			string style2 = "style=\"max-width:{$width}px; max-height:{$height}px;display:inline-block;\"";

			string beatch_thumbnail_name = "";
			CLang::Instance().getOneData("beatch_thumbnail_name", beatch_thumbnail_name);


			string beatch_mtitle = "";
			CLang::Instance().getOneData("beatch_mtitle", beatch_mtitle);

			string style = "";

			int i = 0;
			list<IMAGE_DTO>::iterator iter;
			for (iter = lstImageDto.begin(); iter != lstImageDto.end(); iter++)
			{
				if (i > curr * per)
				{
					break;
				}

				if (i >= (curr - 1) * per && i < curr * per)
				{
					string beatch_thumbnail_li = "";
					if (iter->ext == "mp3" || iter->ext == "mp4" || iter->ext == "gif" || iter->ext == "webm" || iter->ext == "ogg")
					{
						string vcontrol_js = "";
						CLang::Instance().getOneData("vcontrol_js", vcontrol_js);
						if (iter->ext == "gif")
						{
							CLang::Instance().getOneData("vcontrol_gif", vcontrol_js);
						}
						ReplaceOne(vcontrol_js, "o='url'", "");

						CLang::Instance().getOneData("beatch_video_li", beatch_thumbnail_li);
						ReplaceOne(beatch_thumbnail_li, "vcontrol_js", vcontrol_js);
						style = style2;
					}
					else
					{
						CLang::Instance().getOneData("beatch_thumbnail_li", beatch_thumbnail_li);
						style = style1;
					}


					string mtitle = beatch_mtitle;


					string t1 = beatch_thumbnail_name;
					ReplaceOne(t1, "{$name}", iter->name);
					string fol = ori2path(lstRoot, 1, iter->ori);
					ReplaceOne(t1, "{$fol}", fol);


					string temp = beatch_thumbnail_li;
					ReplaceOne(mtitle, "{$beatch_thumbnail_name}", t1);


					ReplaceOne(temp, "{$name}", iter->name);

					if (iter->ext == "mp3" || iter->ext == "mp4" || iter->ext == "webm" || iter->ext == "ogg")
					{
						ReplaceOne(temp, "{$duration}", get_dur_string(iter->duration));
					}

					Replace(temp, "{$id}", iter->id);
					string url = CDataSrv::Instance().get_image_url(1, *iter);
					string url0 = CDataSrv::Instance().get_image_url(0, *iter);

					uint32_t outW = 0;
					uint32_t outH = 0;
					get_imgsize(*iter, maxWidth, maxHeight, outW, outH);
					if (iter->ext == "mp3" || iter->ext == "ogg")
					{
						outW = 100;
						outH = 100;
					}

					ReplaceOne(temp, "{$oriurl}", url);
					Replace(temp, "{$path}", url0);

					float n = static_cast<float>(iter->filesize) / 10.0f;
					ReplaceOne(mtitle, "{$size}", float2Str(n, "%.1f") + "MB");

					string css = "";
					string chk = "";
					string::size_type pos = selection.find("," + iter->id + ",");
					if (pos != string::npos)
					{
						css = "class=\"thumbSelected\"";
						chk = "checked=\"checked\"";
					}
					ReplaceOne(temp, "g='thumbSelected'", css);
					ReplaceOne(temp, "g='checked'", chk);
					ReplaceOne(style, "{$width}", uint2str(outW));
					ReplaceOne(style, "{$height}", uint2str(outH));
					ReplaceOne(temp, "g=\"style\"", style);


					ReplaceOne(mtitle, "{$oriwidth}", uint2str(iter->width));
					ReplaceOne(mtitle, "{$oriheight}", uint2str(iter->height));

					translate(mtitle, "", "");
					Replace(mtitle, "\"", "&quot;");

					ReplaceOne(temp, "beatch_mtitle", mtitle);

					beatch_thumbnail += temp;
				}
				i++;
			}

			//ReplaceOne(_picture_data, "{$element_ids}", element_ids);
			_element_ids = element_ids;

			ReplaceOne(_picture_data, "{$beatch_selectedMessage}", beatch_selectedMessage);
			ReplaceOne(_picture_data, "{$selectSet}", selectSet);
			ReplaceOne(_picture_data, "{$navigationBar_global}", navigationBar_global);
			ReplaceOne(_picture_data, "{$beatch_thumbnail}", beatch_thumbnail);
		}
		else
		{
			CLang::Instance().getOneData("picture_data0", _picture_data);
		}

		ReplaceOne(tpl, "{$beatch_selectedMessage}", beatch_selectedMessage);
		ReplaceOne(tpl, "{$nb_thumbs_page}", nb_thumbs_page);
		ReplaceOne(tpl, "{$nb_thumbs_set}", nb_thumbs_set);

		string orphan_photos = "";
		CLang::Instance().getOneData("orphan_photos", orphan_photos);
		translate(orphan_photos, uint2str(nbOrphan), "");
		ReplaceOne(tpl, "{$orphan_photos}", orphan_photos);

		ReplaceOne(tpl, "{$nbCaddie}", uint2str(nbCaddie));
		ReplaceOne(tpl, "{$nbDuplicates}", uint2str(nbDuplicates));
		ReplaceOne(tpl, "{$nbOrphan}", uint2str(nbOrphan));

		ReplaceOne(tpl, "{$caddie_photos}", uint2str(nbCaddie));
		ReplaceOne(tpl, "{$duplicates_photos}", uint2str(nbDuplicates));

		if (filter == "caddie" && nbCaddie > 0)
		{
			ReplaceOne(tpl, "o='empty_caddie'", "");
		}
		else
		{
			ReplaceOne(tpl, "o='empty_caddie'", "style=\"display: none\"");
		}

		if (filter == "duplicates" && nbDuplicates > 0)
		{
			ReplaceOne(tpl, "o='duplicates'", "");
		}
		else
		{
			ReplaceOne(tpl, "o='duplicates'", "style=\"display: none\"");
		}

		if (filter == "no_album" && nbOrphan > 0)
		{
			ReplaceOne(tpl, "o='delete_orphans'", "");
		}
		else
		{
			ReplaceOne(tpl, "o='delete_orphans'", "style=\"display: none\"");
		}

	}
	else if (mode == "unit")
	{
		Htmlread(IDR_ADMIN_UNIT_HTML, L"html/_admin_unit.html", tpl);
		ReplaceOne(tpl, "{$ov}", CDataSrv::Instance()._ov);
		ReplaceOne(tpl, "{$album}", CDataSrv::Instance()._albumName);

		CHtmlgetAdminProc::Instance().proc_head_htm("admin.batch", nbCaddie, nbDuplicates, nbOrphan, tpl);


		list<ROOT_DTO> lstRoot;
		CDataSrv::Instance().get_roots(lstRoot);


		string nohead = "";
		lookitem(request_dto.szArgs, "nohead=", nohead);
		string style = "";
		if (nohead != "")
		{
			style = "style=\"display:none;\"";
			ReplaceOne(tpl, "id=\"content\"", "");
		}
		Replace(tpl, "o='style'", style);

		int week_starts_on = 6;
		wchar_t buf[MAX_PATH] = { 0 };
		int ret = GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IFIRSTDAYOFWEEK, buf, MAX_PATH);
		if (ret > 0)
		{
			week_starts_on = _wtoi(buf);
		}
		if (week_starts_on == 6)
		{
			week_starts_on = 0;
		}
		else
		{
			week_starts_on = 1;
		}

		ReplaceOne(tpl, "$firstDay", Int2Str(week_starts_on));

		string sLongDate = get_sLongDate();
		ReplaceOne(tpl, "$sLongDate", sLongDate);

		string js = "jquery.ui.datepicker-en-GB.js";
		string lang = CLang::Instance()._language;
		if (lang.find("zh") != string::npos)
		{
			js = "jquery.ui.datepicker-zh-CN.js";
		}
		else if (lang.find("ja") != string::npos)
		{
			js = "jquery.ui.datepicker-ja.js";
		}
		else if (lang.find("es") != string::npos)
		{
			js = "jquery.ui.datepicker-es.js";
		}
		else if (lang.find("fr") != string::npos)
		{
			js = "jquery.ui.datepicker-fr.js";
		}
		else if (lang.find("ru") != string::npos)
		{
			js = "jquery.ui.datepicker-ru.js";
		}
		else if (lang.find("de") != string::npos)
		{
			js = "jquery.ui.datepicker-de.js";
		}
		ReplaceOne(tpl, "jquery.ui.datepicker-en-GB.js", js);
		ReplaceOne(tpl, "{$start}", start);
		ReplaceOne(tpl, "{$display}", display);


		string picids = "";
		string formData = "";// navigationBar;
		{
			string allElementEdits = "<div id=\"galley\">";
			string elementEdit = "";
			CLang::Instance().getOneData("elementEdit", elementEdit);
			int i = 0;
			for (iter = lstImageDto.begin(); iter != lstImageDto.end(); iter++)
			{
				if (i > curr * per)
				{
					break;
				}

				if (i >= (curr - 1) * per && i < curr * per)
				{
					picids += iter->id + ",";
				}
				i++;
			}

			rtrim(picids, ",");
			list<IMAGE_TAG_DTO> lstImageTag;
			Cimagedal::Instance().get_image_tags(picids, lstImageTag);

			i = 0;
			for (iter = lstImageDto.begin(); iter != lstImageDto.end(); iter++)
			{
				if (i > curr * per)
				{
					break;
				}

				if (i >= (curr - 1) * per && i < curr * per)
				{
					string temp = elementEdit;
					string btn_unit_edit = "";

					string thumb = "";
					if (iter->ext == "mp3" || iter->ext == "mp4" || iter->ext == "gif" || iter->ext == "webm" || iter->ext == "ogg")
					{
						string vcontrol_js = "";
						CLang::Instance().getOneData("vcontrol_js", vcontrol_js);
						if (iter->ext == "gif")
						{
							CLang::Instance().getOneData("vcontrol_gif", vcontrol_js);
						}

						CLang::Instance().getOneData("unit_albumThumbnail_mp4", thumb);
						ReplaceOne(thumb, "vcontrol_js", vcontrol_js);
						ReplaceOne(thumb, "{$duration}", get_dur_string(iter->duration));
					}
					else
					{
						CLang::Instance().getOneData("btn_unit_edit", btn_unit_edit);
						CLang::Instance().getOneData("unit_albumThumbnail", thumb);
					}
					ReplaceOne(thumb, "{$id}", iter->id);

					string fol = ori2path(lstRoot, 1, iter->ori);
					ReplaceOne(thumb, "{$fol}", fol);

					string url = CDataSrv::Instance().get_image_url(1, *iter);
					ReplaceOne(thumb, "{$oriurl}", url);

					string url0 = CDataSrv::Instance().get_image_url(0, *iter);
					ReplaceOne(thumb, "{$photourl}", url0);
					ReplaceOne(thumb, "{$imagename}", iter->name);

					ReplaceOne(temp, "{$btn_unit_edit}", btn_unit_edit);
					ReplaceOne(temp, "{$thumb}", thumb);
					Replace(temp, "{$id}", iter->id);
					Replace(temp, "{$name}", iter->name);
					Replace(temp, "{$author}", iter->author);
					Replace(temp, "{$comment}", iter->comment);

					string tags = get_tags_string(iter->id, lstImageTag);
					Replace(temp, "{$tag_value}", tags);
					Replace(temp, "{$createval}", uint64str(iter->date_creation));

					string tm = get_time_string(iter->date_creation, "0");

					Replace(temp, "{$createtime}", tm);

					uint32_t outW = 0;
					uint32_t outH = 0;
					get_imgsize(*iter, maxWidth, maxHeight, outW, outH);

					ReplaceOne(temp, "{$width}", uint2str(outW));
					ReplaceOne(temp, "{$height}", uint2str(outH));
					allElementEdits += temp;
				}
				i++;
			}
			allElementEdits += "</div>";

			formData += allElementEdits;

			string unit_submit = "";
			if (!lstImageDto.empty())
			{
				CLang::Instance().getOneData("unit_submit", unit_submit);
			}
			formData += unit_submit;

			string style = "style=\"display:none;\"";

			string navigationBar_global = "";
			string tt = get_navigationBar_global("unit", per, navigationBar, navigationBar_global);
			ReplaceOne(tpl, "{$display20}", tt);
			if (len <= per)
			{
				navigationBar_global = "";
				if (len < 5)
				{
					ReplaceOne(tpl, "{$display20}", "");
					ReplaceOne(tpl, "o='style20'", style);
				}
				else
				{
					ReplaceOne(tpl, "{$display20}", tt);
					ReplaceOne(tpl, "o='style20'", "");
				}
			}
			else
			{
				ReplaceOne(tpl, "{$display20}", "");
				ReplaceOne(tpl, "o='style20'", style);
			}
			formData += navigationBar_global;
		}

		ReplaceOne(tpl, "{$element_ids}", picids);

		ReplaceOne(tpl, "{$formData}", formData);
	}

	return S_OK;
}


string  CHtmlgetBatch::get_tags_string(string image_id, list<IMAGE_TAG_DTO> lstImageTag)
{
	//string tag_val = "{&quot;name&quot;:&quot;{$name}&quot;,&quot;id&quot;:&quot;~~{$id}~~&quot;}";
	string tag_val = "{&quot;id&quot;:&quot;~~{$id}~~&quot;}";
	string all_tags = "";

	list<IMAGE_TAG_DTO>::iterator iter;
	for (iter = lstImageTag.begin(); iter != lstImageTag.end(); iter++)
	{
		if (iter->image_id == image_id)
		{
			string temp = tag_val;
			ReplaceOne(temp, "{$name}", iter->tag_name);
			ReplaceOne(temp, "{$id}", iter->tag_id);
			all_tags += temp + ",";
		}
	}
	rtrim(all_tags, ",");
	return all_tags;
}

long CHtmlgetBatch::proc_filter_req_htm(list<CATEGORY_DTO> lst, string filter, string& tpl)
{
	if (filter == "")
	{
		if (_f_req.filter_prefilter_use == "on")
		{
			filter = _f_req.filter;
		}
		else
		{
			if (_f_req.filter_search_use == "on" || _f_req.filter_category_use == "on" || _f_req.filter_tags_use == "on" || _f_req.filter_dimension_use == "on" ||
				_f_req.filter_date_use == "on" || _f_req.filter_filetypes_use == "on" || _f_req.filter_osm_use == "on" || _f_req.filter_authors_use == "on")
			{
			}
			else
			{
				filter = "last_import";
			}
		}

	}

	_f_req.filter_prefilter_use = "on";
	_f_req.filter = filter;

	string prefilter = "";
	CLang::Instance().getOneData("prefilter", prefilter);


	if (filter == "all_photos")
	{
		ReplaceOne(prefilter, "g='all_photos'", "selected=\"selected\"");
	}
	else if (filter == "videojs")
	{
		ReplaceOne(prefilter, "g='videojs'", "selected=\"selected\"");
	}
	else if (filter == "osm0")
	{
		ReplaceOne(prefilter, "g='osm0'", "selected=\"selected\"");
	}
	else if (filter == "osm1")
	{
		ReplaceOne(prefilter, "g='osm1'", "selected=\"selected\"");
	}
	else if (filter == "favorites")
	{
		ReplaceOne(prefilter, "g='favorites'", "selected=\"selected\"");
	}
	else if (filter == "no_tag")
	{
		ReplaceOne(prefilter, "g='no_tag'", "selected=\"selected\"");
	}
	else if (filter == "caddie")
	{
		ReplaceOne(prefilter, "g='caddie'", "selected=\"selected\"");
	}
	else if (filter == "duplicates")
	{
		ReplaceOne(prefilter, "g='duplicates'", "selected=\"selected\"");
	}
	else if (filter == "no_album")
	{
		ReplaceOne(prefilter, "g='no_album'", "selected=\"selected\"");
	}
	else
	{
		ReplaceOne(prefilter, "g='last_import'", "selected=\"selected\"");
	}

	ReplaceOne(prefilter, "g='duplicates'", "");
	ReplaceOne(prefilter, "g='all_photos'", "");
	ReplaceOne(prefilter, "g='videojs'", "");
	ReplaceOne(prefilter, "g='osm0'", "");
	ReplaceOne(prefilter, "g='osm1'", "");
	ReplaceOne(prefilter, "g='favorites'", "");
	ReplaceOne(prefilter, "g='last_import'", "");
	ReplaceOne(prefilter, "g='caddie'", "");
	ReplaceOne(prefilter, "g='no_tag'", "");
	ReplaceOne(prefilter, "g='no_album'", "");

	ReplaceOne(tpl, "{$prefilter}", prefilter);

	if (_f_req.filter_category_use == "on")
	{
		//&quot;
		ReplaceOne(tpl, "{$category_value}", "&quot;" + _f_req.cat + "&quot;");
	}
	else
	{
		ReplaceOne(tpl, "{$category_value}", "");
	}

	if (_f_req.filter_tags_use == "on")
	{
		//string tag_val = "{&quot;name&quot;:&quot;{$name}&quot;,&quot;id&quot;:&quot;~~{$id}~~&quot;}";
		string tag_val = "{&quot;id&quot;:&quot;~~{$id}~~&quot;}";
		string all_tags = "";

		list<TAG_DTO> lstTag;
		Cimagedal::Instance().get_tags("", lstTag);

		list<TAG_DTO>::iterator iter;

		if (_f_req.tags != "")
		{
			vector<string> v1 = tokenize(_f_req.tags, ",", true, "");
			int len = (int)v1.size();
			for (int i = 0; i < len; i++)
			{
				string t0 = trim(v1[i], "~~");
				for (iter = lstTag.begin(); iter != lstTag.end(); iter++)
				{
					if (iter->id == t0)
					{
						string temp = tag_val;
						ReplaceOne(temp, "{$name}", iter->name);
						ReplaceOne(temp, "{$id}", iter->id);
						all_tags += temp + ",";
						break;
					}
				}
			}
		}

		all_tags = rtrim(all_tags, ",");
		//{&quot;name&quot;:&quot;777&quot;,&quot;id&quot;:&quot;~~4~~&quot;},{&quot;name&quot;:&quot;888&quot;,&quot;id&quot;:&quot;~~6~~&quot;}
		ReplaceOne(tpl, "{$tag_value}", all_tags);
	}
	else
	{
		ReplaceOne(tpl, "{$tag_value}", "");
	}

	string temp_q = _f_req.search_allwords;
	ReplaceOne(tpl, "{$search_q}", temp_q);

	return 0;
}

string CHtmlgetBatch::get_navigationBar_global(string ty, int per, string navigationBar, string& navigationBar_global)
{
	navigationBar_global = "";

	CLang::Instance().getOneData("navigationBar_global", navigationBar_global);

	string temp = navigationBar;
	Replace(temp, "{$id}", "1");
	ReplaceOne(navigationBar_global, "{$navigationBar}", temp);

	string dis = "";
	string display20 = "";
	string display20_span = "";
	CLang::Instance().getOneData("display20", display20);
	CLang::Instance().getOneData("display20_span", display20_span);

	if (ty == "unit")
	{
		ReplaceOne(display20, "global", "unit");

		if (per == 5)
		{
			temp = display20_span;
			Replace(temp, "20", "5");
			dis = temp + " | ";

			temp = display20;
			Replace(temp, "20", "10");
			dis += temp + " | ";

			temp = display20;
			Replace(temp, "20", "50");
			dis += temp;
		}
		else if (per == 10)
		{
			temp = display20;
			Replace(temp, "20", "5");
			dis = temp + " | ";

			temp = display20_span;
			Replace(temp, "20", "10");
			dis += temp + " | ";

			temp = display20;
			Replace(temp, "20", "50");
			dis += temp;
		}
		else
		{
			temp = display20;
			Replace(temp, "20", "5");
			dis = temp + " | ";

			temp = display20;
			Replace(temp, "20", "10");
			dis += temp + " | ";

			temp = display20_span;
			Replace(temp, "20", "50");
			dis += temp;
		}
	}
	else
	{
		if (per == 20)
		{
			temp = display20_span;
			Replace(temp, "20", "20");
			dis = temp + " | ";

			temp = display20;
			Replace(temp, "20", "50");
			dis += temp + " | ";

			temp = display20;
			Replace(temp, "20", "100");
			dis += temp;
		}
		else if (per == 50)
		{
			temp = display20;
			Replace(temp, "20", "20");
			dis = temp + " | ";

			temp = display20_span;
			Replace(temp, "20", "50");
			dis += temp + " | ";

			temp = display20;
			Replace(temp, "20", "100");
			dis += temp;
		}
		else
		{
			temp = display20;
			Replace(temp, "20", "20");
			dis = temp + " | ";

			temp = display20;
			Replace(temp, "20", "50");
			dis += temp + " | ";

			temp = display20_span;
			Replace(temp, "20", "100");
			dis += temp;
		}
	}

	ReplaceOne(navigationBar_global, "{$display20}", dis);

	return dis;
}