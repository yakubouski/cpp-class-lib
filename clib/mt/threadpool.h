#pragma once

#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <future>

namespace c {
	class threadpool {
	private:
		using threads = std::unordered_map< std::thread::id, std::thread >;
		enum WorkerType { sPersistent, sSecondary, sAsync };
		std::atomic_bool				PoolYet;
		size_t							PoolInitWorkers;
		size_t							PoolMaxWorkers;
		size_t							PoolIndexWorkers;
		std::atomic_long				PoolBusyWorkers;
		threads							PoolWorkers;
		std::queue< std::function<void()> >	PoolTasks;
		std::mutex						PoolQueueSync;
		std::condition_variable			PoolCondition;
		std::vector<size_t>				PoolWorkersBinding;

		void check_resize(size_t NumWorker, size_t NumTasks);
		void hire(size_t NumWorkers, WorkerType Type);

		template<typename F, typename Tuple, size_t ...S >
		static inline decltype(auto)  apply_tuple_impl(F&& fn, Tuple&& t, std::index_sequence<S...>)
		{
			return fn(std::get<S>(std::forward<Tuple>(t))...);
		}
		template<typename F, typename Tuple>
		static inline decltype(auto) apply_from_tuple(F&& fn, Tuple&& t)
		{
			std::size_t constexpr tSize
				= std::tuple_size<typename std::remove_reference<Tuple>::type>::value;
			return apply_tuple_impl(std::forward<F>(fn),
				std::forward<Tuple>(t),
				std::make_index_sequence<tSize>());
		}

	public:
		/*
		* InitialWorkers - number of persistent workers
		* MaxWorkers - number of maximum workers (auto scale up to)
		* CoreBinding - binding threads to cores list
		* CoreExclude - exclude core from binding
		*/
		threadpool(size_t InitialWorkers, size_t MaxWorkers, const std::vector<size_t>& CoreBinding = {}, const std::vector<size_t>& CoreExclude = {});
		~threadpool();

		/*
		* Core binding list
		*/
		inline const std::vector<size_t>& cores() const { return PoolWorkersBinding; }

		/*
		* Wait when all task is processed and all threads down.
		* After call join() no any task will be added
		*/
		void join();


		/*
		* Get current pool status
		*/
		void stats(size_t& NumWorkers, size_t& NumAwaitingTasks, size_t& NumBusy);

		/*
		* Enqueue job with class handler
		*/
		template<class CLASS, class... Args>
		void enqueue(Args&& ... args) {

			auto args_list = std::tuple<Args...>(args...);

			size_t numWorkers = 0, numTasks = 0;
			{
				std::unique_lock<std::mutex> lock(PoolQueueSync);
				if (!PoolYet)
					throw std::runtime_error("enqueue on stopped ThreadPool");

				PoolTasks.emplace([args_list]() { CLASS obj; apply_from_tuple(obj, args_list); });

				numWorkers = PoolWorkers.size();
				numTasks = PoolTasks.size();
			}
			check_resize(numWorkers, numTasks);
			PoolCondition.notify_one();
		}

		/*
		* Enqueue job with lambda or function handler
		*/
		template<class FN, class... Args>
		auto enqueue(FN&& f, Args&& ... args) -> std::future<typename std::result_of<FN(Args...)>::type> {

			using return_type = typename std::result_of<FN(Args...)>::type;

			auto task = std::make_shared< std::packaged_task<return_type()> >(
				std::bind(std::forward<FN>(f), std::forward<Args>(args)...)
				);

			size_t numWorkers = 0, numTasks = 0;
			std::future<return_type> res = task->get_future();
			{
				std::unique_lock<std::mutex> lock(PoolQueueSync);
				if (!PoolYet)
					throw std::runtime_error("enqueue on stopped ThreadPool");

				PoolTasks.emplace([task]() { (*task)(); });

				numWorkers = PoolWorkers.size();
				numTasks = PoolTasks.size();
			}
			check_resize(numWorkers, numTasks);
			PoolCondition.notify_one();
			return res;
		}
	};
}