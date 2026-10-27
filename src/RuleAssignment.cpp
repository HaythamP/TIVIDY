#include "RuleAssignment.h"
#include <algorithm>
#include <random>
#include <string>

namespace tividy {

template <typename T>
void RuleAssignment<T>::filter(std::vector<Participant<T>*>& candidates) const {}

template <typename T>
bool RuleAssignment<T>::overridesSelection() const {
    return true;
}

template <typename T>
void RuleAssignment<T>::select(std::vector<Participant<T>*>& candidates, int numAssignees) const {
    static std::mt19937 engine((std::random_device()()));
    std::shuffle(candidates.begin(), candidates.end(), engine);
    if (numAssignees >= 0 && static_cast<std::size_t>(numAssignees) < candidates.size()) {
        candidates.resize(static_cast<std::size_t>(numAssignees));
    }
}

template class RuleAssignment<int>;
template class RuleAssignment<std::string>;

}