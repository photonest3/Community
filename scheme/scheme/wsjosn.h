#pragma once

class InfoDto {
public:
	InfoDto() { }
	~InfoDto() {};

public:
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
	std::string info_;
	uint32_t id_ = 0;
};


class CateGoryDto {
public:
	CateGoryDto() {}
	~CateGoryDto() {};

public:
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
	std::string id_;
	std::string name_;
	std::string full_id_;
	std::string dir_;
	std::string root_dir_;
	std::string fullname_;
};


class RetImageDto {
public:
	RetImageDto() {}
	~RetImageDto() {};

public:
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
	std::string image_id_;
	std::string ori_;
	std::string src_;
	std::string name_;
	std::string fullname_;
	std::string ext_;
	std::string duration_;
	std::string category_;
};


