#include "Akinator.h"
#include "tree.h"

int PlantTree (char **base, TreeNode *topNode) {
    assert (base);
    assert (topNode);

    int res = 0;

    *base = SkipSpaceSymbols (*base);
    ASSERT_OK (**base != '{' && **base != '}', return INCORRECT_INPUT;)

    (*base)++;
    *base = SkipSpaceSymbols (*base);

    char trait [100] = {};
    int wordLength = 0;

    while (**base != '?' && **base != '#') {
        trait [wordLength] = **base;
        wordLength++;
        (*base)++;
    }

    (*base)++;
    trait[wordLength] = '\0';

    topNode->data = (char *)calloc (wordLength + 1, sizeof (char));
    strncpy (topNode->data, trait, wordLength + 1);

    *base = SkipSpaceSymbols (*base);

    if (**base == '{') {
        topNode->left = (TreeNode *)calloc (1, sizeof (TreeNode));

        res = PlantTree (base, topNode->left);
        ASSERT_OK (res != OK, return INCORRECT_INPUT;)

        topNode->right = (TreeNode *)calloc (1, sizeof (TreeNode));

        res = PlantTree (base, topNode->right);
        ASSERT_OK (res != OK, return INCORRECT_INPUT;)

    }

    if (**base == '}') {
        (*base)++;
        *base = SkipSpaceSymbols (*base);

        return OK;
    }

    return INCORRECT_INPUT;
}

int Guesser (TreeNode *node) {
    assert (node);

    if (node->left == nullptr) {
        printf ("I think it's %s\nAm I right?(Y/N)\n", node->data);

        while (1) {
            int symbol = getchar ();

            while (getchar () != '\n') ;

            if ((char)symbol == 'Y') {
                printf ("Ha-ha-ha!\n");

                return GUESSED;
            }
            else if ((char)symbol == 'N') {
                printf ("Please, enter the object you conceived.\n");
                char obj[50] = {};

                int objSize = GetString (obj);

                node->left = (TreeNode *)calloc (1, sizeof (TreeNode));
                node->right = (TreeNode *)calloc (1, sizeof (TreeNode));

                node->left->data = (char *)calloc (objSize, sizeof (char));
                strncpy (node->left->data, obj, objSize);

                printf ("Please, enter the trait which distinguishes %s from %s.\n", obj, node->data);

                char trait [50] = {};

                int size = GetString (trait);

                node->right->data = (char *)calloc (size, sizeof (char));
                strncpy (node->right->data, node->data, size);

                node->data = (char *)realloc (node->data, sizeof (char) * size);
                strncpy (node->data, trait, size);

                return NOTGUESSED;

            }
            else {
                printf ("Please, enter Y or N!\n");

                continue;
            }
        }
    }
    else {
        printf ("Is it %s?(Y/N)\n", node->data);

        int symbol = getchar ();

        while (getchar () != '\n') ;

        int res = 0;

        if ((char)symbol == 'Y') {
            res = Guesser (node->left);
            ASSERT_OK(res != OK, {PrintErrors (res); return res;})
        }
        else if ((char)symbol == 'N') {
            res = Guesser (node->right);
            ASSERT_OK(res != OK, {PrintErrors (res); return res;})
        }

    }

    return OK;
}

void PrintErrors (const int errNumber) {
    switch (errNumber)
    {
        case UNABLETOOPENFILE: {
            printf ("Unable to open file!\nError code: %d\n", errNumber);
            break;
        }
        case INCORRECT_INPUT: {
            printf ("Incorrect input data!\nError code: %d\n", errNumber);
            break;
        }
        case NOTFOUND: {
            printf ("Cannot find element!\nError code: %d\n", errNumber);
            break;
        }

        default:
            break;
    }
}

int GetFileSize (FILE *txtFile) {
    assert (txtFile);

    int numberOfSymbols = 0;

    fseek (txtFile, 0, SEEK_END);

    numberOfSymbols = ftell (txtFile);

    fseek (txtFile, 0, SEEK_SET);

    return numberOfSymbols;
}

char *SkipSpaceSymbols (char *src) {
    assert (src);

    while (isspace (*src)) {
        src++;
    }

    return src;
}

char *SkipReadWord (char *src) {
    assert (src);

    while (*src != '\0' && *src != ' ') {
        src++;
    }

    return src;
}

int SearchForElement (TreeNode *topNode, char *str, Trait *tmpTrait) {
    assert (topNode);
    assert (str);

    int res = 0;

    if (STR_EQ (str, topNode->data)) {
        return FOUND;
    }

    int i = 0;

    while ((tmpTrait + i)->str != 0) {
        i++;
    }

    (tmpTrait + i)->str = topNode->data;
    (tmpTrait + i)->status = 1;
    

    if (topNode->left == nullptr) {
        (tmpTrait + i)->status = 0;
        (tmpTrait + i)->str = nullptr;

        return NOTFOUND;
    }

    res = SearchForElement (topNode->left, str, tmpTrait);
    ASSERT_OK (res == FOUND, return FOUND;)

    (tmpTrait + i)->status = 0;

    res = SearchForElement (topNode->right, str, tmpTrait);
    ASSERT_OK (res == FOUND, return FOUND;)

    (tmpTrait + i)->status = 0;
    (tmpTrait + i)->str = nullptr;

    return NOTFOUND;
}

