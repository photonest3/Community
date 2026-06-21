///////////////////////////////////////////////////////////////////////////////
// 文件名: stmdef.h
// 模块: scheme/scheme
// 功能描述: 数据结构定义文件
//           定义所有 DTO (Data Transfer Object) 和请求/响应结构体
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-03-10  PhotoNest Team  添加分页支持
//   2024-05-15  PhotoNest Team  添加搜索请求结构
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <list>
#include <vector>
#include <map>

using namespace std;

/// @brief 每页图片数量
/// @note 默认每页显示 30 张图片
#define PERPAGE_NUM							30

/// @brief 总图片数量上限
/// @note 免费版最多支持 300 张图片
#define PERPAGE_TOTAL						300

/// @brief 图片数量限制
/// @note 防止恶意上传，单用户最多 100,000 张图片
#define LIMIT_NB_PHOTOS						100000

/// @brief 过期时间（秒）
/// @note 链接有效期 24 小时（24 * 60 * 60）
#define EXP_SPAN 24 * 60 * 60

/// @brief 请求 URL 映射表
/// @note 映射 URL 到处理函数 ID
typedef map<string, UINT, less<string> > REQ_URL_MAP;

/// @brief HTTP 请求数据传输对象
/// @details 封装 HTTP 请求的所有信息，包括 URL、参数、请求体、文件名、主机名。
///          用于在不同层之间传递请求数据。
typedef struct _REQUEST_DTO
{
	/// @brief 请求 URL
	string szUrl = "";
	
	/// @brief 请求参数（URL 查询字符串）
	string szArgs = "";
	
	/// @brief POST 请求体
	string szBody = "";
	
	/// @brief 上传文件名（宽字符）
	wstring szFile = L"";
	
	/// @brief 主机名
	string host = "";
} REQUEST_DTO, *PREQUEST_DTO;

/// @brief HTTP 响应数据传输对象
/// @details 封装 HTTP 响应的所有信息，包括响应内容、内容类型。
///          用于在不同层之间传递响应数据。
typedef struct _RESPONSE_DTO
{
	/// @brief 响应内容
	string szResponse = "";
	
	/// @brief 内容类型（MIME Type）
	string szContentType = "";
} RESPONSE_DTO, *PRESPONSE_DTO;

/// @brief 模板数据传输对象
/// @details 用于缓存模板数据（ID 和值）。
typedef struct _TEMPL_DTO
{
	/// @brief 模板 ID
	string id = "";
	
	/// @brief 模板值
	string val = "";
} TEMPL_DTO;

/// @brief 文件头结构
/// @details 用于解析二进制文件格式（如加密文件、COI 文件等）。
typedef struct _DE_HEADER
{
	/// @brief 保留字段 1
	unsigned char ot1;
	
	/// @brief CRC 校验和
	unsigned char crc;
	
	/// @brief 偏移量
	unsigned char offset;
	
	/// @brief 类型
	unsigned char type;
	
	/// @brief 保留字段 2
	unsigned short ot2;
	
	/// @brief 保留字段 3
	unsigned short ot3;
	
} DE_HEADER, *PDE_HEADER;

/// @brief 配置数据传输对象
/// @details 用于存储配置项（参数、值、注释）。
typedef struct _CONFIG_DTO
{
	/// @brief 配置参数名
	string param = "";
	
	/// @brief 配置值
	string value = "";
	
	/// @brief 配置注释
	string comment = "";
} CONFIG_DTO;

