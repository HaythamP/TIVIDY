#ifndef INVALIDHANDLERCHAINEXCEPTION_H
#define INVALIDHANDLERCHAINEXCEPTION_H

#include <string>
#include "TividyException.h"

namespace tividy {

class InvalidHandlerChainException : public TividyException {
public:
    explicit InvalidHandlerChainException(const std::string& message);
    ~InvalidHandlerChainException() noexcept;
};

}

#endif