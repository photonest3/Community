#include "StdAfx.h"

//void CImBase::get_open_list(wstring pFileExt, string &flist)
//{
//	flist = "";
//	string open_flist = "";
//	CLang::Instance().getOneData("open_flist", open_flist);
//
//	IEnumAssocHandlers *peah = NULL;
//	IAssocHandler *pah = NULL;
//	HRESULT hr = S_OK;
//	hr = SHAssocEnumHandlers(pFileExt.c_str(), ASSOC_FILTER_RECOMMENDED, &peah);
//	if (SUCCEEDED(hr))
//	{
//		ULONG celtFetched = 0;
//		while (SUCCEEDED(peah->Next(1, &pah, &celtFetched)))
//		{
//			if (pah)
//			{
//				WCHAR *pFileName = NULL;
//				WCHAR *pFileDescription = NULL;
//				if (SUCCEEDED(pah->GetName(&pFileName))
//					&& SUCCEEDED(pah->GetUIName(&pFileDescription)))
//				{
//					wstring t = pFileName;
//					size_t delim = t.find(L"mspaint");
//					if (delim != std::string::npos)
//					{
//						wchar_t buf[MAX_PATH] = { 0 };
//						GetLongPathName(pFileName, buf, MAX_PATH);
//
//						string temp = open_flist;
//						ReplaceOne(temp, "{$desc}", _w2u(pFileDescription));
//						ReplaceOne(temp, "{$fname}", _w2u(buf));
//						flist += temp;
//						break;
//					}
//				}
//				pah->Release();
//				pah = NULL;
//			}
//			else
//			{
//				break;
//			}
//		}
//		peah->Release();
//	}
//}
//


//if (_hEvent != INVALID_HANDLE_VALUE)
//{
//	WaitForSingleObject(_hEvent, INFINITE);
//}
//else
//{
	//if (_hEvent != INVALID_HANDLE_VALUE)
	//{
	//	CloseHandle(_hEvent);
	//}

	//_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

				/*
				void proc_syn(string mk, wstring fpath)
				{
					wchar_t src[MAX_PATH];
					memset(src, 0, MAX_PATH * sizeof(wchar_t));
					GetFullPathName(fpath.c_str(), MAX_PATH, src, NULL);

					struct _stat32  info;
					int result = _wstat32(src, &info);
					if (result == 0)
					{
						__time32_t t = atol(mk.c_str());
						if (info.st_mtime != t)
						{
							//LOG(INFO) << "t: " << t << "st_mtime: " << info.st_mtime;;

							string url = "c.html?act=c.syn_files&fpath=";
							url += CefString(src);
							char *szResponse = 0;
							char *szMimeType = 0;

							mygetfunc(url.c_str(), "", &szResponse, &szMimeType);
							LocalFree(szResponse);
						}
					}
				}
				*/
				
				
				
								/*
								FILETIME fCreateTime, fAccessTime, fWriteTime;
								GetFileTime(_hFile, &fCreateTime, &fAccessTime, &fWriteTime);
								time_t filetime = get_file_time(fWriteTime);

								string if_modified_since = "";
								it = header.find("If-Modified-Since");
								if (it != header.end())
								{
								if_modified_since = it->second;

								time_t timestamp;
								str2time(if_modified_since.c_str(), &timestamp);
								if (filetime <= timestamp)
								{
								_Status = 304;
								_content = "Not Modified";
								_mimeType = "text/plain; charset=utf-8";


								CloseHandle(_hFile);
								_hFile = NULL;
								return;
								}
								}


								//Accept-Ranges: bytes
								//Cache-Control: max-age=315360000
								//Connection: Keep-Alive
								//Content-Length: 0
								//Content-Type: image/gif
								//Date: Sat, 08 Jun 2019 06:41:03 GMT
								//Etag: "0-0509a8580"
								//Expires: Tue, 05 Jun 2029 06:41:03 GMT

								//pair<std::string, std::string> Cache("Cache-Control", "public,must-revalidate,max-age=315360000");
								//_header.insert(Cache);
								//pair<std::string, std::string> Expires("Expires", "Tue, 05 Jun 2029 06:41:03 GMT");
								//_header.insert(Expires);


								//pair<std::string, std::string> Accept("Accept-Ranges", "bytes");
								//_header.insert(Accept);

								//pair<std::string, std::string> Connection("Connection", "Keep-Alive");
								//_header.insert(Connection);

								//pair<std::string, std::string> Alive("Keep-Alive", "timeout=5, max=100");
								//_header.insert(Alive);

								//time_t current_time;
								//time(&current_time); // get current time 

								//char buf0[1024];
								//memset(buf0, 0, 1024);
								//time2str(&current_time, buf0, 1024);
								//pair<std::string, std::string> Date("Date", buf0);
								//_header.insert(Date);

								//pair<std::string, std::string>Server("Server", "Apache/2.4.23 (Win32) OpenSSL/1.0.2j PHP/5.4.45");
								//_header.insert(Server);

								//char buf[1024];
								//memset(buf, 0, 1024);
								//time2str(&filetime, buf, 1024);
								//pair<std::string, std::string>Last("Last-Modified", buf);
								//_header.insert(Last);

								//
								//Accept-Ranges: bytes
								//Connection: Keep-Alive
								//Content-Length: 95957
								//Content-Type: application/javascript
								//Date: Sat, 08 Jun 2019 05:49:57 GMT
								//ETag: "176d5-58614bd99e280"
								//Keep-Alive: timeout=5, max=100
								//Last-Modified: Tue, 09 Apr 2019 08:28:42 GMT
								//Server: Apache/2.4.23 (Win32) OpenSSL/1.0.2j PHP/5.4.45
								//
								*/

				
				
				
				
				
				
	
/*
#define TM_YEAR_BASE 1900
//see: https://tools.ietf.org/html/rfc2616#section-3.3.1
//example: Sun, 06 Nov 1994 08:49:37 GMT
static const char *RFC_1123_DATE = "%a, %d %b %Y %T GMT";

int time2str(const time_t *timestamp, char *buf, size_t size) {
	assert(timestamp);
	assert(buf);
	assert(size > 31);
	struct tm gmt_timestamp;
	gmtime_s(&gmt_timestamp, timestamp);

	strftime(buf, size - 1, RFC_1123_DATE, &gmt_timestamp);

	return 0;
}

//OBSOLETE formats for backward compatibility
//example: Sunday, 06-Nov-94 08:49:37 GMT
static const char *RFC_850_DATE = "%A, %d-%b-%y %T GMT";
//example: Sun Nov  6 08:49:37 1994
static const char *ANSI_C_DATE = "%a %d %e %T %Y";

int str2time(const char *buf, time_t *timestamp) {
	assert(buf);
	assert(timestamp);

	struct tm tm;
	char *r;
	const char **fmt;
	const char *DATE_FORMATS[] = {
		RFC_1123_DATE, RFC_850_DATE, ANSI_C_DATE, NULL
	};

	//try each format
	for(fmt = DATE_FORMATS; *fmt; ++fmt) {
		memset(&tm, 0, sizeof(struct tm));
		r = strptime(buf, *fmt, &tm);
		if(r && *r == '\0') {
			//timegm is nonstandard
			*timestamp = _mkgmtime(&tm);
			if(*timestamp != -1) {
				return 0;
			}
		}
	}
	return -1;
}

const char * strp_weekdays[] =
{ "sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday" };
const char * strp_monthnames[] =
{ "january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december" };

bool strp_atoi(const char * & s, int & result, int low, int high, int offset)
{
	bool worked = false;
	char * end;
	unsigned long num = strtoul(s, &end, 10);
	if (num >= (unsigned long)low && num <= (unsigned long)high)
	{
		result = (int)(num + offset);
		s = end;
		worked = true;
	}
	return worked;
}

char * strptime(const char *s, const char *format, struct tm *tm)
{
	bool working = true;
	while (working && *format && *s)
	{
		switch (*format)
		{
		case '%':
		{
			++format;
			switch (*format)
			{
			case 'a':
			case 'A': // weekday name
				tm->tm_wday = -1;
				working = false;
				for (size_t i = 0; i < 7; ++i)
				{
					size_t len = strlen(strp_weekdays[i]);
					if (!strnicmp(strp_weekdays[i], s, len))
					{
						tm->tm_wday = i;
						s += len;
						working = true;
						break;
					}
					else if (!strnicmp(strp_weekdays[i], s, 3))
					{
						tm->tm_wday = i;
						s += 3;
						working = true;
						break;
					}
				}
				break;
			case 'b':
			case 'B':
			case 'h': // month name
				tm->tm_mon = -1;
				working = false;
				for (size_t i = 0; i < 12; ++i)
				{
					size_t len = strlen(strp_monthnames[i]);
					if (!strnicmp(strp_monthnames[i], s, len))
					{
						tm->tm_mon = i;
						s += len;
						working = true;
						break;
					}
					else if (!strnicmp(strp_monthnames[i], s, 3))
					{
						tm->tm_mon = i;
						s += 3;
						working = true;
						break;
					}
				}
				break;
			case 'd':
			case 'e': // day of month number
				working = strp_atoi(s, tm->tm_mday, 1, 31, 0);
				break;
			case 'D': // %m/%d/%y
			{
				const char * s_save = s;
				working = strp_atoi(s, tm->tm_mon, 1, 12, -1);
				if (working && *s == '/')
				{
					++s;
					working = strp_atoi(s, tm->tm_mday, 1, 31, 0);
					if (working && *s == '/')
					{
						++s;
						working = strp_atoi(s, tm->tm_year, 0, 99, 0);
						if (working && tm->tm_year < 69)
							tm->tm_year += 100;
					}
				}
				if (!working)
					s = s_save;
			}
			break;
			case 'H': // hour
				working = strp_atoi(s, tm->tm_hour, 0, 23, 0);
				break;
			case 'I': // hour 12-hour clock
				working = strp_atoi(s, tm->tm_hour, 1, 12, 0);
				break;
			case 'j': // day number of year
				working = strp_atoi(s, tm->tm_yday, 1, 366, -1);
				break;
			case 'm': // month number
				working = strp_atoi(s, tm->tm_mon, 1, 12, -1);
				break;
			case 'M': // minute
				working = strp_atoi(s, tm->tm_min, 0, 59, 0);
				break;
			case 'n': // arbitrary whitespace
			case 't':
				while (isspace((int)*s))
					++s;
				break;
			case 'p': // am / pm
				if (!strnicmp(s, "am", 2))
				{ // the hour will be 1 -> 12 maps to 12 am, 1 am .. 11 am, 12 noon 12 pm .. 11 pm
					if (tm->tm_hour == 12) // 12 am == 00 hours
						tm->tm_hour = 0;
					s += 2;
				}
				else if (!strnicmp(s, "pm", 2))
				{
					if (tm->tm_hour < 12) // 12 pm == 12 hours
						tm->tm_hour += 12; // 1 pm -> 13 hours, 11 pm -> 23 hours
					s += 2;
				}
				else
					working = false;
				break;
			case 'r': // 12 hour clock %I:%M:%S %p
			{
				const char * s_save = s;
				working = strp_atoi(s, tm->tm_hour, 1, 12, 0);
				if (working && *s == ':')
				{
					++s;
					working = strp_atoi(s, tm->tm_min, 0, 59, 0);
					if (working && *s == ':')
					{
						++s;
						working = strp_atoi(s, tm->tm_sec, 0, 60, 0);
						if (working && isspace((int)*s))
						{
							++s;
							while (isspace((int)*s))
								++s;
							if (!strnicmp(s, "am", 2))
							{ // the hour will be 1 -> 12 maps to 12 am, 1 am .. 11 am, 12 noon 12 pm .. 11 pm
								if (tm->tm_hour == 12) // 12 am == 00 hours
									tm->tm_hour = 0;
							}
							else if (!strnicmp(s, "pm", 2))
							{
								if (tm->tm_hour < 12) // 12 pm == 12 hours
									tm->tm_hour += 12; // 1 pm -> 13 hours, 11 pm -> 23 hours
							}
							else
								working = false;
						}
					}
				}
				if (!working)
					s = s_save;
			}
			break;
			case 'R': // %H:%M
			{
				const char * s_save = s;
				working = strp_atoi(s, tm->tm_hour, 0, 23, 0);
				if (working && *s == ':')
				{
					++s;
					working = strp_atoi(s, tm->tm_min, 0, 59, 0);
				}
				if (!working)
					s = s_save;
			}
			break;
			case 'S': // seconds
				working = strp_atoi(s, tm->tm_sec, 0, 60, 0);
				break;
			case 'T': // %H:%M:%S
			{
				const char * s_save = s;
				working = strp_atoi(s, tm->tm_hour, 0, 23, 0);
				if (working && *s == ':')
				{
					++s;
					working = strp_atoi(s, tm->tm_min, 0, 59, 0);
					if (working && *s == ':')
					{
						++s;
						working = strp_atoi(s, tm->tm_sec, 0, 60, 0);
					}
				}
				if (!working)
					s = s_save;
			}
			break;
			case 'w': // weekday number 0->6 sunday->saturday
				working = strp_atoi(s, tm->tm_wday, 0, 6, 0);
				break;
			case 'Y': // year
				working = strp_atoi(s, tm->tm_year, 1900, 65535, -1900);
				break;
			case 'y': // 2-digit year
				working = strp_atoi(s, tm->tm_year, 0, 99, 0);
				if (working && tm->tm_year < 69)
					tm->tm_year += 100;
				break;
			case '%': // escaped
				if (*s != '%')
					working = false;
				++s;
				break;
			default:
				working = false;
			}
		}
		break;
		case ' ':
		case '\t':
		case '\r':
		case '\n':
		case '\f':
		case '\v':
			// zero or more whitespaces:
			while (isspace((int)*s))
				++s;
			break;
		default:
			// match character
			if (*s != *format)
				working = false;
			else
				++s;
			break;
		}
		++format;
	}
	return (working ? (char *)s : 0);
}
*/
			
/*
//converts timestamp to string of form: Wed, 21 Oct 2015 07:28:00 GMT
//returns -1 or error
//int time2str(const time_t *timestamp, char *buf, size_t size);

//inverse of time2str, tries multiple timestatmp formats
int str2time(const char *buf, time_t *timestamp);

bool strp_atoi(const char * & s, int & result, int low, int high, int offset);

char * strptime(const char *buf, const char *fmt, struct tm *tm);
*/
				

