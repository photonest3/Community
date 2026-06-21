
#define TEMPL_EXPORTS

#ifdef TEMPL_EXPORTS
#define TEMPL_API __declspec(dllexport)
#else
#define TEMPL_API __declspec(dllimport)
#endif


TEMPL_API long Read_htm(unsigned int cr_id, const wchar_t* filename, char** szhtml);
