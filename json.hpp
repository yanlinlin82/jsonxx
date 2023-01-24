#pragma once
#if __cplusplus < 201703L
#error C++17 is required!
#endif

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

namespace yll
{
	class json
	{
	public:
		enum class type { null, boolean, number, string, array, object };
	public:
		static inline const std::string text_null = "null";
		static inline const std::string text_true = "true";
		static inline const std::string text_false = "false";
	private:
		type type_ = type::null;
		std::string text_;
		std::vector<json> array_;
		std::map<std::string, json> object_;
	public:
		// null json
		json() = default;
		// boolean json
		json(bool v): type_(type::boolean), text_(v ? text_true : "") { }
		// number json
		json(int v): type_(type::number), text_(std::to_string(v)) { }
		json(long v): type_(type::number), text_(std::to_string(v)) { }
		json(long long v): type_(type::number), text_(std::to_string(v)) { }
		json(unsigned v): type_(type::number), text_(std::to_string(v)) { }
		json(unsigned long v): type_(type::number), text_(std::to_string(v)) { }
		json(unsigned long long v): type_(type::number), text_(std::to_string(v)) { }
		json(float v): type_(type::number), text_(trim0(std::to_string(v))) { }
		json(double v): type_(type::number), text_(trim0(std::to_string(v))) { }
		json(long double v): type_(type::number), text_(std::to_string(v)) { }
		// string json
		json(const char* s): type_(type::string), text_(s) { }
		json(const std::string& s): type_(type::string), text_(s) { }
		// array json
		template <typename T>
		json(const std::initializer_list<T>& s):
			type_(type::array), array_(s.begin(), s.end()) { }
		// object json
		json(std::string name, const json& v):
			type_(type::object), array_{name}, object_{std::make_pair(name, v)} { }
	public:
		bool to_boolean() const { return !text_.empty(); }
		template <typename T = int>
		T to_number() const { T v; std::istringstream ss(text_); ss >> v; return v; }

	public:
		template <typename T> json&
		operator = (T v) { return operator = (json(v)); }

		template <typename T>
		void set(T v) { operator = (json(v)); }
	private:
		void set_(type t, const std::string s = "") {
			type_ = t;
			text_ = s;
			array_.clear();
			object_.clear();
		}

	public:
		json& append(const json& x) {
			if (this == &x) {
				json copy(x);
				return append(copy);
			}
			if (type_ != type::array) { set_(type::array); }
			array_.push_back(x);
			type_ = type::array;
			return *this;
		}
		json& append(const std::string& name, const json& x) {
			if (this == &x) {
				json copy(x);
				return append(name, copy);
			}
			if (type_ != type::object) { set_(type::object); }
			if (object_.find(name) == object_.end()) { array_.push_back(json(name)); }
			object_[name] = x;
			return *this;
		}
	public:
		json& operator [] (size_t index) {
			if (type_ != type::array) { set_(type::array); }
			if (index >= array_.size()) { array_.resize(index + 1); }
			return array_[index];
		}
		const json& operator [] (size_t index) const {
			if (type_ == type::array && index < array_.size()) {
				return array_[index];
			}
			return null_;
		}

		json& operator [] (const std::string& name) {
			if (type_ != type::object) { set_(type::object); }
			auto it = object_.find(name);
			if (it == object_.end()) {
				array_.push_back(name);
				object_.insert(std::make_pair(name, json()));
				it = object_.find(name);
			}
			return it->second;
		}
		const json& operator [] (const std::string& name) const {
			if (type_ == type::object) {
				auto it = object_.find(name);
				if (it != object_.end()) {
					return it->second;
				}
			}
			return null_;
		}

	public:
		json& operator += (const json& x) { return append(x); }

	public:
		std::string to_string() const {
			std::string s;
			switch (type_) {
			default: assert(false); // should never reach here
			case type::null:
				s = text_null;
				break;
			case type::boolean:
				s = (text_.empty() ? text_false : text_true);
				break;
			case type::number:
				s = text_;
				break;
			case type::string:
				s = "\"" + text_ + "\"";
				break;
			case type::array:
				s = "[";
				for (size_t i = 0; i < array_.size(); ++i) {
					if (i > 0) { s += ","; }
					s += array_[i].to_string();
				}
				s += "]";
				break;
			case type::object:
				s = "{";
				assert(array_.size() == object_.size());
				for (size_t i = 0; i < array_.size(); ++i) {
					if (i > 0) { s += ","; }
					const auto name = array_[i].text_;
					auto it = object_.find(name);
					assert(it != object_.end());
					const auto e = it->second;
					s += "\"" + name + "\":" + e.to_string();
				}
				s += "}";
				break;
			}
			return s;
		}

	public:
		bool parse(std::string_view s) {
			if (s == "null") {
				set_(type::null);
			} else if (s == "false") {
				set(false);
			} else if (s == "true") {
				set(true);
			} else {
				return false;
			}
			return true;
		}

	private:
		static const json null_;

		friend inline std::ostream&
		operator << (std::ostream& os, const json& j) { return (os << j.to_string()); }

		static std::string trim0(std::string s) {
			if (s.find('.') != std::string::npos) {
				s.erase(s.find_last_not_of('0') + 1, std::string::npos);
				s.erase(s.find_last_not_of('.') + 1, std::string::npos);
			}
			return s;
		}
	};
}
