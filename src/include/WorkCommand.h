#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include "Command.h"
#include "WorkComponent.h"
#include "WorkMemento.h"

namespace tividy {

template <typename T>
class WorkCommand : public Command<T> {
public:
    ~WorkCommand();
    void execute() override;
    bool undo() override;

protected:
    explicit WorkCommand(WorkComponent<T>* target);

    virtual void perform() = 0;
    virtual bool canUndo() const;
    virtual std::vector<WorkComponent<T>*> getAffected() const;

    WorkComponent<T>* target;

private:
    WorkCommand(const WorkCommand&) = delete;
    WorkCommand& operator=(const WorkCommand&) = delete;

    std::vector<WorkComponent<T>*> affected;
    std::vector<WorkMemento<T>*> mementos;
};

} // namespace tividy

#endif
