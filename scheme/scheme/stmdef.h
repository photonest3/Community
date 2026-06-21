#pragma once

#include <string>
#include <list>
#include <vector>
#include <map>

using namespace std;

#define PERPAGE_NUM						30
#define PERPAGE_TOTAL					300
#define LIMIT_NB_PHOTOS					100000

#define EXP_SPAN 24 * 60 * 60

typedef map<string, UINT, less<string> > REQ_URL_MAP;

typedef struct _REQUEST_DTO
{
	string szUrl = "";
	string szArgs = "";
	string szBody = "";
	wstring szFile = L"";
	string host = "";
} REQUEST_DTO, *PREQUEST_DTO;

typedef struct _RESPONSE_DTO
{
	string szResponse = "";
	string szContentType = "";
} RESPONSE_DTO, *PRESPONSE_DTO;

typedef struct _TEMPL_DTO
{
	string id = "";
	string val = "";
} TEMPL_DTO;

typedef struct _DE_HEADER
{
	unsigned char ot1;
	unsigned char crc;
	unsigned char offset;
	unsigned char type;
	unsigned short ot2;
	unsigned short ot3;

} DE_HEADER, *PDE_HEADER;

typedef struct _CONFIG_DTO
{
	string param = "";
	string value = "";
	string comment = "";
} CONFIG_DTO;

typedef struct _CATEGORY_DTO
{
	string id = "";
	string name = "";
	string comment = "";
	string dir0 = "";
	uint32_t rank = 0;
	uint32_t visible = 1;
	string representative_id = "";
	string full_id = "";
	string imageorder = "";
	uint64_t lastmodified = 0;
	uint32_t nb_images = 0;
	uint32_t sub_images = 0;
	uint32_t nb_categoires = 0;
	string root_id = "";
	string root_dir0 = "";
	uint32_t addby = 0;
} CATEGORY_DTO;

typedef struct _CATEGORY_DTO2
{
	string id = "";
	wstring dir0 = L"";
	wstring root_id = L"";
	wstring root_dir0 = L"";
} CATEGORY_DTO2;

typedef struct _TAG_DTO
{
	string	id = "";
	string	name = "";
	uint64_t lastmodified = 0;
	uint32_t nb_images = 0;
} TAG_DTO;

typedef struct _ORDER_DTO
{
	string	id = "";
	string	order = "";
	string	title = "";
	string type = "";
} ORDER_DTO;


typedef struct _ROOT_DTO
{
	string	id = "";
	string	root_dir0 = "";
	uint64_t root_sizes = 0;
} ROOT_DTO;

typedef struct _ROOT_DTO2
{
	wstring	id = L"";
	wstring	root_dir0 = L"";
	uint64_t root_sizes = 0;
} ROOT_DTO2;

typedef struct _IMAGE_DTO
{
	string id = "";
	string file = "";
	uint64_t date_available = 0;
	uint64_t date_creation = 0;
	string name = "";
	string comment = "";
	string author = "";
	uint32_t hit = 0;
	uint32_t filesize = 0;
	uint32_t width = 0;
	uint32_t height = 0;
	string coi = "";
	string representative_ext = "";
	uint64_t date_available_day = 0;
	string ori = "";
	string storage_category_id = "";
	uint32_t quality = 0;
	uint64_t md5sum = 0;
	string added_by = "";
	uint32_t rotation = 0;
	float latitude = 0;
	float longitude = 0;
	uint64_t lastmodified = 0;
	string ext = "";
	uint32_t duration = 0;
	uint64_t date_creation_day = 0;
	float ratio = 0;
	uint32_t coi_w = 0;
	uint32_t coi_h = 0;
	uint32_t iscoi = 0;
	uint64_t coi_lastmodified = 0;
	uint32_t coi_quality = 0;
	uint32_t coi_filesize = 0;
	uint32_t orientation = 0;

} IMAGE_DTO;

