#include "StdAfx.h"
#include "unitil.h"
#include <vector>
#include <string>

#include "../../templ/templ/resource.h"

#include "HtmlgetIndexAction.h"
#include "lang.h"
#include "imagedal.h"
#include "Datasrv.h"

#include "HtmlgetMenubar.h"

CHtmlgetIndexAction::CHtmlgetIndexAction(void)
{
	_maxWidth = 144;
	_maxHeight = 144;
}

CHtmlgetIndexAction::~CHtmlgetIndexAction(void)
{
}

long CHtmlgetIndexAction::set_display(string display)
{
	if (display == "square")
	{
		_maxWidth = 120;
		_maxHeight = 120;
	}
	else if (display == "2small")
	{
		_maxWidth = 240;
		_maxHeight = 240;
	}
	else if (display == "xsmall")
	{
		_maxWidth = 432;
		_maxHeight = 324;
	}
	else if (display == "small")
	{
		_maxWidth = 576;
		_maxHeight = 432;
	}
	else if (display == "medium")
	{
		_maxWidth = 792;
		_maxHeight = 594;
	}
	else if (display == "large")
	{
		_maxWidth = 1008;
		_maxHeight = 756;
	}
	else //"thumb"
	{
		_maxWidth = 144;
		_maxHeight = 144;
	}
	return 0;
}

