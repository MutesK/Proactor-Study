#include <iostream>
#include <memory>

#include "AsynchronousOperation.h"
#include "ASynchronousOperationProcessor.h"
#include "Initiator.h"
#include "Proactor.h"

template <typename T>
class SumAsynchronousOperation : public AsynchronousOperation<T>
{
private:
    /**
     * List of elements to compute the sum operation
     */
    std::list<T> elements;
public:
    /**
     * Constructor
     * @param[in] numbers	Elements that take part of the computation
     */
    SumAsynchronousOperation(std::list<T> numbers) {
        elements = numbers;
        // Indicate that the result, should the operation has been calculated, is not updated
        AsynchronousOperation<T>::executed = false;
    }

    /**
     * Delegating constructor
     * @param[in] element	Element to compute
     */
    SumAsynchronousOperation(T element) : SumAsynchronousOperation(std::list<T>({element})) {
    };

    /**
     * Delegating constructor
     * @param[in] element1	First element in the computation
     * @param[in] element2	Second element in the computation
     */
    SumAsynchronousOperation(T element1, T element2) : SumAsynchronousOperation(std::list<T>({element1,element2})) {
    };

    /**
     * Class destructor
     */
    ~SumAsynchronousOperation() {
        elements.clear();
    };

    /**
     * Run the computation over the defined elements
     */
    void executeOperation() {
        // We do not need a lock as the operation is performed by only one thread
        AsynchronousOperation<T>::result = 0;
        for (T element: elements)
            AsynchronousOperation<T>::result += element;

        // Add a random time
        // NOTE: This is only for testing purposes!!! In the real pattern, you should remove
        // this line because it will make the pattern run slowly. However, it is added here
        // to verify the behavior with different threads and operations
        // (maximum sleep time: 9.999 seconds)
        std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 10000));

        AsynchronousOperation<T>::executed = true;
    }

    /**
     * Obtains the result of the computation, or an exception in case the computation
     * has not been done
     * @return	Returns the value of the computation, or an exception if the computation
     * has not finished/been done
     * @see exception::OperationNotFinishedException
     */
    T getResult() const {
        if (AsynchronousOperation<T>::executed)
            return AsynchronousOperation<T>::result;

        throw std::exception();
    }
};

int main()
{
    SumAsynchronousOperation<int> op1 = SumAsynchronousOperation<int>(10, 11);
    SumAsynchronousOperation<int> op2 = SumAsynchronousOperation<int>(20, 21);
    SumAsynchronousOperation<int> op3 = SumAsynchronousOperation<int>(30, 31);
    SumAsynchronousOperation<int> op4 = SumAsynchronousOperation<int>(40, 41);
    SumAsynchronousOperation<int> op5 = SumAsynchronousOperation<int>(50, 51);
    SumAsynchronousOperation<int> op6 = SumAsynchronousOperation<int>(60, 61);


    Initiator<int> initiator;
    initiator.process(&op1);
    initiator.process(&op2);
    initiator.process(&op3);
    initiator.process(&op4);
    initiator.process(&op5);
    initiator.process(&op6);

}
