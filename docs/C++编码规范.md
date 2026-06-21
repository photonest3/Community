# C++ 编码规范

> **项目**: PhotoNest 高容量图像库软件  
> **版本**: v1.0  
> **日期**: 2024-06-21  
> **适用**: C++11/14/17 标准，CEF 框架项目，Windows 平台

---

## 📋 目录

1. [规范原则](#规范原则)
2. [命名规范](#命名规范)
3. [代码格式](#代码格式)
4. [注释规范](#注释规范)
5. [函数和方法](#函数和方法)
6. [类和结构体](#类和结构体)
7. [错误处理](#错误处理)
8. [内存管理](#内存管理)
9. [多线程安全](#多线程安全)
10. [CEF 框架规范](#cef-框架规范)
11. [禁忌与最佳实践](#禁忌与最佳实践)

---

## 规范原则

### ✅ 核心原则

1. **可读性优先**: 代码是写给人看的，顺便让机器执行
2. **一致性**: 整个项目保持统一的编码风格
3. **简洁明确**: 避免过于复杂的语法和隐式行为
4. **安全性**: 防止内存泄漏、越界访问、竞态条件
5. **性能意识**: 在可读性和性能之间找到平衡

### 📝 规范等级

| 等级 | 说明 | 示例 |
|------|------|------|
| **必须** | 严格遵守，违反会导致审查不通过 | 必须使用中文注释 |
| **推荐** | 强烈建议遵守，特殊情况可例外 | 推荐使用 `std::string` 而非 `CString` |
| **可选** | 建议采用，不强制 | 可选使用 `auto` 关键字 |

---

## 命名规范

### 文件命名

| 类型 | 规则 | 示例 |
|------|------|------|
| 头文件 | `小写字母_描述性名称.h` | `client_handler.h`, `image_cache.h` |
| 源文件 | `小写字母_描述性名称.cpp` | `client_handler.cpp`, `image_cache.cpp` |
| 模板文件 | `小写字母_描述性名称.inl` 或 `.h` | `vector.inl` |

**规则**:
- ✅ 使用小写字母和下划线分隔单词
- ✅ 文件名应清晰描述文件内容
- ❌ 避免使用缩写（除非是广为人知的缩写）
- ❌ 避免超过 31 个字符（某些文件系统不区分大小写）

### 命名空间命名

| 规则 | 示例 |
|------|------|
| ✅ 使用小写字母 | `namespace photolib` |
| ✅ 简短且有意义 | `namespace cache`, `namespace utils` |
| ❌ 避免使用下划线开头或结尾 | ❌ `namespace _internal` |

```cpp
// ✅ 正确示例
namespace photolib {
namespace cache {
    // ...
} // namespace cache
} // namespace photolib

// ❌ 错误示例
namespace PhotoLib {  // 不应使用 PascalCase
    // ...
}
```

### 类/结构体命名

| 规则 | 示例 |
|------|------|
| ✅ 使用 PascalCase | `class ClientHandler`, `struct ImageCacheItem` |
| ✅ 类名应是名词或名词短语 | `class ImageDecoder`, `class UrlRouter` |
| ✅ 接口类以 `I` 开头（可选） | `class IImageProcessor` |
| ❌ 避免使用下划线 | ❌ `class image_handler` |

```cpp
// ✅ 正确示例
class ClientHandler : public CefClient {
    // ...
};

struct ImageCacheItem {
    std::string file_path;
    // ...
};

// ❌ 错误示例
class client_handler {  // 应使用 PascalCase
    // ...
};
```

### 函数/方法命名

| 规则 | 示例 |
|------|------|
| ✅ 使用 camelCase | `void loadUrl()`, `bool isValid()` |
| ✅ 函数名应是动词或动词短语 | `void processImage()`, `bool checkPermission()` |
| ✅ 获取器/设置器使用 `get`/`set` 前缀 | `std::string getName()`, `void setName(const std::string& name)` |
| ✅ 布尔返回值函数使用 `is`/`has`/`can` 开头 | `bool isLoading()`, `bool hasPermission()`, `bool canProcess()` |
| ❌ 避免使用下划线 | ❌ `void load_url()` |

```cpp
// ✅ 正确示例
class ImageProcessor {
public:
    /// @brief 加载图像
    bool loadImage(const std::string& filePath);
    
    /// @brief 获取图像宽度
    int getWidth() const { return width_; }
    
    /// @brief 是否正在加载
    bool isLoading() const { return is_loading_; }
    
    /// @brief 设置缓存大小
    void setCacheSize(size_t size_mb);
};

// ❌ 错误示例
class ImageProcessor {
public:
    bool load_image(const std::string& file_path);  // 应使用 camelCase
    int get_width() const;  // 应使用 getWidth
};
```

### 变量命名

| 类型 | 规则 | 示例 |
|------|------|------|
| 局部变量 | camelCase | `int imageCount`, `std::string filePath` |
| 成员变量 | snake_case_ 结尾下划线 | `int image_count_`, `std::string file_path_` |
| 静态成员变量 | snake_case_ 结尾下划线 | `static int instance_count_` |
| 全局变量 | g_ 前缀 + snake_case | `g_log_level`, `g_config_path` |
| 常量 | k 前缀 + PascalCase 或全大写下划线 | `const int kMaxRetryCount`, `const int MAX_RETRY` |
| 枚举值 | k 前缀 + PascalCase 或全大写下划线 | `kImageFormatJPEG`, `IMAGE_FORMAT_JPEG` |

```cpp
// ✅ 正确示例
class ImageCache {
private:
    /// 缓存字典
    std::unordered_map<std::string, ImageCacheItem> cache_;
    
    /// 缓存最大容量（MB）
    size_t max_size_mb_;
    
    /// 实例计数（静态）
    static int instance_count_;
    
    /// 最大重试次数（常量）
    static const int kMaxRetryCount = 3;
};

/// 全局变量
int g_log_level = 0;
std::string g_config_path;

/// 枚举
enum class ImageFormat {
    kJPEG,
    kPNG,
    kGIF,
    kBMP
};

// ❌ 错误示例
class ImageCache {
private:
    std::unordered_map<std::string, ImageCacheItem> Cache;  // 成员变量应使用 snake_case_
    int MaxSize;  // 应使用 snake_case_
};
```

### 宏命名

| 规则 | 示例 |
|------|------|
| ✅ 全大写下划线分隔 | `#define MAX_IMAGE_SIZE 1920` |
| ✅ 头文件保护使用全大写下划线 | `#ifndef PHOTONEST_IMAGE_CACHE_H_` |
| ❌ 避免使用小写 | ❌ `#define max_image_size 1920` |

```cpp
// ✅ 正确示例
#ifndef PHOTONEST_IMAGE_CACHE_H_
#define PHOTONEST_IMAGE_CACHE_H_

/// 最大图像尺寸
#define MAX_IMAGE_WIDTH 1920
#define MAX_IMAGE_HEIGHT 1080

/// 安全释放宏
#define SAFE_RELEASE(ptr) \
    do { \
        if ((ptr) != nullptr) { \
            (ptr)->Release(); \
            (ptr) = nullptr; \
        } \
    } while(0)

#endif // PHOTONEST_IMAGE_CACHE_H_

// ❌ 错误示例
#ifndef photolib_image_cache_h  // 应使用全大写
#define photolib_image_cache_h
```

---

## 代码格式

### 缩进和空格

| 规则 | 说明 |
|------|------|
| ✅ 使用 4 空格缩进 | 不使用 Tab |
| ✅ 花括号 `{}` 独占一行 | K&R 风格 |
| ✅ 括号内外加空格 | `if (condition)`, `function(arg1, arg2)` |
| ✅ 运算符前后加空格 | `a = b + c`, `x == y` |
| ❌ 避免行尾留空格 | 使用 `.gitconfig` 自动清理 |

```cpp
// ✅ 正确示例
if (is_loading_) {
    int retry_count = 0;
    while (retry_count < kMaxRetryCount) {
        ProcessImage(image_path_);
        retry_count++;
    }
}

// ❌ 错误示例
if(is_loading_){  // 缺少空格，花括号应独占一行
  int retry_count=0;  // 运算符缺少空格
    while(retry_count<kMaxRetryCount){  // 缩进不一致
        // ...
    }
}
```

### 行宽限制

| 规则 | 说明 |
|------|------|
| ✅ 每行不超过 120 字符 | 超过时换行 |
| ✅ 换行后与上一行对齐 | 使用空格对齐 |
| ✅ 函数参数换行时，每个参数独占一行 | 提高可读性 |

```cpp
// ✅ 正确示例：函数参数换行
bool LoadImage(const std::string& file_path,
               int max_width,
               int max_height,
               bool generate_thumbnail,
               std::function<void(bool success)> callback);

// ✅ 正确示例：长表达式换行
int total_size = CalculateImageSize(image_width, image_height,
                                   image_format, compression_quality);

// ❌ 错误示例：行宽超过 120 字符
bool LoadImage(const std::string& file_path, int max_width, int max_height, bool generate_thumbnail, std::function<void(bool success)> callback);
```

### 空行和分组

| 规则 | 说明 |
|------|------|
| ✅ 函数之间空 2 行 | 清晰分隔 |
| ✅ 逻辑分组之间空 1 行 | 提高可读性 |
| ✅ 头文件包含分组，组间空 1 行 | 按顺序：标准库、第三方库、项目头文件 |

```cpp
// ✅ 正确示例：头文件包含
#include <iostream>
#include <string>
#include <vector>

#include <cef/cef.h>
#include <sqlite3.h>

#include "client_handler.h"
#include "image_cache.h"


// ✅ 正确示例：逻辑分组
void ProcessImage() {
    // 步骤 1: 验证输入
    if (file_path_.empty()) {
        return;
    }
    
    // 步骤 2: 检查缓存
    auto cache_item = cache_.GetImage(file_path_);
    if (cache_item != nullptr) {
        return cache_item;
    }
    
    // 步骤 3: 从磁盘加载
    // ...
}
```

---

## 注释规范

> **详细规范请参考**: [docs/C++代码注释规范.md](./C++代码注释规范.md)

### 快速参考

| 注释类型 | 使用场景 | 格式 |
|---------|---------|------|
| 文件头注释 | 每个 `.h`/`.cpp` 文件开头 | 参见注释规范 |
| 类/结构体注释 | 所有对外接口 | `/// @brief ...` (Doxygen) |
| 公共方法注释 | 所有 `public` 方法 | `/// @brief ...` (Doxygen) |
| 私有方法注释 | 复杂逻辑的方法 | `/// @brief ...` (可选) |
| 成员变量注释 | 所有 `public`/`protected` 成员 | `/// ...` |
| 代码逻辑注释 | 复杂算法、关键业务逻辑 | `// ...` |

---

## 函数和方法

### 参数传递

| 场景 | 推荐方式 | 示例 |
|------|---------|------|
| 输入参数（小对象） | `const Type&` 或 `Type` | `void Process(int count)` |
| 输入参数（字符串） | `const std::string&` | `void Load(const std::string& path)` |
| 输入参数（大对象） | `const Type&` | `void Process(const BigObject& obj)` |
| 输出参数 | `Type*` 或 `Type&` | `bool GetValue(int* output)` |
| 输入输出参数 | `Type&` | `void Modify(std::string& text)` |
| 智能指针参数 | `std::shared_ptr<Type>` | `void Add(std::shared_ptr<Item> item)` |
| 转移所有权 | `std::unique_ptr<Type>` | `void Set(std::unique_ptr<Item> item)` |

```cpp
// ✅ 正确示例
class ImageProcessor {
public:
    /// @brief 加载图像（输入参数使用 const 引用）
    bool LoadImage(const std::string& file_path);
    
    /// @brief 获取图像数据（返回 const 引用避免拷贝）
    const ImageData& GetImageData() const { return image_data_; }
    
    /// @brief 处理图像（输出参数使用指针）
    /// @param[out] output_data 处理后的图像数据
    /// @return 成功返回 true
    bool ProcessImage(ImageData* output_data);
    
    /// @brief 设置配置（转移所有权）
    void SetConfig(std::unique_ptr<Config> config);
};

// ❌ 错误示例
class ImageProcessor {
public:
    bool LoadImage(std::string file_path);  // 应使用 const 引用，避免不必要的拷贝
    ImageData GetImageData();  // 应返回 const 引用，避免拷贝
    bool ProcessImage(ImageData output_data);  // 应使用指针表示输出参数
};
```

### 返回值

| 场景 | 推荐方式 | 示例 |
|------|---------|------|
| 返回小对象 | 值返回 | `int GetCount()` |
| 返回字符串 | `std::string` 值返回（C++11 后有移动语义） | `std::string GetName()` |
| 返回大对象 | `std::shared_ptr<Type>` 或 `const Type&` | `std::shared_ptr<ImageData> GetImage()` |
| 返回接口 | `std::unique_ptr<Interface>` | `std::unique_ptr<IImageProcessor> CreateProcessor()` |
| 返回多个值 | `std::tuple` 或输出参数 | `std::tuple<bool, std::string> Process()` |

```cpp
// ✅ 正确示例
class ImageCache {
public:
    /// @brief 获取图像（返回智能指针）
    std::shared_ptr<ImageCacheItem> GetImage(const std::string& path) {
        auto it = cache_.find(path);
        if (it != cache_.end()) {
            return it->second;
        }
        return nullptr;
    }
    
    /// @brief 加载图像（返回多个值使用 tuple）
    std::tuple<bool, std::string> LoadImage(const std::string& path) {
        if (path.empty()) {
            return std::make_tuple(false, "路径为空");
        }
        // ...
        return std::make_tuple(true, "成功");
    }
};
```

### 常量成员函数

| 规则 | 说明 |
|------|------|
| ✅ 不修改对象状态的函数应声明为 `const` | 提高代码安全性和可读性 |
| ✅ `const` 函数只能调用其他 `const` 函数 | 编译期保证 |
| ✅ `const` 函数不能修改成员变量 | 除非成员变量被声明为 `mutable` |

```cpp
// ✅ 正确示例
class ImageInfo {
public:
    /// @brief 获取图像宽度（const 函数）
    int GetWidth() const {
        return width_;
    }
    
    /// @brief 获取图像高度（const 函数）
    int GetHeight() const {
        return height_;
    }
    
    /// @brief 获取宽高比（const 函数）
    double GetAspectRatio() const {
        if (height_ == 0) {
            return 0.0;
        }
        return static_cast<double>(width_) / height_;
    }
    
    /// @brief 更新访问时间（非 const 函数）
    void Touch() {
        last_access_time_ = std::chrono::system_clock::now();
        access_count_++;
    }

private:
    int width_;
    int height_;
    
    /// 最后访问时间（mutable 允许在 const 函数中修改）
    mutable std::chrono::system_clock::time_point last_access_time_;
    
    int access_count_;
};
```

---

## 类和结构体

### 继承规范

| 规则 | 说明 |
|------|------|
| ✅ 基类的析构函数应声明为 `virtual` | 确保正确调用派生类析构函数 |
| ✅ 继承使用 `public` 继承 | 避免使用 `private`/`protected` 继承 |
| ✅ 覆盖父类虚函数使用 `override` 关键字 | 编译期检查 |
| ✅ 不希望被覆盖的虚函数使用 `final` 关键字 | 防止进一步覆盖 |

```cpp
// ✅ 正确示例
class BaseProcessor {
public:
    /// 虚析构函数
    virtual ~BaseProcessor() = default;
    
    /// 虚函数
    virtual void Process() {
        // 默认实现
    }
    
    /// 纯虚函数
    virtual void Initialize() = 0;
};

class ImageProcessor : public BaseProcessor {
public:
    /// 覆盖虚函数使用 override
    void Process() override {
        // 自定义实现
    }
    
    /// 实现纯虚函数
    void Initialize() override {
        // 初始化
    }
};

class SpecializedProcessor : public ImageProcessor {
public:
    /// 使用 final 防止进一步覆盖
    void Process() override final {
        // 最终实现
    }
};
```

### 拷贝控制和移动语义

| 规则 | 说明 |
|------|------|
| ✅ 明确指定拷贝构造函数、移动构造函数、赋值运算符 | 使用 `= default`、`= delete`、`= default` |
| ✅ 不需要拷贝的类使用 `DISALLOW_COPY_AND_ASSIGN` 宏 | 防止意外拷贝 |
| ✅ C++11 后优先使用 `= default` 而非手动定义 | 提高代码简洁性 |
| ✅ 管理资源的类应实现移动语义 | 提高性能 |

```cpp
// ✅ 正确示例 1: 禁用拷贝
class ImageCache {
public:
    ImageCache() = default;
    ~ImageCache() = default;
    
    /// 禁用拷贝
    ImageCache(const ImageCache&) = delete;
    ImageCache& operator=(const ImageCache&) = delete;
    
    /// 允许移动
    ImageCache(ImageCache&&) = default;
    ImageCache& operator=(ImageCache&&) = default;
};

// ✅ 正确示例 2: 使用宏禁用拷贝
#define DISALLOW_COPY_AND_ASSIGN(class_name) \
private:                                    \
    class_name(const class_name&);            \
    class_name& operator=(const class_name&)

class ImageDecoder {
public:
    ImageDecoder() = default;
    ~ImageDecoder() = default;
    
private:
    DISALLOW_COPY_AND_ASSIGN(ImageDecoder);
};

// ✅ 正确示例 3: 实现移动语义
class ImageData {
public:
    ImageData() : data_(nullptr), size_(0) {}
    
    /// 移动构造函数
    ImageData(ImageData&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }
    
    /// 移动赋值运算符
    ImageData& operator=(ImageData&& other) noexcept {
        if (this != &other) {
            DeleteData();
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    
    ~ImageData() {
        DeleteData();
    }
    
private:
    void DeleteData() {
        if (data_ != nullptr) {
            delete[] data_;
            data_ = nullptr;
        }
    }
    
private:
    uint8_t* data_;
    size_t size_;
};
```

---

## 错误处理

### 错误处理策略

| 场景 | 推荐方式 | 示例 |
|------|---------|------|
| 可恢复错误 | 返回 `bool` 或 `std::optional` | `bool LoadImage()` |
| 不可恢复错误 | 抛出异常 | `throw std::runtime_error("...")` |
| C 接口 | 返回错误码 | `int ProcessImage()` |
| 性能关键路径 | 返回 `bool` + 输出参数 | `bool Process(bool* success)` |

```cpp
// ✅ 正确示例 1: 返回 bool 表示成功/失败
class ImageLoader {
public:
    /// @brief 加载图像
    /// @return true: 成功; false: 失败
    bool LoadImage(const std::string& file_path) {
        if (file_path.empty()) {
            return false;  // 可恢复错误
        }
        
        // ...
        return true;
    }
};

// ✅ 正确示例 2: 返回 std::optional (C++17)
class ConfigManager {
public:
    /// @brief 获取配置值
    /// @return 配置值，不存在返回 std::nullopt
    std::optional<std::string> GetValue(const std::string& key) {
        auto it = config_.find(key);
        if (it != config_.end()) {
            return it->second;
        }
        return std::nullopt;
    }
};

// ✅ 正确示例 3: 抛出异常（不可恢复错误）
class DatabaseManager {
public:
    /// @brief 连接数据库
    /// @exception std::runtime_error 连接失败
    void Connect(const std::string& db_path) {
        if (sqlite3_open(db_path.c_str(), &db_) != SQLITE_OK) {
            throw std::runtime_error("无法打开数据库: " + db_path);
        }
    }
};

// ✅ 正确示例 4: RAII 自动清理
class SqliteStatement {
public:
    explicit SqliteStatement(sqlite3* db, const char* sql) {
        if (sqlite3_prepare_v2(db, sql, -1, &stmt_, nullptr) != SQLITE_OK) {
            throw std::runtime_error("SQL 准备失败");
        }
    }
    
    ~SqliteStatement() {
        if (stmt_ != nullptr) {
            sqlite3_finalize(stmt_);
        }
    }
    
private:
    sqlite3_stmt* stmt_ = nullptr;
};
```

### 异常安全

| 规则 | 说明 |
|------|------|
| ✅ 使用 RAII 管理资源 | 确保异常时资源自动释放 |
| ✅ 避免使用裸指针管理资源 | 使用智能指针 |
| ✅ 析构函数不应抛出异常 | 使用 `noexcept` 声明 |
| ✅ 提供强异常安全保证 | 操作失败时状态回滚 |

```cpp
// ✅ 正确示例: RAII 管理资源
class ImageFile {
public:
    ImageFile(const std::string& file_path) 
        : file_(fopen(file_path.c_str(), "rb")) {
        if (file_ == nullptr) {
            throw std::runtime_error("无法打开文件: " + file_path);
        }
    }
    
    ~ImageFile() {
        if (file_ != nullptr) {
            fclose(file_);
        }
    }
    
    // 禁用拷贝
    ImageFile(const ImageFile&) = delete;
    ImageFile& operator=(const ImageFile&) = delete;
    
    // 允许移动
    ImageFile(ImageFile&& other) noexcept : file_(other.file_) {
        other.file_ = nullptr;
    }
    
private:
    FILE* file_;
};

// ❌ 错误示例: 不使用 RAII，异常时可能泄漏资源
class ImageFile {
public:
    ImageFile(const std::string& file_path) {
        file_ = fopen(file_path.c_str(), "rb");
        if (file_ == nullptr) {
            throw std::runtime_error("无法打开文件");
        }
        // 如果此处抛出异常，file_ 不会自动关闭
        buffer_ = new uint8_t[1024];
    }
    
    ~ImageFile() {
        if (file_ != nullptr) {
            fclose(file_);
        }
        delete[] buffer_;
    }
    
private:
    FILE* file_;
    uint8_t* buffer_;
};
```

---

## 内存管理

### 智能指针使用规范

| 场景 | 推荐方式 | 示例 |
|------|---------|------|
| 独占所有权 | `std::unique_ptr` | `std::unique_ptr<Image> img` |
| 共享所有权 | `std::shared_ptr` | `std::shared_ptr<Image> img` |
| 弱引用 | `std::weak_ptr` | `std::weak_ptr<Image> weak_img` |
| 数组 | `std::unique_ptr<T[]>` | `std::unique_ptr<uint8_t[]> buf` |

```cpp
// ✅ 正确示例 1: 使用 std::unique_ptr 管理独占资源
class ImageDecoder {
public:
    /// @brief 解码图像
    /// @return 解码后的图像（独占所有权）
    std::unique_ptr<ImageData> Decode(const std::string& file_path) {
        auto image = std::make_unique<ImageData>();
        // ... 解码 ...
        return image;  // 移动语义，无拷贝
    }
};

// ✅ 正确示例 2: 使用 std::shared_ptr 共享资源
class ImageCache {
public:
    /// @brief 获取图像（共享所有权）
    std::shared_ptr<ImageCacheItem> GetImage(const std::string& path) {
        auto it = cache_.find(path);
        if (it != cache_.end()) {
            return it->second;  // 返回 shared_ptr，引用计数 +1
        }
        return nullptr;
    }
    
private:
    std::unordered_map<std::string, std::shared_ptr<ImageCacheItem>> cache_;
};

// ✅ 正确示例 3: 使用 std::weak_ptr 打破循环引用
class Image {
public:
    /// @brief 设置父图像
    void SetParent(std::shared_ptr<Image> parent) {
        parent_ = parent;  // 使用 weak_ptr 避免循环引用
    }
    
private:
    std::weak_ptr<Image> parent_;  // 弱引用，不影响引用计数
};

// ❌ 错误示例: 使用裸指针
class ImageCache {
public:
    ImageCacheItem* GetImage(const std::string& path) {
        // 问题 1: 谁负责删除？
        // 问题 2: 如果调用者忘记删除，内存泄漏
        // 问题 3: 如果多次删除，崩溃
        return new ImageCacheItem();
    }
};
```

### 禁止使用的 C++ 特性

| 特性 | 原因 | 替代方案 |
|------|------|---------|
| `goto` | 破坏代码结构 | 使用循环 + `break`/`continue` |
| 全局变量（非 const） | 导致隐式依赖 | 使用依赖注入 |
| C 风格数组 | 不安全，无边界检查 | 使用 `std::array` 或 `std::vector` |
| `new`/`delete` | 容易泄漏 | 使用智能指针 |
| 宏定义常量 | 无类型检查 | 使用 `constexpr` 或 `const` |
| C 风格类型转换 | 不安全 | 使用 `static_cast` 等 |

```cpp
// ❌ 禁止使用
goto error;  // 破坏代码结构

int g_global_counter = 0;  // 全局变量

int arr[10];  // C 风格数组，无边界检查
arr[10] = 0;  // 越界访问，未定义行为

int* ptr = new int(42);  // 应使用智能指针
// ... 如果此处抛出异常，ptr 泄漏
delete ptr;

#define MAX_SIZE 1024  // 宏定义常量，无类型检查

int* raw_ptr = (int*)malloc(sizeof(int));  // C 风格类型转换

// ✅ 推荐替代方案
// 1. 不使用 goto，使用结构化控制流
if (error_condition) {
    // 错误处理
    return false;
}

// 2. 不使用全局变量，使用依赖注入
class ImageProcessor {
public:
    explicit ImageProcessor(int max_size) : max_size_(max_size) {}
    
private:
    int max_size_;
};

// 3. 使用 std::array 或 std::vector
std::array<int, 10> arr{};  // 栈上分配，有边界检查（at() 方法）
std::vector<int> vec(10);    // 堆上分配，动态大小

// 4. 使用智能指针
auto ptr = std::make_unique<int>(42);

// 5. 使用 constexpr 或 const
constexpr int kMaxSize = 1024;

// 6. 使用 C++ 类型转换
int* raw_ptr = static_cast<int*>(malloc(sizeof(int)));
```

---

## 多线程安全

### 线程安全规则

| 规则 | 说明 |
|------|------|
| ✅ 共享数据访问使用互斥锁保护 | `std::mutex` + `std::lock_guard` |
| ✅ 避免死锁，使用 `std::lock()` 同时锁定多个互斥锁 | RAII 风格 |
| ✅ 优先使用无锁算法或原子操作 | `std::atomic` |
| ✅ 线程间通信使用任务队列或消息传递 | 避免共享状态 |
| ❌ 避免在持有锁时调用外部代码 | 可能导致死锁 |

```cpp
// ✅ 正确示例 1: 使用互斥锁保护共享数据
class ImageCache {
public:
    /// @brief 获取图像（线程安全）
    std::shared_ptr<ImageCacheItem> GetImage(const std::string& path) {
        std::lock_guard<std::mutex> lock(cache_mutex_);  // RAII 自动加锁/解锁
        auto it = cache_.find(path);
        if (it != cache_.end()) {
            return it->second;
        }
        return nullptr;
    }
    
    /// @brief 添加图像（线程安全）
    void AddImage(const std::string& path, std::shared_ptr<ImageCacheItem> item) {
        std::lock_guard<std::mutex> lock(cache_mutex_);
        cache_[path] = item;
    }
    
private:
    std::unordered_map<std::string, std::shared_ptr<ImageCacheItem>> cache_;
    mutable std::mutex cache_mutex_;  // mutable 允许在 const 函数中锁定
};

// ✅ 正确示例 2: 使用原子操作
class AtomicCounter {
public:
    /// @brief 递增计数器（原子操作，线程安全）
    void Increment() {
        counter_.fetch_add(1, std::memory_order_relaxed);
    }
    
    /// @brief 获取当前值
    int Get() const {
        return counter_.load(std::memory_order_relaxed);
    }
    
private:
    std::atomic<int> counter_{0};
};

// ✅ 正确示例 3: 避免死锁，同时锁定多个互斥锁
class BankAccount {
public:
    /// @brief 转账（线程安全，无死锁）
    bool Transfer(BankAccount& to, int amount) {
        // 同时锁定两个账户，使用 std::lock 避免死锁
        std::unique_lock<std::mutex> lock1(mutex_, std::defer_lock);
        std::unique_lock<std::mutex> lock2(to.mutex_, std::defer_lock);
        std::lock(lock1, lock2);  // 同时锁定，避免死锁
        
        if (balance_ < amount) {
            return false;
        }
        
        balance_ -= amount;
        to.balance_ += amount;
        return true;
    }
    
private:
    int balance_ = 0;
    mutable std::mutex mutex_;
};
```

---

## CEF 框架规范

### CEF 对象生命周期管理

| 规则 | 说明 |
|------|------|
| ✅ CEF 对象使用 `CefRefPtr` 管理生命周期 | 引用计数智能指针 |
| ✅ 不要在栈上创建 CEF 对象 | 必须使用 `CefRefPtr` |
| ✅ 避免在回调中长时间持有 `CefRefPtr` | 可能导致对象生命周期延长 |
| ✅ 使用 `CefPostTask` 跨线程调用 | CEF 线程模型 |

```cpp
// ✅ 正确示例 1: 使用 CefRefPtr 管理 CEF 对象
class ClientHandler : public CefClient {
public:
    /// @brief 创建浏览器
    void CreateBrowser(const CefString& url) {
        CefWindowInfo window_info;
        CefBrowserSettings settings;
        
        // 使用 CefRefPtr 管理生命周期
        CefRefPtr<CefClient> client(this);
        
        // 创建浏览器
        CefBrowserHost::CreateBrowser(window_info, client, url, settings, nullptr, nullptr);
    }
    
    /// @brief 获取浏览器对象
    CefRefPtr<CefBrowser> GetBrowser() const {
        return browser_;
    }
    
private:
    /// 浏览器对象（使用 CefRefPtr 管理）
    CefRefPtr<CefBrowser> browser_;
    
    IMPLEMENT_REFCOUNTING(ClientHandler);  // CEF 引用计数宏
};

// ✅ 正确示例 2: 跨线程调用
class BrowserManager {
public:
    /// @brief 在 UI 线程中执行任务
    void ExecuteOnUIThread(std::function<void()> task) {
        // 使用 CefPostTask 将任务抛到 UI 线程
        CefPostTask(TID_UI, base::BindOnce(task));
    }
    
    /// @brief 在渲染进程主线程中执行任务
    void ExecuteOnRenderThread(std::function<void()> task) {
        CefPostTask(TID_RENDERER, base::BindOnce(task));
    }
};
```

### CEF 消息传递

| 规则 | 说明 |
|------|------|
| ✅ 使用 `CefProcessMessage` 在进程间传递消息 | 浏览器进程 ↔ 渲染进程 |
| ✅ 使用 `CefV8Value` 和 `CefV8Handler` 与 JavaScript 交互 | JS ↔ C++ |
| ✅ 进程间传递的数据需要序列化 | 使用 JSON 或自定义二进制格式 |

```cpp
// ✅ 正确示例: 浏览器进程向渲染进程发送消息
class BrowserProcessHandler : public CefBrowserProcessHandler {
public:
    /// @brief 向渲染进程发送消息
    void SendMessageToRenderer(CefRefPtr<CefBrowser> browser,
                               const std::string& message_name,
                               const std::string& message_data) {
        // 创建进程间消息
        CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(message_name);
        
        // 设置消息参数
        CefRefPtr<CefListValue> args = message->GetArgumentList();
        args->SetString(0, message_data);
        
        // 发送到渲染进程
        browser->GetMainFrame()->SendProcessMessage(PID_RENDERER, message);
    }
};

// ✅ 正确示例: 渲染进程接收消息
class RenderProcessHandler : public CefRenderProcessHandler {
public:
    /// @brief 接收来自浏览器进程的消息
    bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                  CefProcessId source_process,
                                  CefRefPtr<CefProcessMessage> message) override {
        std::string message_name = message->GetName();
        CefRefPtr<CefListValue> args = message->GetArgumentList();
        
        if (message_name == "update_image") {
            std::string image_path = args->GetString(0);
            // 处理消息...
            return true;
        }
        
        return false;
    }
};
```

---

## 禁忌与最佳实践

### ❌ 禁忌（不要这样做）

```cpp
// ❌ 禁忌 1: 使用 namespace std
using namespace std;  // 污染全局命名空间

// ❌ 禁忌 2: 在头文件中定义全局变量
// header.h
int g_global_variable = 0;  // 多个编译单元包含时会导致链接错误

// ❌ 禁忌 3: 返回局部变量的引用或指针
int& GetValue() {
    int value = 42;
    return value;  // 返回局部变量引用，未定义行为
}

// ❌ 禁忌 4: 在析构函数中抛出异常
~ImageFile() {
    if (fclose(file_) != 0) {
        throw std::runtime_error("关闭文件失败");  // 析构函数不应抛出异常
    }
}

// ❌ 禁忌 5: 使用 std::auto_ptr (C++11 已弃用)
std::auto_ptr<Image> img(new Image());  // 应使用 std::unique_ptr

// ❌ 禁忌 6: 在头文件中使用 using namespace
// bad.h
#ifndef BAD_H_
#define BAD_H_
using namespace std;  // 污染所有包含此头文件的编译单元
#endif
```

### ✅ 最佳实践

```cpp
// ✅ 最佳实践 1: 使用 constexpr 提高性能
constexpr int GetMaxImageSize() {
    return 1920 * 1080 * 4;  // 编译期计算，无运行时开销
}

// ✅ 最佳实践 2: 使用范围 for 循环
vector<int> values = {1, 2, 3, 4, 5};
for (int value : values) {  // C++11 范围 for，简洁且安全
    cout << value << endl;
}

// ✅ 最佳实践 3: 使用 auto 简化类型声明（适度使用）
auto it = complex_map.find(key);  // 避免写冗长的迭代器类型
// 但避免过度使用 auto，影响代码可读性

// ✅ 最佳实践 4: 使用初始化列表
class Image {
public:
    Image(int width, int height) 
        : width_(width), height_(height), data_(nullptr) {  // 初始化列表，效率更高
        data_ = new uint8_t[width * height * 4];
    }
    
private:
    int width_;
    int height_;
    uint8_t* data_;
};

// ✅ 最佳实践 5: 使用 =default 生成默认函数
class ImageProcessor {
public:
    ImageProcessor() = default;              // 使用编译器生成的默认构造函数
    ~ImageProcessor() = default;            // 使用编译器生成的默认析构函数
    ImageProcessor(const ImageProcessor&) = default;  // 使用编译器生成的拷贝构造函数
    ImageProcessor& operator=(const ImageProcessor&) = default;  // 使用编译器生成的拷贝赋值运算符
};

// ✅ 最佳实践 6: 使用 nullptr 而非 NULL
int* ptr = nullptr;  // C++11 后使用 nullptr，类型安全
// int* ptr = NULL;  // 应避免，NULL 通常是 0，无类型安全

// ✅ 最佳实践 7: 使用 enum class 而非 enum
enum class ImageFormat {  // 枚举类，有作用域，无隐式类型转换
    kJPEG,
    kPNG,
    kGIF
};

ImageFormat format = ImageFormat::kJPEG;  // 必须使用作用域
// int value = format;  // 错误，无隐式类型转换

// ❌ 避免使用传统 enum
enum ImageFormatOld {
    JPEG,
    PNG,
    GIF
};

ImageFormatOld format_old = JPEG;  // 污染全局作用域
int value = format_old;  // 隐式类型转换，可能导致错误
```

---

## 总结

### ✅ 规范检查清单

**提交代码前自查**:

- [ ] 命名符合规范（类名 PascalCase，函数名 camelCase，成员变量 snake_case_）
- [ ] 代码格式正确（4 空格缩进，花括号独占一行）
- [ ] 所有对外接口都有完整注释（符合 Doxygen 规范）
- [ ] 使用智能指针管理资源（不使用 `new`/`delete`）
- [ ] 异常安全（使用 RAII，析构函数不抛异常）
- [ ] 线程安全（共享数据使用互斥锁保护）
- [ ] 无禁忌用法（`goto`、全局变量、`using namespace std` 等）
- [ ] CEF 对象使用 `CefRefPtr` 管理
- [ ] 无警告编译（`-Wall -Wextra -Wpedantic`）

---

**文档版本**: v1.0  
**最后更新**: 2024-06-21  
**维护者**: PhotoNest Team
