#ifndef WORKCOMPONENT_H
#define WORKCOMPONENT_H

#include <string>
#include <vector>

namespace tividy{
    template <typename T> class WorkGroup;
    template <typename T> class Mediator;
    template <typename T> class WorkVisitor;
    template <typename T> class WorkIterator;
    template <typename T> class WorkMemento;
    template <typename T> class AssignmentStrategy;
    template <typename T> class Participant;

    template <typename T>
    class WorkComponent{
        public:
            virtual ~WorkComponent();

            virtual const T& getId() const = 0;
            virtual const std::string& getName() const = 0;

            virtual WorkGroup<T>* getParent() const;
            virtual void setParent(WorkGroup<T>* parent);
            virtual WorkComponent<T>* getNext() const;
            virtual void setNext(WorkComponent<T>* next);
            virtual Mediator<T>* getMediator() const;
            virtual void setMediator(Mediator<T>* mediator);

            virtual std::vector<WorkComponent<T>*> getChildren() const;
            virtual std::vector<WorkComponent<T>*> getItems();
            virtual void add(WorkComponent<T>* child);
            virtual WorkComponent<T>* find(const T& id) = 0;

            virtual void release() = 0;
            virtual bool isComplete() const = 0;
            virtual bool isAvailable() const = 0;

            virtual std::string getStateName() const;
            virtual std::vector<Participant<T>*> getAssignees() const;
            virtual void setAssignees(const std::vector<Participant<T>*>& participants);
            virtual void addStrategy(AssignmentStrategy<T>* strategy);
            virtual std::vector<Participant<T>*> assignWork(const std::vector<Participant<T>*>& candidates, int numAssignees = 0);
            virtual void assignTo(const std::vector<Participant<T>*>& participants);
            virtual void start();
            virtual void complete();
            virtual void reject();
            virtual void cancel();
            virtual void escalate(const std::vector<Participant<T>*>& participants);
            virtual void approve(const std::vector<Participant<T>*>& participants);

            virtual WorkMemento<T>* createMemento() const;
            virtual void restore(const WorkMemento<T>& memento);

            virtual WorkComponent<T>* copy() const = 0;

            virtual void accept(WorkVisitor<T>& visitor) = 0;
            virtual WorkIterator<T>* createIterator();

        protected:
            WorkComponent();

            WorkGroup<T>* parent;
            WorkComponent<T>* next;
            Mediator<T>* mediator;

        private:
            WorkComponent(const WorkComponent&) = delete;
            WorkComponent& operator=(const WorkComponent&) = delete;
    };
}

#endif
