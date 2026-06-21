#include "StdAfx.h"

#include "unitil.h"
#include <vector>
#include <string>
#include "../../templ/templ/resource.h"
#include "lang.h"
#include "HtmlgetMenubar.h"
#include "chatdal.h"

CHtmlgetMenubar::CHtmlgetMenubar(void)
{
	InitializeCriticalSection(&_cs);
}

CHtmlgetMenubar::~CHtmlgetMenubar(void)
{
	DeleteCriticalSection(&_cs);
}

long CHtmlgetMenubar::get_categories_order_by_rank(int ty, list<CATEGORY_DTO>& lstCategory)
{
	EnterCriticalSection(&_cs);
	if (!lstCategory_order_by_rank_.empty())
	{
		lstCategory = lstCategory_order_by_rank_;
	}
	else
	{
		lstCategory_order_by_rank_.clear();
		Cchatdal::Instance().get_categories("", "", lstCategory_order_by_rank_);

		lstCategory = lstCategory_order_by_rank_;
	}
	LeaveCriticalSection(&_cs);
	return 0;
}

long CHtmlgetMenubar::get_categories_order_by_root_dir(int ty, list<CATEGORY_DTO>& lstCategory)
{
	EnterCriticalSection(&_cs);
	if (!lstCategory_order_by_root_dir_.empty())
	{
		lstCategory = lstCategory_order_by_root_dir_;
	}
	else
	{
		lstCategory_order_by_root_dir_.clear();
		Cchatdal::Instance().get_categories2("2", lstCategory_order_by_root_dir_);

		lstCategory = lstCategory_order_by_root_dir_;
	}
	LeaveCriticalSection(&_cs);
	return 0;
}

long CHtmlgetMenubar::get_order_categories(string order, list<CATEGORY_DTO>& lstCategory)
{
	if (order == "0")
	{
		Cchatdal::Instance().get_categories2("0", lstCategory);
	}
	else
	{
		Cchatdal::Instance().get_categories("", order, lstCategory);
	}
	return 0;
}

long CHtmlgetMenubar::get_categories(string catids, list<CATEGORY_DTO>& lstCategory)
{
	Cchatdal::Instance().get_categories(catids, "a.full_id", lstCategory);
	return 0;
}

long CHtmlgetMenubar::clear()
{
	EnterCriticalSection(&_cs);
	lstCategory_order_by_rank_.clear();
	lstCategory_order_by_root_dir_.clear();
	LeaveCriticalSection(&_cs);
	return 0;
}

long CHtmlgetMenubar::proc_categories(list<CATEGORY_DTO>* lstCategroy)
{
	list<CATEGORY_DTO>::iterator iter;
	list<CATEGORY_DTO>::iterator it;

	for (iter = lstCategroy->begin(); iter != lstCategroy->end(); iter++)
	{
		if (iter->visible == 1)
		{
			for (it = lstCategroy->begin(); it != lstCategroy->end(); it++)
			{
				if (it->visible == 1 && iter->id != it->id)
				{
					vector<string> v1 = tokenize(it->full_id, ",", true, "");
					int len = (int)v1.size();
					for (int i = 0; i < len; i++)
					{
						if (iter->id == v1[i])
						{
							iter->nb_categoires++;
							iter->sub_images += it->nb_images;
							break;
						}
					}
				}
			}
		}
	}

	return S_OK;
}

long CHtmlgetMenubar::get_sub_categories_siblings(string cat_id, list<CATEGORY_DTO> lstCat, list<CATEGORY_DTO>& lstSub)
{
	list<CATEGORY_DTO>::iterator it;
	list<string> ids;
	for (it = lstCat.begin(); it != lstCat.end(); it++)
	{
		if (it->visible == 1)
		{
			vector<string> v1 = tokenize(it->full_id, ",", true, "");
			int len = (int)v1.size();
			for (int i = 0; i < len; i++)
			{
				if (cat_id == v1[i])
				{
					if (i + 1 < len)
					{
						ids.push_back(v1[i + 1]);
					}
					break;
				}
			}
		}
	}

	list<string>::iterator z;
	for (it = lstCat.begin(); it != lstCat.end(); it++)
	{
		string temp = "," + it->id + ",";
		for (z = ids.begin(); z != ids.end(); z++)
		{
			if (it->id == *z)
			{
				lstSub.push_back(*it);
				break;
			}
		}
	}

	return S_OK;
}

string CHtmlgetMenubar::get_menubar_categories(string cat_id, uint32_t nb_cat_images, list<CATEGORY_DTO>* lstCategroy)
{
	string menubar_categories_li = "";

	list<CATEGORY_DTO> lstCat;
	list<CATEGORY_DTO>::iterator iter;
	for (iter = lstCategroy->begin(); iter != lstCategroy->end(); iter++)
	{
		if (iter->visible == 1)
		{
			lstCat.push_back(*iter);
		}
	}

	for (iter = lstCat.begin(); iter != lstCat.end(); iter++)
	{
		if (iter->id == iter->full_id)
		{
			menubar_categories_li += get_menubar_categories_li(cat_id, *iter, lstCat);
		}
	}
	string menubar_categories = "";
	CLang::Instance().getOneData("menubar_categories_ul", menubar_categories);

	string totalImages = "";
	if (nb_cat_images > 0)
	{
		CLang::Instance().getOneData("totalImages", totalImages);

		string photo = "";
		if (nb_cat_images == 1)
		{
			CLang::Instance().getOneData("str_photo", photo);
		}
		else if (nb_cat_images > 1)
		{
			CLang::Instance().getOneData("str_photos", photo);
		}
		ReplaceOne(totalImages, "{$photo}", photo);
		translate(totalImages, uint2str(nb_cat_images), "");
	}
	ReplaceOne(menubar_categories, "{$totalImages}", totalImages);

	ReplaceOne(menubar_categories, "{$menubar_categories_li}", menubar_categories_li);
	return menubar_categories;
}

