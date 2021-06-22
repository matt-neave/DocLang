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

#include "error.h"
#include "lexer.h"
#include "utils.h"
#include "parser.h"
#include "functions.h"
#include "instrlist.h"

const char *LECTURERS[] = {
        "kon",
        "tony",
        "ally",
        "antonio",
        "abbas",
        "steffen",
        "dalal",
        "alessandra",
        "iain",
        "mark",
        "sophia",
        "peter",
        "maria",
        "wayne",
        NULL
};

int lineNum = 0;


bool isLecturer(char *name) {
    int i = 0;
    while (*(LECTURERS + i) != NULL) {
        if (!strcmp(*(LECTURERS + i), name)) {
            return true;
        }
        i++;
    }

    return false;
}

int quotationCount(char *string) {
    int quotationCount = 0;
    char quotation = '\"';
    for (; *string != '\0'; string++) {
        if (*string == quotation) {
            quotationCount += 1;
        }
    }
    return quotationCount;
}

char *evaluatePrint(char *string, int lineNum) {
    int numQuotations = quotationCount(string);
    printf("Number of Quotation Marks: %d\n", numQuotations);

    if ((numQuotations % 2) == 0) {
        if (numQuotations == 0) {
            //TODO() - We need to look up the variable
            return string;
        } else if (numQuotations == 2) {
            char *content = strtok(string, "\"");
            strtok(NULL, "\"");
            return content;
        } else {
            outputError(SYNTAX, lineNum);
            exit(EXIT_FAILURE);
        }
    } else {
        outputError(SYNTAX, lineNum);
        exit(EXIT_FAILURE);
    }
}

char *modToken(char *token) {
    token = strtok(NULL, "says ");
    if (token == NULL) {
        outputError(SYNTAX, lineNum);
    }
    char *remaining = calloc(MAX_STRING_LENGTH, sizeof(char));
    concatToken(remaining);
    remaining[strlen(remaining) - 6] = 0;
    return remaining;
}

condInstr *ifCondition(char *token, char *condName) {
    Assignment *condition = evaluateAssignment(token);
    condInstr *newCondInstruction = calloc(1, sizeof(condInstr));
    free(token);
    newCondInstruction->conditionName = condName;
    newCondInstruction->condition = condition;
    newCondInstruction->ifInstr = initInstrList();
    newCondInstruction->elseInstr = initInstrList();
    return newCondInstruction;
}

