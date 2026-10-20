#ifndef TIVIDY_DEPTHFIRSTITERATOR_H
#define TIVIDY_DEPTHFIRSTITERATOR_H

#include <vector>
#include "WorkIterator.h"

namespace tividy {
    template <typename IdType> class WorkComponent;
    template <typename IdType>
class DepthFirstIterator : public WorkIterator<IdType> {
public:
    explicit DepthFirstIterator(const std::vector<WorkComponent<IdType>*>& roots);
    void first() override;// reset to the first root
    void next() override;
    bool isDone() const override;
    WorkComponent<IdType>* getCurrent() const override;
private:
    std::vector<WorkComponent<IdType>*> roots;
    std::vector<WorkComponent<IdType>*> pending;
    WorkComponent<IdType>* current;
};
}
#endif