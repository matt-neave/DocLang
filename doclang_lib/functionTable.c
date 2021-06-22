#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "typedef.h"
#include "functionTable.h"
#include "functions.h"
#include "lexer.h"
#include "utils.h"

int size = 0;

void insert(char *funcName, func *function) {

    func *n;
    n = search(funcName); //Checks if var name already exists
    if (n) {
        printf("The label already exists. Duplicate cant be inserted\n");
        return;
    } else {
        Table *tab;
        tab = malloc(sizeof(Table));
        tab->funcName = funcName;
        tab->function = function;
        tab->next = NULL;
        if (size == 0) {
            first = tab;
            last = tab;
        } else {
            last->next = tab;
            last = tab;
        }
        size++;
    }

}

void del(char lab[]) {
    Table *fst, *lst;
    fst = first;
    if (!search(lab)) {
        printf("Variable not found\n");
        return;
    } else {
        if (strcmp(first->funcName, lab) == 0) {
            first = first->next;
        } else if (strcmp(last->funcName, lab) == 0) {
            lst = fst->next;
            while (strcmp(lst->funcName, lab) != 0) {
                fst = fst->next;
                lst = lst->next;
            }
            fst->next = NULL;
            last = fst;
        } else {
            lst = fst->next;
            while (strcmp(lst->funcName, lab) != 0) {
                fst = fst->next;
                lst = lst->next;
            }
            fst->next = lst->next;
        }
        size--;
    }
}

func *search(char lab[]) {
    int i;
    Table *tab;
    tab = first;
    for (i = 0; i < size; i++) {
        if (strcmp(tab->funcName, lab) == 0) {
            return tab->function;
        }
        tab = tab->next;
    }
    return NULL;
}

/* Print out Function Table for Debugging */
void display() {
    int i;
    Table *tab;
    tab = first;
    printf("Function Table:\n");
    for (i = 0; i < size; i++) {
        printFunction(tab->function);
        tab = tab->next;
    }
}

void freeVariable(Variable *variable) {
    if (!variable->isNumber) {
        free(variable->stringVal);
    } else {
        freeAssignment(variable->numVal);
    }
    free(variable);
}

void freeNumber(Number *num) {    
    if (!num->constant) {
        free(num->var);
    }
    free(num);
}

void freeAssignment(Assignment *ass) {
    if (ass->n1) freeNumber(ass->n1);
    if (ass->n2) freeNumber(ass->n2);
    free(ass);
}

void freeFunctionTable() {
    Table *curr;
    curr = first;
    while (curr != NULL) {
        del(curr->funcName);
        freeFunc(curr->function);
        Table *temp = curr;
        curr = curr->next;
        free(temp);
    }
}