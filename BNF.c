#include "BNF_functions.h"

int main()
{
    //open files
    FILE *opTerminalFilePtr = fopen("op", "r");
    FILE *pre_opTerminalFilePtr = fopen("pre_op", "r");
    FILE *rel_opTerminalFilePtr = fopen("rel_op", "r");
    FILE *set_opTerminalFilePtr = fopen("set_op", "r");
    FILE *varTerminalFilePtr = fopen("var", "r");

    //check file integrity
    if (opTerminalFilePtr == NULL ||
        pre_opTerminalFilePtr == NULL ||
        rel_opTerminalFilePtr == NULL ||
        set_opTerminalFilePtr == NULL ||
        varTerminalFilePtr == NULL)
    {
        printf("At least a file is missing!\n");
        return 1;
    }

    // array to hold files
    FILE **files = malloc(5 * sizeof(FILE *));
    files[0] = opTerminalFilePtr;
    files[1] = pre_opTerminalFilePtr;
    files[2] = rel_opTerminalFilePtr;
    files[3] = set_opTerminalFilePtr;
    files[4] = varTerminalFilePtr;

    struct twoChildren *root;

    //cond node
    struct twoChildren cond;
    strcpy(cond.type, "cond");

    struct threeChildren condFirstChild;
    strcpy(condFirstChild.type, "condFirstChild");
    cond.childOne = &condFirstChild;

    struct threeChildren condSecondChild;
    strcpy(condSecondChild.type, "condSecondChild");
    cond.childTwo = &condSecondChild;

    //expr node
    struct threeChildren expr;
    strcpy(expr.type, "expr");

    struct threeChildren exprFirstChild;
    strcpy(exprFirstChild.type, "exprFirstChild");
    expr.childOne = &exprFirstChild;

    struct twoChildren exprSecondChild;
    strcpy(exprSecondChild.type, "exprSecondChild");
    expr.childTwo = &exprSecondChild;

    struct oneChild exprThirdChild;
    strcpy(exprThirdChild.type, "exprThirdChild");
    expr.childThree = &exprThirdChild;

    //cond first possibility
    condFirstChild.childOne = &cond;
    condFirstChild.childTwo = set_opTerminalFilePtr;
    condFirstChild.childThree = &cond;

    //cond second possibility
    condSecondChild.childOne = &expr;
    condSecondChild.childTwo = rel_opTerminalFilePtr;
    condSecondChild.childThree = &expr;

    //expr first possibility
    exprFirstChild.childOne = &expr;
    exprFirstChild.childTwo = &opTerminalFilePtr;
    exprFirstChild.childThree = &expr;

    //expr second possibility
    exprSecondChild.childOne = pre_opTerminalFilePtr;
    exprSecondChild.childTwo = &expr;

    //expr third possibility
    exprThirdChild.childOne = varTerminalFilePtr;

    srand(time(0));

    root = &cond;

    // print algorithm
    printf("if (");
    printAlgo(root, 0, -1, files);
    printf(") { }\n");

    // close file pointers
    fclose(opTerminalFilePtr);
    fclose(pre_opTerminalFilePtr);
    fclose(rel_opTerminalFilePtr);
    fclose(set_opTerminalFilePtr);
    fclose(varTerminalFilePtr);

    //empty memories
    free(files);
}