typedef struct _FAVORITE_DTO
{
	string user_id = "";
	string image_id = "";
} FAVORITE_DTO;

typedef struct _FACE_DTO
{
	string id = "";
	string name = "";
	string image_id = "";
	string dict = "";
	string merge_id = "";
	string al = "";
	uint32_t nb = 0;
} FACE_DTO;

typedef struct _CADDIE_DTO
{
	string user_id = "";
	string element_id = "";
} CADDIE_DTO;

typedef struct _IMAGE_CATEGORY_DTO
{
	string image_id = "";
	string category_id = "";
	string category_name = "";
	string rank = "";
} IMAGE_CATEGORY_DTO;

typedef struct _IMAGE_TAG_DTO
{
	string image_id = "";
	string tag_id = "";
	string tag_name = "";
} IMAGE_TAG_DTO;

typedef struct _USER_DTO
{
	string id = "";
	string username = "";
	string password = "";
	string hint = "";
}USER_DTO;

typedef struct _QUERY_REQ
{
	string cat_id = "";
	string imageids = "";
	string page_nb = "";
	string favorites = "";
	string caddie = "";
	string most_visited = "";
	string recent_pics = "";
	string no_tag = "";
	string osm = "";
	string longitude = "";
	string latitude = "";
	string list0 = "";
	string posted_list = "";
	string created_list = "";
	string posted = "";
	string date_start = "";
	string date_end = "";
	string recent_cats = "";
	string tags = "";
	string q = "";
	string search = "";
	string start = "";
	string image_order = "";
	string szBody = "";
	string args = "";
	string home = "";
	string art_list = "";
	string face = "";
} QUERY_REQ;

typedef struct _SEARCH_REQ
{
	string search = "1";
	//batchform
	string filter_prefilter_use = "";
	string filter = "";

	//searchfrom
	string page = "";
	string allcats = "";
	string date_type = "date_creation";

	string filter_search_use = "";
	string search_allwords = "";
	string mode = "AND";
	string search_in_name = "";
	string search_in_comment = "";
	string search_in_file = "";
	string search_in_tags = "";


	string filter_category_use = "";
	string cat = "";
	string subcats = "on";

	string filter_filetypes_use = "";
	string search_images = "";
	string search_videos = "";

	string filter_date_use = "";
	string createdate_min = "";
	string createdate_max = "";
	string postdate_min = "";
	string postdate_max = "";

	string createdate_min_o = "";
	string createdate_max_o = "";
	string postdate_min_o = "";
	string postdate_max_o = "";

	string filter_dimension_use = "";
	string dimension_min_width = "";
	string dimension_max_width = "";
	string dimension_min_height = "";
	string dimension_max_height = "";
	string dimension_min_ratio = "";
	string dimension_max_ratio = "";
	string width_min_o = "";
	string width_max_o = "";
	string height_min_o = "";
	string height_max_o = "";
	string ratio_min_o = "";
	string ratio_max_o = "";
	string filesize_min = "";
	string filesize_max = "";
	string filesize_min_o = "";
	string filesize_max_o = "";

	string dimension_type = "widths";
	string filter_authors_use = "";
	string authors = "";

	string filter_osm_use = "";
	string osm0 = "";
	string osm1 = "";

	string filter_tags_use = "";
	string tags = "";
	string tag_mode = "AND";


}SEARCH_REQ;

typedef struct _SLIDER_VALUES
{
	list<uint32_t>  widths;
	list<uint32_t>  heights;
	list<float>  filesizes;
	list<uint64_t> createdates;
	list<uint64_t> postdates;
	list<float>  ratios;

}SLIDER_VALUES;

