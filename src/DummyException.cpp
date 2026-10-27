#include "DummyException.h"

namespace tividy {

DummyException::DummyException(const std::string& message)
    : TividyException(message) {}

DummyException::~DummyException() noexcept {}

} // namespace tividy