# AI Device Bridge

**An open-source framework for discovering, configuring, and integrating AI processing devices with existing applications and operating systems.**

**Project status:** Initial development / Proof of Concept  
**Initial reference hardware:** Hailo-8 M.2 AI Accelerator  
**Primary development language:** C++  
**License:** Apache License 2.0

---

## 1. Introduction

AI Device Bridge is an independent, community-developed project intended to simplify the integration of specialized artificial intelligence processors and accelerators into existing computing environments.

Its purpose is to provide a common interface through which applications can discover available AI hardware, identify supported computational capabilities, configure compatible execution resources, and communicate with those resources using their existing manufacturer-provided software.

The project begins with the **Hailo-8 M.2 neural accelerator** as its first reference implementation and proof of concept.

Over time, AI Device Bridge may expand to support additional AI accelerators, neural processing units (NPUs), GPUs, CPUs, and other suitable computational devices through independently developed backend modules.

The framework is intended to remain useful regardless of which specific manufacturers or hardware architectures are supported.

### Core philosophy

**AI Device Bridge is intended to complement existing systems, not compete with or replace them.**

Rather than creating another independent AI execution ecosystem, it aims to improve accessibility, interoperability, and automated configuration of the resources already available to developers and users.

The project respects the existing responsibilities of operating systems, device drivers, inference runtimes, model compilers, and hardware manufacturers.

---

## 2. Project Objectives

AI Device Bridge is being designed to provide:

- **AI Device Discovery:** Identify available and supported AI-processing hardware.
- **Capability Discovery:** Determine which computational services a device can actually provide.
- **Automatic Configuration:** Assist users in configuring compatible drivers, runtimes, models, and application interfaces.
- **Device Profiles:** Provide selectable operating profiles optimized for specific application requirements.
- **Model Management:** Discover, register, validate, and select compatible executable AI models.
- **Application Interoperability:** Offer a documented interface allowing applications written in different programming languages to request supported AI services.
- **Resource Coordination:** Help applications make appropriate use of available processing resources without unnecessarily interfering with existing workloads.
- **Diagnostics:** Provide hardware status, software compatibility information, and inference diagnostics.
- **Developer Guidance:** Help developers identify appropriate hardware backends and development paths for particular application components.
- **Extensibility:** Allow additional devices, runtimes, and application profiles to be supported through modular contributions.

Not every objective will be implemented in the initial release.

The development process will prioritize reliable hardware integration and a stable public interface before advanced automation.

---

## 3. What AI Device Bridge Is Not

To avoid ambiguity, this project has several intentional boundaries.

### Not an operating system

AI Device Bridge does not replace Linux, Windows, macOS, or any other operating system.

It uses the services provided by the host operating system and appropriate device drivers.

### Not a hardware driver replacement

The project does not seek to reproduce or replace official manufacturer drivers, device firmware, or hardware control interfaces.

Existing official drivers and supported runtimes remain responsible for communicating with their respective hardware.

### Not a universal AI inference engine

AI Device Bridge is not intended to replace established inference frameworks, model compilers, or execution runtimes.

It provides integration with supported systems rather than attempting to implement their internal computational functions.

### Not a model format or model converter

The project does not require all AI models to use a common binary format.

For example, a Hailo accelerator may use HEF files while other devices use different representations.

AI Device Bridge should preserve these differences and use the appropriate backend for each format.

### Not a benchmark competition

The project is not intended to promote one accelerator manufacturer over another or establish competition between hardware ecosystems.

Performance measurements may be provided to help users make informed configuration decisions, not to establish arbitrary rankings.

### Not an application replacement

Existing camera software, AI development environments, simulations, and other applications should continue to function independently.

AI Device Bridge provides optional integration services that applications may choose to use.

### Not a guarantee of universal compatibility

An accelerator's physical presence does not guarantee that a requested model or operation can execute on it.

Compatibility depends on documented hardware capabilities, the installed runtime, model compilation, and application integration.

---

## 4. Initial Proof of Concept: Hailo-8 M.2

