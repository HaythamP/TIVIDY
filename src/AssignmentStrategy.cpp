#include "AssignmentStrategy.h"
#include <string>

namespace tividy {

template <typename T>
AssignmentStrategy<T>::~AssignmentStrategy() {}

template <typename T>
bool AssignmentStrategy<T>::overridesSelection() const {
    return false;
}

template <typename T>
void AssignmentStrategy<T>::select(std::vector<Participant<T>*>& candidates, int numAssignees) const {
    if (numAssignees >= 0 && static_cast<std::size_t>(numAssignees) < candidates.size()) {
        candidates.resize(static_cast<std::size_t>(numAssignees));
    }
}

template class AssignmentStrategy<int>;
template class AssignmentStrategy<std::string>;

}