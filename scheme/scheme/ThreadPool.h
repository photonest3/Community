///////////////////////////////////////////////////////////////////////////////
// 文件名: ThreadPool.h
// 模块: scheme/scheme
// 功能描述: 线程池实现
//           提供任务队列和线程管理功能
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-03-15  PhotoNest Team  添加线程池功能
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

/// @brief 线程池类
/// @details 实现任务队列和线程管理，支持提交异步任务并获取结果。
///          使用 C++11 标准库（thread, future, packaged_task 等）。
/// @note 线程安全，支持动态添加任务
/// @warning 析构函数会等待所有任务完成，确保资源正确释放
class ThreadPool {
public:
	/// @brief 构造函数
	/// @param[in] threads 线程数量
	/// @details 创建指定数量的线程并启动工作循环
	/// @note 线程会在后台等待任务
	ThreadPool(size_t);

	/// @brief 析构函数
	/// @details 设置停止标志，通知所有线程退出，并等待所有线程完成
	/// @note 确保资源正确释放
	~ThreadPool();

	/// @brief 提交任务到线程池
	/// @tparam F 可调用对象类型
	/// @tparam Args 参数类型包
	/// @param[in] f 可调用对象（函数、Lambda、函数对象等）
	/// @param[in] args 参数包
	/// @return future 对象，用于获取任务执行结果
	/// @note 支持任意可调用对象和参数
	/// @warning 线程池停止后调用会抛出异常
	template<class F, class... Args>
	auto enqueue(F&& f, Args&&... args) 
		-> std::future<typename std::result_of<F(Args...)>::type>;

private:
	// need to keep track of threads so we can join them
	/// @brief 工作线程列表
	std::vector< std::thread > workers;
	
	// the task queue
	/// @brief 任务队列
	std::queue< std::function<void()> > tasks;
	
	// synchronization
	/// @brief 队列互斥锁
	std::mutex queue_mutex;
	
	/// @brief 条件变量，用于线程等待任务
	std::condition_variable condition;
	
	/// @brief 停止标志
	bool stop;
}; 

// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads)
	:   stop(false)
{
	for(size_t i = 0;i<threads;++i)
		workers.emplace_back(
			[this]
				{
					for(;;)
						{
							std::function<void()> task;

							{
								std::unique_lock<std::mutex> lock(this->queue_mutex);
								this->condition.wait(lock,
									[this]{ return this->stop || !this->tasks.empty(); });
								if(this->stop && this->tasks.empty())
									return;
								task = std::move(this->tasks.front());
								this->tasks.pop();
							}

							task();
						}
				}
		);
}

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
	-> std::future<typename std::result_of<F(Args...)>::type>
{
	using return_type = typename std::result_of<F(Args...)>::type;

	auto task = std::make_shared< std::packaged_task<return_type()> >(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);
		
	std::future<return_type> res = task->get_future();
	{
		std::unique_lock<std::mutex> lock(queue_mutex);

			// don't allow enqueueing after stopping the pool
			if(stop)
				throw std::runtime_error("enqueue on stopped ThreadPool");

			tasks.emplace([task](){ (*task)(); });
	}
	condition.notify_one();
	return res;
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		stop = true;
	}
	condition.notify_all();
	for(std::thread &worker: workers)
		worker.join();
}

#endif
