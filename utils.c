#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#ifndef TYPEDEF
#define TYPEDEF

#include "typedef.h"

#endif
#ifndef FUNCTABLE
#define FUNCTABLE

#include "functionTable.h"

#endif

#include "utils.h"
#include "functions.h"
#include "error.h"

bool is_num(char *string) {
    if (!strncmp(string, "-", 1)) {
        if (atoi(string + 1) != 0) {
            return true;
        } else {
            return false;
        }
    } else {
        if (atoi(string) != 0 || !strcmp(string, "0")) {
            return true;
        }
        return false;
    }
}

int getNum(char *string) {
    if (!strncmp(string, "-", 1)) {
        return -1 * atoi(string + 1);
    } else {
        return atoi(string);
    }
}

Assignment *evaluateAssignment(char *source) {
    Operation op = getOperation(source);
    char *token;
    token = strtok(source, " ");
    Assignment *newAssignment = calloc(1, sizeof(Assignment));
    Number *n1 = calloc(1, sizeof(Number));
    if (newAssignment == NULL || n1 == NULL) {
        perror("calloc evaluateAssignment");
        exit(EXIT_FAILURE);
    }
    newAssignment->op = op;
    if (is_num(token)) {
        n1->constant = true;
        n1->value = getNum(token);
    } else {
        n1->constant = false;
        n1->var = calloc(strlen(token) + 1, sizeof(char));
        if (n1->var == NULL) {
            perror("calloc evalAssignment");
            exit(EXIT_FAILURE);
        }
        strcpy(n1->var, token);
    }
    newAssignment->n1 = n1;
    if (op != NOOP) {
        Number *n2 = calloc(1, sizeof(Number));
        if (n2 == NULL) {
            perror("calloc evaluateAssignment");
            exit(EXIT_FAILURE);
        }
        switch (op) {
            case ADD:
            case SUB:
            case TIMES:
                for (int i = 0; i < 2; i++) {
                    token = strtok(NULL, " ");
                }
                break;
            case DIVIDE:
                for (int i = 0; i < 3; i++) {
                    token = strtok(NULL, " ");
                }
                break;
            case NOT_EQUAL_TO:
            case GREATER_THAN:
            case LESS_THAN:
                for (int i = 0; i < 5; i++) {
                    token = strtok(NULL, " ");
                }
                break;
            case EQUAL_TO:
                for (int i = 0; i < 4; i++) {
                    token = strtok(NULL, " ");
                }
                break;
            case S_GREATER_THAN:
            case S_LESS_THAN:
                for (int i = 0; i < 6; i++) {
                    token = strtok(NULL, " ");
                }

                break;
            default:
                outputError(SYNTAX, -1);
                break;

        }
        if (is_num(token)) {
            if (n1->constant) {
                n1->value = evaluateOperation(n1->value, getNum(token), op);
                if (op == ADD || op == SUB || op == TIMES || op == DIVIDE) {
                    newAssignment->op = NOOP;
                }
                n2->constant = true;
                n2->value = n1->value;
                newAssignment->n2 = n2;
                return newAssignment;
            }
            n2->constant = true;
            n2->value = getNum(token);
        } else {
            n2->constant = false;
            n2->var = calloc(strlen(token) + 1, sizeof(char));
            if (n2->var == NULL) {
                perror("calloc evalAssignment");
                exit(EXIT_FAILURE);
            }
            strcpy(n2->var, token);
        }
        newAssignment->n2 = n2;
    }
    return newAssignment;
}


Operation getOperation(char *opString) {
    if (strstr(opString, "plus")) {
        return ADD;
    }
    if (strstr(opString, "minus")) {
        return SUB;

    }

    if (strstr(opString, "times")) {
        return TIMES;
    }

    if (strstr(opString, "divided by")) {
        return DIVIDE;
    }
    if (strstr(opString, "is equal to")) {
        return EQUAL_TO;
    }
    if (strstr(opString, "is not equal to")) {
        return NOT_EQUAL_TO;
    }

    if (strstr(opString, "is lower bounded by")) {
        return GREATER_THAN;
    }
    if (strstr(opString, "is upper bounded by")) {
        return LESS_THAN;
    }
    if (strstr(opString, "is strictly upper bounded by")) {
        return S_LESS_THAN;
    }
    if (strstr(opString, "is strictly lower bounded by")) {
        return S_GREATER_THAN;
    }


    return NOOP;
}

int evaluateOperation(int n1, int n2, Operation op) {
    switch (op) {
        case ADD:
            return n1 + n2;
        case TIMES:
            return n1 * n2;
        case DIVIDE:
            return n1 / n2;
        case SUB:
            return n1 - n2;
        case GREATER_THAN:
            return n1 >= n2;
        case LESS_THAN:
            return n1 <= n2;
        case EQUAL_TO:
            return n1 == n2;
        case NOT_EQUAL_TO:
            return n1 != n2;
        case S_GREATER_THAN:
            return n1 > n2;
        case S_LESS_THAN:
            return n1 < n2;
        default:
            outputError(SYNTAX, lineNum);
            return -1;
    }
}

void trimLine(char *string) {
    int index = -1;
    int i = 0;


    while (string[i] != '\0') {
        if (string[i] != ' ' && string[i] != '\t' && string[i] != '\n') {
            index = i;
        }

        i++;
    }

    /* Mark next character to last non-white space character as NULL */
    string[index + 1] = '\0';
}

void concatToken(char *destination) {
    char *token;
    token = strtok(NULL, " ");
    while (token != NULL) {
        strcat(destination, token);
        strcat(destination, " ");
        token = strtok(NULL, " ");
    }
}

char *copyToken(char *token) {
    char *destination = calloc(strlen(token) + 1, sizeof(char));
    strcpy(destination, token);
    return destination;
}

void printOperation(Operation op) {
    switch (op) {
        case GREATER_THAN:
            printf(" >= ");
            break;
        case LESS_THAN:
            printf(" <= ");
            break;
        case EQUAL_TO:
            printf(" == ");
            break;
        case NOT_EQUAL_TO:
            printf(" != ");
            break;
        case S_LESS_THAN:
            printf(" < ");
            break;
        case S_GREATER_THAN:
            printf(" > ");
            break;
        case ADD:
            printf(" + ");
            break;
        case SUB:
            printf(" - ");
            break;
        case TIMES:
            printf(" * ");
            break;
        case DIVIDE:
            printf(" / ");
            break;
        default:
            printf("Unknown Op");
            break;
    }
}

void printAssignment(Assignment *numVal) {
    if (numVal->op == NOOP) {
        printNumber(numVal->n1);
    } else {
        printNumber(numVal->n1);
        printOperation(numVal->op);
        printNumber(numVal->n2);
    }
    printf("\n");
}

void printNumber(Number *num) {
    if (num->constant) {
        printf("%d", num->value);
    } else {
        printf("%s", num->var);
    }
}

void printVar(Variable *variable) {
    if (variable->isNumber) {
        printf("Variable Type: Number\n");
        printf("Variable Assignment: ");
        printAssignment(variable->numVal);
    } else {
        printf("Variable Type: String\n");
        printf("Variable Value: %s", variable->stringVal);
    }
    printf("\n\n");
}