#include "crow.h"
#include "TividyFacade.h"
#include "WorkflowDirector.h"
#include "AuditedWork.h"
#include "EscalatedWork.h"
#include "ApprovedWork.h"
#include <memory>
#include <map>
#include <cstdlib>
#include <algorithm>

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
    std::map<std::string, std::vector<std::string>> approvalSequences;
    std::map<std::string, std::vector<std::string>> escalationSequences;
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

crow::json::wvalue componentJson(AppState& s, WorkComponent<std::string>* c) {
    crow::json::wvalue out;
    out["id"] = c->getId();
    out["name"] = c->getName();
    out["state"] = c->getStateName();
    out["group"] = !c->getChildren().empty();
    out["approval"] = c->hasApproval();
    out["escalation"] = c->hasEscalation();
    out["groupName"] = c->getParent() ? c->getParent()->getName() : "";
    out["groupId"] = c->getParent() ? c->getParent()->getId() : "";
    out["audited"] = dynamic_cast<AuditedWork<std::string>*>(c) != nullptr;
    
    auto children = c->getChildren();
    crow::json::wvalue list = crow::json::wvalue::list();
    for (std::size_t i = 0; i < children.size(); ++i) {
        list[i] = componentJson(s, children[i]);
    }
    out["children"] = std::move(list);
    
    crow::json::wvalue assignees = crow::json::wvalue::list();
    auto people = c->getAssignees();
    for (std::size_t i = 0; i < people.size(); ++i) {
        crow::json::wvalue p;
        p["id"] = people[i]->getId();
        p["name"] = people[i]->getName();
        assignees[i] = std::move(p);
    }
    out["assignees"] = std::move(assignees);
    
    auto chainJson = [&s](const std::map<std::string, std::vector<std::string>>& chains, const std::string& id) {
        crow::json::wvalue chain = crow::json::wvalue::list();
        auto it = chains.find(id);
        if (it != chains.end()) {
            for (std::size_t i = 0; i < it->second.size(); ++i) {
                crow::json::wvalue person;
                auto p = s.participants.find(it->second[i]);
                person["id"] = it->second[i];
                person["name"] = p == s.participants.end() ? it->second[i] : p->second->getName();
                chain[i] = std::move(person);
            }
        }
        return chain;
    };
    
    out["approvalSequence"] = chainJson(s.approvalSequences, c->getId());
    out["escalationSequence"] = chainJson(s.escalationSequences, c->getId());
    
    auto* approvalContact = c->getApprovalContact();
    auto* escalationContact = c->getEscalationContact();
    out["approvalContact"] = approvalContact ? approvalContact->getName() : "";
    out["escalationContact"] = escalationContact ? escalationContact->getName() : "";
    
    auto audits = c->getAuditLog();
    crow::json::wvalue log = crow::json::wvalue::list();
    for (std::size_t i = 0; i < audits.size(); ++i) {
        log[i] = audits[i];
    }
    out["audit"] = std::move(log);
    
    return out;
}

crow::json::wvalue stateJson(AppState& s) {
    crow::json::wvalue out;
    crow::json::wvalue people = crow::json::wvalue::list();
    std::size_t personIndex = 0;
    
    for (auto& p : s.participants) {
        crow::json::wvalue q;
        q["id"] = p.second->getId();
        q["name"] = p.second->getName();
        q["role"] = s.roles[p.first];
        people[personIndex++] = std::move(q);
    }
    out["participants"] = std::move(people);
    
    if (!s.workflow) {
        out["active"] = false;
        return out;
    }
    
    out["active"] = true;
    out["name"] = s.workflow->getName();
    out["complete"] = s.facade.isComplete(s.workflow);
    out["summary"] = s.facade.getProgressReport(s.workflow);
    out["treeText"] = s.facade.printWorkflow(s.workflow);
    
    crow::json::wvalue history = crow::json::wvalue::list();
    const auto commands = s.workflow->getLog();
    for (std::size_t i = 0; i < commands.size(); ++i) {
        history[i] = commands[i];
    }
    out["history"] = std::move(history);
    
    crow::json::wvalue steps = crow::json::wvalue::list();
    const auto& root = s.workflow->getSteps();
    for (std::size_t i = 0; i < root.size(); ++i) {
        steps[i] = componentJson(s, root[i]);
    }
    out["steps"] = std::move(steps);
    
    return out;
}

