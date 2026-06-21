#include "StdAfx.h"
#include "unitil.h"
#include <vector>
#include <string>

#include "../../templ/templ/resource.h"
#include "DataSrv.h"
#include "lang.h"
#include "imagedal.h"

#include "HtmlSearchProc.h"
#include "HtmlgetMenubar.h"


CHtmlSearchProc::CHtmlSearchProc(void)
{
}

CHtmlSearchProc::~CHtmlSearchProc(void)
{
}

long CHtmlSearchProc::proc_search_htm(list<CATEGORY_DTO> lst, const QUERY_REQ req, string num_tags, string& menubar, string& tpl)
{
	SEARCH_REQ search_req = CDataSrv::Instance()._search_req;

	string sel_cat = "";
	lookitem(req.args, "category=", sel_cat);
	if (sel_cat != "")
	{
		search_req.cat = sel_cat;
		if (search_req.subcats == "on")
		{
			string cats = "";
			list<CATEGORY_DTO>::iterator iter;

			for (iter = lst.begin(); iter != lst.end(); iter++)
			{
				string uppercats = "," + iter->full_id + ",";
				string::size_type pos = uppercats.find("," + search_req.cat + ",");
				if (pos != string::npos)
				{
					cats += iter->id + ",";
				}

			}
			rtrim(cats, ",");
			search_req.allcats = cats;
		}

	}
	if (sel_cat == "0")
	{
		search_req.cat = "";
		search_req.allcats = "";
	}


	if (search_req.search_allwords == "")
	{
		string search_allwords = "";
		lookitem(req.args, "search_allwords=", search_allwords);

		URLDecode(search_allwords, 0);
		esc_string(search_allwords);

		search_req.search_allwords = search_allwords;
	}

	Htmlread(IDR_SEARCH_HTML, L"html/_search.html", tpl);
	ReplaceOne(tpl, "strawberry_jam", CDataSrv::Instance()._skin);

	if (CDataSrv::Instance()._skin != "dark" && CDataSrv::Instance()._skin != "quartz")
	{
		ReplaceOne(tpl, "selectize.dark.css", "selectize.clear.css");
	}
	ReplaceOne(tpl, "{$ov}", CDataSrv::Instance()._ov);

	string filter = "";
	CDataSrv::Instance().get_config_val("filter", filter);
	ReplaceOne(tpl, "{$filters}", filter);


	ReplaceOne(tpl, "searchBox", "searchBox1");

	string search_form = "";
	get_search_form(lst, search_form, req);
	ReplaceOne(search_form, "searchform", "searchform1");

	ReplaceOne(tpl, "{$search_form}", search_form);

	ReplaceOne(tpl, "catSearch", "catSearch1");

	//4
	menubar = "";


	return S_OK;
}


