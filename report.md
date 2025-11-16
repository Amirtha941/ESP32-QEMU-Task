# ESP32 Emulation using QEMU – Full Report  

This report documents, step-by-step, how I set up an **ESP32 emulation environment** using:

- **QEMU (Espressif fork)** – to emulate the ESP32 chip  
- **ESP-IDF** – the official ESP32 development framework  

Two applications were developed and run entirely in the emulator:

1. **LED Blink** (GPIO toggle – printed as `LED ON / LED OFF`)  
2. **Temperature Monitor** (simulated temperature values in °C)

The report is written in a **beginner-friendly** way so that anyone familiar with basic Linux can follow the commands and reproduce the setup or use it as a base for **open-source contributions**.

---

## 1. System Information

| Item                     | Details (from commands & logs) |
|--------------------------|---------------------------------|
| Host OS                  | Ubuntu (inside VirtualBox)      |
| Shell                    | Bash                            |
| Python                   | 3.10.x (`python3 --version`)    |
| ESP-IDF used finally     | **v5.1** (for Blink & Temp)     |
| QEMU version             | **9.2.2** (Espressif fork)      |
| Target MCU               | ESP32                           |

---

## 2. Installing Prerequisite Packages

Before building ESP-IDF and QEMU, I installed all development tools and libraries required.

### 2.1 Install build tools and libraries

```bash
sudo apt install -y git python3 python3-pip cmake make gcc g++ \
  libglib2.0-dev libfdt-dev libpixman-1-dev zlib1g-dev \
  ninja-build flex bison
```
**What this does (line by line):**

-   `git` – required to clone ESP-IDF and QEMU repositories.
    
-   `python3` & `python3-pip` – ESP-IDF tools are Python-based.
    
-   `cmake`, `make`, `gcc`, `g++` – standard C/C++ build tools.
    
-   `libglib2.0-dev`, `libfdt-dev`, `libpixman-1-dev`, `zlib1g-dev` – libraries QEMU needs to build properly.
    
-   `ninja-build` – fast build system used by QEMU and ESP-IDF.
    
-   `flex`, `bison` – parser generators needed by some build steps.
    

📷 **Screenshot 01 – Prerequisite installation**  
![Prerequisite installation](./Screenshots/01_prereq_install.png)

----------

### 2.2 Verify key tools

```
python3 -m pip install --user pyserial # serial communication helper git --version
python3 --version
cmake --version
make --version
gcc --version
``` 

This confirms all tools are installed and available in `PATH`.

📷 **Screenshot 02 – Tool verification**  
![Tool verification](./Screenshots/02_tool_verification.png)

----------

## 3. Building QEMU (Espressif Fork)

QEMU is a generic emulator. Espressif maintains a fork that adds ESP32 support.

### 3.1 Clone and configure QEMU

```
cd ~
git clone https://github.com/espressif/qemu.git cd qemu mkdir build && cd build # First configure attempt ../configure --target-list=xtensa-softmmu --enable-debug
``` 

-   `git clone …` downloads the QEMU source.
    
-   `mkdir build && cd build` – keeps build files separate from source.
    
-   `--target-list=xtensa-softmmu` – enables the **Xtensa** CPU backend (used by ESP32).
    
-   `--enable-debug` – compiles QEMU with debug symbols.
    

📷 **Screenshot 03 – Clone & first configure**  
![QEMU clone and configure](./Screenshots/03_qemu_clone_configure.png)

----------

### 3.2 Fix 1 – Missing `gcrypt.h`

Running `ninja` initially failed:

`ninja # … fatal error: gcrypt.h: No such file or directory` 

To fix this, I installed the development package:

`sudo apt install -y libgcrypt20-dev` 

📷 **Screenshot 04 – gcrypt.h error**  
![gcrypt error](./Screenshots/04_qemu_build_error_gcrypt.png)

----------

### 3.3 Fix 2 – Missing `libslirp.h`

The next `ninja` build stopped with:

`fatal error: libslirp.h: No such file or directory` 

This is a networking library QEMU uses. Fixed by:

