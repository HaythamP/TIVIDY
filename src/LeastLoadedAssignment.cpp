#include "LeastLoadedAssignment.h"
#include <algorithm>
#include <string>

namespace tividy {

namespace {

template <typename T>
bool lighterLoad(const Participant<T>* a, const Participant<T>* b) {
    return a->getWorkload() < b->getWorkload();
}

}

template <typename T>
void LeastLoadedAssignment<T>::filter(std::vector<Participant<T>*>& candidates) const {
    std::stable_sort(candidates.begin(), candidates.end(), lighterLoad<T>);
}

template class LeastLoadedAssignment<int>;
template class LeastLoadedAssignment<std::string>;

}