#ifndef WORKMEMENTO_H
#define WORKMEMENTO_H

#include <map>
#include <vector>
#include "WorkItemState.h"

namespace tividy {

template <typename T> class WorkItem;
template <typename T> class ApprovedWork;
template <typename T> class EscalatedWork;
template <typename T> class WorkComponent;
template <typename T> class Participant;
template <typename T> class Handler;


template <typename T>
class WorkMemento {
public:
    
    ~WorkMemento();

private:
    friend class WorkItem<T>;
    friend class ApprovedWork<T>;
    friend class EscalatedWork<T>;

    
    WorkMemento(WorkItemState<T>* state, const std::vector<Participant<T>*>& assignees);
    WorkMemento(const WorkMemento&) = delete;
    WorkMemento& operator=(const WorkMemento&) = delete;

    
    const WorkItemState<T>* getState() const;
    
    const std::vector<Participant<T>*>& getAssignees() const;

    
    void setLayer(const WorkComponent<T>* layer, Handler<T>* chainCopy, int position, Participant<T>* contact, bool escalating);
    
    const Handler<T>* getChain(const WorkComponent<T>* layer) const;
    
    int getPosition(const WorkComponent<T>* layer) const;
    
    Participant<T>* getContact(const WorkComponent<T>* layer) const;
    
    bool isEscalating(const WorkComponent<T>* layer) const;

    WorkItemState<T>* state;  ///< Owned state copy.
    std::vector<Participant<T>*> assignees;  ///< Assignees at snapshot time.
    std::map<const WorkComponent<T>*, Handler<T>*> chains;  ///< Owned chain copies per layer.
    std::map<const WorkComponent<T>*, int> positions;  ///< Chain positions per layer.
    std::map<const WorkComponent<T>*, Participant<T>*> contacts;  ///< Current contacts per layer.
    std::map<const WorkComponent<T>*, bool> escalatingFlags;  ///< Escalating flags per layer.
};

}

#endif
