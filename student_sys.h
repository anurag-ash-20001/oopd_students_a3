#ifndef STUDENT_SYS_H
#define STUDENT_SYS_H

#include "str_sys.h"
#include "io_sys.h"
#include "except_sys.h"
#include "stddef_sys.h"

#define MAX_ROLL 32
#define MAX_NAME 64
#define MAX_BRANCH 8
#define MAX_LEVEL 8

class Student {
private:
    char roll[MAX_ROLL];
    char name[MAX_NAME];
    char branch[MAX_BRANCH];
    char level[MAX_LEVEL];
    double quiz, assign, mid, end, total;

public:
    Student() { roll[0]=name[0]=branch[0]=level[0]='\0'; quiz=assign=mid=end=total=0.0; }

    Student(const char* r, const char* n, const char* b, const char* l,
            double qv, double av, double mv, double ev) {
        // copy with bounds
        long i=0;
        while (i+1 < MAX_ROLL && r[i]) { roll[i]=r[i]; ++i; } roll[i]='\0';
        i=0; while (i+1 < MAX_NAME && n[i]) { name[i]=n[i]; ++i; } name[i]='\0';
        i=0; while (i+1 < MAX_BRANCH && b[i]) { branch[i]=b[i]; ++i; } branch[i]='\0';
        i=0; while (i+1 < MAX_LEVEL && l[i]) { level[i]=l[i]; ++i; } level[i]='\0';
        quiz=qv; assign=av; mid=mv; end=ev;
        computeTotal();
    }

    // compute total from components
    void computeTotal() { total = quiz + assign + mid + end; }

    // getters
    const char* getRoll() const { return roll; }
    const char* getName() const { return name; }
    double getQuiz() const { return quiz; }
    double getAssign() const { return assign; }
    double getMid() const { return mid; }
    double getEnd() const { return end; }
    double getTotal() const { return total; }

    // display
    void display() const {
        sys_write_str(roll); sys_write_str(" | ");
        sys_write_str(name); sys_write_str(" | ");
        sys_write_str(branch); sys_write_str(" | ");
        sys_write_str(level); sys_write_str(" | Quiz:");
        char buf[32]; sys_dtoa2(quiz, buf); sys_write_str(buf);
        sys_write_str(" Assign:"); sys_dtoa2(assign, buf); sys_write_str(buf);
        sys_write_str(" Mid:"); sys_dtoa2(mid, buf); sys_write_str(buf);
        sys_write_str(" End:"); sys_dtoa2(end, buf); sys_write_str(buf);
        sys_write_str(" Total:"); sys_dtoa2(total, buf); sys_write_str(buf);
        sys_write_str("\n");
    }

    // build CSV line into provided buffer (bufsize must be large)
    void toCSVline(char* outbuf, int buflen) const {
        // roll,name,branch,level,quiz,assign,mid,end,total
        long pos = 0;
        long i = 0;
        while (roll[i] && pos+1 < buflen) outbuf[pos++] = roll[i++];
        if (pos+1 < buflen) outbuf[pos++] = ',';
        i = 0;
        while (name[i] && pos+1 < buflen) outbuf[pos++] = name[i++];
        if (pos+1 < buflen) outbuf[pos++] = ',';
        i = 0;
        while (branch[i] && pos+1 < buflen) outbuf[pos++] = branch[i++];
        if (pos+1 < buflen) outbuf[pos++] = ',';
        i = 0;
        while (level[i] && pos+1 < buflen) outbuf[pos++] = level[i++];
        if (pos+1 < buflen) outbuf[pos++] = ',';
        // numeric fields as two-decimal strings
        char num[64];
        sys_dtoa2(quiz, num); long k = 0; while (num[k] && pos+1 < buflen) outbuf[pos++] = num[k++];
        if (pos+1 < buflen) outbuf[pos++] = ',';
        sys_dtoa2(assign, num); k = 0; while (num[k] && pos+1 < buflen) outbuf[pos++] = num[k++];
        if (pos+1 < buflen) outbuf[pos++] = ',';
        sys_dtoa2(mid, num); k = 0; while (num[k] && pos+1 < buflen) outbuf[pos++] = num[k++];
        if (pos+1 < buflen) outbuf[pos++] = ',';
        sys_dtoa2(end, num); k = 0; while (num[k] && pos+1 < buflen) outbuf[pos++] = num[k++];
        if (pos+1 < buflen) outbuf[pos++] = ',';
        sys_dtoa2(total, num); k = 0; while (num[k] && pos+1 < buflen) outbuf[pos++] = num[k++];
        if (pos < buflen) outbuf[pos] = '\0'; else outbuf[buflen-1] = '\0';
    }

    // input validation (simple)
    static void validateInput(const char* r, const char* n) {
        long lenr = 0; while (r[lenr]) ++lenr;
        if (lenr >= MAX_ROLL) throw BufferOverflowException("Roll buffer overflow");

        long lenn = 0; while (n[lenn]) ++lenn;
        if (lenn >= MAX_NAME) throw BufferOverflowException("Name buffer overflow");

        bool hasSpace = false;
        for (long i=0; i<lenn; ++i)
            if (n[i] == ' ') { hasSpace = true; break; }
        if (!hasSpace) throw MissingSecondNameException();

        for (long i=0; i<lenn; ++i) {
            char c = n[i];
            if (c==' ') continue;
            if (!((c>='A' && c<='Z') || (c>='a' && c<='z')))
                throw InvalidCharInNameException();
        }

        for (long i=0; r[i]; ++i) {
            char c = r[i];
            if (!((c>='0' && c<='9') || (c>='A' && c<='Z') || (c>='a' && c<='z')))
                throw InvalidCharInRollException();
        }
    }

    static void validateMarks(double q, double a, double m, double e) {
        if (q > 100.0 || a > 100.0 || m > 100.0 || e > 100.0)
            throw MarksOverflowException();
        if (q < 0.0 || a < 0.0 || m < 0.0 || e < 0.0)
            throw NegativeMarksException();
    }
};

#endif // STUDENT_SYS_H
