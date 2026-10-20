#ifndef APPROVEDWORK_H
#define APPROVEDWORK_H

#include "WorkItemDecorator.h"
#include "Handler.h"

namespace tividy{
    template <typename T>
    class ApprovedWork : public WorkItemDecorator<T>{
        public:
            explicit ApprovedWork(WorkComponent<T>* inner);
            ~ApprovedWork();

            void complete() override;
            void approve(const std::vector<Participant<T>*>& participants) override;

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
