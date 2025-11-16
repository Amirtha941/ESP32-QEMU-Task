# ESP32 Emulation using QEMU – Report

This document explains the complete process of setting up an **ESP32 emulation environment** using **QEMU** and **ESP-IDF**, and demonstrates two programs:
1. LED Blink (simulated)
2. Temperature Sensor (simulated)

---

# 📌 1. Introduction

The aim of this task is to emulate an **ESP32 microcontroller** without using real hardware.  
Using **QEMU**, a processor emulator, we can run ESP32 firmware entirely inside a virtual environment.  
This setup is useful for automated testing systems, virtual IoT labs, and integration with platforms like **Yaksh**.

The task involved:

- Installing ESP-IDF toolchain  
- Building Espressif’s QEMU fork  
- Creating two ESP-IDF applications  
- Running them inside QEMU  
- Documenting the full workflow  

---

# 📌 2. System Information

| Component | Details |
|----------|---------|
| OS | Ubuntu / WSL2 |
| ESP-IDF Version | (add your version here) |
| Python Version | `python3 --version` |
| QEMU Version/Branch | esp32 fork (esp_develop branch or master) |
| Toolchain | xtensa-esp32-elf |

---

---

## 📌 3. Installing Prerequisites

# 📌 **3. Installing Prerequisites**

To prepare the system for building ESP-IDF and Espressif’s custom QEMU, I installed all required development packages including Git, Python 3, build tools, and libraries such as glib, pixman, fdt, and zlib.

### **Command Used**

`sudo apt install -y git python3 python3-pip cmake make gcc g++ \
libglib2.0-dev libfdt-dev libpixman-1-dev zlib1g-dev \
ninja-build flex bison` 

### **Screenshot: Prerequisite Installation**

![Prerequisite Installation](./screenshots/01_prereq_install.png)

This confirms all necessary base packages were successfully installed.

----------

# 📌 **4. Verifying Tool Installations**

After installing dependencies, I verified whether all tools required by ESP-IDF and QEMU were available.

### **Commands Used**

`python3 -m pip install --user pyserial
git --version
python3 --version
cmake --version
make --version
gcc --version` 

These checks ensure the system is correctly prepared for building the toolchains and QEMU.

### **Screenshot: Tool Verification**

![Tool Verification](./screenshots/02_tool_verification.png)

----------

# 📌 **5. Cloning and Configuring QEMU (Espressif Fork)**

To emulate the ESP32 processor, I used Espressif’s maintained QEMU version.

### **Commands Used**

`git clone https://github.com/espressif/qemu.git cd qemu mkdir build && cd build
../configure --target-list=xtensa-softmmu --enable-debug` 

The configuration step detects dependencies, creates a Python virtual environment, and prepares the build system.

### **Screenshot: QEMU Clone & Initial Configure**

![QEMU Clone and Configure](./screenshots/03_qemu_clone_configure.png)

----------

# 📌 **6. Building QEMU — Errors & Fixes**

### ❗ **Issue 1 — Missing `gcrypt.h`**

The initial `ninja` build failed because the header `gcrypt.h` was not found.

### **Fix**

`sudo apt install -y libgcrypt20-dev` 

### **Screenshot: gcrypt Error**

![gcrypt error](./screenshots/04_qemu_build_error_gcrypt.png)

----------

### ❗ **Issue 2 — Missing `libslirp.h`**

Another dependency error occurred during compilation involving the SLIRP networking library.

### **Fix**

`sudo apt install -y libslirp-dev` 

### **Screenshot: slirp Error**

![slirp error](./screenshots/05_qemu_build_error_slirp.png)

----------

# 📌 **7. Successful QEMU Build**

After installing the required libraries, I re-configured and rebuilt QEMU.  
The final build completed successfully.

### **Command**

`ninja
./qemu-system-xtensa --version` 

### **Screenshot: QEMU Build Success**

![QEMU Build Success](./screenshots/06_qemu_build_success.png)

----------

# 📌 **8. Installing ESP-IDF**

