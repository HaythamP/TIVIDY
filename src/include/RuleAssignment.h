#ifndef RULEASSIGNMENT_H
#define RULEASSIGNMENT_H

#include "AssignmentStrategy.h"

namespace tividy {

template <typename T>
class RuleAssignment : public AssignmentStrategy<T> {
public:
    void filter(std::vector<Participant<T>*>& candidates) const override;
    bool overridesSelection() const override;
    void select(std::vector<Participant<T>*>& candidates, int numAssignees) const override;
};

}

#endif