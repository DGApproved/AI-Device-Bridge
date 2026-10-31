# Contributing to AI Device Bridge

Thank you for your interest in contributing to **AI Device Bridge**!

AI Device Bridge is an independent, community-developed, open-source project intended to improve interoperability between AI applications, accelerators, processors, and the software environments that support them.

The project begins with the Hailo-8 M.2 neural accelerator as its first reference implementation, but its architecture is intended to accommodate additional devices and software ecosystems over time.

Contributions of all kinds are welcome, including code, documentation, testing, ideas, questions, corrections, and alternative implementations.

**You do not need to be a professional developer to contribute.**

You also do not need to assume that the project's existing implementations are perfect, or that every design decision was made for a reason that cannot be questioned.

---

## 1. Development Philosophy

The project's central objective is:

**The objective is not to replace what already works. It is to help more of what already works function together.**

This principle applies both to the software we develop and to how we work with one another.

We encourage contributors to follow a simple rule:

**Ask why before assuming, explain why when deciding, and remain willing to reconsider when better evidence or implementations become available.**

An unusual design decision may be intentional, experimental, or simply something that has not yet been improved.

Questions about existing code are welcome.

Constructive disagreement is not considered disrespectful, and suggesting a different approach is not an assertion that the original contributor was wrong to explore theirs.

The goal is to improve the project through shared understanding.

---

## 2. Who Can Contribute?

Anyone interested in the project's purpose is welcome to participate.

Contributors may include:

- Hobbyists and independent developers.
- Students and people learning programming.
- Experienced software engineers.
- AI researchers and model developers.
- Hardware enthusiasts.
- Operating-system and driver developers.
- Documentation writers and testers.
- Users who discover bugs or identify usability problems.

Professional credentials are not required.

Contributions should be considered according to their relevance, correctness, usefulness, maintainability, and compatibility with the project's objectives.

A small correction can be as valuable as a major feature.

---

## 3. Ways to Contribute

### Report bugs

If something does not work as expected, open a GitHub Issue describing the problem.

Where possible, include:

- Operating system and version.
- Hardware model.
- Driver and runtime versions.
- AI Device Bridge version or commit.
- Steps needed to reproduce the issue.
- Expected behavior.
- Actual behavior.
- Relevant diagnostic output.

Please remove passwords, access tokens, personal information, and other sensitive material from logs before sharing them.

### Suggest improvements

You may propose:

- More efficient implementations.
- Simpler code structures.
- Improved device detection.
- More reliable configuration handling.
- Better diagnostics.
- New operating profiles.
- Additional application interfaces.
- Compatibility improvements.
- Changes to existing architectural decisions.

You do not need to implement a suggestion before discussing it.

An Issue can be used to explore an idea before it becomes a Pull Request.

### Improve documentation

Documentation contributions are welcome, including corrections, examples, tutorials, troubleshooting guidance, and explanations of unfamiliar concepts.

Please distinguish between verified capabilities, experimental functionality, and proposed future features.

### Test hardware and runtimes

Testing on different operating systems, HailoRT versions, and AI accelerators will be particularly valuable.

Reports of successful and unsuccessful tests can help establish reliable compatibility documentation.

### Develop new backends

The long-term framework is intended to support independently developed accelerator backends.

Contributors interested in additional hardware should coordinate through an Issue or discussion before undertaking substantial architectural changes.

---

## 4. Getting Started

The project's initial implementation is being developed in C++, with CMake proposed as its primary build system.

As the source code becomes available, platform-specific build instructions and dependency requirements will be maintained in the README and project documentation.

A typical development workflow will be:

1. Read the README and relevant documentation.
2. Review existing Issues and Pull Requests.
3. Fork the repository or create a development branch.
4. Make and test your changes.
5. Document any important design decisions.
6. Submit a Pull Request describing the improvement.

If you are unsure where to begin, opening an Issue with a question is entirely appropriate.

There is no requirement to understand the entire codebase before contributing to an individual component.

---

## 5. Questions About Existing Decisions

Contributors are encouraged to ask why a component was implemented in a particular way.

For example:

- Was this design chosen because of a hardware limitation?
- Does this behavior depend on a specific runtime version?
- Could a standard library provide the same functionality?
- Is this implementation intentionally platform-specific?
- Would another approach be easier to maintain?
- Is this an established requirement or an experimental decision?

Questions should be answered constructively whenever possible.

If a design choice has an important technical reason, documenting that reason is encouraged.

If an alternative proves simpler, safer, more reliable, or better suited to the project's goals, it should receive fair consideration.

**Existing code is not automatically the final authority on how something should be implemented.**

