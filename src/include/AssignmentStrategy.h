#ifndef ASSIGNMENTSTRATEGY_H
#define ASSIGNMENTSTRATEGY_H

#include <vector>
#include "Participant.h"

namespace tividy {

/**
 * @brief Strategy of the Strategy pattern: one rule that narrows the candidates for an assignment.
 *
 * WorkItem::assignWork() is the Context: it applies the supplied strategies in list order, each one filtering the result of the previous one, and then selects the assignees. Strategies are passed per assignment and are never stored; the caller owns them.
 * Strategies are used when assigning from the workflow's whole participant pool. If you want to choose participants yourself, pass an explicit participant list instead and no strategies.
 */
template <typename T>
class AssignmentStrategy {
public:
    /** @brief Virtual destructor. */
    virtual ~AssignmentStrategy();

    /** @brief Narrows (or reorders) the candidate vector in place. */
    virtual void filter(std::vector<Participant<T>*>& candidates) const = 0;
    /** @brief True if the strategy replaces the default final selection (first n). */
    virtual bool overridesSelection() const;
    /**
     * @brief Reduces the candidates to numAssignees (-1 keeps everyone). The default keeps the first ones.
     */
    virtual void select(std::vector<Participant<T>*>& candidates, int numAssignees) const;
};

}

#endif