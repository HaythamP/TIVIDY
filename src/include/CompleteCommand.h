#ifndef COMPLETECOMMAND_H
#define COMPLETECOMMAND_H

#include "WorkCommand.h"
#include "Mediator.h"

namespace tividy {

template <typename T>
class CompleteCommand : public WorkCommand<T> {
public:
    explicit CompleteCommand(WorkComponent<T>* target);
    std::string getDescription() const override;

protected:
    void perform() override;
    bool canUndo() const override;
    std::vector<WorkComponent<T>*> getAffected() const override;
};

} // namespace tividy

#endif
