///////////////////////////////////////////////////////////////////////
/// @file lang3.h
/// @brief 多语言支持类定义(第三版)
/// @details 定义 CLang 类,使用 Singleton 模式实现多语言翻译功能,
///          支持从资源文件读取翻译内容并应用到界面文本
/// @author PhotoNest Team
/// @date 2024
/// @version 1.0
/// @note 本文件是 inscl 模块的多语言支持头文件
///////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <map>
#include <list>
#include "Singleton.h"

using namespace std;

class CLang : public Singleton<CLang>
{
	friend class Singleton<CLang>;
protected:
	CLang(void);
	~CLang(void);
public:
	bool proc_lang(UINT id);
	bool translate(string& strText);
private:
	long read_htm(HINSTANCE hrcInst, UINT cr_id, string& szhtml);
	void proc_html(string common);
	void status_change(string& fout, int& pattern, char& c, char& last);
private:
	map<string, string> _langMap;
};
