#ifndef COMPONENTNOTFOUNDEXCEPTION_H
#define COMPONENTNOTFOUNDEXCEPTION_H

#include <string>
#include "TividyException.h"

namespace tividy {

class ComponentNotFoundException : public TividyException {
public:
    explicit ComponentNotFoundException(const std::string& message);
    ~ComponentNotFoundException() noexcept;
};

}

#endif