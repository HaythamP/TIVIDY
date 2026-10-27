#ifndef WORKITERATOR_H
#define WORKITERATOR_H

namespace tividy {

template <typename T> class WorkComponent;

/**
 * @brief Iterator of the Iterator pattern: traverses WorkComponents. Created by createIterator(); the caller deletes it.
 */
template <typename T>
class WorkIterator {
public:
    /** @brief Virtual destructor. */
    virtual ~WorkIterator();
    /** @brief Restarts the traversal. */
    virtual void first() = 0;
    /** @brief Advances to the next component. */
    virtual void next() = 0;
    /** @brief True when no components are left. */
    virtual bool isDone() const = 0;
    /** @brief Returns the current component (non-owning). */
    virtual WorkComponent<T>* getCurrent() const = 0;
};

}

#endif