typedef struct FuncTab Table;

struct FuncTab {
    char *funcName;
    func *function;
    Table *next;
};

Table *first, *last;

void insert(char *varName, func *var);

void display();

void del(char *varName);

func *search(char *varName);

int getLabels(char *source);

void freeNumber();

void freeAssignment();

void freeVariable(Variable *variable);

void freeFunctionTable();
