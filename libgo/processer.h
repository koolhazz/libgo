#pragma once
#include "config.h"
#include "task.h"
#include "ts_queue.h"

namespace co {

struct ThreadLocalInfo;

// 协程执行器
//   管理一批协程的共享栈和调度, 非线程安全.
class Processer
    : public TSQueueHook
{
private:
#if NO_MULTI_THREAD
    typedef TSQueue<Task> TaskList;
#else
    typedef TSQueue<Task, false> TaskList;
#endif

    Task* current_task_ = nullptr;
    TaskList runnable_list_;
    uint32_t id_;
    static std::atomic<uint32_t> s_id_;

public:
    explicit Processer();

    void AddTaskRunnable(Task *tk);

    uint32_t Run(uint32_t &done_count);

    void CoYield();

    uint32_t GetTaskCount();

    Task* GetCurrentTask();

    std::size_t StealHalf(Processer & other);
};

} //namespace co