int CompareTwoEls (TreeNode *node, char *str1, char *str2) {
    assert (str1);
    assert (str2);

    Trait tmpTrait1[50] = {};
    Trait tmpTrait2[50] = {}; 

    int result = SearchForElement (node, str1, tmpTrait1);
    ASSERT_OK (result == NOTFOUND, return NOTFOUND;)

    result = SearchForElement (node, str2, tmpTrait2);
    ASSERT_OK (result == NOTFOUND, return NOTFOUND;)

    int str1TraitIter = 0;
    int str2TraitIter = 0;

    printf ("%s and %s both: \n", str1, str2);

    while (1) {
        if (STR_EQ (tmpTrait1[str1TraitIter].str, tmpTrait2[str2TraitIter].str)) {
            if (tmpTrait1[str1TraitIter].status == tmpTrait2[str1TraitIter].status) {
                if (tmpTrait1[str1TraitIter].status == 0) {
                    printf ("Not %s\n", tmpTrait1[str1TraitIter].str);
                }
                if (tmpTrait1[str1TraitIter].status == 1) {
                    printf ("%s\n", tmpTrait1[str1TraitIter].str);
                }

                str1TraitIter++;
                continue;
            }
        }
        break;
    }

    if (str1TraitIter == 0) {
        printf ("none.\n");
    }

    str2TraitIter = str1TraitIter;

    ShowElCharacteristics (str1, tmpTrait1, str1TraitIter);

    ShowElCharacteristics (str2, tmpTrait2, str2TraitIter);

    return OK;
}

void ShowElCharacteristics (char *str, Trait *tmpTrait, const int num) {
    assert (str);
    assert (tmpTrait);

    printf ("%s characteristics: \n", str);

    for (int i = num; (tmpTrait + i)->str != nullptr;i++) {
        if (tmpTrait[i].status) {
            printf ("%s\n", (tmpTrait + i)->str);
        }
        else {
            printf ("not %s\n", (tmpTrait + i)->str);
        }
    }
}

int PlayAkinator (TreeNode *topNode) {
    assert (topNode);

    int mode = 0;
    int res = 0;    

    while (1) {
        while (1) {
            printf ("Please, enter the mode you'd like to use:\n1 - Akinator.\n2 - Elements trait.\n3 - Compare two elements.\n");

            scanf ("%d", &mode);

            while (getchar () != '\n') ;

            if (mode < 1 || mode > 3) {
                printf ("Error!Unknown mode!\nPlease, enter correct mode number!\n");

                system ("clear");
                continue;
            }

            break;
        }

        switch (mode) {
            case AKINATOR: {
                res = Guesser (topNode);
                
                if (res == NOTGUESSED) {
                    printf ("Do you wanna save the new base?\n");
                    int symbol = getchar ();

                    while (getchar () != '\n') ;

                    if (symbol == 'Y') {
                        printf ("Enter the file where you want to save new base: ");

                        char fileName [50] = {};
                        scanf ("%s", fileName);

                        while (getchar () != '\n') ;

                        FILE *file = fopen (fileName, "w");
                        if (file == nullptr) {
                            PrintErrors (UNABLETOOPENFILE);
                        }
                        else {
                            res = PrintTreeToFile (topNode, file);
                            ASSERT_OK(res != OK, {PrintErrors (res); return res;})
                            
                        }

                        fclose (file);
                        break;
                    }
                }

                break;
            }
            case TRAIT: {
                printf ("Enter the element you want to find: ");

                char str [50] = {};
                GetString (str);

                Trait tr[50] = {};

                res = SearchForElement (topNode, str, tr);
                ASSERT_OK(res != FOUND, {PrintErrors (res); return res;})

                ShowElCharacteristics (str, tr, 0);
                
                break;
            }
            case COMPARE: {
                printf ("Enter two elements you want to compare: \n");

                char str1[50] = {};
                scanf ("%s", str1);

                char str2[50] = {};
                scanf ("%s", str2);

                while (getchar () != '\n') ;

                int res = CompareTwoEls (topNode, str1, str2);
                ASSERT_OK(res != OK, {PrintErrors (res); return res;})

                break;
            }
            default:    {
                printf ("Unknown mode!\n");
            }
        }

        printf ("\nDo you wanna play again?(Y/N)\n");
        int symbol = getchar ();

        while (getchar() != '\n') ;

        if (symbol == 'Y') {
            system ("clear");

            continue;
        }

        if (symbol == 'N') {
            printf ("Thank you for the game!\n");
            break;
        }
    }

    return OK;
}

int GetString (char *str) {
    assert (str);

    int size = 0;

    int symbol = getchar ();

    while (symbol != '\n') {
        str[size] = symbol;
        symbol = getchar ();
        size++;
    }

    str[size] = '\0';

    return ++size;
}