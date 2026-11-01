#ifndef DEPTHFIRSTITERATOR_H
#define DEPTHFIRSTITERATOR_H

#include <vector>
#include "WorkIterator.h"

namespace tividy {

/**
 * @brief ConcreteIterator: pre-order depth-first traversal of the workflow tree (a group, then its children, then the next sibling). Decorated items are reported as their outermost decorator. It never owns the components.
 */
template <typename T>
class DepthFirstIterator : public WorkIterator<T> {
public:
    /** @brief Creates the iterator over the given roots and positions it on the first component. */
    explicit DepthFirstIterator(const std::vector<WorkComponent<T>*>& roots);

    void first() override;
    void next() override;
    bool isDone() const override;
    WorkComponent<T>* getCurrent() const override;

private:
    DepthFirstIterator(const DepthFirstIterator&) = delete;
    DepthFirstIterator& operator=(const DepthFirstIterator&) = delete;

    /** @brief Moves to the next component using an explicit stack. */
    void advance();

    std::vector<WorkComponent<T>*> roots;  ///< Starting components.
    std::vector<WorkComponent<T>*> pending;  ///< Stack of components still to visit.
    WorkComponent<T>* current;  ///< Component under the cursor.
};

}

#endif