//
// Created by JunMin Kim on 2020/05/01.
//

#ifndef PROACTOR_INITIATOR_H
#define PROACTOR_INITIATOR_H

#include <future>
#include <memory>
#include <thread>

#include "ASynchronousOperationProcessor.h"
#include "CompletionEventQueue.h"
#include "Proactor.h"

template <typename T>
class Initiator : public Observer<AsynchronousOperation<T>>
{
    std::shared_ptr<CompletionEventQueue<T>> completionEventQueue;

    std::shared_ptr<ASynchronousOperationProcessor<T>> asynchoronousProcessor;

    std::unique_ptr<Proactor<T>> proactor;

    std::future<void> proactorThread;
public:
    Initiator()
    :completionEventQueue(std::shared_ptr<CompletionEventQueue<T>>(new CompletionEventQueue<T>())),
    asynchoronousProcessor(std::shared_ptr<ASynchronousOperationProcessor<T>>(new ASynchronousOperationProcessor<T>(completionEventQueue))),
    proactor(std::unique_ptr<Proactor<T>>(new Proactor<T>(completionEventQueue, this)))
    {
        proactorThread = std::async(std::launch::async, &Proactor<T>::execute, proactor.get());
    }

    virtual ~Initiator()
    {
        while(completionEventQueue->size() != 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        proactor->canFinish(true);

        proactorThread.wait();
    }

    void process(AsynchronousOperation<T>* operation)
    {
        asynchoronousProcessor->Register(operation);
    }

    void notify(AsynchronousOperation<T>* operation)
    {
        std::cout << "Result Opeation : " << operation->getResult() << std::endl;
    }
};

#endif //PROACTOR_INITIATOR_H
