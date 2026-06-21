#pragma once

#define SQLITE_HAS_CODEC

#include <string>
#include <list>
#include "stmdef.h"

#include "sqlite3.h"
#include "Singleton.h"

using namespace std;

class Cbasedal : public Singleton<Cbasedal>
{
	friend class Singleton<Cbasedal>;
protected:
	Cbasedal(void);
	~Cbasedal(void);

	// TODO: add your methods here.
public:
	sqlite3 * getDB();
	int refresh_db(string alb, wstring logdir);
	long backupDb(string alb);
	void copy2appData(string alb);
	void upgrade();
	wstring _appDataDir;

private:
	long loadOrSaveDb(sqlite3 *pInMemory, const char *zFilename, int isSave);
	BOOL my_DeleteFile(wstring dst);

private:
	sqlite3 * _pDB;
};
