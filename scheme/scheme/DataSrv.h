///////////////////////////////////////////////////////////////////////////////
// 文件名: DataSrv.h
// 模块: scheme/scheme
// 功能描述: 数据服务层，提供应用程序核心数据访问和业务逻辑
//           包括相册管理、图像查询、标签管理、配置管理等
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-04-15  PhotoNest Team  添加用户管理功能
//   2024-06-01  PhotoNest Team  优化缓存机制
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include <signal.h>
#include <atomic>

// 注释掉的代码已移除 - 原为多关键字排序比较器
// struct cmp
// {
//	   bool operator()(string const a, string const b) const
//	   {
//		       if (a != b)
//		       {
//			           return a < b;
//		       }
//		       else
//		       {
//			           return a < b;
//		       }
//	   }
// };

/// @brief 数据服务类
/// @details 应用程序的核心数据服务层，提供相册管理、图像查询、
///          标签管理、收藏管理、配置管理等业务功能。
///          采用单例模式确保全局唯一实例，
///          使用缓存机制提升数据访问性能。
/// @note 此类线程安全，关键操作使用临界区保护
/// @warning 部分操作会修改数据库，请确保调用前已获取必要权限
class CDataSrv : public Singleton<CDataSrv>
{
	friend class Singleton<CDataSrv>;

protected:
	/// @brief 构造函数
	/// @details 初始化数据服务，创建临界区对象，设置默认值
	CDataSrv(void);
	
	/// @brief 析构函数
	/// @details 释放临界区资源，清理缓存数据
	~CDataSrv(void);

public:
	/// @brief 清除标签缓存
	/// @details 清空内存中的标签列表缓存，下次访问时重新从数据库加载
	/// @return 0: 成功
	/// @note 在标签数据变更后调用此方法确保数据一致性
	long clear_cache_tags();
	
	/// @brief 清除图像 ID 缓存
	/// @details 清空内存中的图像 ID 列表缓存
	/// @return 0: 成功
	long clear_cache_image_ids();
	
	/// @brief 刷新收藏列表
	/// @details 从数据库重新加载当前用户的收藏数据到内存缓存
	/// @return 0: 成功; 其他: 错误码
	long refresh_favorites();
	
	/// @brief 刷新相册标题
	/// @details 从数据库重新加载相册标题配置
	/// @return 0: 成功; 其他: 错误码
	long refresh_gallery_title();
	
	/// @brief 清除轮播数据缓存
	/// @details 清空幻灯片轮播相关数据的缓存
	/// @return 0: 成功
	long clear_cache_slider();
	
	/// @brief 获取图像 URL
	/// @details 根据图像类型和 DTO 对象生成图像的访问 URL，
	///          支持缩略图、原图、视频等不同格式。
	/// @param[in] type 图像类型（1=缩略图, 2=原图, 3=视频等）
	/// @param[in] dto 图像数据传输对象，包含图像路径、ID 等信息
	/// @return 图像的 HTTP 访问 URL 字符串
	/// @note URL 基于当前相册的配置生成
	string get_image_url(int type, IMAGE_DTO dto);
	
	/// @brief 获取菜单栏标签 HTML
	/// @details 生成导航菜单栏的 HTML 代码，包含标签列表和图片 ID。
	///          用于前端页面的动态菜单渲染。
	/// @param[in] type 菜单类型（1=标准, 2=管理）
	/// @param[in] cat_ids 分类 ID 列表（逗号分隔）
	/// @param[in] pic_ids 图片 ID 列表（逗号分隔）
	/// @return 菜单栏 HTML 字符串
	/// @note 返回的 HTML 可直接嵌入网页
	string get_menubar_tags(int type, string cat_ids, string pic_ids);
	
	/// @brief 获取标签列表
	/// @details 从缓存或数据库获取所有标签数据
	/// @return 标签列表指针，调用者不应长时间持有
	/// @note 首次调用会从数据库加载并缓存
	list<TAG_DTO>* get_lstTag();
	
	/// @brief 获取图像 ID 列表
	/// @details 获取当前查询条件下的所有图像 ID，用于批量操作
	/// @param[out] lstImageIds 输出参数，图像 ID 列表
	/// @return 0: 成功; 其他: 错误码
	long get_lstImageIds(list<string>& lstImageIds);
	
	/// @brief 获取轮播数值配置
	/// @details 获取幻灯片轮播的相关配置参数，
	///          如切换间隔、动画效果等
	/// @param[out] sliderValues 输出参数，轮播配置结构体
	/// @return 0: 成功; 其他: 错误码
	/// @see SLIDER_VALUES
	long get_sliderValues(SLIDER_VALUES& sliderValues);
	
