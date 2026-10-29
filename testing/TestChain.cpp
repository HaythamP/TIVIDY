#include <gtest/gtest.h>
#include <string>

#include "Handler.h"
#include "LevelHandler.h"
#include "FallbackHandler.h"
#include "Participant.h"
#include "WorkItem.h"
#include "InvalidHandlerChainException.h"

using namespace tividy;

TEST(ChainOfResponsibilityTest, HandlerLinkingAndTraversal) {
    Participant<int> p1(1, "Alice", {"Engineer"});
    Participant<int> p2(2, "Bob", {"Manager"});

    EXPECT_THROW(LevelHandler<int>(nullptr, 0), InvalidHandlerChainException);
    EXPECT_THROW(LevelHandler<int>(&p1, -1), InvalidHandlerChainException);

    Handler<int>* h0 = new LevelHandler<int>(&p1, 0);
    Handler<int>* h1 = new LevelHandler<int>(&p2, 1);
    Handler<int>* fb = new FallbackHandler<int>();

    h0->setNext(h1);
    h1->setNext(fb);

    EXPECT_EQ(h0->getNext(), h1);
    EXPECT_EQ(h1->getNext(), fb);
    EXPECT_EQ(fb->getNext(), nullptr);

    WorkItem<int> item(10, "Task");

    // Level 0 matched
    Handler<int>* res0 = h0->handle(item, 0);
    ASSERT_NE(res0, nullptr);
    EXPECT_EQ(res0->getParticipant(), &p1);

    // Level 1 matched
    Handler<int>* res1 = h0->handle(item, 1);
    ASSERT_NE(res1, nullptr);
    EXPECT_EQ(res1->getParticipant(), &p2);

    // Unmatched index forwards to FallbackHandler
    Handler<int>* resFallback = h0->handle(item, 99);
    ASSERT_NE(resFallback, nullptr);
    EXPECT_EQ(resFallback->getParticipant(), nullptr);

    delete h0;
}

TEST(ChainOfResponsibilityTest, HandlerDeepCopy) {
    Participant<int> p(1, "Tom", {"Lead"});
    Handler<int>* head = new LevelHandler<int>(&p, 0);
    head->setNext(new FallbackHandler<int>());

    Handler<int>* clone = head->copy();
    ASSERT_NE(clone, nullptr);
    EXPECT_NE(clone, head);
    EXPECT_NE(clone->getNext(), head->getNext());
    EXPECT_EQ(clone->getParticipant(), head->getParticipant());

    delete head;
    delete clone;
}