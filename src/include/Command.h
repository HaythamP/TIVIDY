#ifndef COMMAND_H
#define COMMAND_H

#include <string>

namespace tividy {

/**
 * @brief Command of the Command pattern: a request to change a workflow, packaged as an object so it can be executed, logged and undone.
 */
template <typename T>
class Command {
public:
    /** @brief Virtual destructor. */
    virtual ~Command();
    /** @brief Performs the request. */
    virtual void execute() = 0;
    /** @brief Reverts the request if allowed; returns false when the undo is refused. */
    virtual bool undo() = 0;
    /** @brief Short text such as "Complete Repair". */
    virtual std::string getDescription() const = 0;
};

}

#endif
