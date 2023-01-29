#pragma once
#if __cplusplus < 201703L
#error C++17 is required!
#endif

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <cstring>

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
		type get_type() const { return type_; }

		bool as_boolean() const {
			assert(type_ == type::boolean);
			return !text_.empty();
		}

		template <typename T = int>
		T as_number() const {
			assert(type_ == type::number);
			std::istringstream ss(text_);
			T v;
			ss >> v;
			return v;
		}

		std::string as_string() const {
			assert(type_ == type::string);
			return text_;
		}

		std::vector<json> as_array() const {
			assert(type_ == type::array);
			return array_;
		}

		std::vector<std::pair<std::string, json>> as_object() const {
			assert(type_ == type::object);
			decltype(as_object()) ret;
			for (auto c : array_) {
				auto it = object_.find(c.text_);
				assert(it != object_.end());
				ret.push_back(*it);
			}
			return ret;
		}

	public:
		void clear(type t = type::null, const std::string s = "") {
			type_ = t;
			text_ = s;
			array_.clear();
			object_.clear();
		}

		json& operator = (json j) & { swap(j); return *this; }

		template <typename T>
		void assign(T v) { json j(v); swap(j); }

		void swap(json& j) {
			using std::swap;
			swap(type_, j.type_);
			swap(text_, j.text_);
			swap(array_, j.array_);
			swap(object_, j.object_);
		}

	public:
		json& append(const json& x) {
			if (this == &x) {
				json copy(x);
				return append(copy);
			}
			if (x.type_ == type::null) return *this;
			if (type_ == type::null) {
				assign(x);
			} else if (type_ == type::array) {
				if (x.type_ == type::array) {
					array_.insert(array_.end(), x.array_.cbegin(), x.array_.cend());
				} else {
					array_.push_back(x);
				}
			} else if (type_ == type::object && x.type_ == type::object) {
				for (auto c : x.object_) {
					auto it = object_.find(c.first);
					if (it == object_.end()) {
						array_.push_back(json(c.first));
					}
					object_.insert(c);
				}
			} else {
				json ret;
				ret.type_ = type::array;
				ret.array_.push_back(*this);
				ret.array_.push_back(x);
				assign(ret);
			}
			return *this;
		}

		json& append(const std::string& name, const json& x) {
			if (this == &x) {
				json copy(x);
				return append(name, copy);
			}
			if (type_ != type::object) { clear(type::object); }
			if (object_.find(name) == object_.end()) { array_.push_back(json(name)); }
			object_[name] = x;
			return *this;
		}

	public:
		json& operator [] (size_t index) {
			if (type_ != type::array) { clear(type::array); }
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
			if (type_ != type::object) { clear(type::object); }
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
				s = "\"" + encode(text_) + "\"";
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
					s += "\"" + encode(name) + "\":" + e.to_string();
				}
				s += "}";
				break;
			}
			return s;
		}

	private:
		static const json null_;

		static std::string trim0(std::string s) {
			if (s.find('.') != std::string::npos) {
				s.erase(s.find_last_not_of('0') + 1, std::string::npos);
				s.erase(s.find_last_not_of('.') + 1, std::string::npos);
			}
			return s;
		}
		static std::string encode(std::string s) {
			std::stringstream ss;
			for (auto c : s) {
				switch (c) {
				case '/': ss << "\\/"; break;
				case '"': ss << "\\\""; break;
				case '\\': ss << "\\\\"; break;
				case '\b': ss << "\\b"; break;
				case '\f': ss << "\\f"; break;
				case '\n': ss << "\\n"; break;
				case '\r': ss << "\\r"; break;
				case '\t': ss << "\\t"; break;
				default:
					unsigned int u = static_cast<unsigned char>(c);
					if (u >= 0x7F) {
						ss << "\\u00"
							<< std::hex << std::uppercase
							<< static_cast<unsigned int>(u >> 4)
							<< static_cast<unsigned int>(u & 0x0F)
							<< std::dec << std::nouppercase;
					} else {
						ss << c;
					}
					break;
				}
			}
			return ss.str();
		}
		static void skip_spaces(const char*& s) {
			while (*s) {
				if (isspace(*s)) {
					++s;
				} else if (*s == '/' && *(s + 1) == '*') { // Skip comments
					s += 2;
					while (*s) {
						if (*s == '*' && *(s + 1) == '/') {
							s += 2;
							break;
						}
						++s;
					}
				} else {
					break;
				}
			}
		}

		static unsigned int hex_value(int c) {
			if (c >= '0' && c <= '9') {
				return (c - '0');
			} else if (c >= 'A' && c <= 'F') {
				return (c - 'A') + 10;
			} else if (c >= 'a' && c <= 'f') {
				return (c - 'a') + 10;
			} else {
				return c;
			}
		}

		static bool parse_string(const char*& s, json& v, bool strict = true) {
			skip_spaces(s);
			if (strict) {
				if (*s != '"') return false; else ++s;
			}
			std::string text;
			while (*s) {
				if (strict) {
					if (*s == '"') break;
				} else {
					if (*s == ':' || *s == ',' || *s == '}' || *s == ']' || isspace(*s)) break;
				}
				if (*s == '\\') {
					switch (*(++s)) {
						case '"':
						case '\\':
						case '/': text += *s++; break;
						case 'b': text += '\b'; ++s; break;
						case 'f': text += '\f'; ++s; break;
						case 'n': text += '\n'; ++s; break;
						case 'r': text += '\r'; ++s; break;
						case 't': text += '\t'; ++s; break;
						case 'u':
								  if (!isxdigit(s[1])) return false;
								  if (!isxdigit(s[2])) return false;
								  if (!isxdigit(s[3])) return false;
								  if (!isxdigit(s[4])) return false;
								  text.push_back(static_cast<char>(
											  (hex_value(s[1]) << 12) +
											  (hex_value(s[2]) << 8) +
											  (hex_value(s[3]) << 4) +
											  hex_value(s[4])));
								  s += 5;
								  break;
						default: return false;
					}
				} else {
					text += *s++;
				}
			}
			if (strict) {
				if (*s != '"') return false; else ++s;
			}
			v = text;
			return true;
		}

		static bool parse_number(const char*& s, json& v) {
			const char* s0 = s;
			skip_spaces(s);
			if (*s == '-' || *s == '+') ++s;
			if (!isdigit(*s) && *s != '.') return false; else ++s;
			while (isdigit(*s)) ++s;
			if (*s == '.') {
				++s;
				while (isdigit(*s)) ++s;
			}
			if (*s == 'e' || *s == 'E') {
				++s;
				if (*s == '+' || *s == '-') ++s;
				if (!isdigit(*s)) return false; else ++s;
				while (isdigit(*s)) ++s;
			}
			v.clear(type::number, std::string(s0, s));
			return true;
		}

		static bool parse_object(const char*& s, json& v, bool strict) {
			skip_spaces(s);
			if (*s != '{') return false; else ++s;
			skip_spaces(s);
			v.clear(type::object);
			if (*s != '}') {
				for (;;) {
					if (!strict) {
						skip_spaces(s);
						if (*s == ',') {
							++s;
							continue;
						} else if (*s == '}') {
							break;
						}
					}
					std::string name;
					json o;
					if (*s != '"' && !strict) {
						if (!parse_string(s, o, false)) return false;
					} else {
						if (!parse_string(s, o)) return false;
					}
					name = o.as_string();
					skip_spaces(s);
					if (*s != ':') return false; else ++s;
					if (!parse_value(s, o, strict)) return false;
					skip_spaces(s);
					v[name] = o;
					if (*s == '}') break;
					if (*s == ',') {
						++s;
					} else {
						if (strict) return false;
					}
				}
			}
			++s;
			return true;
		}

		static bool parse_array(const char*& s, json& v, bool strict) {
			skip_spaces(s);
			if (*s != '[') return false; else ++s;
			skip_spaces(s);
			v.clear(type::array);
			if (*s != ']') {
				for (;;) {
					if (!strict) {
						skip_spaces(s);
						if (*s == ',') {
							++s;
							continue;
						} else if (*s == ']') {
							break;
						}
					}
					json o;
					if (!parse_value(s, o, strict)) return false;
					skip_spaces(s);
					v.append(o);
					if (*s == ']') break;
					if (*s == '}') return false;
					if (*s == ',') {
						++s;
					} else {
						if (strict) return false;
					}
				}
			}
			++s;
			return true;
		}

		static inline bool is_valid_separator(char c) {
			return (c == '\0' || strchr(",]} \t\n\r", c) != NULL);
		}

		static inline bool match_symbol(const char*& s, const std::string& t) {
			if (memcmp(s, t.c_str(), t.size()) == 0 && is_valid_separator(s[t.size()])) {
				s += t.size();
				return true;
			}
			return false;
		}

		static bool parse_value(const char*& s, json& v, bool strict) {
			skip_spaces(s);
			if (*s == '"') {
				return parse_string(s, v);
			}
			if (*s == '+' || *s == '-' || *s == '.' || isdigit(*s)) {
				const char* p = s;
				if (parse_number(p, v)) {
					if (strict) {
						s = p;
						return true;
					}
					if (is_valid_separator(*p)) {
						s = p;
						return true;
					}
				} else if (strict) {
					return false;
				}
				// otherwise, try to parse as string
			}
			if (*s == '{') {
				return parse_object(s, v, strict);
			} else if (*s == '[') {
				return parse_array(s, v, strict);
			} else if (match_symbol(s, text_null)) {
				v.clear(type::null);
				return true;
			} else if (match_symbol(s, text_true)) {
				v = true;
				return true;
			} else if (match_symbol(s, text_false)) {
				v = false;
				return true;
			} else if (!strict) {
				return parse_string(s, v, false);
			} else {
				return false;
			}
		}

	public:
		bool parse(const std::string& text, size_t* pos = nullptr, bool strict = true) {
			const char* p = text.c_str();
			if (!parse_value(p, *this, strict)) {
				if (pos) *pos = p - text.c_str();
				return false;
			}
			skip_spaces(p);
			if (pos) *pos = p - text.c_str();
			return (!*p);
		}

		bool load(const std::string& filename, bool strict) {
			std::ifstream file(filename, std::ios::in);
			if (!file.is_open()) {
				std::cerr << "cannot open input json file '" << filename << "'" << std::endl;
				return false;
			}
			std::string text;
			std::string line;
			while (std::getline(file, line)) {
				text += line;
			}
			size_t pos = 0;
			if (!parse(text, &pos, strict)) {
				std::cerr << "invalid json format in file '" << filename << "'" << std::endl;
				return false;
			}
			if (pos != text.size()) {
				std::cerr << "unexpected character after json in file '" << filename << "'" << std::endl;
				return false;
			}
			return true;
		}
	};

	inline std::ostream& operator << (std::ostream& os, const json& j) {
		return (os << j.to_string());
	}
}
