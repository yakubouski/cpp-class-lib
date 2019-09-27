#pragma once
#include "../pch.h"
#include <forward_list>
#include <cstdio>
namespace c {
	template<typename T,typename A_T = std::allocator<T>>
	class mempool {
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef A_T allocator;

		mempool(size_t pool_size = 1024) {
			PoolIt = PoolHead = new pointer[pool_size];
			PoolTail = PoolHead + pool_size;
		}
		~mempool() { 
			while (PoolIt != PoolHead) {
				PoolAllocator.deallocate(*(--PoolIt), 1);
			}
			delete[] PoolHead;
		}

		forceinline void construct(pointer ptr) { new(ptr) value_type; }
		forceinline void construct(pointer ptr, const value_type& val) { new(ptr) value_type(val); }
		forceinline void construct(pointer ptr, value_type&& val) { new(ptr) value_type(std::move(val)); }
		forceinline void destroy(pointer ptr) { if (ptr) { ptr->~value_type(); } }

		forceinline pointer allocate(size_t)
		{
			if (PoolIt != PoolHead) {
				return *(--PoolIt);
			}
			return PoolAllocator.allocate(1);
		}

		forceinline void deallocate(pointer p, size_t)
		{
			if (PoolIt < PoolTail) {
				*PoolIt = p;
				PoolIt++;
				return;
			}
			PoolAllocator.deallocate(p, 1);
		}
	private:
		allocator	PoolAllocator; /* block allocattor */
		pointer*		PoolHead{ nullptr }, *PoolTail{ nullptr }, *PoolIt{ nullptr };
	};
}