`sudo apt install -y libslirp-dev` 

📷 **Screenshot 05 – libslirp error**  
![slirp error](./Screenshots/05_qemu_build_error_slirp.png)

----------

### 3.4 Fix 3 – Python `tomli` missing

Later, while configuring QEMU again, the script reported:

`*** No usable tomli, please install it` 

📷 **Screenshot 17 – tomli missing during configure**  

I installed the missing Python library:

`sudo apt install python3-tomli` 

📷 **Screenshot 18 – Installing python3-tomli**  

----------

### 3.5 Successful QEMU configuration & build

After fixing all dependencies, I ran configure again:

```
cd ~/qemu/build
../configure --target-list=xtensa-softmmu --enable-debug
``` 

📷 **Screenshot 19 – Configure summary (success)**  

Then built QEMU:

```
ninja
./qemu-system-xtensa --version
``` 

📷 **Screenshot 06 – QEMU built and version displayed**  
![QEMU build success](./Screenshots/06_qemu_build_success.png)

At this point, QEMU (with ESP32 support) was ready.

----------

## 4. Installing ESP-IDF

ESP-IDF is the official framework for ESP32. I used **two clones** during experiments (master and v5.1 branch), but the final working setup uses **ESP-IDF v5.1**.

### 4.1 Clone ESP-IDF

```
cd ~
git clone --recursive https://github.com/espressif/esp-idf.git cd esp-idf
``` 

-   `--recursive` fetches all submodules (components, tools, etc.).
    

📷 **Screenshot 07 – ESP-IDF clone**  
![ESP-IDF clone](./Screenshots/07_espidf_clone.png)

----------

### 4.2 Install ESP-IDF tools

`./install.sh` 

This script:

-   Detects Python,
    
-   Downloads the **Xtensa** and **RISC-V** toolchains,
    
-   Installs Python packages required by ESP-IDF.
    

📷 **Screenshot 08 – First ESP-IDF tool installation**  
![ESP-IDF install tools](./Screenshots/08_espidf_install_tools.png)

Later I also cloned the **v5.1 branch** separately (not shown again as code) and ran **another `./install.sh`**, visible here:

📷 **Screenshot 14 – ESP-IDF v5.1 tools installing**  

----------
---

## 4.3 Exporting the ESP-IDF Environment (Screenshots 09 & 15)

Before using any ESP-IDF command such as `idf.py build`, the environment must be activated.

### 🔧 Command
```bash
cd ~/esp-idf        # or esp-idf-v5.1 depending on the clone
. ./export.sh       # note the dot and space
idf.py --version    # verify tool availability
```

📝 Explanation

`. ./export.sh` loads ESP-IDF Python virtual environment

Adds toolchain binaries (xtensa compiler) to PATH

Enables idf.py globally

📷 Screenshot 09 – Export & IDF version
![ESP-IDF export](./Screenshots/09_export_idf_version.png)

![ESP-IDF export](./Screenshots/10_blink_code.png)

![ESP-IDF export](./Screenshots/11_blink_build_config.png)

![ESP-IDF export](./Screenshots/12_blink_build_done.png)

![ESP-IDF export](./Screenshots/14_espidf_install_tools.png)

📷 Screenshot 15 – Export for ESP-IDF v5.1

![ESP-IDF export](./Screenshots/15_espidf_export_environment.png)
4.4 Fixing Missing ESP-IDF Tools (Screenshot 22 & 23)

While building the Blink project, ESP-IDF reported missing Python environment:

idf.py: python_env/idf5.1_py3.10_env/bin/python doesn't exist

🔧 Fix
cd ~/esp-idf-v5.1
./install.sh
. ./export.sh

📷 Screenshot 22 – Missing tools warning

📷 Screenshot 23 – Export success

5. Creating the Blink Application (Screenshots 10–12)
5.1 Creating and Editing the Blink Project (Screenshot 10)
🔧 Commands
cd ~/esp-idf-v5.1
idf.py create-project blink
cd blink
nano main/main.c

📝 Explanation

Creates a template ESP-IDF project

