#pragma once

#include <string>
#include <cctype>
#include <algorithm>
#include <vector>


using namespace std;

string _w2u(wstring w);
wstring _u2w(string u);

wstring Replace(wstring& szBody, wstring szOld, wstring szNew);
string ReplaceOne(string& szBody, string szOld, string szNew);

string trim(string& s, const string drop);
string ltrim(string& s, const string drop);
string rtrim(string& s, const string drop);

string my_tolower(string& s);
vector<string> tokenize(const string& src, string tok, bool btrim, string null_subst);
vector<wstring> tokenize(const wstring& src, wstring tok, bool btrim, wstring null_subst);

int hexchange(unsigned char* dest, const char* src);
unsigned short checksum(USHORT* buffer, int size);

wstring get_module_path0(HMODULE hModule);
wstring get_module_path(HMODULE hModule);
wstring get_appdata_path(string alb);

string Int2Str(int num);
