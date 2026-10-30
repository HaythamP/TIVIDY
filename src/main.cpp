#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include "TividyFacade.h"
#include "TividyException.h"

using namespace tividy;

static void banner(const std::string& title) {
    std::cout << "\n==================================================================\n"
              << title
              << "\n==================================================================" << std::endl;
}

static void section(const std::string& title) {
    std::cout << "\n--- " << title << " ---" << std::endl;
}

static void attempt(const std::string& label, const std::function<void()>& action) {
    try {
        action();
    } catch (const TividyException& error) {
        std::cout << "  ! " << label << " was refused: " << error.getMessage() << std::endl;
    }
}

template <typename T>
static void show(TividyFacade<T>& facade, WorkflowDefinition<T>* workflow) {
    std::cout << "\n" << facade.printWorkflow(workflow) << "  Progress: " << facade.getProgressReport(workflow) << std::endl;
}

template <typename T>
static void showTraversal(WorkflowDefinition<T>* workflow) {
    WorkIterator<T>* iterator = workflow->createIterator();
    std::cout << "  Depth-first traversal:";
    for (iterator->first(); !iterator->isDone(); iterator->next()) {
        std::cout << " " << iterator->getCurrent()->getName() << ";";
    }
    std::cout << std::endl;
    delete iterator;
}

template <typename T>
static void showAudit(TividyFacade<T>& facade, WorkflowDefinition<T>* workflow, const T& id) {
    std::vector<std::string> log = facade.getAuditLog(workflow, id);
    std::cout << "  Audit log of '" << workflow->find(id)->getName() << "':" << std::endl;
    for (std::size_t i = 0; i < log.size(); ++i) {
        std::cout << "    [" << i << "] " << log[i] << std::endl;
    }
}

static void incidentResponse() {
    banner("SCENARIO 1: Incident Response (Power Outage)  -  integer ids");

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

    section("Build the template (Builder + Director) and clone it (Prototype)");
    WorkflowDefinition<int>* prototype = facade.constructIncidentResponseWorkflow(ids);
    std::cout << facade.printWorkflow(prototype);
    WorkflowDefinition<int>* run = facade.cloneWorkflow(prototype);
    run->addParticipant(&dana);
    run->addParticipant(&eli);
    run->addParticipant(&fay);
    run->addParticipant(&gus);
    run->addParticipant(&hana);
    showTraversal(run);

    section("Start the workflow");
    facade.startWorkflow(run);

    section("1. Dispatch Team: assignment by strategy (role = dispatcher)");
    RoleAssignment<int> dispatcherRole("dispatcher");
    RoleAssignment<int> engineerRole("engineer");
    LeastLoadedAssignment<int> leastLoaded;
    RuleAssignment<int> randomPick;
    std::vector<AssignmentStrategy<int>*> dispatchers(1, &dispatcherRole);
    facade.assignWithStrategies(run, 1, dispatchers, 1);
    facade.startWork(run, 1);
    facade.completeWork(run, 1);

    section("2. Investigation (audited): two least-loaded engineers");
    std::vector<AssignmentStrategy<int>*> engineers;
    engineers.push_back(&engineerRole);
    engineers.push_back(&leastLoaded);
    facade.assignWithStrategies(run, 2, engineers, 2);
    facade.startWork(run, 2);
    facade.addAuditEntry(run, 2, "Manual note: transformer at substation 4 is suspected");
    facade.completeWork(run, 2);

    section("Undo the completion, then complete again");
    facade.undo(run);
    facade.completeWork(run, 2);

    section("Audit log: add, edit and delete entries");
    showAudit(facade, run, 2);
    facade.editAuditEntry(run, 2, 0, "Edited: assigned engineers confirmed");
    facade.addAuditEntry(run, 2, "Temporary entry");
    facade.deleteAuditEntry(run, 2, facade.getAuditLog(run, 2).size() - 1);
    showAudit(facade, run, 2);

    section("3. Containment: direct assignment of chosen participants");
    std::vector<Participant<int>*> eliOnly(1, &eli);
    facade.assignToParticipants(run, 3, eliOnly);
    attempt("approving Containment", [&]() { facade.approveWork(run, 3); });
    facade.startWork(run, 3);
    facade.completeWork(run, 3);

    section("4. Repair (audited + escalation + approval)");
    std::vector<AssignmentStrategy<int>*> repairCrew;
    repairCrew.push_back(&engineerRole);
    repairCrew.push_back(&randomPick);
    std::vector<Participant<int>*> chain;
    chain.push_back(&gus);
    chain.push_back(&hana);
    facade.assignWithStrategies(run, 4, repairCrew, 1, chain, chain);
    facade.startWork(run, 4);
    attempt("completing Repair before it is approved", [&]() { facade.completeWork(run, 4); });
    show(facade, run);

    section("Escalate -> Approve -> Complete");
    facade.escalateWork(run, 4);
    facade.escalateWork(run, 4);
    facade.escalateWork(run, 4);
    facade.approveWork(run, 4);
    showAudit(facade, run, 4);

    section("5. Restore");
    std::vector<Participant<int>*> danaOnly(1, &dana);
    facade.assignToParticipants(run, 5, danaOnly);
    facade.startWork(run, 5);
    facade.completeWork(run, 5);

    section("Result");
    show(facade, run);
    std::cout << "  Workflow complete: " << (facade.isComplete(run) ? "yes" : "no") << std::endl;
    std::cout << "  Command history:" << std::endl;
    std::vector<std::string> log = run->getLog();
    for (std::size_t i = 0; i < log.size(); ++i) {
        std::cout << "    " << (i + 1) << ". " << log[i] << std::endl;
    }
    std::cout << "  Workloads - Dana: " << dana.getWorkload() << ", Eli: " << eli.getWorkload()
              << ", Fay: " << fay.getWorkload() << ", Gus: " << gus.getWorkload() << ", Hana: " << hana.getWorkload() << std::endl;

    delete run;
    delete prototype;
}

int main() {
    try {
        incidentResponse();
    } catch (const TividyException& error) {
        std::cout << "Unexpected error: " << error.getMessage() << std::endl;
        return 1;
    }
    std::cout << "\nDemo finished." << std::endl;
    return 0;
}