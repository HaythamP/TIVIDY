#ifndef LEASTLOADEDASSIGNMENT_H
#define LEASTLOADEDASSIGNMENT_H

#include "AssignmentStrategy.h"

namespace tividy {

template <typename T>
class LeastLoadedAssignment : public AssignmentStrategy<T> {
public:
    void filter(std::vector<Participant<T>*>& candidates) const override;
};

}

#endif