#ifndef MEDIATOR_H
#define MEDIATOR_H

namespace tividy {

template <typename T> class WorkComponent;

/**
 * @brief Mediator of the Mediator pattern: the single object that WorkComponents talk to when they change state, so they never reference each other.
 */
template <typename T>
class Mediator {
public:
    /** @brief Virtual destructor. */
    virtual ~Mediator() = default;
    /** @brief A colleague reports that it completed. */
    virtual void notify(WorkComponent<T>* sender) = 0;
    /** @brief Returns the top-level step following the sender's step, or null. */
    virtual WorkComponent<T>* findNext(WorkComponent<T>* sender) const = 0;
    /** @brief Registers a top-level step. */
    virtual void addColleague(WorkComponent<T>* colleague) = 0;
};

}

#endif