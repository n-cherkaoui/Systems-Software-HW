#include "bof.c"
#include "bof.h"
#include "instruction.c"
#include "instruction.h"
#include "machine_types.c"
#include "machine_types.h"
#include "regname.c"
#include "regname.h"
#include "utilities.c"
#include "utilities.h"
#include "vm_registers.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void performOperations(int text_length, BOFFILE bf);
void printRegisters();
int *copyRegisters();
void freeRegisters();
void debugArraysPrint(int PC);