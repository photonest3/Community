// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SCHEME_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SCHEME_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifdef SCHEME_EXPORTS
#define SCHEME_API __declspec(dllexport)
#else
#define SCHEME_API __declspec(dllimport)
#endif

SCHEME_API	long mygetfunc(const char *szUrl, char **szResponse, char **szMimeType);
SCHEME_API	long mypostfunc(const char *szUrl, const char *szBody, const wchar_t *szFile, char **szResponse, char **szMimeType);
