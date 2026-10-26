#include "InsufficientParticipantsException.h"

namespace tividy {

InsufficientParticipantsException::InsufficientParticipantsException(const std::string& message) : TividyException(message) {}

InsufficientParticipantsException::~InsufficientParticipantsException() noexcept {}

}