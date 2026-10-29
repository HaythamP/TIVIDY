#include "crow.h"
#include "TividyFacade.h"
#include "WorkflowDirector.h"
#include "AuditedWork.h"
#include "EscalatedWork.h"
#include "ApprovedWork.h"
#include <fstream>
#include <memory>
#include <sstream>
#include <map>
#include <cstdlib>

using namespace tividy;

struct LocalCors {
    struct context {};
    void before_handle(crow::request&, crow::response&, context&) {}
    void after_handle(crow::request& req, crow::response& res, context&) {
        // Directly opened file:// pages send Origin: null. Keep the exception
        // limited to that case; normal HTTP pages use the same-origin API.
        if (req.get_header_value("Origin") == "null") res.add_header("Access-Control-Allow-Origin", "null");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
        res.add_header("Access-Control-Max-Age", "600");
    }
};

namespace {
struct AppState {
    TividyFacade<std::string> facade;
    std::vector<std::unique_ptr<Participant<std::string>>> owned;
    std::map<std::string, Participant<std::string>*> participants;
    std::map<std::string, std::string> roles;
    WorkflowDefinition<std::string>* workflow = nullptr;
    ~AppState() { delete workflow; }
};

Participant<std::string>* participant(AppState& s, const std::string& id) {
    auto it = s.participants.find(id);
    if (it == s.participants.end()) throw std::runtime_error("Unknown participant id: " + id);
    return it->second;
}
WorkComponent<std::string>* item(AppState& s, const std::string& id) {
    if (!s.workflow) throw std::runtime_error("Create a workflow first");
    WorkComponent<std::string>* found = s.workflow->find(id);
    if (!found) throw std::runtime_error("Unknown work item id: " + id);
    return found;
}
crow::json::wvalue componentJson(WorkComponent<std::string>* c) {
    crow::json::wvalue out;
    out["id"] = c->getId(); out["name"] = c->getName(); out["state"] = c->getStateName();
    out["group"] = !c->getChildren().empty(); out["approval"] = c->hasApproval(); out["escalation"] = c->hasEscalation();
    out["groupName"] = c->getParent() ? c->getParent()->getName() : "";
    out["groupId"] = c->getParent() ? c->getParent()->getId() : "";
    out["audited"] = dynamic_cast<AuditedWork<std::string>*>(c) != nullptr;
    auto children = c->getChildren();
    crow::json::wvalue list = crow::json::wvalue::list();
    for (auto* child : children) list[list.size()] = componentJson(child);
    out["children"] = std::move(list);
    crow::json::wvalue assignees = crow::json::wvalue::list();
    auto people = c->getAssignees();
    for (std::size_t i = 0; i < people.size(); ++i) { crow::json::wvalue p; p["id"] = people[i]->getId(); p["name"] = people[i]->getName(); assignees[i] = std::move(p); }
    out["assignees"] = std::move(assignees);
    auto audits = c->getAuditLog(); crow::json::wvalue log = crow::json::wvalue::list();
    for (std::size_t i = 0; i < audits.size(); ++i) log[i] = audits[i];
    out["audit"] = std::move(log);
    return out;
}
crow::json::wvalue stateJson(AppState& s) {
    crow::json::wvalue out;
    crow::json::wvalue people = crow::json::wvalue::list(); std::size_t personIndex = 0;
    for (auto& p : s.participants) { crow::json::wvalue q; q["id"] = p.second->getId(); q["name"] = p.second->getName(); q["role"] = s.roles[p.first]; people[personIndex++] = std::move(q); }
    out["participants"] = std::move(people);
    if (!s.workflow) { out["active"] = false; return out; }
    out["active"] = true; out["name"] = s.workflow->getName(); out["complete"] = s.facade.isComplete(s.workflow);
    out["summary"] = s.facade.getProgressReport(s.workflow); out["treeText"] = s.facade.printWorkflow(s.workflow);
    crow::json::wvalue steps = crow::json::wvalue::list();
    const auto& root = s.workflow->getSteps();
    for (std::size_t i = 0; i < root.size(); ++i) steps[i] = componentJson(root[i]);
    out["steps"] = std::move(steps);
    return out;
}
crow::response errorResponse(const std::exception& e) { crow::json::wvalue body; body["error"] = e.what(); return crow::response(400, body); }
void appendSavedComponent(WorkflowBuilder<std::string>& builder, const crow::json::rvalue& node) {
    if (node["group"].b()) {
        builder.beginGroup(node["id"].s(), node["name"].s());
        for (const auto& child : node["children"]) appendSavedComponent(builder, child);
        builder.endGroup();
        return;
    }
    builder.newWorkItem(node["id"].s(), node["name"].s());
    if (node["audited"].b()) builder.withAudit();
    if (node["approval"].b()) builder.withApproval();
    if (node["escalation"].b()) builder.withEscalation();
    builder.endWorkItem();
}
void restoreSavedWorkflow(AppState& state, const crow::json::rvalue& saved) {
    if (state.workflow) { delete state.workflow; state.workflow = nullptr; }
    std::map<std::string, Participant<std::string>*> people;
    std::map<std::string, std::string> roles;
    std::vector<std::unique_ptr<Participant<std::string>>> owned;
    for (const auto& p : saved["participants"]) {
        std::string id=p["id"].s(), name=p["name"].s(), role=p["role"].s();
        owned.emplace_back(new Participant<std::string>(id, name)); owned.back()->addRole(role);
        people[id]=owned.back().get(); roles[id]=role;
    }
    if (people.empty()) throw std::runtime_error("Saved workflow contains no participants");
    state.owned.swap(owned); state.participants.swap(people); state.roles.swap(roles);
    WorkflowBuilder<std::string>& builder=state.facade.getBuilder();
    builder.reset(saved["name"].s());
    for (const auto& p : state.participants) builder.addParticipant(p.second);
    for (const auto& step : saved["steps"]) appendSavedComponent(builder, step);
    state.workflow=builder.getResult();
    state.facade.startWorkflow(state.workflow);
}
}

