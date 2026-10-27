#ifndef ROLEASSIGNMENT_H
#define ROLEASSIGNMENT_H

#include <string>
#include "AssignmentStrategy.h"

namespace tividy {

// STRATEGY: ConcreteStrategy. Keeps only participants that hold the required role.
template <typename IdType>
class RoleAssignment : public AssignmentStrategy<IdType> {
public:
    explicit RoleAssignment(const std::string& role);
    void filter(std::vector<Participant<IdType>*>& candidates) const override;
    AssignmentStrategy<IdType>* copy() const override;
private:
    std::string role;
};

} // namespace tividy

#endif