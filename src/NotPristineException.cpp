#include "NotPristineException.h"

namespace tividy {

NotPristineException::NotPristineException(const std::string& message) : TividyException(message) {}

NotPristineException::~NotPristineException() noexcept {}

}