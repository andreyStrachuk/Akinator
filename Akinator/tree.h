#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct TreeNode {
    char *data; // void universal

    TreeNode *left;
    TreeNode *right;
};

enum ERRORS {
    OK = -5,
    UNABLETOOPENFILE,
    INCORRECT_INPUT,
};

int DumpTree (TreeNode *node, FILE *treeDump);

int PreOrdSearch (TreeNode *node, TreeNode *childNode, FILE *file);

int PrintTreeToFile (TreeNode *node, FILE *file);

void DestructTree (TreeNode *topNode);

#endif