The first development target is the Hailo-8 M.2 neural processing accelerator.

This device provides an opportunity to demonstrate integration with specialized AI hardware through an existing, manufacturer-supported software stack.

The initial backend is planned to use:

- Hailo-8 M.2 accelerator hardware.
- Official Hailo PCIe driver.
- Official HailoRT runtime and native API.
- Compatible Hailo Executable Format (HEF) models.
- A C++ device integration module.
- A shared AI Device Bridge application interface.

### Initial demonstration goals

The first working implementation should be able to:

1. Detect a compatible Hailo-8 device.
2. Identify its installed driver and runtime.
3. Report basic hardware and software status.
4. Locate and inspect a compatible HEF.
5. Validate the model against the installed accelerator.
6. Load and execute a supported inference workload.
7. Return inference results through the bridge interface.
8. Expose device status and available services to an external application.

These milestones will establish the first verified backend before the project expands to additional hardware.

### HEF handling

HEF files are compiled neural-network workloads designed for compatible Hailo accelerators.

AI Device Bridge will not treat a HEF as a complete application or operating system.

Instead, it will provide a means for applications to discover and use the computational functions represented by compatible HEFs.

Application-specific adapters may provide preprocessing, postprocessing, and descriptions of the available neural services.

The bridge should distinguish between:

- Hardware capabilities.
- Installed executable models.
- Available runtime services.
- Application-specific integrations.

---

## 5. Planned Architecture

AI Device Bridge is intended to use a modular architecture.

```text
+----------------------------------------+
|          User Applications             |
|   Java / C++ / Python / Other Clients  |
+----------------------------------------+
                    |
                    v
+----------------------------------------+
|           AI Device Bridge             |
|                                        |
|   Common API / Service Discovery       |
|   Configuration / Model Registry       |
|   Device Profiles / Diagnostics        |
|   Resource Coordination                |
+----------------------------------------+
                    |
                    v
+----------------------------------------+
|         Device Backend Modules         |
|                                        |
|   Hailo-8       Future Accelerator     |
|   Backend       Backends               |
+----------------------------------------+
                    |
                    v
+----------------------------------------+
|       Existing Official Software       |
|       Drivers / Runtimes / APIs         |
+----------------------------------------+
                    |
                    v
+----------------------------------------+
|             AI Hardware                |
+----------------------------------------+
```

The common interface should not require applications to understand every hardware-specific detail.

Individual backend modules will be responsible for communicating with their supported runtime and translating common requests into supported device operations.

Hardware-specific differences must remain visible where necessary to prevent incorrect assumptions about compatibility.

---

## 6. Device Operating Profiles

A planned feature is a configurable set of operating profiles.

These profiles would allow users to select how AI Device Bridge prioritizes and organizes available resources for particular workloads.

Potential profiles include:

| Profile | Intended use |
|---|---|
| Camera Optimized | Continuous image inference and vision processing |
| Simulation Optimized | Interactive neural behavior and low-latency inference |
| Language Utilities | Supported classification, recognition, and text-processing tasks |
| Balanced | Multiple compatible workloads sharing available resources |
| Developer / Custom | Manual model selection, diagnostics, and experimental integrations |

Profiles would configure bridge-level services and supported runtime settings.

They would not imply that a device can perform unsupported operations or that proprietary firmware must be modified.

### Automatic HEF discovery

For a Hailo device, the configurator may search registered directories for compatible HEF files.

If a necessary HEF cannot be found, it should allow the user to:

- Select an existing local HEF.
- Specify a custom model directory.
- Review compatibility requirements.
- Follow links to official documentation and model resources.

Models should be validated before a profile is reported as operational.

The project should not automatically download restricted or third-party models without appropriate authorization.

---

## 7. Control Center and User Preferences

The planned Control Center will provide a graphical interface similar in general purpose to hardware configuration utilities.

Possible functions include:

- Connected device information.
- Driver and runtime status.
- Available AI capabilities.
- Model registration and validation.
- Device operating profiles.
- Application-specific configuration.
- Scheduling preferences where supported.
- Diagnostic reports.
- Development and debugging information.
- Links to manufacturer resources and documentation.

