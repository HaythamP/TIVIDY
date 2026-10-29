#ifndef MEDIATOR_H
#define MEDIATOR_H

namespace tividy {

template <typename T> class WorkComponent;


template <typename T>
class Mediator {
public:
   
    virtual ~Mediator() = default;
    virtual void notify(WorkComponent<T>* sender) = 0;
    virtual WorkComponent<T>* findNext(WorkComponent<T>* sender) const = 0;
    virtual void addColleague(WorkComponent<T>* colleague) = 0;
};

}

#endif