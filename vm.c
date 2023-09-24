#include "vm.h"
#include "vm_registers.h"

int *registers;
int **saveRegister;
int PC = 0;

// input: .bof
// output: .myp / .myo
// desc: read and execute instructions or return input if -p flag enabled.
int main(int argc, char *argv[]) {
  if (strcmp("-p", argv[1]) == 0) {
    BOFFILE bf = bof_read_open(argv[2]);
    BOFHeader header = bof_read_header(bf);

    printf(
        "Magic number: %s\nText Start Address: %d\nText Length: %d\nData Start "
        "Address(GP): %d\nData Length: %d\nStack Bottom Address(SP): %d\n",
        header.magic, header.text_start_address, header.text_length,
        header.data_start_address, header.data_length,
        header.stack_bottom_addr);
    // word_type x = bof_read_word(bf);

    printf("Addr Instruction\n");

    for (PC = 0; PC <= header.text_length - 4; PC += 4) {
      bin_instr_t word1 = instruction_read(bf);

      const char *name1 = instruction_assembly_form(word1);

      printf("\t%d %s\n", PC, name1);
    }
  }

  else {

    // int registers[32] = {0};
    BOFFILE bf = bof_read_open(argv[1]);
    BOFHeader header = bof_read_header(bf);
    saveRegister = malloc(sizeof(int *) * 10);
    registers = malloc(sizeof(int) * 32);
    //saveRegister[0] = registers;

    performOperations(header.text_length, bf);
  }

  //
  // We take in header, we get the text length, 3 instructions
}
void performOperations(int text_length, BOFFILE bf) {

  int numOfOperations = text_length / 4;

  for (int i = 0; i < numOfOperations; i++) {
    
    // Copy values of previous register into new register
    registers = copyRegisters();
    printRegisters();
    PC+=4;

    // for (int i = 0; i < 32; i++){
    //   registers[i] = saveRegister[PC/4][i];
    // }

    bin_instr_t it = instruction_read(bf);
    instr_type type = instruction_type(it);

    switch (type) {

    case syscall_instr_type:
      printf("==> addr: \t%d %s\n", PC, instruction_assembly_form(it));
      break;
    case immed_instr_type:
      switch (it.immed.op) {
      default:
        printf("==> addr: \t%d %s\n", PC, instruction_assembly_form(it));
        //saveRegister[PC / 4] = registers;
        addI(it.immed.rs, it.immed.rt, it.immed.immed);
        break;
      }

      // switch(it.reg.func){
      //   case ADD_F: case SUB_F: case AND_F: case BOR_F: case NOR_F: case
      //   XOR_F:
      // 	    func()
      // 	    break;
      // 	case MUL_F: case DIV_F:
      // 	    func()
      // 	    break;
      // 	case MFHI_F: case MFLO_F:
      // 	    func()
      // 	    break;
      // 	case SLL_F: case SRL_F:
      // 	    func()
      // 	    break;
      // 	case JR_F:
      // 	    func()
      // 	    break;
      // }
    }
    //debugArraysPrint(PC);
    // printRegisters();
  }
  for (int i = 0; i < 3; i++){
    debugArraysPrint(i*4);
  }
  //debugArraysPrint(4);
}

void addI(int rs, int rt, int immed) { registers[rt] = rs + immed; }

void printRegisters() {

  printf("PC: %d\n", PC);

  for (int i = 0; i < 32; i++) {
    printf("GPR[%s]: %d ", regnames[i], registers[i]);

    if (i % 4 == 0) {
      printf("\n");
    }
  }
  saveRegister[PC/4] = registers;
  printf("\n\n");
}

int *copyRegisters() {
  int *temp = malloc(sizeof(int) * 32);
  memcpy(temp, registers, sizeof(int) * 32);
  return temp;
}

void debugArraysPrint(int PC) {
  printf("Printing at PC: %d\n", PC);
  for (int i = 0; i < 32; i++) {
    printf("GPR[%s]: %d ", regnames[i], saveRegister[PC/4][i]);

    if (i % 4 == 0) {
      printf("\n");
    }
  }
  
  printf("\n\n");
}

void freeRegisters() {
  for (int i = 0; i < 10; i++) {
    free(saveRegister[i]);
  }
  free(saveRegister);
}