#include "Handler.h"
#include "WorkComponent.h"
#include <string>

namespace tividy {

template <typename T>
Handler<T>::Handler() : next(nullptr) {}

template <typename T>
Handler<T>::~Handler() {
    delete next;
}

template <typename T>
void Handler<T>::setNext(Handler<T>* nextHandler) {
    if (next != nullptr && next != nextHandler) {
        delete next;
    }
    next = nextHandler;
}

template <typename T>
Handler<T>* Handler<T>::getNext() const {
    return next;
}

template <typename T>
Handler<T>* Handler<T>::handle(WorkComponent<T>& work, int position) {
    if (canHandle(work, position)) {
        process(work);
        return this;
    }
    if (next != nullptr) {
        return next->handle(work, position);
    }
    return nullptr;
}

template <typename T>
Handler<T>* Handler<T>::copy() const {
    Handler<T>* clone = cloneSelf();
    if (next != nullptr) {
        try {
            clone->setNext(next->copy());
        } catch (...) {
            delete clone;
            throw;
        }
    }
    return clone;
}

template <typename T>
Participant<T>* Handler<T>::getParticipant() const {
    return nullptr;
}

template class Handler<int>;
template class Handler<std::string>;

}