A command-line interface should remain available for automated installations, headless systems, and developer workflows.

The graphical interface is intended as a convenience rather than a mandatory dependency.

---

## 8. Distribution and Building

The project is intended to follow a straightforward, source-available distribution model.

The long-term goal is a single primary executable that can be compiled from the GitHub repository, along with the support files necessary for installation and operation.

The initial implementation will prioritize Linux and use C++ for its native services.

CMake is the proposed build system.

The primary executable is tentatively named:

```text
aidevicebridge
```

Planned invocation examples:

```bash
aidevicebridge --setup
aidevicebridge --devices
aidevicebridge --diagnose
aidevicebridge --serve
aidevicebridge --gui
```

These commands describe the intended interface and are not yet implemented.

The executable may depend on installed manufacturer runtimes and their associated libraries.

**A single application executable does not imply that official drivers, firmware, or proprietary runtime components will be bundled into the repository.**

Installation procedures and supported dependencies will be documented as implementation progresses.

---

## 9. Application Integration

AI Device Bridge is intended to support applications written in different programming languages.

The common interface should expose operations such as:

```text
DiscoverDevices()
GetDeviceCapabilities()
GetAvailableServices()
RegisterModel()
LoadModel()
SubmitInference()
GetInferenceResult()
ReleaseModel()
GetDeviceStatus()
```

These names are conceptual and do not represent an implemented API.

Applications will be able to request supported services without needing to reproduce the bridge's device-management functionality.

Different execution environments may require different data-transfer methods.

The initial architecture will prioritize a stable, documented local interface, with additional language bindings developed as needed.

### Example applications

Potential applications include:

- Camera-based object recognition.
- Computer vision utilities.
- Robotics experiments.
- Neural behavior simulations.
- Audio or text classification.
- Hardware-assisted research tools.
- Existing AI applications seeking optional accelerator integration.

**The Fishbowl neural simulation is planned as a separate project and repository.**

It may serve as a demonstration of AI Device Bridge's application interface, but it is not part of the bridge itself.

The framework should remain fully useful without Fishbowl or GoddessMatrix installed.

---

## 10. Development Roadmap

### Phase 1 — Foundation

**Goal:** Establish the independent framework.

- Create the public repository.
- Define the architecture and backend interface.
- Establish the C++ build system.
- Implement the initial command-line structure.
- Create configuration and diagnostic interfaces.
- Document licensing and contribution requirements.

### Phase 2 — Hailo-8 Reference Backend

**Goal:** Demonstrate real hardware integration.

- Detect the Hailo-8 M.2.
- Integrate with official HailoRT.
- Report device and runtime compatibility.
- Discover and inspect compatible HEFs.
- Execute a verified inference workload.
- Expose results through the bridge API.

### Phase 3 — Autoconfiguration and Control Center

**Goal:** Improve usability.

- Implement configurable search paths.
- Introduce operating profiles.
- Add model registration.
- Add a preferences interface.
- Provide guided installation and troubleshooting information.
- Document application-specific capabilities.

### Phase 4 — Application Integration

**Goal:** Provide a reusable development interface.

- Publish a documented client protocol.
- Develop example applications.
- Add Java, Python, or additional client integrations.
- Implement service discovery where supported.
- Test concurrency and resource coordination.

### Phase 5 — Additional Hardware Backends

**Goal:** Demonstrate hardware independence.

- Develop a second backend implementation.
- Validate that the common API accommodates different hardware architectures.
- Add backend-specific capability reporting.
- Expand supported operating systems where feasible.
- Document backend development procedures.

### Phase 6 — Community Expansion

**Goal:** Support long-term collaborative development.

- Additional accelerator backends.
- New device profiles.
- More example applications.
- Compatibility and diagnostic improvements.
- Expanded documentation.
- Community-maintained integration modules.

This roadmap describes development intentions, not guaranteed delivery dates or completed features.

---

## 11. For Developers

