///////////////////////////////////////////////////////////////////////////////
// 文件名: HtmlgetMenubar.h
// 模块: scheme/scheme
// 功能描述: 菜单栏分类导航生成类
//           生成侧边栏和顶部菜单栏的分类导航 HTML
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-04-15  PhotoNest Team  添加分类排序功能
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

/// @brief 菜单栏分类导航生成类
/// @details 生成侧边栏和顶部菜单栏的分类导航 HTML，
///          支持分类排序、子分类显示、选中状态标记等。
///          继承自 CImBase 和 Singleton 模式。
/// @note 此类负责生成分类导航相关的所有 HTML
/// @warning 确保分类 ID 经过验证，防止路径遍历攻击
class CHtmlgetMenubar : public CImBase, public Singleton<CHtmlgetMenubar>
{
	friend class Singleton<CHtmlgetMenubar>;

protected:
	/// @brief 构造函数
	/// @details 初始化菜单栏生成对象，初始化临界区
	CHtmlgetMenubar(void);
	
	/// @brief 析构函数
	/// @details 清理资源，删除临界区
	~CHtmlgetMenubar(void);

public:
	/// @brief 清理缓存数据
	/// @return 0: 成功; 其他: 错误码
	/// @note 清理分类列表缓存
	long clear();
	
	/// @brief 按排序等级获取分类列表
	/// @param[in] ty 类型（0: 侧边栏, 1: 顶部菜单）
	/// @param[out] lstCategory 分类 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 根据排序等级（rank）排序分类
	long get_categories_order_by_rank(int ty, list<CATEGORY_DTO>& lstCategory);
	
	/// @brief 按根目录顺序获取分类列表
	/// @param[in] ty 类型（0: 侧边栏, 1: 顶部菜单）
	/// @param[out] lstCategory 分类 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 根据根目录顺序排序分类
	long get_categories_order_by_root_dir(int ty, list<CATEGORY_DTO>& lstCategory);
	
	/// @brief 获取指定分类 ID 列表的分类信息
	/// @param[in] catids 分类 ID 列表（逗号分隔）
	/// @param[out] lstCategory 分类 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 批量获取分类信息
	long get_categories(string catids, list<CATEGORY_DTO>& lstCategory);
	
	/// @brief 获取排序后的分类列表
	/// @param[in] order 排序方式
	/// @param[out] lstCategory 分类 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 根据指定的排序方式获取分类列表
	long get_order_categories(string order, list<CATEGORY_DTO>& lstCategory);
	
	/// @brief 获取子分类列表（兄弟节点）
	/// @param[in] cat_id 分类 ID
	/// @param[in] lstCat 完整分类列表
	/// @param[out] lstSub 子分类 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取指定分类的兄弟分类（同一父分类下的其他分类）
	long get_sub_categories_siblings(string cat_id, list<CATEGORY_DTO> lstCat, list<CATEGORY_DTO>& lstSub);
	
	/// @brief 获取菜单栏分类导航 HTML
	/// @param[in] cat_id 当前分类 ID
	/// @param[in] nb_cat_images 分类图片数量
	/// @param[in] lstCategroy 分类 DTO 列表指针
	/// @return 菜单栏 HTML 字符串
	/// @note 生成侧边栏或顶部菜单的完整 HTML
	string get_menubar_categories(string cat_id, uint32_t nb_cat_images, list<CATEGORY_DTO>* lstCategroy);
	
	/// @brief 处理分类列表
	/// @param[in,out] lstCategroy 分类 DTO 列表指针（可能被修改）
	/// @return 0: 成功; 其他: 错误码
	/// @note 对分类列表进行预处理（排序、过滤等）
	long proc_categories(list<CATEGORY_DTO>* lstCategroy);

private:
	/// @brief 获取菜单栏分类列表项 HTML（递归）
	/// @param[in] cat_id 分类 ID
	/// @param[in] categroyDto 分类 DTO
	/// @param[in] lstCat 完整分类列表
	/// @return 分类列表项 LI 元素 HTML 字符串
	/// @note 递归生成分类树形结构的 LI 元素
	/// @warning 递归深度过大可能导致栈溢出
	string get_menubar_categories_li(string cat_id, CATEGORY_DTO categroyDto, list<CATEGORY_DTO> lstCat);
	
	/// @brief 获取菜单栏分类列表项内容 HTML
	/// @param[in] categroyDto 分类 DTO
	/// @return 列表项内容 HTML 字符串（包含图标、名称、数量等）
	/// @note 生成单个分类的显示内容
	string get_menubar_categories_li_content(CATEGORY_DTO categroyDto);
	
	/// @brief 获取选中状态的分类 HTML
	/// @param[in] cat_id 当前选中分类 ID
	/// @param[in] lstCat 完整分类列表
	/// @return 选中状态的分类 HTML 字符串
	/// @note 生成当前选中分类的高亮显示
	string get_sel_categories(string cat_id, list<CATEGORY_DTO> lstCat);
	
	/// @brief 获取选中分类的子分类 HTML
	/// @param[in] cat_id 当前选中分类 ID
	/// @param[in] lstCat 完整分类列表
	/// @return 子分类 HTML 字符串
	/// @note 生成选中分类的子分类列表
	string get_sel_category_substring(string cat_id, list<CATEGORY_DTO> lstCat);

private:
	/// @brief 按排序等级缓存的分类列表
	list<CATEGORY_DTO> lstCategory_order_by_rank_;
	
	/// @brief 按根目录顺序缓存的分类列表
	list<CATEGORY_DTO> lstCategory_order_by_root_dir_;
	
	/// @brief 临界区对象，用于多线程同步
	CRITICAL_SECTION _cs;
};
