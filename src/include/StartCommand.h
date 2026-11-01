#ifndef STARTCOMMAND_H
#define STARTCOMMAND_H

#include "WorkCommand.h"

namespace tividy {

/**
 * @brief ConcreteCommand: starts the target (Assigned to InProgress). It also restarts an item rejected from Approval.
 */
template <typename T>
class StartCommand : public WorkCommand<T> {
public:
    /** @brief Creates the command for the target. */
    explicit StartCommand(WorkComponent<T>* target);
    /** @brief Returns the description, for example the verb and the item name. */
    std::string getDescription() const override;

protected:
    /** @brief Calls the matching operation on the target. */
    void perform() override;
};

}

#endif