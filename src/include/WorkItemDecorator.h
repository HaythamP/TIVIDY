#ifndef WORKITEMDECORATOR_H
#define WORKITEMDECORATOR_H

#include <string>
#include <vector>
#include "WorkComponent.h"

namespace tividy{
    template <typename T>
    class WorkItemDecorator : public WorkComponent<T> {
    public:
        explicit WorkItemDecorator(WorkComponent<T>* inner);
        ~WorkItemDecorator();

        const T& getId() const override;
        const std::string& getName() const override;
        WorkGroup<T>* getParent() const override;
        void setParent(WorkGroup<T>* parent) override;
        WorkComponent<T>* getNext() const override;
        void setNext(WorkComponent<T>* next) override;
        Mediator<T>* getMediator() const override;
        void setMediator(Mediator<T>* mediator) override;
        std::vector<WorkComponent<T>*> getChildren() const override;
        std::vector<WorkComponent<T>*> getItems() override;
        void add(WorkComponent<T>* child) override;
        WorkComponent<T>* find(const T& id) override;
        void release() override;
        bool isComplete() const override;
        bool isAvailable() const override;
        std::string getStateName() const override;
        std::vector<Participant<T>*> getAssignees() const override;
        void setAssignees(const std::vector<Participant<T>*>& participants) override;
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
        void accept(WorkVisitor<T>& visitor) override;

    protected:
        WorkComponent<T>* inner;
    };
} 

#endif
