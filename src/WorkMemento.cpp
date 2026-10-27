#include "WorkMemento.h"
#include "Handler.h"
#include "Participant.h"
#include <string>

namespace tividy {

template <typename T>
WorkMemento<T>::WorkMemento(WorkItemState<T>* state, const std::vector<Participant<T>*>& assignees)
    : state(state), assignees(assignees) {}

template <typename T>
WorkMemento<T>::~WorkMemento() {
    delete state;
    for (typename std::map<const WorkComponent<T>*, Handler<T>*>::iterator it = chains.begin(); it != chains.end(); ++it) {
        delete it->second;
    }
}

template <typename T>
const WorkItemState<T>* WorkMemento<T>::getState() const {
    return state;
}

template <typename T>
const std::vector<Participant<T>*>& WorkMemento<T>::getAssignees() const {
    return assignees;
}

template <typename T>
void WorkMemento<T>::setLayer(const WorkComponent<T>* layer, Handler<T>* chainCopy, int position, Participant<T>* contact, bool escalating) {
    typename std::map<const WorkComponent<T>*, Handler<T>*>::iterator it = chains.find(layer);
    if (it != chains.end()) {
        delete it->second;
        it->second = chainCopy;
    } else {
        chains[layer] = chainCopy;
    }
    positions[layer] = position;
    contacts[layer] = contact;
    escalatingFlags[layer] = escalating;
}

template <typename T>
const Handler<T>* WorkMemento<T>::getChain(const WorkComponent<T>* layer) const {
    typename std::map<const WorkComponent<T>*, Handler<T>*>::const_iterator it = chains.find(layer);
    return it == chains.end() ? nullptr : it->second;
}

template <typename T>
int WorkMemento<T>::getPosition(const WorkComponent<T>* layer) const {
    typename std::map<const WorkComponent<T>*, int>::const_iterator it = positions.find(layer);
    return it == positions.end() ? -1 : it->second;
}

template <typename T>
Participant<T>* WorkMemento<T>::getContact(const WorkComponent<T>* layer) const {
    typename std::map<const WorkComponent<T>*, Participant<T>*>::const_iterator it = contacts.find(layer);
    return it == contacts.end() ? nullptr : it->second;
}

template <typename T>
bool WorkMemento<T>::isEscalating(const WorkComponent<T>* layer) const {
    typename std::map<const WorkComponent<T>*, bool>::const_iterator it = escalatingFlags.find(layer);
    return it == escalatingFlags.end() ? false : it->second;
}

template class WorkMemento<int>;
template class WorkMemento<std::string>;

}
