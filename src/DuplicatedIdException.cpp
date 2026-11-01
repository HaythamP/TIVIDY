#include "DuplicateIdException.h"

namespace tividy {

DuplicateIdException::DuplicateIdException(const std::string& message) : TividyException(message) {}

DuplicateIdException::~DuplicateIdException() noexcept {}

}