Editing main.c lets us simulate LED blinking with console logs

📷 Screenshot 10 – Blink main.c

5.2 Building the Blink Project (Screenshots 11 & 12)
🔧 Command
idf.py build

📝 Explanation

This performs:

Toolchain detection

CMake configuration

App compilation

Linking to produce blink.elf

📷 Screenshot 11 – Build Setup

📷 Screenshot 12 – Build Complete

5.3 Full Clean & Rebuild (Screenshots 16 & 24)
🔧 Commands
idf.py fullclean
idf.py build

📝 Explanation

Ensures reproducible builds

Deletes old build files

Great for open-source CI/CD

📷 Screenshot 16 – Full Clean + Build

📷 Screenshot 24 – Clean Build (Final)

6. Errors While Running Blink in QEMU (Screenshots 13, 20, 21)
6.1 Wrong QEMU Command Attempts

Several incorrect attempts were made using .elf directly:

❌ Wrong Command
qemu-system-xtensa -machine esp32 -kernel build/blink.elf

📝 Why it Failed

ESP32 QEMU requires:

A flash image

NOT a raw ELF

Correct interface: if=mtd,format=raw

📷 Screenshot 13 – Wrong command

📷 Screenshot 20 – Incorrect kernel/BIOS mix

📷 Screenshot 21 – Errors before using merge_bin

7. Creating the Correct Flash Image (Screenshot 25)
7.1 Merging Bootloader + Partitions + App
🔧 Command
cd ~/blink
esptool.py --chip esp32 merge_bin -o flash.bin \
  0x1000 build/bootloader/bootloader.bin \
  0x8000 build/partition_table/partition-table.bin \
  0x10000 build/blink.bin

📝 Explanation

ESP32 bootloader is always at 0x1000

Partition table at 0x8000

App binary at 0x10000

📷 Screenshot 25

7.2 Running Blink in QEMU (Screenshots 26–29)
🔧 Correct Command
~/qemu/build/qemu-system-xtensa \
  -nographic \
  -machine esp32 \
  -drive file=flash.bin,if=mtd,format=raw

📷 Screenshot 26 – QEMU Bootloader

📷 Screenshot 27 – Blink App Start

📷 Screenshot 28 – LED ON/OFF Output

📷 Screenshot 29 – Continuous LED Logs

8. Temperature Monitoring Application (Screenshots 30–33)
8.1 Project Setup & Environment Export (Screenshot 30)
🔧 Commands
cp -r ~/esp-idf-v5.1/examples/get-started/hello_world ~/temperature
cd ~/temperature
. ~/esp-idf-v5.1/export.sh

📷 Screenshot 30

8.2 Building the Temperature App (Screenshot 31)
🔧 Command
idf.py build

📝 Explanation

Checks:

Python packages

ESP-IDF configuration

Toolchain setup

📷 Screenshot 31

8.3 Creating Flash Image and Running QEMU (Screenshot 32)
🔧 Command
esptool.py --chip esp32 merge_bin -o flash.bin \
  0x1000 build/bootloader/bootloader.bin \
  0x8000 build/partition_table/partition-table.bin \
  0x10000 build/hello_world.bin

truncate -s 4M flash.bin

~/qemu/build/qemu-system-xtensa \
  -nographic \
  -machine esp32 \
  -drive file=flash.bin,if=mtd,format=raw

📷 Screenshot 32

8.4 Temperature Output in QEMU (Screenshot 33)

The application prints simulated temperature values repeatedly.

Example Output
I (2729) TEMP: Temperature: 26 °C
I (3739) TEMP: Temperature: 27 °C
...

📷 Screenshot 33

### 4.3 Export the ESP-IDF environment

To use `idf.py`, you must “activate” ESP-IDF in each new terminal:

`cd ~/esp-idf # or ~/esp-idf-v5.1 for the v5.1 clone . ./export.sh # note the dot + space idf.py --version` 

-   `. ./export.sh` configures `PATH`, `IDF_PATH`, Python venv, etc.
    
-   `idf.py --version` confirms the active ESP-IDF version.
    