crow::response errorResponse(const std::exception& e) {
    crow::json::wvalue body;
    body["error"] = e.what();
    return crow::response(400, body);
}
}

int main() {
    crow::App<LocalCors> app;
    AppState state;
    
    CROW_ROUTE(app, "/")([](const crow::request&, crow::response& res) {
        res.set_static_file_info("public/index.html");
        res.end();
    });
    
    CROW_ROUTE(app, "/api/status")([] {
        crow::json::wvalue r;
        r["status"] = "online";
        r["engine"] = "TIVIDY C++";
        return r;
    });
    
    CROW_ROUTE(app, "/api/<path>").methods(crow::HTTPMethod::OPTIONS)([](const crow::request&, crow::response& res, const std::string&) {
        res.code = 204;
        res.end();
    });
    
    CROW_ROUTE(app, "/api/state")([&state] {
        return stateJson(state);
    });
    
    CROW_ROUTE(app, "/api/assignment/role-count").methods(crow::HTTPMethod::POST)([&state](const crow::request& req) {
        try {
            auto d = crow::json::load(req.body);
            std::string role = d["role"].s();
            int count = 0;
            for (auto& p : state.participants) {
                if (p.second->hasRole(role)) ++count;
            }
            crow::json::wvalue r;
            r["role"] = role;
            r["count"] = count;
            return crow::response(200, r);
        } catch (const std::exception& e) {
            return errorResponse(e);
        }
    });
    
    CROW_ROUTE(app, "/api/participants").methods(crow::HTTPMethod::POST)([&state](const crow::request& req) {
        try {
            auto data = crow::json::load(req.body);
            if (!data || !data.has("participants")) return crow::response(400, "Expected participants array");
            
            std::map<std::string, Participant<std::string>*> next;
            std::map<std::string, std::string> roles;
            std::vector<std::unique_ptr<Participant<std::string>>> owned;
            
            for (auto& p : data["participants"]) {
                std::string id = p["id"].s(), name = p["name"].s(), role = p["role"].s();
                if (id.empty() || name.empty() || role.empty() || next.count(id)) {
                    throw std::runtime_error("Participant id, name and role are required; ids must be unique");
                }
                owned.emplace_back(new Participant<std::string>(id, name));
                owned.back()->addRole(role);
                next[id] = owned.back().get();
                roles[id] = role;
            }
            
            if (next.empty()) throw std::runtime_error("Add at least one participant");
            if (state.workflow) {
                delete state.workflow;
                state.workflow = nullptr;
            }
            state.approvalSequences.clear();
            state.escalationSequences.clear();
            state.owned.swap(owned);
            state.participants.swap(next);
            state.roles.swap(roles);
            
            crow::json::wvalue r;
            r["count"] = state.participants.size();
            r["message"] = "Participants loaded";
            return crow::response(200, r);
        } catch (const std::exception& e) {
            return errorResponse(e);
        }
    });
    
    CROW_ROUTE(app, "/api/participants/import").methods(crow::HTTPMethod::POST)([&state](const crow::request& req) {
        try {
            std::istringstream in(req.body);
            std::string line;
            crow::json::wvalue rows = crow::json::wvalue::list();
            std::size_t n = 0, lineNo = 0;
            
            while (std::getline(in, line)) {
                ++lineNo;
                if (line.empty()) continue;
                std::size_t a = line.find(','), b = a == std::string::npos ? a : line.find(',', a + 1);
                if (a == std::string::npos || b == std::string::npos || line.find(',', b + 1) != std::string::npos) {
                    throw std::runtime_error("Line " + std::to_string(lineNo) + ": use id, name, role");
                }
                auto trim = [](std::string x) {
                    auto f = x.find_first_not_of(" \t\r"), l = x.find_last_not_of(" \t\r");
                    return f == std::string::npos ? std::string() : x.substr(f, l - f + 1);
                };
                crow::json::wvalue p;
                p["id"] = trim(line.substr(0, a));
                p["name"] = trim(line.substr(a + 1, b - a - 1));
                p["role"] = trim(line.substr(b + 1));
                rows[n++] = std::move(p);
            }
            
            crow::json::wvalue body;
            body["participants"] = std::move(rows);
            crow::request converted = req;
            converted.body = body.dump();
            
            // Validate through the same engine-backed participant registration path.
            auto data = crow::json::load(converted.body);
            std::map<std::string, Participant<std::string>*> next;
            std::map<std::string, std::string> roles;
            std::vector<std::unique_ptr<Participant<std::string>>> owned;
            
            for (auto& p : data["participants"]) {
                std::string id = p["id"].s(), name = p["name"].s(), role = p["role"].s();
                if (id.empty() || name.empty() || role.empty() || next.count(id)) {
                    throw std::runtime_error("Participant fields must be non-empty and ids unique");
                }
                owned.emplace_back(new Participant<std::string>(id, name));
                owned.back()->addRole(role);
                next[id] = owned.back().get();
                roles[id] = role;
            }
            
            if (next.empty()) throw std::runtime_error("No participants found");
            if (state.workflow) {
                delete state.workflow;
                state.workflow = nullptr;
            }
            state.approvalSequences.clear();
            state.escalationSequences.clear();
            state.owned.swap(owned);
            state.participants.swap(next);
            state.roles.swap(roles);
            
            crow::json::wvalue r;
            r["count"] = state.participants.size();
            r["message"] = "Participants imported";
            return crow::response(200, r);
        } catch (const std::exception& e) {
            return errorResponse(e);
        }
    });
    
    CROW_ROUTE(app, "/api/workflow/init").methods(crow::HTTPMethod::POST)([&state](const crow::request& req) {
        try {
            auto d = crow::json::load(req.body);
            if (!d) return crow::response(400, "Invalid JSON");
            std::string type = d["type"].s();
            
            if (state.participants.empty()) throw std::runtime_error("Load participants before creating a workflow");
            delete state.workflow;
            state.workflow = nullptr;
            state.approvalSequences.clear();
            state.escalationSequences.clear();
            
            if (type != "custom") {
                WorkflowDirector<std::string> director;
                std::map<std::string, int> counts = {
                    {"incident", 5}, {"hiring", 7}, {"bank-fraud", 9}, {"construction-site", 10},
                    {"healthcare-admission", 11}, {"insurance-claim", 8}, {"it-security", 10},
                    {"logistic-shipment", 9}, {"retail-stock-receiving", 9}, {"agriculture", 9}
                };
                auto it = counts.find(type);
                if (it == counts.end()) throw std::runtime_error("Unknown workflow template: " + type);
                
                std::vector<std::string> ids;
                for (int i = 1; i <= it->second; ++i) ids.push_back("W" + std::to_string(i));
                
                Builder<std::string>& builder = state.facade.getBuilder();
                if (type == "incident") director.constructIncidentResponseWorkflow(builder, ids);
                else if (type == "hiring") director.constructJobApplicationWorkflow(builder, ids);
                else if (type == "bank-fraud") director.constructBankFraudWorkflow(builder, ids);
                else if (type == "construction-site") director.constructConstructionSiteWorkflow(builder, ids);
                else if (type == "healthcare-admission") director.constructHealthcareAdmissionWorkflow(builder, ids);
                else if (type == "insurance-claim") director.constructInsuranceClaimWorkflow(builder, ids);
                else if (type == "it-security") director.constructITSecurityWorkflow(builder, ids);
                else if (type == "logistic-shipment") director.constructLogisticShipmentWorkflow(builder, ids);
                else if (type == "retail-stock-receiving") director.constructRetailStockReceivingWorkflow(builder, ids);
                else director.constructAgricultureWorkflow(builder, ids);
                
                state.workflow = state.facade.getBuilder().getResult();
            } else {
                auto& b = state.facade.getBuilder();
                std::string name = d["name"].s();
                b.reset(name.empty() ? "My Workflow" : name);
                for (auto& p : state.participants) b.addParticipant(p.second);
                
                auto& steps = d["steps"];
                for (auto& step : steps) {
                    if (step["type"].s() == "group") {
                        b.beginGroup(step["id"].s(), step["name"].s());
                        for (auto& child : step["items"]) {
                            b.newWorkItem(child["id"].s(), child["name"].s());
                            if (child["audit"].b()) b.withAudit();
                            if (child["approval"].b()) b.withApproval();
                            if (child["escalation"].b()) b.withEscalation();
                            b.endWorkItem();
                        }
                        b.endGroup();
                    } else {
                        b.newWorkItem(step["id"].s(), step["name"].s());
                        if (step["audit"].b()) b.withAudit();
                        if (step["approval"].b()) b.withApproval();
                        if (step["escalation"].b()) b.withEscalation();
                        b.endWorkItem();
                    }
                }
                state.workflow = b.getResult();
            }
            
            for (auto& p : state.participants) state.workflow->addParticipant(p.second);
            state.facade.startWorkflow(state.workflow);
            return crow::response(200, stateJson(state));
        } catch (const std::exception& e) {
            return errorResponse(e);
        }
    });
    
    CROW_ROUTE(app, "/api/assign").methods(crow::HTTPMethod::POST)([&state](const crow::request& req) {
        try {
            auto d = crow::json::load(req.body);
            auto* target = item(state, d["id"].s());
            std::vector<Participant<std::string>*> approval, escalation, assignees;
            
            if (d.has("approval")) {
                for (auto& x : d["approval"]) approval.push_back(participant(state, x.s()));
            }
            if (d.has("escalation")) {
                for (auto& x : d["escalation"]) escalation.push_back(participant(state, x.s()));
            }
            
            if (d["method"].s() == "manual") {
                for (auto& x : d["participants"]) assignees.push_back(participant(state, x.s()));
                state.facade.assignToParticipants(state.workflow, d["id"].s(), assignees, -1, escalation, approval);
            } else {
                if (!d.has("numAssignees")) throw std::runtime_error("Choose how many participants are needed");
                int required = d["numAssignees"].i();
                if (required < 1) throw std::runtime_error("The number of participants must be a positive whole number");
                
                std::vector<std::unique_ptr<AssignmentStrategy<std::string>>> ownedStrategies;
                std::vector<AssignmentStrategy<std::string>*> strategies;
                
                if (d["leastLoaded"].b()) {
                    ownedStrategies.emplace_back(new LeastLoadedAssignment<std::string>());
                    strategies.push_back(ownedStrategies.back().get());
                }
                if (d["role"].s().size()) {
                    ownedStrategies.emplace_back(new RoleAssignment<std::string>(d["role"].s()));
                    strategies.push_back(ownedStrategies.back().get());
                }
                if (d["rule"].b()) {
                    ownedStrategies.emplace_back(new RuleAssignment<std::string>());
                    strategies.push_back(ownedStrategies.back().get());
                }
                
                if (strategies.empty()) throw std::runtime_error("Choose at least one assignment strategy");
                state.facade.assignWithStrategies(state.workflow, d["id"].s(), strategies, required, escalation, approval);
            }
            
            state.approvalSequences[d["id"].s()].clear();
            state.escalationSequences[d["id"].s()].clear();
            for (auto* p : approval) state.approvalSequences[d["id"].s()].push_back(p->getId());
            for (auto* p : escalation) state.escalationSequences[d["id"].s()].push_back(p->getId());
            
            return crow::response(200, stateJson(state));
        } catch (const std::exception& e) {
            return errorResponse(e);
        }
    });
    
    CROW_ROUTE(app, "/api/command").methods(crow::HTTPMethod::POST)([&state](const crow::request& req) {
        try {
            auto d = crow::json::load(req.body);
            std::string id = d["id"].s(), cmd = d["command"].s();
            item(state, id);
            
            if (cmd == "start") state.facade.startWork(state.workflow, id);
            else if (cmd == "complete") state.facade.completeWork(state.workflow, id);
            else if (cmd == "approve") state.facade.approveWork(state.workflow, id);
            else if (cmd == "reject") state.facade.rejectWork(state.workflow, id);
            else if (cmd == "cancel") state.facade.cancelWork(state.workflow, id);
            else if (cmd == "escalate") state.facade.escalateWork(state.workflow, id);
            else throw std::runtime_error("Unknown command");
            
            return crow::response(200, stateJson(state));
        } catch (const std::exception& e) {
            return errorResponse(e);
        }
    });
    
    CROW_ROUTE(app, "/api/audit/<string>").methods(crow::HTTPMethod::GET)([&state](const std::string& id) {
        try {
            auto* c = item(state, id);
            crow::json::wvalue r;
            auto logs = c->getAuditLog();
            crow::json::wvalue l = crow::json::wvalue::list();
            for (std::size_t i = 0; i < logs.size(); ++i) l[i] = logs[i];
            r["entries"] = std::move(l);
            return crow::response(200, r);
        } catch (const std::exception& e) {
            return errorResponse(e);
        }
    });
    
    CROW_ROUTE(app, "/api/audit/<string>").methods(crow::HTTPMethod::POST)([&state](const crow::request& req, const std::string& id) {
        try {
            auto d = crow::json::load(req.body);
            item(state, id);
            std::string action = d["action"].s();
            
            if (action == "add") {
                std::string entry = d["entry"].s();
                if (entry.empty()) throw std::runtime_error("Diary entry cannot be empty");
                state.facade.addAuditEntry(state.workflow, id, entry);
            } else if (action == "edit") {
                state.facade.editAuditEntry(state.workflow, id, d["index"].i(), d["entry"].s());
            } else if (action == "delete") {
                state.facade.deleteAuditEntry(state.workflow, id, d["index"].i());
            } else {
                throw std::runtime_error("Unknown diary action");
            }
            
            auto logs = item(state, id)->getAuditLog();
            crow::json::wvalue r;
            crow::json::wvalue l = crow::json::wvalue::list();
            for (std::size_t i = 0; i < logs.size(); ++i) l[i] = logs[i];
            r["entries"] = std::move(l);
            return crow::response(200, r);
        } catch (const std::exception& e) {
            return errorResponse(e);
        }
    });
    
    CROW_ROUTE(app, "/api/undo").methods(crow::HTTPMethod::POST)([&state](const crow::request&) {
        try {
            if (!state.workflow) throw std::runtime_error("There is no active workflow");
            const std::vector<std::string> before = state.workflow->getLog();
            bool undone = state.facade.undo(state.workflow);
            auto out = stateJson(state);
            out["undone"] = undone;
            out["undoMessage"] = undone ? "The last C++ engine command was undone." : (before.empty() ? "There is no command to undo." : "The C++ engine refused this undo because the affected work has moved on from the state recorded by that command.");
            return crow::response(200, out);
        } catch (const std::exception& e) {
            return errorResponse(e);
        }
    });
    
    const char* configuredPort = std::getenv("WORKFLOW_PORT");
    app.port(configuredPort ? std::atoi(configuredPort) : 8080).multithreaded().run();
}