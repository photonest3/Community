///////////////////////////////////////////////////////////////////////////////
// 文件名: Singleton.h
// 模块: scheme/scheme
// 功能描述: 单例模式模板类
//           提供线程不安全的单例模式实现
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

/// @brief 单例模式模板类
/// @details 提供单例模式的标准实现，确保全局只有一个实例。
///          使用静态指针存储实例，在程序退出时自动释放（通过 atexit）。
///          此实现**不是线程安全**的，不适用于多线程环境。
/// @note 使用方法：class MyClass : public Singleton<MyClass> {...}
/// @warning 此实现不是线程安全的，多线程环境下需要加锁
template<class T>
class Singleton {

public:
	/// @brief 获取单例实例
	/// @return 单例实例的引用
	/// @note 第一次调用时创建实例，后续调用返回同一实例
	/// @warning 此函数不是线程安全的
	static T& Instance()
	{
		// 如果实例不存在，则创建
		if (0 == _instance)
		{
			_instance = new T();
			atexit(Destroy); // 注册退出时销毁函数
		}
		return *_instance;
	}

protected:
	/// @brief 构造函数（保护）
	/// @details 防止外部直接实例化
	Singleton() {}

	/// @brief 析构函数（保护）
	/// @details 防止外部直接删除实例
	~Singleton() {}

private:
	/// @brief 销毁单例实例
	/// @details 在程序退出时由 atexit 调用，释放实例内存
	static void Destroy()
	{
		if (_instance != 0)
		{
			delete _instance;
			_instance = 0;
		}
	}

private:
	/// @brief 单例实例指针（易失性）
	/// @note 使用 volatile 防止编译器优化
	static T * volatile _instance;
};

/// @brief 单例实例指针初始化
/// @details 将实例指针初始化为 0（空指针）
template<class T>
T * volatile Singleton<T>::_instance = 0;
