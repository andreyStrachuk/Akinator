#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "tree.h"

#define ASSERT_OK(smth, do ) if (smth) do 

#define MAXLENGTHOFNEWINFO 30

#define STR_EQ(str1, str2)      (strncmp (str1, str2, strlen (str1)) == 0)

struct Trait {
    bool status;

    char *str;
    // i want to prog every fucking day of my stupid life but i'm too stupid
    // so i want to die now
};

enum MODE{
    AKINATOR = 1,
    TRAIT,
    COMPARE,
    GUESSED,
    NOTGUESSED
};

enum TYPEOFNODE {
    LEFT,
    RIGHT
};

enum STATUS {
    FOUND,
    NOTFOUND
};

int PlantTree (char **base, TreeNode *topNode);

int CompareTwoEls (TreeNode *node, char *str1, char *str2);

int Guesser (TreeNode *node);

void PrintErrors (const int errNumber);

int GetFileSize (FILE *asmProgram);

char *SkipSpaceSymbols (char *src);

char *SkipReadWord (char *src);

int SearchForElement (TreeNode *topNode, char *str, Trait *tmpTrait);

void ShowElCharacteristics (char *str, Trait *tr, const int num);

int PlayAkinator (TreeNode *topNode);

int GetString (char *str);

#endif