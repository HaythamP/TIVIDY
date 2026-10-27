#ifndef WORKGROUP_H
#define WORKGROUP_H

#include <string>
#include <vector>
#include "WorkComponent.h"

namespace tividy{
    template <typename IdType>
    class WorkGroup : public WorkComponent<IdType>{
        public:
            WorkGroup(const IdType& id, const std::string& name);
            ~WorkGroup();

            const IdType& getId() const override;
            const std::string& getName() const override;

            std::vector<WorkComponent<IdType>*> getChildren() const override;
            void add(WorkComponent<IdType>* child) override;       
            WorkComponent<IdType>* find(const IdType& id) override;

            void release() override;
            bool isComplete() const override;                      
            bool canRevertToCreated() const override;              
            void revertToCreated() override;

            WorkComponent<IdType>* copy() const override;          
            void accept(WorkVisitor<IdType>& visitor) override;

        private:
            IdType id;
            std::string name;
            std::vector<WorkComponent<IdType>*> children;      
    };
}

#endif
