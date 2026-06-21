
#include "precomp.hpp"
#include "persistence.hpp"
#include <windows.h>

namespace cv
{

namespace fs
{


int utf8_to_wcs(const char* str1, wchar_t *str2)
{
 int wlen = MultiByteToWideChar(CP_UTF8, 0, str1, -1, 0, 0);
  
  MultiByteToWideChar(CP_UTF8, 0, str1, -1, str2, wlen);
  return wlen;
}

}

}
