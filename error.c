#include <stdio.h>
#include <stdlib.h>
#include "typedef.h"
#include "error.h"

#define UNKNOWN -1

/* Outputs appropriate error handling messages */
void outputError(ERROR error, int lineNum) {
    lineNum == UNKNOWN ? printf("Error on line UNKNOWN!")
    : printf("Error on line %d.\n", lineNum);
    switch (error) {
        case LINE_ENDING:
            printf("Syntax Error: Please add a fullstop after each line!\n");
            break;
        case SYNTAX:
            printf("Syntax Error: Unidentified Syntax Used!\n");
            break;
        case TYPE_ERROR:
            printf("Type Error: Unsupported operand type!\n");
            break;
        case NAME_ERROR:
            printf("Name Error: Undefined variable given\n");
            break;
        case MISSING_RETURN:
            printf("Syntax Error: All functions must exit with 'OH BABY!'!\n");
            break;
        case ARGUMENT_COUNT:
            printf("Argc Error: Number of arguments do not match!\n");
            break;
    }
    printf("Dont worry! Its Microsoft's fault!\n");
    exit(EXIT_FAILURE);
}