string CHtmlgetIndexAction::get_categoryActions(int ty, list<CATEGORY_DTO> lst, uint32_t nb_images, const QUERY_REQ req, string& switchBoxSizeUrl, string& switchBoxSortUrl)
{
	string clickurl = get_cation_url(req);

	string cal = "";
	lookitem(req.args, "cal=", cal);

	string display = CDataSrv::Instance()._display_thumb;

	string categoryActions = "";
	if (ty == 1)
	{
		categoryActions = "<ul class=\"categoryActions actions\">";
	}

	if (req.tags == "all")
	{
		string categoryActions_cmdEditTags = "";
		CLang::Instance().getOneData("categoryActions_cmdEditTags", categoryActions_cmdEditTags);

		string display_mode = "";
		lookitem(req.args, "display_mode=", display_mode);

		string categoryActions_tag = "";
		if (display_mode == "letters")
		{
			CLang::Instance().getOneData("categoryActions_tag_letters", categoryActions_tag);
		}
		else
		{
			CLang::Instance().getOneData("categoryActions_tag", categoryActions_tag);
		}
		categoryActions += categoryActions_tag;
		categoryActions += categoryActions_cmdEditTags;
		categoryActions += "</ul>";

		return categoryActions;
	}

	string categoryActions_search = "";
	CLang::Instance().getOneData("categoryActions_search", categoryActions_search);

	string categoryActions_face = "";
	CLang::Instance().getOneData("categoryActions_face", categoryActions_face);

	string categoryActions_star = "";
	CLang::Instance().getOneData("categoryActions_star", categoryActions_star);

	if (CDataSrv::Instance()._star == "1")
	{
		ReplaceOne(categoryActions_star, "pn-icon-star", "pn-icon-star-empty");
	}

	if (req.args == "")
	{
		ReplaceOne(categoryActions_search, "{$cat_id}", "0");
	}
	else
	{
		ReplaceOne(categoryActions_search, "{$cat_id}", req.cat_id);
	}

	if (nb_images == 0 && req.search == "q")
	{
		categoryActions += categoryActions_search;
		//if (CDataSrv::Instance()._albumName == "public")
		{
			categoryActions += categoryActions_face;
		}
		categoryActions += "</ul>";

		return categoryActions;
	}


	string categoryActions_sortOrderLink = "";
	{
		string args = "";
		if (req.favorites != "")
		{
			args = "favorites=1";
		}
		else if (req.caddie != "")
		{
			args = "caddie=1";
		}
		else if (req.recent_pics != "")
		{
			args = "recent_pics=1";
		}
		else if (req.no_tag != "")
		{
			args = "no_tag=1";
		}
		else if (req.longitude != "")
		{
			args = "longitude=" + req.longitude + "&latitude=" + req.latitude;
		}
		else if (req.list0 != "")
		{
			args = "list=" + req.list0;
		}
		else if (req.cat_id != "")
		{
			args = "category=" + req.cat_id;
		}
		else if (req.tags != "")
		{
			args = "tags=" + req.tags;
		}
		else if (req.q != "")
		{
			args = "q=" + req.q;
		}
		else if (req.search == "q")
		{
			args = "search=q";
		}
		else if (req.face != "")
		{
			args = "face=" + req.face;
		}

		if (args == "")
		{
			args = "1";
		}

		if (req.posted != "")
		{
			args += "&posted=" + req.posted;
		}

		string sortOrderBox = "";
		if (req.posted_list == "" && req.created_list == "" && req.most_visited == "")
		{
			switchBoxSortUrl = "./index.html?" + args;
			sortOrderBox = get_sortOrderBox(lst, req, args);
			CLang::Instance().getOneData("categoryActions_sortOrderLink", categoryActions_sortOrderLink);
		}

		string settings = "";
		if (req.cat_id != "")
		{
			CLang::Instance().getOneData("order_settings", settings);
			ReplaceOne(settings, "{$id}", req.cat_id);
		}
		ReplaceOne(categoryActions_sortOrderLink, "settings", settings);
		ReplaceOne(categoryActions_sortOrderLink, "{$sortOrderBox}", sortOrderBox);
	}

	if (req.recent_cats != "")
	{
		clickurl += "&recent_cats=1";
	}

	string url0 = clickurl;
	if (req.posted_list != "")
	{
		url0 = clickurl + "&posted_list=1";
	}
	else if (req.created_list != "")
	{
		url0 = clickurl + "&created_list=1";
	}

	switchBoxSizeUrl = url0;

	string categoryActions_derivativeSwitchLink = "";
	CLang::Instance().getOneData("categoryActions_derivativeSwitchLink", categoryActions_derivativeSwitchLink);
	string derivativeSwitchBox = get_derivativeSwitchBox(display);

	ReplaceOne(categoryActions_derivativeSwitchLink, "{$derivativeSwitchBox}", derivativeSwitchBox);

	string action = "";
	lookitem(req.args, "action=", action);

	string categoryActions_cmdEditAlbum = "";
	CLang::Instance().getOneData("categoryActions_cmdEditAlbum", categoryActions_cmdEditAlbum);
	ReplaceOne(categoryActions_cmdEditAlbum, "{$U_EDIT}", "./admin.html?page=admin.album&properties=1&category=" + req.cat_id);

	string categoryActions_cmdMap = "";
	CLang::Instance().getOneData("categoryActions_cmdMap", categoryActions_cmdMap);
	ReplaceOne(categoryActions_cmdMap, "{$U_MAP}", clickurl + "&osm=1&net=1");

	string categoryActions_cmdModReturn = "";
	CLang::Instance().getOneData("categoryActions_cmdModReturn", categoryActions_cmdModReturn);
	ReplaceOne(categoryActions_cmdModReturn, "{$U_MODE_RETURN}", clickurl);

	string categoryActions_cmdModPosted = "";
	CLang::Instance().getOneData("categoryActions_cmdModPosted", categoryActions_cmdModPosted);

	string url = clickurl + "&posted_list=1";
	if (cal != "")
	{
		url += "&cal=" + cal;
	}
	else
	{
		url += "&cal=ylist";
	}
	ReplaceOne(categoryActions_cmdModPosted, "{$U_MODE_POSTED}", url);

	string categoryActions_cmdModCreated = "";
	CLang::Instance().getOneData("categoryActions_cmdModCreated", categoryActions_cmdModCreated);
	url = clickurl + "&created_list=1";
	if (cal != "")
	{
		url += "&cal=" + cal;
	}
	else
	{
		url += "&cal=ylist";
	}
	ReplaceOne(categoryActions_cmdModCreated, "{$U_MODE_CREATED}", url);

	bool b = false;
	bool b1 = false;

	if (req.favorites != "" || req.caddie != "" || req.recent_pics != "" || req.no_tag != ""
		|| req.list0 != "" || req.cat_id != "" || req.longitude != ""
		|| req.posted_list != "" || req.created_list != "" || req.most_visited != "" || req.tags != ""
		|| req.q != "" || req.search == "q")
	{
		if (cal == "weekly")
		{

		}
		else if (cal != "calendar")
		{
			if (req.list0 != "" || req.most_visited != "")
			{
			}
			else
			{
			}
		}

		if (req.posted_list == "" && req.created_list == "")
		{
			b1 = true;
			categoryActions += categoryActions_search;
			if (req.most_visited != "")
			{

			}
			else
			{
				categoryActions += categoryActions_sortOrderLink;
			}
			categoryActions += categoryActions_derivativeSwitchLink;

		}
	}

	if (!b1)
	{
		categoryActions += categoryActions_search;
	}

	if (req.favorites != "")
	{
		string categoryActions_cmdFavorite_empty = "";
		CLang::Instance().getOneData("categoryActions_cmdFavorite_empty", categoryActions_cmdFavorite_empty);

		categoryActions += categoryActions_cmdFavorite_empty;
	}
	else if (req.caddie != "")
	{
		string categoryActions_cmdCaddie_empty = "";
		CLang::Instance().getOneData("categoryActions_cmdCaddie_empty", categoryActions_cmdCaddie_empty);

		categoryActions += categoryActions_cmdCaddie_empty;
	}
	if (req.favorites != "" || req.most_visited != "")
	{
	}

	if (req.cat_id != "")
	{
		if (req.posted_list == "" && req.created_list == "")
		{
			categoryActions += categoryActions_cmdEditAlbum;
		}
	}

	if (req.list0 != "" || req.most_visited != "")
	{
	}
	else
	{
		if (req.posted_list != "")
		{
			categoryActions += categoryActions_cmdModReturn;
			categoryActions += categoryActions_cmdModCreated;
		}
		else if (req.created_list != "")
		{
			categoryActions += categoryActions_cmdModReturn;
			categoryActions += categoryActions_cmdModPosted;
		}
		else
		{
			categoryActions += categoryActions_cmdModPosted;
			categoryActions += categoryActions_cmdModCreated;
		}
	}

	if (nb_images != 0)
	{
		categoryActions += categoryActions_star;
	}

	categoryActions += categoryActions_cmdMap;
	//if (CDataSrv::Instance()._albumName == "public")
	{
		categoryActions += categoryActions_face;
	}

	if (ty == 1)
	{
		categoryActions += "</ul>";
	}

	return categoryActions;
}

