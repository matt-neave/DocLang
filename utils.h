bool is_num(char *string);

int getNum(char *string);

/* Evaluates an assignment */
Assignment *evaluateAssignment(char *source);

void trimLine(char *string);

int evaluateOperation(int n1, int n2, Operation op);

Operation getOperation(char *opString);

void concatToken(char *destination);

char *copyToken(char *token);

void printOperation(Operation op);

void printVar(Variable *variable);

void printNumber(Number *num);

void printAssignment(Assignment *numVal);