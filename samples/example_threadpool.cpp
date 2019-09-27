#include "clib/mt/threadpool.h"

class cthread_worker {
public:
	cthread_worker() { ; }

	int operator()(int nThread) {
		printf("[%2ld] <Begin> Class thread: %lx Sleep (2500) milliseconds\n", nThread, std::this_thread::get_id());
		std::this_thread::sleep_for(std::chrono::milliseconds(2500));
		printf("[%2ld] <End> Classthread: %lx\n", nThread, std::this_thread::get_id());
		return 0;
	}
};

void example_threadpool(int argc, char* argv[]) {

	c::threadpool thds(2,5);

	/* Run job with lambda or function worker */
	thds.enqueue([](int nThread) {
		printf("[%2ld] <Begin> Function thread: %lx Sleep (5)  sec\n", nThread, std::this_thread::get_id());
		std::this_thread::sleep_for(std::chrono::seconds(5));
		printf("[%2ld] <End> Function thread: %lx\n", nThread, std::this_thread::get_id());
	},1);

	/* Run job with class worker, will be instantiate when thread start */
	for (int i = 2; i < 10; i++) {
		thds.enqueue<cthread_worker>(i);
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));

	/* Run job with feature result */

	auto&& value = thds.enqueue([](int nThread) -> int {
		printf("[%2ld] <Begin> Feature thread: %lx Sleep (1)  sec\n", nThread, std::this_thread::get_id());
		std::this_thread::sleep_for(std::chrono::seconds(1));
		printf("[%2ld] <End> Feature thread: %lx\n", nThread, std::this_thread::get_id());

		return nThread * 100;
	}, 10);

	printf("Feature result : %ld\n", value.get());

	thds.join();
}