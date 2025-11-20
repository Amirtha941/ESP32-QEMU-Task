
# 🚀 ESP32 QEMU Emulation Task

This repository contains my implementation of the **FOSSEE OSHW Screening Task**, where I successfully set up a complete **ESP32 emulation environment** using **QEMU (Espressif fork)** and **ESP-IDF**.  
Two ESP32 applications were developed, built, and executed fully inside the emulator:

-   **LED Blink** (virtual GPIO toggling)
    
-   **Temperature Sensor Simulation** (periodic FreeRTOS logs)
    

The project demonstrates how ESP32 programs can run **without real hardware**, enabling virtual labs, automated evaluation, and integration with **Yaksh**, IIT Bombay’s coding assessment platform.

----------

# 📘 Table of Contents
    
- [Overview](#overview)
- [What is an Emulator?](#what-is-an-emulator)
- [What is QEMU?](#what-is-qemu)
- [What is Yaksh?](#what-is-yaksh)
- [Repository Structure](#repository-structure)
- [Environment Used](#environment-used)
- [Programs Included](#programs-included)
- [Building & Running in QEMU](#building--running-in-qemu)
- [Screenshots](#screenshots)
- [Full Documentation](#full-documentation)
- [Learnings](#learnings)
- [License](#license)

    

----------

# 🧩 Overview

This project emulates an **ESP32 microcontroller** entirely in software.  
No physical development board is required.

Using **QEMU**, ESP32 firmware is executed on a _virtual Xtensa-based CPU_, following the exact boot flow:

`Bootloader → Partition  Table → Application` 

This makes it possible to:

-   Test embedded firmware without hardware
    
-   Debug bootloader & logging issues
    
-   Automate evaluation of ESP32 code
    
-   Integrate ESP32 tasks into **Yaksh**
    
-   Conduct virtual labs for students
    

This task demonstrates complete understanding of the ESP-IDF toolchain, QEMU setup, debugging, and documentation.

----------

# 🖥️ What is an Emulator?

An **emulator** is software that mimics real hardware behavior.

In this task:

-   QEMU emulates the **ESP32 hardware**
    
-   It runs bootloaders, partitions, flash memory, FreeRTOS tasks
    
-   It executes ESP-IDF applications exactly like a physical board
    

So you can run:

-   `blink.bin`
    
-   `hello_world.bin`
    
-   any ESP32 project
    

**without owning an ESP32**.

----------

# ⚙️ What is QEMU?

**QEMU (Quick Emulator)** is a popular open-source system emulator.  
Espressif maintains a special QEMU fork that adds:

-   Xtensa architecture support
    
-   ESP32 SoC model
    
-   SPI Flash emulation
    
-   Peripherals simulation
    

This project uses the **Espressif fork** of QEMU to run ESP32 firmware virtually.

----------

# 🎓 What is Yaksh?

**Yaksh** is IIT Bombay’s automated assignment evaluation platform.

It normally evaluates:

-   C, C++, Python, Java
    
-   MATLAB, NumPy, etc.
    

But embedded firmware is harder because it usually requires real hardware.

Using QEMU:

-   Student ESP32 code → automatically built
    
-   `flash.bin` → run inside emulator
    
-   Console output → compared with expected patterns
    
-   Yaksh → auto-grades submissions
    

This enables **scalable hardware programming labs** without physical devices.

----------

# 📂 Repository Structure
```
`ESP32-QEMU-Emulation-Task/
│
├── README.md
├── report.md
│
├── blink/
│   ├── main/
│   │   ├── main.c
│   │   ├── blink.c
│   │   └── CMakeLists.txt
│   ├── CMakeLists.txt
│   └── sdkconfig
│
├── temp-sensor/
│   ├── main/
│   │   ├── temp_sensor.c
│   │   └── CMakeLists.txt
│   ├── CMakeLists.txt
│   └── sdkconfig
│
├── Screenshots/
│   ├── screenshot1.png
│   ├── screenshot2.png
│   ├── ...
│   ├── all/
│   │   ├── p1.png
│   │   ├── p2.png
│   │   ├── ...
│   │   ├── p49.png
│   │   └── raw screenshots
│
└── .gitignore  (optional)` 
```
----------

# ⚙️ Environment Used

-   **OS:** Ubuntu 22.04.5 LTS (inside Oracle VirtualBox)
    
-   **ESP-IDF:** v5.1
    
-   **QEMU:** Espressif fork (`esp_develop` / `xtensa-softmmu`)
    
-   **Toolchain:** `xtensa-esp32-elf`
    
-   **Python:** 3.10.x
    

----------

# 🧪 Programs Included

## 1️⃣ LED Blink (Simulated)

-   Virtual LED toggling
    
-   Prints:
    
    `LED ON LED OFF` 
    

## 2️⃣ Temperature Sensor Simulation

-   Generates random temperature values
    
-   Prints:
    
    `Temperature: 27 °C  Temperature: 30 °C` 
    
-   Demonstrates FreeRTOS periodic tasks
    

----------

# ▶️ Building & Running in QEMU

### **Build**

```
. $HOME/esp/esp-idf/export.sh
idf.py build
``` 

### **Merge Flash Image**

```
esptool.py --chip esp32 merge_bin -o flash.bin \
  0x1000  build/bootloader/bootloader.bin \
  0x8000  build/partition_table/partition-table.bin \
  0x10000 build/app.bin
```
### **Run in QEMU**

```
qemu-system-xtensa \
  -nographic \
  -machine esp32 \
  -drive file=flash.bin,if=mtd,format=raw
  ``` 

----------

# 📸 Screenshots

All execution logs and build proofs are available in the **/Screenshots** folder.

----------

# 📄 Full Documentation

A complete, step-by-step report with all logs, explanations, and screenshots is provided here:

👉 **[report.md](report.md)**

----------

# 🧠 Learnings

-   ESP-IDF toolchain setup and environment handling
    
-   Understanding ESP32 bootloader, partitions, and app flow
    
-   Debugging QEMU build issues (slirp, gcrypt, tomli, Meson)
    
-   Creating QEMU-compatible flash images
    
-   Running firmware virtually without hardware
    
-   Mapping virtual execution to Yaksh evaluation flow
    
-   Improving Linux command proficiency and debugging ability
    

----------

# 📜 License

Open-source for learning, research, and reference.
