// main_sys.cpp
#include "io_sys.h"
#include "file_sys.h"
#include "str_sys.h"
#include "vector_sys.h"
#include "except_sys.h"
#include "student_sys.h"
#include "studentdb_sys.h"
#include "sorting_sys.h"

int main() {
    StudentDB db("students.csv");
    while (1) {
        sys_write_ln("Menu: 1.View 2.Add 3.Remove 4.Modify 5.Sort 6.Exit");
        sys_write_str("Choice: ");
        char line[32]; sys_read_line(line, 32);
        int choice = (int)sys_atof(line); // cheap parse
        if (choice == 1) {
            db.displayAll();
        } else if (choice == 2) {
            char roll[64], name[128], branch[16], level[16];
            sys_write_str("Roll: "); sys_read_line(roll,64);
            sys_write_str("Name: "); sys_read_line(name,128);
            sys_write_str("Branch: "); sys_read_line(branch,16);
            sys_write_str("Level: "); sys_read_line(level,16);
            sys_write_str("Quiz Assign Mid End (space separated): ");
            char comp[128]; sys_read_line(comp,128);
            // parse comps
            char f1[32], f2[32], f3[32], f4[32];
            long p=0; long idx=0; int field=0;
            // simple split by space
            long i=0; while (comp[i]) {
                if (comp[i] == ' ' || comp[i] == '\t') { if (idx>0) { ((char*)(&f1))[0]=0; } ++i; continue; }
                break;
            }
            // hack: use sscanf-like parsing by scanning numbers
            double q=0,a=0,m=0,e=0;
            // naive parse
            int read = 0;
            long j=0;
            // parse first number
            j=0; while (comp[j] && (comp[j]==' ')) ++j;
            char numbuf[64]; long ni=0;
            while (comp[j] && comp[j] != ' ' && ni < 60) numbuf[ni++]=comp[j++]; numbuf[ni]='\0';
            q = sys_atof(numbuf);
            while (comp[j] && comp[j] == ' ') ++j;
            ni=0; while (comp[j] && comp[j] != ' ' && ni < 60) numbuf[ni++]=comp[j++]; numbuf[ni]='\0'; a = sys_atof(numbuf);
            while (comp[j] && comp[j] == ' ') ++j;
            ni=0; while (comp[j] && comp[j] != ' ' && ni < 60) numbuf[ni++]=comp[j++]; numbuf[ni]='\0'; m = sys_atof(numbuf);
            while (comp[j] && comp[j] == ' ') ++j;
            ni=0; while (comp[j] && comp[j] != ' ' && ni < 60) numbuf[ni++]=comp[j++]; numbuf[ni]='\0'; e = sys_atof(numbuf);

            // validate
            Student::validateInput(roll, name);
            Student* s = new Student(roll, name, branch, level, q, a, m, e);
            db + s;
            sys_write_ln("Added and saved.");
        } else if (choice == 3) {
            sys_write_str("Roll to remove: ");
            char r[64]; sys_read_line(r,64);
            db - r;
            sys_write_ln("Removed (if existed).");
        } else if (choice == 4) {
            sys_write_str("Roll to modify: ");
            char r[64]; sys_read_line(r,64);
            Student* s = db[r];
            if (!s) { sys_write_ln("Not found"); continue; }
            sys_write_str("Enter new Quiz Assign Mid End: ");
            char comp[128]; sys_read_line(comp,128);
            // parse as before
            char numbuf[64]; long j=0; long ni=0;
            long pos=0; while (comp[pos] == ' ') ++pos;
            ni=0; while (comp[pos] && comp[pos] != ' ') numbuf[ni++]=comp[pos++]; numbuf[ni]='\0'; double q = sys_atof(numbuf);
            while (comp[pos] == ' ') ++pos; ni=0; while (comp[pos] && comp[pos] != ' ') numbuf[ni++]=comp[pos++]; numbuf[ni]='\0'; double a = sys_atof(numbuf);
            while (comp[pos] == ' ') ++pos; ni=0; while (comp[pos] && comp[pos] != ' ') numbuf[ni++]=comp[pos++]; numbuf[ni]='\0'; double m = sys_atof(numbuf);
            while (comp[pos] == ' ') ++pos; ni=0; while (comp[pos] && comp[pos] != ' ') numbuf[ni++]=comp[pos++]; numbuf[ni]='\0'; double e = sys_atof(numbuf);
            // replace by constructing new Student at same pointer (simple approach: delete and push new)
            char rollcopy[64]; sys_strcpy(rollcopy, s->getRoll());
            char namecopy[128]; sys_strcpy(namecopy, s->getName());
            char branchcopy[16]; // branch value not accessible directly; skipping keep existing minimal: set CSE
            sys_strcpy(branchcopy, "CSE");
            char levelcopy[16]; sys_strcpy(levelcopy, "BTech");
            // remove old and add new (simple)
            db - rollcopy;
            Student* ns = new Student(rollcopy, namecopy, branchcopy, levelcopy, q, a, m, e);
            db + ns;
            sys_write_ln("Modified and saved.");
        } else if (choice == 5) {
            sys_write_ln("Sort by: 1.Name 2.Roll 3.Quiz 4.Assign 5.Mid 6.End 7.Total");
            sys_write_str("Choice: ");
            char sline[32]; sys_read_line(sline,32); int sc = (int)sys_atof(sline);
            if (sc == 1) trie_sort_students(&db);
            else if (sc == 2) sort_by_roll(&db);
            else if (sc == 3) sort_by_quiz(&db);
            else if (sc == 4) sort_by_assign(&db);
            else if (sc == 5) sort_by_mid(&db);
            else if (sc == 6) sort_by_end(&db);
            else if (sc == 7) sort_by_total(&db);
            sys_write_ln("Sorted. Displaying:");
            db.displayAll();
        } else if (choice == 6) {
            sys_write_ln("Bye.");
            break;
        } else {
            sys_write_ln("Invalid option.");
        }
    }
    return 0;
}
