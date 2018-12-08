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
public:
	Json(): type_(JSON_NULL), s_("null") { }

	Json(bool v): type_(JSON_BOOL), s_(v ? "true" : "false") { }
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
		assert(a_.size() == m_.size());
		if (this == &x) {
			Json copy(x);
			return Append(copy);
		}
		if (type_ == JSON_BOOL || type_ == JSON_NUMBER || type_ == JSON_STRING) {
			auto name = GenerateNewName();
			Json copy(*this);
			a_.push_back(name);
			m_[name] = copy;
		}
		auto name = GenerateNewName();
		a_.push_back(name);
		m_[name] = x;
		type_ = JSON_ARRAY;
		return *this;
	}
	Json& Append(const std::string& name, const Json& x) {
		assert(a_.size() == m_.size());
		if (this == &x) {
			auto n = name;
			Json copy(x);
			return Append(n, x);
		}
		if (type_ == JSON_BOOL || type_ == JSON_NUMBER || type_ == JSON_STRING) {
			auto name = GenerateNewName();
			Json copy(*this);
			a_.push_back(name);
			m_[name] = copy;
		}
		a_.push_back(name);
		m_[name] = x;
		type_ = JSON_OBJECT;
		return *this;
	}
public:
	Json& operator [] (size_t index) {
		if (type_ != JSON_ARRAY) { Clear(); }
		if (index < a_.size()) { a_.resize(index + 1); }
		return m_[a_[index]];
	}
	const Json& operator [] (size_t index) const {
		if (type_ == JSON_ARRAY && index < a_.size()) {
			auto it = m_.find(a_[index]);
			assert(it != m_.end());
			return it->second;
		}
		return nullJson_;
	}

	Json& operator [] (const std::string& name) {
		if (type_ != JSON_OBJECT) { Clear(); }
		auto it = m_.find(name);
		if (it == m_.end()) {
			m_.insert(std::make_pair(name, Json()));
			it = m_.find(name);
		}
		return it->second;
	}
	const Json& operator [] (const std::string& name) const {
		if (type_ == JSON_ARRAY) {
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
		case JSON_BOOL:
		case JSON_NUMBER:
			s = s_;
			break;
		case JSON_STRING:
			s = "\"" + s_ + "\"";
			break;
		case JSON_ARRAY:
		case JSON_OBJECT:
			s = (type_ == JSON_ARRAY ? "[" : "{");
			for (size_t i = 0; i < a_.size(); ++i) {
				if (i > 0) { s += ","; }
				const auto key = a_[i];
				if (type_ == JSON_OBJECT) {
					s += "\"" + key + "\":";
				}
				const auto it = m_.find(key);
				assert(it != m_.end());
				s += it->second.ToString();
			}
			s += (type_ == JSON_ARRAY ? "]" : "}");
			break;
		}
		return s;
	}
private:
	std::string GenerateNewName() const {
		std::string name = std::to_string(a_.size() + 1);
		if (m_.find(name) != m_.end()) {
			for (size_t i = 1; ; ++i) {
				std::string key = name + "." + std::to_string(i);
				if (m_.find(key) == m_.end()) {
					name = key;
					break;
				}
			}
		}
		return name;
	}

private:
	Type type_ = JSON_NULL;

	std::string s_;
	std::vector<std::string> a_;
	std::map<std::string, Json> m_;

	const static Json nullJson_;
};

inline std::ostream& operator << (std::ostream& os, const Json& j) { return (os << j.ToString()); }
