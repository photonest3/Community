///////////////////////////////////////////////////////////////////////////////
// 文件名: HtmlgetAdmin.h
// 模块: scheme/scheme
// 功能描述: 管理界面请求处理类
//           处理管理员界面的 GET/POST 请求，提供相册管理、上传、初始化等功能
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"
#include <atomic>
#include <queue>

/// @brief 管理界面请求处理类
/// @details 处理管理员界面的所有 GET 和 POST 请求，包括：
///          - 管理员登录和注册
///          - 相册管理（添加、删除、恢复）
///          - 图片上传和导入
///          - 分类和标签管理
///          - 系统初始化和配置
///          采用单例模式确保全局唯一实例，
///          使用多线程处理耗时操作（如上传、初始化）。
/// @note 此类包含敏感操作，需要确保用户已通过身份验证
/// @warning 所有管理操作都应记录日志，便于审计
class CHtmlgetAdmin : public CImBase, public Singleton<CHtmlgetAdmin>
{
	friend class Singleton<CHtmlgetAdmin>;
	
protected:
	/// @brief 构造函数
	/// @details 初始化管理界面处理对象，设置默认状态
	CHtmlgetAdmin(void);
	
	/// @brief 析构函数
	/// @details 释放资源，等待后台线程完成
	~CHtmlgetAdmin(void);

public:
	// ========================================================================
	// 请求处理方法
	// ========================================================================
	
