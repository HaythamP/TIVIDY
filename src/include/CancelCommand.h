#ifndef CANCELCOMMAND_H
#define CANCELCOMMAND_H

#include "WorkCommand.h"

namespace tividy {

/** @brief ConcreteCommand: cancels the target; the workflow halts. */
template <typename T>
class CancelCommand : public WorkCommand<T> {
public:
    /** @brief Creates the command for the target. */
    explicit CancelCommand(WorkComponent<T>* target);
    /** @brief Returns the description, for example the verb and the item name. */
    std::string getDescription() const override;

protected:
    /** @brief Calls the matching operation on the target. */
    void perform() override;
};

}

#endif