/*
long CHtmlgetAdmin::syn_init()
{
	

HANDLE h = (HANDLE)_beginthreadex(NULL, 0, handle_change, this, NULL, NULL);
SetThreadPriority(h, THREAD_PRIORITY_IDLE);
CloseHandle(h);

return 0;
}

UINT __stdcall CHtmlgetAdmin::handle_change(LPVOID pParam)
{
CHtmlgetAdmin* pThis = (CHtmlgetAdmin*)pParam;

wstring szPath = get_module_path(NULL);
szPath = szPath + L"\\data\\public\\_upload";

list<string> lstSubs;

CHtmlgetFolder::Instance().GetSubFolders(szPath, lstSubs);

wstring synroot = CDataSrv::Instance()._curr_synroot;
string szPath2 = _w2u(synroot);

list<CATEGORY_DTO> lstCategory;
Cchatdal::Instance().get_order_categories("a.uppercats", lstCategory);

list<CATEGORY_DTO>::iterator iter;
for (iter = lstCategory.begin(); iter != lstCategory.end(); iter++)
{
if (iter->dir != "")
{
string temp = szPath2 + iter->dir;
Replace(temp, "/", "\\");
lstSubs.push_back(temp);
}
}

if (lstSubs.empty())
{
return 0;
}

int numThreads = lstSubs.size();
//HANDLE* handle = new HANDLE[len];
HANDLE handle[2000] = { 0 };

list<string>::iterator it;
int i = 0;
for (it = lstSubs.begin(); it != lstSubs.end(); it++)
{
ARGS_DTO* pParam = (ARGS_DTO*)malloc(sizeof(ARGS_DTO));
pParam->pThis = pThis;

memset(pParam->szPath, 0, MAX_PATH);
memset(pParam->szType, 0, MAX_PATH);
strcpy_s(pParam->szPath, it->c_str());


handle[i] = (HANDLE)_beginthreadex(NULL, 0, handle_change_files1, (LPVOID)pParam, NULL, NULL);
SetThreadPriority(handle[i], THREAD_PRIORITY_IDLE);
i++;
}

int tempNumThreads = numThreads;
int tempMax = 0;
while (tempNumThreads >= MAXIMUM_WAIT_OBJECTS)
{
tempNumThreads -= MAXIMUM_WAIT_OBJECTS;
WaitForMultipleObjects(MAXIMUM_WAIT_OBJECTS, &handle[tempMax], TRUE, INFINITE);
tempMax += MAXIMUM_WAIT_OBJECTS;
}
WaitForMultipleObjects(tempNumThreads, &handle[tempMax], TRUE, INFINITE);


//WaitForMultipleObjects(len, handle, TRUE, INFINITE);

for (i = 0; i < numThreads; i++)
{
CloseHandle(handle[i]);
}
//delete[] handle;

//if (pThis->_thread_nb == 0)
{
wstring szPath = get_module_path(NULL);
szPath = szPath + L"\\data\\public\\_upload\\1.txt";
DWORD dwWritten = 0;

HANDLE hWrite = CreateFileW(szPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
if (hWrite != INVALID_HANDLE_VALUE)
{
WriteFile(hWrite, "0", 1, &dwWritten, NULL);
CloseHandle(hWrite);
}

DeleteFile(szPath.c_str());
}

return 0;
}


UINT __stdcall CHtmlgetAdmin::handle_size(LPVOID pParam)
{
CHtmlgetAdmin* pThis = (CHtmlgetAdmin*)pParam;

rtrim(pThis->_ids, ",");

list<IMAGE_DTO> lstImageDto;
Cimagedal::Instance().get_images_by_ids(pThis->_ids, lstImageDto);

wstring szPath = get_module_path(NULL);
string albumName = CDataSrv::Instance()._albumName;

LONG64 filesize = 0;

//if (CDataSrv::Instance()._public_size != 0 && albumName == "public"
//	|| CDataSrv::Instance()._private_size != 0 && albumName == "private")
{
wstring synroot = CDataSrv::Instance()._curr_synroot;
string szPath2 = _w2u(synroot);

list<IMAGE_DTO>::iterator iter;
for (iter = lstImageDto.begin(); iter != lstImageDto.end(); iter++)
{
string ori = albumName + iter->ori;
wstring szFile = szPath + L"\\data\\" + _u2w(ori);
string::size_type  pos = iter->ori.find("/galleries/");
if (pos != string::npos)
{
string t = iter->ori;
ReplaceOne(t, "/galleries", "");
szFile = CDataSrv::Instance()._curr_synroot + _u2w(t);
}

Replace(szFile, L"/", L"\\");

struct _stat32  info;
int result = _wstat32(szFile.c_str(), &info);
if (result == 0)
{
filesize += info.st_size / 1024;
}
}
}

//if (CDataSrv::Instance()._public_size == 0)
//{
//	wstring szFile = szPath + L"\\data\\public\\";
//	LONG64 filesize0 = 0;
//	CHtmlgetFolder::Instance().GetFolderSize(szFile, filesize0);

//	CDataSrv::Instance()._public_size = filesize0;
//}

//if (CDataSrv::Instance()._private_size == 0)
//{
//	wstring szFile = szPath + L"\\data\\public\\";
//	LONG64 filesize0 = 0;
//	CHtmlgetFolder::Instance().GetFolderSize(szFile, filesize0);

//	CDataSrv::Instance()._private_size = filesize0;
//}

list<CONFIG_DTO> lstConfig;
CONFIG_DTO si;
if (albumName == "public")
{
si.param = "public_size";
si.value = Int2Str(CDataSrv::Instance()._public_size + filesize);
}
else
{
si.param = "private_size";
si.value = Int2Str(CDataSrv::Instance()._private_size + filesize);
}
lstConfig.push_back(si);

Cchatdal::Instance().add_configs(lstConfig);


return 0;
}


UINT __stdcall CHtmlgetAdmin::handle_change_files1(LPVOID pParam)
{

ARGS_DTO* pThis = (ARGS_DTO*)pParam;
string strPath = pThis->szPath;


wstring t = _u2w(strPath);


list<string> lstFile;
CHtmlgetFolder::Instance().GetFolderPics(CDataSrv::Instance()._ft, t, lstFile);

string synroot = _w2u(CDataSrv::Instance()._curr_synroot);
string szPath = get_module_pathA(NULL) + "\\data\\public";

list<string>::iterator it;
string files = "";
for (it = lstFile.begin(); it != lstFile.end(); it++)
{
string szFile = *it;
string::size_type  pos = szFile.find(synroot);
if (pos != string::npos)
{
ReplaceOne(szFile, synroot, "/galleries");
}
else
{
ReplaceOne(szFile, szPath, "");
}

Replace(szFile, "\\", "/");

files += string("|") + *it + string("|,");
}
rtrim(files, ",");

list<IMAGE_DTO> lstImageDto;
Cimagedal::Instance().get_images_by_url(files, lstImageDto);

if (!lstImageDto.empty())
{
//syn_files(lstImageDto);

//IMAGE_DTO dto = lstImageDto.front();
//CHtmlgetIndexProc::Instance().update_cache_image(dto);
}

//EnterCriticalSection(&pThis->_cs);
//pThis->_thread_nb--;

//LeaveCriticalSection(&pThis->_cs);


return 0;
}


long CHtmlgetAdmin::proc_folder_sizes()
{
wstring szPath = get_module_path(NULL);

if (CDataSrv::Instance()._public_size == 0)
{
wstring szFile = szPath + L"\\data\\public";
get_sizes("public_size", szFile);
}

if (CDataSrv::Instance()._private_size == 0)
{
wstring szFile = szPath + L"\\data\\private";
get_sizes("private_size", szFile);
}
if (CDataSrv::Instance()._galleries_size == 0)
{
wstring szFile = szPath + L"\\data\\public\\_data\\galleries";
get_sizes("galleries_size", szFile);
}

return 0;
}

long CHtmlgetAdmin::get_sizes(string type, wstring szPath)
{
string szPathA = _w2u(szPath);

list<string>::iterator it;
list<string> lstSubs;
if (type == "private_size")
{
CHtmlgetFolder::Instance().GetOneFolders(szPath, lstSubs);
}
else if (type == "public_size")
{
CHtmlgetFolder::Instance().GetOneFolders(szPath + L"\\_upload", lstSubs);
CHtmlgetFolder::Instance().GetOneFolders(szPath + L"\\_data", lstSubs);

string temp = szPathA + "\\db";
lstSubs.push_back(temp);

temp = szPathA + "\\_data\\galleries";

for (it = lstSubs.begin(); it != lstSubs.end(); it++)
{
if (*it == temp)
{
lstSubs.erase(it);
break;
}
}
}
else
{
list<CATEGORY_DTO> lstCategory;
Cchatdal::Instance().get_order_categories("a.uppercats", lstCategory);

list<CATEGORY_DTO>::iterator iter;
for (iter = lstCategory.begin(); iter != lstCategory.end(); iter++)
{
if (iter->dir != "")
{
string temp = szPathA + iter->dir;
Replace(temp, "/", "\\");
lstSubs.push_back(temp);
}
}
}

if (lstSubs.empty())
{
return 0;
}

int numThreads = lstSubs.size();
//HANDLE* handle = new HANDLE[len];
HANDLE handle[2000] = { 0 };

int i = 0;
for (it = lstSubs.begin(); it != lstSubs.end(); it++)
{
ARGS_DTO* pParam = (ARGS_DTO*)malloc(sizeof(ARGS_DTO));
pParam->pThis = this;
memset(pParam->szPath, 0, MAX_PATH);
memset(pParam->szType, 0, MAX_PATH);

strcpy_s(pParam->szPath, it->c_str());
strcpy_s(pParam->szType, type.c_str());

handle[i] = (HANDLE)_beginthreadex(NULL, 0, handle_get_sizes, (LPVOID)pParam, NULL, NULL);
SetThreadPriority(handle, THREAD_PRIORITY_IDLE);
i++;
}

int tempNumThreads = numThreads;
int tempMax = 0;
while (tempNumThreads >= MAXIMUM_WAIT_OBJECTS)
{
tempNumThreads -= MAXIMUM_WAIT_OBJECTS;
WaitForMultipleObjects(MAXIMUM_WAIT_OBJECTS, &handle[tempMax], TRUE, INFINITE);
tempMax += MAXIMUM_WAIT_OBJECTS;
}
WaitForMultipleObjects(tempNumThreads, &handle[tempMax], TRUE, INFINITE);



//WaitForMultipleObjects(len, handle, TRUE, INFINITE);

for (i = 0; i < numThreads; i++)
{
CloseHandle(handle[i]);
}

//delete[] handle;

LONG64 filesize = 0;
if (type == "private_size")
{
filesize = CDataSrv::Instance()._private_size;
}
else if (type == "public_size")
{
filesize = CDataSrv::Instance()._public_size;
}
else
{
filesize = CDataSrv::Instance()._galleries_size;
}

list<CONFIG_DTO> lstConfig;
CONFIG_DTO si;
si.param = type;
si.value = LONG642Str(filesize);
lstConfig.push_back(si);

Cchatdal::Instance().add_configs(lstConfig);
return S_OK;
}


UINT __stdcall CHtmlgetAdmin::handle_get_sizes(LPVOID pParam)
{
ARGS_DTO* ptr = (ARGS_DTO*)pParam;
CHtmlgetAdmin* pThis = (CHtmlgetAdmin*)ptr->pThis;
string type = ptr->szType;

LONG64 filesize = 0;
CHtmlgetFolder::Instance().GetFolderSize(_u2w(ptr->szPath), filesize);

EnterCriticalSection(&pThis->_cs);

if (type == "private_size")
{
CDataSrv::Instance()._private_size += filesize;
}
else if (type == "public_size")
{
CDataSrv::Instance()._public_size += filesize;
}
else
{
CDataSrv::Instance()._galleries_size += filesize;
}

LeaveCriticalSection(&pThis->_cs);


return 0;
}

*/

/*
long CHtmlgetFolder::GetSubFoldersW(wstring strPath, list<wstring > &lstSubs)
{
wchar_t *buf0 = (wchar_t*)malloc(1024 * sizeof(wchar_t));
if (buf0 == NULL)
{
return 0;
}
wchar_t *buf1 = (wchar_t*)malloc(1024 * sizeof(wchar_t));
if (buf1 != NULL)
{
memset(buf1, 0, 1024 * sizeof(wchar_t));
swprintf_s(buf1, 1024, L"%s\\*", strPath.c_str());
}

WIN32_FIND_DATAW ffd;
HANDLE hFind = FindFirstFile(buf1, &ffd);
if (hFind != INVALID_HANDLE_VALUE)
{
if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
{
if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
{
memset(buf0, 0, 1024 * sizeof(wchar_t));
swprintf_s(buf0, 1024, L"%s\\%s", strPath.c_str(), ffd.cFileName);
lstSubs.push_back(buf0);
GetSubFoldersW(buf0, lstSubs);
}
else
{
//filesize += ffd.nFileSizeLow / 1024;
}
}

while (FindNextFileW(hFind, &ffd))
{
if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
{
if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
{
memset(buf0, 0, 1024 * sizeof(wchar_t));
swprintf_s(buf0, 1024, L"%s\\%s", strPath.c_str(), ffd.cFileName);

lstSubs.push_back(buf0);
GetSubFoldersW(buf0, lstSubs);
}
else
{
//filesize += ffd.nFileSizeLow / 1024;
}
}
}
FindClose(hFind);

}

free(buf1);

free(buf0);

return 0;
}
*/
/*
BOOL CHtmlgetFolder::GetDesktopFolders(wstring path, list<string>& saResult)
{
HRESULT hr;

LPSHELLFOLDER lpDesktop = NULL;
hr = SHGetDesktopFolder(&lpDesktop);
if (FAILED(hr))
{
return FALSE;
}

LPITEMIDLIST pidlWorkDir = NULL;
hr = lpDesktop->ParseDisplayName(NULL, NULL, (LPWSTR)path.c_str(), NULL, &pidlWorkDir, NULL);
if (FAILED(hr))
{
lpDesktop->Release();
return FALSE;
}

GetFoldersHelp(lpDesktop, pidlWorkDir, saResult);
lpDesktop->Release();

return TRUE;
}

BOOL CHtmlgetFolder::GetFoldersHelp(LPSHELLFOLDER lpDesktop, LPITEMIDLIST pidlWorkDir, list<string>& saResult)
{

IShellFolder* lpWorkDir = NULL;
HRESULT hr = lpDesktop->BindToObject(pidlWorkDir, NULL, IID_IShellFolder, (LPVOID*)&lpWorkDir);
if (FAILED(hr))
{
return FALSE;
}


//IStorage * pStg;
//STATSTG statstg;
//memset(&statstg, 0, sizeof(statstg));
//hr = lpDesktop->BindToStorage(pidlWorkDir, NULL, IID_IStorage, (LPVOID*)&pStg);
//if (SUCCEEDED(hr))
//{
//	IEnumSTATSTG *penum = NULL;
//	hr = pStg->EnumElements(NULL, NULL, NULL, &penum);
//	if (FAILED(hr)) throw L"failed IStorage::Enum";
//	hr = penum->Next(1, &statstg, 0);
//	while (S_OK == hr)
//	{
//		ULARGE_INTEGER cbSize = statstg.cbSize;
//		hr = penum->Next(1, &statstg, 0);
//	}

//	pStg->Release();
//}


LPENUMIDLIST lpEnum = NULL;
hr = lpWorkDir->EnumObjects(0, SHCONTF_FOLDERS, &lpEnum);
if (FAILED(hr))
{
lpWorkDir->Release();
return FALSE;
}

LPITEMIDLIST lpi = NULL;
ULONG ulwork = 0;
while (lpEnum->Next(1, &lpi, &ulwork) == S_OK)
{
SHFILEINFO sfi;
hr = SHGetFileInfo((LPCTSTR)lpi, 0, &sfi, sizeof(SHFILEINFO), SHGFI_DISPLAYNAME | SHGFI_PIDL);
if (SUCCEEDED(hr))
{
LPITEMIDLIST lpi2 = ILCombine(pidlWorkDir, lpi);
wchar_t buf[MAX_PATH] = { 0 };
SHGetPathFromIDList(lpi2, buf);

wstring p = buf;// path + wstring(L"\\") + sfi.szDisplayName;
wstring::size_type pos = p.find(L".zip");
if (pos == wstring::npos)
{
saResult.push_back(_w2u(p));
GetFoldersHelp(lpDesktop, lpi2, saResult);
}
ILFree(lpi2);

}
ILFree(lpi);
}

lpWorkDir->Release();
return TRUE;
}

BOOL CHtmlgetFolder::GetSizeHelp(LPSHELLFOLDER lpDesktop, LPITEMIDLIST pidlWorkDir, LONG64 &filesize)
{
IShellFolder* lpWorkDir = NULL;
HRESULT hr = lpDesktop->BindToObject(pidlWorkDir, NULL, IID_IShellFolder, (LPVOID*)&lpWorkDir);
if (FAILED(hr))
{
return FALSE;
}


IStorage * pStg;
STATSTG statstg;
memset(&statstg, 0, sizeof(statstg));
hr = lpDesktop->BindToStorage(pidlWorkDir, NULL, IID_IStorage, (LPVOID*)&pStg);
if (SUCCEEDED(hr))
{
IEnumSTATSTG *penum = NULL;
hr = pStg->EnumElements(NULL, NULL, NULL, &penum);
if (FAILED(hr)) throw L"failed IStorage::Enum";
hr = penum->Next(1, &statstg, 0);
while (S_OK == hr)
{
ULARGE_INTEGER cbSize = statstg.cbSize;
filesize += cbSize.LowPart / 1024;
hr = penum->Next(1, &statstg, 0);
}

pStg->Release();
}


LPENUMIDLIST lpEnum = NULL;
hr = lpWorkDir->EnumObjects(0, SHCONTF_FOLDERS, &lpEnum);
if (FAILED(hr))
{
lpWorkDir->Release();
return FALSE;
}

LPITEMIDLIST lpi = NULL;
ULONG ulwork = 0;
while (lpEnum->Next(1, &lpi, &ulwork) == S_OK)
{
SHFILEINFO sfi;
hr = SHGetFileInfo((LPCTSTR)lpi, 0, &sfi, sizeof(SHFILEINFO), SHGFI_DISPLAYNAME | SHGFI_PIDL);
if (SUCCEEDED(hr))
{
LPITEMIDLIST lpi2 = ILCombine(pidlWorkDir, lpi);
wchar_t buf[MAX_PATH] = { 0 };
SHGetPathFromIDList(lpi2, buf);

wstring p = buf;// path + wstring(L"\\") + sfi.szDisplayName;
wstring::size_type pos = p.find(L".zip");
if (pos == wstring::npos)
{
//saResult.push_back(_w2u(p));
GetSizeHelp(lpDesktop, lpi2, filesize);
}
ILFree(lpi2);

}
ILFree(lpi);
}

lpWorkDir->Release();
return TRUE;
}
long CHtmlgetFolder::GetFolderPics(FILETIME ft, wstring strPath, list<string > &lstFile)
{
strPath.erase(strPath.find_last_not_of(L"\\") + 1);

wchar_t *buf1 = (wchar_t*)malloc(1024 * sizeof(wchar_t));
if (buf1 != NULL)
{
memset(buf1, 0, 1024 * sizeof(wchar_t));
swprintf_s(buf1, 1024, L"%s\\*", strPath.c_str());
}

WIN32_FIND_DATAW ffd;
HANDLE hFind = FindFirstFile(buf1, &ffd);
if (hFind != INVALID_HANDLE_VALUE)
{
if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
{
if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
{
//memset(buf0, 0, 1024 * sizeof(wchar_t));
//swprintf_s(buf0, 1024, L"%s\\%s", strPath.c_str(), ffd.cFileName);
//GetFolderPics(buf0, vecffd);
}
else
{
if(CompareFileTime(&ft, &ffd.ftLastWriteTime) > 0 || CompareFileTime(&ft, &ffd.ftCreationTime) > 0 )
{
bool b = is_picture(ffd.cFileName);
if (b) {
wstring fullname = strPath + L"\\" + ffd.cFileName;
lstFile.push_back(_w2u(fullname));
}
}
}
}

while (FindNextFileW(hFind, &ffd))
{
if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
{
if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
{
//memset(buf0, 0, 1024 * sizeof(wchar_t));
//swprintf_s(buf0, 1024, L"%s\\%s", strPath.c_str(), ffd.cFileName);
//GetFolderPics(buf0, vecffd);
}
else
{
if (CompareFileTime(&ft, &ffd.ftLastWriteTime) > 0 || CompareFileTime(&ft, &ffd.ftCreationTime) > 0)
{
bool b = is_picture(ffd.cFileName);
if (b) {
wstring fullname = strPath + L"\\" + ffd.cFileName;
lstFile.push_back(_w2u(fullname));
}
}
}
}
}
FindClose(hFind);

}

free(buf1);


return 0;
}

*/
//long CHtmlgetFolder::GetFolderPics2(wstring strPath, list<string > &lstFile)
//{
//	strPath.erase(strPath.find_last_not_of(L"\\") + 1);
//
//	wchar_t *buf1 = (wchar_t*)malloc(1024 * sizeof(wchar_t));
//	if (buf1 != NULL)
//	{
//		memset(buf1, 0, 1024 * sizeof(wchar_t));
//		swprintf_s(buf1, 1024, L"%s\\*", strPath.c_str());
//	}
//
//	WIN32_FIND_DATAW ffd;
//	HANDLE hFind = FindFirstFile(buf1, &ffd);
//	if (hFind != INVALID_HANDLE_VALUE)
//	{
//		if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
//		{
//			if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
//			{
//				//memset(buf0, 0, 1024 * sizeof(wchar_t));
//				//swprintf_s(buf0, 1024, L"%s\\%s", strPath.c_str(), ffd.cFileName);
//				//GetFolderPics(buf0, vecffd);
//			}
//			else
//			{
//				bool b = is_picture(ffd.cFileName);
//				if (b) {
//					lstFile.push_back(_w2u(ffd.cFileName));
//				}
//			}
//		}
//
//		while (FindNextFileW(hFind, &ffd))
//		{
//			if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
//			{
//				if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
//				{
//					//memset(buf0, 0, 1024 * sizeof(wchar_t));
//					//swprintf_s(buf0, 1024, L"%s\\%s", strPath.c_str(), ffd.cFileName);
//					//GetFolderPics(buf0, vecffd);
//				}
//				else
//				{
//					bool b = is_picture(ffd.cFileName);
//					if (b) {
//						lstFile.push_back(_w2u(ffd.cFileName));
//					}
//				}
//			}
//		}
//		FindClose(hFind);
//
//	}
//
//	free(buf1);
//
//
//	return 0;
//}
/*
long CHtmlgetFolder::GetOneFolders(wstring strPath, list<string > &lstSubs)
{
WIN32_FIND_DATAW ffd;

wchar_t *buf0 = (wchar_t*)malloc(1024 * sizeof(wchar_t));
if (buf0 == NULL)
{
return 0;
}

wchar_t *buf1 = (wchar_t*)malloc(1024 * sizeof(wchar_t));
if (buf1 != NULL)
{
memset(buf1, 0, 1024 * sizeof(wchar_t));
swprintf_s(buf1, 1024, L"%s\\*", strPath.c_str());
}



HANDLE h = FindFirstFileEx(
buf1,
FindExInfoStandard,
&ffd,
FindExSearchLimitToDirectories,
NULL,
0);

if (h != INVALID_HANDLE_VALUE)
{
if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
{
if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
{
memset(buf0, 0, 1024 * sizeof(wchar_t));
swprintf_s(buf0, 1024, L"%s\\%s", strPath.c_str(), ffd.cFileName);
lstSubs.push_back(_w2u(buf0));
}
else
{
//filesize += ffd.nFileSizeLow / 1024;
}
}

while (FindNextFileW(h, &ffd))
{
if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
{
if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
{
memset(buf0, 0, 1024 * sizeof(wchar_t));
swprintf_s(buf0, 1024, L"%s\\%s", strPath.c_str(), ffd.cFileName);

lstSubs.push_back(_w2u(buf0));
}
else
{
//filesize += ffd.nFileSizeLow / 1024;
}
}
}
FindClose(h);
}

free(buf1);
free(buf0);


return 0;
}


long CHtmlgetFolder::GetFolderSize(wstring strPath, LONG64 &filesize)
{
//HRESULT hr;

//LPSHELLFOLDER lpDesktop = NULL;
//hr = SHGetDesktopFolder(&lpDesktop);
//if (FAILED(hr))
//{
//	return FALSE;
//}

//LPITEMIDLIST pidlWorkDir = NULL;
//hr = lpDesktop->ParseDisplayName(NULL, NULL, (LPWSTR)strPath.c_str(), NULL, &pidlWorkDir, NULL);
//if (FAILED(hr))
//{
//	lpDesktop->Release();
//	return FALSE;
//}

//GetSizeHelp(lpDesktop, pidlWorkDir, filesize);
//lpDesktop->Release();

wchar_t *buf1 = (wchar_t*)malloc(1024 * sizeof(wchar_t));
if (buf1 != NULL)
{
memset(buf1, 0, 1024 * sizeof(wchar_t));
swprintf_s(buf1, 1024, L"%s\\*", strPath.c_str());
}

WIN32_FIND_DATAW ffd;
HANDLE hFind = FindFirstFile(buf1, &ffd);
if (hFind != INVALID_HANDLE_VALUE)
{
if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
{
if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
{
//memset(buf0, 0, 1024 * sizeof(wchar_t));
//swprintf_s(buf0, 1024, L"%s\\%s", strPath.c_str(), ffd.cFileName);
//GetFolderSize(buf0, filesize);
}
else
{
if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
filesize += ffd.nFileSizeLow / 1024;
}
}

while (FindNextFileW(hFind, &ffd))
{
if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
{
if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
{
//memset(buf0, 0, 1024 * sizeof(wchar_t));
//swprintf_s(buf0, 1024, L"%s\\%s", strPath.c_str(), ffd.cFileName);
//GetFolderSize(buf0, filesize);
}
else
{
if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
filesize += ffd.nFileSizeLow / 1024;
}
}
}
FindClose(hFind);

}

free(buf1);

return 0;
}


*/
/*
//if (page == "folder")
//{
//	string dir_files = "";
//	string folder_tr = "";
//	CLang::Instance().getOneData("folder_tr", folder_tr);
//	CLang::Instance().getOneData("dir_files", dir_files);

//	string folder = "";
//	int pos = request_dto.szArgs.find("folder=");
//	if (pos >= 0)
//	{
//		folder = request_dto.szArgs.substr(pos + 7);
//	}
//	URLDecode2(folder);

//	if (folder != "")
//	{
//		string folder_td = "";
//		CLang::Instance().getOneData("folder_td_span", folder_td);
//		ReplaceOne(folder_tr, "{$folder_td}", folder_td);


//		char buf[MAX_PATH];
//		memset(buf, 0, MAX_PATH);
//		strcpy_s(buf, MAX_PATH, folder.c_str());

//		string name = "";
//		char *p = strrchr(buf, '\\');
//		if (p != NULL)
//		{
//			name = p + 1;
//		}

//		if (name == "")
//		{
//			name = buf[0];
//		}

//		list<string > lstSubs;
//		lstSubs.push_back(folder);


//		StringBuffer sb;
//		PrettyWriter<StringBuffer> writer(sb);

//		writer.StartObject();
//		writer.String("stat");
//		writer.String("ok");

//		writer.String("files");
//		writer.StartArray();

//		list<string>::iterator it;
//		for (it = lstSubs.begin(); it != lstSubs.end(); it++)
//		{
//			_trid++;
//			string folder2 = *it;
//			char buf[MAX_PATH];
//			memset(buf, 0, MAX_PATH);
//			strcpy_s(buf, MAX_PATH, folder2.c_str());

//			string name2 = "";
//			char *p = strrchr(buf, '\\');
//			if (p != NULL)
//			{
//				name2 = p + 1;
//			}

//			if (name2 == "")
//			{
//				name2 = buf[0];
//			}

//			vector<WIN32_FIND_DATAW > vecffd;
//			CHtmlgetFolder::Instance().GetFolderPics(_u2w(*it), vecffd);

//			int len = vecffd.size();
//			string html = folder_tr;
//			ReplaceOne(html, "{$id}", "0");
//			ReplaceOne(html, "{$folder}", name2);
//			ReplaceOne(html, "{$uploaded}", " 0" + dir_files);
//			ReplaceOne(html, "{$pending}", Int2Str(len) + dir_files);
//			ReplaceOne(html, "{$sizes}", Int2Str(len));
//			ReplaceOne(html, "{$state}", Int2Str(len));
//			ReplaceOne(html, "g='cancel'", "");
//			Replace(html, "{$trid}", string("trid") + Int2Str(_trid));
//			Replace(html, "{$dir}", folder2);

//			translate(html, "", "");
//			writer.String(html.c_str());
//		}
//		writer.EndArray();
//		writer.EndObject();

//		response_dto.szResponse = sb.GetString();
//		response_dto.szContentType = "text/html";
//		return S_OK;
//	}
//	else
//	{
//		string folder_td = "";
//		CLang::Instance().getOneData("folder_td_a", folder_td);
//		ReplaceOne(folder_tr, "{$folder_td}", folder_td);

//		string trs = "";
//		string loading = "<img src=\"./template/themes/default/images/loader.gif\" />";

//		list<CATEGORY_DTO> lstCategory;
//		list<CATEGORY_DTO>::iterator iter;
//		list<CATEGORY_DTO>::iterator it;

//		Cchatdal::Instance().get_order_categories("a.uppercats", lstCategory);
//		if (!lstCategory.empty())
//		{
//			for (iter = lstCategory.begin(); iter != lstCategory.end(); iter++)
//			{
//				if (iter->dir != "")
//				{
//					string fullname = "";
//					get_fullname(iter->uppercats, &lstCategory, fullname);

//					if (fullname == "")
//					{
//						fullname = iter->name;
//					}

//					string temp = folder_tr;
//					Replace(temp, "{$id}", iter->id);
//					ReplaceOne(temp, "{$folder}", fullname);
//					Replace(temp, "{$dir}", iter->dir);

//					ReplaceOne(temp, "{$uploaded}", loading);
//					ReplaceOne(temp, "{$pending}", "");

//					Replace(temp, "{$trid}", string("trid") + iter->id);
//					ReplaceOne(temp, "g='cancel'", "style=\"display:none;\"");
//					trs += temp;
//				}
//			}
//		}


//		Htmlread(IDR_ADMIN_FOLDER_HTML, L"html/_admin_folder.html", tpl);

//		wstring szPath = get_module_path(NULL);
//		string type = "1";
//		if (CDataSrv::Instance()._albumName == "private")
//		{
//			type = "2";
//			szPath += L"\\data\\private";
//		}
//		else
//		{
//			szPath += L"\\data\\public";
//		}
//		ReplaceOne(tpl, "{$type}", type);
//		ReplaceOne(tpl, "{$path}", _w2u(szPath));

//		string albumSummary_label = "";
//		CLang::Instance().getOneData("albumSummary_label", albumSummary_label);
//		translate(albumSummary_label, "", "");
//		ReplaceOne(tpl, "{$albumSummary_label}", albumSummary_label);

//		string upload_file_types = "";
//		CLang::Instance().getOneData("upload_file_types", upload_file_types);
//		ReplaceOne(tpl, "{$upload_file_types}", upload_file_types);

//		string uploadWarnings = "";
//		CLang::Instance().getOneData("uploadWarnings", uploadWarnings);
//		translate(uploadWarnings, "", "");
//		ReplaceOne(uploadWarnings, "%s", upload_file_types);
//		ReplaceOne(tpl, "{$uploadWarnings}", uploadWarnings);

//		ReplaceOne(tpl, "{$folder_trs}", trs);
//	}
//}
*/
//else if (method == "upload.pending")
//{
//	string id = "";
//	lookitem(request_dto.szArgs, "id=", id);

