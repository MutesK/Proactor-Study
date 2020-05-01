//
// Created by JunMin Kim on 2020/05/01.
//

#ifndef PROACTOR_ASYNCHRONOUSOPERATIONPROCESSOR_H
#define PROACTOR_ASYNCHRONOUSOPERATIONPROCESSOR_H

#include <chrono>
#include <condition_variable>
#include <future>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <deque>
#include "AsynchronousOperation.h"
#include "Observer.h"
#include "CompletionEventQueue.h"

template <typename T>
class ASynchronousOperationProcessor : public Observer<AsynchronousOperation<T>>
{
    size_t poolSize;

    std::mutex _lock;

    std::condition_variable cv;

    std::deque<AsynchronousOperation<T>*> pool;

    std::shared_ptr<CompletionEventQueue<T>> completionEventQueue;
public:
    static const size_t DEFAULT_QUEUE_SIZE = 1;

    ASynchronousOperationProcessor(std::shared_ptr<CompletionEventQueue<T>>& queue,
            const size_t poolSize = DEFAULT_QUEUE_SIZE)
            :poolSize(poolSize), pool(std::deque<AsynchronousOperation<T>*>()),
            completionEventQueue(queue)
            {
            }

    virtual ~ASynchronousOperationProcessor() =default;

    void Register(AsynchronousOperation<T>* operation)
    {
        std::unique_lock<std::mutex> lock(_lock);

        if(pool.size() == poolSize)cv.wait(lock, [&]{return pool.size() < poolSize;});


        operation->setObserver(this);

        completionEventQueue->incrementPendingOperation();

        std::thread t = std::thread(&AsynchronousOperation<T>::execute, operation);
        t.detach();

        pool.push_back(operation);
    }


    void notify(AsynchronousOperation<T>* operation)
    {
        std::unique_lock<std::mutex> lock(_lock);

        completionEventQueue->push(operation);

        bool isFull = false;
        typedef typename std::deque<AsynchronousOperation<T>*>::iterator iterType;

        iterType iter = std::find(pool.begin(), pool.end(), operation);
        if(iter != pool.end())
        {
            isFull = pool.size() == poolSize;
            pool.erase(iter);
        }

        if(isFull)
            cv.notify_one();
    }

};


#endif //PROACTOR_ASYNCHRONOUSOPERATIONPROCESSOR_H
