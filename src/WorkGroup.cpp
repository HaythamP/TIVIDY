#include "WorkGroup.h"
#include "WorkVisitor.h"
#include "DepthFirstIterator.h"
#include "InvalidWorkflowException.h"

namespace tividy {

template <typename T>
WorkGroup<T>::WorkGroup(const T& id, const std::string& name) : id(id), name(name) {}

template <typename T>
WorkGroup<T>::~WorkGroup() {
    for (std::size_t i = 0; i < children.size(); ++i) {
        delete children[i];
    }
}

template <typename T>
const T& WorkGroup<T>::getId() const {
    return id;
}

template <typename T>
const std::string& WorkGroup<T>::getName() const {
    return name;
}

template <typename T>
void WorkGroup<T>::setMediator(Mediator<T>* newMediator) {
    this->mediator = newMediator;
    for (std::size_t i = 0; i < children.size(); ++i) {
        children[i]->setMediator(newMediator);
    }
}

template <typename T>
std::vector<WorkComponent<T>*> WorkGroup<T>::getChildren() const {
    return children;
}

template <typename T>
std::vector<WorkComponent<T>*> WorkGroup<T>::getItems() {
    std::vector<WorkComponent<T>*> items;
    for (std::size_t i = 0; i < children.size(); ++i) {
        std::vector<WorkComponent<T>*> childItems = children[i]->getItems();
        items.insert(items.end(), childItems.begin(), childItems.end());
    }
    return items;
}

template <typename T>
void WorkGroup<T>::add(WorkComponent<T>* child) {
    if (child == nullptr) {
        throw InvalidWorkflowException("Cannot add a null component to group " + name);
    }
    child->setParent(this);
    if (this->mediator != nullptr) {
        child->setMediator(this->mediator);
    }
    children.push_back(child);
}

template <typename T>
WorkComponent<T>* WorkGroup<T>::find(const T& searchId) {
    if (id == searchId) {
        return this;
    }
    for (std::size_t i = 0; i < children.size(); ++i) {
        WorkComponent<T>* found = children[i]->find(searchId);
        if (found != nullptr) {
            return found;
        }
    }
    return nullptr;
}

template <typename T>
void WorkGroup<T>::release() {
    for (std::size_t i = 0; i < children.size(); ++i) {
        children[i]->release();
    }
}

template <typename T>
bool WorkGroup<T>::isComplete() const {
    if (children.empty()) {
        return false;
    }
    for (std::size_t i = 0; i < children.size(); ++i) {
        if (!children[i]->isComplete()) {
            return false;
        }
    }
    return true;
}

template <typename T>
bool WorkGroup<T>::isAvailable() const {
    for (std::size_t i = 0; i < children.size(); ++i) {
        if (children[i]->isAvailable()) {
            return true;
        }
    }
    return false;
}

template <typename T>
WorkComponent<T>* WorkGroup<T>::copy() const {
    WorkGroup<T>* clone = new WorkGroup<T>(id, name);
    try {
        for (std::size_t i = 0; i < children.size(); ++i) {
            clone->add(children[i]->copy());
        }
    } catch (...) {
        delete clone;
        throw;
    }
    return clone;
}

template <typename T>
void WorkGroup<T>::accept(WorkVisitor<T>& visitor) {
    visitor.visit(*this);
    for (std::size_t i = 0; i < children.size(); ++i) {
        children[i]->accept(visitor);
    }
}

template <typename T>
WorkIterator<T>* WorkGroup<T>::createIterator() {
    std::vector<WorkComponent<T>*> roots(1, this);
    return new DepthFirstIterator<T>(roots);
}

template class WorkGroup<int>;
template class WorkGroup<std::string>;

}
