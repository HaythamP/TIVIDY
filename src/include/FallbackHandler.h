#ifndef FALLBACKHANDLER_H
#define FALLBACKHANDLER_H

#include "Handler.h"

namespace tividy {

/**
 * @brief ConcreteHandler that always ends an escalation chain. It handles every request that no level handled, prints a message and completes the item (without asking anybody). It is never part of an approval chain.
 */
template <typename T>
class FallbackHandler : public Handler<T> {
public:
    FallbackHandler();

protected:
    /** @brief Always true. */
    bool canHandle(const WorkComponent<T>& work, int position) const override;
    /** @brief Completes the work item (or hands it to approval, see EscalatedWork). */
    void process(WorkComponent<T>& work) override;
    Handler<T>* cloneSelf() const override;
};

}

#endif