I cloned the ESP-IDF repository (with all its submodules) and installed the toolchain.

### **Commands Used**

`git clone --recursive https://github.com/espressif/esp-idf.git cd esp-idf
./install.sh` 

### **Screenshot: ESP-IDF Clone**

![ESP-IDF Clone](./screenshots/07_espidf_clone.png)

### **Screenshot: ESP-IDF Tool Installation**

![ESP-IDF Install Tools](./screenshots/08_espidf_install_tools.png)

----------

# 📌 **9. Activating ESP-IDF**

Before using `idf.py`, I activated the ESP-IDF environment using:

`. ./export.sh
idf.py --version` 

This sets environment variables, configures Python dependencies, and provides autocompletion.

### **Screenshot: export.sh + Version Check**

----------

# 📌 **10. Creating & Editing the Blink Application**

I created a blink project and edited the default `main.c` to print LED ON / LED OFF messages for QEMU simulation.

### **Screenshot: Blink main.c**

# 📌 10. Screenshots

(Add your screenshots here)


# **11. Blink Application – Initial Build Configuration**

**File:** `11_blink_build_config.png`

This screenshot shows the initial configuration and compilation steps when running:

`idf.py build` 

It includes:

-   C/C++ compiler detection
    
-   ESP32 toolchain versions
    
-   Component checks
    
-   Project metadata detection (`git not a repo` warning)
    
-   SDK configuration generation (`sdkconfig`)
    

This proves the ESP-IDF environment is functioning and able to configure a new project.

----------

# **12. Blink Application – Build Completed Successfully**

**File:** `12_blink_build_done.png`

This image shows the successful completion of the firmware build, including:

-   Linker script generation
    
-   ROM / memory linkage
    
-   Warnings regarding Wi-Fi private include directory
    
-   Overall status: **Finished successfully**
    

This indicates that the Blink firmware (`blink.elf`) was successfully compiled.

----------

# **13. QEMU – Wrong Flash Command Attempt**

**File:** `13_qemu_wrong_command_attempt.png`

This screenshot shows attempts to launch QEMU using an incorrect SPI flash argument:

`-drive file=build/blink.elf,if=mtd,format=raw` 

Errors shown:

-   Only 2,4,8,16 MB flash images supported
    
-   Machine type does not support `if=mtd`
    
-   “Not initializing SPI Flash”
    
-   QEMU terminating immediately
    

These attempts demonstrate incorrect command usage before switching to the correct minimal kernel load method.

----------

# **14. ESP-IDF Installation – Running install.sh**

**File:** `14_espidf_install_tools.png`

This screenshot shows the ESP-IDF installation (`install.sh`) process:

-   Python compatibility check
    
-   Downloading ESP32 toolchains
    
-   Installing required Python packages
    
-   Setting up `.espressif` tools directory
    

This is part of setting up ESP-IDF v5.1.

----------

# **15. ESP-IDF Environment Export**

**File:** `15_espidf_export_environment.png`

Running:

`. ./export.sh` 

This screenshot shows:

-   IDF_PATH being set correctly
    
-   Virtual environment activation
    
-   Python dependency checks
    
-   Confirming ESP-IDF version = v5.1
    

This prepares the environment for building firmware.

----------

# **16. Blink Project – Full Clean + Fresh Build**

**File:** `16_blink_fullclean_build.png`

This screenshot shows:

`idf.py fullclean
idf.py build` 

-   All managed components removed
    
-   Rebuilding the firmware from scratch
    
-   No critical warnings
    
-   Build completes successfully
    

Useful for documenting reproducibility.

----------

# **17. QEMU Configure Error – Missing tomli**

**File:** `17_qemu_configure_error_tomli.png`

This image shows the first attempt to configure QEMU:

`./configure --target-list=xtensa-softmmu --enable-debug` 

Error:

`No  usable  tomli,  please  install  it` 

This prevented the QEMU build from proceeding.

----------

# **18. Fix – Installing python3-tomli**

**File:** `18_fix_install_tomli.png`

This screenshot shows the fix:

`sudo apt install python3-tomli` 

