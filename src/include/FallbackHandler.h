#ifndef FALLBACKHANDLER_H
#define FALLBACKHANDLER_H

#include "Handler.h"

namespace tividy {

template <typename T>
class FallbackHandler : public Handler<T> {
public:
    FallbackHandler();

protected:
    bool canHandle(const WorkComponent<T>& work, int position) const override;
    void process(WorkComponent<T>& work) override;
    Handler<T>* cloneSelf() const override;
};

}

#endif