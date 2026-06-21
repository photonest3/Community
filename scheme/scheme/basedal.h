///////////////////////////////////////////////////////////////////////////////
// 文件名: basedal.h
// 模块: scheme/scheme
// 功能描述: 数据库访问层基类，封装 SQLite 数据库操作
//           采用单例模式确保全局唯一数据库实例
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-03-15  PhotoNest Team  添加数据库备份功能
//   2024-05-20  PhotoNest Team  支持数据库升级
///////////////////////////////////////////////////////////////////////////////

#pragma once

#define SQLITE_HAS_CODEC

#include <string>
#include <list>
#include "stmdef.h"

#include "sqlite3.h"
#include "Singleton.h"

using namespace std;

/// @brief 数据库访问层基类
/// @details 封装 SQLite 数据库的基本操作，包括数据库打开、关闭、
///          备份、升级等功能。采用单例模式设计，确保整个应用程序
///          中只有一个数据库实例，避免数据库连接冲突。
///          支持数据库加密（需 SQLITE_HAS_CODEC 宏定义）。
/// @note 此类线程安全，可在多线程环境中使用
/// @warning 数据库操作可能涉及磁盘 I/O，建议在后台线程调用耗时操作
class Cbasedal : public Singleton<Cbasedal>
{
	friend class Singleton<Cbasedal>;
	
protected:
	/// @brief 构造函数
	/// @details 初始化数据库连接对象和应用程序数据目录
	Cbasedal(void);
	
	/// @brief 析构函数
	/// @details 关闭数据库连接并释放相关资源
	~Cbasedal(void);

	// TODO: add your methods here.
public:
	/// @brief 获取 SQLite 数据库句柄
	/// @return sqlite3 数据库句柄指针
	/// @note 如果数据库未打开，返回 nullptr
	sqlite3 * getDB();
	
	/// @brief 刷新数据库连接
	/// @details 关闭当前数据库连接，重新打开指定相册的数据库文件。
	///          用于切换相册时重新加载数据库。
	/// @param[in] alb 相册名称（对应数据库文件名）
	/// @param[in] logdir 日志目录路径（wstring 类型，支持 Unicode）
	/// @return SQLITE_OK: 成功; 其他: 错误码
	/// @see backupDb()
	int refresh_db(string alb, wstring logdir);
	
	/// @brief 备份数据库
	/// @details 将当前内存中的数据库保存到磁盘文件，
	///          支持热备份，不中断正在进行的数据库操作。
	/// @param[in] alb 相册名称（目标备份文件名）
	/// @return 0: 成功; 其他: 错误码
	/// @note 备份文件保存在应用程序数据目录下
	/// @see loadOrSaveDb()
	long backupDb(string alb);
	
	/// @brief 复制数据库到应用程序数据目录
	/// @details 将指定相册的数据库文件复制到系统的
	///          应用程序数据目录（AppData），用于数据迁移或备份。
	/// @param[in] alb 相册名称
	/// @note 此操作会覆盖目标位置的同名文件
	void copy2appData(string alb);
	
	/// @brief 升级数据库结构
	/// @details 检查当前数据库版本，执行必要的结构升级操作。
	///          包括添加新的表、字段、索引等。
	/// @note 升级前会自动备份原数据库
	/// @warning 此操作会修改数据库结构，请确保已备份
	void upgrade();
	
	/// @brief 应用程序数据目录路径
	/// @details 存储应用程序配置、缓存、数据库等数据的目录。
	///          通常位于系统的 AppData 或 Library 目录。
	wstring _appDataDir;

private:
	/// @brief 加载或保存数据库
	/// @details 在内存数据库和磁盘文件之间进行数据同步。
	///          支持将磁盘数据库加载到内存（提升性能），
	///          或将内存数据库保存到磁盘（持久化）。
	/// @param[in] pInMemory 内存数据库句柄
	/// @param[in] zFilename 磁盘数据库文件路径
	/// @param[in] isSave 操作类型：0=加载，1=保存
	/// @return 0: 成功; 其他: 错误码
	/// @note 此方法是内部实现，外部不应直接调用
	long loadOrSaveDb(sqlite3 *pInMemory, const char *zFilename, int isSave);
	
	/// @brief 删除文件（Unicode 支持）
	/// @details 删除指定路径的文件，支持长文件名和 Unicode 字符。
	///          封装 Windows API 以处理 wstring 路径。
	/// @param[in] dst 目标文件路径（wstring 类型）
	/// @return TRUE: 删除成功; FALSE: 删除失败
	/// @note 此方法是内部工具函数
	BOOL my_DeleteFile(wstring dst);

private:
	/// @brief SQLite 数据库句柄
	/// @details 指向当前打开的数据库连接，
	///          使用 sqlite3_open 打开，sqlite3_close 关闭
	sqlite3 * _pDB;
};
