#ifndef AUDITENTRYNOTFOUNDEXCEPTION_H
#define AUDITENTRYNOTFOUNDEXCEPTION_H

#include <string>
#include "TividyException.h"

namespace tividy {


class AuditEntryNotFoundException : public TividyException {
public:
    
    explicit AuditEntryNotFoundException(const std::string& message);
    ~AuditEntryNotFoundException() noexcept;
};

}

#endif