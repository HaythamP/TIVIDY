#ifndef ROLEASSIGNMENT_H
#define ROLEASSIGNMENT_H

#include <string>
#include "AssignmentStrategy.h"

namespace tividy {

/** @brief ConcreteStrategy: keeps only participants that have a given role. */
template <typename T>
class RoleAssignment : public AssignmentStrategy<T> {
public:
    /** @brief @throws InvalidAssignmentArgumentException if the role is empty. */
    explicit RoleAssignment(const std::string& role);

    /** @brief Removes participants without the role. */
    void filter(std::vector<Participant<T>*>& candidates) const override;

private:
    std::string role;  ///< Required role.
};

}

#endif