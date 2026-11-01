#ifndef WORKFLOWDEFINITION_H
#define WORKFLOWDEFINITION_H

#include <string>
#include <vector>
#include "WorkflowPrototype.h"
#include "WorkComponent.h"
#include "WorkVisitor.h"
#include "WorkIterator.h"
#include "Participant.h"
#include "Mediator.h"
#include "Command.h"
#include "CommandHistory.h"

namespace tividy {

/**
 * @brief ConcretePrototype, Product of the Builder pattern and ObjectStructure of the Visitor pattern: a complete workflow.
 *
 * It holds one ordered list of top-level steps (WorkItems, decorated items or WorkGroups; steps run one after another, children of a group in parallel), the pool of Participants, the Mediator and the CommandHistory. It owns the steps, the mediator and the history but not the participants.
 * Typical use: build a template once with a Builder, clone() it for every run, add participants, start(), then execute commands.
 * @tparam T type of component ids.
 */
template <typename T>
class WorkflowDefinition : public WorkflowPrototype<T> {
public:
    /** @brief Creates an empty workflow with its mediator and command history. */
    explicit WorkflowDefinition(const std::string& name);
    /** @brief Deletes history, steps and mediator (not the participants). */
    ~WorkflowDefinition();

    /**
     * @brief Deep copy of the steps; the participant pointers are shared and the history is empty.
     *
     * @throws NotPristineException if any item already progressed or has assignees.
     */
    WorkflowDefinition<T>* clone() const override;

    /** @brief Workflow name. */
    const std::string& getName() const;

    /**
     * @brief Adds a participant to the pool (non-owning, duplicates ignored). Strategy assignment always selects from this whole pool.
     */
    void addParticipant(Participant<T>* participant);
    /** @brief The participant pool. */
    const std::vector<Participant<T>*>& getParticipants() const;

    /**
     * @brief Appends a top-level step and takes ownership.
     *
     * @throws DuplicateIdException if an id already exists.
     */
    void addStep(WorkComponent<T>* step);
    /** @brief Top-level steps in order. */
    const std::vector<WorkComponent<T>*>& getSteps() const;
    /**
     * @brief Gives every step the mediator and registers the steps as its colleagues (safe to call repeatedly).
     */
    void wire();
    /** @brief Finds a component by id anywhere in the tree (outermost decorator for items), or null. */
    WorkComponent<T>* find(const T& id);

    /**
     * @brief Releases the first step.
     *
     * @throws InvalidWorkflowException if there are no steps.
     */
    void start();
    /** @brief Runs a command through the history (takes ownership). */
    void execute(Command<T>* command);
    /** @brief Undoes the newest command if allowed. */
    bool undoLast();
    /** @brief Descriptions of the executed commands. */
    std::vector<std::string> getLog() const;
    /** @brief True when every top-level step is complete. Rejected or cancelled items prevent this. */
    bool isComplete() const;

    /** @brief Lets a visitor traverse all steps. */
    void accept(WorkVisitor<T>& visitor);
    /** @brief Depth-first iterator over the whole tree; the caller deletes it. */
    WorkIterator<T>* createIterator();

private:
    WorkflowDefinition(const WorkflowDefinition&) = delete;
    WorkflowDefinition& operator=(const WorkflowDefinition&) = delete;

    std::string name;  ///< Workflow name.
    std::vector<WorkComponent<T>*> steps;  ///< Owned top-level steps.
    std::vector<Participant<T>*> participants;  ///< Non-owning participant pool.
    Mediator<T>* mediator;  ///< Owned mediator.
    CommandHistory<T>* history;  ///< Owned command history.
    bool wired;  ///< True once the mediator is connected.
};

}

#endif