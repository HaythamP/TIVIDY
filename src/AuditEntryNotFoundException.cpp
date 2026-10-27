#include "AuditEntryNotFoundException.h"

namespace tividy {

AuditEntryNotFoundException::AuditEntryNotFoundException(const std::string& message) : TividyException(message) {}

AuditEntryNotFoundException::~AuditEntryNotFoundException() noexcept {}

}