📷 **Screenshot 09 – export + idf version (first clone)**  

📷 **Screenshot 15 – export for ESP-IDF v5.1**  

----------

### 4.4 Fixing missing tools later

At one point, running `idf.py` inside the Blink project failed because the venv for v5.1 was missing:

`ERROR: … python_env/idf5.1_py3.10_env/bin/python doesn’t exist` 

📷 **Screenshot 22 – Missing tools / reinstall request**  

To fix it, I simply re-ran:

```
cd ~/esp-idf-v5.1
./install.sh
. ./export.sh
``` 

📷 **Screenshot 23 – ESP-IDF v5.1 export success**  

Now ESP-IDF v5.1 was fully functional.

----------

## 5. Creating the Blink Application (LED Toggle)

### 5.1 Create project and edit `main.c`

```
cd ~ cd esp-idf-v5.1
idf.py create-project blink cd ~/blink
nano main/main.c
``` 

I replaced the default code with a simple log-based blink:

```
#include  "freertos/FreeRTOS.h"  #include  "freertos/task.h"  #include  "esp_log.h"  static  const  char *TAG = "BLINK"; void  app_main(void)
{ while (1) {
        ESP_LOGI(TAG, "LED ON");
        vTaskDelay(pdMS_TO_TICKS(500));
        ESP_LOGI(TAG, "LED OFF");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
``` 

📷 **Screenshot 10 – `main.c` for Blink**  

----------

### 5.2 First build of Blink

```
cd ~/blink
idf.py build
``` 

This command:

-   Configures CMake,
    
-   Detects the Xtensa toolchain,
    
-   Generates the `sdkconfig` file,
    
-   Builds all components and links the final ELF (`build/blink.elf`).
    

📷 **Screenshot 11 – Build configuration and compile**  

After configuration, the build finished successfully:

📷 **Screenshot 12 – Blink build complete**  

----------

### 5.3 Cleaning and rebuilding (for reproducibility)

To be sure the project builds cleanly from scratch, I used:

`idf.py fullclean # remove build/ and managed components idf.py build # build again` 

📷 **Screenshot 16 – Full clean + build (first time)**  

Later (after fixing tools again) I repeated:

📷 **Screenshot 24 – Full clean + build (final)**  

This is a good practice for open-source work – it proves the project is reproducible on any clean machine.

----------

## 6. Early Attempts to Run Blink in QEMU (and Errors)

Running ESP32 firmware in QEMU isn’t as simple as just pointing it at the ELF; you must provide a **flash image** or at least the right options.

### 6.1 Wrong invocation #1 – treating ELF as flash

```
~/qemu/build/qemu-system-xtensa \
  -nographic \
  -machine esp32 \
  -drive file=build/blink.elf,if=mtd,format=raw
``` 

QEMU reported:

-   “Error: only 2, 4, 8, 16 MB flash images are supported”
    
-   “machine type does not support if=mtd,bus=0,unit=0”
    

📷 **Screenshot 13 – Wrong drive / flash usage**  

📷 **Screenshot 20 – Another incorrect kernel/BIOS combination**  

📷 **Screenshot 21 – Blink build + flashbin error**  

These runs taught an important lesson: **QEMU’s ESP32 machine expects a flash image, not a raw ELF.**

----------

## 7. Generating a Proper Flash Image for Blink

### 7.1 Create `flash.bin` from bootloader + partitions + app

ESP-IDF prints a helpful command after build. I used it (adapted for my paths):

```
cd ~/blink
esptool.py --chip esp32 merge_bin -o flash.bin \
  0x1000  build/bootloader/bootloader.bin \
  0x8000  build/partition_table/partition-table.bin \
  0x10000 build/blink.bin
``` 

-   `merge_bin` combines the bootloader, partition table and app into one flash layout.
    
-   `0x1000`, `0x8000`, `0x10000` – standard ESP32 flash offsets.
    

📷 **Screenshot 25 – `flash.bin` created successfully**  

Because QEMU expects a **fixed flash size**, I also truncated:

