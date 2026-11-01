#include "ApprovalIncompleteException.h"

namespace tividy {

ApprovalIncompleteException::ApprovalIncompleteException(const std::string& message) : TividyException(message) {}

ApprovalIncompleteException::~ApprovalIncompleteException() noexcept {}

}