/// @brief 分类数据传输对象
/// @details 存储分类的完整信息，包括名称、路径、排序、可见性、图片数量等。
typedef struct _CATEGORY_DTO
{
	/// @brief 分类 ID
	string id = "";
	
	/// @brief 分类名称
	string name = "";
	
	/// @brief 分类注释
	string comment = "";
	
	/// @brief 分类目录路径（原始）
	wstring dir0 = L"";
	
	/// @brief 排序等级（用于自定义排序）
	uint32_t rank = 0;
	
	/// @brief 可见性（0: 隐藏, 1: 显示）
	uint32_t visible = 1;
	
	/// @brief 代表图片 ID（用于分类封面）
	string representative_id = "";
	
	/// @brief 完整分类 ID（包含父分类路径）
	string full_id = "";
	
	/// @brief 图片排序方式
	string imageorder = "";
	
	/// @brief 最后修改时间（时间戳，秒）
	uint64_t lastmodified = 0;
	
	/// @brief 图片数量
	uint32_t nb_images = 0;
	
	/// @brief 子分类数量
	uint32_t sub_images = 0;
	
	/// @brief 分类数量（可能是递归统计）
	uint32_t nb_categories = 0;
	
	/// @brief 根分类 ID
	string root_id = "";
	
	/// @brief 根分类目录路径
	wstring root_dir0 = L"";
	
	/// @brief 添加者用户 ID
	uint32_t addby = 0;
} CATEGORY_DTO;

/// @brief 分类数据传输对象（版本 2）
/// @details 存储分类的简化信息（用于目录列表）。
typedef struct _CATEGORY_DTO2
{
	/// @brief 分类 ID
	string id = "";
	
	/// @brief 分类目录路径（宽字符）
	wstring dir0 = L"";
	
	/// @brief 根分类 ID（宽字符）
	wstring root_id = L"";
	
	/// @brief 根分类目录路径（宽字符）
	wstring root_dir0 = L"";
} CATEGORY_DTO2;

/// @brief 标签数据传输对象
/// @details 存储标签的完整信息，包括名称、图片数量、最后修改时间。
typedef struct _TAG_DTO
{
	/// @brief 标签 ID
	string	id = "";
	
	/// @brief 标签名称
	string	name = "";
	
	/// @brief 最后修改时间（时间戳，秒）
	uint64_t lastmodified = 0;
	
	/// @brief 使用该标签的图片数量
	uint32_t nb_images = 0;
} TAG_DTO;

/// @brief 排序数据传输对象
/// @details 存储排序信息（ID、排序方式、标题、类型）。
typedef struct _ORDER_DTO
{
	/// @brief 排序 ID
	string	id = "";
	
	/// @brief 排序方式（如 "date_creation", "name", "size" 等）
	string	order = "";
	
	/// @brief 排序标题（显示用）
	string	title = "";
	
	/// @brief 排序类型
	string type = "";
} ORDER_DTO;

/// @brief 根目录数据传输对象
/// @details 存储根目录信息（ID、目录路径、大小）。
typedef struct _ROOT_DTO
{
	/// @brief 根目录 ID
	string	id = "";
	
	/// @brief 根目录路径
	string	root_dir0 = "";
	
	/// @brief 根目录大小（字节）
	uint64_t root_sizes = 0;
} ROOT_DTO;

/// @brief 根目录数据传输对象（版本 2）
/// @details 存储根目录的简化信息（宽字符版本）。
typedef struct _ROOT_DTO2
{
	/// @brief 根目录 ID（宽字符）
	wstring	id = L"";
	
	/// @brief 根目录路径（宽字符）
	wstring	root_dir0 = L"";
	
	/// @brief 根目录大小（字节）
	uint64_t root_sizes = 0;
} ROOT_DTO2;

