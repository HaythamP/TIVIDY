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

TEST(DecoratorPatternTest, DecoratorNullInnerGuard) {
    EXPECT_THROW(AuditedWork<int>(nullptr), InvalidWorkflowException);
    EXPECT_THROW(EscalatedWork<int>(nullptr), InvalidWorkflowException);
    EXPECT_THROW(ApprovedWork<int>(nullptr), InvalidWorkflowException);
}