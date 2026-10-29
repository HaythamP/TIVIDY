#ifndef PROGRESSREPORT_H
#define PROGRESSREPORT_H

#include <map>
#include <string>
#include "WorkVisitor.h"

namespace tividy {

/**
 * @brief ConcreteVisitor: counts items and tallies items per state, then summarises how far the workflow is (for example `3/5 items completed (Completed: 3, ...)`). It computes a snapshot on demand and stores nothing in the tree; compare AuditedWork (history of one item) and TreePrinter (shows the structure).
 */
template <typename T>
class ProgressReport : public WorkVisitor<T> {
public:
    /** @brief Creates an empty report. */
    ProgressReport();

    /** @brief Counts the item and its state. */
    void visit(WorkItem<T>& item) override;
    /** @brief Counts the item and its state. */
    void visit(WorkGroup<T>& group) override;
    /** @brief Counts the item and its state. */
    void visit(AuditedWork<T>& decorator) override;
    /** @brief Counts the item and its state. */
    void visit(EscalatedWork<T>& decorator) override;
    /** @brief Counts the item and its state. */
    void visit(ApprovedWork<T>& decorator) override;

    /** @brief Items visited. */
    int getTotalItems() const;
    /** @brief Items that are Completed. */
    int getCompletedItems() const;
    /** @brief Returns the one-line summary. */
    std::string getSummary() const;

private:
    int totalItems;  ///< Items visited.
    int completedItems;  ///< Completed items.
    std::map<std::string, int> stateCounts;  ///< Number of items per state name.
};

}

#endif