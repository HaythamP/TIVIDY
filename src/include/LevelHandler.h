#ifndef LEVELHANDLER_H
#define LEVELHANDLER_H

#include "Handler.h"
#include "Participant.h"

namespace tividy {

template <typename T>
class LevelHandler : public Handler<T> {
public:
    explicit LevelHandler(Participant<T>* participant);

protected:
    bool canHandle(const WorkComponent<T>& work) const override;
    bool process(WorkComponent<T>& work) override;

private:
    Participant<T>* participant;
};

} // namespace tividy

#endif