#ifndef DUMMY_EXCEPTION_H
#define DUMMY_EXCEPTION_H

#include "TividyException.h"
#include <string>

namespace tividy {

class DummyException : public TividyException {
public:
    explicit DummyException(const std::string& message);
    virtual ~DummyException() noexcept;
};

} // namespace tividy

#endif // DUMMY_EXCEPTION_H