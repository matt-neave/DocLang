#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#include "utils.h"
#include "functions.h"
#include "functionTable.h"
#include "instrlist.h"

instrList *initInstrList() {
    return calloc(1, sizeof(instrList));
}



void freeNode(instrNode *node) {
    if (node -> instruction){
        freeInstr(node->instruction);
    }
}

void freeList(instrList *list) {
    instrNode *ptr = list->first;
    while (ptr != list->last) {
        freeNode(ptr);
        instrNode *temp = ptr;
        ptr = ptr->next;
        free(temp);
    }
    freeNode(ptr);
    free(ptr);
    free(list);
}

void freeInstr(Instr *ins) {    
    if (ins) {
        if (ins->type == ASSIGNMENT) {
            free(ins->assign.var_name);
            freeVariable(ins->assign.variable);
        }
        else if (ins -> type == FUNCTION){
            free(ins -> caller -> funcName);
            for (int i = 0; i < ins -> caller -> argc; i++){
                free(ins -> caller -> args[i]);
            }
            free(ins -> caller->args);
            free(ins -> caller -> returnVar);
            free(ins ->caller);
        }
        else if (ins -> type == CONDITIONAL) {            
            free(ins -> condition -> conditionName);
            freeAssignment(ins -> condition -> condition);
            freeList(ins -> condition ->ifInstr);
            freeList(ins -> condition ->elseInstr);
            free (ins -> condition);
        } else if (ins -> type == RETURN) {
            free(ins -> returnCall -> var_name);
            free(ins -> returnCall);
        }
        else {

        }
    }    
    free(ins);
}

instrNode *initInstrNode(Instr *instruction, bool returnNode) {
    instrNode *newNode = calloc(1, sizeof(instrNode));
    newNode->instruction = instruction;
    newNode->returnNode = returnNode;
    newNode->next = NULL;
    return newNode;
}

void add_to_instrList(instrList *list, Instr *instruction) {
    bool returnNode = (instruction->type == RETURN);
    instrNode *newNode = initInstrNode(instruction, returnNode);
    if (list->first == NULL) {
        list->first = newNode;
        list->last = list->first;
    } else {
        instrNode *curr = list->last;
        curr->next = newNode;
        list->last = list->last->next;
    }
}

void printInstruction(Instr *instruction) {
    if (instruction->type == ASSIGNMENT) {
            printf("Var name: ");
            printf("%s\n", instruction->assign.var_name);
            printVar(instruction->assign.variable);
        } else if (instruction->type == FUNCTION) {
            printCaller(instruction->caller);
        } else if (instruction->type == CONDITIONAL) {
            printf("Conditional Instruction: \n");
            printf("\tCond: ");
            printAssignment(instruction->condition->condition);
            printf("If Branch\n");
            printInstrList(instruction->condition->ifInstr);
            printf("Else Branch\n");
            printInstrList(instruction->condition->elseInstr);
    }
}

void printInstrList(instrList *list) {
    printf("start\n");
    instrNode *curr = list->first;
    while (curr != NULL) {
        printInstruction(curr->instruction);
        curr = curr->next;
    }
}