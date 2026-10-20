#ifndef ESCALATEDWORK_H
#define ESCALATEDWORK_H

#include "WorkItemDecorator.h"
#include "Handler.h"

namespace tividy{
    template <typename T>
    class EscalatedWork : public WorkItemDecorator<T>{
        public:
            explicit EscalatedWork(WorkComponent<T>* inner);
            ~EscalatedWork();

            void escalate(const std::vector<Participant<T>*>& participants) override;

            WorkMemento<T>* createMemento() const override;
            void restore(const WorkMemento<T>& memento) override;
            WorkComponent<T>* copy() const override;

        private:
            void buildChain(const std::vector<Participant<T>*>& participants);
            Handler<T>* handler;
            int position;
    };
}

#endif