	/// @brief 获取图像 ID 映射表
	/// @details 获取图像 ID 到相关数据的映射，用于快速查找
	/// @param[out] lstImageIds 输出参数，图像 ID 列表
	/// @return 0: 成功; 其他: 错误码
	long get_lstImageIds_map(list<string>& lstImageIds);
	
	/// @brief 清除所有缓存和数据
	/// @details 重置数据服务状态，清空所有缓存、搜索请求、
	///          查询结果等数据，恢复到初始状态
	/// @return 0: 成功
	/// @warning 此操作不可恢复，调用后需要重新加载数据
	long clear_all();
	
	/// @brief 获取根分类列表
	/// @details 获取所有顶级分类（没有父分类的分类）
	/// @param[out] lstRoot 输出参数，根分类列表
	/// @return 0: 成功; 其他: 错误码
	long get_roots(list<ROOT_DTO>& lstRoot);
	
	/// @brief 获取配置值
	/// @details 根据配置 ID 获取对应的配置值
	/// @param[in] id 配置 ID 字符串
	/// @param[out] val 输出参数，配置值
	/// @return 0: 成功; 其他: 错误码（如配置不存在）
	/// @see set_config_val()
	long get_config_val(string id, string& val);
	
	/// @brief 设置配置值
	/// @details 设置指定配置 ID 的值，如果配置不存在则创建
	/// @param[in] id 配置 ID 字符串
	/// @param[in] val 配置值
	/// @return 0: 成功; 其他: 错误码
	/// @note 此操作会立即写入数据库
	/// @see get_config_val()
	long set_config_val(string id, string val);
	
	/// @brief 获取播放页面 HTML 模板
	/// @details 根据 ID 获取播放器页面的 HTML 模板代码
	/// @param[in] id 模板 ID
	/// @param[out] tpl 输出参数，HTML 模板字符串
	/// @return 0: 成功; 其他: 错误码
	long get_play_htm(string id, string& tpl);
	
	/// @brief 选择相册
	/// @details 切换到指定相册，加载相册配置和数据。
	///          这是相册切换的核心方法。
	/// @param[in] albumName 相册名称
	/// @param[in] viewer 查看器类型
	/// @param[in] act 操作类型
	/// @param[out] syn 输出参数，同步状态信息
	/// @return 0: 成功; 其他: 错误码
	/// @note 切换相册会触发数据重新加载
	long select_gallery(string albumName, string viewer, string act, string& syn);
	
	/// @brief 写入私有配置
	/// @details 将配置值加密后写入私有配置文件
	/// @param[in] key 配置键
	/// @param[in] val 配置值（C 字符串）
	/// @note 使用私有加密算法保护敏感配置
	void WritePrivate(string key, const char* val);
	
	/// @brief 读取私有配置
	/// @details 从私有配置文件读取并解密配置值
	/// @param[in] key 配置键
	/// @param[out] val 输出参数，配置值
	/// @note 与 WritePrivate 配套使用
	void ReadPrivate(string key, string& val);
	
	/// @brief 获取语言资源
	/// @details 加载指定语言的所有翻译字符串
	/// @param[out] lcid 输出参数，语言 ID 映射表
	/// @param[out] lang 输出参数，翻译字符串映射表（key-value）
	/// @note 支持多语言国际化
	void get_language(map<string, string>& lcid, map<string, string>& lang);
	
	/// @brief 获取 Windows 系统目录
	/// @details 获取当前 Windows 系统的系统目录路径（如 C:\Windows）
	/// @param[out] windir 输出参数，系统目录路径（wstring）
	void get_windir(wstring& windir);
	
	/// @brief 获取当前程序和目录路径
	/// @details 获取应用程序所在目录和当前工作目录
	/// @param[out] szPath 输出参数，应用程序目录路径
	/// @param[out] currdir 输出参数，当前工作目录路径
	void get_currdir(wstring& szPath, wstring& currdir);
	
	/// @brief 获取胶片列表
	/// @details 获取所有胶片相册的数据列表
	/// @param[out] lstfilm 输出参数，胶片数据列表
	void get_films(list<FILM_DTO>& lstfilm);
	
	/// @brief 处理公共访问权限
	/// @details 检查并设置公共访问模式下的权限
	/// @note 用于共享相册的访问控制的
	void proc_public_acc();
	
