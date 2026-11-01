#include "Builder.h"
#include <string>

namespace tividy {

template <typename T>
Builder<T>::~Builder() {}

template class Builder<int>;
template class Builder<std::string>;

}