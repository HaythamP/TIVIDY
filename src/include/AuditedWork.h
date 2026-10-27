#ifndef AUDITEDWORK_H
#define AUDITEDWORK_H

#include <string>
#include <vector>
#include "WorkItemDecorator.h"

namespace tividy{
// DECORATOR: ConcreteDecorator. Records every assign, start and complete before forwarding it.
    template <typename IdType>
    class AuditedWork : public WorkItemDecorator<IdType>{
        public:
            explicit AuditedWork(WorkComponent<IdType>* inner);  

            Participant<IdType>* assignWork(const std::vector<Participant<IdType>*>& participants) override;
            void assignTo(Participant<IdType>* participant) override;
            void start() override;
            void complete() override;

            const std::vector<std::string>& getAuditLog() const;
            WorkComponent<IdType>* copy() const override;                  

        private:
            std::vector<std::string> auditLog;
    };
} 

#endif
