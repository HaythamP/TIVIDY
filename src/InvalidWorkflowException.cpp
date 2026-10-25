#include "InvalidWorkflowException.h"

namespace tividy {

InvalidWorkflowException::InvalidWorkflowException(const std::string& message) : TividyException(message) {}

InvalidWorkflowException::~InvalidWorkflowException() noexcept {}

}