#ifndef RULEASSIGNMENT_H
#define RULEASSIGNMENT_H

#include "AssignmentStrategy.h"

namespace tividy {

template <typename T>
class RuleAssignment : public AssignmentStrategy<T> {
public:
    void filter(std::vector<Participant<T>*>& candidates, int numAssignees) const override;
    AssignmentStrategy<T>* copy() const override;
};

} // namespace tividy

#endif