	/// @brief 获取当前用户名
	/// @details 获取当前登录用户的用户名
	/// @return 用户名字符串
	/// @note 如果未登录返回空字符串或默认值
	string get_username();

private:
	/// @brief 初始化排序规则
	/// @details 从配置加载并初始化图像排序规则
	/// @return 0: 成功; 其他: 错误码
	long init_order();

public:
	/// @brief 搜索请求对象
	/// @details 存储当前搜索请求的参数，包括关键词、标签、日期范围等
	SEARCH_REQ _search_req;
	
	/// @brief 查询请求对象
	/// @details 存储高级查询请求的参数
	QUERY_REQ _q_req;
	
	/// @brief 收藏列表
	/// @details 内存缓存的当前用户的收藏数据
	list<FAVORITE_DTO> _lstFavorite;
	
	/// @brief 排序规则列表
	/// @details 图像显示的顺序规则配置
	list<ORDER_DTO> _lstOrder;
	
	/// @brief OSM（开放街图）数据列表
	/// @details 地理位置相关的标记数据
	list<string> _lstosm;

public:
	/// @brief 当前相册名称
	string _albumName = "";
	
	/// @brief 相册标题
	string _gallery_title = "";
	
	/// @brief 登录用户名（默认 "admin"）
	string _login = "admin";
	
	/// @brief 元数据模式（"nometa"=无元数据）
	string _metadata = "nometa";
	
	/// @brief 视图尺寸模式（"3"=默认）
	string _vrsize = "3";
	
	/// @brief 最后发布时间（用于时间线显示）
	string _last_posted = "";
	
	/// @brief 最后发布时间 2（备用字段）
	string _last_posted2 = "";
	
	/// @brief 验证码或邀请码
	string _code = "";
	
	/// @brief MP3 文件图标 URL
	string _mp3_url = "./templ/themes/default/images/mp3.png";
	
	/// @brief MP4 文件图标 URL
	string _mp4_url = "./templ/themes/default/images/mp4.png";
	
	/// @brief GIF 文件图标 URL
	string _gif_url = "./templ/themes/default/images/gif.png";
	
	/// @brief 排序方式
	string _orderby = "";
	
	/// @brief 只读模式标记（字符串形式）
	string _str_readonly = "";
	
	/// @brief 当前用户名
	string _username = "";
	
	/// @brief 主题皮肤名称（默认 "newspaper"）
	string _skin = "newspaper";
	
	/// @brief 背景图像 URL 或颜色
	string _bg = "";
	
	/// @brief 背景类型（"0"=默认）
	string _bgtype = "0";
	
	/// @brief 星级筛选（"0"=全部）
	string _star = "0";
	
	/// @brief 缩略图显示模式（"thumb"=缩略图）
	string _display_thumb = "thumb";
	
	/// @brief 访问权限级别（"0"=私有, "1"=公开）
	string _acc = "0";
	
	/// @brief 覆盖模式标记
	string _ov = "0";

	/// @brief 首次访问时间戳
	time_t _first = 0;
	
	/// @brief 相册标题缓存时间戳
	time_t _t_masrony_title = 0;
	
	/// @brief 只读模式标记（uint32_t 类型）
	uint32_t _readonly = 0;
	
	/// @brief 最大上传文件大小（MB，默认 200MB）
	uint32_t _maxFileSize = 200;

	/// @brief 时区偏移（分钟）
	/// @details 用于时间显示和存储的时区校正
	long _time_zone_bias = 0;
	
	/// @brief 数据库错误标记
	int _db_error = 0;
	
	/// @brief 日期筛选（天）
	int _day = 0;
	
	/// @brief 数据变更标记
	/// @details 标记数据是否已变更，触发 UI 更新
	int _ischanged = 0;
	
	/// @brief 初始化状态标记
	/// @details 使用原子操作确保线程安全的初始化
	///          2=未初始化, 1=初始化中, 0=已初始化
	atomic_int _uinit = 2;
	
	/// @brief 关闭事件句柄
	/// @details 用于通知数据服务优雅关闭
	HANDLE _shutdownEvent = NULL;
	
	/// @brief 应用程序数据目录
	wstring _appDataDir = L"";
	
	/// @brief 菜单索引（用于界面状态）
	int msix = 0;
	
private:
	/// @brief 轮播配置数据
	SLIDER_VALUES _sliderValues;
	
	/// @brief 标签列表缓存
	list<TAG_DTO> _lstTag;
	
	/// @brief 图像 ID 列表缓存
	list<string> _lstImageIds;
	
	/// @brief 临界区对象
	/// @details 保护共享数据的线程安全访问
	CRITICAL_SECTION _cs;
	
	/// @brief 缓存状态标记
	/// @details 0=无缓存, 1=标签已缓存, 2=图像 ID 已缓存
	int _hasCache = 0;
	
};