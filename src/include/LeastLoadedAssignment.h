#ifndef LEASTLOADEDASSIGNMENT_H
#define LEASTLOADEDASSIGNMENT_H

#include "AssignmentStrategy.h"

namespace tividy {

/**
 * @brief ConcreteStrategy: stable-sorts the candidates by ascending workload without removing anyone, so the default selection picks the least loaded participants.
 */
template <typename T>
class LeastLoadedAssignment : public AssignmentStrategy<T> {
public:
    /** @brief Stable sort by workload, lowest first. */
    void filter(std::vector<Participant<T>*>& candidates) const override;
};

}

#endif