#include <gtest/gtest.h>
#include <string>

#include "WorkVisitor.h"
#include "ProgressReport.h"
#include "TreePrinter.h"
#include "WorkItem.h"
#include "WorkGroup.h"
#include "AuditedWork.h"

using namespace tividy;

TEST(VisitorPatternTest, ProgressReportVisitorCounting) {
    ProgressReport<int> report;
    EXPECT_EQ(report.getTotalItems(), 0);
    EXPECT_EQ(report.getCompletedItems(), 0);

    WorkItem<int> item1(1, "Item1");
    WorkItem<int> item2(2, "Item2");

    report.visit(item1);
    report.visit(item2);

    EXPECT_EQ(report.getTotalItems(), 2);
    EXPECT_EQ(report.getCompletedItems(), 0);
    EXPECT_EQ(report.getSummary(), "0/2 items completed (Created: 2)");
}

TEST(VisitorPatternTest, TreePrinterIndentationAndTags) {
    TreePrinter<int> printer;
    WorkGroup<int> group(10, "PipelineGroup");
    WorkItem<int>* leaf = new WorkItem<int>(1, "AuditTask");
    AuditedWork<int> audited(leaf);

    printer.visit(group);
    printer.visit(audited);
    printer.visit(*leaf);

    std::string output = printer.getOutput();
    EXPECT_NE(output.find("+ [Group] PipelineGroup (10)"), std::string::npos);
    EXPECT_NE(output.find("* AuditTask (1) [Created] {Audited}"), std::string::npos);
}