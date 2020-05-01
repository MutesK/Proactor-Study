//
// Created by JunMin Kim on 2020/05/01.
//

#ifndef PROACTOR_COMPLETIONEVENTQUEUE_H
#define PROACTOR_COMPLETIONEVENTQUEUE_H

#include <deque>
#include <memory>
#include <mutex>
#include <utility>
#include "AsynchronousOperation.h"

template <typename T>
class CompletionEventQueue : private std::deque<AsynchronousOperation<T>*>
{
    typedef std::deque<AsynchronousOperation<T>*> super;
    std::mutex _mutex;

    uint32_t pendingoperation;
public:
    CompletionEventQueue()
    : super(), pendingoperation(0)
    {
    }

    virtual ~CompletionEventQueue()
    {
        this->clear();
    }

    AsynchronousOperation<T>* pop()
    {
        std::lock_guard<std::mutex> lock(_mutex);

        AsynchronousOperation<T>* pOper = this->front();
        this->pop_front();

        return pOper;
    }

    void push(AsynchronousOperation<T>* operation)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        this->push_back(operation);

        if(pendingoperation == 0)
            throw std::exception();

        --pendingoperation;
    }

    const std::size_t size()
    {
        std::lock_guard<std::mutex> lock(_mutex);

        return super::size();
    }

    void incrementPendingOperation()
    {
        std::lock_guard<std::mutex> lock(_mutex);

        ++pendingoperation;
    }

    bool arePendingOperations()
    {
        std::lock_guard<std::mutex> lock(_mutex);

        return (pendingoperation == 0);
    }


};


#endif //PROACTOR_COMPLETIONEVENTQUEUE_H
