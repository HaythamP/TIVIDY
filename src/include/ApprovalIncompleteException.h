#ifndef APPROVALINCOMPLETEEXCEPTION_H
#define APPROVALINCOMPLETEEXCEPTION_H

#include <string>
#include "TividyException.h"

namespace tividy {

class ApprovalIncompleteException : public TividyException {
public:
    explicit ApprovalIncompleteException(const std::string& message);
    ~ApprovalIncompleteException() noexcept;
};

}

#endif