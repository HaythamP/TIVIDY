#include "InvalidAssignmentArgumentException.h"

namespace tividy {

InvalidAssignmentArgumentException::InvalidAssignmentArgumentException(const std::string& message) : TividyException(message) {}

InvalidAssignmentArgumentException::~InvalidAssignmentArgumentException() noexcept {}

}