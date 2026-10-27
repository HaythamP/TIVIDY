#ifndef LEVELHANDLER_H
#define LEVELHANDLER_H

#include "Handler.h"
#include "Participant.h"

namespace tividy {

template <typename T>
class LevelHandler : public Handler<T> {
public:
    LevelHandler(Participant<T>* participant, int level);

    Participant<T>* getParticipant() const override;
    int getLevel() const;

protected:
    bool canHandle(const WorkComponent<T>& work, int position) const override;
    void process(WorkComponent<T>& work) override;
    Handler<T>* cloneSelf() const override;

private:
    Participant<T>* participant;
    int level;
};

}

#endif