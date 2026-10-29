#ifndef ESCALATECOMMAND_H
#define ESCALATECOMMAND_H

#include "WorkCommand.h"

namespace tividy {

/**
 * @brief ConcreteCommand: starts or advances the escalation chain of the target.
 *
 * @throws UnsupportedOperationException when the target has no EscalatedWork.
 */
template <typename T>
class EscalateCommand : public WorkCommand<T> {
public:
    /** @brief Creates the command for the target. */
    explicit EscalateCommand(WorkComponent<T>* target);
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
