#include "StdAfx.h"
#include "unitil.h"
#include <vector>
#include <string>

#include "../../templ/templ/resource.h"
#include "imagedal.h"
#include "chatdal.h"
#include "DataSrv.h"
#include "lang.h"
#include "HtmlgetAdminProc.h"
#include "HtmlgetMenubar.h"
#include "writedb.h"

CHtmlgetAdminProc::CHtmlgetAdminProc(void)
{

}

CHtmlgetAdminProc::~CHtmlgetAdminProc(void)
{
}

long CHtmlgetAdminProc::proc_cat_list_htm(const REQUEST_DTO request_dto, string cat_list_infos, string& tpl)
{
	tpl = "";
	list<CATEGORY_DTO> lst;
	CHtmlgetMenubar::Instance().clear();
	CHtmlgetMenubar::Instance().get_categories_order_by_rank(1, lst);
	CHtmlgetMenubar::Instance().proc_categories(&lst);
	string parent_id = "";
	lookitem(request_dto.szArgs, "parent_id=", parent_id);

	string titlename = "";
	if (parent_id == "" || parent_id == "0")
	{
	}
	else
	{
		get_category_title(lst, "0", parent_id, "", titlename);
	}
	uint32_t nbCaddie = 0;
	uint32_t nbDuplicates = 0;
	uint32_t nbOrphan = 0;

	Htmlread(IDR_ADMIN_CAT_LIST_HTML, L"html/_admin_cat_list.html", tpl);
	ReplaceOne(tpl, "{$ov}", CDataSrv::Instance()._ov);

	CHtmlgetAdminProc::Instance().proc_head_htm("admin.cat_list", nbCaddie, nbDuplicates, nbOrphan, tpl);

	Replace(tpl, "{$parentid}", parent_id);
	ReplaceOne(tpl, "{$titlename}", titlename);
	ReplaceOne(tpl, "{$cat_list_infos}", cat_list_infos);

	string myid = parent_id;
	if (myid == "")
	{
		myid = "0";
	}

	string order_by = "";
	string recursive = "";
	string catOrder = "";
	CDataSrv::Instance().get_config_val(myid, catOrder);

	lookitem(catOrder, "order_by=", order_by);
	lookitem(catOrder, "recursive=", recursive);
	URLDecode(order_by, 1);

	ReplaceOne(tpl, "{$order_by}", order_by);
	ReplaceOne(tpl, "{$recursive}", recursive);



	string categoryUl = "";
	get_categoryUl(lst, parent_id, categoryUl);

	ReplaceOne(tpl, "{$categoryUl}", categoryUl);


	list<CATEGORY_DTO> lst2;
	CHtmlgetMenubar::Instance().get_categories_order_by_root_dir(1, lst2);
	CHtmlgetMenubar::Instance().proc_categories(&lst2);

	if (parent_id == "")
	{
		parent_id = "0";
	}
	string options = "";
	get_category_options("1", lst2, parent_id, "", options);
	ReplaceOne(tpl, "{$catoptions}", options);

	return S_OK;
}

