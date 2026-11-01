#ifndef HANDLER_H
#define HANDLER_H

namespace tividy {

template <typename T> class WorkComponent;
template <typename T> class Participant;

/**
 * @brief Handler of the Chain of Responsibility pattern, also the AbstractClass of a Template Method.
 *
 * handle() is the template method: it asks canHandle(); if true it runs process() and returns itself, otherwise it forwards to the successor. copy() is a second template method (cloneSelf() plus a deep copy of the rest of the chain).
 * Deviation from the textbook: the request carries a position and the decorators advance it one handler per user command (Escalate or Approve), so a chain is walked over several commands. Each handler owns its successor, so deleting the head deletes the chain.
 */
template <typename T>
class Handler {
public:
    /** @brief Creates a handler without a successor. */
    Handler();
    /** @brief Deletes the successor (and therefore the rest of the chain). */
    virtual ~Handler();

    /** @brief Sets the successor and takes ownership (deleting a previous different successor). */
    void setNext(Handler<T>* next);
    /** @brief Returns the successor (non-owning view). */
    Handler<T>* getNext() const;

    /**
     * @brief Template method: lets the first handler whose canHandle() accepts the position process the work. Returns that handler or null.
     */
    Handler<T>* handle(WorkComponent<T>& work, int position);
    /** @brief Template method: deep copy of this handler and all successors. */
    Handler<T>* copy() const;

    /** @brief Returns the participant behind the handler (null for the fallback). */
    virtual Participant<T>* getParticipant() const;

protected:
    /** @brief Primitive operation: is this handler responsible for the position? */
    virtual bool canHandle(const WorkComponent<T>& work, int position) const = 0;
    /** @brief Primitive operation: performs the handling. */
    virtual void process(WorkComponent<T>& work) = 0;
    /** @brief Primitive operation: copies only this handler. */
    virtual Handler<T>* cloneSelf() const = 0;

private:
    Handler(const Handler&) = delete;
    Handler& operator=(const Handler&) = delete;

    Handler<T>* next;  ///< Owned successor.
};

}

#endif