#ifndef EXCEPT_SYS_H
#define EXCEPT_SYS_H

#include "io_sys.h"
#include "syscalls.h"

// --- Base Exception Class ---
class SysException {
protected:
    const char* msg;
public:
    SysException(const char* m) : msg(m) {}
    virtual const char* what() const { return msg; }
};

// --- Derived Exception Classes ---
class BufferOverflowException : public SysException {
public:
    BufferOverflowException(const char* m) : SysException(m) {}
};

class MissingSecondNameException : public SysException {
public:
    MissingSecondNameException() : SysException("No second name") {}
};

class InvalidCharInNameException : public SysException {
public:
    InvalidCharInNameException() : SysException("Invalid character in name") {}
};

class InvalidCharInRollException : public SysException {
public:
    InvalidCharInRollException() : SysException("Invalid character in roll") {}
};

class MarksOverflowException : public SysException {
public:
    MarksOverflowException() : SysException("Marks overflow (value > 100)") {}
};

class NegativeMarksException : public SysException {
public:
    NegativeMarksException() : SysException("Invalid marks (negative value)") {}
};

// --- Error handler ---
static inline void sys_throw_error(const SysException& e) {
    sys_write_str("Exception: ");
    sys_write_ln(e.what());
    // No exit; main() can catch it
}

#endif // EXCEPT_SYS_H
