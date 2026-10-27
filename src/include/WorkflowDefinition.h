#ifndef TIVIDY_WORKFLOWDEFINITION_H
#define TIVIDY_WORKFLOWDEFINITION_H

#include <string>
#include <vector>
#include "WorkComponent.h"
#include "WorkVisitor.h"
#include "WorkIterator.h"
#include "Participant.h"
#include "Mediator.h"
#include "Command.h"
#include "CommandHistory.h"

namespace tividy {

// PROTOTYPE: Prototype and ConcretePrototype. clone() deep-copies the whole workflow,
// which is much cheaper than rebuilding a complex definition from scratch.
// (The copy() methods on components, strategies and handlers are only the deep-copy
// support that clone() relies on, not extra Prototype participants.)
//
// Structure: an ordered list of top-level steps (WorkItems or WorkGroups) linked via
// 'next'; inside a WorkGroup the children run in parallel.
//
// OWNS: steps, participants, mediator, command history (all deleted in the destructor).
// A clone gets its own copies, a new mediator, an empty history, and every item
// Created and unassigned.
template <typename IdType>
class WorkflowDefinition {
public:
    explicit WorkflowDefinition(const std::string& name);
    ~WorkflowDefinition();

    WorkflowDefinition<IdType>* clone() const;               // PROTOTYPE; caller owns the result

    const std::string& getName() const;

    // participants (the pool used by AssignmentStrategies)
    void addParticipant(Participant<IdType>* participant);   // takes ownership
    const std::vector<Participant<IdType>*>& getParticipants() const;

    // structure
    void addStep(WorkComponent<IdType>* step);               // takes ownership; appended to the sequence
    const std::vector<WorkComponent<IdType>*>& getSteps() const;
    void wire();                    // sets 'next' links between steps and the mediator on every component
    WorkComponent<IdType>* find(const IdType& id);

    // running the workflow
    void start();                                            // releases the first step
    void execute(Command<IdType>* command);                  // takes ownership; runs via the history
    bool undoLast();
    std::vector<std::string> getLog() const;
    bool isComplete() const;                                 // every step complete

    // traversal
    void accept(WorkVisitor<IdType>& visitor);
    WorkIterator<IdType>* createIterator();                  // depth first over all steps; caller owns

private:
    WorkflowDefinition(const WorkflowDefinition&) = delete;
    WorkflowDefinition& operator=(const WorkflowDefinition&) = delete;

    std::string name;
    std::vector<WorkComponent<IdType>*> steps;               // OWNED
    std::vector<Participant<IdType>*> participants;          // OWNED
    Mediator<IdType>* mediator;                              // OWNED
    CommandHistory<IdType>* history;                         // OWNED
};

} // namespace tividy

#endif