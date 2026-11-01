#ifndef COMMANDHISTORY_H
#define COMMANDHISTORY_H

#include <string>
#include <vector>
#include "Command.h"

namespace tividy {

/**
 * @brief Invoker of the Command pattern: runs commands and remembers them in order so they can be undone, newest first (multi-level undo). It owns every command it accepted.
 */
template <typename T>
class CommandHistory {
public:
    /** @brief Creates an empty history. */
    CommandHistory();
    /** @brief Deletes all remembered commands. */
    ~CommandHistory();

    /**
     * @brief Takes ownership of the command and executes it; if execution throws the command is deleted and the exception propagates.
     */
    void run(Command<T>* command);
    /**
     * @brief Undoes the most recent command. A successful undo removes it; a refused undo leaves it on the stack and returns false.
     */
    bool undoLast();
    /** @brief Returns the descriptions of the executed commands, oldest first. */
    std::vector<std::string> getDescriptions() const;

private:
    CommandHistory(const CommandHistory&) = delete;
    CommandHistory& operator=(const CommandHistory&) = delete;

    std::vector<Command<T>*> executed;  ///< Owned commands, oldest first.
};

}

#endif
