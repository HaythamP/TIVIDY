#ifndef WORKITERATOR_H
#define WORKITERATOR_H

namespace tividy {
    template <typename IdType> class WorkComponent;
// the CALLER deletes the iterator
template <typename IdType>
class WorkIterator {
public:
    virtual ~WorkIterator();
    virtual void first() = 0;
    virtual void next() = 0;
    virtual bool isDone() const = 0;
    virtual WorkComponent<IdType>* getCurrent() const = 0;// non-owning
};
}
#endif