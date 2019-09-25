#include "../clib/cfg/cmdline.h"

void example_cmdline_option(int argc, char* argv[]) {
	
	/*
	* Run test with commmand line options
	* <program> -V3 --db 'mysql://root:123@localost:3300/db-name/table-name?fff=134&ggg=1edd' --name test
	*/

	/*
	* Parse command line and assign arg values
	* see, c::cmdline::arg ([c::option&] value, [const std::string&] long_option_name, [char] short_option_name, [int] required_flag)
	*	value - refference to store arg value
	*	long_option_name - unique option name (always required) ( cmd line usage --verbose )
	*	short_option_name - short name  ( cmd line usage -v )
	*	required_flag - type of argument
	*		0 -	no_argument (default)
	*		1 -	required_argument (throw exception if value not set)
	*		2 -	optional_argument
	*/
	c::option o_verbose("0"), o_db, o_string;


	try {
		c::cmdline::options(argc, argv, {
			{o_verbose,"verbose",'V',2},
			{o_db,"db",0,1},
			{o_string,"name",'n',1},
		});

		auto&& db = o_db.dsn();

		printf("%10s: %ld\n", "Verbose", o_verbose.number());
		printf("%10s: %s\n", "Name", o_string.get().c_str());
		printf("%10s: %s\n", "Dsn", db.dsn.c_str());
		printf("\t%8s: %s\n", "proto", db.proto.c_str());
		printf("\t%8s: %s\n", "user", db.user.c_str());
		printf("\t%8s: %s\n", "password", db.pwd.c_str());
		printf("\t%8s: %s\n", "host", db.host.c_str());
		printf("\t%8s: %s\n", "port", db.port.c_str());
		printf("\t%8s: %s\n", "path", db.path.c_str());
		printf("\t%8s: %s\n", "filename", db.filename.c_str());
		for (auto&& [o, v] : db.options) {
			printf("\t%8s(%8s) %s\n","option", o.c_str(), v.c_str());
		}
	}
	catch (std::string required_option_name) {
		printf("c::cmdline::options::exception `%s` are required\n", required_option_name.c_str());
	}
}