//	string dir_files = "";
//	CLang::Instance().getOneData("dir_files", dir_files);
//	translate(dir_files, "", "");

//	FOLDER_STATE_DTO state_dto;
//	state_dto.id = id;
//	state_dto.uploaded = "0" + dir_files;
//	state_dto.pending = "0" + dir_files;
//	state_dto.sizes = "0";
//	state_dto.state = "ok";

//	string strPath = "";

//	list<CATEGORY_DTO> *plstCategory = CDataSrv::Instance().get_lstCategory();
//	list<CATEGORY_DTO>::iterator iter;
//	for (iter = plstCategory->begin(); iter != plstCategory->end(); iter++)
//	{
//		if (iter->id == id)
//		{
//			strPath = iter->dir;
//			break;
//		}
//	}

//	list<MD5_DTO> lstmd5;
//	list<MD5_DTO>::iterator I;
//	Cimagedal::Instance().get_image_md5(strPath, lstmd5);

//	list<FILE_DTO> lstFile;
//	CHtmlgetFolder::Instance().GetFolderPics(iter->id, _u2w(strPath), lstFile);

//	int len = lstFile.size();
//	int nbExist = 0;

//	list<FILE_DTO>::iterator it;
//	for (it = lstFile.begin(); it != lstFile.end(); it++)
//	{
//		wstring fname = _u2w(it->filename);
//		struct _stat32  info;
//		int result = _wstat32(fname.c_str(), &info);
//		if (result == 0)
//		{
//			string temp = it->filename;
//			temp += time2Str(info.st_mtime);
//			temp += time2Str(info.st_size);

//			string md5sum = Int2Str(murmur3_32((unsigned char*)temp.c_str(), temp.size(), 123456));

//			bool b = false;
//			for (I = lstmd5.begin(); I != lstmd5.end(); I++)
//			{
//				if (I->md5 == md5sum && I->cat_id == it->cat_id)
//				{
//					b = true;
//					break;
//				}
//			}

//			if (b)
//			{
//				nbExist++;
//			}
//		}
//	}

//	string state = "";

//	int nb = len - nbExist;
//	if (nb == 0)
//	{
//		state = "ok";
//	}
//	else if (nb < 0)
//	{
//		nbExist = len;
//		nb = 0;
//		state = "ok";
//	}
//	else
//	{
//		state = Int2Str(nb);
//	}

//	state_dto.id = iter->id;
//	state_dto.uploaded = Int2Str(nbExist) + dir_files;
//	state_dto.pending = Int2Str(nb) + dir_files;
//	state_dto.sizes = Int2Str(nb);
//	state_dto.state = state;


//	StringBuffer sb;
//	PrettyWriter<StringBuffer> writer(sb);

//	writer.StartObject();
//	writer.String("stat");
//	writer.String("ok");

//	writer.String("result");
//	writer.StartObject();
//	writer.String("id");
//	writer.String(state_dto.id.c_str());
//	writer.String("uploaded");
//	writer.String(state_dto.uploaded.c_str());
//	writer.String("pending");
//	writer.String(state_dto.pending.c_str());
//	writer.String("state");
//	writer.String(state_dto.state.c_str());
//	writer.String("sizes");
//	writer.String(state_dto.sizes.c_str());
//	writer.EndObject();
//	writer.EndObject();

//	response_dto.szResponse = sb.GetString();
//	response_dto.szContentType = "text/plain; charset=utf-8";
//}
/*
long CHtmlgetFolder::GetFolderPics(wstring strPath, list<string > &lstFile)
{
strPath.erase(strPath.find_last_not_of(L"\\") + 1);

wchar_t *buf1 = (wchar_t*)malloc(1024 * sizeof(wchar_t));
if (buf1 != NULL)
{
memset(buf1, 0, 1024 * sizeof(wchar_t));
swprintf_s(buf1, 1024, L"%s\\*", strPath.c_str());
}

WIN32_FIND_DATAW ffd;
HANDLE hFind = FindFirstFile(buf1, &ffd);
if (hFind != INVALID_HANDLE_VALUE)
{
if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
{
if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
{
//memset(buf0, 0, 1024 * sizeof(wchar_t));
//swprintf_s(buf0, 1024, L"%s\\%s", strPath.c_str(), ffd.cFileName);
//GetFolderPics(buf0, vecffd);
}
else
{
bool b = is_picture(ffd.cFileName);
if (b) {
wstring fullname = strPath + L"\\" + ffd.cFileName;
lstFile.push_back(_w2u(fullname));
}
}
}

while (FindNextFileW(hFind, &ffd))
{
if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
{
if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
{
//memset(buf0, 0, 1024 * sizeof(wchar_t));
//swprintf_s(buf0, 1024, L"%s\\%s", strPath.c_str(), ffd.cFileName);
//GetFolderPics(buf0, vecffd);
}
else
{
bool b = is_picture(ffd.cFileName);
if (b) {
wstring fullname = strPath + L"\\" + ffd.cFileName;
lstFile.push_back(_w2u(fullname));
}
}
}
}
FindClose(hFind);

}

free(buf1);


return 0;
}

*/
//if (CDataSrv::Instance()._public_size == 0)
//{
//	wstring szFile = szPath + L"\\data\\public";
//	get_sizes("public_size", szFile);
//}

//if (CDataSrv::Instance()._private_size == 0)
//{
//	wstring szFile = szPath + L"\\data\\private";
//	get_sizes("private_size", szFile);
//}
//if (CDataSrv::Instance()._galleries_size == 0)
//{
//	wstring szFile = szPath + L"\\data\\public\\_data\\galleries";
//	get_sizes("galleries_size", szFile);
//}

//time_t current_time;
//time(&current_time);
//
//CONFIG_DTO lastrun_dto;
//lastrun_dto.param = "lastrun";
//lastrun_dto.value = time2Str(current_time);
//
//list<CONFIG_DTO> lstConfig;
//lstConfig.push_back(lastrun_dto);
//HRESULT hr = Cchatdal::Instance().add_configs(lstConfig);
//
//
//
//else if (iter->param == "lastrun")
//{
//	time_t lastrun = _atoi64(iter->value.c_str());
//	LONGLONG  li = Int32x32To64(lastrun, 10000000) + 116444736000000000;

//	_ft.dwLowDateTime = (DWORD)li;
//	_ft.dwHighDateTime = li >> 32;

//}
//else if (iter->param == "private_size")
//{
//	_private_size = _atoi64(iter->value.c_str());
//}
//else if (iter->param == "public_size")
//{
//	_public_size = _atoi64(iter->value.c_str());
//}
//else if (iter->param == "galleries_size")
//{
//	_galleries_size = _atoi64(iter->value.c_str());
//}


//����C:\����Ŀ¼�����ļ�������ɾ��  
//HANDLE dwChangeHandles[2];

/*
HANDLE dwChangeHandles0 = FindFirstChangeNotification(
iniPath.c_str(), // directory to watch
TRUE, // watch the subtree
FILE_NOTIFY_CHANGE_LAST_WRITE); // watch dir. name changes

if (dwChangeHandles0 == INVALID_HANDLE_VALUE)
{
return 0;
}
//	ExitProcess(GetLastError());




HANDLE dwChangeHandles1 = FindFirstChangeNotification(
_synroot.c_str(), // directory to watch
TRUE, // do not watch the subtree
FILE_NOTIFY_CHANGE_LAST_WRITE); // watch file name changes

if (dwChangeHandles1 != INVALID_HANDLE_VALUE)
{
dwChangeHandles[0] = dwChangeHandles0;
dwChangeHandles[1] = dwChangeHandles1;
}



LOG(ERROR) << "file_change";

//����C:\WindowsĿ¼�µ��ļ�������ɾ��
//ExitProcess(GetLastError());

int tk = 0;
DWORD dwWaitStatus;




// Change notification is set. Now wait on both notification
// handles and refresh accordingly.
RootWindowWin* self = GetUserDataPtr<RootWindowWin*>((HWND)pParam);

while (TRUE)
{

// Wait for notification.
if (dwChangeHandles1 != INVALID_HANDLE_VALUE)
{
dwWaitStatus = WaitForMultipleObjects(2, dwChangeHandles, FALSE, INFINITE);
}
else
{
dwWaitStatus = WaitForSingleObject(dwChangeHandles0, INFINITE);
}

switch (dwWaitStatus)
{
case WAIT_OBJECT_0:

EnterCriticalSection(&(self->_cs));

if (GetTickCount() - tk >= 1000)
{
Sleep(100);
LOG(ERROR) << tk;
tk = GetTickCount();
file_change((HWND)pParam, "upload_files");

}

LeaveCriticalSection(&(self->_cs));

if (FindNextChangeNotification(dwChangeHandles[0]) == FALSE)
{
}
////ExitProcess(GetLastError());
break;

case WAIT_OBJECT_0 + 1:
//��C:\WINDOWSĿ¼�д�����ɾ���ļ� ��
//ˢ�¸�Ŀ¼�����������֪ͨ(change notification).
LOG(ERROR) << "WAIT_OBJECT_0 1";

EnterCriticalSection(&(self->_cs));

if (GetTickCount() - tk >= 1000)
{
Sleep(100);
LOG(ERROR) << tk;
tk = GetTickCount();
file_change((HWND)pParam, "syn_files");

}

LeaveCriticalSection(&(self->_cs));


if (FindNextChangeNotification(dwChangeHandles[1]) == FALSE)
{

}
//ExitProcess(GetLastError());
break;

default:
//ExitProcess(GetLastError());
break;
}
}

UINT __stdcall RootWindowWin::handle_change_files1(LPVOID pParam)
{
HMODULE hModule = ::GetModuleHandle(NULL);

wchar_t buf[MAX_PATH];
memset(buf, 0, MAX_PATH * sizeof(wchar_t));
GetModuleFileNameW(hModule, buf, MAX_PATH);

wchar_t *p = wcsrchr(buf, L'\\');
if (p != NULL)
{
p[0] = 0;
}

p = wcsrchr(buf, L'\\');
if (p != NULL)
{
p[0] = 0;
}

wstring iniPath = buf;
iniPath += L"\\data\\public\\_upload";
LOG(ERROR) << iniPath.c_str();

file_change((HWND)pParam, iniPath);

return 0;
}

int RootWindowWin::file_change(HWND hWnd, wstring iniPath)
{

RootWindowWin* self = GetUserDataPtr<RootWindowWin*>(hWnd);
int tk = 0;

HANDLE hand = CreateFile(iniPath.c_str(),
FILE_LIST_DIRECTORY,
FILE_SHARE_READ | FILE_SHARE_DELETE,
NULL,
OPEN_EXISTING,
FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
NULL
);

char   notify[1024] = { 0 };
FILE_NOTIFY_INFORMATION   *pnotify = (FILE_NOTIFY_INFORMATION   *)notify;
DWORD   cbBytes;

while (true)
{
BOOL ret = ReadDirectoryChangesW(hand,
&notify,
sizeof(notify),
TRUE,
FILE_NOTIFY_CHANGE_LAST_WRITE,
&cbBytes,
NULL,
NULL);

if (ret)
{
if (FILE_ACTION_MODIFIED == pnotify->Action)
{
//char   AnsiChar[1024] = { 0 };
//WideCharToMultiByte(CP_ACP, 0, pnotify->FileName, pnotify->FileNameLength / 2, AnsiChar, 1024, NULL, NULL);

wchar_t buf[1024] = { 0 };
wcscpy_s(buf, pnotify->FileNameLength, pnotify->FileName);

LOG(ERROR) << buf;
EnterCriticalSection(&(self->_cs));

if (GetTickCount() - tk >= 1000)
{
if (CefRefPtr<CefBrowser> browser = self->GetBrowser())
{
Sleep(100);
LOG(ERROR) << tk;
tk = GetTickCount();
//file_change((HWND)pParam, AnsiChar);

char *szResponse = 0;
char *szMimeType = 0;

string fpath = CefString(iniPath);
fpath += CefString("\\");
fpath += CefString(buf);
//CefURIEncode(CefBase64Encode(data.data(), data.size()), false);

string url = "c.html?act=syn_files&fpath=" + fpath;
mygetfunc(url.c_str(), "", &szResponse, &szMimeType);

char id[MAX_PATH];
memset(id, 0, MAX_PATH);
strcpy_s(id, MAX_PATH, szResponse);
LocalFree(szResponse);

string js = "if(typeof update_files === \"function\") update_files('" + string(id) + "');";

browser->GetMainFrame()->ExecuteJavaScript(js, "", 0);
}
}

LeaveCriticalSection(&(self->_cs));
}
else if (FILE_ACTION_ADDED == pnotify->Action)
{
//printf("%s Add\n", AnsiChar);
}
else if (FILE_ACTION_REMOVED == pnotify->Action)
{
//printf("%s Removed\n", AnsiChar);
}
}
}

CloseHandle(hand);

return 0;
}


UINT RootWindowWin::monitor_change_files(HWND hWnd)
{
//unsigned int			ThreadD_ID;
//_beginthreadex(NULL, 0, handle_change_files, hWnd, NULL, &ThreadD_ID);

//unsigned int			ThreadD_ID2;
//_beginthreadex(NULL, 0, handle_change_files1, hWnd, NULL, &ThreadD_ID2);

//LOG(ERROR) << "monitor_change_files";

//char *szResponse = 0;
//char *szMimeType = 0;

//string url = "c.html?act=syn_init";
//mygetfunc(url.c_str(), "", &szResponse, &szMimeType);

//LocalFree(szResponse);



return 0;
}

*/



