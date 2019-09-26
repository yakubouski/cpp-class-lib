#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <list>
#include "../ci/option.h"

namespace c {
	class ini {

		template<typename T>
		class range_iterator {
			using iterator = typename T::const_iterator;
			iterator from, to;
		public:
			range_iterator(const T&& o) : from(o.begin()), to(o.end()) { ; }
			range_iterator(const iterator&& b, const iterator&& e) : from(move(b)), to(move(e)) { ; }
			range_iterator(const range_iterator&& rit) : from(rit.from), to(rit.to) { ; }
			inline iterator begin() { return from; }
			inline iterator end() { return to; }
		};
		class csection {
			friend class ini;
			using container = std::unordered_map<std::string, std::shared_ptr<csection>>;
			using properties = std::unordered_map<std::string, std::string>;
			using options = std::unordered_map<std::string, c::option>;
			container	_sections;
			properties	_properties;
			csection() { ; }
			csection(const csection&& s) : _sections(move(s._sections)), _properties(move(s._properties)) { ; }
		public:
			virtual ~csection() { ; }
			inline std::shared_ptr<csection> section(const std::string&& name) { 
				auto&& it = _sections.find(std::move(name));
				return it != _sections.end() ? it->second : std::shared_ptr<csection>{ nullptr };
			}
			inline range_iterator<container> sections() const { return range_iterator<csection::container>(std::move(_sections)); }
			inline std::string property(const std::string&& name, const std::string&& default_if_empty = {}) {
				auto&& it = _properties.find(std::move(name));
				return it != _properties.end() ? _properties.find(std::move(name))->second : default_if_empty;
			}
			inline range_iterator<properties> props() const { return range_iterator<properties>(std::move(_properties)); }

			inline c::option option(const std::string&& name, const std::string&& default_if_empty = {}) {
				auto&& it = _properties.find(std::move(name));
				c::option value(default_if_empty);
				if (it != _properties.end()) {
					value.set(it->second);
				}
				return value;
			}
		};
		std::unique_ptr<csection>	head;
		inline csection* get_section(std::list<std::string>&);
	public:
		ini();
		ini(const std::string configfile);
		ini& operator = (ini&& cfg);
		inline std::shared_ptr<csection> section(const std::string&& name) const {
			if (!head) throw std::runtime_error("Configuration is empty");
			auto&& it = head->_sections.find(std::move(name));
			return it != head->_sections.end() ? it->second : std::shared_ptr<csection>{nullptr};
		}
		inline range_iterator<csection::container> sections() const {
			if (!head) throw std::runtime_error("Configuration is empty");
			return range_iterator<csection::container>(head->_sections.begin(), head->_sections.end());
		}
	};
}