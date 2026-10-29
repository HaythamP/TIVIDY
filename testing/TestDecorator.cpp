#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "WorkItem.h"
#include "WorkItemDecorator.h"
#include "AuditedWork.h"
#include "EscalatedWork.h"
#include "ApprovedWork.h"
#include "AuditEntryNotFoundException.h"
#include "InvalidWorkflowException.h"

using namespace tividy;

TEST(DecoratorPatternTest, DecoratorNullInnerGuard){
    EXPECT_THROW(AuditedWork<int>(nullptr), InvalidWorkflowException);
    EXPECT_THROW(EscalatedWork<int>(nullptr), InvalidWorkflowException);
    EXPECT_THROW(ApprovedWork<int>(nullptr), InvalidWorkflowException);
}

TEST(DecoratorPatternTest, AuditedWorkLogOperations){
    WorkItem<int>* leaf = new WorkItem<int>(1, "Task");
    AuditedWork<int> audited(leaf);

    EXPECT_EQ(audited.getId(), 1);
    EXPECT_EQ(audited.getName(), "Task");
    EXPECT_TRUE(audited.getAuditLog().empty());

    audited.addAuditEntry("Entry 0");
    audited.addAuditEntry("Entry 1");
    ASSERT_EQ(audited.getAuditLog().size(), 2u);
    EXPECT_EQ(audited.getAuditLog()[0], "Entry 0");

}