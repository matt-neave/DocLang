typedef enum error {
    LINE_ENDING,
    SYNTAX,
    TYPE_ERROR,
    NAME_ERROR,
    MISSING_RETURN,
    ARGUMENT_COUNT
} ERROR;


void outputError(ERROR error, int lineNum);