//HANDLE h = (HANDLE)_beginthreadex(NULL, 0, monitor_files, (LPVOID)hWnd, NULL, NULL);
//SetThreadPriority(h, THREAD_PRIORITY_IDLE);
//CloseHandle(h);
//
//UINT __stdcall RootWindowWin::monitor_files(LPVOID pParam)
//{
//	RootWindowWin* self = GetUserDataPtr<RootWindowWin*>((HWND)pParam);
//
//	//self->hEvent = CreateEvent(NULL, false, NULL, NULL);
//
//	//HANDLE h = (HANDLE)_beginthreadex(NULL, 0, handle_change_files, pParam, NULL, NULL);
//	//SetThreadPriority(h, THREAD_PRIORITY_IDLE);
//	//CloseHandle(h);
//
//
//	//while (true)
//	//{
//	//	Sleep(1000);
//	//	if (CefRefPtr<CefBrowser> browser = self->GetBrowser())
//	//	{
//	//		std::string url = browser->GetMainFrame()->GetURL();
//	//		if (url.find("/picture.html") != string::npos)
//	//		{
//	//		}
//	//		else
//	//		{
//	//			//SetEvent(self->hEvent);
//	//			//self->is_monitor_init_ = false;
//	//			LOG(ERROR) << "monitor_files break";
//	//			break;
//
//	//		}
//	//	}
//	//}
//	return 0;
//}
/*

void CHtmlgetAdmin::EmptyDirectory(string type)
{
	string album = CDataSrv::Instance()._albumName;
	wstring albumName = _u2w(album);

	wstring szPath = get_module_path(NULL);
	wstring path = szPath + L"\\data\\" + albumName + L"\\_upload";

	list<string > lstdir;
	list<string>::iterator it;
	if (type == "0")
	{
		CHtmlgetFolder::Instance().GetSubFolders(path, lstdir);
		lstdir.reverse();

		for (it = lstdir.begin(); it != lstdir.end(); it++)
		{
			RemoveDirectoryA(it->c_str());
		}

		lstdir.clear();
	}

	path = szPath + L"\\data\\" + albumName + L"\\_data";
	CHtmlgetFolder::Instance().GetSubFolders(path, lstdir);
	lstdir.reverse();

	for (it = lstdir.begin(); it != lstdir.end(); it++)
	{
		RemoveDirectoryA(it->c_str());
	}

}
*/
		/*
		$.get('ws.php?method=galleries.syn_init', function (data) {
		var info = jQuery.parseJSON(data);
		if (info.stat == "ok") {

		for (var i = 0; i < info.result.length; i++) {
		var path = info.result[i].fname;
		path = encodeURIComponent(path);

		$.get('ws.php?method=galleries.syn_init&path=' + path, function (data) {
		info = jQuery.parseJSON(data);
		if (info.stat == "ok") {
		}
		});
		}
		}
		});
		}
		*/
		/*
		function update_files(id) {
		alert(id);
		jQuery("#changed_files").show();

		var d = new Date();
		var n = d.getTime();

		var src = $("#" + id).attr('src');
		$("#" + id).attr('src', src + "?t=" + n);
		}
		*/



		//function iFrameHeight() {
		//	var h = $('#ifr').contents().find("body").height() + 10;
		//	var h = $('#ifr').contents().find("body").height() + 10;
		//	$('#ifr').height(h);

		//	var ifm = document.getElementById("ifr");
		//	var subWeb = document.frames ? document.frames["ifr"].document : ifm.contentDocument;
		//	if (ifm != null && subWeb != null) {
		//		ifm.height = subWeb.body.scrollHeight;
		//	}
		//}
		
		
		
/*
long CHtmlgetAdminProc::get_syn_files(string &del_ids, list<FILE_DTO> &change_files)
{

	wstring synroot = CDataSrv::Instance()._curr_synroot;
	wstring szPath = synroot;
	string szPath2 = _w2u(synroot);

	if (szPath == L"")
	{
		return 0;
	}

	list<string > lstSubs;
	CHtmlgetFolder::Instance().GetSubFolders(szPath, lstSubs);

	list<string>::iterator it;
	for (it = lstSubs.begin(); it != lstSubs.end(); it++)
	{
		string strPath = *it;

		string def = strPath;
		ReplaceOne(def, szPath2, "");

		list<MD5_DTO> lstmd5;
		list<MD5_DTO>::iterator I;
		Cimagedal::Instance().get_image_md5(def, lstmd5);

		wstring t = _u2w(strPath);
		list<FILE_DTO> lstFile;
		list<FILE_DTO>::iterator it;
		CHtmlgetFolder::Instance().GetFolderPics("", t, lstFile);

		int len = lstFile.size();
		int nbExist = 0;

		for (it = lstFile.begin(); it != lstFile.end(); it++)
		{
			for (I = lstmd5.begin(); I != lstmd5.end(); I++)
			{
				string::size_type  pos = I->path.find("\\galleries\\");
				if (pos != string::npos)
				{
					pos = it->filename.find(I->path.substr(10));
					if (pos != string::npos)
					{
						it->cat_id = I->cat_id;
						it->pic_id = I->pic_id;
						if (I->lastmodified != it->lastmodified)
						{
							change_files.push_back(*it);
						}
						lstmd5.erase(I);
						break;
					}
				}
			}
		}

		string del_ids = "";
		{
			for (I = lstmd5.begin(); I != lstmd5.end(); I++)
			{
				string::size_type  pos = I->path.find("\\galleries\\");
				if (pos != string::npos)
				{
					bool b = false;
					for (it = lstFile.begin(); it != lstFile.end(); it++)
					{
						pos = it->filename.find(I->path.substr(10));
						if (pos != string::npos  && I->lastmodified == it->lastmodified)
						{
							b = true;
							break;
						}
					}

					if (!b)
					{
						del_ids += I->pic_id + ",";
					}
				}
			}
		}
	}

	return 0;
}

long CHtmlgetAdminProc::get_upload_files(string &del_ids, list<FILE_DTO> &change_files)
{

	wstring szPath = get_module_path(NULL);
	szPath = szPath + L"\\data\\public\\_upload";

	list<string > lstSubs;
	CHtmlgetFolder::Instance().GetSubFolders(szPath, lstSubs);

		list<MD5_DTO> lstmd5;
		list<MD5_DTO>::iterator I;
		Cimagedal::Instance().get_image_md5("", lstmd5);

	list<string>::iterator it;
	for (it = lstSubs.begin(); it != lstSubs.end(); it++)
	{
		string strPath = *it;

		wstring t = _u2w(strPath);
		list<FILE_DTO> lstFile;
		list<FILE_DTO>::iterator it;
		CHtmlgetFolder::Instance().GetFolderPics("", t, lstFile);

		int len = lstFile.size();
		int nbExist = 0;

		for (it = lstFile.begin(); it != lstFile.end(); it++)
		{
			for (I = lstmd5.begin(); I != lstmd5.end(); I++)
			{
				//string::size_type  pos = I->path.find("\\galleries\\");
				//if (pos != string::npos)
				{
					string::size_type pos = it->filename.find(I->path);
					if (pos != string::npos)
					{
						it->cat_id = I->cat_id;
						it->pic_id = I->pic_id;
						if (I->lastmodified != it->lastmodified)
						{
							change_files.push_back(*it);
						}
						lstmd5.erase(I);
						break;
					}
				}
			}
		}

		string del_ids = "";
		{
			for (I = lstmd5.begin(); I != lstmd5.end(); I++)
			{
				string::size_type  pos;// = I->path.find("\\galleries\\");
				//if (pos != string::npos)
				{
					bool b = false;
					for (it = lstFile.begin(); it != lstFile.end(); it++)
					{
						pos = it->filename.find(I->path);
						if (pos != string::npos  && I->lastmodified == it->lastmodified)
						{
							b = true;
							break;
						}
					}

					if (!b)
					{
						del_ids += I->pic_id + ",";
					}
				}
			}
		}
	}

	return 0;
}
*/



	/////////
	////////wchar_t *buf0 = (wchar_t*)malloc(1024 * sizeof(wchar_t));
	////////if (buf0 == NULL)
	////////{
	////////	return 0;
	////////}
	////////wchar_t *buf1 = (wchar_t*)malloc(1024 * sizeof(wchar_t));
	////////if (buf1 != NULL)
	////////{
	////////	memset(buf1, 0, 1024 * sizeof(wchar_t));
	////////	swprintf_s(buf1, 1024, L"%s\\*", strPath.c_str());
	////////}

	////////WIN32_FIND_DATAW ffd;
	////////HANDLE hFind = FindFirstFile(buf1, &ffd);
	////////if (hFind != INVALID_HANDLE_VALUE)
	////////{
	////////	if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
	////////	{
	////////		if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	////////		{
	////////			memset(buf0, 0, 1024 * sizeof(wchar_t));
	////////			swprintf_s(buf0, 1024, L"%s\\%s", strPath.c_str(), ffd.cFileName);
	////////			lstSubs.push_back(_w2u(buf0));
	////////			GetSubFolders(buf0, lstSubs);
	////////		}
	////////		else
	////////		{
	////////			//filesize += ffd.nFileSizeLow / 1024;
	////////		}
	////////	}

	////////	while (FindNextFileW(hFind, &ffd))
	////////	{
	////////		if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
	////////		{
	////////			if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	////////			{
	////////				memset(buf0, 0, 1024 * sizeof(wchar_t));
	////////				swprintf_s(buf0, 1024, L"%s\\%s", strPath.c_str(), ffd.cFileName);

	////////				lstSubs.push_back(_w2u(buf0));
	////////				GetSubFolders(buf0, lstSubs);
	////////			}
	////////			else
	////////			{
	////////				//filesize += ffd.nFileSizeLow / 1024;
	////////			}
	////////		}
	////////	}
	////////	FindClose(hFind);

	////////}

	////////free(buf1);

	////////free(buf0);
	////////














	/*

	$.get('ws.php?method=galleries.init', function (data) {
	jQuery("#folder").html(data);


	jQuery("a.icon-down-open").colorbox({
	transition: "none", width: "95%", height: "95%", fixed: true, iframe: true, maxWidth: 1240,
	onClosed: function () {
	if (_cat_name_change == 1) {
	var catid = $.colorbox.element().attr('data-id');
	$.get('ws.php?method=galleries.modify&id=' + catid, function (data) {
	var info = jQuery.parseJSON(data);
	if (info.stat == "ok") {
	$.colorbox.element().text(info.text);
	}
	});
	}
	}
	});






	if (data != " ") {
	setTimeout(function () {
	pre_data();
	}, 10);
	}
	else {
	$("#albumLoading").hide();
	}

	});


	var f = function (path) {
	var dtd = $.Deferred();
	$.get('ws.php?method=galleries.init&fd=' + path, function (data) {
	jQuery("#folder").append(data);
	dtd.resolve();
	});
	return dtd.promise();
	};



	function app() {
	var tasks = [];
	for (var i = 0; i < info.dirs.length; i++) {
	var path = encodeURIComponent(info.dirs[i].fd);
	tasks.push(f(path));
	}
	return tasks.reduce(function (prev, next) {
	return prev.then(next);
	}, $.Deferred().resolve());
	}

	$.when(app()).done(function () {
	var data = jQuery("#folder").html();
	});













	*/
	//long CHtmlgetIndexProc::proc_film_play_htm(const REQUEST_DTO request_dto, RESPONSE_DTO &response_dto)
	//{
	//	string idx = "";
	//	string start = "";
	//	lookitem(request_dto.szArgs, "idx=", idx);
	//	lookitem(request_dto.szArgs, "start=", start);
	//
	//	QUERY_REQ _req = CDataSrv::Instance()._req;
	//	string title1 = "";
	//	string title2 = "";
	//
	//	string website_url = "http://" + request_dto.host + request_dto.szUrl + "?" + _req.args;
	//	int pos = website_url.find("&posted=");
	//	if (pos < 0)
	//	{
	//		website_url += "&posted=" + _req.posted;
	//	}
	//
	//	string ids = "";
	//	list<string> lstOrderIds;
	//	bool b = false;
	//
	//	if (idx == "")
	//	{
	//		list<FILM_DTO> lstfilm;
	//		Cchatdal::Instance().get_film(website_url, lstfilm);
	//		if (!lstfilm.empty())
	//		{
	//			FILM_DTO dto = lstfilm.front();
	//			ids = dto.content;
	//			title1 = dto.author_id;
	//			title2 = dto.author;
	//
	//			vector<string> v1 = tokenize(ids, ",", true, "");
	//			int len = v1.size();
	//			for (int i = 0; i < len; i++)
	//			{
	//				lstOrderIds.push_back(v1[i]);
	//			}
	//		}
	//	}
	//
	//	if (ids.empty())
	//	{
	//		b = true;
	//		list<string> * plstImageIds = CDataSrv::Instance().get_lstImageIds();
	//		list<string>::iterator it;
	//		for (it = plstImageIds->begin(); it != plstImageIds->end(); it++)
	//		{
	//			ids += *it + ",";
	//			lstOrderIds.push_back(*it);
	//		}
	//		rtrim(ids, ",");
	//	}
	//
	//	string ids0 = "";
	//	string pics = "";
	//
	//	list<IMAGE_DTO> lstOrder;
	//	Cimagedal::Instance().get_images_all_ids(ids, &lstOrderIds, lstOrder);
	//
	//	int nb = lstOrder.size();
	//
	//	int total = 0;
	//
	//	int ind2 = atoi(idx.c_str());
	//	if (start != "" && idx == "")
	//	{
	//		ind2 = atoi(start.c_str());
	//	}
	//
	//	string widths = "";
	//	string heights = "";
	//
	//	int width = 1024;
	//	int height = 512;
	//	int i = 0;
	//	list<IMAGE_DTO>::iterator iter;
	//	for (iter = lstOrder.begin(); iter != lstOrder.end(); iter++)
	//	{
	//		if (nb <= 100 || i >= ind2)
	//		{
	//			get_size(*iter, width, height);
	//
	//			string url = CDataSrv::Instance().get_image_url(0, *iter);
	//			pics += "\"" + url + "?mk=" + time2Str(iter->lastmodified) + "\",";
	//			ids0 += iter->id + ",";
	//
	//			widths += Int2Str(width) + ",";
	//			heights += Int2Str(height) + ",";
	//			total++;
	//		}
	//
	//		if (idx == "")
	//		{
	//			if (title1.empty())
	//			{
	//				title1 = get_title(_req);
	//				title2 = get_time_string(iter->date_creation, "1");
	//			}
	//		}
	//
	//		i++;
	//		if (b)
	//		{
	//			if (i > 100 + ind2)
	//			{
	//				break;
	//			}
	//		}
	//	}
	//	rtrim(ids0, ",");
	//	rtrim(pics, ",");
	//	rtrim(widths, ",");
	//	rtrim(heights, ",");
	//
	//	string tpl = "";
	//	Htmlread(IDR_ART_FILM_HTML, L"html/_art_film.html", tpl);
	//
	//	string actionButtons_editfilm = "";
	//
	//	if (idx == "")
	//	{
	//		CLang::Instance().getOneData("actionButtons_editfilm", actionButtons_editfilm);
	//		actionButtons_editfilm = "<span id=\"cmdedit\" >" + actionButtons_editfilm + "</span>";
	//	}
	//
	//	ReplaceOne(tpl, "$total", Int2Str(total));
	//		ReplaceOne(tpl, "$idx", idx);
	//	ReplaceOne(tpl, "$nb", Int2Str(nb));
	//
	//	ReplaceOne(tpl, "$title1", title1);
	//	ReplaceOne(tpl, "$title2", title2);
	//	ReplaceOne(tpl, "{$actionButtons}", actionButtons_editfilm);
	//
	//	ReplaceOne(tpl, "$widths", widths);
	//	ReplaceOne(tpl, "$heights", heights);
	//
	//
	//	//2
	//	string picture_next = "";
	//	string picture_pre = "";
	//
	//	CLang::Instance().getOneData("picture_pre3", picture_pre);
	//	CLang::Instance().getOneData("picture_next3", picture_next);
	//
	//	ReplaceOne(picture_next, "g='next'", "style=\"display:none;\"");
	//	ReplaceOne(picture_pre, "g='pre'", "style=\"display:none;\"");
	//
	//	string calItem = "";
	//	calItem += picture_pre;
	//	calItem += picture_next;
	//
	//	string calendar_nav = "";
	//	CLang::Instance().getOneData("calendar_nav", calendar_nav);
	//	ReplaceOne(calendar_nav, "{$calItem}", calItem);
	//
	//	ReplaceOne(tpl, "{$calendar_nav}", calendar_nav);
	//
	//	translate(tpl, "", "");
	//
	//	ReplaceOne(tpl, "$ids", ids0);
	//	ReplaceOne(tpl, "$pics", pics);
	//
	//	response_dto.szResponse = tpl;
	//	response_dto.szContentType = "text/html";
	//
	//	return S_OK;
	//}
