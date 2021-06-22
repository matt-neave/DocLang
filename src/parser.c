#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"
#include "error.h"
#include "functionTable.h"
#include "functions.h"
#include "parser.h"

#ifndef LEXER
#define LEXER

#include "lexer.h"

#endif

#include "utils.h"

#define NUMBER_OF_REGS 13
#define NOT_FOUND -1
#define TEMP_VAR "TEMPORARY_VARIABLE"

char *regs[NUMBER_OF_REGS];
int condCount = 0;
int retCount = 0;

void parseInit(instrList *list, FILE *out) {
    char start[MAX_STRING_LENGTH] = "b main";
    output_assembly(start, out);
    parseAllFunction(out);
    char main[6] = "main:";
    output_assembly(main, out);
    parseInstrList(list, out);
}

/* Parse an instruction list */
void parseInstrList(instrList *list, FILE *out) {
    instrNode *ptr = list->first;

    while (ptr != NULL) {
        if (ptr->instruction->type == FUNCTION) {
            parseFuncCaller(ptr, out);
        } else if (ptr->instruction->type == CONDITIONAL) {
            parseConditional(ptr->instruction, out);
        } else if (ptr->instruction->type == ENDIF) {
            char bEnd[MAX_STRING_LENGTH] = "";
            sprintf(bEnd, "end_if_%d:", condCount++);
            output_assembly(bEnd, out);
        } else {
            pass_assignment(ptr->instruction, out);
        }
        ptr = ptr->next;
    }
}

void parseConditional(Instr *instruction, FILE *out) {
    // e.g. kon:
    char *condLabelStr = calloc(strlen(instruction->condition->conditionName) + 2, sizeof(char));
    // e.g. konCond:
    char *condStr = calloc(strlen(instruction->condition->conditionName) + strlen("Cond") + 2, sizeof(char));
    char assemblyStr[MAX_STRING_LENGTH] = "";
    char bEnd[MAX_STRING_LENGTH] = "";
    sprintf(bEnd, "b end_if_%d", condCount);
    strcpy(condLabelStr, instruction->condition->conditionName);
    strcpy(condStr, condLabelStr);
    strcat(condStr, "Cond");
    sprintf(assemblyStr, "b %s", condStr);
    output_assembly(assemblyStr, out); // outputs b konCond [or allyCond / tonyCond wtv]
    strcat(condLabelStr, ":");

    output_assembly(condLabelStr, out);  // output kon: / ally: / tony:

    parseInstrList(instruction->condition->ifInstr, out); // parse if tree
    output_assembly(bEnd, out);    // output b end_label
    strcat(condStr, ":");

    output_assembly(condStr, out);  // outputs konCond: / allyCond: / tonyCond:  
    pass_condition_assignment(instruction->condition, out);    // outputs cmp and branch
    parseInstrList(instruction->condition->elseInstr, out);    // parse else tree
    free(condLabelStr);
    free(condStr);
}

void assign_args_var(func *function) {
    for (int j = 0; j < function->argc; j++) {
        regs[9 + j] = function->args[j]->assign.var_name;
    }
    regs[12] = function->list->last->instruction->returnCall->var_name;
}

void clear_args_var(func *function) {
    for (int j = 0; j < function->argc; j++) {
        regs[9 + j] = NULL;
    }

    regs[12] = NULL;
}

void parseAllFunction(FILE *out) {
    Table *funcTable;
    funcTable = first;
    for (int i = 0; i < size; i++) {
        char *funcLabel = calloc(strlen(funcTable->funcName) + 2, sizeof(char));
        strcpy(funcLabel, funcTable->funcName);
        strcat(funcLabel, ":");
        output_assembly(funcLabel, out);
        func *currFunc = funcTable->function;

        assign_args_var(currFunc);

        instrNode *ptr = currFunc->list->first;

        while (ptr->instruction->type != RETURN) {
            if (ptr->instruction->type == FUNCTION) {
                parseFuncCaller(ptr, out);
            } else if (ptr->instruction->type == CONDITIONAL) {
                parseConditional(ptr->instruction, out);
            } else {
                pass_assignment(ptr->instruction, out);
            }
            ptr = ptr->next;
        }

        char returnLabel[MAX_STRING_LENGTH] = "";
        sprintf(returnLabel, "b ret_%d", retCount);
        output_assembly(returnLabel, out);

        clear_args_var(currFunc);

        free(funcLabel);

        funcTable = funcTable->next;
    }
}

