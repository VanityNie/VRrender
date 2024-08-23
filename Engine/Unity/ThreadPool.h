
#include <functional>
#include <mutex>
#include <queue>
#include <chrono>



template<typename T>

class ThreadSafeQueue
{
private:
    mutable std::mutex      m_mutex;
    std::condition_variable m_condition;
    std::queue<T>           m_queue;
public:


    //copy object to queue
    void Enqueue(const T& task)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push(task);
        }
        m_condition.notify_one();
    }

    //move object to queue
    void Emplace(T&& task)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.emplace(task);
        }
        m_condition.notify_one();
    }

    bool Pop(T& task)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_queue.empty())
        {
            return false;
        }

        task = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }

    bool Empty() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    size_t Size() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.size();
    }
    void Wait()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_queue.empty())
        {
            m_condition.wait(lock, [this] {
                return !m_queue.empty();
                });
        }
    }

    void WaitFor(std::chrono::milliseconds time = std::chrono::milliseconds(1))
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_queue.empty())
        {
            m_condition.wait_for(
                lock, time, [this] {
                    return !m_queue.empty();
                });
        }
    }

    void Wait(const std::atomic_bool& cancellationToken)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_queue.empty())
        {
            m_condition.wait(lock, [this, &cancellationToken] {
                return !m_queue.empty() || (cancellationToken.load() == true);
                });
        }
    }

    void Clear()
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::queue<T>               empty;
            std::swap(m_queue, empty);
        }
        m_condition.notify_all();
    }

};


class ThreadPool{

public:



};