`truncate -s 4M flash.bin # pad/resize flash image to 4 MB` 

(Shown clearly in the temperature section as well.)

----------

### 7.2 First successful QEMU boot for Blink

Finally, I could boot the firmware:

```
~/qemu/build/qemu-system-xtensa \
  -nographic \
  -machine esp32 \
  -drive file=flash.bin,if=mtd,format=raw
``` 

-   `-nographic` – disables GUI, uses terminal only.
    
-   `-machine esp32` – use Espressif’s ESP32 machine model.
    
-   `-drive …if=mtd,format=raw` – tells QEMU to treat `flash.bin` as SPI flash.
    

📷 **Screenshot 26 – ESP-IDF bootloader & partition table in QEMU**  

📷 **Screenshot 27 – Application start & system info**  

Logs show:

-   Bootloader banner
    
-   Chip revision & frequency
    
-   App name: `blink`
    
-   ESP-IDF version: **v5.1**
    
-   Starting scheduler on CPU0 & CPU1
    
-   Calling `app_main()`
    

----------

### 7.3 LED toggle logs in QEMU

Once `app_main()` runs, the application prints:

`LED ON
LED OFF
LED ON
LED OFF
...` 

📷 **Screenshot 28 – LED ON/OFF logs start**  

📷 **Screenshot 29 – Long LED output + QEMU termination**  

This confirms the **Blink application runs continuously** in the emulator and behaves exactly like it would on real hardware (only the LED is “virtual”, represented by logs).

----------

## 8. Temperature Monitoring Application

Next, I implemented a **temperature simulation** example based on `hello_world`.

### 8.1 Create project and set up environment

```
cp -r ~/esp-idf-v5.1/examples/get-started/hello_world ~/temperature cd ~/temperature
nano main/hello_world_main.c
``` 

`hello_world_main.c` was edited so that `app_main()` periodically prints random or incremental temperature values, tagged with `TEMP`.

Initially, trying to build failed because `idf.py` wasn’t found in the current shell (environment not exported). I fixed it with:

`. ~/esp-idf-v5.1/export.sh` 

📷 **Screenshot 30 – Temperature project setup & export**  

----------

### 8.2 Building temperature firmware

After exporting the environment, I ran:

```
cd ~/temperature
idf.py build
``` 

This triggered the normal ESP-IDF build process:

-   Python requirements check
    
-   Toolchain detection
    
-   Project compile & link
    

📷 **Screenshot 31 – Build + Python / tool checks**  

----------

### 8.3 Merging binaries into `flash.bin` and running QEMU

Similar to the Blink app, I created a flash image:

```
cd ~/temperature
esptool.py --chip esp32 merge_bin -o flash.bin \
  0x1000  build/bootloader/bootloader.bin \
  0x8000  build/partition_table/partition-table.bin \
  0x10000 build/hello_world.bin # Ensure size is 4 MB to satisfy QEMU  truncate -s 4M flash.bin
``` 

Then launched QEMU:

```
~/qemu/build/qemu-system-xtensa \
  -nographic \
  -machine esp32 \
  -drive file=flash.bin,if=mtd,format=raw
  ``` 

📷 **Screenshot 32 – QEMU boot & temperature app flash image**  

The log shows:

-   ESP-IDF v5.1 2nd-stage bootloader
    
-   Partition table
    
-   App loading from offset `0x10000`
    

----------

### 8.4 QEMU runtime – temperature readings

Once `app_main()` runs, the application prints simulated temperature values:

```
I (2729) TEMP: Temperature: 26 °C
I (3739) TEMP: Temperature: 27 °C
I (4739) TEMP: Temperature: 29 °C
...
``` 

Temperatures vary between ~26 °C and 40 °C, demonstrating a continuous sensor reading loop.

📷 **Screenshot 33 – Long temperature output & graceful termination**  

The final line `QEMU: Terminated` confirms that the emulator session ended cleanly after producing the desired logs.

----------

## 9. Summary of Key Commands (Cheat-Sheet)

### QEMU build (once per machine)