void pass_condition_assignment(condInstr *condInstr, FILE *out) {
    Assignment *condition = condInstr->condition;
    printAssignment(condition);
    char assemblyStr[MAX_STRING_LENGTH] = "";
    char bFlag[3] = "";
    switch (condition->op) {
        case GREATER_THAN:
            strcat(bFlag, "ge");
            break;
        case LESS_THAN:
            strcat(bFlag, "le");
            break;
        case EQUAL_TO:
            strcat(bFlag, "eq");
            break;
        case NOT_EQUAL_TO:
            strcat(bFlag, "ne");
            break;
        case S_LESS_THAN:
            strcat(bFlag, "lt");
            break;
        case S_GREATER_THAN:
            strcat(bFlag, "gt");
            break;
        default:
            outputError(TYPE_ERROR, lineNum);
    }
    if (condition->n1->constant) {
        if (condition->n2->constant) {
            if (condition->n1->value == 1) {
                sprintf(assemblyStr, "b %s", condInstr->conditionName);
            }
        } else {
            sprintf(assemblyStr, "cmp #%d, r%d\nb%s %s",
                    condition->n1->value, get_reg(condition->n2->var),
                    bFlag, condInstr->conditionName);
        }

    } else {
        if (condition->n2->constant) {
            sprintf(assemblyStr, "cmp r%d, #%d\nb%s %s",
                    get_reg(condition->n1->var), condition->n2->value, bFlag, condInstr->conditionName);
        } else {
            sprintf(assemblyStr, "cmp r%d, r%d\nb%s %s",
                    get_reg(condition->n1->var), get_reg(condition->n2->var),
                    bFlag, condInstr->conditionName);
        }
    }
    output_assembly(assemblyStr, out);
}

/* Parse a function */
void parseFuncCaller(instrNode *start, FILE *out) {
    func *function = callFunction(start->instruction->caller);

    assign_args_var(function);
    char *call = start->instruction->caller->returnVar;
    int callerReg;
    if ((callerReg = get_reg(call)) == NOT_FOUND) {
        callerReg = assign(call);
    }

    for (int i = 0; i < function->argc; i++) {
        pass_assignment(function->args[i], out);
    }

    char branch[MAX_STRING_LENGTH];

    sprintf(branch, "b %s", function->funcName);

    output_assembly(branch, out);
    char returnLabel[MAX_STRING_LENGTH];
    sprintf(returnLabel, "ret_%d:", retCount++);
    output_assembly(returnLabel, out);


    char assembly[MAX_STRING_LENGTH];
    sprintf(assembly, "mov r%d, r12", callerReg);
    file_write(out, assembly);

    clear_args_var(function);
}

/* Passes an assignment instruction to assembly */
void pass_assignment(Instr *instruction, FILE *out) {
    Assignment *assignment = instruction->assign.variable->numVal;
    int regNum;
    char assemblyStr[512] = "";
    if ((regNum = get_reg(instruction->assign.var_name)) == NOT_FOUND) {
        regNum = assign(instruction->assign.var_name);
    }
    if (assignment->op == NOOP) {
        if (assignment->n1->constant) {
            sprintf(assemblyStr, "mov r%d, #%d", regNum, assignment->n1->value);
        } else {
            sprintf(assemblyStr, "mov r%d, r%d", regNum, get_reg(assignment->n1->var));
        }
        output_assembly(assemblyStr, out);
        return;
    } else if (assignment->op == TIMES) {
        sprintf(assemblyStr, "mul r%d, r", regNum);
        // If n1 is a constant, n2 must be register
        // If both are constant, lexer would have done the relevant calculations
        if (assignment->n1->constant) {
            // MLA only supports two register so the constant must be converted
            int tempReg = assign(TEMP_VAR);
            form_mul_conVar(assemblyStr, assignment, tempReg, assignment->n1->value, 1, out);
        } else {
            if (assignment->n2->constant) {
                int tempReg = assign(TEMP_VAR);
                form_mul_conVar(assemblyStr, assignment, tempReg, assignment->n2->value, 0, out);

                remove_reg(TEMP_VAR);
            } else {
                form_mul_varVar(assemblyStr, assignment);
            }
        }
        output_assembly(assemblyStr, out);
        return;

    } else if (assignment->op == ADD || assignment->op == SUB) {
        char header[MAX_STRING_LENGTH / 2] = "";
        form_add_sub(header, regNum, assignment->op);
        // If n1 is a constant, n2 must be register
        // If both are constant, lexer would have done the relevant calculations
        if (assignment->n1->constant) {
            sprintf(assemblyStr, "%s%d, #%d", header, get_reg(assignment->n2->var), assignment->n1->value);
        } else {
            if (assignment->n2->constant) {
                sprintf(assemblyStr, "%s%d, #%d", header, get_reg(assignment->n1->var), assignment->n2->value);
            } else {
                sprintf(assemblyStr, "%s%d, r%d", header, get_reg(assignment->n1->var), get_reg(assignment->n2->var));
            }
        }

        output_assembly(assemblyStr, out);
        return;
    }
}


