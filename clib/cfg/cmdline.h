#pragma once
#include "../ci/option.h"
#include <getopt.h>
#include <vector>

namespace c {
	class cmdline {
	public:
		class arg {
			friend class cmdline;
			std::string long_name;
			char		short_name;
			int			require_arg;
			c::option& value_arg;
			operator struct ::option() const { return { long_name.c_str(),require_arg ,nullptr,short_name }; }
			operator c::option& () const { return value_arg; }
		public:
			arg(c::option& value, const std::string& lname, const char sname = '\0', int require = 0)
				: long_name(lname), short_name(sname), require_arg(require), value_arg(value) {
				;
			}
		};
	public:
		static void options(int argc, char* argv[], std::vector<c::cmdline::arg>&& args, std::string& progname, std::string& pwd, std::string& cwd);
		static inline void options(int argc, char* argv[], std::vector<c::cmdline::arg>&& args, std::string& pwd, std::string& cwd) {
			std::string progname;
			options(argc, argv, std::move(args), pwd, cwd, progname);
		}
		static inline void options(int argc, char* argv[], std::vector<c::cmdline::arg>&& args, std::string& pwd) {
			std::string progname, cwd;
			options(argc, argv, std::move(args), pwd, cwd, progname);
		}
		
		static inline void options(int argc, char* argv[], std::vector<c::cmdline::arg>&& args) {
			std::string progname, cwd, pwd;
			options(argc, argv, std::move(args), pwd, cwd, progname);
		}
	};
}