/// @brief 图片数据传输对象
/// @details 存储图片的完整信息，包括文件名、路径、尺寸、EXIF、GPS、缩略图、COI 等。
///          这是**最核心的 DTO**，包含图片的所有元数据。
typedef struct _IMAGE_DTO
{
	/// @brief 图片 ID
	string id = "";
	
	/// @brief 文件名（不含路径）
	string file = "";
	
	/// @brief 可用日期（时间戳，秒）
	uint64_t date_available = 0;
	
	/// @brief 创建日期（时间戳，秒，来自 EXIF）
	uint64_t date_creation = 0;
	
	/// @brief 图片名称（用户自定义）
	string name = "";
	
	/// @brief 图片注释
	string comment = "";
	
	/// @brief 作者
	string author = "";
	
	/// @brief 点击次数（查看次数）
	uint32_t hit = 0;
	
	/// @brief 文件大小（字节）
	uint32_t filesize = 0;
	
	/// @brief 图片宽度（像素）
	uint32_t width = 0;
	
	/// @brief 图片高度（像素）
	uint32_t height = 0;
	
	/// @brief COI（Clip Object Image）路径
	string coi = "";
	
	/// @brief 代表扩展名（用于封面）
	string representative_ext = "";
	
	/// @brief 可用日期（按天，时间戳，秒）
	uint64_t date_available_day = 0;
	
	/// @brief 原始路径
	string ori = "";
	
	/// @brief 存储分类 ID
	string storage_category_id = "";
	
	/// @brief 图片质量评分（1-5）
	uint32_t quality = 0;
	
	/// @brief MD5 校验和
	uint64_t md5sum = 0;
	
	/// @brief 添加者用户 ID
	string added_by = "";
	
	/// @brief 旋转角度（0, 90, 180, 270）
	uint32_t rotation = 0;
	
	/// @brief GPS 纬度
	float latitude = 0;
	
	/// @brief GPS 经度
	float longitude = 0;
	
	/// @brief 最后修改时间（时间戳，秒）
	uint64_t lastmodified = 0;
	
	/// @brief 文件扩展名
	string ext = "";
	
	/// @brief 视频时长（秒）
	uint32_t duration = 0;
	
	/// @brief 创建日期（按天，时间戳，秒）
	uint64_t date_creation_day = 0;
	
	/// @brief 图片宽高比
	float ratio = 0;
	
	/// @brief COI 宽度（像素）
	uint32_t coi_w = 0;
	
	/// @brief COI 高度（像素）
	uint32_t coi_h = 0;
	
	/// @brief 是否包含 COI（0: 否, 1: 是）
	uint32_t iscoi = 0;
	
	/// @brief COI 最后修改时间（时间戳，秒）
	uint64_t coi_lastmodified = 0;
	
	/// @brief COI 质量评分（1-5）
	uint32_t coi_quality = 0;
	
	/// @brief COI 文件大小（字节）
	uint32_t coi_filesize = 0;
	
	/// @brief EXIF 方向值（1-8）
	uint32_t orientation = 0;
	
} IMAGE_DTO;

/// @brief 收藏数据传输对象
/// @details 存储用户收藏的图片信息。
typedef struct _FAVORITE_DTO
{
	/// @brief 用户 ID
	string user_id = "";
	
	/// @brief 图片 ID
	string image_id = "";
} FAVORITE_DTO;

/// @brief 人脸数据传输对象
/// @details 存储图片中检测到的人脸信息。
typedef struct _FACE_DTO
{
	/// @brief 人脸 ID
	string id = "";
	
	/// @brief 人脸名称（可能是人名）
	string name = "";
	
	/// @brief 图片 ID
	string image_id = "";
	
	/// @brief 字典路径（可能是人脸识别模型文件）
	string dict = "";
	
	/// @brief 合并到的人脸 ID
	string merge_id = "";
	
	/// @brief 人脸区域（可能是坐标或区域描述）
	string al = "";
	
	/// @brief 人脸数量（可能是该图片中的人脸数）
	uint32_t nb = 0;
} FACE_DTO;

/// @brief 购物车数据传输对象
/// @details 存储用户购物车（待处理图片列表）信息。
typedef struct _CADDIE_DTO
{
	/// @brief 用户 ID
	string user_id = "";
	
	/// @brief 元素 ID（图片 ID 列表，逗号分隔）
	string element_id = "";
} CADDIE_DTO;

/// @brief 图片-分类关联数据传输对象
/// @details 存储图片和分类的多对多关联关系。
typedef struct _IMAGE_CATEGORY_DTO
{
	/// @brief 图片 ID
	string image_id = "";
	
	/// @brief 分类 ID
	string category_id = "";
	
	/// @brief 分类名称
	string category_name = "";
	
	/// @brief 排序等级
	string rank = "";
} IMAGE_CATEGORY_DTO;

