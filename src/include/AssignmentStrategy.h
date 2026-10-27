#ifndef ASSIGNMENTSTRATEGY_H
#define ASSIGNMENTSTRATEGY_H

#include <vector>
#include "Participant.h"

namespace tividy {

template <typename T>
class AssignmentStrategy {
public:
    virtual ~AssignmentStrategy();

    virtual void filter(std::vector<Participant<T>*>& candidates) const = 0;
    virtual bool overridesSelection() const;
    virtual void select(std::vector<Participant<T>*>& candidates, int numAssignees) const;
};

}

#endif