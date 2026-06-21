///////////////////////////////////////////////////////////////////////////////
// 文件名: MyMailer.h
// 模块: scheme/scheme
// 功能描述: 邮件发送类
//           封装 SMTP 协议，提供邮件发送功能
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"

/// @brief 邮件发送类
/// @details 封装 SMTP 协议，提供邮件发送功能。
///          支持 SMTP 认证、SSL/TLS 加密连接。
/// @note 使用第三方 SMTP 库（如 libsmtp）
/// @warning 确保邮箱密码已加密存储，避免明文密码
class CMyMailer 
{
public:
	/// @brief 构造函数
	/// @details 初始化邮件发送对象
	CMyMailer(void);
	
	/// @brief 析构函数
	/// @details 清理资源
	~CMyMailer(void);

public:
	/// @brief 发送邮件（简化版）
	/// @param[in] recvName 收件人姓名
	/// @param[in] recvMail 收件人邮箱地址
	/// @param[in] content 邮件内容
	/// @param[out] Result 发送结果（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 使用默认发件人账号发送邮件
	/// @warning 确保收件人邮箱地址有效
	long SendMail(string recvName, string recvMail, string content, string &Result);

private:
	/// @brief 发送邮件（完整版）
	/// @param[in] username 发件人邮箱账号
	/// @param[in] password 发件人邮箱密码（明文，将被加密）
	/// @param[in] recvName 收件人姓名
	/// @param[in] recvMail 收件人邮箱地址
	/// @param[in] content 邮件内容
	/// @param[out] Result 发送结果（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 使用指定账号发送邮件
	/// @warning 密码会以明文形式传递，请确保调用环境安全
	long SendMail(string username, string password, string recvName, string recvMail, string content, string &Result);
	
	/// @brief 解密密码
	/// @param[in] opass 加密后的密码字符串
	/// @return 解密后的明文密码
	/// @note 使用内置算法解密密码
	/// @warning 此算法安全性较低，建议升级为更安全的加密方式
	string decrypt_pwd(string opass);

private:
	/// @brief SMTP 对象
	SMTP _smtp;
	
};