/// @brief 图片-标签关联数据传输对象
/// @details 存储图片和标签的多对多关联关系。
typedef struct _IMAGE_TAG_DTO
{
	/// @brief 图片 ID
	string image_id = "";
	
	/// @brief 标签 ID
	string tag_id = "";
	
	/// @brief 标签名称
	string tag_name = "";
} IMAGE_TAG_DTO;

/// @brief 用户数据传输对象
/// @details 存储用户信息（ID、用户名、密码、提示问题）。
typedef struct _USER_DTO
{
	/// @brief 用户 ID
	string id = "";
	
	/// @brief 用户名
	string username = "";
	
	/// @brief 密码（加密后）
	string password = "";
	
	/// @brief 密码提示问题
	string hint = "";
}USER_DTO;

/// @brief 查询请求结构体
/// @details 封装图片查询的所有条件，包括分类、标签、收藏、日期范围、搜索关键词等。
///          用于生成 SQL WHERE 子句。
typedef struct _QUERY_REQ
{
	/// @brief 分类 ID
	string cat_id = "";
	
	/// @brief 图片 ID 列表（逗号分隔）
	string imageids = "";
	
	/// @brief 每页图片数量
	string page_nb = "";
	
	/// @brief 收藏筛选（"1": 仅显示收藏）
	string favorites = "";
	
	/// @brief 购物车筛选（"1": 仅显示购物车中的图片）
	string caddie = "";
	
	/// @brief 最常访问图片筛选（"1": 按访问次数排序）
	string most_visited = "";
	
	/// @brief 最近图片筛选（"1": 仅显示最近添加的图片）
	string recent_pics = "";
	
	/// @brief 无标签图片筛选（"1": 仅显示无标签的图片）
	string no_tag = "";
	
	/// @brief 打开方式（可能是 "box", "slide" 等）
	string osm = "";
	
	/// @brief 经度（用于地理位置筛选）
	string longitude = "";
	
	/// @brief 纬度（用于地理位置筛选）
	string latitude = "";
	
	/// @brief 列表视图 ID 列表（逗号分隔）
	string list0 = "";
	
	/// @brief 已发布的图片列表（逗号分隔）
	string posted_list = "";
	
	/// @brief 已创建的图片列表（逗号分隔）
	string created_list = "";
	
	/// @brief 发布状态筛选（"1": 仅显示已发布的图片）
	string posted = "";
	
	/// @brief 开始日期（用于日期范围筛选）
	string date_start = "";
	
	/// @brief 结束日期（用于日期范围筛选）
	string date_end = "";
	
	/// @brief 最近访问的分类 ID 列表（逗号分隔）
	string recent_cats = "";
	
	/// @brief 标签 ID 列表（逗号分隔）
	string tags = "";
	
	/// @brief 搜索关键词
	string q = "";
	
	/// @brief 搜索模式（"simple": 简单搜索, "advanced": 高级搜索）
	string search = "";
	
	/// @brief 起始索引（用于分页）
	string start = "";
	
	/// @brief 图片排序方式
	string image_order = "";
	
	/// @brief POST 请求体
	string szBody = "";
	
	/// @brief 请求参数
	string args = "";
	
	/// @brief 首页 URL
	string home = "";
	
	/// @brief 文章列表（可能是博客或文章功能）
	string art_list = "";
	
	/// @brief 人脸筛选（"1": 仅显示有人脸的图片）
	string face = "";
} QUERY_REQ;

