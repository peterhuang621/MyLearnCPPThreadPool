#pragma once
#include "common.h"

template <typename T> class SafeQueue : public NonCopyable
{
  private:
    queue<T> m_queue;
    mutex m_mutex;

  public:
    SafeQueue()
    {
    }
    SafeQueue(const SafeQueue &&other)
    {
    }
    ~SafeQueue()
    {
        poollog("SafeQueue deconstructs!", INFO);
    }

    bool empty()
    {
        lock_guard<mutex> lock(m_mutex);
        return m_queue.empty();
    }

    int size()
    {
        lock_guard<mutex> lock(m_mutex);
        return m_queue.size();
    }

    void enqueue(const T &t)
    {
        lock_guard<mutex> lock(m_mutex);
        m_queue.emplace(t);
    }

    bool dequeue(T &t)
    {
        lock_guard<mutex> lock(m_mutex);
        if (m_queue.empty())
            return false;
        t = std::move(m_queue.front());
        m_queue.pop();

        return true;
    }
};