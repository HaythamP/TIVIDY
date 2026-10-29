#include "ApprovedWork.h"
#include <iostream>
#include "LevelHandler.h"
#include "WorkVisitor.h"
#include "InvalidHandlerChainException.h"
#include "ApprovalIncompleteException.h"

namespace tividy {

template <typename T>
ApprovedWork<T>::ApprovedWork(WorkComponent<T>* inner)
    : WorkItemDecorator<T>(inner), chain(nullptr), chainLength(0), position(-1), currentContact(nullptr) {}

template <typename T>
ApprovedWork<T>::~ApprovedWork() {
    delete chain;
}

template <typename T>
bool ApprovedWork<T>::hasApproval() const {
    return true;
}

template <typename T>
Participant<T>* ApprovedWork<T>::getApprovalContact() const {
    return currentContact;
}

template <typename T>
void ApprovedWork<T>::setApprovalChain(Handler<T>* newChain) {
    if (newChain == nullptr) {
        throw InvalidHandlerChainException("An approval chain with at least one approver is required");
    }
    int length = 0;
    for (Handler<T>* h = newChain; h != nullptr; h = h->getNext()) {
        if (dynamic_cast<LevelHandler<T>*>(h) == nullptr) {
            throw InvalidHandlerChainException("An approval chain may only contain LevelHandlers");
        }
        ++length;
    }
    if (chain != newChain) {
        delete chain;
    }
    chain = newChain;
    chainLength = length;
    position = -1;
    currentContact = nullptr;
}

template <typename T>
void ApprovedWork<T>::start() {
    this->inner->start();
    position = -1;
    currentContact = nullptr;
}

template <typename T>
void ApprovedWork<T>::approve() {
    if (chain == nullptr) {
        throw InvalidHandlerChainException("No approval chain has been assigned");
    }
    bool first = (position == -1);
    this->inner->approve();
    if (first) {
        std::cout << "  Starting approval chain for '" << this->getName() << "' (" << chainLength << " approver(s))..." << std::endl;
        position = 0;
    } else {
        if (currentContact != nullptr) {
            std::cout << "  " << currentContact->getName() << " has signed off '" << this->getName() << "'." << std::endl;
        }
        ++position;
    }
    Handler<T>* handler = chain->handle(*this, position);
    Participant<T>* contact = handler != nullptr ? handler->getParticipant() : nullptr;
    currentContact = contact;
    std::string who = contact != nullptr ? contact->getName() : std::string("The final approver");
    if (position == chainLength - 1) {
        std::cout << "  " << who << " is the final approver and approves '" << this->getName() << "' - all approvals obtained." << std::endl;
        this->inner->complete();
    } else {
        std::cout << "  " << who << " is busy approving '" << this->getName() << "' (approver " << (position + 1) << " of " << chainLength << ")." << std::endl;
    }
}

template <typename T>
void ApprovedWork<T>::complete() {
    if (chain != nullptr && position < chainLength) {
        throw ApprovalIncompleteException("Approval is incomplete for " + this->getName());
    }
    this->inner->complete();
}

template <typename T>
WorkMemento<T>* ApprovedWork<T>::createMemento() const {
    WorkMemento<T>* memento = this->inner->createMemento();
    Handler<T>* chainCopy = nullptr;
    try {
        chainCopy = chain != nullptr ? chain->copy() : nullptr;
        memento->setLayer(this, chainCopy, position, currentContact, false);
    } catch (...) {
        delete chainCopy;
        delete memento;
        throw;
    }
    return memento;
}

template <typename T>
void ApprovedWork<T>::restore(const WorkMemento<T>& memento) {
    this->inner->restore(memento);
    const Handler<T>* saved = memento.getChain(this);
    Handler<T>* chainCopy = saved != nullptr ? saved->copy() : nullptr;
    delete chain;
    chain = chainCopy;
    chainLength = 0;
    for (Handler<T>* h = chain; h != nullptr; h = h->getNext()) {
        ++chainLength;
    }
    position = memento.getPosition(this);
    currentContact = memento.getContact(this);
}

template <typename T>
WorkComponent<T>* ApprovedWork<T>::copy() const {
    ApprovedWork<T>* clone = new ApprovedWork<T>(this->inner->copy());
    try {
        if (chain != nullptr) {
            clone->chain = chain->copy();
        }
    } catch (...) {
        delete clone;
        throw;
    }
    clone->chainLength = chainLength;
    clone->position = position;
    clone->currentContact = currentContact;
    return clone;
}

template <typename T>
void ApprovedWork<T>::accept(WorkVisitor<T>& visitor) {
    visitor.visit(*this);
    this->inner->accept(visitor);
}

template class ApprovedWork<int>;
template class ApprovedWork<std::string>;

}
