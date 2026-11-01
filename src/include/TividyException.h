#ifndef TIVIDYEXCEPTION_H
#define TIVIDYEXCEPTION_H

#include <exception>
#include <string>

namespace tividy {

class TividyException : public std::exception {
public:
    explicit TividyException(const std::string& message);
    virtual ~TividyException() noexcept;

    const char* what() const noexcept override;
    const std::string& getMessage() const;

private:
    std::string message;
};

}

#endif