long CHtmlSearchProc::get_search_req(REQUEST_DTO request_dto, SEARCH_REQ& search_req)
{
	if (!request_dto.szBody.empty())
	{
		string page = "";
		lookitem(request_dto.szArgs, "page=", page);

		search_req.page = page;
		lookitem(request_dto.szBody, "search_allwords=", search_req.search_allwords);
		lookitem(request_dto.szBody, "mode=", search_req.mode);
		lookitem(request_dto.szBody, "search_in_name=", search_req.search_in_name);
		lookitem(request_dto.szBody, "search_in_comment=", search_req.search_in_comment);
		lookitem(request_dto.szBody, "search_in_file=", search_req.search_in_file);
		lookitem(request_dto.szBody, "search_in_tags=", search_req.search_in_tags);

		lookitem(request_dto.szBody, "search_images=", search_req.search_images);
		lookitem(request_dto.szBody, "search_videos=", search_req.search_videos);
		//lookitem(request_dto.szBody, "adv_available=", search_req.adv_available);


		lookitem_arr(request_dto.szBody, "authors=", search_req.authors);
		lookitem_arr(request_dto.szBody, "tags=", search_req.tags);
		lookitem(request_dto.szBody, "tag_mode=", search_req.tag_mode);
		lookitem(request_dto.szBody, "date_type=", search_req.date_type);

		lookitem(request_dto.szBody, "createdate_min=", search_req.createdate_min);
		lookitem(request_dto.szBody, "createdate_max=", search_req.createdate_max);
		lookitem(request_dto.szBody, "postdate_min=", search_req.postdate_min);
		lookitem(request_dto.szBody, "postdate_max=", search_req.postdate_max);


		lookitem_arr(request_dto.szBody, "cat=", search_req.cat);
		lookitem(request_dto.szBody, "subcats=", search_req.subcats);
		search_req.allcats = "";

		lookitem(request_dto.szBody, "dimension_min_width=", search_req.dimension_min_width);
		lookitem(request_dto.szBody, "dimension_max_width=", search_req.dimension_max_width);
		lookitem(request_dto.szBody, "dimension_min_height=", search_req.dimension_min_height);
		lookitem(request_dto.szBody, "dimension_max_height=", search_req.dimension_max_height);
		lookitem(request_dto.szBody, "dimension_min_ratio=", search_req.dimension_min_ratio);
		lookitem(request_dto.szBody, "dimension_max_ratio=", search_req.dimension_max_ratio);

		lookitem(request_dto.szBody, "createdate_min_o=", search_req.createdate_min_o);
		lookitem(request_dto.szBody, "createdate_max_o=", search_req.createdate_max_o);
		lookitem(request_dto.szBody, "postdate_min_o=", search_req.postdate_min_o);
		lookitem(request_dto.szBody, "postdate_max_o=", search_req.postdate_max_o);

		lookitem(request_dto.szBody, "width_min_o=", search_req.width_min_o);
		lookitem(request_dto.szBody, "width_max_o=", search_req.width_max_o);
		lookitem(request_dto.szBody, "height_min_o=", search_req.height_min_o);
		lookitem(request_dto.szBody, "height_max_o=", search_req.height_max_o);
		lookitem(request_dto.szBody, "ratio_min_o=", search_req.ratio_min_o);
		lookitem(request_dto.szBody, "ratio_max_o=", search_req.ratio_max_o);

		lookitem(request_dto.szBody, "filter_search_use=", search_req.filter_search_use);
		lookitem(request_dto.szBody, "filter_category_use=", search_req.filter_category_use);
		lookitem(request_dto.szBody, "filter_filetypes_use=", search_req.filter_filetypes_use);
		lookitem(request_dto.szBody, "filter_date_use=", search_req.filter_date_use);
		lookitem(request_dto.szBody, "filter_dimension_use=", search_req.filter_dimension_use);
		lookitem(request_dto.szBody, "filter_authors_use=", search_req.filter_authors_use);
		lookitem(request_dto.szBody, "filter_tags_use=", search_req.filter_tags_use);

		lookitem(request_dto.szBody, "dimension_type=", search_req.dimension_type);
		lookitem(request_dto.szBody, "filter_osm_use=", search_req.filter_osm_use);
		lookitem(request_dto.szBody, "osm0=", search_req.osm0);
		lookitem(request_dto.szBody, "osm1=", search_req.osm1);


		lookitem(request_dto.szBody, "filesize_min=", search_req.filesize_min);
		lookitem(request_dto.szBody, "filesize_max=", search_req.filesize_max);
		lookitem(request_dto.szBody, "filesize_min_o=", search_req.filesize_min_o);
		lookitem(request_dto.szBody, "filesize_max_o=", search_req.filesize_max_o);


		URLDecode(search_req.search_allwords, 0);
		URLDecode(search_req.authors, 1);
		URLDecode(search_req.tags, 0);

		esc_string(search_req.search_allwords);

		lookitem(request_dto.szBody, "filter_prefilter_use=", search_req.filter_prefilter_use);
		lookitem(request_dto.szBody, "filter_prefilter=", search_req.filter);



		//bool b = false;
		//if (search_req.search_allwords != "")
		//{
		//	b = true;
		//}

		//if (search_req.authors != "")
		//{
		//	b = true;
		//}

		//if (search_req.tags != "")
		//{
		//	b = true;
		//}
		//if (search_req.cat != "")
		//{
		//	b = true;
		//}

		//search_req.changed = "1";
	}

	return 0;
}

