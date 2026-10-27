#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <string>
#include <vector>

namespace tividy {

/**
 * @brief A person (or resource) that can be assigned to work items.
 * Non-owning within workflows. Workload is tracked automatically.
 */
template <typename T>
class Participant {
public:
    /** @brief Creates a participant with an id, a display name, no roles, and zero workload. */
    Participant(const T& id, const std::string& name);

    /** @brief Returns the participant id. */
    const T& getId() const;

    /** @brief Returns the display name. */
    const std::string& getName() const;

    /** @brief Adds a role such as "engineer" (duplicates are ignored). */
    void addRole(const std::string& role);

    /** @brief Returns true if the participant has the given role. */
    bool hasRole(const std::string& role) const;

    /** @brief Returns active work items currently held by the participant. */
    int getWorkload() const;

    /** @brief Adds delta to the workload (never drops below zero). */
    void adjustWorkload(int delta);

private:
    T id;
    std::string name;
    std::vector<std::string> roles;
    int workload;
};

} // namespace tividy

#endif // PARTICIPANT_H