/*
long CHtmlgetIndexProc::proc_art_edit(const REQUEST_DTO request_dto, RESPONSE_DTO &response_dto)
{
	QUERY_REQ _req = CDataSrv::Instance()._req;
	string page = "";
	string box = "";
	string start = "";
	lookitem(request_dto.szArgs, "box=", box);
	lookitem(request_dto.szArgs, "page=", page);
	lookitem(request_dto.szArgs, "start=", start);

	string website_url = "http://" + request_dto.host + request_dto.szUrl + "?" + _req.args;
	int pos = website_url.find("&posted=");
	if (pos < 0)
	{
		website_url += "&posted=" + _req.posted;
	}
	ReplaceOne(website_url, "film_box.html", "film_play.html");

	string selection = ",";
	list<FILM_DTO> lstfilm;
	Cchatdal::Instance().get_film(website_url, lstfilm);
	if (!lstfilm.empty())
	{
		selection += lstfilm.front().photos + ",";
	}

	int start0 = atoi(start.c_str());

	int per = PERPAGE_NUM;

	int curr = atoi(page.c_str()) * per;

	list<IMAGE_DTO> lstOrder;

	int j = 0;
	string ids = "";
	list<string> lstOrderIds;
	list<string> * plstImageIds = CDataSrv::Instance().get_lstImageIds();
	list<string>::iterator it;
	for (it = plstImageIds->begin(); it != plstImageIds->end(); it++)
	{
		if (j > start0 + PERPAGE_TOTAL)
		{
			break;
		}

		if (j >= start0 && j < start0 + PERPAGE_TOTAL)
		{
			ids += *it + ",";
			lstOrderIds.push_back(*it);
		}
		j++;
	}
	rtrim(ids, ",");

	Cimagedal::Instance().get_images_all_ids(ids, &lstOrderIds, lstOrder);

	int maxPage0 = 0;
	int nb_thumbs_set = lstOrder.size();
	if (nb_thumbs_set > PERPAGE_NUM)
	{
		div_t div_result = div(nb_thumbs_set - PERPAGE_NUM, PERPAGE_NUM);
		maxPage0 = div_result.quot;
		if (div_result.rem != 0) {
			maxPage0 += 1;
		}
	}

	list<IMAGE_DTO>::iterator iter;


	string beatch_thumbnail = "";


	string film_data = "";
	if (!lstOrder.empty())
	{
		CLang::Instance().getOneData("film_data", film_data);

		string  nb_thumbs_set = Int2Str(lstOrder.size());

		string style1 = "style=\"width:{$width}px; height:{$height}px;display:inline-block;\"";

		float maxWidth = 144;
		float maxHeight = 144;
		string film_thumbnail_li = "";
		CLang::Instance().getOneData("film_thumbnail_li", film_thumbnail_li);

		int i = 0;
		for (iter = lstOrder.begin(); iter != lstOrder.end(); iter++)
		{
			if (i > curr + per)
			{
				break;
			}

			if (i >= curr && i < curr + per)
			{
				string temp = film_thumbnail_li;
				Replace(temp, "{$id}", iter->id);
				string url0 = CDataSrv::Instance().get_image_url(0, *iter);

				float outW = 0;
				float outH = 0;
				get_imgsize(iter->width, iter->height, maxWidth, maxHeight, outW, outH);
				if (iter->ext == "mp3")
				{
					outW = 100;
					outH = 100;
				}

				Replace(temp, "{$path}", url0);
				Replace(temp, "{$name}", iter->name);

				string css = "";
				string chk = "";
				string sel = "icon-check-empty";
				int pos = selection.find("," + iter->id + ",");
				if (pos >= 0)
				{
					sel = "icon-check";
					css = "thumbSelected";
					chk = "checked=\"checked\"";
				}
				ReplaceOne(temp, "thumbSelected", css);
				ReplaceOne(temp, "g='checked'", chk);
				ReplaceOne(temp, "cssSel", sel);
				ReplaceOne(temp, "{$mk}", time2Str(iter->lastmodified));

				string style = style1;

				ReplaceOne(style, "{$width}", Int2Str(outW));
				ReplaceOne(style, "{$height}", Int2Str(outH));

				ReplaceOne(temp, "g=\"style\"", style);

				beatch_thumbnail += temp;
			}
			i++;
		}

		ReplaceOne(film_data, "{$beatch_thumbnail}", beatch_thumbnail);
	}

	string tpl = "";

	if (box == "1")
	{
		tpl = beatch_thumbnail;
	}
	else
	{
		int nbSelected = 0;
		for (iter = lstOrder.begin(); iter != lstOrder.end(); iter++)
		{
			int pos = selection.find("," + iter->id + ",");
			if (pos >= 0)
			{
				nbSelected++;
			}
		}

		Htmlread(IDR_FILM_BOX_HTML, L"html/_film_box.html", tpl);

		ReplaceOne(tpl, "$nbSelected", Int2Str(nbSelected));
		ReplaceOne(tpl, "$nb_thumbs_set", Int2Str(nb_thumbs_set));
		ReplaceOne(tpl, "$maxPage0", Int2Str(maxPage0));

		translate(tpl, "", "");

		ReplaceOne(tpl, "{$picture_data}", film_data);
	}

	response_dto.szResponse = tpl;
	response_dto.szContentType = "text/html";

	return S_OK;
}
*/
/*
<script type = "text/javascript">

$(function(){
iframeWindow = $('#ifr')[0].contentWindow;

$("#trash").on("dragenter",function(ev){
if(iframeWindow.isCrossIFrameDragging) {
$(ev.target).addClass("hover").text("����ԥ�˸Ͻ����˰�");
}
})
.on('dragleave', function(ev) {
if(iframeWindow.isCrossIFrameDragging) {
$(ev.target).removeClass('hover').text("����������������");
}
})
.on("dragover",function(ev){
if(iframeWindow.isCrossIFrameDragging) {
ev.preventDefault();
ev.originalEvent.dataTransfer.dropEffect = 'move';
}
})
.on("drop",function(ev){
var df = ev.originalEvent.dataTransfer;
var data = df.getData("Text");
if(iframeWindow.isCrossIFrameDragging)  {
var id = data.match(/^cross_iframe_drag_([0-9]+)$/)[1];
$(ev.target).removeClass('hover').text("������["+id+"],������,������");
iframeWindow.removeDraggingItem();
}
});
});
*/

/*

long CHtmlgetAdmin::get_sizes(string album, list<IMAGE_URL_DTO>& lstImageDto)
{
wstring szPath = get_module_path(NULL) + L"\\data\\" + _u2w(album);

list<IMAGE_URL_DTO>::iterator iter;
for (iter = lstImageDto.begin(); iter != lstImageDto.end(); iter++)
{
_off_t off = 0;

struct _stat32  info;
iter->level = 0;

wstring szFile = L"";
wstring url = _u2w(iter->path);
string::size_type  pos = iter->path.find("/galleries");
if (pos != string::npos)
{
szFile = szPath + L"\\_data" + url;
Replace(szFile, L"/", L"\\");
}
else
{
szFile = szPath + url;
Replace(szFile, L"/", L"\\");
int result = _wstat32(szFile.c_str(), &info);
if (result == 0)
{
off = info.st_size;
}

ReplaceOne(url, L"/_upload", L"/_data");
}


int result = _wstat32(szFile.c_str(), &info);
if (result == 0)
{
off += info.st_size;
}

wstring szFile2 = szFile + L"_2.png";
result = _wstat32(szFile2.c_str(), &info);
if (result == 0)
{
off += info.st_size;
}

double d = off * 10;
d = d / 1024.0;
iter->level = d / 10;

if (iter->level == 0)
{
iter->level = 1;
}
}

return 0;
}

UINT __stdcall CHtmlgetAdmin::handle_change_files1(LPVOID pParam)
{
ARGS_DTO* ptr = (ARGS_DTO*)pParam;
string strType = ptr->szType;
string strAlbum = ptr->szAlbum;

int idx = ptr->idx;
CHtmlgetAdmin*pThis = (CHtmlgetAdmin*)ptr->pThis;

string albumName = CDataSrv::Instance()._albumName;

list<IMAGE_URL_DTO> lstImageDto;
Cbasedal::Instance().get_level_field(strType, idx * 10000, lstImageDto);
if (!lstImageDto.empty())
{
pThis->get_sizes(strAlbum, lstImageDto);
Cbasedal::Instance().update_level_field(strType, lstImageDto);
}

free(ptr);

return 0;
}
long CHtmlgetAdmin::get_folder_sizes(LONG64 &public_size, LONG64 &private_size)
{
	if (_start > 0)
	{
		return 1;
	}

	_start = 1;
	string albumName = CDataSrv::Instance()._albumName;

	int numThreads = 2;
	HANDLE handle[2] = { 0 };
	int i = 0;
	//for (i = 0; i < 20; i++)
	{
		ARGS_DTO* pParam = (ARGS_DTO*)malloc(sizeof(ARGS_DTO));
		pParam->pThis = this;

		memset(pParam->szType, 0, MAX_PATH);
		strcpy_s(pParam->szType, "curr");

		memset(pParam->szAlbum, 0, MAX_PATH);
		strcpy_s(pParam->szAlbum, albumName.c_str());

		pParam->idx = 0;

		handle[0] = (HANDLE)_beginthreadex(NULL, 0, handle_change_files1, (LPVOID)pParam, NULL, NULL);
		SetThreadPriority(handle[0], THREAD_PRIORITY_IDLE);
	}

	string other = "public";
	if (albumName == "public")
	{
		other = "private";
	}

	//for (i = 0; i < 20; i++)
	{
		ARGS_DTO* pParam = (ARGS_DTO*)malloc(sizeof(ARGS_DTO));
		pParam->pThis = this;

		memset(pParam->szType, 0, MAX_PATH);
		strcpy_s(pParam->szType, "other");

		memset(pParam->szAlbum, 0, MAX_PATH);
		strcpy_s(pParam->szAlbum, other.c_str());

		pParam->idx = 0;

		handle[1] = (HANDLE)_beginthreadex(NULL, 0, handle_change_files1, (LPVOID)pParam, NULL, NULL);
		SetThreadPriority(handle[1], THREAD_PRIORITY_IDLE);
	}
	WaitForMultipleObjects(numThreads, handle, TRUE, INFINITE);

	for (i = 0; i < numThreads; i++)
	{
		CloseHandle(handle[i]);
	}

	_start = 0;

	LONG64 foldersize = 0;
	Cbasedal::Instance().get_db_sizes("curr", foldersize);

	LONG64 foldersize2 = 0;
	Cbasedal::Instance().get_db_sizes("other", foldersize2);


	if (albumName == "public")
	{
		public_size = foldersize;
		private_size = foldersize2;
	}
	else
	{
		public_size = foldersize2;
		private_size = foldersize;
	}

	return 0;
}

long Cbasedal::get_db_sizes(string type, LONG64 &foldersize)
{
foldersize = 0;
if (type == "curr")
{
get_db_sizes(_pDB, _albumName, foldersize);
}
else
{
string other = "public";

sqlite3 * pTempDB = NULL;
if (_albumName == "public")
{
get_tempDb("private", &pTempDB);
other = "private";
}
else
{
get_tempDb("public", &pTempDB);
other = "public";
}
get_db_sizes(pTempDB, other, foldersize);

sqlite3_close(pTempDB);
}

return 0;
}


long Cbasedal::get_db_sizes(sqlite3 *pTempDB, string type, LONG64 &foldersize)
{
long hr = 0;
char * errmsg = NULL;
int result = 0;

char **dbResult;
int nRow, nColumn;

string sql = " select sum(level) from _images a ";
//if (type == "public")
//{
//	sql += " and c.dir = '' union ";

//	sql += " select sum(level)* 10 / 1024 from _images a,_image_category b,_categories c where a.id = b.image_id and b.category_id = c.id ";
//	sql += " and c.dir != '' ";
//}
//iter->level = iter->level * 10 / 1024;

result = sqlite3_get_table(pTempDB, sql.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
if (SQLITE_OK == result)
{
for (int i = 1; i <= nRow; i++)
{
foldersize += get_int64(dbResult[nColumn * i]);
}
}
sqlite3_free_table(dbResult);


return hr;
}



long Cbasedal::get_level_field(string type, int nb, list<IMAGE_URL_DTO> &lstImageDto)
{
if (type == "curr")
{
get_level_field(_pDB, _albumName, nb, lstImageDto);
}
else
{
string other = "public";

sqlite3 * pTempDB = NULL;
if (_albumName == "public")
{
get_tempDb("private", &pTempDB);
other = "private";
}
else
{
get_tempDb("public", &pTempDB);
other = "public";
}
get_level_field(pTempDB, other, -1, lstImageDto);

sqlite3_close(pTempDB);
}

return 0;
}


long Cbasedal::get_level_field(sqlite3 *pTempDB, string type, int nb, list<IMAGE_URL_DTO> &lstImageDto)
{
long hr = 0;
char * errmsg = NULL;
int result = 0;

char **dbResult;
int nRow, nColumn;
char * zSQL;

if (nb == -1)
{
zSQL = sqlite3_mprintf("SELECT id, path from _images a where a.level=0 ");
}
else
{
zSQL = sqlite3_mprintf("SELECT id, path from _images a where a.level=0 order by  a.level limit %d, 10000", nb);
}

result = sqlite3_get_table(pTempDB, zSQL, &dbResult, &nRow, &nColumn, &errmsg);
sqlite3_free(zSQL);
if (SQLITE_OK == result)
{
for (int i = 1; i <= nRow; i++)
{
IMAGE_URL_DTO dto;
dto.id = get_value(dbResult[nColumn * i]);
dto.path = get_value(dbResult[nColumn * i + 1]);
dto.level = 0;
lstImageDto.push_back(dto);
}
}
sqlite3_free_table(dbResult);


return hr;
}


long Cbasedal::update_level_field(string type, list<IMAGE_URL_DTO> lstImageDto)
{
if (type == "curr")
{
update_level_field(_pDB, _albumName, lstImageDto);
}
else
{
string other = "public";

sqlite3 * pTempDB = NULL;
if (_albumName == "public")
{
get_tempDb("private", &pTempDB);
other = "private";
}
else
{
get_tempDb("public", &pTempDB);
other = "public";
}
update_level_field(pTempDB, other, lstImageDto);

sqlite3_close(pTempDB);
}

return 0;
}


long Cbasedal::update_level_field(sqlite3 *pTempDB, string type, list<IMAGE_URL_DTO> lstImageDto)
{
long hr = 0;
char * errmsg = NULL;
int result = 0;

result = sqlite3_exec(pTempDB, "begin;", NULL, NULL, &errmsg);
if (errmsg != NULL)
{
sqlite3_free(errmsg);
errmsg = NULL;
}

list<IMAGE_URL_DTO>::iterator iter;
for (iter = lstImageDto.begin(); iter != lstImageDto.end(); iter++)
{
char * zSQL = sqlite3_mprintf("update _images set level= %d where id= %q; ", iter->level, iter->id.c_str());
result = sqlite3_exec(pTempDB, zSQL, 0, 0, &errmsg);
if (result != 0)
{
hr = result;
}
if (errmsg != NULL)
{
sqlite3_free(errmsg);
errmsg = NULL;
}
sqlite3_free(zSQL);
}

result = sqlite3_exec(pTempDB, "commit;", NULL, NULL, &errmsg);
if (errmsg != NULL)
{
sqlite3_free(errmsg);
}

return hr;
}
else if (act == "zip")
{
string ids = "";
lookitem(request_dto.szArgs, "ids=", ids);

EnterCriticalSection(&_cs);
//_zip_step = 0;
//_cancel_zip = 0;

string zPath = get_module_pathA(NULL);
string outfilename = zPath + "\\data\\public\\temp\\output.zip";

BOOL b = FALSE;
void* zf = OpenZf(outfilename.c_str(), false);

if (ids == "")
{
list<string> * plstImageIds = CDataSrv::Instance().get_lstImageIds();
list<string>::iterator it;
for (it = plstImageIds->begin(); it != plstImageIds->end(); it++)
{
ids += *it + ",";
}
}
rtrim(ids, ",");

list<IMAGE_DTO> lstImage;
Cimagedal::Instance().get_images_by_ids(ids, lstImage);

string album = CDataSrv::Instance()._albumName;

list<IMAGE_DTO>::iterator iter;
for (iter = lstImage.begin(); iter != lstImage.end(); iter++)
{
wstring ori = _u2w(iter->ori);
const wchar_t *savefilenameinzip = wcsrchr(ori.c_str(), L'/');
if (savefilenameinzip != NULL)
{
savefilenameinzip = savefilenameinzip + 1;
}

string strname = _w2a(savefilenameinzip);

string temp0 = ori2path(iter->ori);
wstring dest = _u2w(temp0);
//const char * p = W2A_CP(savefilenameinzip, CP_ACP);
if (album == "private")
{
wstring szPath = get_module_path(NULL);
wstring wstrinpath = szPath + L"\\data\\" + _u2w(album);
wstring temp = dest + L".krf";
dest = wstrinpath + L"\\temp\\0.tmp";
CUploadParser::Instance().Decryptfile(temp, dest);
}

_zip_step++;
ZipFile(zf, dest.c_str(), strname.c_str(), 5);

if (album == "private")
{
my_DeleteFile(dest.c_str());
}

if (_cancel_zip == 1)
{
b = TRUE;
break;
}
}
int r = CloseZf(zf);

string stat = "ok";
if (b)
{
SetFileAttributesA(outfilename.c_str(), FILE_ATTRIBUTE_NORMAL);
DeleteFileA(outfilename.c_str());
stat = "cancel";
}

StringBuffer sb;
PrettyWriter<StringBuffer> writer(sb);

writer.StartObject();
writer.String("stat");
writer.String(stat.c_str());
writer.String("zip");

string url = "public\\temp\\output.zip";
writer.String(url.c_str());

writer.EndObject();

response_dto.szContentType = "text/plain; charset=utf-8";
response_dto.szResponse = sb.GetString();

LeaveCriticalSection(&_cs);

return S_OK;
}
//function downbox_close() {
//	var ifm = document.getElementsByClassName("cboxIframe");
//	if (ifm.length > 0) {
//		$(".cboxIframe").contents().find('#back').click();
//	}
//	else {
//		jQuery.colorbox.close();
//	}
//}
else if (act == "process_zip")
{
StringBuffer sb;
PrettyWriter<StringBuffer> writer(sb);

writer.StartObject();
writer.String("stat");
writer.String("ok");
writer.String("step");

string t = Int2Str(_zip_step);
writer.String(t.c_str());

writer.EndObject();

response_dto.szContentType = "text/plain; charset=utf-8";
response_dto.szResponse = sb.GetString();


return S_OK;
}
//else if (act == "vr_del_box")
//{
//	string nb = "";
//	lookitem(request_dto.szArgs, "nb=", nb);

//	string del_box = "";
//	CLang::Instance().getOneData("vr_del_box", del_box);
//	Replace(del_box, "{$total}", nb);

//	translate(del_box, "", "");
//	response_dto.szContentType = "text/html";
//	response_dto.szResponse = del_box;

//	return S_OK;
//}

//UINT RootWindowWin::monitor_files(HWND hWnd)
//{
//	HANDLE h = (HANDLE)_beginthreadex(NULL, 0, handle_change_files, (LPVOID)hWnd, NULL, NULL);
//	SetThreadPriority(h, THREAD_PRIORITY_IDLE);
//	CloseHandle(h);
//
//	return 0;
//}

UINT __stdcall RootWindowWin::handle_change_files(LPVOID pParam)
{
	HANDLE dwChangeHandles[3];

	HMODULE hModule = ::GetModuleHandle(NULL);

	wchar_t buf[MAX_PATH];
	memset(buf, 0, MAX_PATH * sizeof(wchar_t));
	GetModuleFileNameW(hModule, buf, MAX_PATH);

	wchar_t *p = wcsrchr(buf, L'\\');
	if (p != NULL)
	{
		p[0] = 0;
	}

	p = wcsrchr(buf, L'\\');
	if (p != NULL)
	{
		p[0] = 0;
	}

	wstring iniPath = buf;
	iniPath += L"\\data1\\_add";
	//LOG(ERROR) << iniPath.c_str();



	HANDLE dwChangeHandles0 = FindFirstChangeNotification(
		iniPath.c_str(), // directory to watch  
		TRUE, // watch the subtree  
		FILE_NOTIFY_CHANGE_LAST_WRITE); // watch dir. name changes  

	if (dwChangeHandles0 == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	//ExitProcess(GetLastError());


	RootWindowWin* self = GetUserDataPtr<RootWindowWin*>((HWND)pParam);

	if (self->hEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle(self->hEvent);
		self->hEvent = INVALID_HANDLE_VALUE;
	}

	self->hEvent = CreateEvent(NULL, false, NULL, NULL);

	dwChangeHandles[0] = self->hEvent;
	dwChangeHandles[1] = dwChangeHandles0;


	HANDLE dwChangeHandles1 = FindFirstChangeNotification(
		L"c:\\aa",// directory to watch  
		TRUE, // do not watch the subtree  
		FILE_NOTIFY_CHANGE_LAST_WRITE); // watch file name changes  

	if (dwChangeHandles1 != INVALID_HANDLE_VALUE)
	{
		dwChangeHandles[2] = dwChangeHandles1;
	}

	//LOG(ERROR) << "file_change";

	//ExitProcess(GetLastError());

	int tk = 0;
	DWORD dwWaitStatus;


	//LOG(ERROR) << "monitor_files start";


	 //Change notification is set. Now wait on both notification  
	 //handles and refresh accordingly.  

	while (TRUE)
	{

		//Wait for notification.  
		if (dwChangeHandles1 != INVALID_HANDLE_VALUE)
		{
			dwWaitStatus = WaitForMultipleObjects(3, dwChangeHandles, FALSE, INFINITE);
		}
		else
		{
			dwWaitStatus = WaitForMultipleObjects(2, dwChangeHandles, FALSE, INFINITE);
			//dwWaitStatus = WaitForSingleObject(dwChangeHandles0, INFINITE);
		}

		switch (dwWaitStatus)
		{
		case WAIT_OBJECT_0:
		{
			//LOG(ERROR) << "WAIT_OBJECT_0";
			break;
		}
		case WAIT_OBJECT_0 + 1:
			//LOG(ERROR) << "WAIT_OBJECT_0 1";

			EnterCriticalSection(&(self->_cs));

			if (GetTickCount() - tk >= 1000)
			{
				Sleep(100);
				//LOG(ERROR) << "tk:" << tk;
				tk = GetTickCount();
				if (CefRefPtr<CefBrowser> browser = self->GetBrowser())
				{
					string js = "if(typeof update_files === \"function\") update_files();";
					browser->GetMainFrame()->ExecuteJavaScript(js, "", 0);
				}

			}

			LeaveCriticalSection(&(self->_cs));

			if (FindNextChangeNotification(dwChangeHandles[1]) == FALSE)
			{
			}
			//ExitProcess(GetLastError());
			break;

		case WAIT_OBJECT_0 + 2:
			//ÔÚC:\WINDOWSÄ¿Â¼ÖÐ´´½¨»òÉ¾³ýÎÄ¼þ ¡£  
			//Ë¢ÐÂ¸ÃÄ¿Â¼Ê÷¼°ÖØÆô¸ü¸ÄÍ¨Öª(change notification).  
			//LOG(ERROR) << "WAIT_OBJECT_0 2";

			EnterCriticalSection(&(self->_cs));

			if (GetTickCount() - tk >= 1000)
			{
				Sleep(100);
				//LOG(ERROR) << "tk:" << tk;
				tk = GetTickCount();

				if (CefRefPtr<CefBrowser> browser = self->GetBrowser())
				{
					string js = "if(typeof update_files === \"function\") update_files();";
					browser->GetMainFrame()->ExecuteJavaScript(js, "", 0);
				}

			}

			LeaveCriticalSection(&(self->_cs));


			if (FindNextChangeNotification(dwChangeHandles[2]) == FALSE)
			{

			}
			//ExitProcess(GetLastError());
			break;

		default:
			//ExitProcess(GetLastError());
			break;
		}
	}


	//CloseHandle(self->hEvent);

	return 0;
}

		//UINT monitor_files(HWND hWnd);
		//static UINT __stdcall monitor_files(LPVOID pParam);
		//static UINT __stdcall handle_change_files(LPVOID pParam);
		//static UINT __stdcall handle_change_files1(LPVOID pParam);
		//static int file_change(HWND hWnd, wstring iniPath);


HICON RootWindowWin::HICONFromHBITMAP(HBITMAP hBitmap)
{
	BITMAP bmp;
	GetObject(hBitmap, sizeof(BITMAP), &bmp);

	HBITMAP hbmMask = ::CreateCompatibleBitmap(::GetDC(NULL),
		bmp.bmWidth, bmp.bmHeight);

	ICONINFO ii = { 0 };
	ii.fIcon = TRUE;
	ii.hbmColor = hBitmap;
	ii.hbmMask = hbmMask;

	HICON hIcon = ::CreateIconIndirect(&ii);
	::DeleteObject(hbmMask);

	return hIcon;
}

				//HANDLE hMapFile = CreateFileMapping(
				//	INVALID_HANDLE_VALUE,    // ÎïÀíÎÄ¼þ¾ä±ú
				//	NULL,                    // Ä¬ÈÏ°²È«¼¶±ð
				//	PAGE_READWRITE/*|SEC_RESERVE*/,          // ¿É¶Á¿ÉÐ´
				//	0,                       // ¸ßÎ»ÎÄ¼þ´óÐ¡
				//	1024 * 64,                // µØÎ»ÎÄ¼þ´óÐ¡
				//	L"Paint_Mapping_Object"                   // ¹²ÏíÄÚ´æÃû³Æ
				//);

				//char *pBuf = (char *)MapViewOfFile(
				//	hMapFile,            // ¹²ÏíÄÚ´æµÄ¾ä±ú
				//	FILE_MAP_WRITE, // ¿É¶ÁÐ´Ðí¿É
				//	0,
				//	0,
				//	1024 * 64
				//);

				//UnmapViewOfFile(pBuf);
				//CloseHandle(hMapFile);

			//HANDLE hMapFile = CreateFileMapping(
			//	INVALID_HANDLE_VALUE,    // ÎïÀíÎÄ¼þ¾ä±ú
			//	NULL,                    // Ä¬ÈÏ°²È«¼¶±ð
			//	PAGE_READWRITE /*| SEC_RESERVE*/,          // ¿É¶Á¿ÉÐ´
			//	0,                       // ¸ßÎ»ÎÄ¼þ´óÐ¡
			//	1024 * 64,                // µØÎ»ÎÄ¼þ´óÐ¡
			//	L"Paint_Mapping_Object"                   // ¹²ÏíÄÚ´æÃû³Æ
			//);

			//char *pBuf = (char *)MapViewOfFile(
			//	hMapFile,            // ¹²ÏíÄÚ´æµÄ¾ä±ú
			//	FILE_MAP_WRITE, // ¿É¶ÁÐ´Ðí¿É
			//	0,
			//	0,
			//	1024 * 64
			//);

			//strncpy(pBuf, "close", 5);
			//pBuf[5] = 0;

			//UnmapViewOfFile(pBuf);
			//CloseHandle(hMapFile);
		//if (wParam == VK_F11) {
		//	if (flag_video_)
		//	{
		//	  return 0;
		//	}

		//	if(f11_)
		//	{
		//		f11_=false;
		//	}
		//	else
		//	{
		//		f11_=true;
		//	}
		//	CefRefPtr<CefBrowser> browser = self->GetBrowser();
		//	if (browser) {
		//		self->OnSetFullscreen(true);
		//	}
		//	return 0;
		//}

		//if (GetKeyState(VK_CONTROL) < 0 )
		//{
		//	CefRefPtr<CefBrowser> browser = self->GetBrowser();
		//	if (browser) {
		//		if (wParam == VK_ADD || wParam == VK_OEM_PLUS)
		//		{
		//			self->delegate_->OnTest(self, ID_PICOFU_ZOOM_ADD);
		//		}
		//		else if (wParam == VK_SUBTRACT || wParam == VK_OEM_MINUS)
		//		{
		//			self->delegate_->OnTest(self, ID_PICOFU_ZOOM_SUB);
		//		}
		//		else if (wParam == VK_NUMPAD0 || wParam == 0x30)
		//		{
		//			self->delegate_->OnTest(self, ID_PICOFU_ZOOM_100);
		//		}


		//		return 0;
		//	}
		//}

