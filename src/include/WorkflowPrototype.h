#ifndef WORKFLOWPROTOTYPE_H
#define WORKFLOWPROTOTYPE_H

namespace tividy {

template <typename T>
class WorkflowPrototype {
public:
    virtual ~WorkflowPrototype() = default;
    virtual WorkflowPrototype<T>* clone() const = 0;
};

}

#endif
