#pragma once

#include <string>
#include <cctype>
#include <algorithm>
#include <vector>

using namespace std;

string _w2u(wstring w);
wstring _u2w(string u);

wstring Replace(wstring &szBody, wstring szOld, wstring szNew);
string trim(string& s, const string drop);

vector<string> tokenize(const string& src, string tok, bool btrim, string null_subst);
wstring get_module_path(HMODULE hModule);
