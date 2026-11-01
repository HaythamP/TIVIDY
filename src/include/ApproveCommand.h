#ifndef APPROVECOMMAND_H
#define APPROVECOMMAND_H

#include "WorkCommand.h"

namespace tividy {

/**
 * @brief ConcreteCommand: starts or advances the approval chain of the target; the last approver completes it.
 *
 * @throws UnsupportedOperationException when the target has no ApprovedWork.
 */
template <typename T>
class ApproveCommand : public WorkCommand<T> {
public:
    /** @brief Creates the command for the target. */
    explicit ApproveCommand(WorkComponent<T>* target);
    /** @brief Returns the description, for example the verb and the item name. */
    std::string getDescription() const override;

protected:
    /** @brief Calls the matching operation on the target. */
    void perform() override;
    /** @brief Target plus successor items. */
    std::vector<WorkComponent<T>*> getAffected() const override;
};

}

#endif