typedef struct _PHOTO_DTO
{
	string id = "";
	string name = "";
	string author = "";
	uint64_t date_creation = 0;
	string associate = "";
	string categories = "";
	string tags = "";
	string description = "";
	string coi = "";
	string ori = "";
	string file = "";
	string hit = "";
	uint64_t date_available = 0;
	string filesize = "";
	string width = "";
	string height = "";
	string ext = "";
	string rotation = "";
	string representative_ext = "";
	uint32_t duration = 0;
	string added_by = "";
	uint32_t coi_w = 0;
	uint32_t coi_h = 0;
	uint32_t iscoi = 0;
	uint32_t storage_category_id = 0;
	string storage_category = "";

}PHOTO_DTO;

typedef struct _DASHBOARD_DTO
{
	uint32_t nb_photos = 0;
	uint32_t nb_albums = 0;
	uint32_t nb_tags = 0;
	uint32_t nb_comments = 0;
	uint32_t nb_views = 0;
	uint32_t nb_tag_images = 0;
	uint64_t nb_first_add = 0;
}DASHBOARD_DTO;

typedef struct _CAL_DTO
{
	uint32_t year = 0;
	uint32_t month = 0;
	uint32_t day = 0;
	uint32_t nb_year_images = 0;
	uint32_t nb_month_images = 0;
	uint32_t nb_day_images = 0;
}CAL_DTO;

typedef struct _UPLOAD_DATA
{
	wstring krf = L"";
	uint32_t width = 0;
	uint32_t height = 0;
	wstring fullname = L"";
	uint32_t orientation = 0;
	wstring srcfile = L"";

} UPLOAD_DATA;

typedef struct _HISTORY_DTO
{
	string id = "";
	uint64_t lastmodified = 0;
	string user_id = "";

} HISTORY_DTO;

typedef struct _HISTORY_SUMMARY_DTO
{
	string year = "";
	string month = "";
	string day = "";
	string hour = "";
	uint32_t nb_pages = 0;

} HISTORY_SUMMARY_DTO;


typedef struct _LOG_VIEWER_DTO
{
	string id = "";
	string folder = "";
	string file = "";
	uint64_t lastmodified = 0;
} LOG_VIEWER_DTO;


typedef struct _FILE_DTO
{
	string cat_id = "";
	wstring filename = L"";
	wstring file = L"";
	DWORD dwSize = 0;
} FILE_DTO;

typedef struct _FOLDER_STATE_DTO
{
	string id = "";
	string uploaded = "";
	string pending = "";
	string state = "";
	string alb = "";
	string root_id = "";
	string path = "";

} FOLDER_STATE_DTO;

typedef struct _IDX_DTO
{
	int idx = 0;
	LPVOID pParam = 0;

} IDX_DTO;


typedef struct _DAY_IMAGES_DTO
{
	string image_id = "";
	uint64_t day2 = 0;
	uint32_t nb_images = 0;
} DAY_IMAGES_DTO;

typedef struct _WEEK_IMAGES_DTO
{
	string image_id = "";
	uint64_t day2 = 0;
	uint32_t nb_images = 0;
} WEEK_IMAGES_DTO;

typedef struct _FILM_DTO
{
	uint32_t id = 0;
	uint64_t md5sum = 0;
	string title1 = "";
	string title2 = "";
	string photos = "";
	uint64_t lastmodified = 0;
	string type = "";
	string music = "";
}FILM_DTO;

typedef struct _MD5_DTO
{
	string cat_id = "";
	string pic_id = "";
	uint64_t md5sum = 0;
	uint64_t lastmodified = 0;
	string path = "";
	string msg = "";

}MD5_DTO;

typedef struct _CAT_NB_DTO
{
	string id = "";
	string root_id = "";
	wstring dir0 = L"";
	uint32_t nb_db_files = 0;
	uint32_t nb_db_pics = 0;
	uint32_t nb_err_pics = 0;
	uint32_t nb_folder_files = 0;
	uint32_t nb_folder_pics = 0;
	int nb_changed = 0;
} CAT_NB_DTO;

typedef struct _TR_DTO
{
	string id = "";
	string tr = "";
} TR_DTO;

typedef map<string, list<MD5_DTO>> MAP_MD5;
