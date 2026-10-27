#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <string>
#include <vector>
#include <algorithm>

namespace tividy {

/**
 * @brief A person (or resource) that can be assigned to work items.
 *
 * Participants are owned by the application, never by a workflow: one participant can work in several WorkflowDefinition objects and every pointer to a Participant inside TIVIDY is non-owning.
 * The workload counter is maintained automatically by WorkItem: it grows when the participant is assigned and is released when the item reaches Completed, Rejected or Cancelled (and re-acquired when an item restarts or an undo restores it).
 * @tparam T type of the participant id (int or std::string).
 */
template <typename T>
class Participant {
public:
    /** @brief Creates a participant with an id, a display name, no roles and zero workload. */
    Participant(const T& id, const std::string& name, std::vector<std::string> roles);

    /** @brief Returns the participant id. */
    const T& getId() const;
    
    /** @brief Returns the display name (used in console messages). */
    const std::string& getName() const;

    /** @brief Returns true if the participant has the given role. */
    bool hasRole(const std::string& role) const;

    /**
     * @brief Returns the number of active work items currently held by the participant. Used by LeastLoadedAssignment.
     */
    int getWorkload() const;
    
    /** @brief Adds delta to the workload (never drops below zero). Normally called only by WorkItem. */
    void adjustWorkload(int delta);

private:
    T id;  ///< Unique id.
    std::string name;  ///< Display name.
    std::vector<std::string> roles;  ///< Roles held by the participant.
    int workload;  ///< Active work item count.
};

}

#endif