And after installation, build attempts resume normally.  
This is important for documenting dependency troubleshooting.

----------

# **19. QEMU Configure – Successful**

**File:** `19_qemu_configure_success..png`

This shows a **successful QEMU configuration** after installing missing packages:

-   Meson build system initialization
    
-   Python version detection
    
-   Host/target info
    
-   Enabled/disabled feature lists
    
-   No more tomli errors
    

This proves QEMU is now ready to be built via `ninja`.

----------

# **20. QEMU Kernel Load – Incorrect Command (Final Wrong Attempt)**

**File:** `20_wrong_qemu_kernel_command.png`

This screenshot shows another failed attempt:

`~/qemu/build/qemu-system-xtensa -machine esp32 -kernel build/blink.elf` 

Error:

`Warning: both -bios and -kernel arguments specified Only loading the -kernel file. Not initializing SPI Flash
QEMU: Terminated` 

This documents repeated wrong usage attempts **before discovering the correct QEMU command**.
Example:

### **Blink Output**
![Blink Output](./screenshots/blink_output.png)

### **Temperature Output**
![Temp Output](./screenshots/temp_output.png)

---




## **🖼️ 21_blink_build_and_flashbin_error.png**

This screenshot shows the Blink project fully built, but the attempt to run QEMU using the wrong flash image (`blink.bin` directly) fails because QEMU requires a **merged flash image** or a compatible loader.  
The error message `machine type does not support if=mtd` confirms wrong command usage.

----------

## **🖼️ 22_idf_missing_tools_reinstall.png**

Here the `idf.py` and `esptool.py` commands fail because the ESP-IDF environment was not activated.  
Running `export.sh` reports missing Python virtual env (`idf5.1_py3.10_env/bin/python doesn't exist`).  
This leads to reinstalling necessary ESP-IDF tools using:

`./install.sh` 

----------

## **🖼️ 23_espidf_export_success.png**

After reinstall, `export.sh` successfully activates ESP-IDF.  
All Python requirements and PATH variables are set correctly:

-   `Python 3.10.12`
    
-   `ESP-IDF v5.1`
    
-   Tools added to PATH
    
-   Environment ready for building and flashing
    

----------

## **🖼️ 24_blink_fullclean_build.png**

You run:

`idf.py fullclean
idf.py build` 

This produces a fresh clean build of the Blink firmware with no leftover artifacts.

----------

## **🖼️ 25_blink_build_flashbin_created.png**

The `flash.bin` is successfully created using:

`esptool.py  --chip esp32 merge_bin -o flash.bin ...` 

Flash image is ready for QEMU execution.

----------

## **🖼️ 26_qemu_bootloader_blink.png**

QEMU correctly loads the bootloader and application from `flash.bin`.  
You can see:

-   Bootloader logs
    
-   Partition table
    
-   Application start line
    
-   ESP-IDF version v5.1
    
-   App name: _blink_
    

----------

## **🖼️ 27_qemu_blink_app_start.png**

QEMU continues the boot process:

-   CPU0 and CPU1 scheduler start
    
-   Application `app_main()` is called
    
-   GPIO initialization starts
    
-   ESP32 boots properly inside QEMU
    

----------

## **🖼️ 28_qemu_blink_led_output.png**

Blink app starts printing LED toggle messages:

`LED ON LED OFF LED ON LED OFF` 

This confirms GPIO simulation output is working in QEMU.

----------

## **🖼️ 29_full_led_output.png**

Continuous LED blinking logs — long output showing stable execution:

`LED ON LED OFF LED ON LED OFF ...` 

QEMU keeps running the Blink example successfully.

# **📘 7. Temperature Monitoring Application (Screenshots 30–33)**

This section describes the setup, building, flashing, and QEMU simulation output for the **ESP32 Temperature Monitoring Application**. The process closely follows the workflow established in earlier experiments, ensuring toolchain consistency and replicability.

----------

## **📌 7.1 Project Setup and Environment Initialization**

The experiment begins by creating a new project named **temperature**, copied from the ESP-IDF example template:

