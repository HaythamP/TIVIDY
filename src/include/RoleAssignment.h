#ifndef ROLEASSIGNMENT_H
#define ROLEASSIGNMENT_H

#include <string>
#include "AssignmentStrategy.h"

namespace tividy {

template <typename T>
class RoleAssignment : public AssignmentStrategy<T> {
public:
    explicit RoleAssignment(const std::string& role);

    void filter(std::vector<Participant<T>*>& candidates) const override;

private:
    std::string role;
};

}

#endif