#ifndef INVALIDASSIGNMENTARGUMENTEXCEPTION_H
#define INVALIDASSIGNMENTARGUMENTEXCEPTION_H

#include <string>
#include "TividyException.h"

namespace tividy {

class InvalidAssignmentArgumentException : public TividyException {
public:
    explicit InvalidAssignmentArgumentException(const std::string& message);
    ~InvalidAssignmentArgumentException() noexcept;
};

}

#endif