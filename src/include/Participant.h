#ifndef TIVIDY_PARTICIPANT_H
#define TIVIDY_PARTICIPANT_H

#include <string>
#include <vector>

namespace tividy {

// A person (or resource) that can be assigned work.
// Plain data: copying a Participant is allowed (WorkflowDefinition::clone copies them).
template <typename IdType>
class Participant {
public:
    Participant(const IdType& id, const std::string& name);

    const IdType& getId() const;
    const std::string& getName() const;

    void addRole(const std::string& role);
    bool hasRole(const std::string& role) const;

    int getWorkload() const;              // number of open work items assigned
    void adjustWorkload(int delta);

private:
    IdType id;
    std::string name;
    std::vector<std::string> roles;
    int workload;
};

} // namespace tividy

#endif