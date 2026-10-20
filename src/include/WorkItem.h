#ifndef WORKITEM_H
#define WORKITEM_H

#include <string>
#include <vector>
#include "WorkComponent.h"
#include "WorkItemState.h"
#include "AssignmentStrategy.h"
#include "WorkMemento.h"

namespace tividy{
    template <typename T>
    class WorkItem : public WorkComponent<T>{
        public:
            WorkItem(const T& id, const std::string& name);
            ~WorkItem();

            const T& getId() const override;
            const std::string& getName() const override;
            std::string getStateName() const override;
            std::vector<Participant<T>*> getAssignees() const override;
            void setAssignees(const std::vector<Participant<T>*>& participants) override;

            std::vector<WorkComponent<T>*> getItems() override;
            WorkComponent<T>* find(const T& id) override;
            void release() override;
            bool isComplete() const override;
            bool isAvailable() const override;

            void addStrategy(AssignmentStrategy<T>* strategy) override;
            std::vector<Participant<T>*> assignWork(const std::vector<Participant<T>*>& candidates, int numAssignees = 0) override;
            void assignTo(const std::vector<Participant<T>*>& participants) override;
            void start() override;
            void complete() override;
            void reject() override;
            void cancel() override;
            void escalate(const std::vector<Participant<T>*>& participants) override;
            void approve(const std::vector<Participant<T>*>& participants) override;

            WorkMemento<T>* createMemento() const override;
            void restore(const WorkMemento<T>& memento) override;

            WorkComponent<T>* copy() const override;
            void accept(WorkVisitor<T>& visitor) override;

            void setState(WorkItemState<T>* state);

        private:
            T id;
            std::string name;
            WorkItemState<T>* state;
            std::vector<Participant<T>*> assignees;
            std::vector<AssignmentStrategy<T>*> strategies;
    };
}

#endif
