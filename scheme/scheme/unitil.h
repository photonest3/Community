#pragma once

#include <string>
#include <cctype>
#include <algorithm>
#include <vector>


using namespace std;

string _w2u(wstring w);
wstring _u2w(string u);

int HexDigit(char c);
string URLDecode(string& sURL, int type);
string URLDecode2(string& sURL);

string Replace(string& szBody, string szOld, string szNew);
wstring Replace(wstring& szBody, wstring szOld, wstring szNew);
string ReplaceOne(string& szBody, string szOld, string szNew);
wstring ReplaceOne(wstring& szBody, wstring szOld, wstring szNew);

string trim(string& s, const string drop);
string ltrim(string& s, const string drop);
string rtrim(string& s, const string drop);

int lookitem(string sz1, const char* szname, string& value, int nstart = 0);
wstring my_tolower(wstring& s);

string my_tolower(string& s);
vector<string> tokenize(const string& src, string tok, bool btrim, string null_subst);

int hexchange(unsigned char* dest, const char* src);
unsigned short checksum(USHORT* buffer, int size);

wstring get_module_path0(HMODULE hModule);
wstring get_module_path(HMODULE hModule);
wstring get_appdata_path(string alb);

string float2Str(float num, char* fmt);

string Int2Str(int num);
string Int2Str(uint32_t num);
string uint2str(uint32_t num);
string uint64str(uint64_t num);
uint32_t get_uint32(char* ptr);
uint32_t get_uint32(const char* ptr);
uint64_t get_uint64(char* ptr);

void get_request_url(const char* szUrl, string& strhost, string& strUrl, string& strArgs);
long lookitem_arr(string szBody, const char* szname, string& value);
string myEntity(string& s);
string _data2_thumb(string& src);


char* get_value(char* ptr);

float get_float(char* ptr);
string get_today(time_t tt);
