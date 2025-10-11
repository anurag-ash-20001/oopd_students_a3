#ifndef SORTING_SYS_H
#define SORTING_SYS_H

#include "studentdb_sys.h"
#include "trie_sys.h"

// simple bubble sorts; descending
static void sort_by_roll(StudentDB* db) {
    long n = db->size();
    for (long i=0;i<n;i++) for (long j=i+1;j<n;j++) {
        if (sys_strcmp(db->at(i)->getRoll(), db->at(j)->getRoll()) < 0) db->swap_indices(i,j);
    }
}
static void sort_by_quiz(StudentDB* db) {
    long n = db->size();
    for (long i=0;i<n;i++) for (long j=i+1;j<n;j++) {
        if (db->at(i)->getQuiz() < db->at(j)->getQuiz()) db->swap_indices(i,j);
    }
}
static void sort_by_assign(StudentDB* db) {
    long n = db->size();
    for (long i=0;i<n;i++) for (long j=i+1;j<n;j++) {
        if (db->at(i)->getAssign() < db->at(j)->getAssign()) db->swap_indices(i,j);
    }
}
static void sort_by_mid(StudentDB* db) {
    long n = db->size();
    for (long i=0;i<n;i++) for (long j=i+1;j<n;j++) {
        if (db->at(i)->getMid() < db->at(j)->getMid()) db->swap_indices(i,j);
    }
}
static void sort_by_end(StudentDB* db) {
    long n = db->size();
    for (long i=0;i<n;i++) for (long j=i+1;j<n;j++) {
        if (db->at(i)->getEnd() < db->at(j)->getEnd()) db->swap_indices(i,j);
    }
}
static void sort_by_total(StudentDB* db) {
    long n = db->size();
    for (long i=0;i<n;i++) for (long j=i+1;j<n;j++) {
        if (db->at(i)->getTotal() < db->at(j)->getTotal()) db->swap_indices(i,j);
    }
}

#endif // SORTING_SYS_H
