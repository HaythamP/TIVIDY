#ifndef APPROVECOMMAND_H
#define APPROVECOMMAND_H

#include <vector>
#include "WorkCommand.h"
#include "Participant.h"

namespace tividy {

template <typename T>
class ApproveCommand : public WorkCommand<T> {
public:
    ApproveCommand(WorkComponent<T>* target, const std::vector<Participant<T>*>& participants);
    std::string getDescription() const override;

protected:
    void perform() override;

private:
    std::vector<Participant<T>*> participants;
};

} // namespace tividy

#endif
