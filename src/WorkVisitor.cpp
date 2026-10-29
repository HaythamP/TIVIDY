#include "WorkVisitor.h"
#include <string>

namespace tividy {

template <typename T>
WorkVisitor<T>::~WorkVisitor() {}

template class WorkVisitor<int>;
template class WorkVisitor<std::string>;

}
