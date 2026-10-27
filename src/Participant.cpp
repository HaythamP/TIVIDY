#include "Participant.h"
#include <algorithm>

namespace tividy {

template <typename T>
Participant<T>::Participant(const T& id, const std::string& name)
    : id(id), name(name), roles(), workload(0) {}

template <typename T>
const T& Participant<T>::getId() const {
    return id;
}

template <typename T>
const std::string& Participant<T>::getName() const {
    return name;
}

template <typename T>
void Participant<T>::addRole(const std::string& role) {
    if (!hasRole(role)) {
        roles.push_back(role);
    }
}

template <typename T>
bool Participant<T>::hasRole(const std::string& role) const {
    return std::find(roles.begin(), roles.end(), role) != roles.end();
}

template <typename T>
int Participant<T>::getWorkload() const {
    return workload;
}

template <typename T>
void Participant<T>::adjustWorkload(int delta) {
    workload += delta;
    if (workload < 0) {
        workload = 0;
    }
}

template class Participant<int>;
template class Participant<std::string>;

} // namespace tividy