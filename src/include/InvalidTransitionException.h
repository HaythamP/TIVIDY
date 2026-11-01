#ifndef INVALIDTRANSITIONEXCEPTION_H
#define INVALIDTRANSITIONEXCEPTION_H

#include <string>
#include "TividyException.h"

namespace tividy {

class InvalidTransitionException : public TividyException {
public:
    InvalidTransitionException(const std::string& stateName, const std::string& operation);
    ~InvalidTransitionException() noexcept;

    const std::string& getStateName() const;
    const std::string& getOperation() const;

private:
    std::string stateName;
    std::string operation;
};

}

#endif