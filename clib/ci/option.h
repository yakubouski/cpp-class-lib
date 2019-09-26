#pragma once
#include <string>
#include <set>
#include <unordered_map>
#include <deque>

namespace c {
	class option {
		std::string	opt_value;
		bool		opt_empty;
	public:
		option(const std::string& def_value = {}) : opt_value(def_value), opt_empty(true) { ; }

		inline auto& set(const std::string& v) { opt_value = v; opt_empty = false; return *this; }
		inline bool isset() { return !opt_empty; }
		inline void unset() { opt_empty = true; opt_value.erase(); }
		inline const auto& get() { return opt_value; }


		/*
		* Convert time string aka [<H>h [<M>m ]] <S>[s] to seconds
		*/
		size_t seconds();

		/*
		* Convert value to integer value
		*/
		ssize_t number(size_t base = 10);

		/*
		* Convert value to float value
		*/
		double decimal(ssize_t precision = -1);

		/*
		* Convert string to number of byte (<N>[G|M|K|B])
		*/
		size_t bytes();

		/*
		* Expand string to incremented sequense.
		* 1-4,8 expanded to 1,2,3,4,8
		* at9-at3, et3-et5 expanded to at3,at4,at5,at6,at7,at8,at9,et3,et4,et5
		*/
		std::set<std::string> sequence(const std::string& delimiter_chain = ",", const std::string& delimiter_range = "-", bool trim_values = true);

		/*
		* Split string
		*/
		std::deque<std::string> split(const std::string& delimiter, bool trim_values = true);

		/*
		* Expand dsn string aka <proto>://<user>:<pwd>@<host>:<port></path/to/><filename>?<opt1>=<val1>&<opt2>=<val2>
		*/

		using dsn_params = struct {
			std::string dsn,proto,user,pwd,host,port,path,filename;
			std::unordered_map<std::string, std::string> options;
		};
			//std::tuple<std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::unordered_map<std::string, std::string>>;

		dsn_params dsn();
	};
}
