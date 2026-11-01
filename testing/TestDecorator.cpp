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

    audited.editAuditEntry(0, "Edited 0");
    EXPECT_EQ(audited.getAuditLog()[0], "Edited 0");

    audited.deleteAuditEntry(0);
    ASSERT_EQ(audited.getAuditLog().size(), 1u);
    EXPECT_EQ(audited.getAuditLog()[0], "Entry 1");

    EXPECT_THROW(audited.editAuditEntry(10, "Out of bounds"), AuditEntryNotFoundException);
    EXPECT_THROW(audited.deleteAuditEntry(10), AuditEntryNotFoundException);

}

TEST(DecoratorPatternTest, EscalatedAndApprovedWorkFlags){
    WorkItem<int>* leaf1 = new WorkItem<int>(1, "Item1");
    EscalatedWork<int> esc(leaf1);
    EXPECT_TRUE(esc.hasEscalation());
    EXPECT_FALSE(esc.hasApproval());
    EXPECT_EQ(esc.getEscalationContact(), nullptr);

    WorkItem<int>* leaf2 = new WorkItem<int>(2, "Item2");
    ApprovedWork<int> app(leaf2);
    EXPECT_TRUE(app.hasApproval());
    EXPECT_FALSE(app.hasEscalation());
    EXPECT_EQ(app.getApprovalContact(), nullptr);
}