/* Parse an instruction list */
void parseInstrList(instrList *list, FILE *out);

/* Parse a function */
void parseFuncCaller(instrNode *start, FILE *out);

/* Assign r9, r10, r11, 12 as arg1, arg2, arg3, return */
void assign_args_var(func *function);

/* Clear assignment of r9, r10, r11, r12 */
void clear_args_var(func *function);

void parseAllFunction(FILE *out);

/* Passes an assignment instruction to assembly */
void pass_assignment(Instr *instruction, FILE *out);

/* Assigns temporary variables */
Variable *assign_temp_var(int value);

void parseInit(instrList *list, FILE *out);

/* Parse Conditional Instructions */
void parseConditional(Instr *instruction, FILE *out);

void pass_condition_assignment(condInstr *condition, FILE *out);

/* Returns the register of the variable */
int get_reg(char *var);

/* Checks if a variable already has assignment */
int exists(char *var);

/* Finds an empty register and assigns the variable to it */
int assign(char *var);

/* Removes a variable from a register */
int remove_reg(char *var);

/* Creates the assembly for multiplying a constant and a variable */
void form_mul_conVar(char *assembly, Assignment *assignment,
                     int reg, int val, int varPos, FILE *out);

/* Creates the assembly for add instructions */
void form_add_sub(char *assembly, int reg, int type);

/* Writes a line of assembly to a file */
void file_write(FILE *file, char *str);

/* Outputs the line of assembly and writes it to the file */
void output_assembly(char *assembly, FILE *out);

/* Parses multiplication of two variables */
void form_mul_varVar(char *assembly, Assignment *assignment);

void output_registers();