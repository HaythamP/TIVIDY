#include "WorkIterator.h"
#include <string>

namespace tividy {

template <typename T>
WorkIterator<T>::~WorkIterator() {}

template class WorkIterator<int>;
template class WorkIterator<std::string>;

}
