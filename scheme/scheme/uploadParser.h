///////////////////////////////////////////////////////////////////////////////
// 文件名: uploadParser.h
// 模块: scheme/scheme
// 功能描述: 文件上传加密/解密解析类
//           使用 Blowfish 算法加密和解密上传的文件
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-04-10  PhotoNest Team  添加文件加密功能
//   2024-06-01  PhotoNest Team  优化解密性能
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include "Singleton.h"
#include "Blowfish.h"

using namespace std;

/// @brief 文件上传加密/解密解析类
/// @details 使用 Blowfish 算法加密和解密上传的文件。
///          支持设置加密密钥、加密文件、解密文件。
///          采用单例模式确保全局唯一实例。
/// @note 此类用于保护上传的文件（防止未授权访问）
/// @warning 确保加密密钥安全存储，避免泄露
class CUploadParser : public Singleton<CUploadParser>
{
	friend class Singleton<CUploadParser>;

protected:
	/// @brief 构造函数
	/// @details 初始化上传解析对象，创建 Blowfish 对象
	CUploadParser(void);
	
	/// @brief 析构函数
	/// @details 清理资源，删除 Blowfish 对象
	~CUploadParser(void);

public:
	/// @brief 设置加密密钥
	/// @param[in] enckey 加密密钥字符串
	/// @note 设置用于加密和解密的密钥
	/// @warning 确保密钥安全，避免硬编码
	void SetKey(string enckey);

	/// @brief 获取当前加密密钥
	/// @return 加密密钥字符串
	/// @note 返回当前使用的加密密钥
	string GetKey();

	/// @brief 解密文件
	/// @param[in] src 源文件路径（宽字符）
	/// @param[in] Decryptname 解密后文件路径（宽字符）
	/// @return 0: 成功; 其他: 错误码
	/// @note 使用 Blowfish 算法解密文件
	/// @warning 确保输出文件路径可写
	int Decryptfile(wstring src, wstring Decryptname);

	/// @brief 加密文件
	/// @param[in] src 源文件路径（宽字符）
	/// @param[in] Encryptname 加密后文件路径（宽字符）
	/// @return 0: 成功; 其他: 错误码
	/// @note 使用 Blowfish 算法加密文件
	/// @warning 确保输出文件路径可写
	int Encryptfile(wstring src, wstring Encryptname);

private:
	/// @brief 加密密钥
	string _enckey;
	
	/// @brief Blowfish 算法对象指针
	CBlowfish * _pBF;
	
};
