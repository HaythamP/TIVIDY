#ifndef UNSUPPORTEDOPERATIONEXCEPTION_H
#define UNSUPPORTEDOPERATIONEXCEPTION_H

#include <string>
#include "TividyException.h"

namespace tividy {

class UnsupportedOperationException : public TividyException {
public:
    explicit UnsupportedOperationException(const std::string& message);
    ~UnsupportedOperationException() noexcept;
};

}

#endif