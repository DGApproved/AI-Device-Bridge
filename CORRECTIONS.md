# CORRECTIONS.md

**Transparency should not require premature disclosure of vulnerabilities that remain exploitable.**

# AI Device Bridge — Vulnerability Discovery, Corrections, and Resolution History

**Project:** AI Device Bridge  
**Status:** Initial Development / Proof of Concept  
**Initial Reference Hardware:** Hailo-8 M.2 AI Accelerator  
**License:** Apache License 2.0

---

## 1. Purpose

This document maintains the historical record of publicly disclosed security vulnerabilities, corrective actions, mitigations, and resolutions associated with AI Device Bridge.

Its purpose is to help users, developers, maintainers, and future contributors understand:

- What vulnerabilities were discovered.
- Which components and versions were affected.
- How vulnerabilities were investigated.
- What corrective actions were taken.
- Whether temporary mitigations were implemented.
- How corrections were tested.
- When vulnerabilities were considered resolved.
- Whether subsequent findings required additional changes.

This document is intended to preserve useful technical knowledge without unnecessarily exposing users to unresolved security risks.

**The purpose of recording a correction is not merely to establish that something changed, but to explain why the change was necessary and whether it resolved the problem.**

---

## 2. Relationship to SECURITY.md

`SECURITY.md` establishes the project's vulnerability reporting, investigation, correction, and responsible disclosure procedures.

`CORRECTIONS.md` records the relevant historical outcomes of those procedures.

The documents serve different purposes:

| Document | Responsibility |
|---|---|
| `SECURITY.md` | Reporting, handling, and disclosure policies |
| `CORRECTIONS.md` | Historical record of disclosed vulnerabilities, mitigations, corrections, and resolutions |
| `CONTRIBUTING.md` | Contribution procedures and development principles |

Security-sensitive discoveries should follow the procedures in `SECURITY.md` before being documented publicly.

---

## 3. Responsible Disclosure

Public transparency must be balanced against the security of people using the software.

Details of vulnerabilities that remain exploitable should not be published prematurely when disclosure could unnecessarily increase risk.

Vulnerabilities may initially be documented through private security reports, restricted maintainer discussions, or other appropriate confidential channels.

Once disclosure is appropriate, their public history may be recorded here.

Publication may occur after a verified correction, after an effective mitigation, or through an appropriately coordinated disclosure process.

This document must not be treated as a requirement to publish sensitive exploit instructions.

Where disclosure is necessary before full resolution, information should be limited to what users need to understand the risk and protect themselves.

---

## 4. Terminology

To maintain an understandable historical record, the following terms are used throughout this document.

### Vulnerability

A weakness in software, configuration, communication, or integration that may compromise confidentiality, integrity, availability, or another relevant security property.

### Discovery

The identification of a suspected or confirmed vulnerability.

A reported issue is not automatically considered a verified vulnerability.

### Investigation

The process of determining whether a reported vulnerability exists, what causes it, and which components or configurations are affected.

### Mitigation

A temporary or permanent measure that reduces security risk without necessarily correcting the underlying cause.

For example, disabling an affected feature may mitigate a vulnerability while its implementation is being corrected.

### Correction

A change intended to address an identified vulnerability or its underlying cause.

A correction may require further testing before the vulnerability can be considered resolved.

### Resolution

The documented outcome of a vulnerability investigation.

A vulnerability may be considered resolved when an appropriate correction has been verified, the affected functionality has been safely retired, or another justified action has adequately addressed the identified risk.

Resolution does not automatically mean that every related limitation has disappeared.

### Verification

Testing or other evidence used to determine whether a correction or mitigation addresses the identified issue.

### Disclosure

The communication of vulnerability information to affected users, maintainers, or the public.

### Regression

The reintroduction of a previously corrected vulnerability or closely related failure.

---

## 5. Vulnerability Status

Each recorded vulnerability should have a clearly identified status.

| Status | Meaning |
|---|---|
| Reported | A potential vulnerability has been submitted |
| Under Investigation | The issue is being evaluated |
| Confirmed | A vulnerability has been verified |
| Mitigated | Risk has been reduced, but the underlying issue may remain |
| Correction in Progress | A corrective implementation is being developed |
| Correction Available | A proposed correction exists but may require further verification |
| Resolved | The documented resolution has been verified or otherwise justified |
| Reopened | New evidence requires further investigation |
| Not Reproducible | Available evidence has not established the reported behavior |
| Not Applicable | Investigation determined the issue does not affect the identified component or version |

A report's status may change as additional evidence becomes available.

**A vulnerability should not be marked Resolved solely because a code change was submitted or merged.**

Verification and any remaining limitations should be considered.

---

## 6. Historical Record

### Current Record

**No publicly disclosed AI Device Bridge vulnerabilities or verified security corrections to such have been recorded in this document at the time of its initial creation.**

The foundational record, 0000-0000-0000-0001, identifies the risk of losing historical knowledge about flaws, corrections, and resolutions.

