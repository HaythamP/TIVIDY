#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "TividyFacade.h"
#include "TividyException.h"
#include "RoleAssignment.h"
#include "LeastLoadedAssignment.h"
#include "RuleAssignment.h"
#include "Participant.h"
#include "WorkflowDefinition.h"

using namespace tividy;


// SCENARIO 1: Incident Response (Power Outage)
TEST(EndToEndScenarioTest, IncidentResponseWorkflowIntegerIds) {
    TividyFacade<int> facade;

   
    Participant<int> dana(1, "Dana", {"dispatcher"});
    Participant<int> eli(2, "Eli", {"engineer"});
    Participant<int> fay(3, "Fay", {"engineer"});
    Participant<int> gus(4, "Gus", {"supervisor"});
    Participant<int> hana(5, "Hana", {"manager"});

    std::vector<int> ids;
    for (int i = 1; i <= 5; ++i) {
        ids.push_back(i);
    }

  
    WorkflowDefinition<int>* prototype = facade.constructIncidentResponseWorkflow(ids);
    ASSERT_NE(prototype, nullptr);

    WorkflowDefinition<int>* run = facade.cloneWorkflow(prototype);
    ASSERT_NE(run, nullptr);

    run->addParticipant(&dana);
    run->addParticipant(&eli);
    run->addParticipant(&fay);
    run->addParticipant(&gus);
    run->addParticipant(&hana);

    
    WorkIterator<int>* iterator = run->createIterator();
    ASSERT_NE(iterator, nullptr);
    int stepCount = 0;
    for (iterator->first(); !iterator->isDone(); iterator->next()) {
        EXPECT_FALSE(iterator->getCurrent()->getName().empty());
        ++stepCount;
    }
    EXPECT_GT(stepCount, 0);
    delete iterator;

    
    facade.startWorkflow(run);

  
    RoleAssignment<int> dispatcherRole("dispatcher");
    RoleAssignment<int> engineerRole("engineer");
    LeastLoadedAssignment<int> leastLoaded;
    RuleAssignment<int> randomPick;

    std::vector<AssignmentStrategy<int>*> dispatchers(1, &dispatcherRole);
    facade.assignWithStrategies(run, 1, dispatchers, 1);
    facade.startWork(run, 1);
    facade.completeWork(run, 1);

   
    std::vector<AssignmentStrategy<int>*> engineers;
    engineers.push_back(&engineerRole);
    engineers.push_back(&leastLoaded);

    facade.assignWithStrategies(run, 2, engineers, 2);
    facade.startWork(run, 2);
    facade.addAuditEntry(run, 2, "Manual note: transformer at substation 4 is suspected");
    facade.completeWork(run, 2);

    
    facade.undo(run);
    facade.completeWork(run, 2);

   
    std::vector<std::string> logBefore = facade.getAuditLog(run, 2);
    EXPECT_FALSE(logBefore.empty());

    facade.editAuditEntry(run, 2, 0, "Edited: assigned engineers confirmed");
    EXPECT_EQ(facade.getAuditLog(run, 2)[0], "Edited: assigned engineers confirmed");

    facade.addAuditEntry(run, 2, "Temporary entry");
    std::size_t sizeAfterAdd = facade.getAuditLog(run, 2).size();
    facade.deleteAuditEntry(run, 2, sizeAfterAdd - 1);
    EXPECT_EQ(facade.getAuditLog(run, 2).size(), sizeAfterAdd - 1);

   
    std::vector<Participant<int>*> eliOnly(1, &eli);
    facade.assignToParticipants(run, 3, eliOnly);

    
    EXPECT_THROW(facade.approveWork(run, 3), TividyException);

    facade.startWork(run, 3);
    facade.completeWork(run, 3);

    
    std::vector<AssignmentStrategy<int>*> repairCrew;
    repairCrew.push_back(&engineerRole);
    repairCrew.push_back(&randomPick);

    std::vector<Participant<int>*> chain;
    chain.push_back(&gus);
    chain.push_back(&hana);

    facade.assignWithStrategies(run, 4, repairCrew, 1, chain, chain);
    facade.startWork(run, 4);

    
    EXPECT_THROW(facade.completeWork(run, 4), TividyException);

    
    facade.escalateWork(run, 4);
    facade.escalateWork(run, 4);
    facade.escalateWork(run, 4);
    facade.approveWork(run, 4);

    std::vector<std::string> repairLog = facade.getAuditLog(run, 4);
    EXPECT_FALSE(repairLog.empty());

   
    std::vector<Participant<int>*> danaOnly(1, &dana);
    facade.assignToParticipants(run, 5, danaOnly);
    facade.startWork(run, 5);
    facade.completeWork(run, 5);

    
    EXPECT_TRUE(facade.isComplete(run));
    EXPECT_FALSE(run->getLog().empty());

    delete run;
    delete prototype;
}


