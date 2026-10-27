#ifndef STARTCOMMAND_H
#define STARTCOMMAND_H

#include "WorkCommand.h"

namespace tividy {

// COMMAND: ConcreteCommand. Assigned -> InProgress. Undo restores the memento.
template <typename IdType>
class StartCommand : public WorkCommand<IdType> {
public:
    explicit StartCommand(WorkComponent<IdType>* target);
    std::string getDescription() const override;
protected:
    void perform() override;
};

} // namespace tividy

#endif