*/

	/*
	case LGC_EDIT_COI_HTML:		//lgc_edit_coi.html
	{
		if (CDataSrv::Instance()._readonly != 0)
		{
			response_dto.szResponse = "fail";
			response_dto.szContentType = "text/plain; charset=utf-8";
			return S_OK;
		}

		string reset = "";
		string id = "";
		lookitem(request_dto.szArgs, "reset=", reset);
		lookitem(request_dto.szArgs, "id=", id);
		if (reset == "1")
		{
			list<string> _lstFiles;
			list<string> _lstCoi;
			Cimagedal::Instance().get_files("0", id, "", _lstFiles, _lstCoi);

			wstring root = L"";
			wstring krf = L"";
			wstring szPath = get_module_path(NULL);
			if (CDataSrv::Instance()._albumName == "private")
			{
				root = szPath + L"\\data2";
			}
			else
			{
				root = szPath + L"\\data1";
			}
			krf = L".krf";

			IMAGE_DTO curr;
			curr.coi = "";
			curr.height = 0;
			curr.id = id;

			map<string, string> attr;
			list<string>::iterator iter;
			for (iter = _lstFiles.begin(); iter != _lstFiles.end(); iter++)
			{
				wstring ori = root + _u2w(*iter);

				wchar_t src[MAX_PATH];
				memset(src, 0, MAX_PATH * sizeof(wchar_t));
				DWORD dwSize = GetFullPathName(ori.c_str(), MAX_PATH, src, NULL);
				wstring file = src;
				_path2data(file);

				ReplaceOne(file, L".mp4", L".png");

				wstring dst = file + L"_coi.png" + krf;

				SetFileAttributes(dst.c_str(), FILE_ATTRIBUTE_NORMAL);
				DeleteFile(dst.c_str());

				map<string, string> roots = CDataSrv::Instance()._roots_map;
				map<string, string> ::iterator I;

				for (I = roots.begin(); I != roots.end(); I++)
				{
					string::size_type  pos = iter->find(I->first);
					if (pos != string::npos)
					{
						string t = *iter;
						ReplaceOne(t, I->first, "/");
						ori = _u2w(I->second + t);
						break;
					}
				}

				CHtmlupload::Instance().get_exif_value(ori, "1", attr);
			}

			if (!attr.empty())
			{
				map<string, string>::iterator I;
				for (I = attr.begin(); I != attr.end(); I++)
				{
					if (I->first == "width")
					{
						curr.width = atoi(I->second.c_str());
					}
					if (I->first == "height")
					{
						curr.height = atoi(I->second.c_str());
					}
					if (I->first == "rotation")
					{
						curr.rotation = atoi(I->second.c_str());
					}
				}
			}

			long hr = 1;
			if (curr.height > 0)
			{
				hr = Cimagedal::Instance().update_image(curr);
				CHtmlgetIndexProc::Instance().update_cache_image(curr);
			}
			else
			{
				hr = Cimagedal::Instance().update_images("coi", id, "");
			}

			if (hr == 0)
			{
				response_dto.szResponse = "ok";
			}
			else
			{
				response_dto.szResponse = "fail";
			}
			response_dto.szContentType = "text/plain; charset=utf-8";
		}
		else
		{
			CHtmlupload::Instance().post_img_say(request_dto, response_dto);
		}
		CHtmlgetIndexProc::Instance().clear_data();

		return S_OK;
	}
	
	string CHtmlgetIndexProc::encrypt_pwd(string pass)
{
	string opass = "";

	CBlowfish  * pBF = new CBlowfish();
	if (pBF != NULL)
	{
		pBF->SetPassword(BF_PASSWORD);

		div_t div_8 = div(pass.size(), 8);
		if (div_8.rem != 0)
		{
			div_8.quot += 1;
		}

		int len = div_8.quot * 8 + 1;

		char * ptr = (char*)malloc(len);
		if (ptr != NULL)
		{
			memset(ptr, 0, len);
			sprintf_s(ptr, len, "%s", pass.c_str());

			pBF->Encrypt((void*)ptr, div_8.quot * 8);

			char buf2[2048];
			memset(buf2, 0, 2048);
			for (int i = 0; i < div_8.quot * 8; i++)
			{
				sprintf_s(buf2 + i * 2, 2048 - i * 2, "%02x", ((unsigned char*)ptr)[i]);
			}

			opass = buf2;

			free(ptr);
		}

		delete pBF;
		pBF = NULL;
	}

	return opass;
}

string CHtmlgetIndexProc::decrypt_pwd(string opass)
{
	string pass = "";
	CBlowfish  * pBF = new CBlowfish();
	if (pBF != NULL)
	{
		pBF->SetPassword(BF_PASSWORD);

		unsigned char buf3[2048];
		memset(buf3, 0, 2048);
		hexchange(buf3, opass.c_str());

		pBF->Decrypt((void*)buf3, opass.length());

		pass = (char*)buf3;

		delete pBF;
		pBF = NULL;
	}

	return pass;
}

	
	
	
	
	
	
	
	
	
	
	*/
	//long CHtmlgetAdmin::syn_files_help(list<IMAGE_DTO> &lstImageDto)
	//{
	//	long ret = 0;
	//	//return 0;
	//	string albumName = CDataSrv::Instance()._albumName;
	//
	//	list<FILE_DTO> change_files;
	//	wstring szPath = get_module_path(NULL);
	//
	//	list<IMAGE_DTO>::iterator iter;
	//	for (iter = lstImageDto.begin(); iter != lstImageDto.end(); iter++)
	//	{
	//		string type = "upload_files";
	//
	//		string ori = albumName + iter->ori;
	//		wstring szFile = szPath + L"\\data\\" + _u2w(ori);
	//
	//		map<string, string> roots = CDataSrv::Instance()._roots_map;
	//		map<string, string> ::iterator I;
	//
	//		for (I = roots.begin(); I != roots.end(); I++)
	//		{
	//			string::size_type pos = iter->ori.find(I->first);
	//			if (pos != string::npos)
	//			{
	//				string t = iter->ori;
	//				ReplaceOne(t, I->first, "/");
	//				szFile = _u2w(I->second + t);
	//				type = "";
	//				break;
	//			}
	//		}
	//
	//		Replace(szFile, L"/", L"\\");
	//
	//		struct _stat32  info;
	//		int result = _wstat32(szFile.c_str(), &info);
	//		if (result == 0)
	//		{
	//			if (atoi(iter->md5sum.c_str()) != info.st_size)
	//			{
	//				FILE_DTO dto;
	//				dto.cat_id = type;
	//				dto.pic_id = iter->id;
	//				dto.filename = _w2u(szFile);
	//				change_files.push_back(dto);
	//			}
	//		}
	//	}
	//
	//	if (!change_files.empty())
	//	{
	//		string ids = "";
	//
	//		list<FILE_DTO>::iterator I;
	//		for (I = change_files.begin(); I != change_files.end(); I++)
	//		{
	//			list<string> _lstFiles;
	//			list<string> _lstCoi;
	//			Cimagedal::Instance().get_files("0", I->pic_id, "", _lstFiles, _lstCoi);
	//
	//			delete_image_files("syn", _lstFiles, _lstCoi);
	//
	//			ids += I->pic_id + ",";
	//
	//			REQUEST_DTO req_dto;
	//			req_dto.szArgs = "&pic_id=" + I->pic_id + "&type=" + I->cat_id;
	//			req_dto.szFile = _u2w(I->filename);
	//
	//			RESPONSE_DTO res_dto;
	//			long ret = CHtmlupload::Instance().post_upload_htm(req_dto, res_dto);
	//
	//			string::size_type pos = res_dto.szResponse.find("error");
	//			if (pos == string::npos)
	//			{
	//				//files = I->pic_id;
	//			}
	//		}
	//
	//		if (ids != "")
	//		{
	//			rtrim(ids, ",");
	//			list<IMAGE_DTO> tempDto;
	//			Cimagedal::Instance().get_images_by_ids(ids, tempDto);
	//
	//			list<IMAGE_DTO>::iterator I;
	//			for (I = tempDto.begin(); I != tempDto.end(); I++)
	//			{
	//				for (iter = lstImageDto.begin(); iter != lstImageDto.end(); iter++)
	//				{
	//					if (iter->id == I->id)
	//					{
	//						iter->rotation = I->rotation;
	//						iter->width = I->width;
	//						iter->height = I->height;
	//						iter->ratio = I->ratio;
	//						iter->coi = I->coi;
	//						ret = 1;
	//						break;
	//					}
	//				}
	//
	//			}
	//		}
	//	}
	//
	//	return ret;
	//}
	//private
	//long CHtmlgetAdmin::syn_files(string fpath, string &id)
	//{
	//	string szFile = fpath;
	//
	//	map<string, string> roots = CDataSrv::Instance()._roots_map;
	//	map<string, string> ::iterator I;
	//	for (I = roots.begin(); I != roots.end(); I++)
	//	{
	//		string::size_type pos = fpath.find(I->first);
	//		if (pos != string::npos)
	//		{
	//			string root_id = I->first;
	//			rtrim(root_id, "/");
	//
	//			ReplaceOne(szFile, I->second, root_id);
	//		}
	//		else
	//		{
	//			string szPath = _w2u(get_module_path(NULL)) + "\\data1";
	//			ReplaceOne(szFile, szPath, "");
	//		}
	//	}
	//
	//	Replace(szFile, "\\", "/");
	//
	//	list<IMAGE_DTO> lstImageDto;
	//	Cimagedal::Instance().get_images_by_url("|" + szFile + "|", lstImageDto);
	//
	//	if (!lstImageDto.empty())
	//	{
	//		syn_files_help(lstImageDto);
	//
	//		IMAGE_DTO dto = lstImageDto.front();
	//		id = dto.id;
	//		CHtmlgetIndexProc::Instance().update_cache_image(dto);
	//	}
	//
	//	return 0;
	//}
