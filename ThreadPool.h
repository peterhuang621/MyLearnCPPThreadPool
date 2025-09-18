#pragma once
#include "SafeQueue.h"

class ThreadPool : public NonCopyable
{
  private:
    SafeQueue<function<void()>> m_queue;
    condition_variable m_conditional_lock;
    mutex m_conditional_mutex;
    bool m_shutdown;

    class ThreadWorker : public NonCopyable
    {
      private:
        int m_id;
        ThreadPool *m_pool;

      public:
        ThreadWorker() = delete;
        ThreadWorker(ThreadPool *pool, const int id);
        void operator()();
        ~ThreadWorker();
    };

  public:
    template <typename F, typename... Args>
    auto submit(F &&f, Args &&...args) -> future<decltype(forward<F>(f)(forward<Args>(args)...))>
    {
        using return_type = decltype(forward<F>(f)(forward<Args>(args)...));

        function<return_type()> func = bind(forward<F>(f), forward<Args>(args)...);
        shared_ptr<packaged_task<return_type()>> task_ptr = make_shared<packaged_task<return_type()>>(func);

        function<void()> wrapper_func = [task_ptr]() { (*task_ptr)(); };

        m_queue.enqueue(wrapper_func);
        m_conditional_lock.notify_one();
        return task_ptr->get_future();
    }

    ~ThreadPool();
};