# MiniShell

A simple Unix-like shell implemented in C++.

This project was built to understand how shells work internally, including:

- Reading user input
- Parsing commands and arguments
- Process creation using `fork()`
- Program execution using `execvp()`
- Process synchronization using `waitpid()`
- understanding parallel programming
- Built-in shell commands (`cd`, `help`, `exit`)
- we still separating arguments with space, no quoting or backslash escaping.

---

## Requirements

- Linux or Unix-like operating system
- GCC/G++ 15 or later

Verify your compiler version:

```bash
g++ --version
```

---

## Build

Compile the shell:

```bash
g++ myshell.cpp -o psh
```

---

## Run

Start the shell:

```bash
./psh
```

Example:

```bash
psh> pwd
psh> ls -l
psh> cd ..
psh> help
psh> exit
```

---

## Debug Build

Compile with debugging symbols:

```bash
g++ -g myshell.cpp -o gdbsh
```

Run the debug build:

```bash
./gdbsh
```

Launch with GDB:

```bash
gdb ./gdbsh
```

---

## Features

- Execute external commands
- Built-in commands:
  - `cd`
  - `help`
  - `exit`
- Parent/child process management
- Error handling for invalid commands

---

## Learning Objectives

This project was created to gain hands-on experience with:

- Operating system concepts
- Unix process management
- System calls
- Shell architecture
- C++ systems programming

---

## Future Improvements

- Command history
- Pipes (`|`)
- Background jobs (`&`)
- Signal handling (`Ctrl+C`, `Ctrl+Z`)
