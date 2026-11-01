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

/**
 * @brief Memento of the Memento pattern: an opaque snapshot of a work item.
 *
 * It stores a copy of the state, the assignees and, for each decorator layer, a copy of its handler chain, position, contact and escalating flag. Only the originators (WorkItem, EscalatedWork, ApprovedWork) are friends and can read it (wide interface); the caretaker (WorkCommand) can only hold it (narrow interface). The memento owns the state and chain copies.
 */
template <typename T>
class WorkMemento {
public:
    /** @brief Deletes the state copy and chain copies. */
    ~WorkMemento();

private:
    friend class WorkItem<T>;
    friend class ApprovedWork<T>;
    friend class EscalatedWork<T>;

    /** @brief Takes ownership of a state copy and remembers the assignees. */
    WorkMemento(WorkItemState<T>* state, const std::vector<Participant<T>*>& assignees);
    WorkMemento(const WorkMemento&) = delete;
    WorkMemento& operator=(const WorkMemento&) = delete;

    /** @brief Stored state (originator only). */
    const WorkItemState<T>* getState() const;
    /** @brief Stored assignees (originator only). */
    const std::vector<Participant<T>*>& getAssignees() const;

    /** @brief Stores one decorator layer's data and takes ownership of the chain copy. */
    void setLayer(const WorkComponent<T>* layer, Handler<T>* chainCopy, int position, Participant<T>* contact, bool escalating);
    /** @brief Stored chain copy for the layer, or null. */
    const Handler<T>* getChain(const WorkComponent<T>* layer) const;
    /** @brief Stored handler position for the layer (-1 if none). */
    int getPosition(const WorkComponent<T>* layer) const;
    /** @brief Stored contact for the layer. */
    Participant<T>* getContact(const WorkComponent<T>* layer) const;
    /** @brief Stored escalating flag for the layer. */
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
