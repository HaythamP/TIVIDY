#ifndef COMPLETECOMMAND_H
#define COMPLETECOMMAND_H

#include "WorkCommand.h"

namespace tividy {

/**
 * @brief ConcreteCommand: completes the target. An escalating item stops its chain; an approved item must be fully approved first. Successor items are included in the affected set.
 */
template <typename T>
class CompleteCommand : public WorkCommand<T> {
public:
    /** @brief Creates the command for the target. */
    explicit CompleteCommand(WorkComponent<T>* target);
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
