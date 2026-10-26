#ifndef DUPLICATEIDEXCEPTION_H
#define DUPLICATEIDEXCEPTION_H

#include <string>
#include "TividyException.h"

namespace tividy {

class DuplicateIdException : public TividyException {
public:
    explicit DuplicateIdException(const std::string& message);
    ~DuplicateIdException() noexcept;
};

}

#endif