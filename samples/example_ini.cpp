#include "clib/cfg/ini.h"

void example_ini(int argc, char* argv[]) {

	c::ini ini(argv[0]);
	
	printf("Check section/value exists: section(%s) %s\n", "section-1/sub-section-1", ini.section("section-1")->section("sub-section-1")  ? "true" : "false");
	printf("Check section/value exists: section(%s) %s\n", "section-1/unknown-section", ini.section("section-1")->section("unknown-section") ? "true" : "false");
	printf("\n");

	printf("%s = %s\n", "section-1/sub-section-1/value", ini.section("section-1")->section("sub-section-1")->property("value").c_str());
	printf("%s = %s\n", "section-1/sub-section-2/string", ini.section("section-1")->section("sub-section-2")->property("string").c_str());
	printf("%s = %s\n", "section-1/sub-section-2/level-sub-section-2/string", ini.section("section-1")->section("sub-section-2")->section("level-sub-section-2")->property("string").c_str());

	printf("%s = %s\n", "section-2/string", ini.section("section-2")->property("string").c_str());
	printf("%s = %ld sec.\n", "section-2/sub-section-1/delay", ini.section("section-2")->section("sub-section-1")->option("delay").seconds());
	printf("\nEnum example\n");
	for (auto&& [name,ssects] : ini.section("enum-example")->sections()) {
		if (ssects) {
			for (auto&& [pname, pval] : ssects->props()) {
				printf("%s/%s/%s = %s\n", "enum-example", name.c_str(), pname.c_str(),pval.c_str());
			}
		}
		else {
			printf("%s/%s = <empty>\n", "enum-example",name.c_str());
		}
	}
}