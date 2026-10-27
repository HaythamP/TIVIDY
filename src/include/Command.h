#ifndef COMMAND_H
#define COMMAND_H

#include <string>

namespace tividy {


template <typename T>
class Command {
public:
    
    virtual ~Command();
    
    virtual void execute() = 0;
    
    virtual bool undo() = 0;
    
    virtual std::string getDescription() const = 0;
};

}

#endif
