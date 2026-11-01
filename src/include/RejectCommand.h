#ifndef REJECTCOMMAND_H
#define REJECTCOMMAND_H

#include "WorkCommand.h"

namespace tividy {

/** @brief ConcreteCommand: rejects (fails) the target; the workflow halts. */
template <typename T>
class RejectCommand : public WorkCommand<T> {
public:
    /** @brief Creates the command for the target. */
    explicit RejectCommand(WorkComponent<T>* target);
    /** @brief Returns the description, for example the verb and the item name. */
    std::string getDescription() const override;

protected:
    /** @brief Calls the matching operation on the target. */
    void perform() override;
};

}

#endif
