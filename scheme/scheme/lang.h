///////////////////////////////////////////////////////////////////////////////
// 文件名: lang.h
// 模块: scheme/scheme
// 功能描述: 多语言支持类
//           使用 TinyXML 解析语言文件，提供多语言翻译功能
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-03-10  PhotoNest Team  添加语言切换功能
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "tinyxml/tinyxml.h"
#include "Singleton.h"

/// @brief 多语言支持类
/// @details 使用 TinyXML 解析 XML 语言文件，提供多语言翻译功能。
///          支持动态切换语言、解析语言文件、翻译字符串等。
///          采用单例模式确保全局唯一实例。
/// @note 语言文件使用 XML 格式，支持占位符替换
/// @warning 确保语言文件路径正确，否则翻译会失败
class CLang : public Singleton<CLang>
{
	friend class Singleton<CLang>;

protected:
	/// @brief 构造函数
	/// @details 初始化多语言支持对象
	CLang(void);
	
	/// @brief 析构函数
	/// @details 清理资源
	~CLang(void);

public:
	/// @brief 重置语言数据
	/// @param[in] ty 语言类型（"zh", "en" 等）
	/// @return 0: 成功; 其他: 错误码
	/// @note 清空当前语言数据，准备加载新语言
	long reset(string ty);
	
	/// @brief 刷新语言数据
	/// @return 0: 成功; 其他: 错误码
	/// @note 重新从语言文件加载语言数据
	long refresh_language();

	/// @brief 解析语言 XML 文件
	/// @param[in] ty 语言类型（"zh", "en" 等）
	/// @param[in] szhtml HTML 模板字符串（可能包含语言标记）
	/// @return 0: 成功; 其他: 错误码
	/// @note 解析语言 XML 文件，提取翻译数据
	/// @see TinyXML 文档
	int ParseLangXML(string ty, string szhtml);
	
	/// @brief 获取单个翻译值
	/// @param[in] id 翻译 ID
	/// @param[out] value 翻译值（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 根据 ID 获取对应的翻译字符串
	long getOneData(string id, string &value);
	
	/// @brief 翻译字符串（带参数）
	/// @param[in,out] strText 待翻译字符串（输出时被修改）
	/// @param[in] intVal 整数值参数
	/// @param[in] szVal 字符串参数
	/// @return 翻译是否成功
	/// @note 翻译字符串并替换占位符（如 %d, %s）
	bool translate(string &strText, string intVal, string szVal);
	
	/// @brief 翻译字符串（无参数）
	/// @param[in,out] strText 待翻译字符串（输出时被修改）
	/// @return 翻译是否成功
	/// @note 直接翻译字符串，不支持占位符
	bool translate(string &strText);
	
	/// @brief 当前语言
	/// @note 存储当前语言类型（"zh", "en" 等）
	string _language;
	
private:
	/// @brief 处理语言文件
	/// @param[in] lang 语言文件路径（宽字符）
	/// @return 处理是否成功
	/// @note 加载并解析语言文件
	bool proc_lang(wstring lang);
	
	/// @brief 解析 XML 节点
	/// @param[in] parent 父节点指针
	/// @param[in,out] dto 模板 DTO（输出时被修改）
	/// @return 0: 成功; 其他: 错误码
	/// @note 递归解析 XML 节点，提取翻译数据
	long Parse(TiXmlNode* parent, TEMPL_DTO &dto);
	
	/// @brief 根据标签名获取元素列表
	/// @param[in] parent 父节点指针
	/// @param[in] name 标签名
	/// @param[out] list 元素列表（输出）
	/// @return 元素数量
	/// @note 获取指定父节点下所有指定标签名的子节点
	list<TiXmlNode*> GetElementsByTagName(TiXmlNode* parent, string name, list<TiXmlNode*> &list);
	
	/// @brief 获取节点值
	/// @param[in] node 节点指针
	/// @return 节点值字符串
	/// @note 获取 XML 节点的文本内容
	string _GetNodeValue(TiXmlNode* node);
	
	/// @brief 读取文件内容
	/// @param[in] filename 文件路径（宽字符）
	/// @param[out] szhtml 文件内容字符串（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 读取语言文件的完整内容
	long Read_file(wstring filename, string &szhtml);
	
	/// @brief 判断字符串是否在列表中
	/// @param[in] str 待判断字符串
	/// @param[in] lstKey 关键字列表
	/// @return 是否存在
	/// @note 检查字符串是否包含列表中的任意关键字
	bool Judge(string str, list<string> &lstKey);

private:
	/// @brief 语言映射表（ID -> 翻译）
	map<string, string> _langMap;
	
	/// @brief 模板 DTO 列表（缓存）
	list<TEMPL_DTO> _listTempDto;
	
};
