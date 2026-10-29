#include "LevelHandler.h"
#include "WorkComponent.h"
#include "InvalidHandlerChainException.h"

namespace tividy {

template <typename T>
LevelHandler<T>::LevelHandler(Participant<T>* participant, int level) : participant(participant), level(level) {
    if (participant == nullptr) {
        throw InvalidHandlerChainException("LevelHandler requires a participant");
    }
    if (level < 0) {
        throw InvalidHandlerChainException("LevelHandler level must not be negative");
    }
}

template <typename T>
Participant<T>* LevelHandler<T>::getParticipant() const {
    return participant;
}

template <typename T>
int LevelHandler<T>::getLevel() const {
    return level;
}

template <typename T>
bool LevelHandler<T>::canHandle(const WorkComponent<T>& work, int position) const {
    return position == level;
}

template <typename T>
void LevelHandler<T>::process(WorkComponent<T>& work) {}

template <typename T>
Handler<T>* LevelHandler<T>::cloneSelf() const {
    return new LevelHandler<T>(participant, level);
}

template class LevelHandler<int>;
template class LevelHandler<std::string>;

}