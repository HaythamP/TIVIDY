#ifndef WORKVISITOR_H
#define WORKVISITOR_H

namespace tividy {

template <typename T> class WorkItem;
template <typename T> class WorkGroup;
template <typename T> class AuditedWork;
template <typename T> class EscalatedWork;
template <typename T> class ApprovedWork;

/**
 * @brief Visitor of the Visitor pattern: one visit() overload for every concrete element (WorkItem, WorkGroup and the three decorators), so decoration is visible to visitors. Elements call it from accept().
 */
template <typename T>
class WorkVisitor {
public:
    /** @brief Virtual destructor. */
    virtual ~WorkVisitor();
    /** @brief Visits one concrete element. */
    virtual void visit(WorkItem<T>& item) = 0;
    /** @brief Visits one concrete element. */
    virtual void visit(WorkGroup<T>& group) = 0;
    /** @brief Visits one concrete element. */
    virtual void visit(AuditedWork<T>& decorator) = 0;
    /** @brief Visits one concrete element. */
    virtual void visit(EscalatedWork<T>& decorator) = 0;
    /** @brief Visits one concrete element. */
    virtual void visit(ApprovedWork<T>& decorator) = 0;
};

}

#endif