///////////////////////////////////////////////////////////////////////////////
// 文件名: Blowfish.h
// 模块: scheme/scheme
// 功能描述: Blowfish 加密算法实现
//           提供对称加密、解密和 CRC8 校验功能
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#ifndef ___BLOWFISH_H___
#define ___BLOWFISH_H___

#include <cstdint>

/// @brief 子密钥数量（18 个 P-数组）
#define NUM_SUBKEYS   18

/// @brief S-盒数量（4 个 S-盒）
#define NUM_S_BOXES   4

/// @brief S-盒每个条目数量（256 个条目）
#define NUM_ENTRIES   256

/// @brief 最大字符串长度（字节）
#define MAX_STRING   256

/// @brief 最大密码长度（56 字节 = 448 位）
#define MAX_PASSWD   56   /* 56 bytes = 448 bits */

/// @brief 定义字节序
///        根据平台选择合适的字节序定义
///        LITTLE_ENDIAN: Intel x86 架构（DCBA）
///        BIG_ENDIAN: Motorola 架构（ABCD）
///        VAX: VAX 架构（BADC）
// ============================================================================
// 如果其他文件已定义其中之一，可以注释掉下面的定义
// ============================================================================
#define LITTLE_ENDIAN

/// @brief 大端字节序的字节结构
#ifdef BIG_ENDIAN
struct BFWordByte
{
   unsigned int zero:8;   ///< 最低字节
   unsigned int one:8;    ///< 第二个字节
   unsigned int two:8;    ///< 第三个字节
   unsigned int three:8;  ///< 最高字节
};
#endif /* endif BIG_ENDIAN defined */

/// @brief 小端字节序的字节结构
#ifdef LITTLE_ENDIAN
struct BFWordByte
{
   unsigned int three:8;  ///< 最高字节
   unsigned int two:8;    ///< 第三个字节
   unsigned int one:8;    ///< 第二个字节
   unsigned int zero:8;   ///< 最低字节
};
#endif /* endif LITTLE_ENDIAN defined */

/// @brief VAX 字节序的字节结构
#ifdef VAX
struct BFWordByte
{
   unsigned int one:8;    ///< 第二个字节
   unsigned int zero:8;   ///< 最低字节
   unsigned int three:8;  ///< 最高字节
   unsigned int two:8;    ///< 第三个字节
};
#endif /* endif VAX defined */

/// @brief Blowfish 字类型（32 位）
/// @details 联合体，可以同时按字或按字节访问
union BFWord
{
   unsigned int word;   ///< 32 位字
   BFWordByte byte;     ///< 字节结构
};

/// @brief Blowfish 双字类型（64 位）
/// @details 包含两个 BFWord，用于 64 位数据块操作
struct BFDWord
{
   BFWord word0;  ///< 低 32 位
   BFWord word1;  ///< 高 32 位
};

/// @brief Blowfish 加密算法类
/// @details 实现 Bruce Schneier 的 Blowfish 对称加密算法。
///          支持可变长度密钥（最多 448 位），使用 64 位数据块。
///          适用于需要快速加密和解密的场景。
/// @note Blowfish 算法适用于密钥不频繁变化的场景
/// @warning 现代应用中建议使用更安全的算法（如 AES），Blowfish 主要用于向后兼容
class CBlowfish
{
public:
   /// @brief 构造函数
   /// @details 初始化 Blowfish 对象，设置默认状态
   CBlowfish();
   
   /// @brief 析构函数
   /// @details 释放资源，清零敏感数据（防止内存残留）
   ~CBlowfish();

   /// @brief 重置加密状态
   /// @details 清零所有子密钥和 S-盒，恢复到初始状态
   void Reset();

   /// @brief 设置加密密码
   /// @details 使用指定密码生成子密钥和 S-盒。
   ///          密码长度应至少为 8 字节，最长 56 字节（448 位）。
   /// @param[in] Passwd 密码字符串指针（以 null 结尾）
   /// @note 密码长度不足 8 字节会降低安全性
   /// @warning 密码过短（< 8 字节）会显著降低加密强度
   void SetPassword(char *Passwd);

   /// @brief 加密数据
   /// @details 对指定内存区域进行 Blowfish 加密。
   ///          数据长度必须是 8 的倍数（64 位块大小）。
   /// @param[in,out] Ptr 要加密的数据指针
   /// @param[in] nBytes 数据长度（字节，必须是 8 的倍数）
   /// @note 加密后的数据会直接写回原内存区域
   /// @warning 确保数据长度是 8 的倍数，否则行为未定义
   void Encrypt(void *Ptr, unsigned long nBytes);
   
   /// @brief 解密数据
   /// @details 对指定内存区域进行 Blowfish 解密。
   ///          数据长度必须是 8 的倍数（64 位块大小）。
   /// @param[in,out] Ptr 要解密的数据指针
   /// @param[in] nBytes 数据长度（字节，必须是 8 的倍数）
   /// @note 解密后的数据会直接写回原内存区域
   /// @warning 确保数据长度是 8 的倍数，否则行为未定义
   void Decrypt(void *Ptr, unsigned long nBytes);
   
   /// @brief 计算 CRC8 校验值
   /// @details 计算数据的 8 位循环冗余校验值，用于数据完整性验证。
   /// @param[in] data 数据指针
   /// @param[in] size 数据长度（字节）
   /// @return CRC8 校验值（8 位）
   /// @note 使用标准的 CRC-8 算法
   uint8_t crc8(uint8_t *data, int size);
   
   /// @brief CRC8 计算结果
   /// @details 存储最近一次 crc8() 调用的计算结果
   uint8_t _crc;

private:
   /// @brief 子密钥数组（P-数组）
   /// @details 18 个 32 位子密钥，用于加密/解密的初始变换
   unsigned int PA[NUM_SUBKEYS];
   
   /// @brief S-盒数组
   /// @details 4 个 S-盒，每个包含 256 个 32 位条目
   ///          用于加密/解密的非线性变换
   unsigned int SB[NUM_S_BOXES][NUM_ENTRIES];

   /// @brief 生成子密钥
   /// @details 使用密码初始化子密钥和 S-盒。
   ///          通过密钥扩展和 salt 处理生成最终的子密钥。
   /// @param[in] Passwd 密码字符串指针
   /// @note 此方法是内部实现，外部不应直接调用
   void GenerateSubkeys(char *Passwd);

   /// @brief Blowfish 加密轮函数
   /// @details 执行一次 Blowfish 加密轮运算（Feistel 网络）。
   ///          此方法是内联的，以提升性能。
   /// @param[in,out] x1 第一个 32 位数据字
   /// @param[in,out] x2 第二个 32 位数据字
   /// @note 此方法是内部实现，外部不应直接调用
   inline void BF_En(BFWord *x1, BFWord *x2);
   
   /// @brief Blowfish 解密轮函数
   /// @details 执行一次 Blowfish 解密轮运算（Feistel 网络）。
   ///          此方法是内联的，以提升性能。
   /// @param[in,out] x1 第一个 32 位数据字
   /// @param[in,out] x2 第二个 32 位数据字
   /// @note 此方法是内部实现，外部不应直接调用
   inline void BF_De(BFWord *x1, BFWord *x2);
};

#endif /* ___BLOWFISH_H___ */
