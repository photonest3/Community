
#define LOCALES_EXPORTS

#ifdef LOCALES_EXPORTS
#define LOCALES_API __declspec(dllexport)
#else
#define LOCALES_API __declspec(dllimport)
#endif


LOCALES_API long Read_xml(unsigned int cr_id, const wchar_t* filename, char** szhtml);
