
## 🚀 ESP32 QEMU Emulation Task

This repository contains my implementation of the **OSHW Screening Task**, where I set up a complete ESP32 emulation environment using **QEMU** and **ESP-IDF**, and demonstrated two working programs: LED Blink and Temperature Sensor simulation.

----------

## 📌 **Introduction**

This project emulates an ESP32 microcontroller **without using real hardware**.  
Using QEMU and ESP-IDF, ESP32 programs can be built and executed inside a **virtual environment**.  
This enables automated testing, virtual labs, and integration with the **Yaksh evaluation system**.  
The repository includes the full setup, example programs, screenshots, and documentation.

----------

## 📂 **Repository Structure**

```
ESP32-QEMU-Emulation-Task/
│
├── README.md
├── report.md
│
├── blink/ # LED Blink program (ESP-IDF) │   ├── main/
│   ├── CMakeLists.txt
│   └── sdkconfig
│
├── temp-sensor/ # Temperature simulation program │   ├── main/
│   ├── CMakeLists.txt
│   └── sdkconfig
│
├── screenshots/
│   ├── blink_output.png
│   ├── temp_output.png
│
└── qemu_build/ # Optional: QEMU build notes or files
``` 

----------

## ⚙️ **Environment Used**

-   **OS:** Ubuntu
    
-   **ESP-IDF Version:** v5.1
    
-   **QEMU Commit/Branch:** esp_develop / esp32-fork
    
-   **Toolchain:** xtensa-esp32-elf
    

----------

## 🧪 **Programs Included**

### 1️⃣ LED Blink (Simulated)

-   Toggles virtual LED state
    
-   Prints `LED ON` / `LED OFF` to console
    
-   Useful to test GPIO logic without real hardware
    

### 2️⃣ Temperature Sensor (Simulated)

-   Generates random temperature values
    
-   Prints readings using `printf` or `ESP_LOGI`
    
-   Demonstrates periodic tasks in FreeRTOS
    

----------

## ▶️ **How to Build & Run in QEMU (Short Guide)**

### **Build**

```
. $HOME/esp/esp-idf/export.sh
idf.py build
``` 

### **Run in QEMU**

```
qemu-system-xtensa -nographic -M esp32 -drive file=build/app.elf,if=mtd,format=raw
``` 

----------

## 📸 **Screenshots**

Screenshots showing program outputs are available in `/screenshots`.

----------

## 📄 **Documentation**

A full step-by-step detailed report is available in:

👉 `report.md`

----------

## ✨ **Output Samples**

`LED ON LED OFF  Temperature:  28 C Temperature:  29 C` 

----------

## 🧠 **Learnings**

-   How ESP32 executes firmware internally
    
-   Setting up ESP-IDF toolchain
    
-   Building QEMU for Xtensa architecture
    
-   Running ESP32 firmware without hardware
    
-   Understanding virtualized IoT environments
    

----------

## 📜 **License**

Open-source for learning and reference.
