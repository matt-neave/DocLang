#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef TYPEDEF
#define TYPEDEF

#include "typedef.h"

#endif

#include "error.h"
#include "instrlist.h"
#include "lexer.h"
#include "functionTable.h"
#include "functions.h"
#include "utils.h"

void freeFunc(func *fun) {
    free(fun->funcName);

    for (int i = 0; i < fun->argc; i++) {
        freeInstr(fun->args[i]);
    }

    free(fun->args);  

    freeList(fun->list);
    free(fun);

}

func *createFunction(char *funcName, DataType returnType) {
    func *newFunction = calloc(1, sizeof(func));

    if (newFunction == NULL) {
        perror("calloc createFunc");
        exit(EXIT_FAILURE);
    }

    newFunction->funcName = funcName;
    newFunction->retType = returnType;
    newFunction->list = initInstrList();
    newFunction->args = calloc(1, sizeof(Instr *));
    newFunction->argc = 0;
    return newFunction;
}

void addArgs(func *function, char *arg_name, bool isNum) {
    Instr *newArgInstr = calloc(1, sizeof(Instr));
    Variable *newVariable = calloc(1, sizeof(Variable));
    Assignment *newAssignment = calloc(1, sizeof(Assignment));
    Number *newNum = calloc(1, sizeof(Number));

    if (newArgInstr == NULL || newVariable == NULL
        || newAssignment == NULL || newNum == NULL) {
        perror("addArgs calloc");
        exit(EXIT_FAILURE);
    }

    // Allocate n1
    newNum->constant = true;
    newNum->value = 0;

    // Allocate new Assignment
    newAssignment->op = NOOP;
    newAssignment->n1 = newNum;

    // Allocate Variable
    newVariable->isNumber = isNum;
    newVariable->numVal = newAssignment;

    // Allocate to Instr
    newArgInstr->type = ASSIGNMENT;
    newArgInstr->assign.variable = newVariable;
    newArgInstr->assign.var_name = arg_name;

    // Allocate to function Args
    function->args[function->argc] = newArgInstr;
    function->argc++;
    function->args = realloc(function->args, (function->argc + 1) * sizeof(Instr *));

}

void checkFunction(func *function) {

}

funcCall *createCaller(char *funcName) {
    funcCall *caller = calloc(1, sizeof(funcCall));
    caller->funcName = funcName;
    caller->argc = 0;
    caller->args = calloc(1, sizeof(char *));
    return caller;
}

void callerArgs(funcCall *caller, char *args) {
    caller->args[caller->argc] = args;
    caller->argc++;
    caller->args = realloc(caller->args, (caller->argc + 1) * sizeof(char *));
}

func *callFunction(funcCall *caller) {
    func *function = search(caller->funcName);

    if (function->argc != caller->argc) {
        outputError(ARGUMENT_COUNT, lineNum);
    }

    for (int i = 0; i < function->argc; i++) {
        Assignment *newAssignment = evaluateAssignment(caller->args[i]);
        freeAssignment(function->args[i]->assign.variable->numVal);
        function->args[i]->assign.variable->numVal = newAssignment;
    }
    return function;
}

void printFunction(func *function) {
    printf("Function Name: %s\n", function->funcName);
    printf("Args:");
    for (int i = 0; i < function->argc; i++) {
        printf("\t%s", function->args[i]->assign.var_name);
    }
    instrNode *curr = function->list->first;
    printf("\nInstructions: \n");
    int i = 1;
    while (curr != NULL) {
        printf("No %d - \t", i);
        if (curr->returnNode) {
            printf("Return %s\n", curr->instruction->returnCall->var_name);
        } else if (curr->instruction->type == ASSIGNMENT) {
            printf("%s:\n", curr->instruction->assign.var_name);
            printVar(curr->instruction->assign.variable);
        } else if (curr->instruction->type == CONDITIONAL) {
            printf("Conditional Function\n");
            // printf("Conditional Instruction: \n");
            printf("\tCond: ");
            printAssignment(curr->instruction->condition->condition);
            printf("If Branch\n");
            printInstrList(curr->instruction->condition->ifInstr);
            printf("Else Branch\n");
            printInstrList(curr->instruction->condition->elseInstr);
        } else {
            printf("Call Function: %s\n", curr->instruction->caller->funcName);
        }
        curr = curr->next;
        i++;
    }
}

void printCaller(funcCall *caller) {
    printf("Calling Function: %s\n", caller->funcName);
    for (int i = 0; i < caller->argc; i++) {
        printf("Arg %d : %s\n", i, caller->args[i]);
    }
    printf("Return Var: %s\n", caller->returnVar);
}