Variable *assign_temp_var(int value) {
    Variable *tempVar = malloc(sizeof(Variable));
    Number *num = malloc(sizeof(Number));
    num->constant = true;
    num->value = value;
    Assignment *ass = malloc(sizeof(Assignment));
    ass->n1 = num;
    tempVar->isNumber = true;
    tempVar->numVal = ass;
    return tempVar;
}


/* Returns the register of the variable */
int get_reg(char *var) {
    for (int i = 0; i < NUMBER_OF_REGS; i++) {
        if (regs[i] != NULL) {
            if (strcmp(var, regs[i]) == 0) {
                return i;
            }
        }
    }
    return NOT_FOUND;
}

/* Checks if a variable already has assignment */
int exists(char *var) {
    for (int i = 0; i < NUMBER_OF_REGS; i++) {
        if (regs[i] != NULL) {
            if (strcmp(var, regs[i]) == 0) {
                return 1;
            }
        }
    }
    return 0;
}

/* Finds an empty register and assigns the variable to it */
int assign(char *var) {
    for (int i = 0; i < NUMBER_OF_REGS; i++) {
        if (regs[i] == NULL) {
            regs[i] = var;
            return i;
        }
    }
    return NOT_FOUND;
}

/* Removes a variable from a register */
int remove_reg(char *var) {
    for (int i = 0; i < NUMBER_OF_REGS; i++) {
        if (strcmp(regs[i], var) == 0) {
            regs[i] = NULL; // Remove assignment
            return i;
        }
    }
    return NOT_FOUND;
}

/* Creates the assembly for multiplying a constant and a variable */
void form_mul_conVar(char *assembly, Assignment *assignment,
                     int reg, int val, int varPos, FILE *out) {
    char strSrcVal[MAX_STRING_LENGTH];

    /* Form the move instruction */

    char movCons[MAX_STRING_LENGTH];
    sprintf(movCons, "mov r%d, #%d", reg, val);
    output_assembly(movCons, out);
    /* Form the mul instrution */
    sprintf(strSrcVal, "%d, r%d", reg, varPos ? get_reg(assignment->n2->var)
                                              : get_reg(assignment->n1->var));

    strcat(assembly, strSrcVal);
}

void form_mul_varVar(char *assembly, Assignment *assignment) {
    char regVal[MAX_STRING_LENGTH];
    sprintf(regVal, "%d, r%d", get_reg(assignment->n1->var), get_reg(assignment->n2->var));
    strcat(assembly, regVal);
}

/* Creates the assembly for add and minus instructions */
void form_add_sub(char *assembly, int reg, int type) {
    char strReg[50];
    sprintf(strReg, "%s%d, r", type ? "sub r" : "add r", reg);
    strcat(assembly, strReg);
}

/* Writes a line of assembly to a file */
void file_write(FILE *file, char str[MAX_STRING_LENGTH]) {
    strcat(str, "\n");
    fputs(str, file);
}

/* Outputs the line of assembly and writes it to the file */
void output_assembly(char assembly[MAX_STRING_LENGTH], FILE *out) {
    printf("Assembly: %s\n\n", assembly);
    file_write(out, assembly);
}

void output_registers() {
    FILE *regMap = fopen("registerMap.txt", "w");
    char *header = "Register\tVariable\n";
    fputs(header, regMap);
    for (int i = 0; i < NUMBER_OF_REGS; i++) {
        char regMapStr[MAX_STRING_LENGTH] = "";
        if (regs[i] != NULL) {
            sprintf(regMapStr, "r%d\t\t%*s\n", i, 8, regs[i]);
        }
        fputs(regMapStr, regMap);
    }
    fclose(regMap);
}