At the same time, a proposed improvement should account for compatibility requirements and existing functionality.

---

## 6. Technical Development Principles

AI Device Bridge is designed to complement existing hardware and software ecosystems.

Contributions should follow these principles.

### Preserve official software responsibilities

Use documented manufacturer interfaces and supported runtimes wherever practical.

Do not unnecessarily replace existing drivers, firmware, inference engines, or model compilers.

### Keep the common core hardware-independent

Hardware-specific functionality should remain within the appropriate backend.

Adding support for a new accelerator should not require redesigning every application-facing interface.

### Respect device capabilities

Do not advertise unsupported or unverified operations as available.

A device's theoretical computational potential should not be confused with functionality accessible through its documented software stack.

### Preserve application independence

AI Device Bridge should remain usable without any particular client application.

Example applications, including Fishbowl and GoddessMatrix integrations, should not become mandatory dependencies of the framework.

### Avoid disruptive automation

Automatic configuration should not silently replace functioning drivers, flash firmware, modify critical system settings, or interfere with existing workloads.

Potentially disruptive operations should require informed user approval.

### Prefer understandable implementations

Simple, well-documented code is generally preferable to unnecessary complexity.

Optimization is welcome when it produces meaningful benefits, but those benefits should be demonstrable where practical.

### Maintain diagnostics

Errors should help users and developers determine whether a problem originates in the bridge, a backend, the runtime, the model, or the hardware.

### Respect cross-platform differences

Linux is the initial development priority.

Platform-specific functionality should be isolated so that future Windows, macOS, and other operating-system integrations can be developed without unnecessarily weakening existing implementations.

---

## 7. Code Contributions

### Keep changes focused

Where practical, submit changes that address a specific issue or improvement.

Separating unrelated changes makes review, testing, and maintenance easier.

### Document important behavior

Comments should explain behavior that may otherwise be difficult to understand, particularly hardware requirements, compatibility restrictions, and non-obvious design decisions.

Avoid unnecessary comments that merely repeat what the code already expresses.

### Preserve existing functionality

Changes should avoid breaking working features unless the break is necessary, documented, and accepted.

When changing a public API, explain the compatibility implications.

### Include appropriate tests

Where practical, provide tests or reproducible instructions demonstrating that your change works.

Hardware-specific features should document the hardware, operating system, driver, and runtime versions used during testing.

Testing without physical hardware is valuable, but it should not be presented as verification on a real device.

### Avoid unrelated dependencies

New dependencies should provide a clear benefit and should be compatible with the project's licensing and distribution goals.

Dependencies requiring proprietary software should be documented and, where practical, isolated within the applicable backend.

Including comments in the code that reveal the logic behind implementation's use and detailing previous implementations is not discouraged.
- note: comments are not code, they are logical reference points, and they may be removed when they no longer appear relevant to the current build.
- one possible comment:
```cpp
/*
 * Historical implementation:
 *
 * Earlier versions initialized a separate inference context
 * for each request.
 *
 * The current implementation reuses an initialized context
 * to reduce repeated setup overhead.
 *
 * The earlier approach may still be useful for isolated
 * testing or investigating runtime compatibility.
 */
```
Comments are not mandatory.
- Different contributors have different development processes and documentation preferences. Some 
may prefer to document their reasoning directly within the source code, while others may add 
explanations during later revisions or code reviews.

- Contributors are welcome to add, revise, or remove comments where appropriate. The absence of 
comments should not, by itself, be considered a deficiency in a contribution.

- However, important compatibility requirements, non-obvious behavior, and architectural decisions 
should remain understandable and documented where necessary, whether through code comments, 
project documentation, or other accessible references.

---

## 8. Accelerator Backend Contributions

AI Device Bridge is intended to support different AI processing architectures through modular integrations.

A new backend should ideally provide:

- Device identification.
- Driver and runtime detection.
- Capability reporting.
- Model or workload compatibility checks.
- Model initialization and release.
- Inference execution through supported APIs.
- Diagnostic reporting.
- Clear error handling.
- Documentation for installation and testing.

Not all accelerators expose identical functionality.

Backends should report their actual capabilities rather than simulate unsupported features merely to satisfy a common interface.

Where an operation is unsupported, an explicit unsupported-operation response is preferable to silently performing an unrelated action.

### Hailo-8 reference backend

The first backend will target the Hailo-8 M.2 using the official HailoRT runtime and compatible HEF models.

Hailo-specific implementations should remain separate from hardware-independent bridge components.

Changes to this backend should preserve compatibility with documented Hailo software interfaces and identify the runtime versions against which they were tested.

---

## 9. Pull Requests

When submitting a Pull Request, please include a description of the change and its purpose.