/// @brief 搜索请求结构体
/// @details 封装高级搜索的所有条件，包括文件类型、日期范围、尺寸范围、作者、标签等。
///          比 QUERY_REQ 更详细，支持更多筛选条件。
typedef struct _SEARCH_REQ
{
	/// @brief 搜索开关（"1": 启用搜索）
	string search = "1";
	
	// batchform（可能是批量操作表单）
	
	/// @brief 筛选器-预设筛选器使用
	string filter_prefilter_use = "";
	
	/// @brief 筛选器-预设筛选器值
	string filter = "";
	
	
	// searchfrom（搜索来源）
	
	/// @brief 页码
	string page = "";
	
	/// @brief 所有分类（可能是 "1": 搜索所有分类）
	string allcats = "";
	
	/// @brief 日期类型（"date_creation": 创建日期, "date_available": 可用日期）
	string date_type = "date_creation";
	
	
	/// @brief 筛选器-搜索使用
	string filter_search_use = "";
	
	/// @brief 搜索-所有关键词（可能是 "1": 匹配所有关键词）
	string search_allwords = "";
	
	/// @brief 搜索模式（"AND": 与, "OR": 或）
	string mode = "AND";
	
	/// @brief 搜索-在名称中搜索
	string search_in_name = "";
	
	/// @brief 搜索-在注释中搜索
	string search_in_comment = "";
	
	/// @brief 搜索-在文件中搜索
	string search_in_file = "";
	
	/// @brief 搜索-在标签中搜索
	string search_in_tags = "";
	
	
	/// @brief 筛选器-分类使用
	string filter_category_use = "";
	
	/// @brief 分类 ID
	string cat = "";
	
	/// @brief 子分类（"on": 包含子分类）
	string subcats = "on";
	
	
	/// @brief 筛选器-文件类型使用
	string filter_filetypes_use = "";
	
	/// @brief 搜索-图片（"on": 包含图片）
	string search_images = "";
	
	/// @brief 搜索-视频（"on": 包含视频）
	string search_videos = "";
	
	
	/// @brief 筛选器-日期使用
	string filter_date_use = "";
	
	/// @brief 创建日期-最小值
	string createdate_min = "";
	
	/// @brief 创建日期-最大值
	string createdate_max = "";
	
	/// @brief 发布日期-最小值
	string postdate_min = "";
	
	/// @brief 发布日期-最大值
	string postdate_max = "";
	
	
	/// @brief 创建日期-最小值（原始值）
	string createdate_min_o = "";
	
	/// @brief 创建日期-最大值（原始值）
	string createdate_max_o = "";
	
	/// @brief 发布日期-最小值（原始值）
	string postdate_min_o = "";
	
	/// @brief 发布日期-最大值（原始值）
	string postdate_max_o = "";
	
	
	/// @brief 筛选器-尺寸使用
	string filter_dimension_use = "";
	
	/// @brief 尺寸-最小宽度
	string dimension_min_width = "";
	
	/// @brief 尺寸-最大宽度
	string dimension_max_width = "";
	
	/// @brief 尺寸-最小高度
	string dimension_min_height = "";
	
	/// @brief 尺寸-最大高度
	string dimension_max_height = "";
	
	/// @brief 尺寸-最小宽高比
	string dimension_min_ratio = "";
	
	/// @brief 尺寸-最大宽高比
	string dimension_max_ratio = "";
	
	/// @brief 宽度-最小值（原始值）
	string width_min_o = "";
	
	/// @brief 宽度-最大值（原始值）
	string width_max_o = "";
	
	/// @brief 高度-最小值（原始值）
	string height_min_o = "";
	
	/// @brief 高度-最大值（原始值）
	string height_max_o = "";
	
	/// @brief 宽高比-最小值（原始值）
	string ratio_min_o = "";
	
	/// @brief 宽高比-最大值（原始值）
	string ratio_max_o = "";
	
	/// @brief 文件大小-最小值
	string filesize_min = "";
	
	/// @brief 文件大小-最大值
	string filesize_max = "";
	
	/// @brief 文件大小-最小值（原始值）
	string filesize_min_o = "";
	
	/// @brief 文件大小-最大值（原始值）
	string filesize_max_o = "";
	
	
	/// @brief 尺寸类型（"widths": 按宽度, "heights": 按高度, "ratios": 按宽高比）
	string dimension_type = "widths";
	
	/// @brief 筛选器-作者使用
	string filter_authors_use = "";
	
	/// @brief 作者列表（逗号分隔）
	string authors = "";
	
	
	/// @brief 筛选器-OSM 使用
	string filter_osm_use = "";
	
	/// @brief OSM 值 0
	string osm0 = "";
	
	/// @brief OSM 值 1
	string osm1 = "";
	
	
	/// @brief 筛选器-标签使用
	string filter_tags_use = "";
	
	/// @brief 标签 ID 列表（逗号分隔）
	string tags = "";
	
	/// @brief 标签模式（"AND": 与, "OR": 或）
	string tag_mode = "AND";
	
	
}SEARCH_REQ;

