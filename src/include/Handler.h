#ifndef HANDLER_H
#define HANDLER_H

namespace tividy {

template <typename T> class WorkComponent;


template <typename T>
class Handler {
public:
    Handler();
    virtual ~Handler();

    void setNext(Handler<T>* next);
    Handler<T>* getNext() const;
    Handler<T>* handle(WorkComponent<T>& work);

protected:
    virtual bool canHandle(const WorkComponent<T>& work) const = 0;
    virtual bool process(WorkComponent<T>& work) = 0;

private:
    Handler(const Handler&) = delete;
    Handler& operator=(const Handler&) = delete;

    Handler<T>* next;
};

} // namespace tividy

#endif