#ifndef WORKGROUP_H
#define WORKGROUP_H

#include <string>
#include <vector>
#include "WorkComponent.h"

namespace tividy {

/**
 * @brief Composite of the Composite pattern: a stage that contains other components.
 *
 * All components directly inside a group run in parallel; the group is complete once every child is complete. Sequencing between steps is handled by the Mediator. A group owns and deletes its children and rejects item-only operations.
 * Visitors see a group once, followed by its children.
 */
template <typename T>
class WorkGroup : public WorkComponent<T> {
public:
    /** @brief Creates an empty group with an id and a name. */
    WorkGroup(const T& id, const std::string& name);
    /** @brief Deletes all children. */
    ~WorkGroup();

    const T& getId() const override;
    const std::string& getName() const override;

    /** @brief Stores the mediator and propagates it to every child. */
    void setMediator(Mediator<T>* mediator) override;
    std::vector<WorkComponent<T>*> getChildren() const override;
    std::vector<WorkComponent<T>*> getItems() override;
    /** @brief Takes ownership of the child, sets its parent and gives it the group's mediator. */
    void add(WorkComponent<T>* child) override;
    WorkComponent<T>* find(const T& id) override;

    /** @brief Releases every child (parallel start). */
    void release() override;
    /** @brief True when the group has children and all of them are complete. */
    bool isComplete() const override;
    /** @brief True when at least one child is available. */
    bool isAvailable() const override;

    /** @brief Deep copies the group and all its children. */
    WorkComponent<T>* copy() const override;
    /** @brief Visits the group, then each child. */
    void accept(WorkVisitor<T>& visitor) override;
    WorkIterator<T>* createIterator() override;

private:
    T id;  ///< Group id.
    std::string name;  ///< Group name.
    std::vector<WorkComponent<T>*> children;  ///< Owned children (items, decorated items or nested groups).
};

}

#endif