//private
string CHtmlgetMenubar::get_menubar_categories_li(string cat_id, CATEGORY_DTO categroyDto, list<CATEGORY_DTO> lstCat)
{
	//if (categroyDto.nb_images == 0 && categroyDto.sub_images == 0)
	//{
	//	return "";
	//}

	string menubar_categories_li = "";
	if (cat_id == categroyDto.id)
	{
		menubar_categories_li = "<li class=\"selected\">";
	}
	else
	{
		menubar_categories_li = "<li >";
	}

	menubar_categories_li += get_menubar_categories_li_content(categroyDto);

	string subAlbum = "";
	list<CATEGORY_DTO>::iterator iter;
	for (iter = lstCat.begin(); iter != lstCat.end(); iter++)
	{
		string uppercats = "," + iter->full_id + ",";
		string::size_type pos = uppercats.find("," + categroyDto.id + ",");
		if (pos != string::npos)
		{
			vector<string> v1 = tokenize(iter->full_id, ",", true, "");
			if (categroyDto.id == v1[0])
			{
				subAlbum = get_sel_categories(categroyDto.id, lstCat);
				break;
			}
		}
	}
	menubar_categories_li += subAlbum;
	menubar_categories_li += "</li>";

	return menubar_categories_li;
}

string CHtmlgetMenubar::get_menubar_categories_li_content(CATEGORY_DTO categroyDto)
{
	uint32_t sub = categroyDto.nb_categoires;
	string subalbums = "";
	{
		if (sub == 1)
		{
			CLang::Instance().getOneData("str_sub_album", subalbums);
		}
		else if (sub > 1)
		{
			CLang::Instance().getOneData("str_sub_albums", subalbums);
		}

		if (!subalbums.empty())
		{
			translate(subalbums, "", "");
			ReplaceOne(subalbums, "%d", uint2str(sub));

			if (categroyDto.sub_images > 0)
			{
				string strphoto = get_photo_string(categroyDto.sub_images);
				subalbums = strphoto + subalbums;
			}
		}
	}

	string menubar_categories_li_content = "";
	uint32_t nb = categroyDto.nb_images + categroyDto.sub_images;
	if (nb == 0)
	{
		CLang::Instance().getOneData("menubar_categories_li_content_span", menubar_categories_li_content);
	}
	else
	{
		CLang::Instance().getOneData("menubar_categories_li_content", menubar_categories_li_content);

		string photo_album = "";
		if (categroyDto.nb_images > 0)
		{
			if (categroyDto.nb_images == 1)
			{
				CLang::Instance().getOneData("photo_album", photo_album);
			}
			else if (categroyDto.nb_images > 1)
			{
				CLang::Instance().getOneData("photos_album", photo_album);
			}

			translate(photo_album, uint2str(categroyDto.nb_images), "");
		}
		Replace(menubar_categories_li_content, "{$photo_album}", photo_album);
		ReplaceOne(menubar_categories_li_content, "{$num}", uint2str(nb));

		if (subalbums != "")
		{
			subalbums = " / " + subalbums;
		}
		Replace(menubar_categories_li_content, " {$sub-albums}", subalbums);
		translate(menubar_categories_li_content, "", "");
	}

	ReplaceOne(menubar_categories_li_content, "{$name}", categroyDto.name);
	ReplaceOne(menubar_categories_li_content, "{$cat.URL}", "./index.html?category=" + categroyDto.id);

	return menubar_categories_li_content;
}

string CHtmlgetMenubar::get_sel_categories(string cat_id, list<CATEGORY_DTO> lstCat)
{
	list<CATEGORY_DTO>::iterator iter;
	list<CATEGORY_DTO> lstSub;
	get_sub_categories_siblings(cat_id, lstCat, lstSub);

	string ul_subalbum = "";
	string subAlbum = "";
	for (iter = lstSub.begin(); iter != lstSub.end(); iter++)
	{
		if (iter->nb_images == 0 && iter->sub_images == 0)
		{
			continue;
		}
		string temp = get_menubar_categories_li_content(*iter);
		if (temp != "")
		{
			subAlbum += "<li>";
			subAlbum += temp;
		}

		string subAlbum4 = get_sel_category_substring(iter->id, lstCat);
		subAlbum += subAlbum4;
		subAlbum += "</li>";
	}

	if (subAlbum != "")
	{
		CLang::Instance().getOneData("ul_subalbum", ul_subalbum);
		ReplaceOne(ul_subalbum, "{$subAlbum}", subAlbum);
	}

	return ul_subalbum;
}

string CHtmlgetMenubar::get_sel_category_substring(string cat_id, list<CATEGORY_DTO> lstCat)
{
	list<CATEGORY_DTO> lstSub3;
	list<CATEGORY_DTO>::iterator I;

	string ul_subalbum = "";
	string subAlbum3 = "";
	get_sub_categories_siblings(cat_id, lstCat, lstSub3);
	for (I = lstSub3.begin(); I != lstSub3.end(); I++)
	{
		string temp = get_menubar_categories_li_content(*I);
		if (temp != "")
		{
			subAlbum3 += "<li>";
			subAlbum3 += temp;
		}

		subAlbum3 += get_sel_category_substring(I->id, lstCat);
		subAlbum3 += "</li>";

	}
	if (subAlbum3 != "")
	{
		CLang::Instance().getOneData("ul_subalbum", ul_subalbum);
		ReplaceOne(ul_subalbum, "{$subAlbum}", subAlbum3);
	}

	return ul_subalbum;
}
