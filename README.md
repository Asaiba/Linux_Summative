# Systems Programming and Reverse Engineering Assessment

## Overview

This assessment comprises five major tasks covering reverse engineering, low-level programming, performance optimization, multithreading, and network programming. The goal is to develop practical experience with ELF binary analysis, x86 Assembly, C extensions for Python, multithreading, and socket-based inter-process communication.

---

## Assessment Goals

- Analyze ELF binaries and reconstruct their functionalities.
- Implement x86 assembly code and execute it on Linux.
- Create and integrate C extensions into Python programs.
- Develop multi-threaded systems with proper synchronization.
- Build inter-process communication systems using sockets.

---

## Questions and Deliverables

### ✅ Question 1 – Reverse Engineering an ELF Binary [6pts]

**Objective:**  
Perform static and dynamic analysis on an ELF binary using:
- `objdump` (for disassembly)
- `strace` (for runtime syscall tracing)
- `gdb` (for execution flow and memory inspection)

**Steps:**
- List all functions identified in disassembly.
- Highlight conditional/unconditional jumps and explain control flow.
- Use `strace` to identify system calls and I/O/memory interactions.
- Use `gdb` to:
  - Set breakpoints
  - Trace memory and function calls

**Deliverables:**
- Report including:
  - Control flow diagram or textual explanation
  - Function purposes and logic
  - Memory access insights (stack/heap/globals)
  - Summary of `strace` output and runtime behavior

---

### ✅ Question 2 – File Line Counter in x86 Assembly [2pts]

**Objective:**  
Implement an x86 assembly program to:
- Open and read a file containing 30–100 names.
- Traverse line-by-line and count non-empty lines.
- Print the final count to the console.

**Deliverables:**
- Assembly source file (`line_counter.asm`)
- Commented explanation on:
  - File handling logic
  - String traversal and line counting
  - Loop and conditional structure

---

### ✅ Question 3 – C Extension for Python Statistics [5pts]

**Objective:**  
Optimize performance by implementing a C extension to replace a heavy Python statistics module.

**Functions to Implement:**
- `sum_values()`
- `mean_values()`
- `std_dev()`
- `mode_value()`
- `count_values()`

**Deliverables:**
- `statsmodule.c` – C source code for Python extension
- `setup.py` – Setup script for building the module
- `test_stats.py` – Sample Python script demonstrating usage
- Code documentation including:
  - Explanation of each function
  - Memory usage notes
  - Time complexity insights

---

### ✅ Question 4 – Multithreaded Producer-Consumer Simulation [6pts]

**Objective:**  
Simulate a manufacturing system using:
- A producer thread that adds items every 2 seconds.
- A consumer thread that removes and processes items every 3 seconds.
- Thread synchronization for race-free operation on a bounded queue (size 10).

**Deliverables:**
- Source code (e.g., `producer_consumer.c`)
- Sample console output showing production and packaging events
- Code comments explaining:
  - Thread synchronization (e.g., mutex, condition variables)
  - Shared queue memory handling
  - Queue behavior during full and empty states

---

### ✅ Question 5 – Real-Time Multi-Client Chat System [6pts]

**Objective:**  
Design a Linux-based chat system using TCP sockets that:
- Supports up to 4 clients with username-based authentication.
- Shows online users.
- Allows real-time, bidirectional messaging.

**Server Responsibilities:**
- Authenticate users
- Manage list of online users
- Route messages

**Client Responsibilities:**
- Prompt username
- Show online users
- Initiate chat with another user
- Format messages as: `From <sender>: <message>`

**Deliverables:**
- `server.c` – Central server source code
- `client.c` – Client application source code
- Demo log or terminal output of:
  - User login and authentication
  - Display of online users
  - Real-time message exchange
- Code documentation explaining:
  - Authentication process
  - Message routing and delivery
  - Thread/concurrency management

---

## How to Run

For C and Assembly programs:
```bash
gcc -o program program.c -lpthread        # Compile C programs

nasm -f elf64 line_counter.asm && ld -o line_counter line_counter.o  # Compile assembly
./program
```

For the Python C extension:

```bash
python3 setup.py build_ext --inplace
python3 test_stats.py
```
For the chat system:

```bash
# In separate terminals:
./server
./client
```

Dependencies
`gcc`, `gdb`, `strace`, `objdump`, `nasm`
Python 3.x with `setuptools` and `distutils`
POSIX-compliant system (e.g., Linux)
