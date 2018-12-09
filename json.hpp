#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

class Json
{
public:
	enum Type { JSON_NULL, JSON_BOOL, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT };

public:
	Json(): type_(JSON_NULL) { }

	Json(bool v)              : type_(JSON_BOOL),   text_(v ? TEXT_TRUE : "") { }
	Json(int v)               : type_(JSON_NUMBER), text_(std::to_string(v)) { }
	Json(long v)              : type_(JSON_NUMBER), text_(std::to_string(v)) { }
	Json(long long v)         : type_(JSON_NUMBER), text_(std::to_string(v)) { }
	Json(unsigned v)          : type_(JSON_NUMBER), text_(std::to_string(v)) { }
	Json(unsigned long v)     : type_(JSON_NUMBER), text_(std::to_string(v)) { }
	Json(unsigned long long v): type_(JSON_NUMBER), text_(std::to_string(v)) { }
	Json(float v)             : type_(JSON_NUMBER), text_(std::to_string(v)) { }
	Json(double v)            : type_(JSON_NUMBER), text_(std::to_string(v)) { }
	Json(long double v)       : type_(JSON_NUMBER), text_(std::to_string(v)) { }
	Json(const std::string& v): type_(JSON_STRING), text_(v) { }
	Json(const char* v)       : type_(JSON_STRING), text_(v) { }

public:
	bool AsBool() const { return !text_.empty(); }
	std::string AsString() const { return text_; }
	template <typename T> T AsNumber() const { T v; std::istringstream ss(text_); ss >> v; return v; }

public:
	template <typename T> Json& operator = (T v) { Set(v); return *this; }

	void Set(bool v)               { Set(JSON_BOOL,   (v ? TEXT_TRUE : "")); }
	void Set(int v)                { Set(JSON_NUMBER, std::to_string(v)); }
	void Set(long v)               { Set(JSON_NUMBER, std::to_string(v)); }
	void Set(long long v)          { Set(JSON_NUMBER, std::to_string(v)); }
	void Set(unsigned v)           { Set(JSON_NUMBER, std::to_string(v)); }
	void Set(unsigned long v)      { Set(JSON_NUMBER, std::to_string(v)); }
	void Set(unsigned long long v) { Set(JSON_NUMBER, std::to_string(v)); }
	void Set(float v)              { Set(JSON_NUMBER, std::to_string(v)); }
	void Set(double v)             { Set(JSON_NUMBER, std::to_string(v)); }
	void Set(long double v)        { Set(JSON_NUMBER, std::to_string(v)); }
	void Set(const std::string& v) { Set(JSON_STRING, v); }
	void Set(const char* v)        { Set(JSON_STRING, v); }

	void Set(Type type = JSON_NULL, const std::string s = "") {
		type_ = type;
		text_ = s;
		array_.clear();
		object_.clear();
	}

public:
	Json& Append(const Json& x) {
		if (this == &x) {
			Json copy(x);
			return Append(copy);
		}
		if (type_ != JSON_ARRAY) { Set(JSON_ARRAY); }
		array_.push_back(x);
		type_ = JSON_ARRAY;
		return *this;
	}
	Json& Append(const std::string& name, const Json& x) {
		if (this == &x) {
			Json copy(x);
			return Append(name, copy);
		}
		if (type_ != JSON_OBJECT) { Set(JSON_OBJECT); }
		if (object_.find(name) == object_.end()) { array_.push_back(Json(name)); }
		object_[name] = x;
		return *this;
	}
public:
	Json& operator [] (size_t index) {
		if (type_ != JSON_ARRAY) { Set(JSON_ARRAY); }
		if (index >= array_.size()) { array_.resize(index + 1); }
		return array_[index];
	}
	const Json& operator [] (size_t index) const {
		if (type_ == JSON_ARRAY && index < array_.size()) {
			return array_[index];
		}
		return nullJson_;
	}

	Json& operator [] (const std::string& name) {
		if (type_ != JSON_OBJECT) { Set(JSON_OBJECT); }
		auto it = object_.find(name);
		if (it == object_.end()) {
			array_.push_back(name);
			object_.insert(std::make_pair(name, Json()));
			it = object_.find(name);
		}
		return it->second;
	}
	const Json& operator [] (const std::string& name) const {
		if (type_ == JSON_OBJECT) {
			auto it = object_.find(name);
			if (it != object_.end()) {
				return it->second;
			}
		}
		return nullJson_;
	}

public:
	Json& operator += (const Json& x) { return Append(x); }
	Json& operator += (const std::pair<std::string, Json>& p) { return Append(p.first, p.second); }

public:
	std::string ToString() const {
		std::string s;
		switch (type_) {
		default: assert(false); // should never reach here
		case JSON_NULL:
			s = TEXT_NULL;
			break;
		case JSON_BOOL:
			s = (text_.empty() ? TEXT_FALSE : TEXT_TRUE);
			break;
		case JSON_NUMBER:
			s = (text_.empty() ? TEXT_ZERO : text_);
			break;
		case JSON_STRING:
			s = "\"" + text_ + "\"";
			break;
		case JSON_ARRAY:
			s = "[";
			for (size_t i = 0; i < array_.size(); ++i) {
				if (i > 0) { s += ","; }
				s += array_[i].ToString();
			}
			s += "]";
			break;
		case JSON_OBJECT:
			s = "{";
			assert(array_.size() == object_.size());
			for (size_t i = 0; i < array_.size(); ++i) {
				if (i > 0) { s += ","; }
				const auto name = array_[i].text_;
				auto it = object_.find(name);
				assert(it != object_.end());
				const auto e = it->second;
				s += "\"" + name + "\":" + e.ToString();
			}
			s += "}";
			break;
		}
		return s;
	}

private:
	Type type_ = JSON_NULL;

	std::string text_;
	std::vector<Json> array_;
	std::map<std::string, Json> object_;

	static const Json nullJson_;

	static constexpr const char* TEXT_NULL = "null";
	static constexpr const char* TEXT_TRUE = "true";
	static constexpr const char* TEXT_FALSE = "false";
	static constexpr const char* TEXT_ZERO = "0";

	friend inline std::ostream& operator << (std::ostream& os, const Json& j) { return (os << j.ToString()); }
};
