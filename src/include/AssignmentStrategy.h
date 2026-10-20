#ifndef ASSIGNMENTSTRATEGY_H
#define ASSIGNMENTSTRATEGY_H

#include <vector>
#include "Participant.h"

namespace tividy {

template <typename T>
class AssignmentStrategy {
public:
    virtual ~AssignmentStrategy();
    virtual void filter(std::vector<Participant<T>*>& candidates, int numAssignees) const = 0;
    virtual AssignmentStrategy<T>* copy() const = 0;
};

} // namespace tividy

#endif