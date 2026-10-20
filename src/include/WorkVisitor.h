#ifndef WORKVISITOR_H
#define WORKVISITOR_H

namespace tividy {
    template <typename IdType> class WorkItem;
    template <typename IdType> class WorkGroup;
    template <typename IdType>
class WorkVisitor {
public:
    virtual ~WorkVisitor();
    virtual void visit(WorkItem<IdType>& item) = 0;
    virtual void visit(WorkGroup<IdType>& group) = 0;
};
}
#endif