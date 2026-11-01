#include "FallbackHandler.h"
#include "WorkComponent.h"
#include <iostream>
#include <string>

namespace tividy {

template <typename T>
FallbackHandler<T>::FallbackHandler() {}

template <typename T>
bool FallbackHandler<T>::canHandle(const WorkComponent<T>& work, int position) const {
    return true;
}

template <typename T>
void FallbackHandler<T>::process(WorkComponent<T>& work) {
    std::cout << "  End of the escalation chain: FallbackHandler resolves '" << work.getName() << "'." << std::endl;
    work.complete();
}

template <typename T>
Handler<T>* FallbackHandler<T>::cloneSelf() const {
    return new FallbackHandler<T>();
}

template class FallbackHandler<int>;
template class FallbackHandler<std::string>;

}