string CHtmlgetIndexAction::get_derivativeSwitchBox(string  display)
{
	string derivativeSwitchBox = "";

	string derivativeSwitchBox_Thumbnails = "";
	CLang::Instance().getOneData("derivativeSwitchBox_Thumbnails", derivativeSwitchBox_Thumbnails);

	string derivativeSwitchBox_XXS = "";
	CLang::Instance().getOneData("derivativeSwitchBox_XXS", derivativeSwitchBox_XXS);

	string derivativeSwitchBox_xsmall = "";
	CLang::Instance().getOneData("derivativeSwitchBox_xsmall", derivativeSwitchBox_xsmall);

	string derivativeSwitchBox_small = "";
	CLang::Instance().getOneData("derivativeSwitchBox_small", derivativeSwitchBox_small);

	string derivativeSwitchBox_medium = "";
	CLang::Instance().getOneData("derivativeSwitchBox_medium", derivativeSwitchBox_medium);

	string derivativeSwitchBox_large = "";
	CLang::Instance().getOneData("derivativeSwitchBox_large", derivativeSwitchBox_large);

	string derivativeSwitchBox_li_span = "";
	CLang::Instance().getOneData("derivativeSwitchBox_li_span", derivativeSwitchBox_li_span);

	if (display == "2small")
	{
		_maxWidth = 240;
		_maxHeight = 240;
		ReplaceOne(derivativeSwitchBox_XXS, "hidden", "visible");
		ReplaceOne(derivativeSwitchBox_XXS, "switchbox0", "switchbox1");
	}
	else if (display == "xsmall")
	{
		_maxWidth = 432;
		_maxHeight = 324;
		ReplaceOne(derivativeSwitchBox_xsmall, "hidden", "visible");
		ReplaceOne(derivativeSwitchBox_xsmall, "switchbox0", "switchbox1");
	}
	else if (display == "small")
	{
		_maxWidth = 576;
		_maxHeight = 432;
		ReplaceOne(derivativeSwitchBox_small, "hidden", "visible");
		ReplaceOne(derivativeSwitchBox_small, "switchbox0", "switchbox1");
	}
	else if (display == "medium")
	{
		_maxWidth = 792;
		_maxHeight = 594;
		ReplaceOne(derivativeSwitchBox_medium, "hidden", "visible");
		ReplaceOne(derivativeSwitchBox_medium, "switchbox0", "switchbox1");
	}
	else if (display == "large")
	{
		_maxWidth = 1008;
		_maxHeight = 756;
		ReplaceOne(derivativeSwitchBox_large, "hidden", "visible");
		ReplaceOne(derivativeSwitchBox_large, "switchbox0", "switchbox1");
	}
	else //"thumb"
	{
		_maxWidth = 144;
		_maxHeight = 144;
		ReplaceOne(derivativeSwitchBox_Thumbnails, "hidden", "visible");
		ReplaceOne(derivativeSwitchBox_Thumbnails, "switchbox0", "switchbox1");
	}

	derivativeSwitchBox += derivativeSwitchBox_Thumbnails;
	derivativeSwitchBox += derivativeSwitchBox_XXS;
	derivativeSwitchBox += derivativeSwitchBox_xsmall;
	derivativeSwitchBox += derivativeSwitchBox_small;
	derivativeSwitchBox += derivativeSwitchBox_medium;
	derivativeSwitchBox += derivativeSwitchBox_large;

	return derivativeSwitchBox;
}


