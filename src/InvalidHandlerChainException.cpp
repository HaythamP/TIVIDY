#include "InvalidHandlerChainException.h"

namespace tividy {

InvalidHandlerChainException::InvalidHandlerChainException(const std::string& message) : TividyException(message) {}

InvalidHandlerChainException::~InvalidHandlerChainException() noexcept {}

}