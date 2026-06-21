
#pragma once  

#include <time.h>
#include <string>
#include <map>
#include <cctype>
#include <algorithm>
#include <vector>

using namespace std;

int HexDigit(char c);
string URLDecode(string& sURL);

void string2map(const wchar_t* szSyn, map<string, string>& m);

vector<string> tokenize(const string& src, string tok, bool btrim, string null_subst);
wstring ReplaceOne(wstring& szBody, wstring szOld, wstring szNew);
string ReplaceOne(string& szBody, string szOld, string szNew);
wstring Replace(wstring& szBody, wstring szOld, wstring szNew);

wstring get_module_path();
string get_module_pathA();

long transl(const char* szSrc, wstring& szDst);
void transl2(HWND hDlg, UINT id, int isBtn = 0);
void transl2(HWND hDlg, UINT id, wchar_t* txt);
bool isurlok(string val);