/*


long CHtmlgetAdmin::uninstall_root()
{
	string alb = "private";

	CDataSrv::Instance()._albumName = alb;
	int res = Cbasedal::Instance().refresh_db(alb.c_str());

	Cimagedal::Instance().set_alb(alb);
	Cchatdal::Instance().set_alb(alb);

	sqlite3 * pDb = Cbasedal::Instance().getDB();
	Cimagedal::Instance().setDb(pDb);
	Cchatdal::Instance().setDb(pDb);
	CDataSrv::Instance().refresh_gallery_title();

	string root_id = "";
	map<string, string> ::iterator I;
	for (I = CDataSrv::Instance()._roots_map.begin(); I != CDataSrv::Instance()._roots_map.end(); I++)
	{
		root_id = I->first;
		string root_dir = I->second;
		root_dir += "\";";

		string::size_type pos0 = root_id.find("_");
		if (pos0 != string::npos)
		{
			root_id = root_id.substr(pos0 + 1);
			rtrim(root_id, "/");

			list<CATEGORY_DTO> lstCategroy;
			Cchatdal::Instance().get_categories("", lstCategroy);

			string ids = "";
			list<CATEGORY_DTO>::iterator iter;
			for (iter = lstCategroy.begin(); iter != lstCategroy.end(); iter++)
			{
				if (iter->root_id == root_id)
				{
					ids += iter->id + ",";
				}
			}

			Cchatdal::Instance().del_root(root_id);

			if (ids != "")
			{
				ids = ids.substr(0, ids.length() - 1);
			}

			list<string> _lstFiles;
			list<string> _lstCoi;
			Cimagedal::Instance().get_files("0", "", ids, _lstFiles, _lstCoi);

			long hr = Cimagedal::Instance().update_images("delete_cat", ids, "");
			if (hr == 0)
			{
				delete_image_files("syn", _lstFiles, _lstCoi);
				if (CDataSrv::Instance()._albumName == "private")
				{
					decrypt_root(root_id, root_dir, _lstFiles);
				}

			}
		}

	}
	return 0;
}
long CLang::gen_language()
{
	wstring szPath = get_module_path(NULL);

	list<LANGUAGE_DTO> lstLanguage;
	Cchatdal::Instance().get_Language("", lstLanguage);

	list<LANGUAGE_DTO>::iterator I;
	for (I = lstLanguage.begin(); I != lstLanguage.end(); I++)
	{
		_langMap.clear();

		string name = "zh_CN";
		name = I->id;

		string lang = name + ".properties";
		proc_lang(lang, "gen");

		string strText = "";
		Read_file(L"html\\trans000.txt", strText);
		string prop = get_data(strText);

		DWORD dwWritten = 0;
		wstring src = szPath + L"\\templ\\locales\\";
		src += _u2w(name) + L".properties";

		HANDLE hWrite = CreateFile(src.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if (hWrite != INVALID_HANDLE_VALUE)
		{
			WriteFile(hWrite, prop.c_str(), prop.length(), &dwWritten, NULL);
			CloseHandle(hWrite);
		}
	}

	return 0;
}
bool CLang::proc_lang(string lang, string type)
{
	wstring	path = L"templ/locales/";
	if (type == "gen")
	{
		path = L"templ/language/";
	}
	path += _u2w(lang);

	string parData = "";
	Read_file(path, parData);

	if (type == "gen")
	{
		_parentLang = releaseLangData("0", parData);
		if (_parentLang != "")
		{
			string parData = "";
			path = L"templ/locales/" + _u2w(_parentLang) + L".properties";
			Read_file(path, parData);
			releaseLangData("1", parData);
		}

		if (parData == "" || lang == "ms_MY" || lang == "az_AZ")
		{
			parData = "";
			path = L"templ/locales/en_UK.properties";
			Read_file(path, parData);
			releaseLangData("1", parData);
		}
	}
	else
	{
		procLangData(parData);
	}
	return true;
}




string CLang::releaseLangData(string type, string common)
{
	string parentLang = "";

	//ReplaceOne(common, "$lang_info", "$lang");
	if (_genlang)
	{

		ReplaceOne(common, "['month'][1]", "['month1']");
		ReplaceOne(common, "['month'][2]", "['month2']");
		ReplaceOne(common, "['month'][3]", "['month3']");
		ReplaceOne(common, "['month'][4]", "['month4']");
		ReplaceOne(common, "['month'][5]", "['month5']");
		ReplaceOne(common, "['month'][6]", "['month6']");
		ReplaceOne(common, "['month'][7]", "['month7']");
		ReplaceOne(common, "['month'][8]", "['month8']");
		ReplaceOne(common, "['month'][9]", "['month9']");
		ReplaceOne(common, "['month'][10]", "['month10']");
		ReplaceOne(common, "['month'][11]", "['month11']");
		ReplaceOne(common, "['month'][12]", "['month12']");


		ReplaceOne(common, "['day'][0]", "['day0']");
		ReplaceOne(common, "['day'][1]", "['day1']");
		ReplaceOne(common, "['day'][2]", "['day2']");
		ReplaceOne(common, "['day'][3]", "['day3']");
		ReplaceOne(common, "['day'][4]", "['day4']");
		ReplaceOne(common, "['day'][5]", "['day5']");
		ReplaceOne(common, "['day'][6]", "['day6']");
	}

	string fout = "";
	int pattern = 3;

	char last = '\0';
	for (unsigned int i = 0; i < common.length(); i++)
	{
		StatusChange(fout, pattern, common[i], last);
	}

	string key = "";
	string val = "";
	vector<string> v1 = tokenize(fout, "\n", true, "");
	int len = v1.size();

	if (type == "0")
	{
		int len0 = len > 20 ? 20 : len;
		for (int i = 0; i < len0; i++)
		{
			int ndx = v1[i].find('=');
			if (ndx != -1)
			{
				// yup; save the args....
				val = v1[i].substr(ndx + 1);
				// strip from file name....
				key = v1[i].substr(0, ndx);

				trim(key, " ");
				//Replace(key, "\"", "\\\"");

				//	get parent $lang_info['parent']= "en_UK";
				if (key == "$lang_info['parent']")
				{
					parentLang = val;
					rtrim(parentLang, ";");
					trim(parentLang, " ");
					trim(parentLang, "'");
					trim(parentLang, "\"");
				}
				//else if (key == "$lang_info['direction']")
				//{
				//	string _direction = val;
				//	rtrim(_direction, ";");
				//	trim(_direction, " ");
				//	trim(_direction, "'");
				//	trim(_direction, "\"");
				//	_langMap.insert(map<string, string>::value_type("{'ltr'}", _direction));
				//}
				//else if (key == "$lang_info['code']")
				//{
				//	string _code = val;
				//	rtrim(_code, ";");
				//	trim(_code, " ");
				//	trim(_code, "'");
				//	trim(_code, "\"");
				//	_langMap.insert(map<string, string>::value_type("{'en'}", _code));
				//}
			}
		}
	}

	for (int i = len - 1; i >= 0; i--)
	{
		int ndx = v1[i].find('=');
		if (ndx != -1)
		{
			// yup; save the args....
			val = v1[i].substr(ndx + 1);
			// strip from file name....
			key = v1[i].substr(0, ndx);
			trim(key, " ");

			ReplaceOne(key, "$lang[", "{");
			ReplaceOne(key, "]", "}");

			rtrim(val, ";");
			trim(val, " ");
			trim(val, "'");
			trim(val, "\"");
			//Replace(val, "\"", "\\\"");

			_langMap.insert(map<string, string>::value_type(key, val));
		}
	}
	return parentLang;
}



*/

//string CLang::get_data(string &strText)
//{
//	list<string> lstKey;
//	Judge(strText, lstKey);
//
//	Replace(strText, "|@translate|escape", "");
//	Replace(strText, "|@translate", "");
//
//	string all = "";
//	list<string> lstKey2;
//
//	list<string>::iterator it;
//	list<string>::iterator iter;
//
//
//	for (iter = lstKey.begin(); iter != lstKey.end(); iter++)
//	{
//
//		bool b = false;
//		for (it = lstKey2.begin(); it != lstKey2.end(); it++)
//		{
//			if (*iter == *it)
//			{
//				b = true;
//				break;
//			}
//
//		}
//		if (!b)
//		{
//			lstKey2.push_back(*iter);
//		}
//	}
//
//
//	for (iter = lstKey2.begin(); iter != lstKey2.end(); iter++)
//	{
//		all += *iter;
//		all += " = ";
//
//		map<string, string>::iterator I = _langMap.find(*iter);
//		if (I != _langMap.end())
//		{
//			string szT = I->second;
//			trim(szT, " ");
//			all += "'";
//			all += szT;
//			all += "'\n";
//		}
//		else {
//
//			all += "''\n";
//		}
//	}
//
//	return all;
//}



//time_t CHtmlgetFolder::FileTimeToTime_t(FILETIME  ft)
//{
//	time_t  t;
//
//	LONGLONG  ll;
//
//	ULARGE_INTEGER            ui;
//	ui.LowPart = ft.dwLowDateTime;
//	ui.HighPart = ft.dwHighDateTime;
//
//	ll = (ft.dwHighDateTime << 32) + ft.dwLowDateTime;
//
//	t = ((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
//	return t;
//}



//string CImBase::get_time_string(time_t tt, string fmt)
//{
//	//string slongdate = "";
//	//char buf[100] = { 0 };
//	//int ret = GetLocaleInfoA(LOCALE_USER_DEFAULT,
//	//	LOCALE_SLONGDATE,
//	//	buf,
//	//	100);
//	//if (ret > 0)
//	//{
//	//	slongdate = buf;
//	//}
//
//	//dddd, MMMM d, yyyy
//
//	string strDate = "";
//
//	time_t current_time;
//	time(&current_time);
//
//	if (tt < 137000000 || tt > current_time + 3600 * 24 * 3650)
//	{
//		return "";
//	}
//
//	struct tm t;
//	localtime_s(&t, &tt);
//
//	if (t.tm_year == -1 || t.tm_mon < 0 || t.tm_mon >= 12)
//	{
//		return strDate;
//	}
//
//	char stamped[MAX_PATH];
//	memset(stamped, 0, MAX_PATH);
//
//	if (fmt == "%c")
//	{
//		strftime( stamped, MAX_PATH, "%Y:%m:%d %H:%M:%S", &t );
//		strDate = stamped;
//		return strDate;
//	}
//
//	string lang = CLang::Instance()._language;
//	if (lang.find("zh") != string::npos)
//	{
//		if (fmt == "0")
//		{
//			sprintf_s(stamped, MAX_PATH, " %04d{'Year'|@translate}%d{'Month'|@translate}%d{'Day'|@translate}, {'day%d'|@translate}",
//				t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_wday);
//		}
//		else if (fmt == "1")
//		{
//			sprintf_s(stamped, MAX_PATH, " %04d{'Year'|@translate}%d{'Month'|@translate}%d{'Day'|@translate}",
//				t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
//		}
//	}
//	else if (lang.find("ja") != string::npos)
//	{
//		if (fmt == "0")
//		{
//			sprintf_s(stamped, MAX_PATH, " %04d{'Year'|@translate}%d{'Month'|@translate}%d{'Day'|@translate}{'day%d'|@translate}",
//				t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_wday);
//		}
//		else if (fmt == "1")
//		{
//			sprintf_s(stamped, MAX_PATH, " %04d{'Year'|@translate}%d{'Month'|@translate}%d{'Day'|@translate}",
//				t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
//		}
//	}
//	else if (lang.find("es") != string::npos)
//	{
//		if (fmt == "0")
//		{
//			sprintf_s(stamped, MAX_PATH, " {'day%d'|@translate} %d de {'month%d'|@translate} de %04d",
//				t.tm_wday, t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);
//		}
//		else if (fmt == "1")
//		{
//			sprintf_s(stamped, MAX_PATH, " %d de {'month%d'|@translate} de %04d",
//				t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);
//		}
//	}
//	else if (lang.find("fr") != string::npos)
//	{
//		if (fmt == "0")
//		{
//			sprintf_s(stamped, MAX_PATH, " {'day%d'|@translate} %d {'month%d'|@translate} %04d",
//				t.tm_wday, t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);
//		}
//		else if (fmt == "1")
//		{
//			sprintf_s(stamped, MAX_PATH, " %d {'month%d'|@translate} %04d",
//				t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);
//		}
//	}
//	else if (lang.find("ru") != string::npos)
//	{
//		if (fmt == "0")
//		{
//			sprintf_s(stamped, MAX_PATH, " {'day%d'|@translate}, %d {'month%d'|@translate} %04d",
//				t.tm_wday, t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);
//		}
//		else if (fmt == "1")
//		{
//			sprintf_s(stamped, MAX_PATH, " %d {'month%d'|@translate} %04d",
//				t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);
//		}
//	}
//	else
//	{
//		LCID lcd = GetUserDefaultUILanguage();
//		if (lcd== 0x0409 || lang.find("de") != string::npos || lang.find("it") != string::npos)
//		{
//			if (fmt == "0")
//			{
//				sprintf_s(stamped, MAX_PATH, " {'day%d'|@translate}, {'month%d'|@translate} %d, %04d",
//					t.tm_wday, t.tm_mon + 1, t.tm_mday, t.tm_year + 1900);
//			}
//			else if (fmt == "1")
//			{
//				sprintf_s(stamped, MAX_PATH, " {'month%d'|@translate} %d, %04d",
//					t.tm_mon + 1, t.tm_mday, t.tm_year + 1900);
//			}
//
//		}
//		else
//		{
//			if (fmt == "0")
//			{
//				sprintf_s(stamped, MAX_PATH, " {'day%d'|@translate}, %d {'month%d'|@translate}, %04d",
//					t.tm_wday, t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);
//			}
//			else if (fmt == "1")
//			{
//				sprintf_s(stamped, MAX_PATH, " %d {'month%d'|@translate}, %04d",
//					t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);
//			}
//		}
//	}
//	strDate = stamped;
//
//	return strDate;
//}


