#include "RoleAssignment.h"
#include "InvalidAssignmentArgumentException.h"

namespace tividy {

template <typename T>
RoleAssignment<T>::RoleAssignment(const std::string& role) : role(role) {
    if (role.empty()) {
        throw InvalidAssignmentArgumentException("RoleAssignment requires a non-empty role");
    }
}

template <typename T>
void RoleAssignment<T>::filter(std::vector<Participant<T>*>& candidates) const {
    std::vector<Participant<T>*> kept;
    for (std::size_t i = 0; i < candidates.size(); ++i) {
        if (candidates[i]->hasRole(role)) {
            kept.push_back(candidates[i]);
        }
    }
    candidates.swap(kept);
}

template class RoleAssignment<int>;
template class RoleAssignment<std::string>;

}