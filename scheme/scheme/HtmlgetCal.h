///////////////////////////////////////////////////////////////////////////////
// 文件名: HtmlgetCal.h
// 模块: scheme/scheme
// 功能描述: 日历视图界面 HTML 生成类
//           生成日历视图界面的 HTML 内容（按日期浏览图片）
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

/// @brief 日历视图界面 HTML 生成类
/// @details 生成日历视图界面的 HTML 内容，包括：
///          - 日历网格（按日、周、月显示）
///          - 日期导航（上一年/月、下一年/月）
///          - 日期缩略图展示
///          - 日期筛选和搜索
///          采用单例模式确保全局唯一实例。
/// @note 日历视图是按日期浏览图片的重要界面
/// @warning 日历数据量大的时候需注意性能优化
class CHtmlgetCal : public CImBase, public Singleton<CHtmlgetCal>
{
	friend class Singleton<CHtmlgetCal>;
	
protected:
	/// @brief 构造函数
	/// @details 初始化日历视图处理对象
	CHtmlgetCal(void);
	
	/// @brief 析构函数
	/// @details 释放资源
	~CHtmlgetCal(void);

public:
	/// @brief 生成日历视图 HTML
	/// @details 根据查询请求生成日历视图的 HTML 内容
	/// @param[in] req 查询请求对象
	/// @param[in] cal 日历类型（"day", "week", "month"）
	/// @param[in] year 年份
	/// @return 生成的日历视图 HTML 字符串
	/// @note 支持日视图、周视图、月视图
	string get_calendarViews(QUERY_REQ req, string cal, string year);
	
	/// @brief 生成日历字符串
	/// @details 生成指定类型和日期范围的日历 HTML 字符串
	/// @param[in] req 查询请求对象
	/// @param[in] search_req 搜索请求对象
	/// @return 日历 HTML 字符串
	/// @note 根据搜索条件筛选图片
	string get_cal_string(QUERY_REQ req, SEARCH_REQ search_req);
	
	/// @brief 生成日历 URL
	/// @details 生成日历视图的导航 URL（用于日期跳转）
	/// @param[in] req 查询请求对象
	/// @param[in] type URL 类型
	/// @param[in] cal 日历类型
	/// @param[in] year 年份
	/// @return 生成的 URL 字符串
	string get_cal_url(QUERY_REQ req, string type, string cal, string year);
	
	/// @brief 生成日历标题
	/// @details 根据日期参数生成日历视图的标题（如"2024 年 6 月"）
	/// @param[in] req 查询请求对象
	/// @param[in] cal 日历类型
	/// @param[in] tm_year 年份
	/// @param[in] tm_mon 月份
	/// @param[in] tm_mday 日期
	/// @return 生成的标题字符串
	string get_calendar_title(QUERY_REQ req, string cal, string tm_year, string tm_mon, string tm_mday);
	
	/// @brief 生成周视图字符串
	/// @details 生成按周展示的日历视图 HTML
	/// @param[in] req 查询请求对象
	/// @param[in] search_req 搜索请求对象
	/// @param[in] type 周视图类型
	/// @param[in] year 年份
	/// @return 周视图 HTML 字符串
	string get_weekly_string(QUERY_REQ req, SEARCH_REQ search_req, string type, string year);
	
	/// @brief 获取日历缩略图
	/// @details 获取日历视图中每个日期对应的图片缩略图
	/// @param[in] bfirst 是否首次加载
	/// @param[in] req 查询请求对象
	/// @param[in] search_req 搜索请求对象
	/// @param[out] calendarBar_html 输出参数，日历缩略图 HTML
	/// @param[out] nb 输出参数，缩略图数量
	/// @param[out] calendarTitle 输出参数，日历标题
	/// @return 0: 成功; 其他: 错误码
	/// @note 缩略图用于日历日期下方的小图预览
	long get_cal_thumbnails(bool bfirst, QUERY_REQ req, SEARCH_REQ search_req, string &calendarBar_html, uint32_t &nb, string &calendarTitle);
	
	/// @brief 输出 SQL 字符串
	/// @details 存储最近一次查询的 SQL 语句（用于调试）
	string _outsql = "";

private:
	/// @brief 时间戳转换为系统时间
	/// @details 将 Unix 时间戳转换为 SYSTEMTIME 结构体
	/// @param[in] t Unix 时间戳
	/// @return 系统时间结构体
	/// @note 用于日期计算和显示
	SYSTEMTIME time_t_to_systemtime(time_t t);
	
	/// @brief 生成日历 HTML
	/// @details 内部方法，生成日历网格的 HTML
	/// @param[in] req 查询请求对象
	/// @param[in] search_req 搜索请求对象
	/// @param[in] week_starts_on 一周起始日（0=周日, 1=周一）
	/// @return 日历 HTML 字符串
	string get_cal(QUERY_REQ req, SEARCH_REQ search_req, int week_starts_on);
	
	/// @brief 生成周视图 HTML
	/// @details 内部方法，生成周视图的 HTML
	/// @param[in] req 查询请求对象
	/// @param[in] search_req 搜索请求对象
	/// @param[in] type 周视图类型
	/// @param[in] week_starts_on 一周起始日
	/// @param[in] year 年份
	/// @return 周视图 HTML 字符串
	string get_weekly(QUERY_REQ req, SEARCH_REQ search_req, string type, int week_starts_on, string year);
	
	/// @brief 获取日历月份表头
	/// @details 生成日历月份视图的表头 HTML（星期名称）
	/// @param[in,out] week_starts_on 一周起始日（可能被修改）
	/// @param[out] calMonth_thead 输出参数，表头 HTML
	/// @return 0: 成功; 其他: 错误码
	long get_calMonth_thead(int &week_starts_on, string &calMonth_thead);

private:
	/// @brief 日历数据映射表
	/// @details 存储日历中每个日期的图片数量（日期 -> 数量）
	map<uint32_t, uint32_t> _mapCal;
};
