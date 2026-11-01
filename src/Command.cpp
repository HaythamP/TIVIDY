#include "Command.h"
#include <string>

namespace tividy {

template <typename T>
Command<T>::~Command() {}

template class Command<int>;
template class Command<std::string>;

}
