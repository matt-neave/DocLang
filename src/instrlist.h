instrList *initInstrList();

void freeNode(instrNode *node);
void freeList(instrList *list);
void freeInstr(Instr *ins);

instrNode *initInstrNode(Instr *instruction, bool returnNode);

void add_to_instrList(instrList *list, Instr *instruction);

void printInstruction(Instr *instruction);

void printInstrList(instrList *list);