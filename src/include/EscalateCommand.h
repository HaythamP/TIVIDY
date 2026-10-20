#ifndef ESCALATECOMMAND_H
#define ESCALATECOMMAND_H

#include <vector>
#include "WorkCommand.h"
#include "Participant.h"

namespace tividy {

template <typename T>
class EscalateCommand : public WorkCommand<T> {
public:
    EscalateCommand(WorkComponent<T>* target, const std::vector<Participant<T>*>& participants);
    std::string getDescription() const override;

protected:
    void perform() override;

private:
    std::vector<Participant<T>*> participants;
};

} // namespace tividy

#endif
