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
