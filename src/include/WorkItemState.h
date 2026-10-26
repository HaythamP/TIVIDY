#ifndef WORKITEMSTATE_H
#define WORKITEMSTATE_H

#include <string>
#include <vector>

namespace tividy {

template <typename T> class WorkItem;
template <typename T> class Participant;

template <typename T>
class WorkItemState {
public:
    virtual ~WorkItemState();

    virtual std::string getName() const = 0;
    virtual WorkItemState<T>* copy() const = 0;

    virtual bool isComplete() const;
    virtual bool isAvailable() const;
    virtual bool isTerminal() const;

    virtual void makeAvailable(WorkItem<T>& item);
    virtual void assign(WorkItem<T>& item, const std::vector<Participant<T>*>& participants);
    virtual void start(WorkItem<T>& item);
    virtual void complete(WorkItem<T>& item);
    virtual void reject(WorkItem<T>& item);
    virtual void cancel(WorkItem<T>& item);
    virtual void escalate(WorkItem<T>& item);
    virtual void approve(WorkItem<T>& item);
};

}

#endif
