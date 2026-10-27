#ifndef HANDLER_H
#define HANDLER_H

namespace tividy {

template <typename T> class WorkComponent;
template <typename T> class Participant;

template <typename T>
class Handler {
public:
    Handler();
    virtual ~Handler();

    void setNext(Handler<T>* next);
    Handler<T>* getNext() const;

    Handler<T>* handle(WorkComponent<T>& work, int position);
    Handler<T>* copy() const;

    virtual Participant<T>* getParticipant() const;

protected:
    virtual bool canHandle(const WorkComponent<T>& work, int position) const = 0;
    virtual void process(WorkComponent<T>& work) = 0;
    virtual Handler<T>* cloneSelf() const = 0;

private:
    Handler(const Handler&) = delete;
    Handler& operator=(const Handler&) = delete;

    Handler<T>* next;
};

}

#endif