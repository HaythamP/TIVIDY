#ifndef INSUFFICIENTPARTICIPANTSEXCEPTION_H
#define INSUFFICIENTPARTICIPANTSEXCEPTION_H

#include <string>
#include "TividyException.h"

namespace tividy {

class InsufficientParticipantsException : public TividyException {
public:
    explicit InsufficientParticipantsException(const std::string& message);
    ~InsufficientParticipantsException() noexcept;
};

}

#endif