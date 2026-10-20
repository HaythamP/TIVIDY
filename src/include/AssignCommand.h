#ifndef ASSIGNCOMMAND_H
#define ASSIGNCOMMAND_H

#include <vector>
#include "WorkCommand.h"
#include "Participant.h"

namespace tividy {

template <typename T>
class AssignCommand : public WorkCommand<T> {
public:
    AssignCommand(WorkComponent<T>* target, const std::vector<Participant<T>*>& participants, int numAssignees = 0);
    std::string getDescription() const override;

protected:
    void perform() override;

private:
    std::vector<Participant<T>*> participants;
    int numAssignees;
};

} // namespace tividy

#endif
