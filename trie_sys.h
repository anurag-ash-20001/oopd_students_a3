#ifndef TRIE_SYS_H
#define TRIE_SYS_H

#include "studentdb_sys.h"

// simple trie node with 128 ASCII children
struct TrieNode {
    TrieNode* children[128];
    SysVector holders; // to hold pointers of students ending here
    bool isEnd;
    TrieNode() { for (int i=0;i<128;++i) children[i]=nullptr; isEnd=false; vector_init(&holders,4); }
};

// helper to free trie nodes recursively (not necessary for short runs but included)
static void trie_free(TrieNode* node) {
    if (!node) return;
    for (int i=0;i<128;++i) if (node->children[i]) trie_free(node->children[i]);
    for (long i=0;i<node->holders.size;++i) ; // no deletion of students here
    vector_free(&node->holders);
    // free node via munmap not implemented; skip deallocation
}

// Build trie and traverse
static void trie_sort_students(StudentDB* db) {
    TrieNode root = {}; // initialize root
    // insert each student
    for (long i = 0; i < db->size(); ++i) {
        Student* s = db->at(i);
        TrieNode* cur = &root;
        const char* name = s->getName();
        for (long k = 0; name[k]; ++k) {
            unsigned char ch = (unsigned char)name[k];
            if (!cur->children[ch]) cur->children[ch] = new TrieNode();
            cur = cur->children[ch];
        }
        cur->isEnd = true;
        vector_push(&cur->holders, s);
    }

    // collect in-order into temp vector
    SysVector temp;
    vector_init(&temp, db->size() > 4 ? db->size() : 4);

    // recursive traversal function
    struct Recursor {
        static void go(TrieNode* node, SysVector* acc) {
            if (!node) return;
            if (node->isEnd) {
                for (long i = 0; i < node->holders.size; ++i)
                    vector_push(acc, node->holders.data[i]);
            }
            for (int i = 0; i < 128; ++i) // ascending ASCII
                if (node->children[i]) go(node->children[i], acc);
        }
    };
    Recursor::go(&root, &temp);

    // overwrite db->vec.data with temp.data
    SysVector* vec = db->getRawVector();
    for (long i = 0; i < temp.size && i < vec->size; ++i) {
        vec->data[i] = temp.data[i];
    }

    // cleanup
    vector_free(&temp);
    for (int i = 0; i < 128; ++i)
        if (root.children[i]) trie_free(root.children[i]);
}


#endif // TRIE_SYS_H