/// @brief 滑块值范围结构体
/// @details 存储用于滑块筛选（slider filter）的各种值范围。
typedef struct _SLIDER_VALUES
{
	/// @brief 宽度列表
	list<uint32_t>  widths;
	
	/// @brief 高度列表
	list<uint32_t>  heights;
	
	/// @brief 文件大小列表
	list<float>  filesizes;
	
	/// @brief 创建日期列表
	list<uint64_t> createdates;
	
	/// @brief 发布日期列表
	list<uint64_t> postdates;
	
	/// @brief 宽高比列表
	list<float>  ratios;
	
}SLIDER_VALUES;

/// @brief 图片完整信息数据传输对象
/// @details 存储图片的完整信息（用于导入/导出），包括名称、作者、日期、分类、标签、描述、COI、缩略图等。
typedef struct _PHOTO_DTO
{
	/// @brief 图片 ID
	string id = "";
	
	/// @brief 图片名称
	string name = "";
	
	/// @brief 作者
	string author = "";
	
	/// @brief 创建日期（时间戳，秒）
	uint64_t date_creation = 0;
	
	/// @brief 关联图片 ID（可能是拼接图片、对比图片等）
	string associate = "";
	
	/// @brief 分类列表（逗号分隔）
	string categories = "";
	
	/// @brief 标签列表（逗号分隔）
	string tags = "";
	
	/// @brief 图片描述
	string description = "";
	
	/// @brief COI 路径
	string coi = "";
	
	/// @brief 原始路径
	string ori = "";
	
	/// @brief 文件名
	string file = "";
	
	/// @brief 点击次数
	string hit = "";
	
	/// @brief 可用日期（时间戳，秒）
	uint64_t date_available = 0;
	
	/// @brief 文件大小（字节）
	string filesize = "";
	
	/// @brief 宽度（像素）
	string width = "";
	
	/// @brief 高度（像素）
	string height = "";
	
	/// @brief 文件扩展名
	string ext = "";
	
	/// @brief 旋转角度
	string rotation = "";
	
	/// @brief 代表扩展名（用于封面）
	string representative_ext = "";
	
	/// @brief 视频时长（秒）
	uint32_t duration = 0;
	
	/// @brief 添加者用户 ID
	string added_by = "";
	
	/// @brief COI 宽度（像素）
	uint32_t coi_w = 0;
	
	/// @brief COI 高度（像素）
	uint32_t coi_h = 0;
	
	/// @brief 是否包含 COI（0: 否, 1: 是）
	uint32_t iscoi = 0;
	
	/// @brief 存储分类 ID
	uint32_t storage_category_id = 0;
	
	/// @brief 存储分类名称
	string storage_category = "";
	
}PHOTO_DTO;

/// @brief 仪表盘数据传输对象
/// @details 存储仪表盘（Dashboard）的统计信息，包括图片总数、相册数、标签数、评论数、浏览数等。
typedef struct _DASHBOARD_DTO
{
	/// @brief 图片总数
	uint32_t nb_photos = 0;
	
	/// @brief 相册总数
	uint32_t nb_albums = 0;
	
	/// @brief 标签总数
	uint32_t nb_tags = 0;
	
	/// @brief 评论总数
	uint32_t nb_comments = 0;
	
	/// @brief 浏览总数
	uint32_t nb_views = 0;
	
	/// @brief 有标签的图片数量
	uint32_t nb_tag_images = 0;
	
	/// @brief 首次添加图片的时间（时间戳，秒）
	uint64_t nb_first_add = 0;
}DASHBOARD_DTO;

