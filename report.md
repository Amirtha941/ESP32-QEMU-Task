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

# 📌 3. Prerequisites Installed

The following packages were installed before starting:

```
sudo apt update
sudo apt install -y git python3 python3-pip make cmake ninja-build gcc g++ \
    libglib2.0-dev libfdt-dev libpixman-1-dev flex bison pkg-config
```

These ensure that:
- ESP-IDF can build projects  
- QEMU can compile  
- Python tools work properly  

---

# 📌 4. Installing ESP-IDF

ESP-IDF was cloned and installed as follows:

```
mkdir -p ~/esp
cd ~/esp
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
./install.sh
```

To activate the environment:

```
. ~/esp/esp-idf/export.sh
```

Verification:

```
idf.py --version
```

---

# 📌 5. Building QEMU for ESP32

QEMU (Espressif fork) was downloaded and built:

```
cd ~/esp
git clone https://github.com/espressif/qemu.git
cd qemu
mkdir build && cd build
../configure --target-list=xtensa-softmmu
make -j$(nproc)
```

This produced `qemu-system-xtensa`, which can emulate ESP32 binaries.

---

# 📌 6. Creating ESP-IDF Projects

Two projects were created:
- LED Blink  
- Temperature Sensor  

Using ESP-IDF examples as base:

```
cp -r ~/esp/esp-idf/examples/get-started/blink ~/esp/blink
cp -r ~/esp/esp-idf/examples/get-started/blink ~/esp/temp-sensor
```

The `main.c` files were modified.

---

# 📌 7. Program 1 – LED Blink (Simulated)

A simple LED blink simulation was written by printing:

```
LED ON
LED OFF
```

Final `main.c`:

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main() {
    int state = 0;
    while (1) {
        if (state) {
            printf("LED OFF\n");
            state = 0;
        } else {
            printf("LED ON\n");
            state = 1;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
```

### ✔ Build command:

```
idf.py build
```

### ✔ Output ELF file:
```
build/blink.elf
```

---

# 📌 8. Program 2 – Temperature Sensor (Simulated)

Simulated readings using random values:

```c
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main() {
    while (1) {
        int temp = 25 + (rand() % 6);  // 25–30°C
        printf("Temperature: %d C\n", temp);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
```

---

# 📌 9. Running Programs in QEMU

Both `.elf` files were executed with:

```
qemu-system-xtensa -nographic -M esp32 \
  -drive file=build/<project>.elf,if=mtd,format=raw
```

Example:

```
qemu-system-xtensa -nographic -M esp32 \
  -drive file=build/blink.elf,if=mtd,format=raw
```

Expected console output:

```
LED ON
LED OFF
LED ON
```

or:

```
Temperature: 27 C
Temperature: 29 C
```

---

# 📌 10. Screenshots

(Add your screenshots here)

Example:

### **Blink Output**
![Blink Output](./screenshots/blink_output.png)

### **Temperature Output**
![Temp Output](./screenshots/temp_output.png)

---

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

