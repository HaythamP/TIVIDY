#include "ComponentNotFoundException.h"

namespace tividy {

ComponentNotFoundException::ComponentNotFoundException(const std::string& message) : TividyException(message) {}

ComponentNotFoundException::~ComponentNotFoundException() noexcept {}

}