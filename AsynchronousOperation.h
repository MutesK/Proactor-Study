//
// Created by JunMin Kim on 2020/05/01.
//

#ifndef PROACTOR_ASYNCHRONOUSOPERATION_H
#define PROACTOR_ASYNCHRONOUSOPERATION_H

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>
#include "Observer.h"

template <typename T>
class AsynchronousOperation
{
    static uint64_t operationId;

protected:
    const uint64_t opId;

    Observer<AsynchronousOperation<T>>* _observer;
    bool executed;
    T result;

    AsynchronousOperation()
    : opId(++operationId), executed(false), _observer(nullptr), result()
    {
    }


    virtual void executeOperation() = 0;

public:
    virtual ~AsynchronousOperation() = default;

    void setObserver(Observer<AsynchronousOperation<T>>* observer)
    {
        this->_observer = observer;
    }

    void execute()
    {
        executeOperation();

        if(_observer)
            _observer->notify(this);
    }

    const uint64_t getId() const
    {
        return opId;
    }

    virtual T getResult() const = 0;
};

template <typename T>
uint64_t AsynchronousOperation<T>::operationId = 0;

#endif //PROACTOR_ASYNCHRONOUSOPERATION_H