long CHtmlSearchProc::proc_slider_req_htm(SEARCH_REQ req, string& szResponse)
{
	string slider_widths = "";
	string slider_heights = "";
	string slider_ratios = "";
	string slider_filesize = "";
	string slider_createdate = "";
	string slider_postdate = "";

	string width_min = "";
	string width_max = "";
	string height_min = "";
	string height_max = "";
	string filesize_min = "";
	string filesize_max = "";
	string createdate_min = "";
	string createdate_max = "";
	string ratio_min = "";
	string ratio_max = "";
	string postdate_min = "";
	string postdate_max = "";

	SLIDER_VALUES sliderValues;
	CDataSrv::Instance().get_sliderValues(sliderValues);

	if (!sliderValues.widths.empty())
	{
		width_min = uint2str(sliderValues.widths.front());
		width_max = uint2str(sliderValues.widths.back());
		CLang::Instance().getOneData("slider_widths", slider_widths);
	}

	if (!sliderValues.heights.empty())
	{
		height_min = uint2str(sliderValues.heights.front());
		height_max = uint2str(sliderValues.heights.back());
		CLang::Instance().getOneData("slider_heights", slider_heights);
	}

	if (!sliderValues.filesizes.empty())
	{
		filesize_min = float2Str(sliderValues.filesizes.front(), "%.1f");
		filesize_max = float2Str(sliderValues.filesizes.back(), "%.1f");
		CLang::Instance().getOneData("slider_filesize", slider_filesize);
	}

	if (!sliderValues.createdates.empty())
	{
		createdate_min = get_today(sliderValues.createdates.front());
		createdate_max = get_today(sliderValues.createdates.back());
		CLang::Instance().getOneData("slider_createdate", slider_createdate);
	}

	if (!sliderValues.postdates.empty())
	{
		postdate_min = get_today(sliderValues.postdates.front());
		postdate_max = get_today(sliderValues.postdates.back());
		CLang::Instance().getOneData("slider_postdate", slider_postdate);
	}

	if (!sliderValues.ratios.empty())
	{
		ratio_min = float2Str(sliderValues.ratios.front(), "%.2f");
		ratio_max = float2Str(sliderValues.ratios.back(), "%.2f");
		CLang::Instance().getOneData("slider_ratios", slider_ratios);
	}
	string ratio_sq0 = "";
	string ratio_sq1 = "";

	string ratio_min1 = "";
	string ratio_max0 = "";
	{
		list<float>::iterator I;
		for (I = sliderValues.ratios.begin(); I != sliderValues.ratios.end(); I++)
		{
			if (*I < 0.95)
			{
				ratio_min1 = float2Str(*I, "%.2f");
			}
			if (*I > 1.05)
			{
				if (ratio_max0 == "")
				{
					ratio_max0 = float2Str(*I, "%.2f");
				}
			}
			if (*I >= 0.95 && *I <= 1.0)
			{
				ratio_sq0 = float2Str(*I, "%.2f");
			}
			if (*I >= 1.0 && *I <= 1.05)
			{
				ratio_sq1 = float2Str(*I, "%.2f");
			}
		}
	}

	Replace(slider_widths, "{$width_min_o}", width_min);
	Replace(slider_widths, "{$width_max_o}", width_max);
	Replace(slider_heights, "{$height_min_o}", height_min);
	Replace(slider_heights, "{$height_max_o}", height_max);


	Replace(slider_filesize, "{$filesize_min_o}", filesize_min);
	Replace(slider_filesize, "{$filesize_max_o}", filesize_max);


	string slider_portrait = "";
	if (ratio_min1 != "" && ratio_max0 != "")
	{
		CLang::Instance().getOneData("slider_portrait", slider_portrait);

		ReplaceOne(slider_portrait, "{$ratio_min_o}", ratio_min);
		ReplaceOne(slider_portrait, "{$ratio_min1}", ratio_min1);
	}
	ReplaceOne(slider_ratios, "slider_portrait", slider_portrait);

	Replace(slider_ratios, "{$ratio_min_o}", ratio_min);
	Replace(slider_ratios, "{$ratio_max_o}", ratio_max);

	string slider_square = "";
	if (ratio_sq0 != "" && ratio_sq1 != "")
	{
		CLang::Instance().getOneData("slider_square", slider_square);

		ReplaceOne(slider_square, "{$ratio_sq0}", ratio_sq0);
		ReplaceOne(slider_square, "{$ratio_sq1}", ratio_sq1);
	}
	ReplaceOne(slider_ratios, "slider_square", slider_square);


	string slider_landscape = "";
	if (ratio_min1 != "" && ratio_max0 != "")
	{
		CLang::Instance().getOneData("slider_landscape", slider_landscape);

		ReplaceOne(slider_landscape, "{$ratio_max0}", ratio_max0);
		Replace(slider_landscape, "{$ratio_max_o}", ratio_max);
	}
	ReplaceOne(slider_ratios, "slider_landscape", slider_landscape);

	Replace(slider_createdate, "{$createdate_min_o}", createdate_min);
	Replace(slider_createdate, "{$createdate_max_o}", createdate_max);

	Replace(slider_postdate, "{$postdate_min_o}", postdate_min);
	Replace(slider_postdate, "{$postdate_max_o}", postdate_max);


	if (req.dimension_min_width != "")
	{
		width_min = req.dimension_min_width;
	}
	if (req.dimension_max_width != "")
	{
		width_max = req.dimension_max_width;
	}
	if (req.dimension_min_height != "")
	{
		height_min = req.dimension_min_height;
	}
	if (req.dimension_max_height != "")
	{
		height_max = req.dimension_max_height;
	}

	if (req.createdate_min != "")
	{
		createdate_min = req.createdate_min;
	}
	if (req.createdate_max != "")
	{
		createdate_max = req.createdate_max;
	}
	if (req.dimension_min_ratio != "")
	{
		ratio_min = req.dimension_min_ratio;
	}
	if (req.dimension_max_ratio != "")
	{
		ratio_max = req.dimension_max_ratio;
	}
	if (req.postdate_min != "")
	{
		postdate_min = req.postdate_min;
	}
	if (req.postdate_max != "")
	{
		postdate_max = req.postdate_max;
	}
	if (req.filesize_min != "")
	{
		filesize_min = req.filesize_min;
	}
	if (req.filesize_max != "")
	{
		filesize_max = req.filesize_max;
	}


	ReplaceOne(slider_widths, "{$width_min}", width_min);
	ReplaceOne(slider_widths, "{$width_max}", width_max);

	ReplaceOne(slider_heights, "{$height_min}", height_min);
	ReplaceOne(slider_heights, "{$height_max}", height_max);

	ReplaceOne(slider_ratios, "{$ratio_min}", ratio_min);
	ReplaceOne(slider_ratios, "{$ratio_max}", ratio_max);

	ReplaceOne(slider_filesize, "{$filesize_min}", filesize_min);
	ReplaceOne(slider_filesize, "{$filesize_max}", filesize_max);

	ReplaceOne(slider_createdate, "{$createdate_min}", createdate_min);
	ReplaceOne(slider_createdate, "{$createdate_max}", createdate_max);

	ReplaceOne(slider_postdate, "{$postdate_min}", postdate_min);
	ReplaceOne(slider_postdate, "{$postdate_max}", postdate_max);

	translate(slider_widths, "", "");
	ReplaceOne(slider_widths, "%d", width_min);
	ReplaceOne(slider_widths, "%d", width_max);

	translate(slider_heights, "", "");
	ReplaceOne(slider_heights, "%d", height_min);
	ReplaceOne(slider_heights, "%d", height_max);

	translate(slider_ratios, "", "");
	ReplaceOne(slider_ratios, "%s", ratio_min);
	ReplaceOne(slider_ratios, "%s", ratio_max);

	translate(slider_filesize, "", "");
	ReplaceOne(slider_filesize, "%s", filesize_min);
	ReplaceOne(slider_filesize, "%s", filesize_max);

	translate(slider_createdate, "", "");
	ReplaceOne(slider_createdate, "%s", createdate_min);
	ReplaceOne(slider_createdate, "%s", createdate_max);

	translate(slider_postdate, "", "");
	ReplaceOne(slider_postdate, "%s", postdate_min);
	ReplaceOne(slider_postdate, "%s", postdate_max);

	string slider_dimension = slider_widths + slider_heights + slider_ratios + slider_filesize;
	translate(slider_dimension, "", "");

	if (sliderValues.widths.empty())
	{
		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		writer.StartObject();
		writer.String("stat");
		writer.String("err");
		writer.EndObject();
		szResponse = sb.GetString();
	}
	else
	{
		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		writer.StartObject();
		writer.String("stat");
		writer.String("ok");
		writer.String("result");
		writer.StartObject();
		writer.String("slider_josn");
		writer.StartObject();

		string slider_item = "{'between %d and %d pixels'|@translate}";
		translate(slider_item, "", "");
		writer.String("widths");
		get_slider_josn(sliderValues, &writer, "widths", width_min, width_max, slider_item);

		slider_item = "{'between %d and %d pixels'|@translate}";
		translate(slider_item, "", "");
		writer.String("heights");
		get_slider_josn(sliderValues, &writer, "heights", height_min, height_max, slider_item);

		slider_item = "{'between %s and %s'|@translate}";
		translate(slider_item, "", "");
		writer.String("ratios");
		get_slider_josn(sliderValues, &writer, "ratios", ratio_min, ratio_max, slider_item);

		slider_item = "{'between %s and %s MB'|@translate}";
		translate(slider_item, "", "");
		writer.String("filesizes");
		get_slider_josn(sliderValues, &writer, "filesizes", filesize_min, filesize_max, slider_item);

		slider_item = "%s - %s";
		writer.String("createdates");
		get_slider_josn(sliderValues, &writer, "createdates", createdate_min, createdate_max, slider_item);

		slider_item = "%s - %s";
		writer.String("postdates");
		get_slider_josn(sliderValues, &writer, "postdates", postdate_min, postdate_max, slider_item);

		writer.EndObject();
		writer.String("slider_dimension");
		writer.String(slider_dimension.c_str());
		writer.String("slider_filesize");
		writer.String(slider_filesize.c_str());
		writer.String("slider_postdate");
		writer.String(slider_postdate.c_str());
		writer.String("slider_createdate");
		writer.String(slider_createdate.c_str());
		writer.EndObject();
		writer.EndObject();

		szResponse = sb.GetString();
	}

	return S_OK;
}