/// @brief 日历数据传输对象
/// @details 存储日历视图的统计信息（按年、月、日分组）。
typedef struct _CAL_DTO
{
	/// @brief 年份
	uint32_t year = 0;
	
	/// @brief 月份（1-12）
	uint32_t month = 0;
	
	/// @brief 日期（1-31）
	uint32_t day = 0;
	
	/// @brief 该年的图片数量
	uint32_t nb_year_images = 0;
	
	/// @brief 该月的图片数量
	uint32_t nb_month_images = 0;
	
	/// @brief 该日的图片数量
	uint32_t nb_day_images = 0;
}CAL_DTO;

/// @brief 上传数据传输对象
/// @details 存储文件上传的相关信息，包括密钥文件路径、尺寸、方向、完整路径、源文件等。
typedef struct _UPLOAD_DATA
{
	/// @brief 密钥文件路径（用于加密）
	wstring krf = L"";
	
	/// @brief 目标宽度（像素）
	uint32_t width = 0;
	
	/// @brief 目标高度（像素）
	uint32_t height = 0;
	
	/// @brief 完整文件路径（宽字符）
	wstring fullname = L"";
	
	/// @brief 旋转角度
	uint32_t orientation = 0;
	
	/// @brief 源文件路径（宽字符）
	wstring srcfile = L"";
	
} UPLOAD_DATA;

/// @brief 历史记录数据传输对象
/// @details 存储用户历史记录信息（如最近查看的图片）。
typedef struct _HISTORY_DTO
{
	/// @brief 图片 ID
	string id = "";
	
	/// @brief 最后修改时间（时间戳，秒）
	uint64_t lastmodified = 0;
	
	/// @brief 用户 ID
	string user_id = "";
	
} HISTORY_DTO;

/// @brief 历史记录汇总数据传输对象
/// @details 存储历史记录的汇总信息（按年、月、日、小时分组）。
typedef struct _HISTORY_SUMMARY_DTO
{
	/// @brief 年份
	string year = "";
	
	/// @brief 月份（1-12）
	string month = "";
	
	/// @brief 日期（1-31）
	string day = "";
	
	/// @brief 小时（0-23）
	string hour = "";
	
	/// @brief 页数（可能是分页信息）
	uint32_t nb_pages = 0;
	
} HISTORY_SUMMARY_DTO;

/// @brief 日志查看器数据传输对象
/// @details 存储日志查看器的信息（文件夹、文件名、最后修改时间）。
typedef struct _LOG_VIEWER_DTO
{
	/// @brief 日志 ID
	string id = "";
	
	/// @brief 文件夹路径
	string folder = "";
	
	/// @brief 文件名
	string file = "";
	
	/// @brief 最后修改时间（时间戳，秒）
	uint64_t lastmodified = 0;
	
} LOG_VIEWER_DTO;

/// @brief 文件数据传输对象
/// @details 存储文件信息（分类 ID、文件名、完整路径、大小）。
typedef struct _FILE_DTO
{
	/// @brief 分类 ID
	string cat_id = "";
	
	/// @brief 文件名（宽字符）
	wstring filename = L"";
	
	/// @brief 完整文件路径（宽字符）
	wstring file = L"";
	
	/// @brief 文件大小（字节）
	DWORD dwSize = 0;
	
} FILE_DTO;

/// @brief 文件夹状态数据传输对象
/// @details 存储文件夹状态信息（上传、待处理、状态、相册、根 ID、路径）。
typedef struct _FOLDER_STATE_DTO
{
	/// @brief 文件夹 ID
	string id = "";
	
	/// @brief 已上传文件列表（逗号分隔）
	string uploaded = "";
	
	/// @brief 待处理文件列表（逗号分隔）
	string pending = "";
	
	/// @brief 状态（如 "uploading", "processing", "done"）
	string state = "";
	
	/// @brief 相册名称
	string alb = "";
	
	/// @brief 根 ID
	string root_id = "";
	
	/// @brief 文件夹路径
	string path = "";
	
} FOLDER_STATE_DTO;

