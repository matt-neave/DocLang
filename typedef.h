#include <stdint.h>
#include <stdbool.h>

#define MAX_STRING_LENGTH 512

extern int lineNum;
extern int condCount;
extern int size;
extern int retCount;

typedef struct instructionNode instrNode;

typedef enum operation {
    ADD,
    SUB,
    TIMES,
    DIVIDE,
    NOOP,
    GREATER_THAN,
    LESS_THAN,
    EQUAL_TO,
    NOT_EQUAL_TO,
    S_LESS_THAN,
    S_GREATER_THAN
} Operation;

typedef enum datatype {
    STRING,
    NUMBER,
    VOID
} DataType;

typedef struct assignment Assignment;

typedef struct var {
    DataType isNumber;
    union {
        Assignment *numVal;
        char *stringVal;
    };
} Variable;

typedef struct num Number;

struct num {
    bool constant;
    union {
        int value;
        char *var;
    };
};

struct assignment {
    Operation op;
    Number *n1;
    Number *n2;
};

typedef struct returns {
    char *var_name;
} ReturnCall;

typedef enum instrType {
    FUNCTION,
    ASSIGNMENT,
    CONDITIONAL,
    RETURN,
    ENDIF
} InstrType;

typedef struct funcCaller {
    char *funcName;
    char **args;
    int argc;
    char *returnVar;
} funcCall;

typedef struct instructionList {
    instrNode *first;
    instrNode *last;
} instrList;

typedef struct conditionals {
    char *conditionName;
    Assignment *condition;
    instrList *ifInstr;
    instrList *elseInstr;
} condInstr;

typedef struct instruction {
    InstrType type;
    union {
        struct {
            char *var_name;
            Variable *variable;
        } assign;
        ReturnCall *returnCall;
        funcCall *caller;
        condInstr *condition;
    };
} Instr;


typedef struct function {
    char *funcName;
    DataType retType;
    int argc;
    Instr **args;
    instrList *list;
} func;


struct instructionNode {
    instrNode *next;
    Instr *instruction;
    bool returnNode;
};
