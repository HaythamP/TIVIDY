#include "UnsupportedOperationException.h"

namespace tividy {

UnsupportedOperationException::UnsupportedOperationException(const std::string& message) : TividyException(message) {}

UnsupportedOperationException::~UnsupportedOperationException() noexcept {}

}