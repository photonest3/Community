#pragma once

#include "stmdef.h"
#include "tinyxml/tinyxml.h"
#include "Singleton.h"

class CLang : public Singleton<CLang>
{
	friend class Singleton<CLang>;
protected:
	CLang(void);
	~CLang(void);

public:
	long reset(string ty);
	long refresh_language();

	int ParseLangXML(string ty, string szhtml);
	long getOneData(string id, string &value);
	bool translate(string &strText, string intVal, string szVal);
	bool translate(string &strText);
	string _language;
	
private:
	bool proc_lang(wstring lang);
	long Parse(TiXmlNode* parent, TEMPL_DTO &dto);
	list<TiXmlNode*> GetElementsByTagName(TiXmlNode* parent, string name, list<TiXmlNode*> &list);
	string _GetNodeValue(TiXmlNode* node);
	long Read_file(wstring filename, string &szhtml);
	bool Judge(string str, list<string> &lstKey);

private:
	map<string, string> _langMap;
	list<TEMPL_DTO> _listTempDto;

};