/// @brief 索引数据传输对象
/// @details 存储索引信息（可能是用于数据库索引或搜索索引）。
typedef struct _IDX_DTO
{
	/// @brief 索引 ID
	int idx = 0;
	
	/// @brief 参数指针
	LPVOID pParam = 0;
	
} IDX_DTO;

/// @brief 每日图片数据传输对象
/// @details 存储按日分组的图片信息（用于日历视图）。
typedef struct _DAY_IMAGES_DTO
{
	/// @brief 图片 ID
	string image_id = "";
	
	/// @brief 日期（时间戳，秒）
	uint64_t day2 = 0;
	
	/// @brief 该日图片数量
	uint32_t nb_images = 0;
} DAY_IMAGES_DTO;

/// @brief 每周图片数据传输对象
/// @details 存储按周分组的图片信息（用于周视图）。
typedef struct _WEEK_IMAGES_DTO
{
	/// @brief 图片 ID
	string image_id = "";
	
	/// @brief 日期（时间戳，秒）
	uint64_t day2 = 0;
	
	/// @brief 该周图片数量
	uint32_t nb_images = 0;
} WEEK_IMAGES_DTO;

/// @brief 胶片数据传输对象
/// @details 存储胶片（可能是图片拼接或幻灯片）信息。
typedef struct _FILM_DTO
{
	/// @brief 胶片 ID
	uint32_t id = 0;
	
	/// @brief MD5 校验和
	uint64_t md5sum = 0;
	
	/// @brief 标题 1（可能是主标题）
	string title1 = "";
	
	/// @brief 标题 2（可能是副标题）
	string title2 = "";
	
	/// @brief 图片列表（逗号分隔）
	string photos = "";
	
	/// @brief 最后修改时间（时间戳，秒）
	uint64_t lastmodified = 0;
	
	/// @brief 胶片类型（可能是 "horizontal", "vertical" 等）
	string type = "";
	
	/// @brief 背景音乐文件路径
	string music = "";
}FILM_DTO;

/// @brief MD5 数据传输对象
/// @details 存储图片 MD5 校验和错误信息。
typedef struct _MD5_DTO
{
	/// @brief 分类 ID
	string cat_id = "";
	
	/// @brief 图片 ID
	string pic_id = "";
	
	/// @brief MD5 校验和
	uint64_t md5sum = 0;
	
	/// @brief 最后修改时间（时间戳，秒）
	uint64_t lastmodified = 0;
	
	/// @brief 文件路径
	string path = "";
	
	/// @brief 错误信息
	string msg = "";
	
}MD5_DTO;

/// @brief 分类数量数据传输对象
/// @details 存储分类及其图片数量信息。
typedef struct _CAT_NB_DTO
{
	/// @brief 分类 ID
	string id = "";
	
	/// @brief 根分类 ID
	string root_id = "";
	
	/// @brief 分类目录路径（宽字符）
	wstring dir0 = L"";
	
	/// @brief 数据库中的文件数量
	uint32_t nb_db_files = 0;
	
	/// @brief 数据库中的图片数量
	uint32_t nb_db_pics = 0;
	
	/// @brief 错误图片数量（MD5 校验和不匹配）
	uint32_t nb_err_pics = 0;
	
	/// @brief 文件夹中的文件数量
	uint32_t nb_folder_files = 0;
	
	/// @brief 文件夹中的图片数量
	uint32_t nb_folder_pics = 0;
	
	/// @brief 变更数量（可能是新增或删除的文件数）
	int nb_changed = 0;
} CAT_NB_DTO;

/// @brief 翻译数据传输对象
/// @details 存储翻译信息（可能是用于多语言支持的临时结构）。
typedef struct _TR_DTO
{
	/// @brief 翻译 ID
	string id = "";
	
	/// @brief 翻译文本
	string tr = "";
	
} TR_DTO;

/// @brief MD5 映射表
/// @details 映射分类 ID 到 MD5 DTO 列表。
typedef map<string, list<MD5_DTO>> MAP_MD5;
