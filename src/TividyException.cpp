#include "TividyException.h"

namespace tividy {

TividyException::TividyException(const std::string& message) : message(message) {}

TividyException::~TividyException() noexcept {}

const char* TividyException::what() const noexcept {
    return message.c_str();
}

const std::string& TividyException::getMessage() const {
    return message;
}

}