#include "ThreadPool.h"

ThreadPool::ThreadPool(const int n_threads) : m_threads(vector<thread>(n_threads)), m_shutdown(false)
{
}

void ThreadPool::init()
{
    for (int i = 0; i < m_threads.size(); i++)
    {
        m_threads[i] = thread([this, i] { ThreadWorker(this, i)(); });
    }
}

void ThreadPool::shutdown()
{
    m_shutdown = true;
    m_conditional_lock.notify_all();
    for (int i = 0; i < m_threads.size(); i++)
    {
        if (m_threads[i].joinable())
        {
            m_threads[i].join();
        }
    }
}

ThreadPool::~ThreadPool()
{
    poollog("ThreadPool deconstructs!", INFO);
}

ThreadPool::ThreadWorker::ThreadWorker(ThreadPool *pool, const int id) : m_pool(pool), m_id(id)
{
}

void ThreadPool::ThreadWorker::operator()()
{
    function<void()> func;
    bool dequeued;
    while (!m_pool->m_shutdown)
    {
        {
            unique_lock<mutex> lock(m_pool->m_conditional_mutex);

            m_pool->m_conditional_lock.wait(lock, [this] { return m_pool->m_shutdown || !m_pool->m_queue.empty(); });
            if (m_pool->m_shutdown)
            {
                if (likely(m_pool->m_queue.empty()))
                {
                    return;
                }
                else
                {
                    poollog(
                        "shutdowning, but task queue is not empty, ThreadWorker will close after the queue is empty!",
                        WARN);
                }
            }

            dequeued = m_pool->m_queue.dequeue(func);
        }
        if (dequeued)
            func();
    }
}

ThreadPool::ThreadWorker::~ThreadWorker()
{
    poollog("ThreadWorker deconstruct!", INFO);
}
