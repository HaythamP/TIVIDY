#ifndef LEVELHANDLER_H
#define LEVELHANDLER_H

#include "Handler.h"
#include "Participant.h"

namespace tividy {

/**
 * @brief ConcreteHandler: one escalation or approval level held by one participant. It handles the request when the chain position equals its level. Handling is the act of taking responsibility (the decorator records the participant as the current contact), so process() does nothing.
 */
template <typename T>
class LevelHandler : public Handler<T> {
public:
    /**
     * @brief @param participant non-owning participant (must not be null).
     *
     * @param level zero-based position in the chain.
     * @throws InvalidHandlerChainException for a null participant or negative level.
     */
    LevelHandler(Participant<T>* participant, int level);

    /** @brief Returns the level's participant. */
    Participant<T>* getParticipant() const override;
    /** @brief Returns the zero-based level. */
    int getLevel() const;

protected:
    /** @brief True when position equals the level. */
    bool canHandle(const WorkComponent<T>& work, int position) const override;
    void process(WorkComponent<T>& work) override;
    Handler<T>* cloneSelf() const override;

private:
    Participant<T>* participant;  ///< Non-owning participant.
    int level;  ///< Zero-based level.
};

}

#endif