long CHtmlSearchProc::get_slider_josn(SLIDER_VALUES sliderValues, Writer<StringBuffer>* writer, string type, string width_min, string width_max, string slider_item)
{
	writer->StartObject();
	writer->String("values");
	writer->StartArray();

	if (type == "widths")
	{
		list<uint32_t>::iterator iter;
		for (iter = sliderValues.widths.begin(); iter != sliderValues.widths.end(); iter++)
		{
			writer->Int(*iter);
		}
	}
	if (type == "heights")
	{
		list<uint32_t>::iterator iter;
		for (iter = sliderValues.heights.begin(); iter != sliderValues.heights.end(); iter++)
		{
			writer->Int(*iter);
		}
	}
	if (type == "ratios")
	{
		string ratios_values = "";
		list<float>::iterator I;
		for (I = sliderValues.ratios.begin(); I != sliderValues.ratios.end(); I++)
		{
			ratios_values = float2Str(*I, "%.2f");
			writer->String(ratios_values.c_str());
		}
	}
	if (type == "filesizes")
	{
		string filesize_values = "";
		list<float>::iterator I;
		for (I = sliderValues.filesizes.begin(); I != sliderValues.filesizes.end(); I++)
		{
			filesize_values = float2Str(*I, "%.1f");
			writer->String(filesize_values.c_str());
		}
	}
	else if (type == "createdates")
	{
		string createdate_values = "'";
		list<uint64_t>::iterator it;
		for (it = sliderValues.createdates.begin(); it != sliderValues.createdates.end(); it++)
		{
			createdate_values = get_today(*it);
			writer->String(createdate_values.c_str());
		}
	}
	else if (type == "postdates")
	{
		string postdate_values = "'";
		list<uint64_t>::iterator it;
		for (it = sliderValues.postdates.begin(); it != sliderValues.postdates.end(); it++)
		{
			postdate_values = get_today(*it);
			writer->String(postdate_values.c_str());
		}
	}

	writer->EndArray();
	writer->String("selected");
	writer->StartObject();
	writer->String("min");
	writer->String(width_min.c_str());
	writer->String("max");
	writer->String(width_max.c_str());
	writer->EndObject();
	writer->String("text");
	writer->String(slider_item.c_str());
	writer->EndObject();
	return S_OK;
}
long CHtmlSearchProc::get_search_form(list<CATEGORY_DTO> lst, string& search_form, const QUERY_REQ req)
{
	search_form = "";
	Htmlread(IDR_SEARCH_FORM_HTML, L"html/search_form.html", search_form);


	SEARCH_REQ search_req = CDataSrv::Instance()._search_req;

	string sel_cat = "";
	lookitem(req.args, "category=", sel_cat);
	if (sel_cat != "")
	{
		search_req.cat = sel_cat;
		if (search_req.subcats == "on")
		{
			string cats = "";
			list<CATEGORY_DTO>::iterator iter;

			for (iter = lst.begin(); iter != lst.end(); iter++)
			{
				string uppercats = "," + iter->full_id + ",";
				string::size_type pos = uppercats.find("," + search_req.cat + ",");
				if (pos != string::npos)
				{
					cats += iter->id + ",";
				}

			}
			rtrim(cats, ",");
			search_req.allcats = cats;
		}
	}
	if (sel_cat == "0")
	{
		search_req.cat = "";
		search_req.allcats = "";
	}

	if (search_req.search_allwords == "")
	{
		string search_allwords = "";
		lookitem(req.args, "search_allwords=", search_allwords);

		URLDecode(search_allwords, 0);
		esc_string(search_allwords);

		search_req.search_allwords = search_allwords;
	}

	//menubar = "";

	ReplaceOne(search_form, "{$search_allwords}", search_req.search_allwords);


	string val = "0";
	string dis = "display:none;";
	//if (search_req.adv_available == "1")
	if (search_req.tags != "" || search_req.authors != "")
	{
		val = "1";
		dis = "";
	}
	ReplaceOne(search_form, "{$adv_available}", val);


	bool b0 = false;
	list<TAG_DTO>::iterator it;
	list<TAG_DTO>* ptrlstTag = CDataSrv::Instance().get_lstTag();
	for (it = ptrlstTag->begin(); it != ptrlstTag->end(); it++)
	{
		if (it->nb_images > 0)
		{
			b0 = true;
			break;
		}
	}

	string tag_opt = "";
	if (!b0)
	{
	}
	else
	{

		string tags = "," + search_req.tags + ",";
		string opt = "<option value=\"{$id}\" {$sel}>{$name} ({$str_photo})</option>";
		for (it = ptrlstTag->begin(); it != ptrlstTag->end(); it++)
		{
			if (it->nb_images > 0)
			{
				string sel = "";
				string temp = opt;
				ReplaceOne(temp, "{$id}", it->id);
				ReplaceOne(temp, "{$name}", it->name);

				string::size_type pos = tags.find("," + it->id + ",");
				if (pos != string::npos)
				{
					sel = "selected=\"selected\"";
				}
				ReplaceOne(temp, "{$sel}", sel);

				string str_photo = get_photo_string(it->nb_images);

				ReplaceOne(temp, "{$str_photo}", str_photo);
				tag_opt += temp;
			}
		}
	}
	ReplaceOne(search_form, "{$tag_opt}", tag_opt);

	string cat = "," + search_req.cat + ",";
	string category_opt = "";
	string opt = "<option value=\"{$id}\"  {$sel} >{$name}</option>";


	list<CATEGORY_DTO> lstCategroy;
	CHtmlgetMenubar::Instance().get_order_categories("a.full_id", lstCategroy);

	if (!lstCategroy.empty()) {

		list<CATEGORY_DTO>::iterator iter;
		list<CATEGORY_DTO>::iterator I;
		for (iter = lstCategroy.begin(); iter != lstCategroy.end(); iter++)
		{
			string fullname = "";
			vector<string> v1 = tokenize(iter->full_id, ",", true, "");
			int len = (int)v1.size();
			for (int i = 0; i < len; i++)
			{
				for (I = lstCategroy.begin(); I != lstCategroy.end(); I++)
				{
					if (I->id == trim(v1[i], " "))
					{
						fullname += I->name + "/";
					}
				}
			}
			rtrim(fullname, "/");

			string sel = "";
			string temp = opt;
			ReplaceOne(temp, "{$id}", iter->id);
			ReplaceOne(temp, "{$name}", fullname);

			string::size_type pos = cat.find("," + iter->id + ",");
			if (pos != string::npos)
			{
				sel = "selected=\"selected\"";
			}
			ReplaceOne(temp, "{$sel}", sel);

			category_opt += temp;
		}
	}
	ReplaceOne(search_form, "{$category_opt}", category_opt);

	map<string, uint32_t > mapAuthor;
	Cimagedal::Instance().get_image_authors(mapAuthor);

	string author_opt = "";
	if (mapAuthor.empty())
	{
		ReplaceOne(search_form, "g='author'", "style=\"display:none;\"");
	}
	else
	{
		ReplaceOne(search_form, "g='author'", "");

		string authors = "," + search_req.authors + ",";
		opt = "<option value=\"{$id}\" {$sel}>{$name} ({$str_photo})</option>";

		map<string, uint32_t >::iterator itr;
		for (itr = mapAuthor.begin(); itr != mapAuthor.end(); itr++)
		{
			if (itr->first != "")
			{
				string sel = "";
				string temp = opt;
				ReplaceOne(temp, "{$id}", itr->first);
				ReplaceOne(temp, "{$name}", itr->first);

				string::size_type pos = authors.find("," + itr->first + ",");
				if (pos != string::npos)
				{
					sel = "selected=\"selected\"";
				}
				ReplaceOne(temp, "{$sel}", sel);

				string str_photo = get_photo_string(itr->second);

				ReplaceOne(temp, "{$str_photo}", str_photo);
				author_opt += temp;
			}
		}
	}
	ReplaceOne(search_form, "{$author_opt}", author_opt);

	if (!b0 && mapAuthor.empty())
	{
		ReplaceOne(search_form, "ooobyadvooo", "display:none;");
	}
	else
	{
		ReplaceOne(search_form, "ooobyadvooo", "");
	}

	return 0;
}
