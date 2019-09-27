#include "clib/mm/mempool.h"
#include "clib/ci/chrono.h"
#include <forward_list>

template <typename AL> 
void test_allocator(AL&& allocator, size_t nRounds, size_t nElements) {
	c::chrono timer,g_timer;
	double allocated_tm{ 0.0 }, construct_tm{ 0.0 }, deallocated_tm { 0.0 }, destruct_tm{ 0.0 };

	std::forward_list<typename AL::pointer> items;

	g_timer.start();

	for (auto n = nRounds; n--;) {
		timer.start();
		for (auto i = nElements; i--;) {
			//timer.start();
			auto p = allocator.allocate(1);
			//allocated_tm += timer.round();
			allocator.construct(p);
			//construct_tm += timer.round();
			items.emplace_front(p);
		}
		allocated_tm += timer.round();
		for (typename AL::pointer it = nullptr; !items.empty() && (it = items.front());)
		{
			allocator.destroy(it);
			allocator.deallocate(it,1);
			items.pop_front();
		}
		deallocated_tm += timer.round();
	}
	g_timer.stop();

	std::string unit;
	printf("\ttotal time: %.2f %s\n", g_timer.elapse(g_timer.stop_at, unit), unit.c_str());
	printf("\tagv round allocate time: %.2f %s\n", timer.avg(allocated_tm, nRounds, unit), unit.c_str());
	printf("\tagv round deallocate time: %.2f %s\n", timer.avg(deallocated_tm, nRounds, unit), unit.c_str());
	printf("\tagv element alloc time: %.2f %s\n", timer.avg(allocated_tm, nRounds * nElements, unit), unit.c_str());
	printf("\tagv element deallocate time: %.2f %s\n", timer.avg(deallocated_tm, nRounds * nElements, unit), unit.c_str());
	printf("\n");
}

void example_mempool_perf(int argc, char* argv[]) {

	struct mytype {
		int n{0};
		uint8_t buffer[2048];
	};
	size_t rounds = 100, elements = 10000;
	{
		printf("std::allocator<%ld,%ld>\n", rounds, elements);
		test_allocator(std::allocator<mytype>(), rounds, elements);
	}

	{
		printf("c::mempool<%ld,%ld> no GC\n", rounds, elements);
		test_allocator(c::mempool<mytype>(elements), rounds, elements);
	}
	/*
	std::allocator<100,10000>
        total time: 468.03 ms
        agv round allocate time: 3.64 ms
        agv round deallocate time: 1.04 ms
        agv element alloc time: 363.84 ns
        agv element deallocate time: 103.59 ns

	c::mempool<100,10000> no GC
        total time: 61.93 ms
        agv round allocate time: 281.64 mks
        agv round deallocate time: 332.84 mks
        agv element alloc time: 28.16 ns
        agv element deallocate time: 33.28 ns
	*/
}