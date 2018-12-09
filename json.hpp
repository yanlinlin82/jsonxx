#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

class Json
{
private:
	enum Type { JSON_NULL, JSON_BOOL, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT };

	static constexpr const char* TEXT_NULL = "null";
	static constexpr const char* TEXT_TRUE = "true";
	static constexpr const char* TEXT_FALSE = "false";
	static constexpr const char* TEXT_ZERO = "0";

public:
	Json(): type_(JSON_NULL) { }

	Json(bool v): type_(JSON_BOOL), s_(v ? TEXT_TRUE : "") { }
	Json(int v): type_(JSON_NUMBER), s_(std::to_string(v)) { }
	Json(long v): type_(JSON_NUMBER), s_(std::to_string(v)) { }
	Json(long long v): type_(JSON_NUMBER), s_(std::to_string(v)) { }
	Json(unsigned v): type_(JSON_NUMBER), s_(std::to_string(v)) { }
	Json(unsigned long v): type_(JSON_NUMBER), s_(std::to_string(v)) { }
	Json(unsigned long long v): type_(JSON_NUMBER), s_(std::to_string(v)) { }
	Json(float v): type_(JSON_NUMBER), s_(std::to_string(v)) { }
	Json(double v): type_(JSON_NUMBER), s_(std::to_string(v)) { }
	Json(long double v): type_(JSON_NUMBER), s_(std::to_string(v)) { }
	Json(const std::string& v): type_(JSON_STRING), s_(v) { }
	Json(const char* v): type_(JSON_STRING), s_(v) { }

	template <typename T> Json& operator = (T v) { *this = Json(v); return *this; }

	void Clear() { type_ = JSON_NULL; s_.clear(); a_.clear(); m_.clear(); }

	Json& Append(const Json& x) {
		if (this == &x) {
			Json copy(x);
			return Append(copy);
		}
		if (type_ != JSON_ARRAY) { Clear(); type_ = JSON_ARRAY; }
		a_.push_back(x);
		type_ = JSON_ARRAY;
		return *this;
	}
	Json& Append(const std::string& name, const Json& x) {
		if (this == &x) {
			auto n = name;
			Json copy(x);
			return Append(n, x);
		}
		if (type_ != JSON_OBJECT) { Clear(); type_ = JSON_OBJECT; }
		if (m_.find(name) == m_.end()) { a_.push_back(Json(name)); }
		m_[name] = x;
		return *this;
	}
public:
	Json& operator [] (size_t index) {
		if (type_ != JSON_ARRAY) { Clear(); type_ = JSON_ARRAY; }
		if (index < a_.size()) { a_.resize(index + 1); }
		return a_[index];
	}
	const Json& operator [] (size_t index) const {
		if (type_ == JSON_ARRAY && index < a_.size()) {
			return a_[index];
		}
		return nullJson_;
	}

	Json& operator [] (const std::string& name) {
		if (type_ != JSON_OBJECT) { Clear(); type_ = JSON_OBJECT; }
		auto it = m_.find(name);
		if (it == m_.end()) {
			m_.insert(std::make_pair(name, Json()));
			it = m_.find(name);
		}
		return it->second;
	}
	const Json& operator [] (const std::string& name) const {
		if (type_ == JSON_OBJECT) {
			auto it = m_.find(name);
			if (it != m_.end()) {
				return it->second;
			}
		}
		return nullJson_;
	}

	std::string ToString() const {
		std::string s;
		switch (type_) {
		default: assert(false); // should never reach here
		case JSON_NULL:
			s = TEXT_NULL;
			break;
		case JSON_BOOL:
			s = (s_.empty() ? TEXT_FALSE : TEXT_TRUE);
			break;
		case JSON_NUMBER:
			s = (s_.empty() ? TEXT_ZERO : s_);
			break;
		case JSON_STRING:
			s = "\"" + s_ + "\"";
			break;
		case JSON_ARRAY:
			s = "[";
			for (size_t i = 0; i < a_.size(); ++i) {
				if (i > 0) { s += ","; }
				s += a_[i].ToString();
			}
			s += "]";
			break;
		case JSON_OBJECT:
			s = "{";
			assert(a_.size() == m_.size());
			for (size_t i = 0; i < a_.size(); ++i) {
				if (i > 0) { s += ","; }
				const auto name = a_[i].s_;
				auto it = m_.find(name);
				assert(it != m_.end());
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

	std::string s_;
	std::vector<Json> a_;
	std::map<std::string, Json> m_;

	static const Json nullJson_;
};

inline std::ostream& operator << (std::ostream& os, const Json& j) { return (os << j.ToString()); }