string CHtmlgetIndexAction::get_sortOrderBox(list<CATEGORY_DTO> lst, QUERY_REQ req, string args)
{
	string sortOrderBox = "";

	string id = "";
	lookitem(args, "category=", id);

	list<ORDER_DTO> lstO = CDataSrv::Instance()._lstOrder;
	list<ORDER_DTO>::iterator it;

	bool b = false;
	if (id != "")
	{
		string order = "";
		list<CATEGORY_DTO>::iterator iter;
		for (iter = lst.begin(); iter != lst.end(); iter++)
		{
			if (iter->id == id)
			{
				order = iter->imageorder;
				break;
			}
		}

		if (order != "")
		{
			string sortOrderBox_Default = "";
			if (order == "b.rank asc")
			{
				CLang::Instance().getOneData("Manual_Default", sortOrderBox_Default);
			}
			else
			{
				string temp = "";
				for (it = lstO.begin(); it != lstO.end(); it++)
				{
					if (it->order == order)
					{
						temp = it->title;
						break;
					}
				}
				Replace(temp, "h2", "span");
				Replace(temp, "calendarTitle", "");
				CLang::Instance().getOneData("auto_Default", sortOrderBox_Default);
				ReplaceOne(sortOrderBox_Default, "sortOrder", temp);
			}
			ReplaceOne(sortOrderBox_Default, "{$order}", "");

			if (req.image_order == "" || req.image_order == "0")
			{
				b = true;
				ReplaceOne(sortOrderBox_Default, "hidden", "visible");
				ReplaceOne(sortOrderBox_Default, "#fff", "#6BA1FF");
			}
			sortOrderBox += sortOrderBox_Default;
		}
	}

	list<string> lstOrder;
	string sortOrderBox_AZ = "";
	CLang::Instance().getOneData("sortOrderBox_AZ", sortOrderBox_AZ);
	lstOrder.push_back(sortOrderBox_AZ);

	string sortOrderBox_ZA = "";
	CLang::Instance().getOneData("sortOrderBox_ZA", sortOrderBox_ZA);
	lstOrder.push_back(sortOrderBox_ZA);

	string sortOrderBox_AZ_1 = "";
	CLang::Instance().getOneData("sortOrderBox_AZ_1", sortOrderBox_AZ_1);
	lstOrder.push_back(sortOrderBox_AZ_1);

	string sortOrderBox_ZA_1 = "";
	CLang::Instance().getOneData("sortOrderBox_ZA_1", sortOrderBox_ZA_1);
	lstOrder.push_back(sortOrderBox_ZA_1);

	string sortOrderBox_NO = "";
	CLang::Instance().getOneData("sortOrderBox_NO", sortOrderBox_NO);
	lstOrder.push_back(sortOrderBox_NO);

	string sortOrderBox_ON = "";
	CLang::Instance().getOneData("sortOrderBox_ON", sortOrderBox_ON);
	lstOrder.push_back(sortOrderBox_ON);

	string sortOrderBox_NO_1 = "";
	CLang::Instance().getOneData("sortOrderBox_NO_1", sortOrderBox_NO_1);
	lstOrder.push_back(sortOrderBox_NO_1);

	string sortOrderBox_ON_1 = "";
	CLang::Instance().getOneData("sortOrderBox_ON_1", sortOrderBox_ON_1);
	lstOrder.push_back(sortOrderBox_ON_1);

	string sortOrderBox_HL = "";
	CLang::Instance().getOneData("sortOrderBox_HL", sortOrderBox_HL);
	lstOrder.push_back(sortOrderBox_HL);

	string sortOrderBox_LH = "";
	CLang::Instance().getOneData("sortOrderBox_LH", sortOrderBox_LH);
	lstOrder.push_back(sortOrderBox_LH);

	uint32_t od = 0;
	if (!b)
	{
		od = atoi(req.image_order.c_str());
		if (od == 0)
		{
			od = 5;
			if (req.recent_pics != "")
			{
				od = 7;
			}
			else if (req.search == "q")
			{
				if (CDataSrv::Instance()._search_req.date_type == "date_available")
				{
					od = 7;
				}
				else if (CDataSrv::Instance()._search_req.date_type == "date_creation")
				{
					od = 5;
				}
			}
			else
			{
				for (it = lstO.begin(); it != lstO.end(); it++)
				{
					if (it->order == CDataSrv::Instance()._orderby)
					{
						od = atoi(it->id.c_str());
						break;
					}
				}
			}
		}
	}

	uint32_t i = 0;
	list<string>::iterator iter;
	for (iter = lstOrder.begin(); iter != lstOrder.end(); iter++)
	{
		string temp = *iter;

		if (od == i + 1)
		{
			ReplaceOne(temp, "hidden", "visible");
			ReplaceOne(temp, "switchbox0", "switchbox1");
		}

		ReplaceOne(temp, "{$order}", "image_order=" + uint2str(i + 1));
		sortOrderBox += temp;
		i++;
	}

	return sortOrderBox;
}

