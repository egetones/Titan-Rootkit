<div align="center">

# Titan LKM Rootkit

![C](https://img.shields.io/badge/Language-C-blue?style=flat&logo=c)
![Linux](https://img.shields.io/badge/Platform-Linux_Kernel-yellow?style=flat&logo=linux)
![Type](https://img.shields.io/badge/Type-LKM_Rootkit-red)

<p>
  <strong>A Proof-of-Concept (PoC) Linux Kernel Module demonstrating Direct Kernel Object Manipulation (DKOM) for process hiding.</strong>
</p>

[Report Bug](https://github.com/your-username/titan-rootkit/issues)

</div>

---

## ⚠️ CRITICAL WARNING

**This is a Ring-0 (Kernel Mode) project.**
Coding errors in this space will not just crash the program; they will crash the entire Operating System (**Kernel Panic**).
  **DO NOT** run this on your host machine.
  **ONLY** use a Virtual Machine (Fedora/Ubuntu/Kali) that you can snapshot/restore.

---

## Description

**Titan** is a minimalist Linux Kernel Module (LKM) designed to demonstrate how rootkits operate at the deepest level of the OS.

Unlike user-land rootkits (like `LD_PRELOAD` or `ptrace` based tools), Titan operates in **Ring 0**. It does not hook system calls; instead, it directly manipulates the kernel's internal memory structures.

### The Concept: DKOM (Direct Kernel Object Manipulation)

When you run commands like `lsmod`, the kernel iterates through a specific data structure called a **Doubly Linked List**. Every loaded module is a node in this list.

Titan performs a "magic trick" upon loading:
1.  It locates its own entry in the kernel's module list.
2.  It modifies the `prev` and `next` pointers of the adjacent modules to point to each other, bypassing Titan.
3.  **Result:** Titan remains active in memory, but the kernel's listing tools physically skip over it during iteration.



---

## Usage

### 1. Prerequisites
You need kernel headers installed to compile LKM code.
```bash
# On Fedora
sudo dnf install kernel-devel kernel-headers
```

### 2. Compile
Use the provided Makefile to build the kernel object (`.ko`).
```bash
make
```

### 3. Load & Vanish
Insert the module into the kernel. It is programmed to hide immediately upon initialization.
```bash
sudo insmod titan.ko
```

### 4. Verify "Invisibility"
Check the kernel logs to see that it loaded:
```bash
sudo dmesg | tail
# Output: [Titan]: Yüklendi ve karanlığa karışıyor...
```

Now, try to find it with standard tools:
```bash
lsmod | grep titan
# Output: (EMPTY)
```
*The module is running, but `lsmod` cannot see it.*

### 5. Unload (Recovery)
Since the module is hidden from the system list, standard removal via `rmmod` might fail or require specific address targeting. The simplest way to unload this PoC is to **reboot the machine**.

---

## ⚠️ Disclaimer

This software is for **EDUCATIONAL USE ONLY**. It demonstrates weaknesses in kernel structure protection. Developing or deploying rootkits on unauthorized systems is a severe cybercrime. The author assumes no liability for misuse.

---

## License

Distributed under the MIT License. See `LICENSE` for more information.
