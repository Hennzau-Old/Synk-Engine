#pragma once

#include <queue>

#include "core/utils/logs/Logger.h"

template <typename T>
class Queue
{
    public:

        /* structure */

        /* functions */

        Queue()
        {

        }

        ~Queue()
        {
          
        }

        T                 get()
        {
            return m_queue.front();
        }
        void              push(T obj)
        {
            m_queue.push(obj);
        }
        void              pop()
        {
            m_queue.pop();
        }

        /* variables */

        std::queue<T>     getQueue() const
        {
            return m_queue;
        }

    private:

        /* structure */

        /* functions */

        /* variables */

        std::queue<T>     m_queue;
};