string CHtmlgetIndexAction::get_thumbnailCategories(string type, string cat_id, list<CATEGORY_DTO> lstCat)
{
	list<CATEGORY_DTO> lstSub;
	if (cat_id != "")
	{
		if (type == "recent_cats")
		{
			vector<string> v1 = tokenize(cat_id, ",", true, "");
			int len = (int)v1.size();
			for (int i = 0; i < len; i++)
			{
				list<CATEGORY_DTO>::iterator iter;
				for (iter = lstCat.begin(); iter != lstCat.end(); iter++)
				{
					if (iter->id == v1[i])
					{
						lstSub.push_back(*iter);
					}
				}
			}
		}
		else
		{
			CHtmlgetMenubar::Instance().get_sub_categories_siblings(cat_id, lstCat, lstSub);
		}
	}
	else
	{
		list<CATEGORY_DTO>::iterator I;
		for (I = lstCat.begin(); I != lstCat.end(); I++)
		{
			if (I->id == I->full_id)
			{
				lstSub.push_back(*I);
			}
		}
	}

	string tempCategories = "";
	{
		string thumbnailCategories_li = "";
		CLang::Instance().getOneData("thumbnailCategories_li", thumbnailCategories_li);

		list<CATEGORY_DTO>::iterator iter;
		list<IMAGE_DTO > lstImage;
		Cimagedal::Instance().get_images_by_rnd(lstSub, lstImage);

		list<IMAGE_DTO >::iterator it2;
		for (iter = lstSub.begin(); iter != lstSub.end(); iter++)
		{
			int nb = 0;
			string url1 = "";
			string url2 = "";
			string mk1 = "";
			string mk2 = "";

			IMAGE_DTO curr;
			nb = (int)iter->nb_images;
			if (nb > 0)
			{
				//3
				for (it2 = lstImage.begin(); it2 != lstImage.end(); )
				{
					if (it2->storage_category_id == iter->id)
					{
						if (it2->id == iter->representative_id)
						{
							curr = *it2;
						}
						else
						{
							if (url1 == "")
							{
								url1 = CDataSrv::Instance().get_image_url(0, *it2);
								mk1 = uint64str(it2->lastmodified);
							}
							else if (url2 == "")
							{
								url2 = CDataSrv::Instance().get_image_url(0, *it2);
								mk2 = uint64str(it2->lastmodified);
							}
						}
						it2 = lstImage.erase(it2);
					}
					else
					{
						it2++;

					}
				}
			}


			long height = curr.height;

			string url = CDataSrv::Instance().get_image_url(0, curr);
			string mk = uint64str(curr.lastmodified);
			string templ = thumbnailCategories_li;

			ReplaceOne(templ, "{$id}", iter->id);

			string image_stack = "";
			{
				if (curr.id == "")
				{
					CLang::Instance().getOneData("single_photo_0", image_stack);
				}
				else if (url2 != "")
				{
					CLang::Instance().getOneData("image_stack", image_stack);
				}
				else
				{
					CLang::Instance().getOneData("single_photo", image_stack);
				}
				//1
				string photo = "";
				{
					if (nb == 1)
					{
						CLang::Instance().getOneData("str_photo", photo);
					}
					else if (nb > 1)
					{
						CLang::Instance().getOneData("str_photos", photo);
					}
					translate(photo, uint2str(iter->nb_images), "");
				}
				ReplaceOne(image_stack, "{$photo}", photo);


				//2
				string fullname = iter->name;
				if (type == "recent_cats")
				{
					get_fullname(iter->full_id, lstCat, fullname);
				}

				Replace(image_stack, "{$name}", fullname);

				if (url2 != "")
				{
					ReplaceOne(image_stack, "{$png}", url2);
					ReplaceOne(image_stack, "{$png1}", url1);
					ReplaceOne(image_stack, "{$png2}", url);
				}
				else
				{
					ReplaceOne(image_stack, "{$png}", url);
				}
				translate(image_stack, "", "");

				//6
				string subalbums = "";
				{
					if (iter->nb_categoires == 1 && iter->sub_images > 0)
					{
						CLang::Instance().getOneData("str_sub_album", subalbums);
					}
					else if (iter->nb_categoires > 1 && iter->sub_images > 0)
					{
						CLang::Instance().getOneData("str_sub_albums", subalbums);
					}

					if (subalbums != "")
					{
						translate(subalbums, "", "");
						ReplaceOne(subalbums, "%d", uint2str(iter->nb_categoires));

						string strphoto = get_photo_string(iter->sub_images);
						subalbums = strphoto + subalbums;
						subalbums = "<br/>" + subalbums;
					}
				}
				Replace(image_stack, "{$sub-albums}", subalbums);
			}

			ReplaceOne(templ, "{$photoinfo}", image_stack);

			tempCategories += templ;
		}
	}

	string thumbnailCategories = "";
	if (tempCategories != "")
	{
		CLang::Instance().getOneData("thumbnailCategories", thumbnailCategories);
		ReplaceOne(thumbnailCategories, "{$tempCategories}", tempCategories);
	}
	return thumbnailCategories;
}