// SCENARIO 2: Job Application (Tech Company)

TEST(EndToEndScenarioTest, JobApplicationWorkflowHappyPathStringIds) {
    TividyFacade<std::string> facade;

   
    Participant<std::string> rita("P1", "Rita", {"recruiter"});
    Participant<std::string> tom("P2", "Tom", {"technical"});
    Participant<std::string> bea("P3", "Bea", {"behavioural"});
    Participant<std::string> max("P4", "Max", {"hiring-manager"});
    Participant<std::string> dee("P5", "Dee", {"director"});

    std::vector<std::string> ids;
    for (int i = 1; i <= 7; ++i) {
        ids.push_back("JA-" + std::to_string(i));
    }

    WorkflowDefinition<std::string>* prototype = facade.constructJobApplicationWorkflow(ids);
    ASSERT_NE(prototype, nullptr);

    WorkflowDefinition<std::string>* run = facade.cloneWorkflow(prototype);
    ASSERT_NE(run, nullptr);

    run->addParticipant(&rita);
    run->addParticipant(&tom);
    run->addParticipant(&bea);
    run->addParticipant(&max);
    run->addParticipant(&dee);

    facade.startWorkflow(run);

    RoleAssignment<std::string> recruiterRole("recruiter");
    RoleAssignment<std::string> technicalRole("technical");
    RoleAssignment<std::string> behaviouralRole("behavioural");

    std::vector<AssignmentStrategy<std::string>*> recruiters(1, &recruiterRole);
    std::vector<AssignmentStrategy<std::string>*> technical(1, &technicalRole);
    std::vector<AssignmentStrategy<std::string>*> behavioural(1, &behaviouralRole);

   
    facade.assignWithStrategies(run, "JA-2", recruiters, 1);
    facade.assignWithStrategies(run, "JA-3", recruiters, 1);
    facade.startWork(run, "JA-2");
    facade.startWork(run, "JA-3");
    facade.completeWork(run, "JA-3");
    facade.completeWork(run, "JA-2");

 
    facade.assignWithStrategies(run, "JA-5", technical, 1);
    facade.assignWithStrategies(run, "JA-6", behavioural, 1);
    facade.startWork(run, "JA-5");
    facade.startWork(run, "JA-6");
    facade.addAuditEntry(run, "JA-5", "Candidate passed the coding exercise");
    facade.completeWork(run, "JA-5");
    facade.completeWork(run, "JA-6");

    std::vector<std::string> techLog = facade.getAuditLog(run, std::string("JA-5"));
    EXPECT_FALSE(techLog.empty());

   
    std::vector<Participant<std::string>*> decisionMaker(1, &max);
    std::vector<Participant<std::string>*> approvers;
    approvers.push_back(&max);
    approvers.push_back(&dee);

    facade.assignToParticipants(run, "JA-7", decisionMaker, 1, std::vector<Participant<std::string>*>(), approvers);
    facade.startWork(run, "JA-7");
    facade.approveWork(run, "JA-7");

    
    facade.rejectWork(run, "JA-7");
    EXPECT_FALSE(facade.isComplete(run));

    
    facade.startWork(run, "JA-7");
    facade.approveWork(run, "JA-7");
    facade.approveWork(run, "JA-7");

    EXPECT_TRUE(facade.isComplete(run));

    delete run;
    delete prototype;
}


// SCENARIO 2b: Candidate Fails Early Screening

TEST(EndToEndScenarioTest, JobApplicationWorkflowEarlyRejectionStopsFlow) {
    TividyFacade<std::string> facade;

    Participant<std::string> rita("P1", "Rita", {"recruiter"});

    std::vector<std::string> ids;
    for (int i = 1; i <= 7; ++i) {
        ids.push_back("JA-" + std::to_string(i));
    }

    WorkflowDefinition<std::string>* prototype = facade.constructJobApplicationWorkflow(ids);
    ASSERT_NE(prototype, nullptr);

    WorkflowDefinition<std::string>* failed = facade.cloneWorkflow(prototype);
    ASSERT_NE(failed, nullptr);

    failed->addParticipant(&rita);
    facade.startWorkflow(failed);

    RoleAssignment<std::string> recruiterRole("recruiter");
    std::vector<AssignmentStrategy<std::string>*> recruiters(1, &recruiterRole);

    facade.assignWithStrategies(failed, "JA-2", recruiters, 1);
    facade.startWork(failed, "JA-2");
    facade.rejectWork(failed, "JA-2");

    // Restarting a step rejected outside Approval should throw
    EXPECT_THROW(facade.startWork(failed, "JA-2"), TividyException);

    facade.assignWithStrategies(failed, "JA-3", recruiters, 1);
    facade.startWork(failed, "JA-3");
    facade.completeWork(failed, "JA-3");


    EXPECT_FALSE(facade.isComplete(failed));

    delete failed;
    delete prototype;
}