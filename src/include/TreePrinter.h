#ifndef TREEPRINTER_H
#define TREEPRINTER_H

#include <string>
#include <vector>
#include "WorkVisitor.h"

namespace tividy {

/**
 * @brief ConcreteVisitor: prints the structure of the workflow as indented text. Groups are printed as `+ [Group] name (id)`, items as `* name (id) [State]`, indented by their depth; decorators add tags such as {Audited, Escalation, Approval} to their item's line, and assignees are listed after `<-`. Unlike ProgressReport it does not count; it shows the shape and the state of every component.
 */
template <typename T>
class TreePrinter : public WorkVisitor<T> {
public:
    /** @brief Creates an empty printer. */
    TreePrinter();

    /** @brief Prints a line (items, groups) or remembers a tag (decorators). */
    void visit(WorkItem<T>& item) override;
    /** @brief Prints a line (items, groups) or remembers a tag (decorators). */
    void visit(WorkGroup<T>& group) override;
    /** @brief Prints a line (items, groups) or remembers a tag (decorators). */
    void visit(AuditedWork<T>& decorator) override;
    /** @brief Prints a line (items, groups) or remembers a tag (decorators). */
    void visit(EscalatedWork<T>& decorator) override;
    /** @brief Prints a line (items, groups) or remembers a tag (decorators). */
    void visit(ApprovedWork<T>& decorator) override;

    /** @brief Returns the text printed so far. */
    const std::string& getOutput() const;

private:
    /** @brief Returns the spaces for a depth. */
    static std::string indentation(int depth);

    std::string output;  ///< Accumulated text.
    std::vector<std::string> tags;  ///< Decorator tags waiting for the next item line.
};

}

#endif
