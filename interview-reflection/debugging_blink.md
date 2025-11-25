
# 📝 **Interview Reflection — Blink Program (Debugging & Mistakes Analysis)**

_FOSSEE IITB – ESP32–QEMU Task_

This document explains the mistakes I made during the interview, why they happened, what went wrong technically, and how I fixed them afterward.  
Every screenshot is referenced in order from:

**`interview-reflection/blink_pics/01` → `interview-reflection/blink_pics/10`**

----------

----------

# ## 1️⃣ Incorrect Code Modifications

### _(Missing semicolon, missing #include → caused compiler failure)_

During the interview, I was asked to intentionally remove a semicolon and comment out a header file.  
I did this, but I failed to rebuild the firmware properly.  
As a result, the old binary kept running and I mistakenly thought “even with errors, the code runs”.

### 📌 What really happened

-   ESP-IDF does **NOT** rebuild unless I run `idf.py build` or `idf.py fullclean && idf.py build`.
    
-   QEMU was still running my **previously compiled**, **error-free** `flash.bin`.
    
-   So even though the source had errors, QEMU kept executing the old image.
    

----------

![02_make_errors_in_blink.png](../blink_pics/02_make_errors_in_blink.png)


`![Incorrect Code](interview-reflection/blink_pics/01_initializing_esp-idf.png)`

This image shows the invalid code:

-   Missing `#include <stdio.h>`
    
-   Missing semicolon in `vTaskDelay(...)`
    

These mistakes must trigger compilation errors — but the firmware didn’t update because I hadn’t rebuilt it yet.

----------

----------

# ## 2️⃣ Exporting ESP-IDF Environment

### _(Correct step, but done **after** editing the code)_

Before any ESP-IDF command, I must activate the environment:

`. ~/esp-idf-v5.1/export.sh` 

This sets:

-   Python virtual environment
    
-   Xtensa compiler
    
-   ESP-IDF tools PATH
    
-   Build system variables
    

----------

![01_initializing_esp-idf.png](../blink_pics/01_initializing_esp-idf.png)


`![ESP-IDF Export](interview-reflection/blink_pics/02_make_errors_in_blink.png)`

This output shows the environment activating correctly.

----------

----------

# ## 3️⃣ Build Attempt With Errors

### _(idf.py build correctly detects mistakes)_

When I finally ran:

`idf.py build` 

ESP-IDF correctly reported the errors in the file.

----------

### 📷 Screenshot 03 — _Build Started_

![Build Started](interview-reflection/blink_pics/03_build_idf.png)

----------

### 📷 Screenshot 04 — _Compilation Errors Detected_

![Compilation Error](interview-reflection/blink_pics/04_compilation_error.png)

Errors shown:

-   **Missing `<stdio.h>`**
    
-   **Missing semicolon before `}`**
    
-   **pTdRiff_t undefined** → because `<stddef.h>` indirectly comes from `<stdio.h>`
    

----------

### 📷 Screenshot 05 — _Build Failed – ninja stopped_

![Build Failed](interview-reflection/blink_pics/05_idf_fail.png)

This is correct behavior — the build MUST fail.

----------

----------

# ## 4️⃣ Correcting the Mistakes

After the interview, I restored the proper code:

-   Added `#include <stdio.h>`
    
-   Added the missing semicolon
    

----------

![06_corrected_code.png](../blink_pics/06_corrected_code.png)


`![Corrected Code](interview-reflection/blink_pics/06_corrected_code.png)`

----------

----------

# ## 5️⃣ Full Clean & Rebuild

### _(This step finally removed old compiled output)_

To avoid using old binaries, we must do:

`idf.py fullclean
idf.py build` 

-   `fullclean` removes the entire build folder
    
-   `build` creates a fresh new binary
    

----------



![Rebuild Successful](interview-reflection/blink_pics/07_rebuild.png)

This is the **new, correct build**.

----------

----------

# ## 6️⃣ Merging Flash Image

### _(Creating flash.bin from bootloader + partitions + app)_

Command used:

```
esptool.py --chip esp32 merge_bin -o flash.bin \
  0x1000  build/bootloader/bootloader.bin \
  0x8000  build/partition_table/partition-table.bin \
  0x10000 build/blink.bin
  ``` 

### Explanation:

-   **0x1000** → bootloader address
    
-   **0x8000** → partition table
    
-   **0x10000** → application binary
    
-   `flash.bin` → combined flash image
    

----------
Then create 4MB flash:

`truncate -s 4M flash.bin`


![Flash + Truncate](interview-reflection/blink_pics/08_flash_and_truncate.png)

`truncate -s 4M flash.bin` ensures QEMU sees a **4 MB flash chip**.

----------

----------

# ## 7️⃣ Running in QEMU

### _(Using Xtensa ESP32 machine with SPI flash)_

Command used:

`~/qemu/build/qemu-system-xtensa \
  -nographic \
  -machine esp32 \
  -drive file=flash.bin,if=mtd,format=raw` 

Explanation:

-   `qemu-system-xtensa` → emulator for Xtensa CPU
    
-   `-machine esp32` → enables ESP32 peripherals
    
-   `-drive if=mtd` → attaches SPI flash
    
-   `-nographic` → print UART output to terminal
    

----------

### _Bootloader + Partition Table Output_

![Boot](interview-reflection/blink_pics/09_initialize_flash.png)

----------


This confirms the corrected program is finally running properly.

##  Guru Meditation Error (WDT Timeout)
![11_guru_meditation_error.png](../blink_pics/11_guru_meditation_error.png)


Here the ESP32 hits:

`Guru Meditation Error: Core 0 panic'ed (Interrupt wdt timeout)` 

This happened because:

-   The LED prints are too fast.
    
-   QEMU’s simulation timing is slow.
    
-   The FreeRTOS watchdog inside QEMU doesn't get CPU time and resets.
    

This is a **QEMU limitation**, not a code bug.

The real ESP32 will not show this watchdog error.

----------
## 11. Final Working Output

### Final LED ON/OFF Output (Correct)

![Blink Final Output](interview-reflection/blink_pics/10_output.png)

Despite the watchdog errors, the application runs fine afterwards.

LED ON / LED OFF prints appear continuously.

This proves:

-   Build & flash steps were finally correct.
    
-   QEMU watchdog resets are normal for tight loops.
    

----------

# ## Summary of My Actual Mistakes (Blunt + Honest)

1.  **I edited the code but didn’t rebuild before flashing**, so I ended up running an old binary.  
    → This is why the LED worked even when syntax errors were present.
    
2.  **I assumed QEMU detects changes automatically.**  
    → It does not. You _must_ rebuild and regenerate the `.bin`.
    
3.  **I flashed even when build failed.**  
    → This guaranteed that the old firmware was still running.
    
4.  **I misinterpreted the Guru Meditation error.**  
    → It’s caused by QEMU’s timing and is normal behavior.
    
5.  **I renamed folders and changed CMakeLists but didn’t update project properly.**  
    → Caused the “hello_world” project name confusion.