#include "Akinator.h"
#include "tree.h"

int main (int argc, const char *argv[]) {
    ASSERT_OK (argc < 2, {PrintErrors (UNABLETOOPENFILE); return 0;})
    FILE *baseFile = fopen (argv [1], "r");
    ASSERT_OK (baseFile == nullptr, PrintErrors (UNABLETOOPENFILE);)

    int fileSize = GetFileSize (baseFile);

    char *base = (char *)calloc (fileSize + 1, sizeof (char));

    fileSize = fread (base, sizeof (char), fileSize, baseFile);
    fclose (baseFile);

    TreeNode node = {};

    int res = PlantTree (&base, &node);
    ASSERT_OK (res != OK, {PrintErrors (INCORRECT_INPUT);})

    res = PlayAkinator (&node);
    ASSERT_OK (res != OK, {PrintErrors (res);})

    FILE *dump = fopen ("DumpTree.txt", "w");
    DumpTree (&node, dump);

    DestructTree (&node);

    base -= fileSize;

    free (base);

    return 0;
}