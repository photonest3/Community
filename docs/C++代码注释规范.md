# C++ 代码注释规范

> **项目**: PhotoNest 高容量图像库软件  
> **版本**: v1.0  
> **日期**: 2024-06-21  
> **适用**: C++11/14/17 标准，CEF 框架项目

---

## 📋 目录

1. [规范原则](#规范原则)
2. [文件头注释](#文件头注释)
3. [命名空间注释](#命名空间注释)
4. [类/结构体注释](#类结构体注释)
5. [函数/方法注释](#函数方法注释)
6. [成员变量注释](#成员变量注释)
7. [代码逻辑注释](#代码逻辑注释)
8. [宏定义注释](#宏定义注释)
9. [注释格式示例](#注释格式示例)
10. [禁忌与最佳实践](#禁忌与最佳实践)

---

## 规范原则

### ✅ 核心原则

1. **非破坏性原则**: 注释只添加，不修改任何可执行代码
2. **意图优先**: 注释说明"为什么"而非"是什么"
3. **适度原则**: 代码清晰时少注释，复杂逻辑多注释
4. **中文优先**: 团队内部项目使用中文注释
5. **同步更新**: 代码修改时同步更新注释

### 📝 注释类型选择

| 注释类型 | 使用场景 | 格式 |
|---------|---------|------|
| 文档注释 | 类、函数、结构体等对外接口 | `///` 或 `/** */` (Doxygen) |
| 实现注释 | 复杂逻辑、算法说明 | `//` 单行或 `/* */` 多行 |
| TODO 注释 | 待完成功能 | `// TODO(username): 描述` |
| FIXME 注释 | 需要修复的问题 | `// FIXME: 描述` |
| NOTE 注释 | 重要注意事项 | `// NOTE: 描述` |

---

## 文件头注释

### 规范格式

```cpp
///////////////////////////////////////////////////////////////////////////////
// 文件名: [文件名]
// 模块: [所属模块]
// 功能描述: [简要描述文件功能]
// 作者: [可选]
// 创建日期: [YYYY-MM-DD]
// 修改记录:
//   [YYYY-MM-DD] [修改者] [修改内容]
///////////////////////////////////////////////////////////////////////////////
```

### 示例

```cpp
///////////////////////////////////////////////////////////////////////////////
// 文件名: client_handler.h
// 模块: cefclient/browser
// 功能描述: CEF 浏览器客户端处理器，处理浏览器生命周期事件、
//           加载状态、键盘鼠标事件等
// 作者: PhotoNest Team
// 创建日期: 2024-01-15
// 修改记录:
//   2024-03-20  张三  添加 OSR 渲染支持
//   2024-05-10  李四  优化事件处理逻辑
///////////////////////////////////////////////////////////////////////////////
```

---

## 命名空间注释

### 规范格式

```cpp
///////////////////////////////////////////////////////////////////////////////
// 命名空间: [namespace_name]
// 功能描述: [命名空间的功能说明]
///////////////////////////////////////////////////////////////////////////////
namespace [namespace_name] {
    // ...
} // namespace [namespace_name]
```

### 示例

```cpp
///////////////////////////////////////////////////////////////////////////////
// 命名空间: photolib
// 功能描述: 照片库核心功能命名空间，包含图像加载、缓存、元数据管理
///////////////////////////////////////////////////////////////////////////////
namespace photolib {
    // ...
} // namespace photolib
```

---

## 类/结构体注释

### 规范格式

```cpp
/// @brief [类/结构体简要描述]
/// @details [详细描述，包括设计思路、使用场景、注意事项等]
/// @note [注意事项]
/// @warning [警告信息]
class [ClassName] {
    // ...
};
```

### 示例

```cpp
/// @brief CEF 浏览器客户端处理器
/// @details 继承自 CefClient，处理浏览器各种事件和请求。
///          该类是 CEF 浏览器与应用程序交互的核心桥梁，
///          负责处理浏览器生命周期、加载状态、键盘鼠标事件等。
///          采用单例模式设计，确保全局唯一实例。
/// @note 此类必须在浏览器进程的主线程中使用
/// @warning 不要在渲染进程中直接使用此类
class ClientHandler : public CefClient,
                     public CefLifeSpanHandler,
                     public CefLoadHandler {
    // ...
};
```

---

## 函数/方法注释

### 规范格式

```cpp
/// @brief [函数简要描述]
/// @details [详细描述，包括算法思路、特殊处理等]
/// @param[in] [参数名] [参数描述]
/// @param[out] [参数名] [参数描述，说明输出内容]
/// @param[in,out] [参数名] [参数描述，说明输入输出]
/// @return [返回值描述]
/// @retval [具体值] [该返回值的含义]
/// @exception [异常类型] [异常描述]
/// @note [注意事项]
/// @warning [警告信息]
/// @see [相关函数或类]
virtual [返回类型] [函数名]([参数列表]) [override];
```

### 示例 1: 虚函数/接口

```cpp
/// @brief 浏览器创建完成后调用
/// @details 当浏览器实例创建完成并准备好接收事件时，此方法被调用。
///          可在此方法中进行浏览器初始化操作，如注入 JavaScript、
///          设置 Cookie、注册自定义协议等。
/// @param[in] browser CEF 浏览器对象指针，包含浏览器窗口和渲染进程信息
/// @note 此方法在浏览器进程 UI 线程中调用
/// @warning 不要在此方法中执行耗时操作，会阻塞浏览器创建
/// @see OnBeforeClose()
virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
```

### 示例 2: 普通函数

```cpp
/// @brief 加载指定 URL 到浏览器
/// @details 停止当前加载并导航到指定 URL。支持自定义协议，
///          如 photo://、local:// 等。URL 会进行编码处理。
/// @param[in] url 目标 URL 字符串，支持 HTTP/HTTPS/自定义协议
/// @param[in] bUsePost 是否使用 POST 请求（默认 false）
/// @param[in] extraHeaders 额外 HTTP 头（可选）
/// @return true: 加载成功启动; false: URL 无效或浏览器未就绪
/// @note 如果当前有页面正在加载，会先调用 StopLoading()
/// @warning URL 长度不能超过 2048 字符
bool LoadURL(const CefString& url, 
             bool bUsePost = false,
             const CefRequest::HeaderMap& extraHeaders = {});
```

### 示例 3: 返回复杂类型的函数

```cpp
/// @brief 获取图像元数据
/// @details 从图像文件中提取 EXIF、IPTC 等元数据信息。
///          支持 JPEG、PNG、TIFF、RAW 等格式。
/// @param[in] imagePath 图像文件路径（支持相对和绝对路径）
/// @return 元数据键值对字典，如果提取失败返回空字典
/// @retval 非空 map 成功提取的元数据
/// @retval 空 map 文件不存在、格式不支持或读取失败
/// @exception std::invalid_argument 路径为空或包含非法字符
/// @note 此操作可能涉及磁盘 I/O，建议在后台线程调用
/// @see ImageCache::GetImageInfo()
MetadataMap GetImageMetadata(const std::string& imagePath);
```

---

## 成员变量注释

### 规范格式

```cpp
/// [成员变量描述]
/// @note [可选：注意事项]
[类型] [变量名];
```

### 示例

```cpp
/// 浏览器对象弱引用，用于发送消息到浏览器
/// @note 使用 WeakPtr 避免循环引用导致内存泄漏
CefRefPtr<CefBrowser> browser_;
```

```cpp
/// 是否正在加载页面
/// @note 用于 UI 状态管理，控制加载动画显示
bool is_loading_;
```

```cpp
/// 图像缓存字典，key 为图像路径，value 为缓存对象
/// @note 使用 LRU 策略管理缓存，最大容量 500MB
std::unordered_map<std::string, ImageCacheItem> image_cache_;
```

---

## 代码逻辑注释

### 1. 复杂算法注释

```cpp
// ============================================================================
// 图像缩放算法：双三次插值（Bicubic Interpolation）
// ============================================================================
// 使用 4x4 邻域像素进行插值，比双线性插值更平滑
// 公式：pixel = sum(i=-1 to 2) sum(j=-1 to 2) w(i) * w(j) * P(x+i, y+j)
// 其中 w(x) 是权重函数，P 是像素值
// ============================================================================

for (int i = -1; i <= 2; i++) {
    for (int j = -1; j <= 2; j++) {
        float weight_x = BicubicWeight(i - dx);
        float weight_y = BicubicWeight(j - dy);
        result += weight_x * weight_y * GetPixel(x + i, y + j);
    }
}
```

### 2. 业务流程注释

```cpp
// ============================================================================
// 处理流程：
// 1. 验证输入参数
// 2. 检查缓存中是否已存在
// 3. 如果缓存未命中，从磁盘加载
// 4. 解码图像数据
// 5. 生成缩略图
// 6. 更新缓存
// 7. 返回结果
// ============================================================================

// 步骤 1: 验证输入参数
if (imagePath.empty()) {
    return nullptr;
}

// 步骤 2: 检查缓存
auto cacheIter = image_cache_.find(imagePath);
if (cacheIter != image_cache_.end()) {
    return cacheIter->second->GetImage();
}

// 步骤 3: 从磁盘加载（缓存未命中）
// ...
```

### 3. 关键逻辑注释

```cpp
// NOTE: 此处必须使用双重检查锁（Double-Checked Locking）
//       以确保在多线程环境下单例的唯一性和性能
if (instance_ == nullptr) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance_ == nullptr) {
        instance_ = new Singleton();
    }
}
```

```cpp
// FIXME: CEF 在某些情况下会在渲染进程崩溃后继续发送事件
//        需要在此处添加额外的状态检查，避免访问已释放的资源
//        相关问题：https://bitbucket.org/chromiumembedded/cef/issues/1234
if (browser && browser->GetHost()) {
    browser->GetHost()->CloseBrowser(false);
}
```

```cpp
// TODO(zhangsan): 优化图像解码性能
//      当前实现使用同步解码，大图像会阻塞 UI 线程
//      计划改为异步解码 + 进度回调
//      预计完成时间：2024-07-30
DecodeImage(imagePath);
```

### 4. 条件分支注释

```cpp
if (IsOsrEnabled()) {
    // OSR (Off-Screen Rendering) 模式：
    // 使用自定义渲染器，不直接显示浏览器窗口
    // 适用于需要嵌入浏览器内容到自定义 UI 的场景
    CreateOsrBrowserWindow();
} else {
    // 标准模式：
    // 使用系统原生窗口，CEF 直接管理窗口生命周期
    CreateStandardBrowserWindow();
}
```

---

## 宏定义注释

### 规范格式

```cpp
/// @brief [宏功能简要描述]
/// @details [详细描述，包括参数说明、使用场景、注意事项]
/// @param [参数名] [参数描述]（如果宏有参数）
[宏定义]
```

### 示例

```cpp
/// @brief 安全释放 COM 对象宏
/// @details 释放 COM 接口指针并将指针置为 nullptr，
///          避免悬空指针和重复释放问题
/// @param ptr COM 接口指针
#define SAFE_RELEASE(ptr) \
    do { \
        if ((ptr) != nullptr) { \
            (ptr)->Release(); \
            (ptr) = nullptr; \
        } \
    } while(0)
```

```cpp
/// @brief 禁用类的拷贝构造函数和赋值运算符
/// @details 将拷贝构造函数和赋值运算符声明为 private 且不实现，
///          防止类被拷贝，适用于单例、资源管理等场景
/// @param className 类名
#define DISALLOW_COPY_AND_ASSIGN(className) \
    private: \
        className(const className&); \
        className& operator=(const className&)
```

---

## 注释格式示例

### 完整类示例

```cpp
///////////////////////////////////////////////////////////////////////////////
// 文件名: image_cache.h
// 模块: photolib/cache
// 功能描述: 图像缓存管理，支持 LRU 淘汰策略和异步加载
// 作者: PhotoNest Team
// 创建日期: 2024-02-10
///////////////////////////////////////////////////////////////////////////////

#ifndef PHOTONEST_IMAGE_CACHE_H_
#define PHOTONEST_IMAGE_CACHE_H_

#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>

namespace photolib {
namespace cache {

/// @brief 图像缓存项，包含图像数据和元数据
/// @details 封装单个图像的缓存数据，包括原始图像、缩略图、
///          加载时间和访问计数等信息
struct ImageCacheItem {
    /// 图像文件路径
    std::string file_path;
    
    /// 图像数据（原始尺寸）
    /// @note 使用 shared_ptr 共享数据，减少内存拷贝
    std::shared_ptr<std::vector<uint8_t>> image_data;
    
    /// 缩略图数据（固定 256x256）
    std::shared_ptr<std::vector<uint8_t>> thumbnail_data;
    
    /// 最后访问时间（时间戳）
    std::chrono::system_clock::time_point last_access_time;
    
    /// 访问计数，用于 LRU 算法
    size_t access_count;
    
    /// @brief 更新访问信息
    /// @details 更新最后访问时间并增加访问计数
    void Touch() {
        last_access_time = std::chrono::system_clock::now();
        access_count++;
    }
};

/// @brief 图像缓存管理器
/// @details 实现 LRU (Least Recently Used) 缓存策略，
///          自动管理缓存大小，当超过上限时自动淘汰最久未使用的项。
///          支持异步加载，避免阻塞 UI 线程。
/// @note 此类线程安全，可在多线程环境中使用
/// @warning 缓存的图像数据可能占用大量内存，请合理设置缓存上限
class ImageCache {
public:
    /// @brief 构造函数
    /// @param max_size_mb 缓存最大容量（MB），默认 500MB
    explicit ImageCache(size_t max_size_mb = 500);
    
    /// @brief 析构函数
    /// @details 释放所有缓存数据，等待异步加载任务完成
    ~ImageCache();
    
    /// @brief 获取图像（如果缓存中不存在则从磁盘加载）
    /// @param[in] imagePath 图像文件路径
    /// @param[in] generate_thumbnail 是否生成缩略图（默认 true）
    /// @return 缓存项指针，如果加载失败返回 nullptr
    /// @note 此方法是线程安全的
    /// @warning 返回的指针在缓存项被淘汰后失效，请及时使用
    std::shared_ptr<ImageCacheItem> GetImage(const std::string& imagePath,
                                              bool generate_thumbnail = true);
    
    /// @brief 预加载图像到缓存
    /// @details 异步加载图像，不阻塞调用线程。
    ///          适用于预加载即将显示的图片。
    /// @param[in] imagePath 图像文件路径
    /// @param[in] callback 加载完成回调函数（可选）
    /// @return true: 成功启动预加载; false: 图像已在缓存中或路径无效
    /// @note 回调函数将在后台线程中执行
    bool PreloadImage(const std::string& imagePath,
                      std::function<void(bool success)> callback = nullptr);
    
    /// @brief 清除所有缓存
    /// @details 释放所有缓存的图像数据，重置缓存状态
    /// @note 此方法会等待所有异步加载任务完成
    void Clear();
    
    /// @brief 获取当前缓存大小（MB）
    /// @return 当前缓存大小（MB）
    size_t GetCurrentSize() const { return current_size_mb_; }
    
    /// @brief 获取缓存命中率
    /// @return 命中率（0.0 - 1.0）
    /// @note 用于性能监控和缓存策略调优
    double GetHitRate() const;
    
private:
    /// @brief LRU 淘汰算法
    /// @details 淘汰最久未使用的缓存项，直到缓存大小低于上限的 80%
    /// @note 此方法是私有方法，仅供内部调用
    void EvictLRU();
    
    /// 缓存字典，key 为图像路径
    std::unordered_map<std::string, std::shared_ptr<ImageCacheItem>> cache_;
    
    /// 缓存最大容量（MB）
    size_t max_size_mb_;
    
    /// 当前缓存大小（MB）
    size_t current_size_mb_;
    
    /// 缓存命中次数（用于统计命中率）
    size_t hit_count_;
    
    /// 缓存未命中次数
    size_t miss_count_;
    
    /// 互斥锁，保护缓存字典
    /// @note 使用 mutable 允许在 const 方法中锁定
    mutable std::mutex cache_mutex_;
    
    DISALLOW_COPY_AND_ASSIGN(ImageCache);
};

} // namespace cache
} // namespace photolib

#endif // PHOTONEST_IMAGE_CACHE_H_
```

---

## 禁忌与最佳实践

### ❌ 禁忌（不要这样做）

```cpp
// ❌ 错误示例 1: 废话注释
int age = 25; // 设置 age 为 25

// ❌ 错误示例 2: 注释与代码不一致
// 计算图像面积
int area = width * height * depth; // 实际上计算了体积

// ❌ 错误示例 3: 过度注释
// 将 x 加 1
x = x + 1;

// ❌ 错误示例 4: 使用英文注释但拼写错误
// This functon calcualte the image size
int GetImageSize() { ... }

// ❌ 错误示例 5: 注释掉的代码
// TODO: 删除此行
// int old_code = 0;
// old_function();
```

### ✅ 最佳实践

```cpp
// ✅ 正确示例 1: 说明意图
// 使用 4KB 对齐提升内存访问性能（CPU cache line 友好）
size_t aligned_size = (size + 4095) & ~4095;

// ✅ 正确示例 2: 解释复杂正则表达式
// 匹配图像文件扩展名：jpg/jpeg/png/gif/bmp/tiff/webp
// 不区分大小写
std::regex image_regex(R"(.*\.(jpg|jpeg|png|gif|bmp|tiff|webp)$)",
                       std::regex::icase);

// ✅ 正确示例 3: 说明特殊处理逻辑
// NOTE: CEF 在 Windows 下需要显式调用 SetFocus，
//       否则键盘事件无法正确传递到浏览器
if (IsWindows()) {
    SetFocus(browser_window_);
}

// ✅ 正确示例 4: 标注性能关键路径
// 性能关键路径：使用内存池避免频繁内存分配
// 测试数据：10000 次调用，内存池版本比 new/delete 快 3.5 倍
void* MemoryPool::Allocate(size_t size) { ... }
```

### 📏 注释长度建议

| 注释对象 | 建议长度 | 说明 |
|---------|---------|------|
| 文件头 | 10-20 行 | 包含版权、功能描述、修改记录 |
| 类 | 5-15 行 | 说明设计思路、使用场景、注意事项 |
| 公共函数 | 5-10 行 | 说明功能、参数、返回值、注意事项 |
| 私有函数 | 2-5 行 | 说明功能和算法思路 |
| 复杂逻辑块 | 3-10 行 | 说明算法、流程、特殊处理 |
| 成员变量 | 1-3 行 | 说明用途和注意事项 |

---

## Doxygen 常用标签速查表

| 标签 | 说明 | 示例 |
|-----|------|------|
| `@brief` | 简要描述 | `@brief 初始化浏览器` |
| `@details` | 详细描述 | `@details 创建浏览器窗口并设置事件处理器` |
| `@param[in]` | 输入参数 | `@param[in] url 目标 URL` |
| `@param[out]` | 输出参数 | `@param[out] result 处理结果` |
| `@param[in,out]` | 输入输出参数 | `@param[in,out] buffer 数据缓冲区` |
| `@return` | 返回值描述 | `@return 成功返回 true` |
| `@retval` | 具体返回值 | `@retval 0 成功; @retval -1 失败` |
| `@exception` | 异常说明 | `@exception std::runtime_error 文件打开失败` |
| `@note` | 注意事项 | `@note 此方法线程安全` |
| `@warning` | 警告信息 | `@warning 不要在渲染进程调用` |
| `@see` | 参见 | `@see OnAfterCreated()` |
| `@since` | 从哪个版本开始 | `@since v1.2.0` |
| `@deprecated` | 已弃用 | `@deprecated 使用 NewFunction() 替代` |
| `@todo` | 待完成 | `@todo 添加异步支持` |
| `@bug` | 已知 Bug | `@bug 大图像时可能崩溃 (Issue #123)` |
| `@example` | 示例代码 | `@example sample.cpp` |

---

## 注释审查清单

### ✅ 提交代码前自查

- [ ] 所有公共 API（类、函数、结构体）都有完整注释
- [ ] 所有注释都是中文（团队内部项目）
- [ ] 注释与代码逻辑一致，没有过时注释
- [ ] 复杂算法和关键逻辑都有注释说明
- [ ] 没有注释掉的代码（使用版本控制）
- [ ] 注释格式符合 Doxygen 规范
- [ ] 没有废话注释（如 `i++; // 递增 i`）
- [ ] 命名清晰的自解释代码可以少注释
- [ ] 所有 `TODO`、`FIXME`、`NOTE` 都有负责人和日期
- [ ] 文件头注释完整，包含修改记录

---

## 总结

好的注释应该：

1. **解释为什么** (Why)，而不仅仅是做什么 (What) 或怎么做 (How)
2. **补充代码无法表达的信息**，如设计思路、业务背景、特殊处理
3. **保持简洁准确**，避免冗长和歧义
4. **与代码同步更新**，避免过时注释误导他人
5. **使用统一的格式**，便于工具生成文档和团队协作

> **记住**: 代码告诉计算机做什么，注释告诉人类为什么这样做。

---

**文档版本**: v1.0  
**最后更新**: 2024-06-21  
**维护者**: PhotoNest Team
