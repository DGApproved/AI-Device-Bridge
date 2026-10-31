# Security Policy

## AI Device Bridge

**Project status:** Initial development / Proof of Concept  
**Initial reference hardware:** Hailo-8 M.2 AI Accelerator  
**License:** Apache License 2.0

AI Device Bridge is an independent, community-developed framework intended to improve interoperability between applications and existing AI processing hardware.

Security is an important consideration because the project may interact with system services, device drivers, inference runtimes, executable AI models, local applications, and potentially shared computational resources.

The project is designed to complement existing software environments rather than replace their security mechanisms.

**The objective is not to replace what already works. It is to help more of what already works function together.**

---

## 1. Supported Versions

AI Device Bridge is currently in its initial development stage.

Until the first stable release, security support will primarily focus on the current development branch.

| Version | Security support |
|---|---|
| Current development branch | Best-effort review and correction |
| Experimental branches | Not guaranteed |
| Older development snapshots | Not guaranteed |
| Future stable releases | To be defined |

This table will be updated as releases become available.

Development versions should not be assumed suitable for security-sensitive production environments.

---

## 2. Reporting a Vulnerability

If you discover a potential security vulnerability, please report it responsibly.

**Do not initially publish exploit details in a public GitHub Issue.**

Where available, use GitHub's private vulnerability reporting feature:

1. Open the AI Device Bridge repository.
2. Navigate to the **Security** tab.
3. Select **Report a vulnerability**, if enabled.
4. Describe the issue and provide relevant reproduction information.

If private vulnerability reporting is not yet enabled, please avoid posting sensitive exploit details publicly. A private reporting contact will be documented when available.

### Useful information to include

Where practical, include:

- A description of the vulnerability.
- The affected AI Device Bridge version or commit.
- Operating system and version.
- Accelerator or processor model.
- Driver and runtime versions.
- Relevant model format or backend.
- Steps needed to reproduce the issue.
- Expected and observed behavior.
- Potential security consequences.
- Suggested mitigations, if known.

Please remove credentials, personal information, private files, and other sensitive data from diagnostic material.

A complete technical explanation is helpful, but incomplete reports are welcome.

---

## 3. Responsible Disclosure

Contributors and security researchers are encouraged to allow maintainers an opportunity to investigate and address vulnerabilities before detailed public disclosure.

The project does not currently guarantee a particular response or remediation timeline.

Maintainers should make a reasonable effort to:

- Acknowledge reports when possible.
- Assess whether the issue affects AI Device Bridge.
- Determine whether the issue also involves an external dependency.
- Develop and test an appropriate correction.
- Coordinate disclosure when necessary.
- Document relevant security changes.
- Corrections.md should contain accurate Vulnerability Discovery and Fix History.

Security reports should be evaluated according to their technical merits, regardless of the reporter's professional background or experience.

Responsible security research is welcome. Unauthorized access, destructive testing against systems without permission, or exposure of third-party information is not.

---

## 4. Security Design Principles

### 4.1 Preserve manufacturer security boundaries

AI Device Bridge should use documented and supported manufacturer interfaces wherever practical.

The project is not intended to bypass hardware restrictions, defeat security mechanisms, or replace official firmware and drivers.

A backend should operate within the permissions and capabilities provided by its supported runtime.

### 4.2 Prefer least privilege

Components should operate with only the permissions necessary for their functions.

The primary application and bridge services should not require root or administrator privileges for routine inference operations unless genuinely necessary.

Privileged setup tasks should remain separate from ordinary runtime execution wherever practical.

### 4.3 Avoid destructive automation

The autoconfigurator should not silently:

- Replace functioning device drivers.
- Flash or modify device firmware.
- Change critical system permissions.
- Modify kernel configuration.
- Disable operating-system security protections.
- Alter hardware power or performance settings outside supported interfaces.
- Remove unrelated software.
- Overwrite user-managed models or configuration files.

Operations that may affect system stability or security should require informed user approval.

### 4.4 Respect application boundaries

Applications using AI Device Bridge should retain control of their own data and requested operations.

The bridge should not grant one application unrestricted access to another application's memory, models, configuration, or inference results.

Where supported, application-specific permissions and resource isolation should be implemented.

### 4.5 Report actual capabilities

Device capabilities must be based on documented hardware and software functionality.