`cp -r ~/esp-idf-v5.1/examples/get-started/hello_world ~/temperature cd ~/temperature` 

The source file `main/hello_world_main.c` was modified to implement temperature reading logic.

When running `idf.py build` initially, the command failed because the ESP-IDF environment variables were not active.  
Re-exporting the environment resolved the issue:

`. ~/esp-idf-v5.1/export.sh` 

📷 **Screenshot Reference:**  
`30_temperature_project_start_export.png`

----------

## **📌 7.2 Build Process and Dependency Fixes**

After exporting the environment, `idf.py build` was executed again.  
During the build process:

-   ESP-IDF Python packages were verified
    
-   Toolchain paths were re-added
    
-   Unused tools from different IDF versions were detected
    
-   The build completed **successfully**
    

This step confirms that the newly installed ESP-IDF v5.1 environment is functioning correctly.

📷 **Screenshot Reference:**  
`31_temperature_build_error_and_fix.png`

----------

## **📌 7.3 Generating Flash Image and Running in QEMU**

After the project was built, the required binaries were merged into a single `flash.bin` image using:

`esptool.py --chip esp32 merge_bin -o flash.bin \
  0x1000 build/bootloader/bootloader.bin \
  0x8000 build/partition_table/partition-table.bin \
  0x10000 build/hello_world.bin` 

Since QEMU requires the flash image to be **4 MB in size**, padding was applied:

`truncate -s 4M flash.bin` 

The flash image was then executed inside QEMU:

`~/qemu/build/qemu-system-xtensa \
  -nographic \
  -machine esp32 \
  -drive file=flash.bin,if=mtd,format=raw` 

QEMU successfully:

-   Initialized the ESP32 bootloader
    
-   Loaded all firmware partitions
    
-   Jumped into `app_main()` of the temperature project
    

📷 **Screenshot Reference:**  
`32_temperature_mergebin_qemu_output.png`

----------

## **📌 7.4 Temperature Output Simulation (QEMU Runtime Logs)**

Once the application started running in QEMU, it began printing simulated temperature values at regular intervals.

The log showed:

-   Constant boot logs
    
-   `Project name: temperature`
    
-   `ESP-IDF version: v5.1`
    
-   Repeated temperature readings ranging between **26 °C – 40 °C** (simulated values generated by software)
    

Example output:

`I  (2729)  TEMP:  Temperature:  26 °C  I  (3739)  TEMP:  Temperature:  27 °C  I  (4739)  TEMP:  Temperature:  29 °C  I  (5739)  TEMP:  Temperature:  30 °C  ...  I  (63749)  TEMP:  Temperature:  39 °C` 

The QEMU session terminated cleanly afterward, confirming stable application execution.

📷 **Screenshot References:**

-   `33_temperature_output_long.png`

# 📌 11. Challenges & Fixes

| Issue | Fix |
|-------|-----|
| QEMU build errors | Installed missing dependencies like pixman, glib |
| ESP-IDF not detected | Ran `. export.sh` before using idf.py |
| ELF not loading in QEMU | Corrected drive format to `if=mtd,format=raw` |

---

# 📌 12. Learnings

From this task, I learned:
- How ESP-IDF builds firmware internally  
- How QEMU emulates the ESP32 chip  
- How ELF files contain complete firmware information  
- How FreeRTOS tasks run on ESP32  
- How to debug firmware without real hardware  
- How this setup can support automated grading systems like Yaksh  

---

# 📌 13. How This Can Be Used in Yaksh

This setup allows Yaksh to:
- Compile student submissions  
- Run them inside QEMU  
- Capture outputs automatically  
- Validate correctness without requiring real ESP32 boards  

It provides a **fully automated, scalable** firmware testing environment.

---

# 📌 14. Conclusion

This project successfully:
- Set up the ESP-IDF environment  
- Built the ESP32-compatible QEMU  
- Developed two example applications  
- Executed and tested them in QEMU  
- Documented the entire process  

The environment is now ready for automation and CI-based evaluation workflows.

---

# ✔ End of Report

