#ifndef PROGRESSREPORT_H
#define PROGRESSREPORT_H

#include <map>
#include <string>
#include "WorkVisitor.h"

namespace tividy {
    template <typename IdType>
class ProgressReport : public WorkVisitor<IdType> {
public:
    ProgressReport();
    void visit(WorkItem<IdType>& item) override;
    void visit(WorkGroup<IdType>& group) override;

    int getTotalItems() const;
    int getCompletedItems() const;
    std::string getSummary() const;
private:
    int totalItems;
    int completedItems;
    std::map<std::string, int> stateCounts;
};
}
#endif