string CHtmlgetIndexAction::get_thumbnails(QUERY_REQ req, list<IMAGE_DTO> lstImageDto, ORDER_DTO orderDto, list<CATEGORY_DTO> lst)
{
	string thumbnails_li = "";

	string mtitle = "";

	CLang::Instance().getOneData("thumbnails_li_3_title", mtitle);
	translate(mtitle, "", "");
	Replace(mtitle, "\"", "&quot;");
	//Replace(thumbnails_li_3_title, ">", "&gt;");
	//Replace(thumbnails_li_3_title, "<", "&lt;");

	string thumbnails_li_1 = "";
	CLang::Instance().getOneData("thumbnails_li_1", thumbnails_li_1);

	string thumbnails_li_1_paint = "";
	CLang::Instance().getOneData("thumbnails_li_1_paint", thumbnails_li_1_paint);
	ReplaceOne(thumbnails_li_1_paint, "thumbnails_li_1_title", mtitle);

	ReplaceOne(thumbnails_li_1, "{$thumbnails_li_1_paint}", thumbnails_li_1_paint);

	string thumbnails_li_1_video = "";
	CLang::Instance().getOneData("thumbnails_li_1_video", thumbnails_li_1_video);
	string thumbnails_li_gif = thumbnails_li_1_video;

	string thumbnails_li_2 = "";
	CLang::Instance().getOneData("thumbnails_li_2", thumbnails_li_2);

	string clickurl = get_picurl(req);

	string all = "";
	lookitem(req.args, "all=", all);

	string arg = "";
	if (all != "")
	{
		arg += "&all=1";
	}

	if (req.posted != "")
	{
		arg += "&posted=" + req.posted;
	}

	clickurl += arg;

	time_t t = 0;
	if (req.page_nb == "0" || req.page_nb == "")
	{
		CDataSrv::Instance()._t_masrony_title = 0;
	}

	t = CDataSrv::Instance()._t_masrony_title;

	int idx = 0;
	list<IMAGE_DTO>::iterator iter;
	for (iter = lstImageDto.begin(); iter != lstImageDto.end(); iter++)
	{
		idx++;
		string tm = "";

		if (orderDto.type == "date_creation")
		{
			if (t != iter->date_creation_day)
			{
				t = iter->date_creation_day;
				tm = get_time_string(iter->date_creation, "1");
				translate(tm, "", "");
			}
		}
		else if (orderDto.type == "date_available")
		{
			if (t != iter->date_available_day)
			{
				t = iter->date_available_day;
				tm = get_time_string(iter->date_available, "1");
				translate(tm, "", "");
			}
		}

		if (tm != "")
		{
			string temp2 = thumbnails_li_2;
			ReplaceOne(temp2, "{$DATE}", tm);
			thumbnails_li += temp2;
		}

		string imgvisits = "";
		if (iter->date_available > 0)
		{
			uint32_t q = (iter->iscoi == 1) ? iter->coi_quality : iter->quality;
			if (q > 0)
			{
				imgvisits = "({'Quality'|@translate}: " + float2Str(static_cast<float>(q) / 100.0f, "%.1f") + ")";
				translate(imgvisits, "", "");
			}
		}

		uint32_t width = (iter->iscoi == 1) ? iter->coi_w : iter->width;
		uint32_t height = (iter->iscoi == 1) ? iter->coi_h : iter->height;

		bool b = false;
		if (iter->ext == "mp3" || iter->ext == "ogg")
		{
			if (iter->representative_ext == "")
			{
				b = true;
			}
		}
		else if (iter->ext == "mp4" || iter->ext == "webm")
		{
			if (iter->representative_ext != "")
			{
				b = true;
			}
		}

		char buf[MAX_PATH] = { 0 };
		if (b)
		{
			sprintf_s(buf, ", &middot; %s", iter->ext.c_str());
		}
		else
		{
			sprintf_s(buf, ", %d&times;%d &middot; %s", width, height, iter->ext.c_str());
		}

		string tm0 = get_time_string(iter->date_creation, "0");
		string imgtitle = tm0 + buf;

		uint32_t outW = 0;
		uint32_t outH = 0;
		get_flex_size(*iter, _maxWidth, _maxHeight, outW, outH);

		string url = CDataSrv::Instance().get_image_url(0, *iter);
		string temp = thumbnails_li_1;

		if (iter->ext == "mp4" || iter->ext == "mp3" || iter->ext == "webm" || iter->ext == "ogg")
		{
			temp = thumbnails_li_1_video;
			ReplaceOne(temp, "{$duration}", get_dur_string(iter->duration));
		}
		else if (iter->ext == "gif")
		{
			temp = thumbnails_li_gif;
			ReplaceOne(temp, "{$duration}", "");
		}
		else
		{
			string v = "";
			uint32_t q = (iter->iscoi == 1) ? iter->coi_quality : iter->quality;
			if (q > 8000)
			{
				v = float2Str(static_cast<float>(q) / 100.0f, "%.1f");

				if (CDataSrv::Instance()._star == "1")
				{
					ReplaceOne(temp, "nostar", "");
				}
			}
			Replace(temp, "{$quality}", v);

			string angle = get_angle(iter->rotation);
			ReplaceOne(temp, "0deg", angle + "deg");
		}

		ReplaceOne(temp, "{$clickurl}", clickurl + "&id=" + iter->id);
		Replace(temp, "{$image_id}", iter->id);
		ReplaceOne(temp, "{$imgurl}", url);

		string catname = iter->storage_category_id;

		list<CATEGORY_DTO>::iterator iter2;
		for (iter2 = lst.begin(); iter2 != lst.end(); iter2++)
		{
			if (iter2->id == iter->storage_category_id)
			{
				catname = iter2->name;
				break;
			}
		}
		ReplaceOne(temp, "{$cat_id}", iter->storage_category_id);
		ReplaceOne(temp, "{$cat_name}", catname);

		ReplaceOne(temp, "{$imgvisits}", imgvisits);

		ReplaceOne(temp, "{$width}", uint2str(outW));
		ReplaceOne(temp, "{$height}", uint2str(outH));
		Replace(temp, "{$imgtitle}", imgtitle);
		ReplaceOne(temp, "{$imgtitle1}", iter->name + "." + iter->ext);
		ReplaceOne(temp, "{$imgtitle2}", iter->name);
		thumbnails_li += temp;
	}

	CDataSrv::Instance()._t_masrony_title = t;

	if (req.page_nb != "0")
	{
		return thumbnails_li;
	}

	string thumbnails = "";
	if (thumbnails_li != "")
	{
		CLang::Instance().getOneData("thumbnails", thumbnails);
		ReplaceOne(thumbnails, "{$thumbnails_li}", thumbnails_li);
	}

	return thumbnails;
}

