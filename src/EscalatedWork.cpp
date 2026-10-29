#include "EscalatedWork.h"
#include <iostream>
#include "FallbackHandler.h"
#include "WorkVisitor.h"
#include "InvalidHandlerChainException.h"
#include "InvalidTransitionException.h"

namespace tividy {

template <typename T>
EscalatedWork<T>::EscalatedWork(WorkComponent<T>* inner)
    : WorkItemDecorator<T>(inner), chain(nullptr), position(-1), escalating(false), currentContact(nullptr) {}

template <typename T>
EscalatedWork<T>::~EscalatedWork() {
    delete chain;
}

template <typename T>
bool EscalatedWork<T>::hasEscalation() const {
    return true;
}

template <typename T>
Participant<T>* EscalatedWork<T>::getEscalationContact() const {
    return currentContact;
}

template <typename T>
void EscalatedWork<T>::setEscalationChain(Handler<T>* newChain) {
    if (newChain == nullptr) {
        throw InvalidHandlerChainException("An escalation chain is required");
    }
    Handler<T>* last = newChain;
    while (last->getNext() != nullptr) {
        last = last->getNext();
    }
    if (dynamic_cast<FallbackHandler<T>*>(last) == nullptr) {
        throw InvalidHandlerChainException("An escalation chain must end with a FallbackHandler");
    }
    if (chain != newChain) {
        delete chain;
    }
    chain = newChain;
    position = -1;
    escalating = false;
    currentContact = nullptr;
}

template <typename T>
void EscalatedWork<T>::start() {
    this->inner->start();
    position = -1;
    escalating = false;
    currentContact = nullptr;
}

template <typename T>
void EscalatedWork<T>::escalate() {
    if (chain == nullptr) {
        throw InvalidHandlerChainException("No escalation chain has been assigned");
    }
    bool first = (position == -1);
    this->inner->escalate();
    if (first) {
        std::cout << "  Starting escalation chain for '" << this->getName() << "'..." << std::endl;
    }
    escalating = true;
    ++position;
    Handler<T>* handler = chain->handle(*this, position);
    Participant<T>* contact = handler != nullptr ? handler->getParticipant() : nullptr;
    currentContact = contact;
    if (contact != nullptr) {
        std::cout << "  " << contact->getName() << " is busy handling the escalation of '" << this->getName()
                  << "' (level " << (position + 1) << ")." << std::endl;
    }
}

template <typename T>
void EscalatedWork<T>::complete() {
    if (!escalating) {
        this->inner->complete();
        return;
    }
    escalating = false;
    try {
        bool approval = this->inner->hasApproval();
        std::cout << "  Escalation of '" << this->getName() << "' is resolved"
                  << (approval ? "; handing over to the approval chain." : ".") << std::endl;
        if (approval) {
            this->inner->approve();
        } else {
            this->inner->complete();
        }
    } catch (...) {
        escalating = true;
        throw;
    }
}

template <typename T>
void EscalatedWork<T>::reject() {
    this->inner->reject();
    escalating = false;
}

template <typename T>
void EscalatedWork<T>::cancel() {
    this->inner->cancel();
    escalating = false;
}

template <typename T>
void EscalatedWork<T>::approve() {
    if (escalating) {
        throw InvalidTransitionException(this->inner->getStateName(), "approve");
    }
    this->inner->approve();
}

template <typename T>
WorkMemento<T>* EscalatedWork<T>::createMemento() const {
    WorkMemento<T>* memento = this->inner->createMemento();
    Handler<T>* chainCopy = nullptr;
    try {
        chainCopy = chain != nullptr ? chain->copy() : nullptr;
        memento->setLayer(this, chainCopy, position, currentContact, escalating);
    } catch (...) {
        delete chainCopy;
        delete memento;
        throw;
    }
    return memento;
}

template <typename T>
void EscalatedWork<T>::restore(const WorkMemento<T>& memento) {
    this->inner->restore(memento);
    const Handler<T>* saved = memento.getChain(this);
    Handler<T>* chainCopy = saved != nullptr ? saved->copy() : nullptr;
    delete chain;
    chain = chainCopy;
    position = memento.getPosition(this);
    currentContact = memento.getContact(this);
    escalating = memento.isEscalating(this);
}

template <typename T>
WorkComponent<T>* EscalatedWork<T>::copy() const {
    EscalatedWork<T>* clone = new EscalatedWork<T>(this->inner->copy());
    try {
        if (chain != nullptr) {
            clone->chain = chain->copy();
        }
    } catch (...) {
        delete clone;
        throw;
    }
    clone->position = position;
    clone->escalating = escalating;
    clone->currentContact = currentContact;
    return clone;
}

template <typename T>
void EscalatedWork<T>::accept(WorkVisitor<T>& visitor) {
    visitor.visit(*this);
    this->inner->accept(visitor);
}

template class EscalatedWork<int>;
template class EscalatedWork<std::string>;

}