/*

string CHtmlgetCal::get_calCalHead_string(QUERY_REQ req, string type, string cal, CAL_DTO dto, int &nb, int &mon, string templ)
{
	string calCalHead = "";
	if (dto.year != mon)
	{
		string tem = templ;
		mon = dto.year;
		string url = get_cal_url(req, type, cal, uint2str(dto.year));

		ReplaceOne(tem, "{$url}", url);

		ReplaceOne(tem, "{$ID}", uint2str(dto.year));
		ReplaceOne(tem, "{$nb}", uint2str(dto.nb_year_images));
		calCalHead = tem;
	}
	return calCalHead;
}

string CHtmlgetCal::get_calCalHead_string2(QUERY_REQ req, string type, string cal, CAL_DTO dto, int &nb, string templ)
{
	string calCalHead = "";
	string tem = templ;
	string url = get_cal_url(req, type, cal, uint2str(dto.year) + "-" + uint2str(dto.month));
	ReplaceOne(tem, "{$url}", url);

	ReplaceOne(tem, "{$ID}", uint2str(dto.month));
	ReplaceOne(tem, "{$nb}", uint2str(dto.nb_month_images));

	calCalHead = tem;
	nb += dto.nb_month_images;

	return calCalHead;
}


long IsoHandler::add_dir_bin(IFsiDirectoryItem* pRootDirItem, const wchar_t* pSrcDir)
{
	HRESULT hr;

	IFsiItem* subdir = NULL;
	hr = pRootDirItem->AddDirectory(CComBSTR(L"bin"));
	hr = pRootDirItem->get_Item(CComBSTR(L"bin"), &subdir);
	if (hr == S_OK)
	{
		mySendMessage(pSrcDir, L"\\bin");

		wchar_t pDir[MAX_PATH];
		memset(pDir, 0, sizeof(wchar_t) * MAX_PATH);
		wcscpy_s(pDir, MAX_PATH, pSrcDir);
		wcscat_s(pDir, MAX_PATH, L"\\bin");

		list<wstring> lstFile;
		GetFolderFiles(pDir, lstFile);

		list<wstring>::iterator it;
		for (it = lstFile.begin(); it != lstFile.end(); it++)
		{
			const wchar_t* ptr = wcsrchr(it->c_str(), L'\\') + 1;

			if (_wcsicmp(ptr, L"Cache") != 0)
			{
				IStream* pFileS = NULL;
				SHCreateStreamOnFile(it->c_str(), STGM_SHARE_DENY_WRITE, &pFileS);
				if (pFileS != NULL)
				{
					hr = ((IFsiDirectoryItem*)subdir)->AddFile(CComBSTR(ptr), pFileS);
					pFileS->Release();
				}
			}

			if (_cancel)
			{
				hr = S_FALSE;
				break;
			}
		}

		//
		memset(pDir, 0, sizeof(wchar_t) * MAX_PATH);
		wcscpy_s(pDir, MAX_PATH, pSrcDir);
		wcscat_s(pDir, MAX_PATH, L"\\bin\\locales");
		hr = ((IFsiDirectoryItem*)subdir)->AddTree(CComBSTR(pDir), VARIANT_TRUE);
		mySendMessage(pSrcDir, L"\\bin\\locales");

		memset(pDir, 0, sizeof(wchar_t) * MAX_PATH);
		wcscpy_s(pDir, MAX_PATH, pSrcDir);
		wcscat_s(pDir, MAX_PATH, L"\\bin\\swiftshader");
		hr = ((IFsiDirectoryItem*)subdir)->AddTree(CComBSTR(pDir), VARIANT_TRUE);

		mySendMessage(pSrcDir, L"\\bin\\swiftshader");

		//link
		memset(pDir, 0, sizeof(wchar_t) * MAX_PATH);
		wcscpy_s(pDir, MAX_PATH, pSrcDir);
		wcscat_s(pDir, MAX_PATH, L"\\launch.exe");

		IStream* pFileS = NULL;
		SHCreateStreamOnFile(CComBSTR(pDir), STGM_SHARE_DENY_WRITE, &pFileS);
		if (pFileS != NULL)
		{
			hr = pRootDirItem->AddFile(CComBSTR(L"launch.exe"), pFileS);
			pFileS->Release();
		}

		subdir->Release();
	}

	return hr;
}

long IsoHandler::add_dir_templ(IFsiDirectoryItem* pRootDirItem, const wchar_t* pSrcDir)
{
	HRESULT hr;
	wchar_t pDir[MAX_PATH];

	//memset(pDir, 0, sizeof(wchar_t) * MAX_PATH);
	//wcscpy_s(pDir, MAX_PATH, pSrcDir);
	//wcscat_s(pDir, MAX_PATH, L"\\html");
	//hr = pRootDirItem->AddTree(CComBSTR(pDir), VARIANT_TRUE);

	hr = pRootDirItem->AddDirectory(CComBSTR(L"templ"));

	IFsiItem* subdir = NULL;
	hr = pRootDirItem->get_Item(CComBSTR(L"templ"), &subdir);
	if (hr == S_OK)
	{
		memset(pDir, 0, sizeof(wchar_t) * MAX_PATH);
		wcscpy_s(pDir, MAX_PATH, pSrcDir);
		wcscat_s(pDir, MAX_PATH, L"\\templ");

		list<wstring> lstFile;
		GetFolderDirs(pDir, lstFile);
		list<wstring>::iterator it;
		for (it = lstFile.begin(); it != lstFile.end(); it++)
		{
			memset(pDir, 0, sizeof(wchar_t) * MAX_PATH);
			wcscpy_s(pDir, MAX_PATH, it->c_str());

			hr = ((IFsiDirectoryItem*)subdir)->AddTree(CComBSTR(pDir), VARIANT_TRUE);

			mySendMessage(pDir, L"");

			if (_cancel)
			{
				hr = S_FALSE;
				break;
			}
		}
		subdir->Release();

	}
	return hr;
}


long IsoHandler::add_dir_db(IFsiDirectoryItem* pRootDirItem, const wchar_t* pSrcDir, wchar_t* pName)
{
	HRESULT hr;
	wchar_t pDir[MAX_PATH];

	wstring dd = L"data2";
	if (wcscmp(pName, L"public") == 0)
	{
		dd = L"data1";
	}

	IFsiItem* subdir = NULL;

	hr = pRootDirItem->get_Item(CComBSTR(dd.c_str()), &subdir);
	if (hr == S_OK)
	{
		//IFsiItem *subdir1 = NULL;
		//hr = ((IFsiDirectoryItem*)subdir)->get_Item(CComBSTR(pName), &subdir1);
		//if (hr == S_OK)
		{
			hr = ((IFsiDirectoryItem*)subdir)->AddDirectory(CComBSTR(L"temp"));

			hr = ((IFsiDirectoryItem*)subdir)->AddDirectory(CComBSTR(L"_db"));
			IFsiItem* subdir2 = NULL;
			hr = ((IFsiDirectoryItem*)subdir)->get_Item(CComBSTR(L"_db"), &subdir2);
			if (hr == S_OK)
			{

				memset(pDir, 0, sizeof(wchar_t) * MAX_PATH);
				wcscpy_s(pDir, MAX_PATH, pSrcDir);
				wcscat_s(pDir, MAX_PATH, L"\\");
				wcscat_s(pDir, MAX_PATH, dd.c_str());
				if (_wcsicmp(pName, L"public") == 0)
				{
					wcscat_s(pDir, MAX_PATH, L"\\temp\\");
					wcscat_s(pDir, MAX_PATH, L"photo1.db");
				}
				else
				{
					wcscat_s(pDir, MAX_PATH, L"\\temp\\");
					wcscat_s(pDir, MAX_PATH, L"photo2.db");
				}

				IStream* pFileS = NULL;
				SHCreateStreamOnFile(CComBSTR(pDir), STGM_SHARE_DENY_WRITE, &pFileS);
				if (pFileS != NULL)
				{
					if (_wcsicmp(pName, L"public") == 0)
					{
						hr = ((IFsiDirectoryItem*)subdir2)->AddFile(CComBSTR(L"photo1.db"), pFileS);
					}
					else
					{
						hr = ((IFsiDirectoryItem*)subdir2)->AddFile(CComBSTR(L"photo2.db"), pFileS);
					}
					pFileS->Release();
				}

				subdir2->Release();
			}
			//subdir1->Release();
		}

		subdir->Release();
	}

	return hr;
}

long IsoHandler::add_dir_data(IFsiDirectoryItem* pRootDirItem, const wchar_t* pSrcDir, wchar_t* pName, wchar_t* pSubName)
{
	HRESULT hr;
	wchar_t pDir[MAX_PATH];

	wstring dd = L"data2";
	if (wcscmp(pName, L"public") == 0)
	{
		dd = L"data1";
	}

	IFsiItem* subdir = NULL;

	hr = pRootDirItem->get_Item(CComBSTR(dd.c_str()), &subdir);
	if (hr == S_OK)
	{
		//IFsiItem *subdir1 = NULL;
		//hr = ((IFsiDirectoryItem*)subdir)->get_Item(CComBSTR(pName), &subdir1);
		//if (hr == S_OK)
		{
			hr = ((IFsiDirectoryItem*)subdir)->AddDirectory(CComBSTR(pSubName));

			IFsiItem* subdir2 = NULL;
			hr = ((IFsiDirectoryItem*)subdir)->get_Item(CComBSTR(pSubName), &subdir2);
			if (hr == S_OK)
			{
				memset(pDir, 0, sizeof(wchar_t) * MAX_PATH);
				wcscpy_s(pDir, MAX_PATH, pSrcDir);
				wcscat_s(pDir, MAX_PATH, L"\\");
				wcscat_s(pDir, MAX_PATH, dd.c_str());
				wcscat_s(pDir, MAX_PATH, L"\\");
				wcscat_s(pDir, MAX_PATH, pSubName);

				list<wstring> lstYear;
				GetFolderDirs(pDir, lstYear);

				list<wstring>::iterator it;
				for (it = lstYear.begin(); it != lstYear.end(); it++)
				{
					memset(pDir, 0, sizeof(wchar_t) * MAX_PATH);
					wcscpy_s(pDir, MAX_PATH, it->c_str());

					hr = ((IFsiDirectoryItem*)subdir2)->AddTree(CComBSTR(pDir), VARIANT_TRUE);

					mySendMessage(pDir, L"");

					if (_cancel)
					{
						hr = S_FALSE;
						break;
					}
				}
				subdir2->Release();
			}
			//subdir1->Release();
		}

		subdir->Release();
	}

	return hr;
}

long IsoHandler::add_dir_data(IFsiDirectoryItem* pRootDirItem, const wchar_t* pSrcDir)
{
	HRESULT hr;
	wchar_t pDir[MAX_PATH];

	IFsiItem* subdir = NULL;

	hr = pRootDirItem->get_Item(CComBSTR(L"original_files"), &subdir);
	if (hr == S_OK)
	{
		const wchar_t* ptr = wcsrchr(pSrcDir, L'\\') + 1;
		hr = ((IFsiDirectoryItem*)subdir)->AddDirectory(CComBSTR(ptr));

		IFsiItem* subdir2 = NULL;
		hr = ((IFsiDirectoryItem*)subdir)->get_Item(CComBSTR(ptr), &subdir2);
		if (hr == S_OK)
		{
			list<wstring> lstYear;
			GetFolderDirs(pSrcDir, lstYear);

			list<wstring>::iterator it;
			for (it = lstYear.begin(); it != lstYear.end(); it++)
			{

				memset(pDir, 0, sizeof(wchar_t) * MAX_PATH);
				wcscpy_s(pDir, MAX_PATH, it->c_str());

				hr = ((IFsiDirectoryItem*)subdir2)->AddTree(CComBSTR(pDir), VARIANT_TRUE);

				mySendMessage(pDir, L"");

				if (_cancel)
				{
					hr = S_FALSE;
					break;
				}
			}
			subdir2->Release();
		}

		subdir->Release();
	}

	return hr;
}


UINT IsoHandler::SaveIso1()
{
	_start = true;

	HRESULT					hr;
	IFileSystemImage* pSystemImage = NULL;
	IFsiDirectoryItem* pRootDirItem = NULL;
	IFileSystemImageResult* pSystemImageResult = NULL;
	IStream* pImageStream = NULL;
	IStream* pFileStream = NULL;

	hr = CoCreateInstance(__uuidof(MsftFileSystemImage),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IFileSystemImage),
		(LPVOID*)&pSystemImage);
	if (SUCCEEDED(hr))
	{
		pSystemImage->put_VolumeName(CComBSTR(_isofile.c_str()));
		pSystemImage->ChooseImageDefaultsForMediaType(IMAPI_MEDIA_TYPE_DISK);

		int fsi = FsiFileSystemJoliet | FsiFileSystemISO9660;
		pSystemImage->put_FileSystemsToCreate((FsiFileSystems)fsi);

		pSystemImage->get_Root(&pRootDirItem);

		if (!_cancel)
		{
			hr = add_dir_bin(pRootDirItem, _pSrcDir);

		}

		if (!_cancel)
		{
			hr = add_dir_templ(pRootDirItem, _pSrcDir);
		}

		wstring dDir = (_albumName == "private") ? L"data2" : L"data1";

		hr = pRootDirItem->AddDirectory(CComBSTR(dDir.c_str()));

		IFsiItem* subdir = NULL;
		hr = pRootDirItem->get_Item(CComBSTR(dDir.c_str()), &subdir);
		if (hr == S_OK)
		{
			if (_albumName == "private")
			{
				//hr = ((IFsiDirectoryItem*)subdir)->AddDirectory(CComBSTR(L"private"));
				//if (hr == S_OK)
				{
					if (!_cancel)
					{
						hr = add_dir_db(pRootDirItem, _pSrcDir, L"private");
					}

					//if (!_cancel)
					//{
					//	hr = add_dir_data(pRootDirItem, _pSrcDir, L"private", L"_data");
					//}

					if (!_cancel)
					{
						hr = add_dir_data(pRootDirItem, _pSrcDir, L"private", L"_thumb");
					}
				}
			}
			else
			{
				//hr = ((IFsiDirectoryItem*)subdir)->AddDirectory(CComBSTR(L"public"));
				//if (hr == S_OK)
				{
					if (!_cancel)
					{
						hr = add_dir_db(pRootDirItem, _pSrcDir, L"public");
					}

					if (!_cancel)
					{
						hr = add_dir_data(pRootDirItem, _pSrcDir, L"public", L"_data");
					}

					if (!_cancel)
					{
						hr = add_dir_data(pRootDirItem, _pSrcDir, L"public", L"_thumb");
					}
				}
			}

			subdir->Release();
		}

		hr = pRootDirItem->AddDirectory(CComBSTR(L"original_files"));
		if (hr == S_OK)
		{
			if (!_cancel)
			{
				map<string, string> ::iterator I;
				for (I = _roots_map.begin(); I != _roots_map.end(); I++)
				{
					hr = add_dir_data(pRootDirItem, _u2w(I->second).c_str());
				}
			}
		}


		mySendMessage(L"write", L"");

		if (!_cancel)
		{
			hr = pSystemImage->CreateResultImage(&pSystemImageResult);
			if (SUCCEEDED(hr))
			{
				long bloksize = 0;
				long totalblocks = 0;
				pSystemImageResult->get_BlockSize(&bloksize);
				pSystemImageResult->get_TotalBlocks(&totalblocks);

				hr = pSystemImageResult->get_ImageStream(&pImageStream);
				if (SUCCEEDED(hr))
				{
					STATSTG			statstg;
					SHCreateStreamOnFileEx(_pIsoPath,
						STGM_CREATE | STGM_WRITE | STGM_DIRECT | STGM_SHARE_EXCLUSIVE,
						FILE_ATTRIBUTE_NORMAL,
						TRUE,
						NULL,
						&pFileStream);

					if (pFileStream != NULL)
					{
						pImageStream->Stat(&statstg, STATFLAG_DEFAULT);

						ULONGLONG t = (ULONGLONG)totalblocks * bloksize;
						if (statstg.cbSize.QuadPart == t)
						{
							ldiv_t div1 = ldiv(totalblocks, 100);
							int step = div1.quot;

							unsigned char* buf = (unsigned char*)malloc(bloksize);

							for (int i = 0; i < totalblocks; i++)
							{
								ULONG pcbRead;
								ULONG pcbWrite;

								memset(buf, 0, bloksize);
								pImageStream->Read(buf, bloksize, &pcbRead);

								pFileStream->Write(buf, pcbRead, &pcbWrite);

								div_t div0 = div(i, step);
								if (div0.rem == 0)
								{
									if (i > 0)
									{
										wchar_t pDir[MAX_PATH];
										memset(pDir, 0, sizeof(wchar_t) * MAX_PATH);
										swprintf_s(pDir, MAX_PATH, L"%d", i);
										mySendMessage(pDir, L"");
									}
								}

								if (_cancel)
								{
									break;
								}
							}

							free(buf);
						}

						pFileStream->Release();
					}

					pImageStream->Release();
				}

				pSystemImageResult->Release();
			}
		}

		pRootDirItem->Release();
		pSystemImage->Release();
	}

	wchar_t pDir[MAX_PATH];
	memset(pDir, 0, sizeof(wchar_t) * MAX_PATH);
	wcscpy_s(pDir, MAX_PATH, _pSrcDir);

	if (_albumName == "private")
	{
		wcscat_s(pDir, MAX_PATH, L"\\data2\\temp\\photo2.db");
	}
	else
	{
		wcscat_s(pDir, MAX_PATH, L"\\data1\\temp\\photo1.db");
	}
	DeleteFile(pDir);

	if (_cancel)
	{
		mySendMessage(L"cancel", L"");
	}
	else
	{
		mySendMessage(L"ok", L"");
	}

	_start = false;
	return 0;
}


wstring IsoHandler::regGetBuyURL(wstring publisher, wstring appName, wstring appVer)
{
	HKEY hKey = NULL;
	wstring buyURL = L"";
	wchar_t buf[1024];
	memset(buf, 0, 1024 * sizeof(wchar_t));

	// form the registry key path
	wstring keyPath = L"SOFTWARE\\Digital River\\SoftwarePassport\\" + publisher + L"\\" + appName + L"\\" + appVer;

	// read the "BuyURL" value from HKEY_LOCAL_MACHINE branch first
	LONG lRet = RegOpenKeyExW(HKEY_LOCAL_MACHINE, keyPath.c_str(), 0, KEY_READ, &hKey);
	if (lRet != ERROR_SUCCESS) {
		// fail to read from HKEY_LOCAL_MACHINE branch, try HKEY_CURRENT_USER
		lRet = RegOpenKeyExW(HKEY_CURRENT_USER, keyPath.c_str(), 0, KEY_READ, &hKey);
	};

	if (lRet == ERROR_SUCCESS) {
		//LPSTR   lpBuyURL   = buyURL.GetBuffer( 1024 );
		DWORD   dwSize = 1024;

		// Read the "BuyURL" value from the registry key.
		RegQueryValueExW(hKey, L"BuyURL", 0, NULL, (LPBYTE)buf, &dwSize);
		//buyURL.ReleaseBuffer();

		RegCloseKey(hKey);
	}

	buyURL = buf;

	return buyURL;
}
   <!--       <span class="cmdTrash" title="{'Edit'|@translate}" style="visibility: visible; transform: rotate(180deg);vertical-align:middle;">
        <a href="javascript:;" onclick="trash_click();return false;" title="{'Edit'|@translate}" class="stm-state-default stm-button"><span class="stm-icon stm-icon-category-edit"></span><span class="stm-button-text">{'Edit'|@translate}</span>
        </a>
      </span>-->
<!-- <ul style="display:none;">
              <li><a href="javascript:;" onclick="mbprofile();return false;" title="{'Settings'|@translate}">{'Settings'|@translate}</a></li>
          </ul>-->



long CHtmlgetIndexProc::proc_art_film(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto)
{
	string ids = "";
	string reload = "";
	lookitem(request_dto.szArgs, "ids=", ids);
	lookitem(request_dto.szArgs, "reload=", reload);
	rtrim(ids, ",");

	list<IMAGE_DTO> lstOrder;
	uint64_t md5sum = get_md5sum(ids, lstOrder);

	string title1 = "";
	string title2 = "";
	string musci_src = "";
	string artid = "";

	list<FILM_DTO> lstfilm;
	Cchatdal::Instance().get_films("", md5sum, lstfilm);
	if (!lstfilm.empty())
	{
		FILM_DTO dto = lstfilm.front();
		title1 = dto.title1;
		title2 = dto.title2;
		musci_src = dto.music;
		artid = uint2str(dto.id);
	}

	string ids0 = "";
	string pics = "";

	uint32_t nb = (uint32_t)lstOrder.size();

	uint32_t total = 0;
	string widths = "";
	string heights = "";

	uint32_t width = 1024;
	uint32_t height = 512;

	//wstring szPath = get_module_path(NULL);

	//wstring data = L"/data1";
	//if (CDataSrv::Instance()._albumName == "private")
	//{
	//	data = L"/data2";
	//}

	list<IMAGE_DTO>::iterator iter;
	for (iter = lstOrder.begin(); iter != lstOrder.end(); iter++)
	{
		get_size(*iter, width, height);

		wstring ori = _u2w(iter->ori);
		if (iter->coi != "")
		{
			ori = _u2w(iter->coi);
		}

		string temp0 = ori2path(1, iter->coi != "" ? iter->coi : iter->ori);
		wstring src = _u2w(temp0);
		wstring temp = szPath + data + L"/_temp/0.jpg";

		int ret = resize_img2(src.c_str(), temp.c_str(), width, height);

		wstring dst = szPath + data + L"/_thumb" + ori + L"_3.jpg.krf";
		ReplaceOne(dst, L"/_data/", L"/");

		CUploadParser::Instance().Encryptfile(temp, dst);
		my_DeleteFile(temp);

		string url = CDataSrv::Instance().get_image_url(9, *iter);
	
		string url = CDataSrv::Instance().get_image_url(1, *iter);

		pics += "\"" + url + "\",";
		ids0 += iter->id + ",";

		widths += uint2str(width) + ",";
		heights += uint2str(height) + ",";
		total++;

		if (title1.empty())
		{
			title1 = get_title(CDataSrv::Instance()._q_req);
			title2 = get_time_string(iter->date_creation, "1");
		}
	}

	rtrim(ids0, ",");
	rtrim(pics, ",");
	rtrim(widths, ",");
	rtrim(heights, ",");

	string tpl = "";
	Htmlread(IDR_ART_FILM_HTML, L"html/_art_film.html", tpl);

	ReplaceOne(tpl, "$reload", reload);
	ReplaceOne(tpl, "{$musci_src}", musci_src);
	ReplaceOne(tpl, "$total", uint2str(total));
	ReplaceOne(tpl, "$nb", uint2str(nb));

	ReplaceOne(tpl, "$title1", title1);
	ReplaceOne(tpl, "$title2", title2);
	ReplaceOne(tpl, "$artid", artid);

	ReplaceOne(tpl, "$widths", widths);
	ReplaceOne(tpl, "$heights", heights);

	uint32_t r = CDataSrv::Instance()._readonly;
	ReplaceOne(tpl, "{$readonly}", uint2str(r));
	ReplaceOne(tpl, "{$error_readonly}", CDataSrv::Instance()._str_readonly);

	//2
	string picture_next = "";
	string picture_pre = "";

	CLang::Instance().getOneData("picture_pre3", picture_pre);
	CLang::Instance().getOneData("picture_next3", picture_next);

	ReplaceOne(picture_next, "g='next'", "style=\"display:none;\"");
	ReplaceOne(picture_pre, "g='pre'", "style=\"display:none;\"");

	string calItem = "";
	calItem += picture_pre;
	calItem += picture_next;

	string calendar_nav = "";
	CLang::Instance().getOneData("calendar_nav", calendar_nav);
	ReplaceOne(calendar_nav, "{$calItem}", calItem);

	ReplaceOne(tpl, "{$calendar_nav}", calendar_nav);

	translate(tpl, "", "");

	ReplaceOne(tpl, "$ids", ids0);
	ReplaceOne(tpl, "$pics", pics);

	response_dto.szResponse = tpl;
	response_dto.szContentType = "text/html";

	return S_OK;
}
*/