uint32_t CHtmlgetIndexAction::get_maxHeight()
{
	return _maxHeight;
}

uint32_t CHtmlgetIndexAction::get_maxWidth()
{
	return _maxWidth;
}

string CHtmlgetIndexAction::get_cation_url(const QUERY_REQ req)
{
	string clickurl = "";
	if (req.favorites != "")
	{
		clickurl = "./index.html?favorites=1";
	}
	else if (req.caddie != "")
	{
		clickurl = "./index.html?caddie=1";
	}
	else if (req.most_visited != "")
	{
		clickurl = "./index.html?most_visited=1";
	}
	else if (req.recent_pics != "")
	{
		clickurl = "./index.html?recent_pics=1";
	}
	else if (req.no_tag != "")
	{
		clickurl = "./index.html?no_tag=1";
	}
	else if (req.longitude != "")
	{
		clickurl = "./index.html?longitude=" + req.longitude + "&latitude=" + req.latitude;
	}
	else if (req.list0 != "")
	{
		clickurl = "./index.html?list=" + req.list0;
	}
	else if (req.tags != "")
	{
		clickurl = "./index.html?tags=" + req.tags;
	}
	else if (req.q != "")
	{
		clickurl = "./index.html?q=" + req.q;
	}
	else if (req.search != "")
	{
		clickurl = "./index.html?search=" + req.search;
	}
	else if (req.face != "")
	{
		clickurl = "./index.html?face=" + req.face;
	}

	if (req.cat_id != "")
	{
		if (clickurl == "")
		{
			clickurl = "./index.html?category=" + req.cat_id;
		}
		else
		{
			clickurl += "&category=" + req.cat_id;
		}
	}

	if (clickurl == "")
	{
		clickurl = "./index.html?home=1";
	}
	return clickurl;
}

long CHtmlgetIndexAction::get_fullname(string full_id, list<CATEGORY_DTO> lstCat, string& fullname)
{
	list<CATEGORY_DTO>::iterator it;

	fullname = "";
	vector<string> v1 = tokenize(full_id, ",", true, "");
	int len = (int)v1.size();
	for (int i = 0; i < len; i++)
	{
		for (it = lstCat.begin(); it != lstCat.end(); it++)
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

