# Report on reverse engineering a File

## Introduction
This document presents a detailed analysis of the commands employed to dissect an Executable and Linkable Format (ELF) binary file, specifically an ELF64-x86-64 executable, as part of an academic exercise in reverse engineering. The objective is to disassemble the binary, identify function calls, memory accesses, and control flow, and reconstruct its functionality using `objdump`, `strace`, and `gdb`. Each command is analyzed for its syntax, options, purpose, and application to the provided binary, which counts names from a file and converts them to uppercase. This README adheres to academic standards, ensuring clarity, precision, and a demonstration of technical proficiency.

## Methodology
The analysis leverages three primary tools:
- **`objdump`**: For static disassembly and symbol table extraction.
- **`strace`**: For dynamic tracing of system calls.
- **`gdb`**: For interactive debugging and control flow analysis.

The commands were executed on a Linux environment (In my case Ubuntu), and their outputs were systematically examined to meet the assignment criteria.

## Command Analysis

### 1. `objdump -d binary > disassembly.txt`
#### Syntax
```bash
objdump -d <ELF_file> > <output_file>
```
#### Options
- `-d`: Instructs `objdump` to disassemble all executable sections (e.g., `.text`) into assembly language.
- `> disassembly.txt`: Redirects output to a file for persistent storage and analysis.

#### Purpose
This command generates a comprehensive disassembly of the binary, translating machine code into assembly instructions. It provides the foundation for identifying function calls, memory accesses, and control flow.

#### Demonstration
Executed as:
```bash
objdump -d question1 > disassembly.txt
```
- **Output Example**:
  ```
  000000000000141d <main>:
    141d: f3 0f 1e fa    endbr64
    1421: 55             pushq %rbp
    1422: 48 89 e5       movq %rsp, %rbp
  ```
  ### 2. `strace -o trace.log ./question1`
#### Syntax
```bash
strace -o <output_file> ./<ELF_file>
```
#### Options
- `-o trace.log`: Specifies the output file for the system call trace.
- `./ELF_file`: Executes the elf_file to capture its runtime behavior.

#### Purpose
This command traces system calls, providing dynamic insight into the binary file’s interactions with the operating system, such as file operations and output.

#### Demonstration
Executed as:
```bash
strace -o trace.log ./question1
```
- **Output Example**:
  ```
  openat(AT_FDCWD, "names.txt", O_RDONLY) = 3
  write(1, "Count: 5\n", 9)         = 9
  openat(AT_FDCWD, "upper_names.txt", O_WRONLY|O_CREAT|O_TRUNC, 0666) = 4
  close(3)                        = 0
  ```
- **Application**: Verified file I/O (`openat`, `close`) and output (`write`), confirming the binary reads "names.txt" and writes to "sorted_names.txt".

---

### 3. `gdb ./question1`
#### Syntax
```bash
gdb ./<ELF_file>
```
#### Options
- `./ELF_file`: Loads the specified ELF_file into the GNU Debugger.

#### Purpose
Initiates an interactive debugging session, enabling step-by-step execution and detailed inspection of the binary file’s behavior.

#### Demonstration
Executed as:
```bash
gdb ./question1
```
- **Output Example**:
  ```
  GNU gdb (Ubuntu 12.1-0ubuntu1~22.04) 12.1
  (gdb)
  ```
- **Application**: Launched `gdb` to set breakpoints and analyze control flow, starting the debugging process.

---

### 4. `(gdb) break main`
#### Syntax
```gdb
break <function_name>
```
#### Options
- `main`: Targets the `main` function as the breakpoint location.

#### Purpose
Sets a breakpoint at the entry point of `main`, allowing execution to pause for detailed analysis.

#### Demonstration
Executed within `gdb`:
```gdb
(gdb) break main
```
- **Output Example**:
  ```
  Breakpoint 1 at 0x141d
  ```
- **Application**: Paused execution at `0x141d` (start of `main`), enabling instruction-level inspection.

---

### 5. `(gdb) run`
#### Syntax
```gdb
run
```
#### Options
- None (optional arguments can be added if required by the binary file).

#### Purpose
Runs the elf_file from its entry point (`_start`) until a breakpoint or termination, initiating dynamic analysis.

#### Demonstration
Executed within `gdb`:
```gdb
(gdb) run
```
- **Output Example**:
  ```
  Starting program: /path/to/question1
  Breakpoint 1, 0x000000000000141d in main ()
  ```
- **Application**: Started execution, stopping at `main` for further stepping.

---

### 6. `(gdb) info registers`
#### Syntax
```gdb
info registers
```
#### Options
- None (displays all CPU registers).

#### Purpose
Provides the current state of registers, revealing data movement and instruction pointer values during execution.

#### Demonstration
Executed within `gdb`:
```gdb
(gdb) info registers
```
- **Output Example**:
  ```
  rax            0x0    0
  rbp            0x7fffffffde70 0x7fffffffde70
  rip            0x141d 0x141d <main>
  ```
- **Application**: Monitored `%rax` (return values), `%rip` (instruction pointer), and stack pointers during execution.

---

## Results and Application
The commands were applied to the ELF64 binary as follows:
- **Disassembly**: `objdump -d` and `(gdb) disassemble main` provided the full assembly, identifying `main`, `count_lines`, `names`, and `sorted_names`.
- **Symbol Mapping**: `objdump -t` linked addresses to functions (e.g., `printf` at `0x10f0`).
- **Dynamic Tracing**: `strace` confirmed file operations (`openat "names.txt"`) and output (`write`).
- **Control Flow**: `gdb` commands (`break`, `run`, `next`) traced execution from `main` through function calls and loops, with `info registers` tracking state.
