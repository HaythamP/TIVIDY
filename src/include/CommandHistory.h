#ifndef TIVIDY_COMMANDHISTORY_H
#define TIVIDY_COMMANDHISTORY_H

#include <string>
#include <vector>
#include "Command.h"

namespace tividy {

// COMMAND: Invoker. OWNS every command it has run (deleted in the destructor).
// The list of descriptions doubles as a simple audit trail of the workflow.
template <typename IdType>
class CommandHistory {
public:
    CommandHistory();
    ~CommandHistory();

    void run(Command<IdType>* command);              // takes ownership; executes and stores it
    bool undoLast();                                 // false if empty or the command refuses to undo
    std::vector<std::string> getDescriptions() const;

private:
    CommandHistory(const CommandHistory&) = delete;
    CommandHistory& operator=(const CommandHistory&) = delete;

    std::vector<Command<IdType>*> executed;          // OWNED
};

} // namespace tividy

#endif