This record establishes the purpose and necessity of maintaining a continuing vulnerability history. It is a logically derived architectural concern rather than a verified software exploit.

Subsequent vulnerability discoveries, mitigations, corrections, and resolutions are to be documented within the designated recording area bounded by <!-- RECORD START --> and <!-- RECORD END -->.

The absence of subsequent entries should not be interpreted as a security certification.

*START*
```html
### 0000-0000-0000-0001 — Lack of Tracking of Flaw to Feature Loss Tracking

**Record ID:** 0000-0000-0000-0001  
**Status:** Resolved  
**Severity:** Essential  
**Discovery date:** Project Start  
**Disclosure date:** Initialization of project  
**Resolution date:** Resolved at initialization  
**Affected components:** Entire Project  
**Affected versions:** Every version  
**Affected environments:** None

#### Discovery

Logically derived.

#### Technical Explanation

History repeats when forgotten.

Without a maintained record of discovered flaws, corrections, and the reasons behind implementation changes, future contributors may unknowingly repeat earlier mistakes or reintroduce previously corrected problems.

This entry documents an architectural risk identified at project initialization, rather than a demonstrated software exploit.

#### Potential Impact

While loop.

In this context, a repeated cycle in which an issue is discovered, corrected, forgotten, and subsequently reintroduced.

#### Mitigation

This Document.

#### Correction

Document Exists.

The establishment of `CORRECTIONS.md` provides a designated location for recording vulnerabilities, mitigations, corrections, verification, and resolutions.

#### Verification

Untested.

The existence of this document is established; its long-term effectiveness in preventing repeated problems has not yet been verified.

#### Resolution

This is an example.

The initial documentation gap has been addressed by establishing the historical record. Its effectiveness remains subject to future use and maintenance.

#### Remaining Limitations

Documents work.

Documentation requires ongoing maintenance, accurate contributions, and continued use to remain effective.

#### Related References

Historical.

#### Historical Updates

Exclusive Forgetfulness.
```

The following recording area is reserved for subsequent vulnerability discoveries and their documented mitigations, corrections, and resolutions.
<!-- RECORD START -->

<!-- RECORD END -->
This identifies the underlying concern that necessitates preserving subsequent vulnerability records. The absence of recorded history can allow previously discovered flaws, abandoned corrections, and their underlying reasoning to be forgotten.
*END*

---

## 7. Vulnerability Record Format

Each public record should use a consistent structure.

The following template may be copied when documenting a new issue.

### AIDB-SEC-YYYY-NNN — Vulnerability Title

**Record ID:** AIDB-SEC-YYYY-NNN  
**Status:** Reported / Investigating / Mitigated / Resolved / Other  
**Severity:** Critical / High / Medium / Low / Informational / Undetermined  
**Discovery date:** YYYY-MM-DD  
**Disclosure date:** YYYY-MM-DD or Not yet publicly disclosed  
**Resolution date:** YYYY-MM-DD or Not resolved

**Affected components:**  
Identify the relevant bridge core, backend, interface, configuration system, or other affected component.

**Affected versions:**  
Identify known affected releases or commit ranges.

**Affected environments:**  
Record relevant operating systems, hardware models, drivers, runtimes, or configuration conditions.

#### Discovery

Explain how the issue was identified and what evidence established that it was a vulnerability.

Do not unnecessarily publish sensitive exploit details.

#### Technical explanation

Describe the underlying cause and relevant implementation behavior.

Distinguish confirmed findings from assumptions or incomplete investigations.

#### Potential impact

Describe the security consequences and conditions under which the issue may affect users.

#### Mitigation

Describe temporary or permanent risk-reduction measures, if any.

If no mitigation is available, state that clearly.

#### Correction

Describe the implementation change intended to address the vulnerability.

Include relevant code references where appropriate.

#### Verification

Describe how the correction was tested.

Identify any environments or configurations that remain untested.

#### Resolution

Explain the final or current outcome.

State whether the vulnerability was corrected, mitigated, retired with the affected functionality, or remains unresolved.

#### Remaining limitations

Document any known restrictions, dependencies, or additional work required.

#### Related references

Link to relevant public Pull Requests, commits, releases, advisories, or other documentation.

#### Historical updates

Record significant later findings, reopened investigations, regressions, or revised explanations.

---

## 8. Recording Corrections and Resolutions

A correction and a resolution should be recorded separately when they represent different events.

For example:

1. A vulnerability is reported.
2. An investigation confirms the issue.
3. A temporary mitigation is introduced.
4. A correction is developed.
5. The correction is tested.
6. A corrected release becomes available.
7. The vulnerability is declared resolved after appropriate verification.
8. The public historical record is updated.

These steps may overlap or occur in a different order depending on the circumstances.

An issue may remain mitigated for an extended period while a permanent correction is being developed.

A correction may also need to be revised after additional testing.

The historical record should reflect what actually occurred rather than implying that every vulnerability followed an idealized process.

---

## 9. Historical Accuracy

This document should preserve accurate information about vulnerability discovery, corrective work, and resolution decisions.