	/// @brief 处理管理员界面 GET 请求
	/// @details 生成管理员界面的 HTML 页面，包括登录页、控制台、相册管理等
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[out] response_dto HTTP 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 未登录用户会被重定向到登录页
	long get_admin_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 处理管理员界面 POST 请求
	/// @details 处理管理员界面的表单提交，如保存配置、修改分类等
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[out] response_dto HTTP 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	long post_admin_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 处理 WebSocket PHP 请求（GET）
	/// @details 处理通过 WebSocket 发送的 GET 请求（用于实时通信）
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[out] response_dto HTTP 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	long get_ws_php(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 处理 WebSocket PHP 请求（POST）
	/// @details 处理通过 WebSocket 发送的 POST 请求（用于实时通信）
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[out] response_dto HTTP 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	long post_ws_php(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 删除图像文件
	/// @details 从文件系统和数据库中删除指定的图像文件
	/// @param[in] type 删除类型（"pic"=图片, "cat"=分类等）
	/// @param[in] _lstFiles 要删除的文件路径列表
	/// @param[in] _lstCoi 要删除的 COI 列表
	/// @return 0: 成功; 其他: 错误码
	/// @warning 此操作不可恢复，会同时删除原图和缩略图
	long delete_image_files(string type, list<string> _lstFiles, list<string> _lstCoi);

	// ========================================================================
	// 文件夹和相册管理
	// ========================================================================
	
	/// @brief 获取文件夹大小
	/// @details 计算指定类型的文件夹占用空间大小
	/// @param[in] ty 文件夹类型（0=相册, 1=缓存等）
	/// @param[out] fsize 输出参数，文件夹大小（字节）
	/// @return 0: 成功; 其他: 错误码
	long get_folder_sizes(int ty, uint64_t& fsize);
	
	/// @brief 删除根分类
	/// @details 递归删除根分类及其所有子分类和图片
	/// @param[in] first 是否首次调用（用于递归）
	/// @param[in] lst 要删除的分类列表
	/// @param[in] root_id 根分类 ID
	/// @param[in] root_dir 根分类目录路径
	/// @return 0: 成功; 其他: 错误码
	/// @warning 此操作不可恢复
	long remove_root(string first, list<CATEGORY_DTO> lst, string root_id, string root_dir);
	
	/// @brief 卸载（清理资源）
	/// @details 清理管理界面的临时资源和缓存
	/// @return 0: 成功
	long unins();
	
	/// @brief 解析语言 XML 文件
	/// @details 加载指定语言的翻译字符串，用于界面国际化
	/// @return 0: 成功; 其他: 错误码
	/// @note 支持多语言，根据用户界面语言加载对应翻译
	long ParseLangXML();
	
	/// @brief 通过查看器添加图片
	/// @details 处理从查看器界面发起的图片添加请求
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[out] response_dto HTTP 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	long addbyViewer(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 获取可恢复的删除项
	/// @details 获取最近删除但尚未永久删除的项目列表
	/// @param[out] fol 输出参数，可恢复的项目列表
	/// @return 0: 成功; 其他: 错误码
	/// @note 删除的项目会在回收站保留一段时间
	long get_restore(list<string>& fol);

private:
	// ========================================================================
	// 内部辅助方法
	// ========================================================================
	
	/// @brief 获取表单字段值
	/// @details 从 HTTP 请求体中解析指定字段的值
	/// @param[in] szBody HTTP 请求体字符串
	/// @param[in] name 字段名称
	/// @param[in] id 字段 ID（可选）
	/// @return 字段值字符串
	/// @note 用于解析 POST 请求的表单数据
	string get_field_item(string  szBody, string name, string id);
	
	/// @brief 获取分类全路径名称
	/// @details 根据分类的全路径 ID 获取其完整名称（包含父分类）
	/// @param[in] full_id 分类全路径 ID
	/// @param[in] ptrlstCategory 分类列表指针
	/// @param[out] fullname 输出参数，完整分类名称
	/// @return 0: 成功; 其他: 错误码
	long get_fullname(string full_id, list<CATEGORY_DTO>* ptrlstCategory, string& fullname);
	
	/// @brief 处理登录请求
	/// @details 验证管理员用户名和密码
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @return 0: 登录成功; 其他: 错误码
	/// @note 登录成功会设置 session
	long proc_login(const REQUEST_DTO request_dto);
	
	/// @brief 处理注册请求
	/// @details 创建新的管理员账户
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @return 0: 注册成功; 其他: 错误码
	/// @warning 注册功能应根据配置决定是否启用
	long proc_registr(const REQUEST_DTO request_dto);
	
	/// @brief 获取标签 ID 列表
	/// @details 将标签名称列表转换为标签 ID 列表
	/// @param[in] tags 标签名称列表（逗号分隔）
	/// @param[out] val 输出参数，标签 ID 列表（逗号分隔）
	/// @return 0: 成功; 其他: 错误码
	long get_tagids(string tags, string& val);
	
	/// @brief 获取查看器三级权限
	/// @details 检查查看器的访问权限（公开/密码/私有）
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[out] response_dto HTTP 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	long get_viewer_u3_degree(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);

	// ========================================================================
	// 后台线程处理函数
	// ========================================================================
	
	/// @brief 后台线程： Brisque 图像质量评估
	/// @details 在线程中执行 Brisque 图像质量评估算法
	/// @param[in] pParam 线程参数（包含索引等信息）
	/// @return 线程退出码
	/// @note Brisque 是盲图像质量评估算法
	static UINT __stdcall sb_brisque(LPVOID pParam);
	
	/// @brief Brisque 图像处理
	/// @details 对指定索引的图片进行 Brisque 质量评估
	/// @param[in] idx 图片索引
	/// @return 0: 成功; 其他: 错误码
	long brisque(int idx);

public:
	// ========================================================================
	// 相册管理操作方法
	// ========================================================================
	
	/// @brief 添加相册文件夹
	/// @details 创建新的相册文件夹并初始化数据库
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[out] response_dto HTTP 响应数据传输对象
	void galleries_addfolder(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 刷新分类
	/// @details 重新加载分类数据到内存缓存
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[out] response_dto HTTP 响应数据传输对象
	void categories_refresh(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 初始化相册
	/// @details 首次创建相册时的初始化操作
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[out] response_dto HTTP 响应数据传输对象
	void galleries_init(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 获取待处理项
	/// @details 获取需要处理的图片列表（如未生成缩略图的）
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[out] response_dto HTTP 响应数据传输对象
	void galleries_pending(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 确认待处理项完成
	/// @details 标记待处理项已完成
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[out] response_dto HTTP 响应数据传输对象
	void galleries_pendingok(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 清空相册
	/// @details 删除相册中的所有图片和分类（保留相册结构）
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[out] response_dto HTTP 响应数据传输对象
	/// @warning 此操作不可恢复
	void galleries_clear(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 获取相册分类
	/// @details 获取相册的分类树结构
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[out] response_dto HTTP 响应数据传输对象
	void galleries_categories(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 上传完成回调
	/// @details 图片上传完成后的处理（生成缩略图、更新数据库等）
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[out] response_dto HTTP 响应数据传输对象
	void upload_complete(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 删除相册
	/// @details 删除指定的相册及其所有数据
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[out] response_dto HTTP 响应数据传输对象
	/// @warning 此操作不可恢复
	void galleries_remove(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 恢复相册
	/// @details 从回收站恢复已删除的相册
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[out] response_dto HTTP 响应数据传输对象
	void galleries_restore(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);

private:
	// ========================================================================
	// 文件夹状态检查
	// ========================================================================
	
	/// @brief 检查文件夹是否变更
	/// @details 检查文件夹内容是否有变化（用于自动刷新）
	/// @return true: 有变更; false: 无变更
	bool folder_ischange();

	// ========================================================================
	// 初始化线程
	// ========================================================================
	
	/// @brief 初始化处理
	/// @details 在线程中执行初始化操作（避免阻塞 UI）
	/// @param[in] idxdto 初始化数据传输对象
	void init_proc(IDX_DTO idxdto);
	
	/// @brief 后台线程：初始化
	/// @param[in] pParam 线程参数
	/// @return 线程退出码
	static UINT __stdcall sb_init(LPVOID pParam);
	
	/// @brief 初始化线程状态数组
	/// @details 记录每个初始化线程的状态（0=未启动, 1=运行中, 2=完成）
	///          最多支持 100 个并发初始化线程
	atomic_int _nb_init0_thread[100] = { 0 };
	
	/// @brief 状态队列数组
	/// @details 每个线程对应的状态队列，存储初始化过程中的状态数据
	queue<FOLDER_STATE_DTO> _queState[100];

	// ========================================================================
	// 上传线程
	// ========================================================================
	
	/// @brief 后台线程：上传
	/// @param[in] pParam 线程参数
	/// @return 线程退出码
	static UINT __stdcall sb_upload(LPVOID pParam);
	
	/// @brief 上传处理
	/// @details 处理图片上传，包括保存到文件系统、生成缩略图、更新数据库
	void upload();
	
	/// @brief 上传线程状态
	/// @details 记录上传线程的运行状态
	atomic_int _nb_gg2_thread;
	
	/// @brief 上传进度：总长度
	/// @details 当前上传任务的总字节数
	atomic_int _progress_len3;
	
	/// @brief 上传进度：当前步骤
	/// @details 当前上传任务的进度步骤（0-100）
	atomic_int _progress_step3;
	
	/// @brief 上传进度：取消标记
	/// @details 如果为 1，上传线程会取消当前上传
	atomic_int _progress_cancel;

	// ========================================================================
	// 上传统计
	// ========================================================================
	
	/// @brief 上传成功数量
	atomic_int _nb_upload_ok = 0;
	
	/// @brief 上传错误数量（错误码 30）
	atomic_int _nb_upload_err30 = 0;
	
	/// @brief 上传错误数量（错误码 40）
	atomic_int _nb_upload_err40 = 0;
	
	/// @brief 上传队列
	/// @details 等待上传的文件队列
	queue<FILE_DTO> _queUpload;

public:
	// ========================================================================
	// 进度和状态（公开成员）
	// ========================================================================
	
	/// @brief 处理进度步骤
	/// @details 当前处理任务的进度步骤
	atomic_int _progress_step;
	
	/// @brief 处理进度总长度
	/// @details 当前处理任务的总长度
	atomic_int _progress_len;
	
	/// @brief CRC8 校验值
	uint8_t _crc;
	
	/// @brief 更新分类数量
	/// @details 重新计算并更新所有分类的图片数量
	/// @param[in] mapCat 分类数量映射表
	void update_catnb(map<string, uint32_t> mapCat);
	
	/// @brief 相册 Brisque 评估
	/// @details 对相册中的图片进行 Brisque 质量评估
	/// @param[in] idx 图片索引
	void galleries_brisque(int idx);
	
	/// @brief 查看器数量统计
	/// @details 统计查看器的访问数量
	void viewer_nb();

private:
	// ========================================================================
	// 数量统计线程
	// ========================================================================
	
	/// @brief 后台线程：数量统计
	/// @param[in] pParam 线程参数
	/// @return 线程退出码
	static UINT __stdcall sb_nb(LPVOID pParam);
	
public:
	/// @brief 数量统计处理
	/// @details 执行数量统计操作（分类图片数、标签使用数等）
	void nb_proc();
	
	/// @brief 分类数量统计完成标记
	/// @details 标记分类数量统计是否完成
	atomic_int _lstCatNbOk;
	
private:
	/// @brief 数量统计线程函数
	/// @param[in] pParam 线程参数
	/// @return 线程退出码
	static UINT __stdcall proc_nb_(LPVOID pParam);
	
	/// @brief 数量统计完成处理
	/// @details 数量统计完成后的清理和通知操作
	void proc_nb_ok();
	
	/// @brief 数据库待处理文件列表
	/// @details 数据库中记录但尚未处理的文件
	list<FILE_DTO> _lstDbPending;
	
	/// @brief 文件系统待处理文件列表
	/// @details 文件系统中存在但尚未记录到数据库的文件
	list<FILE_DTO> _lstFsPending;
	
	/// @brief 分类数量队列
	/// @details 等待处理的分类数量数据
	queue<CAT_NB_DTO> _queCatNb2;

	// ========================================================================
	// 状态管理
	// ========================================================================
	
	/// @brief 状态列表
	/// @details 存储文件夹和相册的状态信息
	list<FOLDER_STATE_DTO > _lstState;
	
	/// @brief 线程状态
	/// @details 记录后台线程的运行状态
	atomic_int _nb_thread;
	
	/// @brief 删除 ID 列表
	/// @details 等待删除的项目 ID 列表（逗号分隔）
	string _del_ids = "";
	
	/// @brief MD5 映射表
	/// @details 文件路径到 MD5 值的映射，用于去重
	MAP_MD5 _lst_md5;
	
	/// @brief 分类数量列表
	/// @details 分类 ID 到图片数量的映射
	list<CAT_NB_DTO> _lstCatNb;
	
	/// @brief 初始化数据传输对象列表
	/// @details 存储初始化过程中的数据传输对象
	list<FOLDER_STATE_DTO> _lstInitDto;

	// ========================================================================
	// 线程同步
	// ========================================================================
	
	/// @brief 数量统计临界区
	/// @details 保护数量统计数据的线程安全访问
	CRITICAL_SECTION _nbcs;
	
	/// @brief 通用临界区
	/// @details 保护共享数据的线程安全访问
	CRITICAL_SECTION _cs0;

	// ========================================================================
	// 配置和状态
	// ========================================================================
	
	/// @brief 分类映射表
	/// @details 分类 ID 到分类名称的映射
	map<string, string> _cats_map;
	
	/// @brief 文件夹大小映射表
	/// @details 文件夹路径到大小的映射（wstring -> uint32_t）
	map<wstring, uint32_t> _mapSizes;
	
	/// @brief 翻译字符串
	/// @details 界面翻译字符串（用于国际化）
	string _trs = "";
	
	/// @brief CPU 核心数
	/// @details 系统 CPU 核心数，用于多线程任务分配
	int _cpu = 5;
	
	/// @brief Brisque 评估进行中标记
	/// @details 如果为 1，表示正在执行 Brisque 评估
	atomic_int _brisquing = 0;

	// ========================================================================
	// 解密线程
	// ========================================================================
	
	/// @brief 后台线程：解密
	/// @param[in] pParam 线程参数
	/// @return 线程退出码
	static UINT __stdcall sb_decrypt(LPVOID pParam);
	
	/// @brief 解密处理
	/// @details 对加密的文件进行解密
	void decrypt();
	
	/// @brief 解密队列
	/// @details 等待解密的文件路径队列
	queue<wstring> _queDecrypt;
	
	/// @brief 缩略图队列
	/// @details 等待生成缩略图的文件路径队列
	queue<wstring> _queThumb;
	
	/// @brief 目录列表
	/// @details 需要处理的目录列表
	list<wstring> _lstDir;
	
	/// @brief 处理完成
	/// @details 处理完成后的清理和通知操作
	/// @param[in] root_id 根分类 ID
	void processok(string root_id);

public:
	/// @brief Brisque 评估结束标记
	/// @details 如果为 1，表示 Brisque 评估已结束
	atomic_int _brisque_end = 1;

private:
	// ========================================================================
	// 预测线程
	// ========================================================================
	
	/// @brief 后台线程：预测
	/// @param[in] pParam 线程参数
	/// @return 线程退出码
	static UINT __stdcall sb_predict(LPVOID pParam);
	
	/// @brief 预测处理
	/// @details 执行图像识别预测（如人脸识别、场景分类等）
	void predict_proc();
	
	/// @brief 预测进行中标记
	/// @details 如果为 1，表示正在执行预测
	atomic_int _predict = 0;
	
	/// @brief 训练数量
	/// @details 已训练的图片数量
	atomic_int _nbTrain = 0;
	
	/// @brief 人脸 ID 列表
	/// @details 等待处理的人脸 ID 列表（逗号分隔）
	string _faceIds = "";
	
	/// @brief 图像 ID 列表
	/// @details 等待预测的图像 ID 列表
	list<string> _lstImageId;

	/// @brief 获取分组人脸
	/// @details 从指定路径获取分组后的人脸图像
	/// @param[in] strPath 文件路径
	/// @param[out] mapFiles 输出参数，分组人脸映射表
	void get_group_face(wstring strPath, map<string, list<string>>& mapFiles);
};
