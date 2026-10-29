#include "DepthFirstIterator.h"
#include "WorkComponent.h"
#include <string>

namespace tividy {

template <typename T>
DepthFirstIterator<T>::DepthFirstIterator(const std::vector<WorkComponent<T>*>& roots) : roots(roots), current(nullptr) {
    first();
}

template <typename T>
void DepthFirstIterator<T>::advance() {
    if (pending.empty()) {
        current = nullptr;
        return;
    }
    current = pending.back();
    pending.pop_back();
    std::vector<WorkComponent<T>*> children = current->getChildren();
    for (std::size_t i = children.size(); i > 0; --i) {
        pending.push_back(children[i - 1]);
    }
}

template <typename T>
void DepthFirstIterator<T>::first() {
    pending.clear();
    for (std::size_t i = roots.size(); i > 0; --i) {
        pending.push_back(roots[i - 1]);
    }
    advance();
}

template <typename T>
void DepthFirstIterator<T>::next() {
    advance();
}

template <typename T>
bool DepthFirstIterator<T>::isDone() const {
    return current == nullptr;
}

template <typename T>
WorkComponent<T>* DepthFirstIterator<T>::getCurrent() const {
    return current;
}

template class DepthFirstIterator<int>;
template class DepthFirstIterator<std::string>;

}
