#ifndef REJECTEDSTATE_H
#define REJECTEDSTATE_H

#include "WorkItemState.h"

namespace tividy {

/**
 * @brief ConcreteState: the item failed. The workflow halts because nothing after a rejected item is released.
 *
 * Rejected is terminal unless it was reached from Approval: such an item is restartable and the Start command moves it back to InProgress (InProgress, Approval, Rejected, InProgress).
 */
template <typename T>
class RejectedState : public WorkItemState<T> {
public:
    /** @brief Creates the state; restartable is true only when the rejection came from Approval. */
    explicit RejectedState(bool restartable = false);

    std::string getName() const override;
    WorkItemState<T>* copy() const override;
    bool isTerminal() const override;
    /** @brief True if the Start command may restart the item. */
    bool isRestartable() const;
    /**
     * @brief Moves back to InProgress when restartable.
     *
     * @throws InvalidTransitionException otherwise.
     */
    void start(WorkItem<T>& item) override;

private:
    bool restartable;  ///< True when rejected from Approval.
};

}

#endif