A useful Pull Request explains:

- **What changed?** Identify the components or behavior affected.
- **Why was it changed?** Explain the problem or improvement.
- **How was it tested?** Describe automated tests, manual tests, or relevant hardware testing.
- **What might be affected?** Mention compatibility considerations, dependencies, or limitations.
- **What remains unfinished?** Identify known limitations or future work.

Screenshots may be helpful for interface changes, while logs or sample outputs may be helpful for diagnostic features.

An initial implementation does not need to be perfect to be worth discussing.

Draft Pull Requests are welcome for work that is incomplete or exploratory.

Submission does not guarantee that a change will be merged. Maintainers may request revisions, additional testing, or a different approach to preserve the project's overall architecture.

Decisions should be explained constructively.

---

## 10. Experimental Features and Alternative Implementations

Experimentation is encouraged.

Some contributions may introduce ideas that are not yet suitable for the main branch but could become useful after additional testing.

Possible approaches include:

- Experimental branches.
- Forks.
- Draft Pull Requests.
- Prototype backends.
- Optional feature flags.
- Independent demonstrations.

A contributor may explore an alternative architecture without immediately requiring the main project to adopt it.

Successful experiments should be evaluated according to their technical merits and compatibility with the project's objectives.

**A different approach is not automatically a competing approach.**

Multiple implementations may reveal useful information about efficiency, compatibility, reliability, and maintainability.

Where appropriate, comparisons between implementations can help validate results and identify unintended differences.

---

## 11. Licensing and Third-Party Components

AI Device Bridge is distributed under the Apache License, Version 2.0.

By submitting a contribution, you agree that it may be distributed under the project's license, unless a separate arrangement has been explicitly established.

Please ensure that you have the necessary rights to contribute any submitted code, documentation, or other materials.

Do not include:

- Proprietary drivers without redistribution permission.
- Restricted firmware images.
- HEF files or models whose licenses prohibit redistribution.
- Copyrighted code copied from incompatible sources.
- Private credentials or access tokens.
- Confidential third-party information.

Manufacturer-provided software should generally remain independently installed and governed by its own licensing terms.

Contributors should disclose relevant third-party dependencies and licensing considerations.

---

## 12. Respectful Collaboration

Contributors are expected to participate respectfully.

Technical criticism is welcome. Personal attacks, harassment, threats, discrimination, and deliberately disruptive behavior are not.

Disagreements should focus on evidence, implementation details, and project requirements rather than assumptions about another contributor's knowledge or intentions.

Questions from inexperienced contributors should receive the same basic consideration as questions from experienced developers.

Please be patient with incomplete explanations, unfamiliar terminology, and differing approaches to problem-solving.

The purpose of review is to improve the work and share understanding.

---

## 13. Security and Responsible Reporting

Security-sensitive issues should not be accompanied by public exploit details that could unnecessarily expose users to harm.

If a private vulnerability-reporting channel is available through the repository, use it for suspected vulnerabilities.

Otherwise, contact the maintainers through a designated private reporting method once one has been documented.

The project should not request that users disable essential system security protections merely to simplify installation or configuration.

Hardware and driver modifications should be approached cautiously.

---

## 14. Project Direction and Maintenance

AI Device Bridge is intended to evolve through incremental, verifiable development.

The initial Hailo-8 implementation is a proof of concept, not a permanent limitation of the framework.

As additional hardware backends are developed, some architectural assumptions may need to change.

Major decisions should consider:

- Compatibility with existing applications.
- Stability of public interfaces.
- Maintainability.
- Hardware and runtime requirements.
- User expectations.
- Development effort.
- The project's complementary rather than replacement-oriented purpose.

Not every proposed feature must become part of the core application.

Some capabilities may be more appropriate as optional plugins, independent integrations, or separate projects.

---

## 15. A Note to Future Contributors

This project begins as an independent development effort rather than a claim of professional authority or a declaration that its first implementation is the best possible one.

You are welcome to download it, study it, question it, improve it, or develop an alternative implementation in accordance with its license.

If you notice an inefficiency, an error, an unnecessary complication, or an opportunity to make something work better, you do not need to assume it was intentionally designed that way.

Ask questions. Share observations. Test your ideas.

If an existing implementation has a useful reason behind it, that reason should be explained.

If a better implementation is demonstrated, it deserves consideration.

The project's original design is a starting point for development, not an obstacle to future understanding.

**Ask why before assuming, explain why when deciding, and remain willing to reconsider when better evidence or implementations become available.**

Thank you for helping make existing AI technologies more accessible, useful, and interoperable.

**The objective is not to replace what already works. It is to help more of what already works function together.**
