#ifndef WORKCOMMAND_H
#define WORKCOMMAND_H

#include <string>
#include <vector>
#include "Command.h"
#include "WorkComponent.h"
#include "WorkMemento.h"
#include "Mediator.h"

namespace tividy {

/**
 * @brief AbstractClass of a Template Method, base of all work-item commands and Caretaker of the Memento pattern.
 *
 * execute() is the template: print the command, collect the affected components, take a memento of each, call the primitive perform(), record the resulting states and roll back if something throws. undo() is the second template: it checks canUndo() and restores the mementos in reverse order.
 * A command may be undone only if every affected component is still in the exact state the command left it, and the affected components include the successor items a completion may have released. Commands target WorkItems (decorated items included); a group target throws UnsupportedOperationException. The command owns its mementos; the target is a non-owning pointer.
 */
template <typename T>
class WorkCommand : public Command<T> {
public:
    /** @brief Deletes the mementos. */
    ~WorkCommand();
    /** @brief Template method: snapshot, perform(), record states, roll back on failure. */
    void execute() override;
    /**
     * @brief Template method: restores the snapshots if canUndo(); returns false (and prints why) otherwise.
     */
    bool undo() override;

protected:
    /** @brief @throws ComponentNotFoundException if the target is null. */
    explicit WorkCommand(WorkComponent<T>* target);

    /** @brief Primitive operation: the actual change on the target. */
    virtual void perform() = 0;
    /** @brief Hook: true when all affected components are still in the state the command produced. */
    virtual bool canUndo() const;
    /** @brief Hook: components to snapshot; by default only the target. */
    virtual std::vector<WorkComponent<T>*> getAffected() const;

    /** @brief Helper: the target plus the items of the next step, because completing can release them. */
    std::vector<WorkComponent<T>*> withSuccessors() const;

    WorkComponent<T>* target;  ///< Receiver (non-owning).

private:
    WorkCommand(const WorkCommand&) = delete;
    WorkCommand& operator=(const WorkCommand&) = delete;

    std::vector<WorkComponent<T>*> affected;  ///< Components snapshotted by execute().
    std::vector<WorkMemento<T>*> mementos;  ///< Owned snapshots, one per affected component.
    std::vector<std::string> resultingStates;  ///< State names recorded after perform().
};

}

#endif
