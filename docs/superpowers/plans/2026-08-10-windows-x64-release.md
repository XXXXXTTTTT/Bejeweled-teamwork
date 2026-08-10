# Windows x64 Release Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Produce Windows x64 portable and installer distributions that start a LAN Bejeweled game without requiring Qt installation.

**Architecture:** Keep the existing Qt client/server pair. Move the client endpoint into an explicit runtime configuration, add a host launcher that starts the bundled server before the local client, and package both executables with Qt deployment dependencies. Use one Release staging directory as the source for both ZIP and installer outputs.

**Tech Stack:** Qt 6.7.2, qmake, MinGW 64-bit, PowerShell, Inno Setup (optional installer backend).

---

### Task 0: Restore the exact Windows x64 toolchain

**Files:**
- Create: `scripts/bootstrap-qt.ps1`
- Modify: `.gitignore`

- [ ] **Step 1: Write the failing toolchain check** requiring Qt `6.7.2` MinGW 64-bit `qmake` and `windeployqt` at an explicit path.
- [ ] **Step 2: Run the check and verify it fails** because the deleted Qt installation is absent.
- [ ] **Step 3: Implement the bootstrap script** using the official Qt installer or aqtinstall, with an explicit `D:\QT\6.7.2\mingw_64` target and no credentials embedded in the repository.
- [ ] **Step 4: Run the check after installation and verify it passes.**
- [ ] **Step 5: Commit only the bootstrap script and ignore rules.**

### Task 1: Define and test runtime endpoint configuration

**Files:**
- Modify: `Bejeweled_Client/clientthread.h`
- Modify: `Bejeweled_Client/clientthread.cpp`
- Modify: `Bejeweled_Client/main.cpp`
- Create: `tests/client_endpoint_test.ps1`

- [ ] **Step 1: Write the failing test** asserting `--host` and `--port` are accepted and the default endpoint is `127.0.0.1:12345`.
- [ ] **Step 2: Run the test and verify it fails** because the current singleton always uses a compiled endpoint.
- [ ] **Step 3: Implement the minimal endpoint injection** by parsing exact command-line options in `main.cpp` and passing them to `ClientThread` without changing existing message handling.
- [ ] **Step 4: Run the focused test and verify it passes.**
- [ ] **Step 5: Commit only the endpoint changes and test.**

### Task 2: Add host and join launchers

**Files:**
- Create: `Bejeweled_Launcher/Bejeweled_Launcher.pro`
- Create: `Bejeweled_Launcher/main.cpp`
- Create: `scripts/launch-host.ps1`
- Create: `scripts/launch-client.ps1`
- Create: `tests/launcher_contract_test.ps1`

- [ ] **Step 1: Write the failing launcher contract test** for server-first startup, local endpoint selection, and explicit remote endpoint forwarding.
- [ ] **Step 2: Run the test and verify it fails** because launcher entries do not exist.
- [ ] **Step 3: Implement the minimal launchers** with deterministic executable-relative paths and clear process/port failure messages.
- [ ] **Step 4: Run the focused contract test and verify it passes.**
- [ ] **Step 5: Commit launcher changes.**

### Task 3: Create reproducible Qt Release packaging

**Files:**
- Create: `scripts/build-release.ps1`
- Create: `scripts/package-portable.ps1`
- Create: `packaging/Bejeweled.iss`
- Create: `tests/package_layout_test.ps1`

- [ ] **Step 1: Write the failing package layout test** for both executables, database, Qt platform plugin, SQLite driver, and multimedia plugin.
- [ ] **Step 2: Run the test and verify it fails** before staging exists.
- [ ] **Step 3: Implement the build script** to call the Qt 6.7.2 MinGW qmake/mingw32-make Release builds and `windeployqt` into a clean staging directory.
- [ ] **Step 4: Implement ZIP staging and the Inno Setup script** using the same staging directory.
- [ ] **Step 5: Run the package layout test and verify it passes** after building with the restored Qt toolchain.
- [ ] **Step 6: Commit packaging scripts and installer definition.**

### Task 4: Update project documentation

**Files:**
- Modify: `README.md`
- Create: `docs/BUILD-WINDOWS-X64.md`

- [ ] **Step 1: Document exact prerequisites, Qt kit version, build commands, package commands, host/join operation, firewall port, and known limitations.**
- [ ] **Step 2: Add a troubleshooting table tied to verifiable error messages and paths.**
- [ ] **Step 3: Validate all commands and paths against the scripts and `.pro` files.**
- [ ] **Step 4: Commit documentation.**

### Task 5: End-to-end verification

**Files:**
- Test: `tests/client_endpoint_test.ps1`
- Test: `tests/launcher_contract_test.ps1`
- Test: `tests/package_layout_test.ps1`

- [ ] **Step 1: Run all focused tests.**
- [ ] **Step 2: Build both Release executables.**
- [ ] **Step 3: Start the packaged server and verify TCP `12345` is listening.**
- [ ] **Step 4: Start the packaged host client and verify it connects to `127.0.0.1:12345`.**
- [ ] **Step 5: Record any checks that require a second LAN machine and report them explicitly.**
