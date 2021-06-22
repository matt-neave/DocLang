void addArgs(func *function, char *arg_name, bool isNum);

func *createFunction(char *funcName, DataType returnType);

void freeFunc(func *fun);

void printFunction(func *function);

funcCall *createCaller(char *funcName);

void callerArgs(funcCall *caller, char *args);

void printCaller(funcCall *caller);

func *callFunction(funcCall *caller);