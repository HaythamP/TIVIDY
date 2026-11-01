#include "WorkflowDirector.h"
#include "InvalidWorkflowException.h"
#include <string>

namespace tividy {

template <typename T>
WorkflowDirector<T>::WorkflowDirector() {}

template <typename T>
void WorkflowDirector<T>::constructIncidentResponseWorkflow(Builder<T>& builder, const std::vector<T>& ids) const {
    if (ids.size() != 5) {
        throw InvalidWorkflowException("The Incident Response workflow requires exactly 5 ids");
    }
    builder.reset("Incident Response (Power Outage)");
    builder.newWorkItem(ids[0], "Dispatch Team").endWorkItem();
    builder.newWorkItem(ids[1], "Investigation").withAudit().endWorkItem();
    builder.newWorkItem(ids[2], "Containment").endWorkItem();
    builder.newWorkItem(ids[3], "Repair").withAudit().withEscalation().withApproval().endWorkItem();
    builder.newWorkItem(ids[4], "Restore").endWorkItem();
}

template <typename T>
void WorkflowDirector<T>::constructJobApplicationWorkflow(Builder<T>& builder, const std::vector<T>& ids) const {
    if (ids.size() != 7) {
        throw InvalidWorkflowException("The Job Application workflow requires exactly 7 ids");
    }
    builder.reset("Job Application (Tech Company)");

    builder.beginGroup(ids[0], "Initial Screening");
    builder.newWorkItem(ids[1], "Application Review").withAudit().endWorkItem();
    builder.newWorkItem(ids[2], "Reference Check").withAudit().endWorkItem();
    builder.endGroup();

    builder.beginGroup(ids[3], "Interview");
    builder.newWorkItem(ids[4], "Technical Interview").withAudit().endWorkItem();
    builder.newWorkItem(ids[5], "Behavioural Interview").withAudit().endWorkItem();
    builder.endGroup();

    builder.newWorkItem(ids[6], "Final Decision").withApproval().endWorkItem();
}

template <typename T>
void WorkflowDirector<T>::constructBankFraudWorkflow(Builder<T>& builder, const std::vector<T>& ids) const {
    if (ids.size() != 9) throw InvalidWorkflowException("The Bank Fraud workflow requires exactly 9 ids");
    builder.reset("Bank Fraud Response");
    builder.beginGroup(ids[0], "Alert Triage");
    builder.newWorkItem(ids[1], "Validate Fraud Alert").withAudit().endWorkItem();
    builder.newWorkItem(ids[2], "Secure Customer Account").withEscalation().endWorkItem();
    builder.newWorkItem(ids[3], "Notify Fraud Operations").endWorkItem();
    builder.endGroup();
    builder.beginGroup(ids[4], "Investigation");
    builder.newWorkItem(ids[5], "Review Transaction Evidence").withAudit().endWorkItem();
    builder.newWorkItem(ids[6], "Contact Customer").withAudit().endWorkItem();
    builder.endGroup();
    builder.newWorkItem(ids[7], "Approve Recovery Action").withApproval().withAudit().endWorkItem();
    builder.newWorkItem(ids[8], "Close Fraud Case").withAudit().endWorkItem();
}

template <typename T>
void WorkflowDirector<T>::constructConstructionSiteWorkflow(Builder<T>& builder, const std::vector<T>& ids) const {
    if (ids.size() != 10) throw InvalidWorkflowException("The Construction Site workflow requires exactly 10 ids");
    builder.reset("Construction Site Delivery");
    builder.beginGroup(ids[0], "Mobilisation");
    builder.newWorkItem(ids[1], "Confirm Permits and Site Access").withAudit().endWorkItem();
    builder.newWorkItem(ids[2], "Complete Safety Induction").withAudit().endWorkItem();
    builder.endGroup();
    builder.beginGroup(ids[3], "Site Execution");
    builder.newWorkItem(ids[4], "Prepare Work Area").withAudit().endWorkItem();
    builder.newWorkItem(ids[5], "Complete Construction Work").withEscalation().withAudit().endWorkItem();
    builder.newWorkItem(ids[6], "Inspect Workmanship").withAudit().endWorkItem();
    builder.endGroup();
    builder.beginGroup(ids[7], "Handover");
    builder.newWorkItem(ids[8], "Resolve Snag List").withAudit().endWorkItem();
    builder.newWorkItem(ids[9], "Approve Site Handover").withApproval().endWorkItem();
    builder.endGroup();
}

template <typename T>
void WorkflowDirector<T>::constructHealthcareAdmissionWorkflow(Builder<T>& builder, const std::vector<T>& ids) const {
    if (ids.size() != 11) throw InvalidWorkflowException("The Healthcare Admission workflow requires exactly 11 ids");
    builder.reset("Healthcare Admission");
    builder.beginGroup(ids[0], "Pre-admission Checks");
    builder.newWorkItem(ids[1], "Verify Patient Identity").withAudit().endWorkItem();
    builder.newWorkItem(ids[2], "Confirm Coverage and Consent").withAudit().endWorkItem();
    builder.endGroup();
    builder.beginGroup(ids[3], "Clinical Assessment");
    builder.newWorkItem(ids[4], "Complete Nursing Assessment").withAudit().endWorkItem();
    builder.newWorkItem(ids[5], "Review Medication and Risks").withAudit().withEscalation().endWorkItem();
    builder.endGroup();
    builder.newWorkItem(ids[6], "Assign Bed and Care Team").withAudit().endWorkItem();
    builder.newWorkItem(ids[7], "Confirm Admission Plan").withApproval().endWorkItem();
    builder.beginGroup(ids[8], "Care Handoff");
    builder.newWorkItem(ids[9], "Notify Receiving Unit").withAudit().endWorkItem();
    builder.newWorkItem(ids[10], "Complete Patient Handoff").withAudit().endWorkItem();
    builder.endGroup();
}

template <typename T>
void WorkflowDirector<T>::constructInsuranceClaimWorkflow(Builder<T>& builder, const std::vector<T>& ids) const {
    if (ids.size() != 8) throw InvalidWorkflowException("The Insurance Claim workflow requires exactly 8 ids");
    builder.reset("Insurance Claim Assessment");
    builder.beginGroup(ids[0], "Claim Intake");
    builder.newWorkItem(ids[1], "Register Claim and Policy Details").withAudit().endWorkItem();
    builder.newWorkItem(ids[2], "Check Required Documents").withAudit().endWorkItem();
    builder.endGroup();
    builder.beginGroup(ids[3], "Assessment");
    builder.newWorkItem(ids[4], "Verify Loss and Evidence").withAudit().endWorkItem();
    builder.newWorkItem(ids[5], "Estimate Covered Damage").withAudit().withEscalation().endWorkItem();
    builder.endGroup();
    builder.newWorkItem(ids[6], "Approve Claim Decision").withApproval().withAudit().endWorkItem();
    builder.newWorkItem(ids[7], "Issue Settlement and Close Claim").withAudit().endWorkItem();
}

template <typename T>
void WorkflowDirector<T>::constructITSecurityWorkflow(Builder<T>& builder, const std::vector<T>& ids) const {
    if (ids.size() != 10) throw InvalidWorkflowException("The IT Security workflow requires exactly 10 ids");
    builder.reset("IT Security Incident Response");
    builder.beginGroup(ids[0], "Triage and Containment");
    builder.newWorkItem(ids[1], "Validate Security Alert").withAudit().endWorkItem();
    builder.newWorkItem(ids[2], "Classify Incident Severity").withAudit().endWorkItem();
    builder.newWorkItem(ids[3], "Contain Affected Systems").withEscalation().withAudit().endWorkItem();
    builder.endGroup();
    builder.beginGroup(ids[4], "Investigation and Recovery");
    builder.newWorkItem(ids[5], "Investigate Root Cause").withAudit().endWorkItem();
    builder.newWorkItem(ids[6], "Remove Threat and Restore Services").withAudit().endWorkItem();
    builder.newWorkItem(ids[7], "Verify Security Controls").withAudit().endWorkItem();
    builder.endGroup();
    builder.newWorkItem(ids[8], "Approve Return to Service").withApproval().endWorkItem();
    builder.newWorkItem(ids[9], "Record Lessons and Close Incident").withAudit().endWorkItem();
}

template <typename T>
void WorkflowDirector<T>::constructLogisticShipmentWorkflow(Builder<T>& builder, const std::vector<T>& ids) const {
    if (ids.size() != 9) throw InvalidWorkflowException("The Logistic Shipment workflow requires exactly 9 ids");
    builder.reset("Logistic Shipment Fulfilment");
    builder.beginGroup(ids[0], "Dispatch Preparation");
    builder.newWorkItem(ids[1], "Verify Order and Inventory").withAudit().endWorkItem();
    builder.newWorkItem(ids[2], "Book Carrier and Route").withAudit().endWorkItem();
    builder.endGroup();
    builder.beginGroup(ids[3], "Transport");
    builder.newWorkItem(ids[4], "Release Shipment").withAudit().endWorkItem();
    builder.newWorkItem(ids[5], "Track Transit and Exceptions").withEscalation().withAudit().endWorkItem();
    builder.endGroup();
    builder.beginGroup(ids[6], "Delivery and Reconciliation");
    builder.newWorkItem(ids[7], "Confirm Proof of Delivery").withAudit().endWorkItem();
    builder.newWorkItem(ids[8], "Reconcile Shipment and Close").withAudit().endWorkItem();
    builder.endGroup();
}

template <typename T>
void WorkflowDirector<T>::constructRetailStockReceivingWorkflow(Builder<T>& builder, const std::vector<T>& ids) const {
    if (ids.size() != 9) throw InvalidWorkflowException("The Retail Stock Receiving workflow requires exactly 9 ids");
    builder.reset("Retail Stock Receiving");
    builder.beginGroup(ids[0], "Arrival and Check-in");
    builder.newWorkItem(ids[1], "Register Delivery").withAudit().endWorkItem();
    builder.newWorkItem(ids[2], "Inspect Packaging and Seal").withAudit().endWorkItem();
    builder.endGroup();
    builder.beginGroup(ids[3], "Quantity and Quality Verification");
    builder.newWorkItem(ids[4], "Match Items to Purchase Order").withAudit().endWorkItem();
    builder.newWorkItem(ids[5], "Record Damaged or Missing Stock").withAudit().withEscalation().endWorkItem();
    builder.endGroup();
    builder.beginGroup(ids[6], "Stock Placement");
    builder.newWorkItem(ids[7], "Update Inventory Records").withAudit().endWorkItem();
    builder.newWorkItem(ids[8], "Approve Stock for Sale").withApproval().endWorkItem();
    builder.endGroup();
}

template <typename T>
void WorkflowDirector<T>::constructAgricultureWorkflow(Builder<T>& builder, const std::vector<T>& ids) const {
    if (ids.size() != 9) throw InvalidWorkflowException("The Agriculture workflow requires exactly 9 ids");
    builder.reset("Agriculture Crop Cycle");
    builder.beginGroup(ids[0], "Crop Planning");
    builder.newWorkItem(ids[1], "Assess Field and Soil Conditions").withAudit().endWorkItem();
    builder.newWorkItem(ids[2], "Approve Crop and Input Plan").withApproval().endWorkItem();
    builder.endGroup();
    builder.beginGroup(ids[3], "Field Operations");
    builder.newWorkItem(ids[4], "Prepare and Plant Fields").withAudit().endWorkItem();
    builder.newWorkItem(ids[5], "Monitor Crop Health and Irrigation").withAudit().withEscalation().endWorkItem();
    builder.endGroup();
    builder.beginGroup(ids[6], "Harvest and Storage");
    builder.newWorkItem(ids[7], "Harvest and Grade Produce").withAudit().endWorkItem();
    builder.newWorkItem(ids[8], "Record Yield and Storage").withAudit().endWorkItem();
    builder.endGroup();
}

template class WorkflowDirector<int>;
template class WorkflowDirector<std::string>;

}
