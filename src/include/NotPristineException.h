#ifndef NOTPRISTINEEXCEPTION_H
#define NOTPRISTINEEXCEPTION_H

#include <string>
#include "TividyException.h"

namespace tividy {

class NotPristineException : public TividyException {
public:
    explicit NotPristineException(const std::string& message);
    ~NotPristineException() noexcept;
};

}

#endif