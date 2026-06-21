///////////////////////////////////////////////////////////////////////////////
// 文件名: scheme.h
// 模块: scheme/scheme
// 功能描述: DLL 导出宏和 API 函数声明
//           定义 SCHEME_API 宏，声明 HTTP 请求处理接口
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

// 以下 ifdef 块是创建宏的标准方式，用于简化从 DLL 导出。
// 在命令行上使用 SCHEME_EXPORTS 符号编译的所有文件
// 都会将 SCHEME_API 定义为 __declspec(dllexport)。
// 使用此 DLL 的所有其他项目都会将 SCHEME_API 定义为 __declspec(dllimport)。
// 这样，任何使用此 DLL 的其他项目都会将 SCHEME_API 函数视为从 DLL 导入，
// 而此 DLL 会将符号视为导出。

#ifdef SCHEME_EXPORTS
#define SCHEME_API __declspec(dllexport)
#else
#define SCHEME_API __declspec(dllimport)
#endif

/// @brief HTTP GET 请求处理函数（DLL 导出）
/// @param[in] szUrl 请求 URL
/// @param[out] szResponse 响应内容指针的指针（输出）
/// @param[out] szMimeType MIME 类型指针的指针（输出）
/// @return 0: 成功; 其他: 错误码
/// @note 处理 HTTP GET 请求并返回响应内容和 MIME 类型
/// @warning szResponse 和 szMimeType 的内存由调用方释放
SCHEME_API	long mygetfunc(const char *szUrl, char **szResponse, char **szMimeType);

/// @brief HTTP POST 请求处理函数（DLL 导出）
/// @param[in] szUrl 请求 URL
/// @param[in] szBody POST 请求体
/// @param[in] szFile 上传文件路径（宽字符）
/// @param[out] szResponse 响应内容指针的指针（输出）
/// @param[out] szMimeType MIME 类型指针的指针（输出）
/// @return 0: 成功; 其他: 错误码
/// @note 处理 HTTP POST 请求（支持文件上传）并返回响应内容和 MIME 类型
/// @warning szResponse 和 szMimeType 的内存由调用方释放
SCHEME_API	long mypostfunc(const char *szUrl, const char *szBody, const wchar_t *szFile, char **szResponse, char **szMimeType);
