#include <gtest/gtest.h>

#include "DummyException.h"
#include "TividyException.h"
#include "InvalidTransitionException.h"
#include "ComponentNotFoundException.h"
#include "InvalidAssignmentArgumentException.h"
#include "InsufficientParticipantsException.h"
#include "InvalidHandlerChainException.h"
#include "InvalidWorkflowException.h"
#include "NotPristineException.h"
#include "DuplicateIdException.h"
#include "ApprovalIncompleteException.h"
#include "UnsupportedOperationException.h"

using namespace tividy;

TEST(ExceptionsTest, InstantiateAllExceptions) {
    TividyException e1("base error");
    EXPECT_STREQ(e1.what(), "base error");

    InvalidTransitionException e2("Created", "complete");
    EXPECT_STREQ(e2.what(), "Cannot complete while in state Created");

    ComponentNotFoundException e3("not found");
    EXPECT_STREQ(e3.what(), "not found");

    InvalidAssignmentArgumentException e4("invalid arg");
    EXPECT_STREQ(e4.what(), "invalid arg");

    InsufficientParticipantsException e5("not enough participants");
    EXPECT_STREQ(e5.what(), "not enough participants");

    InvalidHandlerChainException e6("invalid chain");
    EXPECT_STREQ(e6.what(), "invalid chain");

    InvalidWorkflowException e7("invalid workflow");
    EXPECT_STREQ(e7.what(), "invalid workflow");

    NotPristineException e8("not pristine");
    EXPECT_STREQ(e8.what(), "not pristine");

    DuplicateIdException e9("duplicate id");
    EXPECT_STREQ(e9.what(), "duplicate id");

    ApprovalIncompleteException e10("approval incomplete");
    EXPECT_STREQ(e10.what(), "approval incomplete");

    UnsupportedOperationException e11("unsupported operation");
    EXPECT_STREQ(e11.what(), "unsupported operation");

    DummyException e12("dummy exception");
    EXPECT_STREQ(e12.what(), "dummy exception");

    
}