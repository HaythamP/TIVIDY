#ifndef AUDITEDWORK_H
#define AUDITEDWORK_H

#include <string>
#include <vector>
#include "WorkItemDecorator.h"

namespace tividy {

/**
 * @brief ConcreteDecoratorB (behavioural decorator): keeps a history of what happened to one work item.
 *
 * After every successful operation (release, assign, start, complete, reject, cancel, escalate, approve) an entry such as `start: Assigned -> InProgress` is appended; undoing a command appends an `undone` entry. Users can also add, edit and delete entries by hand.
 * Difference to ProgressReport: AuditedWork is a per-item, stored history over time that belongs to the item; ProgressReport is a visitor that computes a one-off summary of the whole workflow's current state and stores nothing in the tree.
 */
template <typename T>
class AuditedWork : public WorkItemDecorator<T> {
public:
    /** @brief Wraps the component with an empty log. */
    explicit AuditedWork(WorkComponent<T>* inner);

    /** @brief Forwards and logs `release` when the state changed. */
    void release() override;
    /** @brief Forwards and logs `assign`. */
    void assignTo(const std::vector<Participant<T>*>& participants) override;
    void start() override;
    void complete() override;
    void reject() override;
    void cancel() override;
    void escalate() override;
    void approve() override;
    /** @brief Forwards and logs `undone`. */
    void restore(const WorkMemento<T>& memento) override;

    /** @brief Returns a copy of the log. */
    std::vector<std::string> getAuditLog() const override;
    /** @brief Appends a free text entry. */
    void addAuditEntry(const std::string& entry) override;
    /**
     * @brief Replaces the entry at the index.
     *
     * @throws AuditEntryNotFoundException if the index is out of range.
     */
    void editAuditEntry(std::size_t index, const std::string& entry) override;
    /**
     * @brief Removes the entry at the index.
     *
     * @throws AuditEntryNotFoundException if the index is out of range.
     */
    void deleteAuditEntry(std::size_t index) override;

    /** @brief Copies the wrapped component; the copy starts with an empty log. */
    WorkComponent<T>* copy() const override;
    /** @brief Visits the decorator, then the wrapped component. */
    void accept(WorkVisitor<T>& visitor) override;

private:
    /** @brief Appends `operation: before -> after` using the current state name. */
    void record(const std::string& operation, const std::string& before);

    std::vector<std::string> auditLog;  ///< Chronological, user-editable log entries.
};

template class AuditedWork<int>;
template class AuditedWork<std::string>;
}

#endif
