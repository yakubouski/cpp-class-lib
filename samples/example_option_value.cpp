#include "clib/ci/option.h"


void example_cmdline_value(int, char*[]) {

	printf("number: %lu\n\n", c::option(" 45   ").number());
	printf("decimal( ): %f\n", c::option("  0.8761234 ").decimal());
	printf("decimal(0): %f\n", c::option("  0.8761234 ").decimal(0));
	printf("decimal(2): %f\n\n", c::option("  0.8761234 ").decimal(2));
	printf("bytes(12M): %lu\n", c::option("12M").bytes());
	printf("bytes(89K): %lu\n", c::option("89K").bytes());
	printf("bytes(123): %lu\n\n", c::option("123").bytes());
	printf("seconds(5h12s): %lu\n", c::option("5h12s").seconds());
	printf("seconds(5m): %lu\n", c::option("5 m").seconds());
	printf("seconds(2m3): %lu\n", c::option("2m3").seconds());
	printf("seconds(123): %lu\n\n", c::option("123").seconds());
		
	printf("sequence(1-6, 8, 17-15): "); 
	{
		for (auto&& el : c::option("1-6, 8, 17-15").sequence()) {
			printf("%s, ", el.c_str());
		}
		printf("\n");
	}
	printf("sequence(eth0-eth4,eth11,eth2): ");
	{
		for (auto&& el : c::option("eth0-eth4,eth11,eth2").sequence()) {
			printf("%s, ", el.c_str());
		}
		printf("\n\n");
	}
	printf("split(read|write|lock|read|unread|write): ");
	{
		for (auto&& el : c::option("read|write|lock|read|unread|write").split("|")) {
			printf("%s, ", el.c_str());
		}
		printf("\n\n");
	}

	printf("dsn(mysql://user:pwd@localhost:9000/path/to/table?opt1=aaa&opt2=bbb): \n");
	{
		auto con = c::option("mysql://user:pwd@localhost:9000/path/to/table?opt1=aaa&opt2=bbb").dsn();

		printf("\t%8s: %s\n", "proto", con.proto.c_str());
		printf("\t%8s: %s\n", "user", con.user.c_str());
		printf("\t%8s: %s\n", "password", con.pwd.c_str());
		printf("\t%8s: %s\n", "host", con.host.c_str());
		printf("\t%8s: %s\n", "port", con.port.c_str());
		printf("\t%8s: %s\n", "path", con.path.c_str());
		printf("\t%8s: %s\n", "filename", con.filename.c_str());
		for (auto&& [o, v] : con.options) {
			printf("\t%8s(%8s) %s\n", "option", o.c_str(), v.c_str());
		}
		printf("\n");
	}

	printf("dsn(/path/to/fileame.txt): \n");
	{
		auto con = c::option("/path/to/fileame.txt").dsn();

		printf("\t%8s: %s\n", "proto", con.proto.c_str());
		printf("\t%8s: %s\n", "user", con.user.c_str());
		printf("\t%8s: %s\n", "password", con.pwd.c_str());
		printf("\t%8s: %s\n", "host", con.host.c_str());
		printf("\t%8s: %s\n", "port", con.port.c_str());
		printf("\t%8s: %s\n", "path", con.path.c_str());
		printf("\t%8s: %s\n", "filename", con.filename.c_str());
		for (auto&& [o, v] : con.options) {
			printf("\t%8s(%8s) %s\n", "option", o.c_str(), v.c_str());
		}
		printf("\n");
	}
}