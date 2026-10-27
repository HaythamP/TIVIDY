#ifndef LEASTLOADEDASSIGNMENT_H
#define LEASTLOADEDASSIGNMENT_H

#include "AssignmentStrategy.h"

namespace tividy {

// STRATEGY: ConcreteStrategy. Keeps only the participants with the lowest workload.
template <typename IdType>
class LeastLoadedAssignment : public AssignmentStrategy<IdType> {
public:
    void filter(std::vector<Participant<IdType>*>& candidates) const override;
    AssignmentStrategy<IdType>* copy() const override;
};

} // namespace tividy

#endif