int main(int argc, char **argv) {
    FILE *file;
    FILE *out = fopen("out.s", "w");
    func *inFunc = NULL;
    condInstr *condInstruction = NULL;
    bool addIf = false;
    bool addElse = false;
    bool freed = false;
    instrList *globalList = initInstrList();

    if (argc != 2) {
        printf("Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    } else {
        printf("You typed: \'%s %s\'\n\n", argv[0], argv[1]);
    }
    if ((file = fopen(argv[1], "r")) == NULL) {
        perror("Error: file does not exist!");
        exit(-1);


    }
    //Reads line by line from the source file
    char line[512];


    while (fgets((line), sizeof(line), file) != NULL) {
        lineNum++;
        trimLine(line);
        char *token;
        token = strtok(line, "\n");

        if (token == NULL || !strncmp(token, "//", 2)) {
            continue;
        }

        printf("Source Code: %s\n", line);
        if (line[strlen(line) - 1] != '.') {
            outputError(LINE_ENDING, lineNum);
            exit(EXIT_FAILURE);
        }

        token = strtok(line, ".");

        if (strstr(line, "shell") && !strstr(line, "ssh")) {
            token = strtok(line, "_");
        } else {
            token = strtok(line, " ");
        }

        Variable *variable = calloc(1, sizeof(Variable));
        char *variable_name = calloc(MAX_STRING_LENGTH, sizeof(char));
        if (variable == NULL || variable_name == NULL) {
            perror("calloc var");
            exit(EXIT_FAILURE);
        }

        while (token != NULL) {
            if (!strcmp(token, "piazza")) {
                token = strtok(NULL, " ");
                if (!strcmp(token, "says")) {
                    char *remaining = calloc(MAX_STRING_LENGTH, sizeof(char));
                    concatToken(remaining);
                    printf("You Printed: %s\n", evaluatePrint(remaining, lineNum));
                    free(remaining);
                }
            } else if (!strcmp(token, "ssh")) { // For calling a function
                token = strtok(NULL, "_");
                token = strtok(NULL, "@");
                if (token == NULL) {
                    outputError(SYNTAX, lineNum);
                }

                char *callerName = copyToken(token);
                funcCall *caller = createCaller(callerName);
                token = strtok(NULL, " ");
                char *returnVar = copyToken(token);
                caller->returnVar = returnVar;
                token = strtok(NULL, " ");

                if (token == NULL) {
                    outputError(SYNTAX, lineNum);
                }
                while (token != NULL) {
                    char *argValue = copyToken(token);
                    callerArgs(caller, argValue);
                    token = strtok(NULL, " ");
                }
                Instr *newInstr = calloc(1, sizeof(Instr));
                newInstr->type = FUNCTION;
                newInstr->caller = caller;

                freed = false;

                if (addIf) {
                    add_to_instrList(condInstruction->ifInstr, newInstr);
                    token = strtok(NULL, " ");
                    continue;
                }

                if (addElse) {
                    add_to_instrList(condInstruction->elseInstr, newInstr);
                    token = strtok(NULL, " ");
                    continue;
                }

                if (inFunc == NULL) {
                    add_to_instrList(globalList, newInstr);
                } else {
                    add_to_instrList(inFunc->list, newInstr);
                }
                
            } else if (!strcmp(token, "OH")) { //For returning from a function
                token = strtok(NULL, " ");
                if (strcmp(token, "BABY!")) {
                    outputError(SYNTAX, lineNum);
                }
                Instr *newInstr = calloc(1, sizeof(Instr));
                token = strtok(NULL, " ");
                newInstr->type = RETURN;
                newInstr->returnCall = calloc(1, sizeof(ReturnCall));
                newInstr->returnCall->var_name = copyToken(token);
                add_to_instrList(inFunc->list, newInstr);
                insert(inFunc->funcName, inFunc);
                //freeFunc(inFunc);
                inFunc = NULL;
                freed = false;
            } else if (!strcmp(token, "okie")) {
                token = strtok(NULL, " ");
                if (!strcmp(token, "dokie")) {
                    Instr *newInstruction = calloc(1, sizeof(Instr));
                    newInstruction->type = ENDIF;
                    add_to_instrList(condInstruction->elseInstr, newInstruction);
                    condInstruction = NULL;
                    addIf = false;
                    addElse = false;
                    freed = false;
                } else {
                    outputError(SYNTAX, lineNum);
                }
            } else if (!strcmp(token, "panopto")) {
                if (condInstruction == NULL) {
                    outputError(SYNTAX, lineNum);
                }
                token = strtok(NULL, " ");
                if (strcmp(token, "says")) {
                    outputError(SYNTAX, lineNum);
                }
                addIf = false;
                addElse = true;
                freed = false;
            } else if (isLecturer(token)) { // For Conditionals
                freed = false;
                char *condName = copyToken(token);
                if (condInstruction == NULL) {
                    condInstruction = ifCondition(modToken(token), condName);
                    Instr *newInstruction = calloc(1, sizeof(Instr));
                    newInstruction->type = CONDITIONAL;
                    newInstruction->condition = condInstruction;
                    if (inFunc != NULL) {
                        add_to_instrList(inFunc->list, newInstruction);
                    } else {
                        add_to_instrList(globalList, newInstruction);
                    }
                    addIf = true;
                } else {
                    //if the previous condition was satisfied
                    //if you still want to add this condition uncomment the lines
                    condInstr *newCondition = ifCondition(modToken(token), condName);
                    Instr *newInstr = calloc(1, sizeof(Instr));
                    newInstr->type = CONDITIONAL;
                    newInstr->condition = newCondition;
                    add_to_instrList(condInstruction->elseInstr, newInstr);
                    condInstruction = newCondition;
                    addIf = true;
                    addElse = false;
                }
            } else if (!strcmp(token, "shell")) { //For defining a function
                token = strtok(NULL, " ");
                if (token == NULL) {
                    outputError(SYNTAX, lineNum);
                }
                char *func_name = copyToken(token);
                token = strtok(NULL, " ");
                bool returnNum = !strcmp(token, "number");
                func *newFunction = createFunction(func_name, returnNum);
                token = strtok(NULL, "( ");
                while (token != NULL) {
                    bool isNumber = !strcmp(token, "number");
                    token = strtok(NULL, ",)");
                    if (token == NULL) {
                        outputError(SYNTAX, lineNum);
                    }

                    char *arg_name = copyToken(token);
                    addArgs(newFunction, arg_name, isNumber);
                    token = strtok(NULL, " ");
                }
                freed = false;
                inFunc = newFunction;
            } else {
                if (*variable_name == '\0') {
                    strcpy(variable_name, token);
                    token = strtok(NULL, " ");
                    continue;
                }

                if (!strcmp(token, "is")) {
                    token = strtok(NULL, " ");
                    if (!strcmp(token, "obviously")) {
                        char *remaining = calloc(MAX_STRING_LENGTH, sizeof(char));
                        concatToken(remaining);
                        variable->isNumber = true;
                        variable->numVal = evaluateAssignment(remaining);
                        free(remaining);
                    } else if (!strcmp(token, "trivially")) {
                        variable->isNumber = false;
                        variable->stringVal = calloc(MAX_STRING_LENGTH, sizeof(char));
                        if (variable->stringVal == NULL) {
                            perror("calloc stringVal");
                            exit(EXIT_FAILURE);
                        }
                        concatToken(variable->stringVal);
                    } else {
                        outputError(SYNTAX, lineNum);
                    }
                    freed = true;

                    Instr *newInstr = calloc(1, sizeof(Instr));
                    newInstr->type = ASSIGNMENT;
                    newInstr->assign.var_name = variable_name;
                    newInstr->assign.variable = variable;

                    if (addIf) {
                        add_to_instrList(condInstruction->ifInstr, newInstr);
                        token = strtok(NULL, " ");
                        continue;
                    }

                    if (addElse) {
                        add_to_instrList(condInstruction->elseInstr, newInstr);
                        token = strtok(NULL, " ");
                        continue;
                    }

                    if (inFunc != NULL) {
                        add_to_instrList(inFunc->list, newInstr);
                    } else {
                        add_to_instrList(globalList, newInstr);
                    }

                }
            }
            token = strtok(NULL, " ");
        }
        //the variable isn't always added to the table so freeing the table wont
        //free the variable and the name
        if (!freed) {
            free(variable);
            free(variable_name);
        }
    }
    printInstrList(globalList);
    parseInit(globalList, out);
    output_registers();
    fclose(file);
    fclose(out);
    freeFunctionTable();
    freeList(globalList);
    return 1;
}