long CHtmlgetAdminProc::proc_tag_htm(const REQUEST_DTO request_dto, string& tpl)
{
	string nohead = "";
	lookitem(request_dto.szArgs, "nohead=", nohead);

	string tags = "";
	string selectAction = "";

	lookitem(request_dto.szArgs, "selectAction=", selectAction);
	lookitem_arr(request_dto.szArgs, "tags=", tags);

	vector<string> v1 = tokenize(tags, ",", true, "");
	int len = (int)v1.size();

	if (selectAction == "edit" || selectAction == "duplicate")
	{
		if (selectAction == "edit")
		{
			Htmlread(IDR_TAG_EDIT_HTML, L"html/tag_edit_form.html", tpl);
		}
		else
		{
			Htmlread(IDR_TAG_DUPLICATE_HTML, L"html/tag_duplicate_form.html", tpl);
		}
		ReplaceOne(tpl, "{$values}", tags);
		ReplaceOne(tpl, "{$nohead}", nohead);

		list<TAG_DTO> lstTag;
		Cimagedal::Instance().get_tags(tags, lstTag);


		list<TAG_DTO>::iterator iter;
		for (iter = lstTag.begin(); iter != lstTag.end(); iter++)
		{
			Replace(tpl, "{$id}", iter->id);
			Replace(tpl, "{$name}", iter->name);
		}

	}
	else if (selectAction == "merge")
	{
		Htmlread(IDR_TAG_MERGE_HTML, L"html/tag_merge_form.html", tpl);
		ReplaceOne(tpl, "{$values}", tags);
		ReplaceOne(tpl, "{$nohead}", nohead);

		list<TAG_DTO> lstTag;
		Cimagedal::Instance().get_tags(tags, lstTag);

		string label = "";
		string tag_merge_form_label = "";
		CLang::Instance().getOneData("tag_merge_form_label", tag_merge_form_label);

		int i = 0;
		list<TAG_DTO>::iterator iter;
		for (iter = lstTag.begin(); iter != lstTag.end(); iter++)
		{
			string temp = tag_merge_form_label;
			Replace(temp, "{$id}", iter->id);
			Replace(temp, "{$name}", iter->name);
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

		ReplaceOne(tpl, "{$tag_merge_form_label}", label);
	}
	else if (selectAction == "delete")
	{
		string sel = "";
		lookitem(request_dto.szArgs, "sel=", sel);

		Htmlread(IDR_TAG_DEL_HTML, L"html/tag_del_form.html", tpl);


		list<TAG_DTO> lstTag;
		Cimagedal::Instance().get_tags(tags, lstTag);

		string name = "";
		list<TAG_DTO>::iterator iter;
		for (iter = lstTag.begin(); iter != lstTag.end(); iter++)
		{
			name += iter->name + ",";
		}
		rtrim(name, ",");

		if (lstTag.size() > 1)
		{
			name = "{" + name + "}";
		}
		ReplaceOne(tpl, "{$values}", tags);
		ReplaceOne(tpl, "{$sel}", sel);
		Replace(tpl, "{$name}", name);
	}
	else if (selectAction == "orphan")
	{
		Htmlread(IDR_TAG_ORPHAN_HTML, L"html/tag_orphan_form.html", tpl);

		list<TAG_DTO> lstTag;
		Cimagedal::Instance().get_tags("", lstTag);

		int nb = 0;
		string name = "";
		list<TAG_DTO>::iterator iter;
		for (iter = lstTag.begin(); iter != lstTag.end(); iter++)
		{
			if (iter->nb_images == 0)
			{
				nb++;
				name += iter->name + ",";
			}
		}
		rtrim(name, ",");

		translate(tpl, "", Int2Str(nb));

		Replace(tpl, "{$name}", name);
	}
	else
	{
		tpl = "";
		Htmlread(IDR_ADMIN_TAGS_HTML, L"html/_admin_tags.html", tpl);
		ReplaceOne(tpl, "{$ov}", CDataSrv::Instance()._ov);
		ReplaceOne(tpl, "{$seltags}", tags);

		string style = "";
		if (nohead != "")
		{
			style = "style=\"display:none;\"";
			ReplaceOne(tpl, "id=\"content\"", "");
			ReplaceOne(tpl, "calc(100% - 80px)", "calc(100% - 20px)");
		}

		Replace(tpl, "o='style'", style);
		ReplaceOne(tpl, "{$nohead}", nohead);

		string sel = "";
		lookitem(request_dto.szArgs, "sel=", sel);
		ReplaceOne(tpl, "{$sel}", sel);

		int nb = 0;

		list<TAG_DTO> lstTag;
		Cimagedal::Instance().get_tags("", lstTag);
		if (lstTag.empty())
		{
			ReplaceOne(tpl, "{$tags}", "");
			ReplaceOne(tpl, "{$nb}", "0");
		}
		else
		{
			ReplaceOne(tpl, "{$nb}", Int2Str((int)lstTag.size()));

			string str_photo = "";
			string str_photos = "";
			string tag_no_photo = "";
			string tag_have_photo = "";
			CLang::Instance().getOneData("str_photo", str_photo);
			CLang::Instance().getOneData("str_photos", str_photos);
			CLang::Instance().getOneData("tag_no_photo", tag_no_photo);
			CLang::Instance().getOneData("tag_have_photo", tag_have_photo);

			string mtitle = "";
			CLang::Instance().getOneData("tag_mtitle", mtitle);

			string alltags = "";
			string tag_li = "";
			CLang::Instance().getOneData("tag_li", tag_li);

			list<TAG_DTO>::iterator iter;
			for (iter = lstTag.begin(); iter != lstTag.end(); iter++)
			{
				string temp = tag_li;

				string t1 = tag_no_photo;
				if (iter->nb_images == 1)
				{
					t1 = tag_have_photo;
					Replace(t1, "photonb", str_photo);
				}
				else if (iter->nb_images > 1)
				{
					t1 = tag_have_photo;
					Replace(t1, "photonb", str_photos);
				}
				else
				{
					nb++;
				}
			
				string title = mtitle;
				Replace(title, "tag-dropdown-header", t1);
				translate(title, "", "");
				Replace(title, "\"", "&quot;");
			
				ReplaceOne(temp, "tag_mtitle", title);
			
				
				Replace(temp, "{$id}", iter->id);
				Replace(temp, "{$name}", iter->name);

				translate(temp, "", "");
				Replace(temp, "%d", uint2str(iter->nb_images));

				bool b = false;
				for (int i = 0; i < len; i++)
				{
					if (v1[i] == iter->id)
					{
						b = true;
						break;
					}
				}
				if (!b)
				{
					ReplaceOne(temp, "checked", "");
				}

				alltags += temp;
			}
			ReplaceOne(tpl, "{$tags}", alltags);
		}
		string refresh = "";
		lookitem(request_dto.szArgs, "refresh=", refresh);

		string tag_orphan = "";
		if (refresh == "" && nb > 0)
		{
			CLang::Instance().getOneData("tag_orphan", tag_orphan);
			translate(tag_orphan, "", "");
			ReplaceOne(tag_orphan, "%s", Int2Str(nb));
		}

		ReplaceOne(tpl, "{$tag_orphan}", tag_orphan);

	}
	return S_OK;
}

long CHtmlgetAdminProc::proc_stats_htm(const REQUEST_DTO request_dto, string photo_infos, string& tpl)
{
	string year = "";
	string month = "";
	string day = "";
	lookitem(request_dto.szArgs, "year=", year);
	lookitem(request_dto.szArgs, "month=", month);
	lookitem(request_dto.szArgs, "day=", day);

	list<HISTORY_SUMMARY_DTO> lstSummary;
	list<HISTORY_SUMMARY_DTO>::iterator iter;
	Cchatdal::Instance().get_summary(year, month, day, CDataSrv::Instance()._time_zone_bias, lstSummary);

	uint32_t nb_max = 0;
	for (iter = lstSummary.begin(); iter != lstSummary.end(); iter++)
	{
		if (iter->nb_pages > nb_max)
		{
			nb_max = iter->nb_pages;
		}
	}

	Htmlread(IDR_ADMIN_STATS_HTML, L"html/_admin_stats.html", tpl);
	ReplaceOne(tpl, "{$ov}", CDataSrv::Instance()._ov);

	string title = "";
	get_stats_title(year, month, day, title);
	ReplaceOne(tpl, "{$title}", title);

	if (year != "" && month != "" && day != "")
	{
		ReplaceOne(tpl, "{$head0}", "{'Hour'|@translate}");
	}
	else if (year != "" && month != "")
	{
		ReplaceOne(tpl, "{$head0}", "{'Day'|@translate}");
	}
	else if (year != "")
	{
		ReplaceOne(tpl, "{$head0}", "{'Month'|@translate}");
	}
	else
	{
		ReplaceOne(tpl, "{$head0}", "{'Year'|@translate}");
	}

	string tr_stats = "";
	if (!lstSummary.empty())
	{
		string a_html = "<a href=\"admin.html?page=admin.stats&{$args}\">{$date}</a>";
		string templ = "";
		CLang::Instance().getOneData("tr_stats", templ);

		if (year != "" && month != "" && day != "")
		{
			for (int i = 0; i < 24; i++)
			{
				uint32_t nb_pages = 0;
				for (iter = lstSummary.begin(); iter != lstSummary.end(); iter++)
				{
					if (i == atoi(iter->hour.c_str()))
					{
						nb_pages = iter->nb_pages;
						break;
					}
				}

				string temp = templ;

				char buf[100] = { 0 };
				sprintf_s(buf, 100, "%02d", i);

				ReplaceOne(temp, "{$a}", buf);
				ReplaceOne(temp, "{$number}", (nb_pages == 0) ? "" : uint2str(nb_pages));

				float len = static_cast<float>(nb_pages) * 400.0f / static_cast<float>(nb_max);
				ReplaceOne(temp, "0px", float2Str(len, "%.1fpx;"));
				tr_stats += temp;
			}
		}
		else if (year != "" && month != "")
		{
			tm tm1;
			tm1.tm_year = atoi(year.c_str()) - 1900;
			tm1.tm_mon = atoi(month.c_str()) - 1;
			tm1.tm_mday = 1;
			tm1.tm_hour = 0;
			tm1.tm_min = 0;
			tm1.tm_sec = 0;
			tm1.tm_isdst = 0;

			time_t t1 = _mkgmtime(&tm1);

			struct tm tm2;
			gmtime_s(&tm2, &t1);
			int weekstart = tm2.tm_wday;

			int days = getdays(atoi(year.c_str()), atoi(month.c_str()));
			for (int i = 1; i <= days; i++)
			{
				string a = a_html;
				ReplaceOne(a, "{$args}", "year=" + year + "&month=" + month + "&day=" + Int2Str(i));
				ReplaceOne(a, "{$date}", get_day_name(i, weekstart));

				uint32_t nb_pages = 0;
				for (iter = lstSummary.begin(); iter != lstSummary.end(); iter++)
				{
					if (i == atoi(iter->day.c_str()))
					{
						nb_pages = iter->nb_pages;
						break;
					}
				}
				string temp = templ;
				if (nb_pages == 0)
				{
					a = get_day_name(i, weekstart);
				}

				ReplaceOne(temp, "{$a}", a);
				ReplaceOne(temp, "{$number}", (nb_pages == 0) ? "" : uint2str(nb_pages));

				string w = get_width_px(nb_pages, nb_max);
				ReplaceOne(temp, "0px", w + "px");

				tr_stats += temp;

				weekstart++;
				if (weekstart > 6)
				{
					weekstart = 0;
				}
			}
		}
		else if (year != "")
		{
			for (int i = 1; i < 13; i++)
			{
				string a = a_html;
				ReplaceOne(a, "{$args}", "year=" + year + "&month=" + Int2Str(i));
				ReplaceOne(a, "{$date}", get_month_name(i));

				uint32_t nb_pages = 0;
				for (iter = lstSummary.begin(); iter != lstSummary.end(); iter++)
				{
					if (i == atoi(iter->month.c_str()))
					{
						nb_pages = iter->nb_pages;
						break;
					}
				}
				string temp = templ;
				if (nb_pages == 0)
				{
					a = get_month_name(i);
				}

				ReplaceOne(temp, "{$a}", a);
				ReplaceOne(temp, "{$number}", (nb_pages == 0) ? "" : uint2str(nb_pages));

				string w = get_width_px(nb_pages, nb_max);
				ReplaceOne(temp, "0px", w + "px");
				tr_stats += temp;
			}
		}
		else
		{
			for (iter = lstSummary.begin(); iter != lstSummary.end(); iter++)
			{
				string a = a_html;
				ReplaceOne(a, "{$args}", "year=" + iter->year);
				ReplaceOne(a, "{$date}", iter->year);

				string temp = templ;
				ReplaceOne(temp, "{$a}", a);
				ReplaceOne(temp, "{$number}", (iter->nb_pages == 0) ? "" : uint2str(iter->nb_pages));

				string w = get_width_px(iter->nb_pages, nb_max);
				ReplaceOne(temp, "0px", w + "px");

				tr_stats += temp;
			}
		}
	}

	ReplaceOne(tpl, "{$tr_stats}", tr_stats);

	return S_OK;
}

long CHtmlgetAdminProc::get_stats_title(string year, string month, string day, string& title)
{
	string title0 = "<a href=\"./admin.html?page=admin.stats\">{'Overall'|@translate}</a>";
	if (year != "" && month != "" && day != "")
	{
		title = title0;
		string temp = " / <a href=\"./admin.html?page=admin.stats&year={$year}\">{$year}</a>";
		Replace(temp, "{$year}", year);
		title += temp;

		temp = " / <a href=\"./admin.html?page=admin.stats&year={$year}&month={$month}\">{$name}</a>";
		Replace(temp, "{$year}", year);
		Replace(temp, "{$month}", month);
		Replace(temp, "{$name}", "{'month" + month + "'|@translate}");
		title += temp;

		temp = " / {$name}";
		Replace(temp, "{$name}", day + " ({'day" + "0" + "'|@translate})");
		title += temp;
	}
	else if (year != "" && month != "")
	{
		title = title0;
		string temp = " / <a href=\"./admin.html?page=admin.stats&year={$year}\">{$year}</a>";
		Replace(temp, "{$year}", year);
		title += temp;

		temp = " / {$name}";
		Replace(temp, "{$name}", "{'month" + month + "'|@translate}");
		title += temp;
	}
	else if (year != "")
	{
		string temp = " / {$year}";
		Replace(temp, "{$year}", year);
		title = title0 + temp;
	}
	else
	{
		title = title0;
	}
	return 0;
}

long CHtmlgetAdminProc::proc_photo_htm(string page, const REQUEST_DTO request_dto, string photo_infos, string& tpl)
{
	string id = "";
	string unit = "";
	string idx = "";
	string category = "";
	string osm = "";
	lookitem(request_dto.szArgs, "photo=", id);
	lookitem(request_dto.szArgs, "unit=", unit);
	lookitem(request_dto.szArgs, "idx=", idx);
	lookitem(request_dto.szArgs, "category=", category);
	lookitem(request_dto.szArgs, "osm=", osm);

	if (idx != "")
	{
		int index = atoi(idx.c_str());

		int i = 0;
		list<string> plstImageIds;
		if (osm != "")
		{
			plstImageIds = CDataSrv::Instance()._lstosm;
		}
		else
		{
			CDataSrv::Instance().get_lstImageIds(plstImageIds);
		}

		list<string>::iterator it;
		for (it = plstImageIds.begin(); it != plstImageIds.end(); it++)
		{
			if (index == i)
			{
				id = *it;
				break;
			}
			i++;
		}
	}

	if (id == "")
	{
		id = "0";
	}

	PHOTO_DTO photo_dto;
	Cimagedal::Instance().get_photo(id, photo_dto);

	if (photo_dto.ori == "")
	{
		tpl = "{'nothing'|@translate}";
		translate(tpl, "", "");
		return S_OK;
	}

	{
		Htmlread(IDR_PIC_PROPERTIES, L"html/_pic_properties.html", tpl);

		if (CDataSrv::Instance()._skin != "dark" && CDataSrv::Instance()._skin != "quartz")
		{
			ReplaceOne(tpl, "selectize.dark.css", "selectize.clear.css");
		}
		if (CDataSrv::Instance()._skin == "blueberry" || CDataSrv::Instance()._skin == "dark" || CDataSrv::Instance()._skin == "quartz")
		{
			ReplaceOne(tpl, "strawberry_jam", "newspaper");
		}
		else
		{
			ReplaceOne(tpl, "strawberry_jam", CDataSrv::Instance()._skin);
		}
		ReplaceOne(tpl, "{$ov}", CDataSrv::Instance()._ov);

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

		uint32_t r = CDataSrv::Instance()._readonly;
		ReplaceOne(tpl, "{$readonly}", uint2str(r));
		ReplaceOne(tpl, "{$error_readonly}", CDataSrv::Instance()._str_readonly);

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

		string ext = photo_dto.ext;
		my_tolower(ext);

		list<ROOT_DTO> lstRoot;
		CDataSrv::Instance().get_roots(lstRoot);

		Replace(tpl, "{$id}", id);

		string dest = ori2path(lstRoot, 1, photo_dto.ori);
		if (CDataSrv::Instance()._albumName == "private")
		{
			dest += ".krf";
		}
		string albumLinks = "";
		string photo_infos_file = "";
		CLang::Instance().getOneData("photo_infos_file", photo_infos_file);
		translate(photo_infos_file, "", "");
		ReplaceOne(photo_infos_file, "{$dest}", dest);
		ReplaceOne(photo_infos_file, "{$filen}", dest);

		string photo_infos_posted = "";
		CLang::Instance().getOneData("photo_infos_posted", photo_infos_posted);
		ReplaceOne(photo_infos_posted, "%s", get_time_string(photo_dto.date_available, "1"));

		string photo_infos_added = "";
		CLang::Instance().getOneData("photo_infos_added", photo_infos_added);
		translate(photo_infos_added, "", "");
		ReplaceOne(photo_infos_added, "%s", photo_dto.added_by);

		string photo_infos_size = "";
		CLang::Instance().getOneData("photo_infos_size", photo_infos_size);

		bool b = false;
		if (photo_dto.ext == "mp3" || photo_dto.ext == "ogg")
		{
			if (photo_dto.representative_ext == "")
			{
				b = true;
			}
		}
		else if (photo_dto.ext == "mp4" || photo_dto.ext == "webm")
		{
			if (photo_dto.representative_ext != "")
			{
				b = true;
			}
		}

		float flen = static_cast<float>(get_uint32(photo_dto.filesize.c_str())) / 10.0f;
		char buf2[MAX_PATH] = { 0 };

		if (b)
		{
			sprintf_s(buf2, "%.1fMB ", flen);
		}
		else
		{
			sprintf_s(buf2, "%s &times %s %.1fMB ", photo_dto.width.c_str(), photo_dto.height.c_str(), flen);
		}


		translate(photo_infos_size, "", "");
		ReplaceOne(photo_infos_size, "{$size}", buf2);

		string photo_infos_hit = "";
		CLang::Instance().getOneData("photo_infos_hit", photo_infos_hit);
		translate(photo_infos_hit, "", "");
		ReplaceOne(photo_infos_hit, "%d", photo_dto.hit);

		string photo_infos_creation = "";
		CLang::Instance().getOneData("photo_infos_creation", photo_infos_creation);
		translate(photo_infos_creation, "", "");

		string t = get_time_string(photo_dto.date_creation, "0");
		ltrim(t, " ");
		Replace(photo_infos_creation, "{$creation}", t);

		Replace(photo_infos_creation, "{$createval}", uint64str(photo_dto.date_creation));
		Replace(photo_infos_creation, "{$id}", photo_dto.id);

		string photo_infos_id = "";
		CLang::Instance().getOneData("photo_infos_id", photo_infos_id);
		translate(photo_infos_id, "", "");
		ReplaceOne(photo_infos_id, "%d", photo_dto.id);

		albumLinks += photo_infos_file;
		albumLinks += photo_infos_posted;
		albumLinks += photo_infos_added;
		albumLinks += photo_infos_size;
		albumLinks += photo_infos_creation;
		albumLinks += photo_infos_hit;
		albumLinks += photo_infos_id;
		Replace(tpl, "{$albumLinks}", albumLinks);

		string tag_val = "{&quot;id&quot;:&quot;~~{$id}~~&quot;}";
		string all_tags = "";

		if (photo_dto.tags != "")
		{
			vector<string> v1 = tokenize(photo_dto.tags, ",", true, "");
			int len = (int)v1.size();
			for (int i = 0; i < len; i++)
			{
				string temp = tag_val;
				ReplaceOne(temp, "{$name}", v1[i]);
				ReplaceOne(temp, "{$id}", v1[i]);
				all_tags += temp + ",";
			}
		}
		rtrim(all_tags, ",");

		string id3 = uint2str(photo_dto.storage_category_id);
		string opt = "\"~~{$id}~~\"";

		string associate = "";
		if (photo_dto.associate != "")
		{
			vector<string> v1 = tokenize(photo_dto.associate, ",", true, "");
			int len = (int)v1.size();
			for (int i = 0; i < len; i++)
			{
				if (id3 != v1[i])
				{
					string temp = opt;
					ReplaceOne(temp, "{$id}", v1[i]);
					associate += temp + ",";
				}
			}
		}
		rtrim(associate, ",");

		Replace(tpl, "{$imagename}", photo_dto.name);
		ReplaceOne(tpl, "{$author}", photo_dto.author);
		ReplaceOne(tpl, "$associate", associate);
		ReplaceOne(tpl, "{$categories}", photo_dto.storage_category);
		ReplaceOne(tpl, "{$tags}", all_tags);
		ReplaceOne(tpl, "{$description}", photo_dto.description);
		ReplaceOne(tpl, "{$category}", category);
		ReplaceOne(tpl, "{$catid}", id3);

		IMAGE_DTO image_dto;
		image_dto.coi = photo_dto.coi;
		image_dto.ori = photo_dto.ori;
		image_dto.ext = photo_dto.ext;
		image_dto.coi_w = photo_dto.coi_w;
		image_dto.coi_h = photo_dto.coi_h;
		image_dto.iscoi = photo_dto.iscoi;
		image_dto.representative_ext = photo_dto.representative_ext;
		string url = CDataSrv::Instance().get_image_url(1, image_dto);
		ReplaceOne(tpl, "{$oriurl}", url);

		string url0 = CDataSrv::Instance().get_image_url(0, image_dto);
		Replace(tpl, "{$photourl}", url0);
		Replace(tpl, "{$photo}", photo_dto.id);
	}
	return S_OK;
}

long CHtmlgetAdminProc::proc_head_htm(string page, uint32_t& nbCaddie, uint32_t& nbDuplicates, uint32_t& nbOrphan, string& tpl)
{
	uint32_t r = CDataSrv::Instance()._readonly;
	ReplaceOne(tpl, "{$readonly}", uint2str(r));
	ReplaceOne(tpl, "{$error_readonly}", CDataSrv::Instance()._str_readonly);

	string head = "";
	CLang::Instance().getOneData("admin_head", head);
	ReplaceOne(head, "{$gallery_title}", CDataSrv::Instance()._gallery_title);
	ReplaceOne(tpl, "{$admin_head}", head);

	string menubar = "";
	Htmlread(IDR_ADMIN_MEMBER_HTML, L"html/_admin_menubar.html", menubar);

	ReplaceOne(menubar, "{$album}", CDataSrv::Instance()._albumName);

	string pending_span = "";
	if (page != "admin.galleries" && page != "admin.photos_add")
	{
		CLang::Instance().getOneData("pending_span", pending_span);
		if (CDataSrv::Instance()._ischanged != 0)
		{
			ReplaceOne(pending_span, "#f00;", "");
		}
		else
		{
			ReplaceOne(pending_span, "#f00;", "display:none;");
		}
	}
	ReplaceOne(menubar, "{$pending_span}", pending_span);

	time_t current_time;
	time(&current_time);
	struct tm tm0;
	localtime_s(&tm0, &current_time);
	string yearmonth = "year=" + Int2Str(tm0.tm_year + 1900);
	ReplaceOne(menubar, "{yearmonth}", yearmonth);

	string menubar_in_caddie = "";

	list<CADDIE_DTO> lstCaddie;
	Cchatdal::Instance().get_caddies("", lstCaddie);
	if (!lstCaddie.empty())
	{
		CLang::Instance().getOneData("menubar_in_caddie", menubar_in_caddie);
		ReplaceOne(menubar_in_caddie, "{$NB_PHOTOS_IN_CADDIE}", uint2str((uint32_t)lstCaddie.size()));

	}
	ReplaceOne(menubar, "{$menubar_in_caddie}", menubar_in_caddie);

	nbCaddie = (uint32_t)lstCaddie.size();

	list<string> lstImageIds;
	Cimagedal::Instance().get_duplicates(lstImageIds);
	nbDuplicates = (uint32_t)lstImageIds.size();

	string menubar_duplicates = "";
	if (nbDuplicates > 0)
	{
		CLang::Instance().getOneData("menubar_duplicates", menubar_duplicates);
		ReplaceOne(menubar_duplicates, "{$NB_PHOTOS_DUP}", uint2str(nbDuplicates));
	}
	ReplaceOne(menubar, "{$menubar_duplicates}", menubar_duplicates);

	string menubar_no_album = "";

	nbOrphan = 0;
	Cimagedal::Instance().get_orphan_nb_photos(nbOrphan);
	if (nbOrphan > 0)
	{
		CLang::Instance().getOneData("menubar_no_album", menubar_no_album);
		ReplaceOne(menubar_no_album, "{$NB_PHOTOS_NO_ALBUM}", uint2str(nbOrphan));
	}
	ReplaceOne(menubar, "{$menubar_no_album}", menubar_no_album);

	if (page == "")
	{
		ReplaceOne(menubar, "g='dashboard'", "class=\"meun-selected\"");
	}
	else if (page == "admin.galleries")
	{
		ReplaceOne(menubar, "g='galleries'", "class=\"meun-selected\"");
	}
	else if (page == "admin.tags")
	{
		ReplaceOne(menubar, "g='tags'", "class=\"meun-selected\"");
	}
	else if (page == "admin.cat_list" || page == "admin.album")
	{
		ReplaceOne(menubar, "g='cat_list'", "class=\"meun-selected\"");
	}
	else if (page == "admin.batch")
	{
		ReplaceOne(menubar, "g='global'", "class=\"meun-selected\"");
	}
	else if (page == "admin.config")
	{
		ReplaceOne(menubar, "g='configuration'", "class=\"meun-selected\"");
	}
	else if (page == "admin.stats")
	{
		ReplaceOne(menubar, "g='stats'", "class=\"meun-selected\"");
	}

	ReplaceOne(menubar, "g='dashboard'", "");
	ReplaceOne(menubar, "g='tags'", "");
	ReplaceOne(menubar, "g='cat_list'", "");

	ReplaceOne(menubar, "g='duplicates'", "");
	ReplaceOne(menubar, "g='caddie'", "");
	ReplaceOne(menubar, "g='no_album'", "");
	ReplaceOne(menubar, "g='global'", "");

	ReplaceOne(menubar, "g='configuration'", "");
	ReplaceOne(menubar, "g='stats'", "");

	ReplaceOne(tpl, "{$menubar}", menubar);

	return S_OK;
}

long CHtmlgetAdminProc::proc_properties_htm(string page, const REQUEST_DTO request_dto, string cat_infos, string& tpl)
{
	string cat_id = "";
	string properties = "";
	lookitem(request_dto.szArgs, "category=", cat_id);
	lookitem(request_dto.szArgs, "properties=", properties);

	CATEGORY_DTO curr;
	list<CATEGORY_DTO> lst;
	CHtmlgetMenubar::Instance().clear();
	CHtmlgetMenubar::Instance().get_categories_order_by_root_dir(1, lst);
	CHtmlgetMenubar::Instance().proc_categories(&lst);

	list<CATEGORY_DTO>::iterator iter;
	for (iter = lst.begin(); iter != lst.end(); iter++)
	{
		if (iter->id == cat_id)
		{
			curr = *iter;
			break;
		}
	}
	string titlename = "";
	get_category_title(lst, "0", "", cat_id, titlename);

	string uppercats = curr.full_id;
	vector<string> v1 = tokenize(uppercats, ",", true, "");
	int i = 0;
	int len = (int)v1.size();

	//category=35&page=admin.album&properties=1
	if (properties != "")
	{
		Htmlread(IDR_ADMIN_CAT_PROPERTIES_HTML, L"html/_admin_cat_properties.html", tpl);
		ReplaceOne(tpl, "{$ov}", CDataSrv::Instance()._ov);

		ReplaceOne(tpl, "{$titlename}", titlename);
		ReplaceOne(tpl, "{$photo}", curr.representative_id);

		string categoryUl_li_2 = "";
		{
			if (curr.nb_categoires > 0 && curr.sub_images > 0)
			{
				CLang::Instance().getOneData("categoryUl_li_2", categoryUl_li_2);
			}
			else
			{
				CLang::Instance().getOneData("categoryUl_li_2s", categoryUl_li_2);
			}
			ReplaceOne(categoryUl_li_2, "{$id}", curr.id);
		}

		ReplaceOne(tpl, "{$categoryUl_li_2}", categoryUl_li_2);

		uint32_t nbCaddie = 0;
		uint32_t nbDuplicates = 0;
		uint32_t nbOrphan = 0;
		proc_head_htm(page, nbCaddie, nbDuplicates, nbOrphan, tpl);
		IMAGE_DTO dto;
		string oriurl = "";
		string repsrc = "";
		{
			list<IMAGE_DTO> lstImage;
			Cimagedal::Instance().get_images_by_ids(curr.representative_id, lstImage);

			list<IMAGE_DTO>::iterator it;
			for (it = lstImage.begin(); it != lstImage.end(); it++)
			{
				dto = *it;
				oriurl = CDataSrv::Instance().get_image_url(1, *it);
				repsrc = CDataSrv::Instance().get_image_url(0, *it);
				break;
			}
		}

		string albumLinks_p = "";
		uint32_t nb_images = 0;
		uint64_t min = 0;
		uint64_t max = 0;
		Cimagedal::Instance().get_category_images(cat_id, min, max, nb_images);

		string manage_photos_li = "";
		string cat_albumThumbnail = "";
		if (nb_images == 0)
		{
			CLang::Instance().getOneData("albumLinks_p0", albumLinks_p);
			translate(albumLinks_p, "", "");
			ReplaceOne(albumLinks_p, "%d", curr.id);
		}
		else
		{
			if (min == max)
			{
				CLang::Instance().getOneData("albumLinks_p1", albumLinks_p);
			}
			else
			{
				CLang::Instance().getOneData("albumLinks_p", albumLinks_p);
			}
			translate(albumLinks_p, "", "");
			ReplaceOne(albumLinks_p, "%d", uint2str(curr.nb_images));
			ReplaceOne(albumLinks_p, "%d", curr.id);

			CLang::Instance().getOneData("manage_photos_li", manage_photos_li);

			string vcontrol_js = "";
			CLang::Instance().getOneData("vcontrol_js", vcontrol_js);
			if (dto.ext == "gif")
			{
				CLang::Instance().getOneData("vcontrol_gif", vcontrol_js);
			}

			CLang::Instance().getOneData("cat_albumThumbnail2", cat_albumThumbnail);
			ReplaceOne(cat_albumThumbnail, "vcontrol_js", vcontrol_js);
		}

		ReplaceOne(cat_albumThumbnail, "{$oriurl}", oriurl);
		Replace(cat_albumThumbnail, "{$repsrc}", repsrc);

		Replace(cat_albumThumbnail, "{$id}", curr.representative_id);
		if (dto.ext == "mp3" || dto.ext == "mp4" || dto.ext == "gif" || dto.ext == "webm" || dto.ext == "ogg")
		{
			Replace(cat_albumThumbnail, "display_pic", "display:none;");
			Replace(cat_albumThumbnail, "display_vdo", "");
			ReplaceOne(cat_albumThumbnail, "{$duration}", get_dur_string(dto.duration));
			ReplaceOne(cat_albumThumbnail, "thumbnail imgbg", "thumbnail");
		}
		else
		{
			Replace(cat_albumThumbnail, "display_pic", "");
			Replace(cat_albumThumbnail, "display_vdo", "display:none;");
			ReplaceOne(cat_albumThumbnail, "{$duration}", "");
		}

		ReplaceOne(tpl, "{$manage_photos_li}", manage_photos_li);
		ReplaceOne(tpl, "{$albumThumbnail}", cat_albumThumbnail);

		Replace(tpl, "{$photo}", curr.representative_id);

		if (nb_images != 0)
		{
			ReplaceOne(albumLinks_p, "%s", get_time_string(min, "0"));
			ReplaceOne(albumLinks_p, "%s", get_time_string(max, "0"));
		}

		if (CDataSrv::Instance()._albumName == "private")
		{
			ReplaceOne(tpl, "dispadd", "lihide");
		}

		string::size_type  pos = curr.dir0.find_first_of("/");
		if (pos != string::npos)
		{
			ReplaceOne(tpl, "dispdel", "lihide");
			ReplaceOne(tpl, "dispsyn", "lishow");
			ReplaceOne(tpl, "dispadd", "lihide");
		}
		else
		{
			ReplaceOne(tpl, "dispdel", "lishow");
			ReplaceOne(tpl, "dispsyn", "lihide");
		}
		ReplaceOne(tpl, "{$synroot}", curr.dir0);
		ReplaceOne(tpl, "{$root}", curr.dir0);
		ReplaceOne(tpl, "{$albumLinks_p}", albumLinks_p);

		string parent = cat_id;
		for (i = 0; i < len; i++)
		{
			if (cat_id == trim(v1[i], " "))
			{
				break;
			}
			parent = v1[i];
		}

		if (parent == cat_id)
		{
			parent = "0";
		}

		Replace(tpl, "{$cat_id}", curr.id);
		ReplaceOne(tpl, "{$comment}", curr.comment);
		ReplaceOne(tpl, "{$name}", curr.name);
		ReplaceOne(tpl, "{$parent_id}", parent);

		string options = "";
		get_category_options("0", lst, "", parent, options);
		ReplaceOne(tpl, "{$parent_options}", options);

	}
	else
	{
		Htmlread(IDR_ADMIN_CAT_ORDER_HTML, L"html/_admin_cat_order.html", tpl);
		ReplaceOne(tpl, "{$ov}", CDataSrv::Instance()._ov);
		Replace(tpl, "{$cat_id}", cat_id);

		Replace(titlename, "properties", "sort_order");
		ReplaceOne(tpl, "{$titlename}", titlename);


		list<ORDER_DTO> lstO = CDataSrv::Instance()._lstOrder;
		list<ORDER_DTO>::iterator it;

		string title = "";
		for (it = lstO.begin(); it != lstO.end(); it++)
		{
			if (it->order == CDataSrv::Instance()._orderby)
			{
				title = it->title;
				break;
			}
		}
		Replace(title, "h2", "span");
		Replace(title, "calendarTitle", "");
		ReplaceOne(tpl, "{$sortOrder}", title);

		if (curr.imageorder == "")
		{
			ReplaceOne(tpl, "g='image_order_default'", "checked=\"checked\"");
			ReplaceOne(tpl, "g='image_order_rank'", "");
			ReplaceOne(tpl, "g='image_order_user_define'", "");
		}
		else if (curr.imageorder == "b.rank asc")
		{
			ReplaceOne(tpl, "g='image_order_default'", "");
			ReplaceOne(tpl, "g='image_order_rank'", "checked=\"checked\"");
			ReplaceOne(tpl, "g='image_order_user_define'", "");
		}
		else
		{
			ReplaceOne(tpl, "g='image_order_default'", "");
			ReplaceOne(tpl, "g='image_order_rank'", "");
			ReplaceOne(tpl, "g='image_order_user_define'", "checked=\"checked\"");
		}
		string order0 = "";
		string order1 = "";
		string order2 = "";

		vector<string> v1 = tokenize(curr.imageorder, ",", true, "");
		int len = (int)v1.size();
		for (int i = 0; i < len; i++)
		{
			if (i == 0)
			{
				order0 = v1[0];
			}
			else if (i == 1)
			{
				order1 = v1[1];
			}
			else if (i == 2)
			{
				order2 = v1[2];
			}
		}

		string image_order_tpl = "";
		CLang::Instance().getOneData("image_order", image_order_tpl);

		string temp = get_image_order_htm(image_order_tpl, order0);
		ReplaceOne(tpl, "{$image_order0}", temp);

		string thumbnails = "";
		{
			string rank_of_image = "";
			CLang::Instance().getOneData("rank_of_image", rank_of_image);

			QUERY_REQ req;
			req.cat_id = cat_id;
			req.page_nb = "9999";
			req.args = "rank";

			list<string> lstImageIds;
			Cimagedal::Instance().get_image_ids(req, lstImageIds);

			list<IMAGE_DTO> lstImageDto;
			Cimagedal::Instance().get_images_by_cats(cat_id, 0, lstImageDto);
			int len = (int)lstImageIds.size();

			div_t div_result = div(len - PERPAGE_NUM, PERPAGE_NUM);
			uint32_t maxPage = div_result.quot;
			if (div_result.rem != 0) {
				maxPage += 1;
			}
			ReplaceOne(tpl, "{$maxPage}", uint2str(maxPage));

			uint32_t i = 10;

			list<IMAGE_DTO>::iterator iter;
			for (iter = lstImageDto.begin(); iter != lstImageDto.end(); iter++)
			{
				i += 10;
				string temp = rank_of_image;

				string url0 = CDataSrv::Instance().get_image_url(1, *iter);
				ReplaceOne(temp, "{$url}", url0);
				ReplaceOne(temp, "{$id}", iter->id);
				ReplaceOne(temp, "{$rank}", uint2str(i));
				Replace(temp, "{$name}", iter->name);
				thumbnails += temp;
			}

			if (thumbnails != "")
			{
				thumbnails = "<ul class=\"thumbnails ui-sortable\">" + thumbnails + "</ul>";
			}
		}

		uint32_t nbCaddie = 0;
		uint32_t nbDuplicates = 0;
		uint32_t nbOrphan = 0;
		proc_head_htm(page, nbCaddie, nbDuplicates, nbOrphan, tpl);

		Replace(tpl, "{$thumbnails}", thumbnails);
	}

	string options = "";
	get_category_options("0", lst, "", cat_id, options);
	ReplaceOne(tpl, "{$catoptions}", options);

	return 0;
}

long CHtmlgetAdminProc::proc_cat_name_htm(const REQUEST_DTO request_dto, string cat_infos, string& tpl)
{
	string cat_id = "";
	lookitem(request_dto.szArgs, "category=", cat_id);

	list<CATEGORY_DTO> lst;
	CHtmlgetMenubar::Instance().clear();
	CHtmlgetMenubar::Instance().get_categories_order_by_root_dir(1, lst);
	CHtmlgetMenubar::Instance().proc_categories(&lst);

	CATEGORY_DTO curr;
	list<CATEGORY_DTO>::iterator iter;
	for (iter = lst.begin(); iter != lst.end(); iter++)
	{
		if (iter->id == cat_id)
		{
			curr = *iter;
			break;
		}
	}
	string titlename = "";
	get_category_title(lst, "1", "", cat_id, titlename);

	string uppercats = curr.full_id;
	vector<string> v1 = tokenize(uppercats, ",", true, "");
	int i = 0;
	int len = (int)v1.size();

	Htmlread(IDR_ADMIN_CAT_NAME_HTML, L"html/_admin_cat_name.html", tpl);
	ReplaceOne(tpl, "{$ov}", CDataSrv::Instance()._ov);

	ReplaceOne(tpl, "{$titlename}", titlename);
	ReplaceOne(tpl, "{$photo}", curr.representative_id);
	ReplaceOne(tpl, "{$synroot}", curr.dir0);
	ReplaceOne(tpl, "{$root}", curr.dir0);






	string parent = cat_id;
	for (i = 0; i < len; i++)
	{
		if (cat_id == trim(v1[i], " "))
		{
			break;
		}
		parent = v1[i];
	}

	if (parent == cat_id)
	{
		parent = "0";
	}




	string options = "";
	get_category_options("0", lst, "", parent, options);
	ReplaceOne(tpl, "{$parent_options}", options);

	IMAGE_DTO dto;
	string oriurl = "";
	string repsrc = "";
	{
		list<IMAGE_DTO> lstImage;
		Cimagedal::Instance().get_images_by_ids(curr.representative_id, lstImage);

		list<IMAGE_DTO>::iterator it;
		for (it = lstImage.begin(); it != lstImage.end(); it++)
		{
			dto = *it;
			oriurl = CDataSrv::Instance().get_image_url(1, *it);
			repsrc = CDataSrv::Instance().get_image_url(0, *it);
			break;
		}
	}

	string albumLinks_p = "";
	uint32_t nb_images = 0;
	uint64_t min = 0;
	uint64_t max = 0;
	Cimagedal::Instance().get_category_images(cat_id, min, max, nb_images);

	string cat_albumThumbnail = "";
	if (nb_images == 0)
	{
		CLang::Instance().getOneData("albumLinks_p0", albumLinks_p);
		translate(albumLinks_p, "", "");
		ReplaceOne(albumLinks_p, "%d", curr.id);
	}
	else
	{
		if (min == max)
		{
			CLang::Instance().getOneData("albumLinks_p1", albumLinks_p);
		}
		else
		{
			CLang::Instance().getOneData("albumLinks_p", albumLinks_p);
		}
		translate(albumLinks_p, "", "");
		ReplaceOne(albumLinks_p, "%d", uint2str(curr.nb_images));
		ReplaceOne(albumLinks_p, "%d", curr.id);

		string vcontrol_js = "";
		if (dto.ext == "mp3" || dto.ext == "mp4" || dto.ext == "webm" || dto.ext == "ogg")
		{
			CLang::Instance().getOneData("vcontrol_js", vcontrol_js);
			ReplaceOne(vcontrol_js, "o='url'", "style=\"display:none;\"");
		}

		CLang::Instance().getOneData("cat_albumThumbnail2", cat_albumThumbnail);
		ReplaceOne(cat_albumThumbnail, "vcontrol_js", vcontrol_js);
		if (dto.ext == "mp3" || dto.ext == "mp4" || dto.ext == "gif" || dto.ext == "webm" || dto.ext == "ogg")
		{
			ReplaceOne(cat_albumThumbnail, "thumbnail imgbg", "thumbnail");
		}
	}

	ReplaceOne(cat_albumThumbnail, "{$oriurl}", oriurl);
	Replace(cat_albumThumbnail, "{$repsrc}", repsrc);

	Replace(cat_albumThumbnail, "{$photo}", curr.representative_id);
	if (dto.ext == "mp3" || dto.ext == "mp4" || dto.ext == "webm" || dto.ext == "ogg")
	{
		Replace(cat_albumThumbnail, "display_pic", "display:none;");
		Replace(cat_albumThumbnail, "display_vdo", "");
		ReplaceOne(cat_albumThumbnail, "{$duration}", get_dur_string(dto.duration));
	}
	else
	{
		Replace(cat_albumThumbnail, "display_pic", "");
		Replace(cat_albumThumbnail, "display_vdo", "display:none;");
		ReplaceOne(cat_albumThumbnail, "{$duration}", "");
	}

	ReplaceOne(tpl, "{$albumThumbnail}", cat_albumThumbnail);

	Replace(tpl, "{$photo}", curr.representative_id);

	if (nb_images != 0)
	{
		ReplaceOne(albumLinks_p, "%s", get_time_string(min, "0"));
		ReplaceOne(albumLinks_p, "%s", get_time_string(max, "0"));
	}

	string::size_type  pos = curr.dir0.find_first_of("/");
	if (pos != string::npos)
	{
		ReplaceOne(tpl, "dispdel", "lihide");
		ReplaceOne(tpl, "dispsyn", "lishow");
	}
	else
	{
		ReplaceOne(tpl, "dispdel", "lishow");
		ReplaceOne(tpl, "dispsyn", "lihide");
	}
	ReplaceOne(tpl, "{$synroot}", curr.dir0);
	ReplaceOne(tpl, "{$albumLinks_p}", albumLinks_p);

	Replace(tpl, "{$cat_id}", curr.id);
	ReplaceOne(tpl, "{$comment}", curr.comment);
	ReplaceOne(tpl, "{$name}", curr.name);
	return 0;
}

long CHtmlgetAdminProc::proc_config_htm(string page, const REQUEST_DTO request_dto, string config_infos, string& tpl)
{
	Htmlread(IDR_ADMIN_CONFIG_HTML, L"html/_admin_config.html", tpl);
	ReplaceOne(tpl, "{$ov}", CDataSrv::Instance()._ov);
	ReplaceOne(tpl, "{$config_infos}", config_infos);
	ReplaceOne(tpl, "{$maxFileSize}", uint2str(CDataSrv::Instance()._maxFileSize));

	string ids = "'gallery_title','order_by'";
	list<CONFIG_DTO> lstConfig;
	Cchatdal::Instance().get_configs(ids, lstConfig);

	string gallery_title = "";
	string order_by = "";

	list<CONFIG_DTO>::iterator iter;
	for (iter = lstConfig.begin(); iter != lstConfig.end(); iter++)
	{
		if (iter->param == "gallery_title")
		{
			gallery_title = iter->value;
		}
		else if (iter->param == "order_by")
		{
			order_by = iter->value;
		}
	}

	if (gallery_title == "")
	{
		gallery_title = CDataSrv::Instance()._gallery_title;
	}

	ReplaceOne(tpl, "{$gallery_title}", gallery_title);

	if (order_by == "")
	{
		order_by = "date_creation DESC";
	}
	string filter = "";
	string orderby_filter = "";

	vector<string> v1 = tokenize(order_by, ",", true, "");
	int len = (int)v1.size();
	for (int i = 0; i < len; i++)
	{
		CLang::Instance().getOneData("orderby_filter", orderby_filter);
		string part = v1[i];
		ReplaceOne(orderby_filter, "g='file_ASC'", get_sel_opt(part, "file ASC"));
		ReplaceOne(orderby_filter, "g='file_DESC'", get_sel_opt(part, "file DESC"));
		ReplaceOne(orderby_filter, "g='name_ASC'", get_sel_opt(part, "a.name ASC"));
		ReplaceOne(orderby_filter, "g='name_DESC'", get_sel_opt(part, "a.name DESC"));
		ReplaceOne(orderby_filter, "g='date_creation_DESC'", get_sel_opt(part, "date_creation DESC"));
		ReplaceOne(orderby_filter, "g='date_creation_ASC'", get_sel_opt(part, "date_creation ASC"));
		ReplaceOne(orderby_filter, "g='date_available_DESC'", get_sel_opt(part, "date_available DESC"));
		ReplaceOne(orderby_filter, "g='date_available_ASC'", get_sel_opt(part, "date_available ASC"));

		ReplaceOne(orderby_filter, "g='hit_DESC'", get_sel_opt(part, "hit DESC"));
		ReplaceOne(orderby_filter, "g='hit_ASC'", get_sel_opt(part, "hit ASC"));

		filter += orderby_filter;
		break;
	}
	ReplaceOne(tpl, "{$filter}", filter);

	return 0;
}

string CHtmlgetAdminProc::get_ago_string(uint64_t date_available)
{
	time_t current_time;
	time(&current_time);

	int day = static_cast<int>(current_time - date_available) / (24 * 3600);
	//week0

	string fmt = "";
	int per = 1;
	if (day >= 730) {
		per = 365;
		CLang::Instance().getOneData("year99", fmt);
	}
	else if (day >= 365) {
		per = 365;
		CLang::Instance().getOneData("year0", fmt);
	}
	else if (day >= 60) {
		per = 30;
		CLang::Instance().getOneData("month99", fmt);
	}
	else if (day >= 30) {
		per = 30;
		CLang::Instance().getOneData("month0", fmt);
	}
	else if (day >= 14) {
		per = 7;
		CLang::Instance().getOneData("week99", fmt);
	}
	else if (day >= 7) {
		per = 7;
		CLang::Instance().getOneData("week0", fmt);
	}
	else if (day >= 2) {
		per = 1;
		CLang::Instance().getOneData("day99", fmt);
	}
	else if (day >= 1)
	{
		per = 1;
		CLang::Instance().getOneData("day0", fmt);
	}
	else
	{
		struct tm t;
		gmtime_s(&t, &current_time);
		t.tm_hour = 0;
		t.tm_min = 0;
		t.tm_sec = 0;
		t.tm_isdst = 0;

		time_t t0 = _mkgmtime(&t);// date_available is gmt

		if (date_available < (uint64_t)t0)
		{
			day = 1;
			per = 1;
			CLang::Instance().getOneData("day0", fmt);
		}
		else
		{
			per = 1;
			fmt = "{'today'|@translate}";
		}
	}

	div_t div_result = div(day, per);
	uint32_t nb = div_result.quot;
	translate(fmt, uint2str(nb), "");
	return fmt;
}

string CHtmlgetAdminProc::get_image_order_htm(string image_order_tpl, string image_order)
{
	string temp = image_order_tpl;
	string chk = "selected=\"selected\"";

	ReplaceOne(temp, "g='none'", image_order == "" ? chk : "");
	ReplaceOne(temp, "g='file ASC'", image_order == "file ASC" ? chk : "");
	ReplaceOne(temp, "g='file DESC'", image_order == "file DESC" ? chk : "");
	ReplaceOne(temp, "g='name ASC'", image_order == "a.name ASC" ? chk : "");
	ReplaceOne(temp, "g='name DESC'", image_order == "a.name DESC" ? chk : "");
	ReplaceOne(temp, "g='date_creation DESC'", image_order == "date_creation DESC" ? chk : "");
	ReplaceOne(temp, "g='date_creation ASC'", image_order == "date_creation ASC" ? chk : "");
	ReplaceOne(temp, "g='date_available DESC'", image_order == "date_available DESC" ? chk : "");
	ReplaceOne(temp, "g='date_available ASC'", image_order == "date_available ASC" ? chk : "");
	ReplaceOne(temp, "g='hit DESC'", image_order == "hit DESC" ? chk : "");
	ReplaceOne(temp, "g='hit ASC'", image_order == "hit ASC" ? chk : "");

	return temp;
}

string CHtmlgetAdminProc::get_sel_opt(string orderby, string val)
{
	string::size_type pos = orderby.find(val);
	if (pos != string::npos)
	{
		return "selected=\"selected\"";
	}
	return "";
}

string CHtmlgetAdminProc::get_day_name(int i, int weekstart)
{
	return Int2Str(i) + "({'day" + Int2Str(weekstart) + "'|@translate})";
}

string CHtmlgetAdminProc::get_month_name(int i)
{
	return "{'month" + Int2Str(i) + "'|@translate}";
}

string CHtmlgetAdminProc::get_width_px(uint32_t nb_pages, uint32_t nb_max)
{
	float len = static_cast<float>(nb_pages) * 400.0f / static_cast<float>(nb_max);
	return float2Str(len, "%.1f");
}

long CHtmlgetAdminProc::get_category_title(list<CATEGORY_DTO> lst, string type, string parent_id, string cat_id, string& titlename)
{
	string id = parent_id == "" ? cat_id : parent_id;

	CATEGORY_DTO curr;
	list<CATEGORY_DTO>::iterator iter;
	for (iter = lst.begin(); iter != lst.end(); iter++)
	{
		if (iter->id == id)
		{
			curr = *iter;
			break;
		}
	}

	string uppercats = curr.full_id;
	vector<string> v1 = tokenize(uppercats, ",", true, "");
	int i = 0;
	int len = (int)v1.size();

	titlename = "";
	string szT0 = "{$name}";

	if (type == "0")
	{
		szT0 = "<a href=\"./admin.html?page=admin.cat_list&parent_id={$cat_id}&properties=1\">{$name}</a>";
		if (parent_id != "")
		{
			szT0 = "<a href=\"./admin.html?page=admin.cat_list&parent_id={$cat_id}\">{$name}</a>";
		}
	}

	for (i = 0; i < len - 1; i++)
	{
		for (iter = lst.begin(); iter != lst.end(); iter++)
		{
			if (v1[i] == iter->id)
			{
				string temp = szT0;
				ReplaceOne(temp, "{$cat_id}", iter->id);
				ReplaceOne(temp, "{$name}", iter->name);
				titlename += temp;
				titlename += " / ";
			}
		}

		if (v1[i] == id)
		{
			break;
		}
	}

	titlename += curr.name;
	string cat_titlename = "{$titlename}";
	if (type == "0")
	{
		CLang::Instance().getOneData("cat_titlename1", cat_titlename);
	}
	ReplaceOne(cat_titlename, "{$titlename}", titlename);

	titlename = cat_titlename;
	return 0;
}

long CHtmlgetAdminProc::get_category_options(string ty, list<CATEGORY_DTO> lstCategroy, string parent_id, string cat_id, string& options)
{
	options = "";

	list<CATEGORY_DTO> lst;
	list<CATEGORY_DTO>::iterator iter;
	list<CATEGORY_DTO>::iterator it;

	if (ty == "2")
	{
		wstring path0 = L"";
		wstring szPath = CDataSrv::Instance()._appDataDir;//get_module_path(NULL);
		string album = CDataSrv::Instance()._albumName;

		wstring dDir = (album == "private") ? L"/data2" : L"/data1";
		path0 = szPath + dDir + L"/_data";

		for (iter = lstCategroy.begin(); iter != lstCategroy.end(); iter++)
		{
			if (iter->nb_images == 0 && iter->sub_images == 0)
			{
				continue;
			}

			string t9 = iter->dir0;
			my_tolower(t9);
			string::size_type pos0 = t9.find(_w2u(path0));
			if (pos0 != string::npos)
			{
				lst.push_back(*iter);
			}
		}
	}
	else if (parent_id == "")
	{
		for (iter = lstCategroy.begin(); iter != lstCategroy.end(); iter++)
		{
			if (iter->nb_images == 0 && iter->sub_images == 0)
			{
				continue;
			}
			lst.push_back(*iter);
		}
	}
	else
	{
		for (iter = lstCategroy.begin(); iter != lstCategroy.end(); iter++)
		{
			string fullname = "," + iter->full_id + ",";

			for (it = lstCategroy.begin(); it != lstCategroy.end(); it++)
			{
				if (it->nb_images == 0 && it->sub_images == 0)
				{
					continue;
				}
				string fullname2 = "," + it->full_id + ",";

				string::size_type pos = fullname2.find(fullname);
				if (pos != string::npos && fullname2 != fullname)
				{
					lst.push_back(*iter);
					break;
				}
			}
		}
	}

	vector<string> v0 = tokenize(cat_id, ",", true, "");
	int len0 = (int)v0.size();

	for (iter = lst.begin(); iter != lst.end(); iter++)
	{
		if (iter->nb_images == 0 && iter->sub_images == 0)
		{
			continue;
		}

		string fullname = "";
		vector<string> v1 = tokenize(iter->full_id, ",", true, "");
		int len = (int)v1.size();
		for (int i = 0; i < len; i++)
		{
			for (it = lst.begin(); it != lst.end(); it++)
			{
				if (it->id == trim(v1[i], " "))
				{
					fullname += it->name + "/";
				}
			}
		}

		if (ty == "0" || ty == "2")
		{
			rtrim(fullname, "/");
		}

		bool b = false;
		if (parent_id == iter->id)
		{
			b = true;
		}

		if (cat_id != "")
		{
			for (int i = 0; i < len0; i++)
			{
				if (v0[i] == iter->id)
				{
					b = true;
					break;
				}
			}
		}

		if (b)
		{
			options += " <option value=\"" + iter->id + "\" selected=\"selected\" >" + fullname + "</option>";
		}
		else
		{
			options += " <option value=\"" + iter->id + "\">" + fullname + "</option>";
		}
	}

	return 0;
}

long CHtmlgetAdminProc::get_categoryUl(list<CATEGORY_DTO> lst, string parent_id, string& categoryUl)
{
	categoryUl = "";

	list<CATEGORY_DTO> lstSub;
	list<CATEGORY_DTO>::iterator iter;
	if (parent_id == "" || parent_id == "0")
	{
		for (iter = lst.begin(); iter != lst.end(); iter++)
		{
			if (iter->nb_images == 0 && iter->sub_images == 0)
			{
				continue;
			}
			if (iter->id == iter->full_id)
			{
				lstSub.push_back(*iter);
			}
		}
	}
	else
	{
		list<CATEGORY_DTO> lstSub2;
		CHtmlgetMenubar::Instance().get_sub_categories_siblings(parent_id, lst, lstSub2);
		for (iter = lstSub2.begin(); iter != lstSub2.end(); iter++)
		{
			if (iter->nb_images == 0 && iter->sub_images == 0)
			{
				continue;
			}
			lstSub.push_back(*iter);
		}
	}

	string categoryUl_li = "";
	CLang::Instance().getOneData("categoryUl_li", categoryUl_li);

	string categoryUl_li_1 = "";
	string categoryUl_li_1s = "";

	CLang::Instance().getOneData("categoryUl_li_1", categoryUl_li_1);
	CLang::Instance().getOneData("categoryUl_li_1s", categoryUl_li_1s);

	uint32_t i = 0;
	for (iter = lstSub.begin(); iter != lstSub.end(); iter++)
	{
		if (iter->nb_images == 0 && iter->sub_images == 0)
		{
			continue;
		}

		i += 10;
		string temp = categoryUl_li;
		if (iter->nb_categoires > 0 && iter->sub_images > 0)
		{
			ReplaceOne(temp, "{$categoryUl_li_1}", categoryUl_li_1);
		}
		else
		{
			ReplaceOne(temp, "{$categoryUl_li_1}", categoryUl_li_1s);
		}

		if (iter->root_id != "")
		{
			ReplaceOne(temp, "page=admin.photos_add", "page=admin.galleries");
		}

		string categoryUl_li_2 = "";
		{
			if (iter->nb_categoires > 0 && iter->sub_images > 0)
			{
				CLang::Instance().getOneData("categoryUl_li_2", categoryUl_li_2);
				ReplaceOne(categoryUl_li_2, "{$id}", iter->id);
			}
			else
			{
				CLang::Instance().getOneData("categoryUl_li_2s", categoryUl_li_2);
			}
		}

		ReplaceOne(temp, "{$categoryUl_li_2}", categoryUl_li_2);

		//ReplaceOne(temp, "{$parent_id}", iter->id);
		Replace(temp, "{$id}", iter->id);
		Replace(temp, "{$name}", iter->name);
		ReplaceOne(temp, "{$orderVal}", uint2str(i));

		uint32_t num = iter->nb_images;
		string photo = "";
		string sub_photo = "";
		string sub_album = "";
		if (num == 1)
		{
			CLang::Instance().getOneData("str_photo", photo);
			CLang::Instance().getOneData("str_photo", sub_photo);
			CLang::Instance().getOneData("str_sub_album", sub_album);
		}
		else
		{
			CLang::Instance().getOneData("str_photos", photo);
			CLang::Instance().getOneData("str_photos", sub_photo);
			CLang::Instance().getOneData("str_sub_albums", sub_album);
		}
		translate(photo, uint2str(iter->nb_images), "");
		translate(sub_photo, uint2str(iter->sub_images), "");
		translate(sub_album, uint2str(iter->nb_categoires), "");

		if (iter->nb_categoires == 0)
		{
			sub_album = "";
		}

		if (iter->sub_images == 0)
		{
			sub_photo = "";
			sub_album = "";
		}

		ReplaceOne(temp, "{$photo}", photo);
		ReplaceOne(temp, "{$sub_photo}", sub_photo);
		ReplaceOne(temp, "{$sub_album}", sub_album);

		categoryUl += temp;
	}
	return 0;
}

long CHtmlgetAdminProc::move_categories(string cat_id, string parent)
{
	if (cat_id == parent)
	{
		return 0;
	}

	int retVal = 1;

	CHtmlgetMenubar::Instance().clear();

	list<CATEGORY_DTO> lst;
	CHtmlgetMenubar::Instance().get_categories_order_by_rank(0, lst);

	//1
	string uppercats1 = "";
	string tagert = "";

	list<CATEGORY_DTO>::iterator iter;
	for (iter = lst.begin(); iter != lst.end(); iter++)
	{
		if (iter->id == cat_id)
		{
			uppercats1 = "," + iter->full_id + ",";
		}
		if (iter->id == parent)
		{
			tagert = iter->full_id;
		}
	}

	bool b = true;
	string uppercats = "," + tagert + ",";
	string::size_type pos0 = uppercats.find("," + parent + ",");//1,2
	string::size_type pos1 = uppercats.find("," + cat_id + ","); //1
	if (pos0 != string::npos && pos1 != string::npos)
	{
		if (pos0 >= pos1)
		{
			b = false;
		}
	}
	if (!b) {
		return 0;
	}

	//3
	multimap<string, string> mov_map;

	for (iter = lst.begin(); iter != lst.end(); iter++)
	{
		string uppercats = "," + iter->full_id + ",";
		string::size_type pos = uppercats.find("," + cat_id + ",");
		if (pos != string::npos)
		{
			if (tagert != "")
			{
				Replace(uppercats, uppercats1, "," + tagert + "," + cat_id + ",");
			}
			else
			{
				Replace(uppercats, uppercats1, "," + cat_id + ",");
			}
			trim(uppercats, ",");
			mov_map.insert(multimap<string, string>::value_type(iter->id, uppercats));
		}
	}

	//4
	writedb::Instance().move_categories(mov_map);

	if (mov_map.empty())
	{
		retVal = 0;
	}

	return retVal;
}
