#ifndef FALLBACKHANDLER_H
#define FALLBACKHANDLER_H

#include "Handler.h"
#include "Participant.h"

namespace tividy {

template <typename T>
class FallbackHandler : public Handler<T> {
public:
    explicit FallbackHandler(Participant<T>* participant);

protected:
    bool canHandle(const WorkComponent<T>& work) const override;
    bool process(WorkComponent<T>& work) override;

private:
    Participant<T>* participant;
};

} // namespace tividy

#endif