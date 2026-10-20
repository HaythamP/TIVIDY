#ifndef COMMAND_H
#define COMMAND_H

#include <string>

namespace tividy {

// COMMAND: Command.
template <typename IdType>
class Command {
public:
    virtual ~Command();
    virtual void execute() = 0;
    virtual bool undo() = 0;                       // false if the command cannot be undone now
    virtual std::string getDescription() const = 0;
};

} // namespace tividy

#endif
