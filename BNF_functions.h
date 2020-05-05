#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct oneChild
{
    char type[10];
    void *childOne;
};

struct twoChildren
{
    char type[10];
    void *childOne;
    void *childTwo;
};

struct threeChildren
{
    char type[10];
    void *childOne;
    void *childTwo;
    void *childThree;
};

int randChoice(int ancestor)
{
    switch (ancestor)
    {
    case 0: //cond
        return rand() % 2;
        break;
    case 1: //expr
        return rand() % 3;
        break;

    default:
        return -1;
        break;
    }
}

int findLineCount(FILE *file)
{
    int lineCount = 0;
    char buffer[40], charTemp;

    charTemp = getc(file);
    while (charTemp != EOF)
    {
        if (charTemp == '\n')
        {
            lineCount = lineCount + 1;
        }
        charTemp = getc(file);
    }
    return lineCount;
}

char *selectFromTerminal(FILE *file, int selection)
{
    char *str = malloc(10 * sizeof(char));

    for (size_t i = 0; i < selection; i++)
    {
        fgets(str, 10, file);
    }

    fseek(file, 0, SEEK_SET);

    //to strip the '\n'
    str[strlen(str) - 1] = 0;
    return str;
}

char *randTerminalSelector(FILE *file)
{
    int lineCount = findLineCount(file);
    fseek(file, 0, SEEK_SET);

    int randSelection = (rand() % (lineCount - 1 + 1)) + 1; //between 1-lineCount

    return selectFromTerminal(file, randSelection);
}

void printAlgo(void *root, int ancestor, int chosen, FILE **files)
{
    int randNum;
    char *terminal;

    //terminal symbols
    if (ancestor == -1 && chosen == -1) //op
    {
        terminal = randTerminalSelector(files[0]);
        printf("%s", terminal);
        free(terminal);
    }
    else if (ancestor == -1 && chosen == -2) //pre_op
    {
        terminal = randTerminalSelector(files[1]);
        printf("%s", terminal);
        free(terminal);
    }
    else if (ancestor == -1 && chosen == -3) //rel_op
    {
        terminal = randTerminalSelector(files[2]);
        printf("%s", terminal);
        free(terminal);
    }
    else if (ancestor == -1 && chosen == -4) //set_op
    {
        terminal = randTerminalSelector(files[3]);
        printf("%s", terminal);
        free(terminal);
    }
    else if (ancestor == 1 && chosen == 2) //var
    {
        terminal = randTerminalSelector(files[4]);
        printf("%s", terminal);
        free(terminal);
    }

    //main nodes (cond or expr)
    if (chosen == -1 && ancestor != -1)
    {
        switch (ancestor)
        {
        case 0: //cond
            randNum = randChoice(ancestor);
            if (randNum == 0) //first child of cond
            {
                (printAlgo(
                    ((struct twoChildren *)root)->childOne, 0, 0, files));
            }
            else if (randNum == 1) //second child of cond
            {
                printAlgo(
                    ((struct twoChildren *)root)->childTwo, 0, 1, files);
            }
            break;

        case 1: //expr
            randNum = randChoice(ancestor);
            if (randNum == 0) //first child of expr
            {
                printAlgo(
                    ((struct threeChildren *)root)->childOne, 1, 0, files);
            }
            else if (randNum == 1) //second child of expr
            {
                printAlgo(
                    ((struct threeChildren *)root)->childTwo, 1, 1, files);
            }
            else if (randNum == 2) //third child of expr
            {
                printAlgo(
                    ((struct threeChildren *)root)->childThree, 1, 2, files);
            }
            break;

        default:
            printf("Wrong ancestor number {{{{%d}}}}}!\n", ancestor);
            break;
        } //end of ancestor switch case
    }     //end of main node if case
    else  //if input isn't a main node
    {
        struct twoChildren cond;
        strcpy(cond.type, "cond");

        struct threeChildren expr;
        strcpy(expr.type, "expr");

        switch (ancestor)
        {
        case 0:              //cond
            if (chosen == 0) //cond first child
            {
                printf("(");
                printAlgo(&cond, 0, -1, files);
                printAlgo(&cond, -1, -4, files); //set_op
                printAlgo(&cond, 0, -1, files);
                printf(")");
            }
            else if (chosen == 1) //cond second child
            {
                printf("(");
                printAlgo(&expr, 1, -1, files);
                printAlgo(&cond, -1, -3, files); //rel_op
                printAlgo(&expr, 1, -1, files);
                printf(")");
            }
            break;
        case 1:              //expr
            if (chosen == 0) //expr first child
            {
                printf("(");
                printAlgo(&expr, 1, -1, files);
                printAlgo(&cond, -1, -1, files); //op
                printAlgo(&expr, 1, -1, files);
                printf(")");
            }
            else if (chosen == 1) //expr second child
            {
                printAlgo(&cond, -1, -2, files); //pre_op
                printf("(");
                printAlgo(&expr, 1, -1, files);
                printf(")");
            }
            // chosen == 2 is handled in terminal files part
            break;
        default:
            if (ancestor != -1)
            {
                printf("Wrong ancestor number\n");
            }
            break;
        }
    }
}