Unsupported, incompatible, or untested operations should not be advertised as available.

The bridge should not attempt potentially unsafe fallback operations merely to conceal a compatibility failure.

---

## 5. Driver, Firmware, and Runtime Security

AI Device Bridge depends on existing manufacturer-provided software.

For the initial Hailo-8 reference implementation, this includes the official Hailo driver and HailoRT runtime.

Users should obtain these components through authorized distribution channels.

The project should not automatically replace manufacturer software merely because a different version is available.

### Compatibility verification

Where practical, the bridge should inspect:

- Detected hardware identity.
- Installed driver version.
- Installed runtime version.
- Model compatibility.
- Relevant backend requirements.

The project should clearly report known incompatibilities.

### Firmware updates

AI Device Bridge is not intended to function as an independent firmware flashing utility.

Firmware updates, where applicable, should remain under the control of the manufacturer's supported tools and procedures.

If future versions provide guided firmware maintenance, such operations must be explicitly authorized and clearly distinguished from ordinary configuration.

---

## 6. AI Model and HEF Security

AI Device Bridge may discover, inspect, register, and execute compatible AI models through supported runtimes.

For the initial Hailo-8 implementation, these may include HEF files.

**Model files must not automatically be considered trustworthy simply because they use a recognized format.**

### Model discovery

The bridge should:

- Use explicitly configured model directories.
- Avoid automatically executing arbitrary files found during discovery.
- Validate model compatibility before activation.
- Distinguish trusted, unverified, and incompatible models where practical.
- Handle malformed or unsupported files without compromising the application.
- Avoid overwriting existing models without authorization.

### Model provenance

Where possible, model registration may record:

- File name and location.
- File checksum.
- Model source.
- Associated license.
- Compatible hardware.
- Runtime compatibility.
- Application-specific metadata.

Checksums can help identify file changes but do not independently establish that a model is trustworthy.

### Third-party models

Users are responsible for reviewing the origin, permissions, and redistribution terms of third-party models.

AI Device Bridge should not silently download and execute unverified model files.

Official model resources and community contributions should remain distinguishable.

---

## 7. Local Services and Application Communication

AI Device Bridge is intended to expose computational services to local applications through a documented interface.

These services may eventually use operating-system facilities such as Unix domain sockets, D-Bus, Windows named pipes, or other supported communication mechanisms.

### Local access

Services should restrict access to authorized local users and applications where practical.

Local communication does not automatically guarantee security.

### Request validation

The bridge should validate incoming requests, including:

- Requested operations.
- Model identifiers.
- Tensor dimensions and data sizes.
- File paths.
- Resource requirements.
- Application permissions.
- Protocol versions.

Malformed or unsupported requests should fail safely.

### Resource limits

Where practical, the bridge should implement appropriate limits on:

- Concurrent inference requests.
- Request queue sizes.
- Memory consumption.
- Input buffer sizes.
- Diagnostic log growth.
- Model loading.
- Service timeouts.

One application should not be able to exhaust shared resources without appropriate controls.

### Remote access

Remote network access is not an initial project objective.

If implemented in future versions, it should require an explicit security design, authentication, authorization, and appropriate transport protection.

Services should not expose inference endpoints to external networks by default.

---

## 8. Configuration and File Security

Configuration files may contain device preferences, model paths, application settings, and backend information.

The bridge should avoid unnecessarily storing credentials or sensitive data.

When sensitive configuration is required, it should use appropriate operating-system security facilities.

### Configuration changes

The project should:

- Avoid silently overwriting user configuration.
- Validate configuration before applying it.
- Report invalid or unsupported settings.
- Preserve existing configurations during updates where practical.
- Avoid following untrusted file paths into privileged operations.
- Use appropriate filesystem permissions.

### Temporary files

Temporary files should be created securely and removed when no longer needed.

Predictable temporary paths should not be used in ways that permit unauthorized modification or replacement of files.

---

## 9. Logging and Diagnostics

Diagnostic information is important for hardware development and troubleshooting.

However, logs may contain sensitive information.

Depending on the application, inference data may include images, text, file paths, model names, or other user-provided information.

### Logging principles

The bridge should:

- Prefer minimal diagnostic logging by default.
- Avoid logging raw inference inputs and outputs unless explicitly enabled.
- Never intentionally record passwords, API keys, or authentication tokens.
- Provide reasonable log retention and size limits.
- Make verbose logging configurable.
- Allow users to inspect logs before sharing them publicly.
- Avoid unnecessarily disclosing information about unrelated applications.

Debugging should not require users to publish sensitive datasets or proprietary models.

---

## 10. Dependency and Supply-Chain Security

AI Device Bridge is intended to use existing manufacturer libraries and supported development tools.

Third-party dependencies should be documented.

Where practical, contributors should:

- Prefer maintained dependencies.
- Use official distribution sources.
- Review dependency licenses.
- Avoid unnecessary additions.
- Document required versions.
- Report known security issues.
- Verify downloaded artifacts where trustworthy checksums or signatures are provided.

The project should avoid downloading executable dependencies from unverified locations during routine operation.

### Build integrity

Future releases may include reproducible build instructions, release checksums, signed artifacts, or other integrity-verification mechanisms.

Such protections should only be advertised once actually implemented.

---

## 11. Hardware Safety and Reliability

AI Device Bridge may interact with hardware used by other applications.

Incorrect configuration, uncontrolled resource consumption, or incompatible runtime operations may affect system stability.

Contributors should avoid changes that unnecessarily interfere with device ownership, scheduling, or operating-system resource management.

Hardware-specific testing should document the device model, firmware or runtime versions where known, operating system, and relevant test conditions.

The project should not require unsupported hardware modifications or unsafe operating conditions.

Users should follow the manufacturer's documentation for installation, cooling, power requirements, and maintenance.

---

## 12. Experimental Features

Experimental functionality is welcome when clearly identified.

An experimental feature should not be presented as secure, stable, or production-ready without appropriate testing.

Possible precautions include:

- Disabled-by-default experimental functionality.
- Explicit opt-in configuration.
- Clear compatibility warnings.
- Separate experimental branches.
- Logging and reproducible diagnostics.
- Appropriate rollback procedures.

Experimental code should not bypass established security protections merely to demonstrate functionality.

The absence of professional certification does not eliminate the responsibility to handle security issues carefully.

---

## 13. Contributions and Security Reviews

Security improvements are welcome from contributors of all experience levels.

This includes:

- Vulnerability reports.
- Safer configuration handling.
- Input validation.
- Resource isolation.
- Dependency updates.
- Improved diagnostics.
- Documentation corrections.
- Security testing.
- Alternative implementations.

Contributors may question existing security decisions and propose improvements.

The project follows the development principle:

**Ask why before assuming, explain why when deciding, and remain willing to reconsider when better evidence or implementations become available.**

Where an implementation is retained for compatibility or security reasons, documenting those reasons is encouraged.

A security concern should not be dismissed solely because the affected code has existed for a long time.

---

## 14. Third-Party Vulnerabilities

Some reported issues may originate in external drivers, firmware, runtimes, model parsers, or operating-system components.

AI Device Bridge maintainers may not have the ability or authority to correct those components directly.

Where appropriate, issues should be reported to their respective maintainers through authorized security channels.

The bridge may implement reasonable mitigations within its own scope when possible.

Users should not assume that AI Device Bridge provides independent security guarantees for third-party components.

---

## 15. Security Limitations and Disclaimer

AI Device Bridge is an unofficial, independent, community-developed project.

It is not affiliated with or endorsed by hardware manufacturers unless explicitly stated otherwise.

The project is provided under the terms of its applicable license and without warranties of security, reliability, compatibility, or fitness for a particular purpose.

Users are responsible for understanding the security implications of installing software that interacts with hardware devices and operating-system services.

The project does not claim to replace the security mechanisms provided by the operating system, manufacturer drivers, or existing runtimes.

Security capabilities described as planned or recommended in this document should not be interpreted as already implemented.

---

## 16. Long-Term Security Philosophy

As AI Device Bridge expands to support additional hardware, operating systems, and application environments, its security practices should evolve alongside its capabilities.

The objective is to support useful interoperability while preserving the boundaries and protections of existing systems.

Development should favor transparency, understandable behavior, reversible configuration, appropriate permissions, and documented compatibility.

No implementation should be assumed immune to error simply because it was previously accepted.

Security improvements should remain open to evidence, experimentation, review, and responsible contribution.

**The objective is not to replace what already works. It is to help more of what already works function together.**
