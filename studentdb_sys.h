#ifndef STUDENTDB_SYS_H
#define STUDENTDB_SYS_H

#include "student_sys.h"
#include "file_sys.h"
#include "str_sys.h"
#include "vector_sys.h"
#include "io_sys.h"

#define MAX_LINE 512

class StudentDB {
private:
    SysVector vec;
    char filename[256];

    // split CSV by commas into fields (no quoting support)
    static int split_fields(const char* line, char fields[][128], int max_fields) {
    int fi = 0, pi = 0;
    for (int i = 0; line[i] != '\0' && fi < max_fields; i++) {
        if (line[i] == ',') {
            fields[fi][pi < 127 ? pi : 127] = '\0';  // clamp safely
            fi++;
            pi = 0;
        } else {
            if (pi < 127) fields[fi][pi++] = line[i];  // don't overflow
        }
    }
    if (fi < max_fields) fields[fi][pi < 127 ? pi : 127] = '\0';
    return fi + 1;
}


public:
    StudentDB(const char* fname) {
        vector_init(&vec, 8);
        long i=0; while (fname[i] && i+1 < 256) { filename[i] = fname[i]; ++i; } filename[i] = '\0';
        loadFromCSV();
    }
    ~StudentDB() {
        for (long i = 0; i < vec.size; ++i) delete (Student*)vec.data[i];
        vector_free(&vec);
    }

    void loadFromCSV() {
        // clear existing
        for (long i = 0; i < vec.size; ++i) delete (Student*)vec.data[i];
        vec.size = 0;
        // open file
        int fd = sys_open_file(filename, SYS_O_RDONLY, SYS_MODE_644);
        if (fd < 0) return;
        char buf[4096];
        long n = sys_read_file(fd, buf, 4095);
        if (n <= 0) { sys_close_file(fd); return; }
        buf[n] = '\0';
        // parse lines
        char* p = buf;
        while (*p) {
            // copy line
            char line[MAX_LINE]; int li = 0;
            while (*p && *p != '\n' && li+1<MAX_LINE) { line[li++] = *p++; }
            line[li] = '\0';
            if (*p == '\n') ++p;
            if (li == 0) continue;
            // split fields
            char fields[12][128];
            int fcount = split_fields(line, fields, 12);
            if (fcount < 9) continue;
            // fields: roll,name,branch,level,quiz,assign,mid,end,total
            double q = sys_atof(fields[4]); double a = sys_atof(fields[5]);
            double m = sys_atof(fields[6]); double e = sys_atof(fields[7]);
            Student* s = new Student(fields[0], fields[1], fields[2], fields[3], q, a, m, e);
            vector_push(&vec, s);
        }
        sys_close_file(fd);
    }

    void saveToCSV() {
        // open/create truncate
        int fd = sys_open_file(filename, SYS_O_WRONLY | SYS_O_CREAT | SYS_O_TRUNC, SYS_MODE_644);
        if (fd < 0) { sys_write_ln("Cannot open students.csv for write"); return; }
        // build content in buffer
        char line[512];
        for (long i = 0; i < vec.size; ++i) {
            Student* s = (Student*)vec.data[i];
            s->toCSVline(line, 512);
            long L = sys_strlen(line);
            sys_write_file(fd, line, L);
            sys_write_file(fd, "\n", 1);
        }
        sys_close_file(fd);
    }

    void displayAll() {
        for (long i = 0; i < vec.size; ++i) ((Student*)vec.data[i])->display();
    }

    // operator +
    StudentDB& operator+(Student* s) {
        vector_push(&vec, s);
        saveToCSV();
        return *this;
    }

    // operator -
    StudentDB& operator-(const char* roll) {
        for (long i = 0; i < vec.size; ++i) {
            Student* s = (Student*)vec.data[i];
            if (sys_strcmp(s->getRoll(), roll) == 0) {
                delete s;
                vector_erase_at(&vec, i);
                saveToCSV();
                break;
            }
        }
        return *this;
    }

    // operator []
    Student* operator[](const char* roll) {
        for (long i = 0; i < vec.size; ++i) {
            Student* s = (Student*)vec.data[i];
            if (sys_strcmp(s->getRoll(), roll) == 0) return s;
        }
        return nullptr;
    }

    // access to vector pointer for sorting module (raw)
    SysVector* getRawVector() { return &vec; }
    long size() { return vec.size; }
    Student* at(long idx) { return (Student*)vec.data[idx]; }
    void swap_indices(long i, long j) {
        void* t = vec.data[i]; vec.data[i] = vec.data[j]; vec.data[j] = t;
    }
};

#endif // STUDENTDB_SYS_H
