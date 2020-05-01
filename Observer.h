//
// Created by JunMin Kim on 2020/05/01.
//

#ifndef PROACTOR_OBSERVER_H
#define PROACTOR_OBSERVER_H

template <typename T>
class Observer
{
public:
    virtual void notify(T* operation) = 0;

    virtual ~Observer() {};
};


#endif //PROACTOR_OBSERVER_H
