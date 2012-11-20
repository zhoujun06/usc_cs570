#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <cstring>
#include <functional>
#include "hw4_include.h"
#include "mazedata.hpp"


enum QueueType_t
{
    FIFO_QUEUE = 1,
    LIFO_QUEUE,
    PRIO_QUEUE,
};

template <class T>
class Queue
{
public:

    virtual bool isEmpty()=0;

    virtual T pop()=0;

    virtual void push(T& item)=0;

    virtual size_t size()=0;

    virtual void clear()=0;

};

template <class T>
class FIFOQueue : public Queue<T> {
public:

    bool isEmpty()
    {
        return _fifo_queue.empty();
    }

    T pop()
    {
        T item = _fifo_queue.front();
        _fifo_queue.pop();

        return item;
    }

    void push(T& item)
    {
        _fifo_queue.push(item);
    }

    size_t size()
    {
        return _fifo_queue.size();
    }

    void clear()
    {
        while (!_fifo_queue.empty()) {
            _fifo_queue.pop();
        }
    }

private:
    std::queue<T> _fifo_queue;

};

class PrioCompare
{
public:
    bool operator() (State *ln, State *rn) const
    {
        return ln->getPathCost() > rn->getPathCost();
    }
};

template <class T>
class PrioQueue : public Queue<T>
{
public:
    bool isEmpty()
    {
        return _prio_queue.empty();
    }

    T pop()
    {
        T item = _prio_queue.top();
        _prio_queue.pop();
        return item;
    }

    void push(T& item)
    {
        _prio_queue.push(item);
    }

    size_t size()
    {
        return _prio_queue.size();
    }

    void clear()
    {
        while (!_prio_queue.empty()) {
            _prio_queue.pop();
        }
    }

private:
    std::priority_queue<T, std::vector<T>, PrioCompare > _prio_queue;
};


#endif 