```
# Clone QEMU (Espressif fork) git clone https://github.com/espressif/qemu.git cd qemu mkdir build && cd build # Install missing libs if needed sudo apt install -y libgcrypt20-dev libslirp-dev python3-tomli # Configure & build ../configure --target-list=xtensa-softmmu --enable-debug
ninja
./qemu-system-xtensa --version
``` 

### ESP-IDF setup (once per clone)

```
git clone --recursive https://github.com/espressif/esp-idf.git esp-idf-v5.1 cd esp-idf-v5.1
./install.sh
. ./export.sh
idf.py --version
``` 

### Build + run any ESP-IDF project on QEMU

`# 1. Build project idf.py fullclean
idf.py build # 2. Merge binaries into one flash image esptool.py --chip esp32 merge_bin -o flash.bin \
  0x1000  build/bootloader/bootloader.bin \
  0x8000  build/partition_table/partition-table.bin \
  0x10000 build/<app>.bin # 3. Force flash size to 4 MB  truncate -s 4M flash.bin # 4. Run in QEMU ~/qemu/build/qemu-system-xtensa \
  -nographic \
  -machine esp32 \
  -drive file=flash.bin,if=mtd,format=raw` 

Replace `<app>.bin` with `blink.bin`, `hello_world.bin`, etc.

----------

## 10. Challenges & How They Were Solved

Category

Problem

Fix

System dependencies

`gcrypt.h` / `libslirp.h` missing during QEMU build

Installed `libgcrypt20-dev` and `libslirp-dev`

Python packages

QEMU configure: _No usable tomli_

Installed `python3-tomli`

ESP-IDF environment

`idf.py` not found / Python venv missing

Re-ran `./install.sh` and `. ./export.sh` for ESP-IDF v5.1

QEMU flash handling

Treating ELF as flash (`if=mtd` errors, wrong size)

Used `esptool.py merge_bin` + `truncate -s 4M flash.bin`

Reproducibility

Stale build artifacts

Regularly used `idf.py fullclean` before final builds

----------

## 11. Learnings (Useful for Open-Source Contributions)

From this exercise, I learned:

-   How to set up a **complete ESP32 toolchain** on Linux using ESP-IDF.
    
-   How to build and debug **QEMU (Espressif fork)** and understand its dependency chain.
    
-   The **binary layout** of ESP32 flash (bootloader, partition table, app offset).
    
-   How to convert a normal ESP-IDF project build into a QEMU-compatible **flash image**.
    
-   How FreeRTOS-based apps (`app_main()`, tasks, logs) behave when emulated.
    
-   How to systematically debug build and runtime issues (missing packages, wrong options).
    

These skills are directly relevant when:

-   Contributing to **ESP-IDF examples or tools**,
    
-   Working on **QEMU/ESP32 support**,
    
-   Or building **automated CI pipelines** where firmware is tested in QEMU instead of on real boards.
    

----------

## 12. Potential Use in Platforms like Yaksh

With this setup:

-   Student submissions (Blink, sensor code, etc.) can be compiled using `idf.py`.
    
-   The generated `flash.bin` can be **executed in QEMU**.
    
-   Scripts can capture the console output and automatically test:
    
    -   Whether temperature prints are in a valid range,
        
    -   Whether LED toggles happen periodically, etc.
        

This removes the need for physical ESP32 hardware while still keeping behavior close to real devices.

----------

## 13. Conclusion

This report demonstrated, step by step:

1.  Installing all Linux prerequisites for ESP-IDF and QEMU.
    
2.  Building Espressif’s QEMU with proper dependencies.
    
3.  Installing and configuring ESP-IDF v5.1.
    
4.  Creating, building and **successfully emulating**:
    
    -   A **Blink** application (LED ON/OFF logs).
        
    -   A **Temperature monitor** application (simulated °C readings).
        
5.  Documenting every issue encountered and its fix.
    

With the commands and screenshots provided, **any beginner** should be able to:

-   Reproduce the environment,
    
-   Understand what each command does, and
-   Use this setup as a foundation for further open-source contributions to ESP-IDF and ESP32 emulation.
