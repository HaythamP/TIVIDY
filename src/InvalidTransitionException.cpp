#include "InvalidTransitionException.h"

namespace tividy {

InvalidTransitionException::InvalidTransitionException(const std::string& stateName, const std::string& operation)
    : TividyException("Cannot " + operation + " while in state " + stateName),
      stateName(stateName),
      operation(operation) {}

InvalidTransitionException::~InvalidTransitionException() noexcept {}

const std::string& InvalidTransitionException::getStateName() const {
    return stateName;
}

const std::string& InvalidTransitionException::getOperation() const {
    return operation;
}

}