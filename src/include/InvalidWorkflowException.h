#ifndef INVALIDWORKFLOWEXCEPTION_H
#define INVALIDWORKFLOWEXCEPTION_H

#include <string>
#include "TividyException.h"

namespace tividy {

class InvalidWorkflowException : public TividyException {
public:
    explicit InvalidWorkflowException(const std::string& message);
    ~InvalidWorkflowException() noexcept;
};

}

#endif