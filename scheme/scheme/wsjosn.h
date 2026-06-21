#pragma once

class InfoDto {
public:
	/// @brief 默认构造函数
	InfoDto() { }

	/// @brief 析构函数
	~InfoDto() {};

public:
	/// @brief 序列化对象为 JSON 格式
	/// @tparam Writer RapidJSON 的 Writer 类型
	/// @param[in,out] writer RapidJSON 的 Writer 对象，用于输出 JSON
	/// @note 此基类仅输出 name-value 对，不包装在对象中
	template <typename Writer>
	void Serialize(Writer& writer) const {
		// This base class just write out name-value pairs, without wrapping within an object.
		writer.StartObject();
		writer.String("info");
#if RAPIDJSON_HAS_STDSTRING
		writer.String(info_);
#else
		writer.String(info_.c_str(), static_cast<SizeType>(info_.length())); // Supplying length of string is faster.
#endif
		writer.String("id");
		writer.Int(id_);

		writer.EndObject();
	}

public:
	std::string info_;  ///< 信息字符串
	uint32_t id_ = 0;      ///< ID 标识
};


////////////////////////////////////////////////////////////////////
/// @class CateGoryDto
/// @brief 分类信息 JSON 序列化类
/// @details 用于将分类信息序列化为 JSON 格式，
///          包括分类 ID、名称、完整 ID、目录路径等字段。
/// @note 所有字符串字段都使用 RapidJSON 的 String() 方法输出
////////////////////////////////////////////////////////////////////
class CateGoryDto {
public:
	/// @brief 默认构造函数
	CateGoryDto() {}

	/// @brief 析构函数
	~CateGoryDto() {};

public:
	/// @brief 序列化对象为 JSON 格式
	/// @tparam Writer RapidJSON 的 Writer 类型
	/// @param[in,out] writer RapidJSON 的 Writer 对象，用于输出 JSON
	template <typename Writer>
	void Serialize(Writer& writer) const {
		// This base class just write out name-value pairs, without wrapping within an object.
		writer.StartObject();
		writer.String("id");
		writer.String(id_.c_str(), static_cast<SizeType>(id_.length()));

		writer.String("name");
		writer.String(name_.c_str(), static_cast<SizeType>(name_.length()));

		//writer.String("comment");
		//writer.String(comment_.c_str(), static_cast<SizeType>(comment_.length()));

		writer.String("full_id");
		writer.String(full_id_.c_str(), static_cast<SizeType>(full_id_.length()));

		writer.String("dir");
		writer.String(dir_.c_str(), static_cast<SizeType>(dir_.length()));

		writer.String("root_dir");
		writer.String(root_dir_.c_str(), static_cast<SizeType>(root_dir_.length()));

		//writer.String("nb_images");
		//writer.String(nb_images_.c_str(), static_cast<SizeType>(nb_images_.length()));

		writer.String("fullname");
		writer.String(fullname_.c_str(), static_cast<SizeType>(fullname_.length()));
		writer.EndObject();
	}

public:
	std::string id_;           ///< 分类 ID
	std::string name_;         ///< 分类名称
	std::string full_id_;     ///< 完整分类 ID（包含父分类路径）
	std::string dir_;          ///< 分类目录路径
	std::string root_dir_;    ///< 根目录路径
	std::string fullname_;     ///< 完整名称（包含路径）
};


////////////////////////////////////////////////////////////////////
/// @class RetImageDto
/// @brief 返回图片信息 JSON 序列化类
/// @details 用于将图片信息序列化为 JSON 格式，
///          包括图片 ID、源路径、原始路径、名称、扩展名、时长、分类等字段。
/// @note 用于 API 返回图片列表时的 JSON 格式化
////////////////////////////////////////////////////////////////////
class RetImageDto {
public:
	/// @brief 默认构造函数
	RetImageDto() {}

	/// @brief 析构函数
	~RetImageDto() {};

public:
	/// @brief 序列化对象为 JSON 格式
	/// @tparam Writer RapidJSON 的 Writer 类型
	/// @param[in,out] writer RapidJSON 的 Writer 对象，用于输出 JSON
	template <typename Writer>
	void Serialize(Writer& writer) const {
		// This base class just write out name-value pairs, without wrapping within an object.
		writer.StartObject();
		writer.String("image_id");
		writer.String(image_id_.c_str(), static_cast<SizeType>(image_id_.length()));

		writer.String("src");
		writer.String(src_.c_str(), static_cast<SizeType>(src_.length()));

		writer.String("ori");
		writer.String(ori_.c_str(), static_cast<SizeType>(ori_.length()));

		writer.String("name");
		writer.String(name_.c_str(), static_cast<SizeType>(name_.length()));

		writer.String("fullname");
		writer.String(fullname_.c_str(), static_cast<SizeType>(fullname_.length()));

		writer.String("ext");
		writer.String(ext_.c_str(), static_cast<SizeType>(ext_.length()));

		writer.String("duration");
		writer.String(duration_.c_str(), static_cast<SizeType>(duration_.length()));

		writer.String("category");
		writer.String(category_.c_str(), static_cast<SizeType>(category_.length()));

		writer.EndObject();
	}

public:
	std::string image_id_;   ///< 图片 ID
	std::string ori_;        ///< 原始路径
	std::string src_;        ///< 缩略图路径
	std::string name_;       ///< 图片名称
	std::string fullname_;   ///< 完整名称（包含路径）
	std::string ext_;        ///< 文件扩展名
	std::string duration_;   ///< 媒体时长（视频/音频）
	std::string category_;    ///< 所属分类
};


