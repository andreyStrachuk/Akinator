#include "tree.h"
#include "Akinator.h"

int DumpTree (TreeNode *node, FILE *treeDump) {
    assert (node);

    fprintf (treeDump, "digraph D {\n");

    fprintf (treeDump, "    %lu [fillcolor=purple, style=\"rounded,filled\", label=\"%s\"];\n\n", (unsigned long)node % 65536, node->data);

    int res = PreOrdSearch (node, node->left, treeDump);
    ASSERT_OK(res != OK, {PrintErrors (res); return res;})
    res = PreOrdSearch (node, node->right, treeDump);
    ASSERT_OK(res != OK, {PrintErrors (res); return res;})
    
    fprintf (treeDump, "}");

    fclose (treeDump);

    system ("dot -Tpng DumpTree.txt -O\n"
            "xdg-open DumpTree.txt.png\n");

    return OK;
}

int PreOrdSearch (TreeNode *node, TreeNode *childNode, FILE *file) {
    assert (node);
    assert (childNode);

    fprintf (file, "    %lu -> %lu;\n\n", (unsigned long)node % 65536, (unsigned long)childNode % 65536);

    if (childNode->left == nullptr || childNode->right == nullptr) {
        fprintf (file, "    %lu [fillcolor=yellow, style=\"rounded,filled\", label=\"%s\"];\n", (unsigned long)childNode % 65536, childNode->data);

        return OK;
    }

    fprintf (file, "    %lu [fillcolor=cyan, style=\"rounded,filled\", label=\"%s\"];\n", (unsigned long)childNode % 65536, childNode->data);

    int res = 0;

    res = PreOrdSearch (childNode, childNode->left, file);
    ASSERT_OK(res != OK, {PrintErrors (res); return res;})

    res = PreOrdSearch (childNode, childNode->right, file);
    ASSERT_OK(res != OK, {PrintErrors (res); return res;})

    return OK;
}

int PrintTreeToFile (TreeNode *node, FILE *file) {
    assert (node);
    assert (file);

    int res = 0;

    if (node->left == nullptr) {
        fprintf (file, "{ %s#\n}\n", node->data);

        return OK;
    }

    fprintf (file, "{ %s?\n", node->data);

    res = PrintTreeToFile (node->left, file);
    ASSERT_OK(res != OK, {PrintErrors (res); return res;})

    res = PrintTreeToFile (node->right, file);
    ASSERT_OK(res != OK, {PrintErrors (res); return res;})

    fprintf (file, "}\n");

    return OK;
}

void DestructTree (TreeNode *topNode) {
    assert (topNode);

    if (topNode->left == nullptr) {
        free (topNode->data);

        return;
    }

    free (topNode->data);

    DestructTree (topNode->left);

    free (topNode->left);

    DestructTree (topNode->right);

    free (topNode->right);

}