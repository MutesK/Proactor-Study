//
// Created by JunMin Kim on 2020/05/01.
//

#ifndef PROACTOR_PROACTOR_H
#define PROACTOR_PROACTOR_H

#include <condition_variable>
#include <memory>
#include <mutex>
#include <mutex>
#include "CompletionEventQueue.h"
#include "Observer.h"

template <typename T>
class Proactor
{
    std::shared_ptr<CompletionEventQueue<T>> completionEventQueue;

    Observer<AsynchronousOperation<T>>* observer;
    bool isfinish;
public:
    Proactor(std::shared_ptr<CompletionEventQueue<T>> completionEventQueue,
            Observer<AsynchronousOperation<T>>* observer)
            :completionEventQueue(completionEventQueue),
            observer(observer), isfinish(false)
    {
    }
    virtual ~Proactor()
    {
    }

    void canFinish(const bool finish)
    {
        this->isfinish = finish;
    }

    void execute()
    {
        while(!isfinish || completionEventQueue->arePendingOperations())
        {
            while(completionEventQueue->size() == 0 && !isfinish)
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            if(completionEventQueue->size() > 0)
            {
                auto operation = completionEventQueue->pop();
                observer->notify(operation);
            }
        }
    }
};
#endif //PROACTOR_PROACTOR_H
