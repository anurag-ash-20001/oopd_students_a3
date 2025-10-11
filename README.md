# OOPD Student Management System (Syscall-Only Version)

![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![Linux](https://img.shields.io/badge/Platform-Linux-green)
![Syscalls](https://img.shields.io/badge/Implementation-Syscalls-orange)

## Overview

This project implements a **student management system** for the **OOPD course (Monsoon 2025)** using **pure system calls only**, without relying on any C++ standard libraries.  
The system supports:

- Tracking students of **all levels** (BTech, MTech, PhD)  
- Tracking **branches** (CSE, ECE)  
- Storing **marks and grades** for each course component  
- Adding, modifying, and removing student records using **roll numbers**  
- **Exception handling** for invalid inputs  
- **Sorting students** by **name (using Trie), roll number, or marks**  
- Persistent storage in **CSV files** (`students.csv`)  

---

## Features

### 1. Object-Oriented Design
- `Student` class: stores individual student info privately  
- `StudentDB` class: manages the collection of students using a **custom vector**  
- Operator overloading implemented for:  
  - `+` to add students  
  - `=` or custom functions to modify records  

### 2. Exception Handling
Custom exception classes handle:

- Buffer overflow in input  
- Missing second name  
- Numbers or special characters in names  
- Unrecognized characters in roll numbers  

All exceptions are thrown at appropriate points during CSV parsing or manual input.

### 3. Total Marks Calculation
- Each student has marks per course component  
- Function `computeTotal()` computes total marks for each student  

### 4. Sorting
- **Name**: Trie-based sorting  
- **Roll number**: Simple pointer-based sorting  
- **Marks**: Sort by a specific course component  

### 5. Syscall-Only Implementation
- No `<iostream>`, `<vector>`, `<string>`, or `<stddef.h>`  
- Custom headers used:  
  - `syscalls.h` → low-level Linux syscalls  
  - `vector_sys.h` → dynamic array (vector) using `mmap`/`munmap`  
  - `stddef_sys.h` → replacement for `size_t`, `NULL`, `offsetof`  
- CSV reading/writing implemented using `SYS_READ` / `SYS_WRITE` syscalls  

---

## File Structure

| File | Description |
|------|-------------|
| `main_sys.cpp` | Main program with CLI menu |
| `student_sys.h` | `Student` class (private members, total marks) |
| `studentdb_sys.h` | `StudentDB` class for managing students, add/modify/search |
| `vector_sys.h` | Custom dynamic vector using syscalls |
| `syscalls.h` | Low-level syscall wrapper (3 & 6 arguments) |
| `stddef_sys.h` | Replacement for `stddef.h` (size_t, NULL, offsetof) |
| `sorting.h` | Trie-based name sorting and other sort helpers |
| `students.csv` | Persistent storage of student data |

---

## Build Instructions

```bash
# Clean previous builds
make clean

# Compile the program
make

# Run
./oopd_students_sys