Contributions are welcome from developers interested in AI hardware interoperability, native runtimes, application integration, and system utilities.

Potential contribution areas include:

- C++ core development.
- Device backend implementations.
- HailoRT integration.
- Runtime compatibility testing.
- GUI development.
- Linux system integration.
- Windows and macOS support.
- Java and Python client libraries.
- Model metadata and manifests.
- Diagnostics and configuration tools.
- Documentation and examples.

### Development principles

Contributors should follow these architectural principles:

1. **Preserve manufacturer interfaces.** Use documented and supported drivers, APIs, and runtimes wherever possible.
2. **Avoid unnecessary duplication.** Do not reimplement existing software merely to make it part of the bridge.
3. **Separate hardware-specific code.** Device backends should remain independent of the common API.
4. **Maintain application independence.** No individual application should be required to operate the framework.
5. **Respect user configuration.** Do not overwrite working device installations or change resource ownership without authorization.
6. **Report actual capabilities.** Unsupported or untested features must not be advertised as operational.
7. **Document compatibility.** Include device, runtime, model format, and operating-system requirements.
8. **Support incremental development.** Additional hardware support should be possible without redesigning the entire application.
9. **Maintain clear diagnostics.** Developers should be able to identify which software layer is responsible for a failure.
10. **Encourage interoperability.** Design interfaces that can coexist with other applications and frameworks.

The project may adopt additional coding conventions, automated tests, and contribution procedures as implementation progresses.

---

## 12. Official Hailo Resources

The first reference implementation will use the Hailo-8 software ecosystem.

Useful starting points:

- [Hailo Official Website](https://hailo.ai/)
- [Hailo Developer Zone](https://hailo.ai/developer-zone/)
- [HailoRT GitHub Repository — Hailo-8 Branch](https://github.com/hailo-ai/hailort/tree/hailo8)
- [Hailo Model Zoo — v2.x](https://github.com/hailo-ai/hailo_model_zoo/tree/v2.x)
- [Hailo Community Forum](https://community.hailo.ai/)

Users should obtain official drivers, firmware, and development tools through the manufacturer's authorized channels.

**Compatibility note:** Hailo-8 and Hailo-8L use the relevant Hailo-8 software branches. Software intended for newer accelerator families may not be compatible.

AI Device Bridge will not independently guarantee the availability, redistribution rights, or compatibility of third-party software and HEF files.

---

## 13. Unofficial Project Disclaimer

**AI Device Bridge is an unofficial, independent, community-developed project.**

It is not affiliated with, sponsored by, endorsed by, or maintained by Hailo Technologies or any other accelerator manufacturer unless explicitly stated otherwise.

All manufacturer names, product names, trademarks, and related software belong to their respective owners.

The project does not claim ownership of manufacturer-provided drivers, firmware, runtimes, model compilers, or proprietary model files.

Users are responsible for following the applicable licenses, installation requirements, hardware documentation, and distribution restrictions of third-party components.

AI Device Bridge is provided without warranties of hardware compatibility, reliability, performance, or fitness for any particular purpose.

---

## 14. License

AI Device Bridge is intended to be distributed under the **Apache License, Version 2.0**.

See the repository's `LICENSE` file for the full license terms.

The project's license applies to its own code and documentation, not automatically to the independent software, firmware, models, or libraries with which it interoperates.

Contributors must respect the licenses of any external components included in their submissions.

---

## 15. Long-Term Vision

The long-term vision is to make specialized AI hardware easier to approach, understand, and integrate into ordinary software development.

An application developer should eventually be able to examine available hardware, determine which supported capabilities are appropriate for different parts of an application, and follow a clear development path for utilizing those resources.

The framework should make useful existing capabilities more accessible without requiring manufacturers to abandon their software ecosystems or developers to migrate their applications into a new one.

AI Device Bridge begins with one reference device and one practical integration challenge.

Its future development should be guided by demonstrated compatibility, documented interfaces, reproducible testing, and contributions from developers who find useful applications for existing AI processing hardware.

**The objective is not to replace what already works. It is to help more of what already works function together.**