Historical entries may be expanded or corrected when new evidence becomes available.

Where practical, significant revisions should be explained rather than silently replacing earlier conclusions.

For example:

- A vulnerability initially believed to affect one runtime version may later be found in additional versions.
- A correction may prove incomplete.
- A previously resolved issue may reappear under different conditions.
- An apparent security vulnerability may later be identified as a non-security defect.
- A temporary mitigation may eventually become the permanent resolution.

If an earlier conclusion was inaccurate, the record should explain the revised understanding.

Git history may preserve the sequence of document changes, while the written record should explain their technical significance.

The objective is an accurate and understandable account, not a demonstration that every earlier decision was correct.

---

## 10. Third-Party Vulnerabilities

AI Device Bridge depends on existing operating systems, hardware drivers, runtimes, and other software components.

Some vulnerabilities may originate outside the project's own code.

Examples include:

- Manufacturer-provided device drivers.
- Accelerator firmware.
- Inference runtimes.
- Model parsing libraries.
- Operating-system interfaces.
- External dependencies.

Vulnerabilities affecting third-party components should ordinarily be reported to their respective maintainers through appropriate channels.

Where a third-party vulnerability materially affects AI Device Bridge, the project may document its impact, applicable mitigations, supported dependency versions, and relevant public advisories.

Such records should distinguish between:

**Upstream correction:** A security fix implemented by the external software maintainer.

**Bridge mitigation:** A change within AI Device Bridge that reduces exposure to the external vulnerability.

**Bridge resolution:** The verified outcome for AI Device Bridge users after appropriate updates, mitigations, or configuration changes.

AI Device Bridge should not claim to have corrected vulnerabilities within third-party software when it has only implemented a workaround.

---

## 11. Compatibility and Regression History

Security corrections should be evaluated for their effects on existing functionality.

A correction that addresses one problem may introduce compatibility problems elsewhere.

Where relevant, records should include:

- Changes to supported device configurations.
- Changes to runtime compatibility.
- API modifications.
- Model-loading restrictions.
- Configuration migrations.
- Performance or resource-management effects.
- Regressions identified after release.

A change should not be considered fully understood merely because the original security test passes.

Testing should include relevant existing functionality wherever practical.

If a correction creates a new issue, that issue should receive appropriate tracking and documentation.

---

## 12. Relationship to Development Comments

The project's contribution guidelines permit comments explaining historical implementations and their underlying reasoning.

Such comments may help explain why a vulnerability existed or why a correction was selected.

However, source-code comments and this document serve different purposes.

**Source-code comments** may explain local implementation decisions.

**CORRECTIONS.md** preserves the broader security history, including discovery, impact, mitigation, verification, and resolution.

Neither format is a substitute for appropriate testing.

Contributors are not required to reproduce the entire vulnerability history inside source-code comments.

Where useful, comments may reference a corresponding public vulnerability record.

---

## 13. Maintaining This Document

Contributors may propose corrections to this document through the normal contribution process, provided those changes do not prematurely disclose sensitive vulnerability information.

Maintainers should review entries for:

- Factual accuracy.
- Clear identification of affected versions.
- Appropriate disclosure.
- Correct status.
- Evidence supporting the stated resolution.
- Relevant testing information.
- Consistency with `SECURITY.md`.

A record does not need to be lengthy to be useful.

Concise, accurate documentation is preferable to speculative technical explanations.

Incomplete information should be identified as incomplete rather than filled with unsupported assumptions.

### Historical continuity

Entries should normally remain available after resolution.

Removing an entry solely because its vulnerability has been corrected would undermine the purpose of maintaining a historical record.

However, inaccurate, duplicated, or unnecessarily sensitive information may be revised or removed when appropriate.

Significant changes should be documented where practical.

---

## 14. Future Development

As AI Device Bridge grows, this document may evolve to include:

- Public vulnerability identifiers.
- Links to GitHub Security Advisories.
- CVE references where applicable.
- Release-specific correction summaries.
- Backend-specific security histories.
- Automated regression-test references.
- Documentation of resolved dependency vulnerabilities.
- Compatibility information for affected hardware.

These features are development possibilities, not guarantees that such infrastructure already exists.

The format may be refined through contributor feedback.

---

## 15. Project Security Philosophy

AI Device Bridge is intended to improve interoperability between existing AI hardware and software systems.

Its security practices should preserve the protections provided by those systems while allowing the bridge's own implementation to be examined, corrected, and improved.

Contributors are encouraged to question existing designs, identify weaknesses, propose alternatives, and document the reasoning behind corrections.

No implementation should be presumed immune to error.

No contributor should be discouraged from reporting a genuine concern because they lack professional credentials.

The project follows the principle:

**Ask why before assuming, explain why when deciding, and remain willing to reconsider when better evidence or implementations become available.**

Security history should be preserved to help future contributors understand both successful corrections and the limitations of earlier approaches.

**Transparency should not require premature disclosure of vulnerabilities that remain exploitable.**

**The objective is not to replace what already works. It is to help more of what already works function together.**
