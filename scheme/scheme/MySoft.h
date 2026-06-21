///////////////////////////////////////////////////////////////////////////////
// 文件名: MySoft.h
// 模块: scheme/scheme
// 功能描述: 软件授权管理类
//           处理软件授权验证、注册码生成和验证
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-04-10  PhotoNest Team  添加授权验证功能
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include "Singleton.h"

using namespace std;

/// @brief 授权文件头结构
/// @details 定义授权文件（.lic）的二进制格式。
///          用于验证软件的合法授权。
/// @note 此结构直接映射到授权文件的前 N 个字节
typedef struct _lic_header
{
	/// @brief 版本号
	unsigned char version;
	
	/// @brief 模式（MODE_GALLERY: 10, MODE_CUTOUT: 30）
	unsigned char mode;
	
	/// @brief 授权数量（用户数或设备数）
	unsigned short num;
	
	/// @brief 校验和（用于验证文件完整性）
	unsigned short chksum;
	
	/// @brief 溢出标志（可能用于扩展字段）
	unsigned char ov;
	
	/// @brief 授权年份（用于时间限制）
	unsigned char year;
	
	/// @brief 授权日期（时间戳，秒）
	unsigned long long date;
	
} lic_header;

/// @brief 模式定义：画廊模式
/// @note 用于 PhotoNest Gallery 版本
#define MODE_GALLERY 10

/// @brief 模式定义：裁剪模式
/// @note 用于 PhotoNest Cutout 版本
//#define MODE_ENCRYPTED 20  // 已注释：加密模式
#define MODE_CUTOUT 30

/// @brief 错误码定义：签名错误
#define ERROR_SIGN 4

/// @brief 错误码定义：授权错误
#define ERROR_LIC 5

/// @brief 错误码定义：过期错误
#define ERROR_OVERDUE 6

/// @brief 软件授权管理类
/// @details 处理软件授权验证、注册码生成和验证。
///          支持在线激活、离线激活、授权文件验证等。
///          采用单例模式确保全局唯一实例。
/// @note 授权文件使用自定义二进制格式（.lic）
/// @warning 授权验证失败会限制软件功能
class CMySoft : public Singleton<CMySoft>
{
	friend class Singleton<CMySoft>;

protected:
	/// @brief 构造函数
	/// @details 初始化授权管理对象
	CMySoft(void);
	
	/// @brief 析构函数
	/// @details 清理资源
	~CMySoft(void);

public:
	/// @brief 验证授权
	/// @param[in] s2 授权字符串或文件路径
	/// @param[in] mode 验证模式（MODE_GALLERY, MODE_CUTOUT 等）
	/// @param[out] lic 授权头结构（输出）
	/// @return 0: 成功; 其他: 错误码（ERROR_SIGN, ERROR_LIC, ERROR_OVERDUE）
	/// @note 验证授权字符串或授权文件的有效性
	/// @warning 授权验证失败会限制软件功能
	int Verify(string s2, int mode, lic_header &lic);
	
	/// @brief 添加注册表项
	/// @param[in] szSubKey 注册表子键路径
	/// @param[in] key 键值名
	/// @param[in] val 键值数据
	/// @return 0: 成功; 其他: 错误码
	/// @note 将授权信息写入注册表（Windows）或配置文件（Linux）
	long add_lic(string szSubKey, string key, string val);
	
	/// @brief 读取注册表项
	/// @param[in] szSubKey 注册表子键路径
	/// @param[out] v3 键值数据（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 从注册表读取授权信息
	void get_lic(string szSubKey, string & v3);
	
	/// @brief 注册软件
	/// @param[in] s2 注册码或授权文件路径
	/// @param[out] lic 授权头结构（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 注册软件并写入授权信息
	/// @warning 注册失败会限制软件功能
	int Reg(string s2, lic_header &lic);
	
	/// @brief 生成授权字符串
	/// @param[in] lic 授权头结构
	/// @param[out] sLic 授权字符串（输出）
	/// @return TRUE: 成功; FALSE: 失败
	/// @note 根据授权头结构生成授权字符串或授权文件
	BOOL Grn(lic_header lic, string& sLic);

private:
	/// @brief 清除敏感数据
	/// @param[in,out] b 数据缓冲区指针（输出时被修改）
	/// @param[in] cb 缓冲区大小（字节）
	/// @note 安全清除内存中的敏感数据（如密码、密钥）
	/// @warning 确保缓冲区有效，否则可能导致内存访问错误
	void ScrubBlob(void *b, DWORD cb);
	
};
