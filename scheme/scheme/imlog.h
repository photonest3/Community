#pragma once

#include <string>
#include "Singleton.h"


using namespace std;

class Cimlog : public Singleton<Cimlog> 
{
	friend class Singleton<Cimlog>;
protected:
	Cimlog();
	~Cimlog(void);
		 
public:
	void setLogName(wstring szLog);
	void LogToFile(string text, int lv = 1);
	
private:
	 wstring _szFilename;
};
