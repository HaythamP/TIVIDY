#ifndef RULEASSIGNMENT_H
#define RULEASSIGNMENT_H

#include "AssignmentStrategy.h"

namespace tividy {

/**
 * @brief ConcreteStrategy: selects the final assignees at random. It does not filter; it replaces the default selection (overridesSelection() is true).
 */
template <typename T>
class RuleAssignment : public AssignmentStrategy<T> {
public:
    /** @brief Does nothing. */
    void filter(std::vector<Participant<T>*>& candidates) const override;
    /** @brief Always true. */
    bool overridesSelection() const override;
    /** @brief Shuffles the candidates and keeps numAssignees (-1 keeps all). */
    void select(std::vector<Participant<T>*>& candidates, int numAssignees) const override;
};

}

#endif