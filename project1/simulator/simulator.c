/*
 * Project 1
 * EECS 370 LC-2K Instruction-level simulator
 *
 * Make sure to NOT modify printState or any of the associated functions
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// DO NOT CHANGE THE FOLLOWING DEFINITIONS

// Machine Definitions
#define MEMORYSIZE 65536 /* maximum number of words in memory (maximum number of lines in a given file)*/
#define NUMREGS 8        /*total number of machine registers [0,7]*/

// File Definitions
#define MAXLINELENGTH 1000 /* MAXLINELENGTH is the max number of characters we read */

typedef struct
    stateStruct
{
    int pc;
    int mem[MEMORYSIZE];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);

static inline int convertNum(int32_t);

int main(int argc, char **argv)
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;
    int num_instructions;

    if (argc != 2)
    {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL)
    {
        printf("error: can't open file %s , please ensure you are providing the correct path", argv[1]);
        perror("fopen");
        exit(2);
    }

    /* read the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; state.numMemory++)
    {
        if (state.numMemory >= MEMORYSIZE)
        {
            fprintf(stderr, "exceeded memory size\n");
            exit(2);
        }
        if (sscanf(line, "%x", state.mem + state.numMemory) != 1)
        {
            fprintf(stderr, "error in reading address %d\n", state.numMemory);
            exit(2);
        }
        printf("memory[%d]=0x%08X\n", state.numMemory, state.mem[state.numMemory]);
    }

    state.pc = 0;
    for (int i = 0; i < NUMREGS; i++)
    {
        state.reg[i] = 0;
    }

    while (1)
    {
        printState(&state);
        num_instructions++;
        int temp = state.mem[state.pc];
        int opcode = (temp >> 22) & 0x7;

        if (opcode == 6)
        {
            break;
        }

        if (opcode == 0)
        {
            int regA = (temp >> 19) & 0x7;
            int regB = (temp >> 16) & 0x7;
            int destReg = temp & 0x7;
            state.reg[destReg] = state.reg[regA] + state.reg[regB];
            state.pc++;
        }
        else if (opcode == 1)
        {
            int regA = (temp >> 19) & 0x7;
            int regB = (temp >> 16) & 0x7;
            int destReg = temp & 0x7;
            state.reg[destReg] = ~(state.reg[regA] | state.reg[regB]);
            state.pc++;
        }
        else if (opcode == 2)
        {
            int regA = (temp >> 19) & 0x7;
            int regB = (temp >> 16) & 0x7;
            int offsetField = convertNum(temp & 0xFFFF);
            state.reg[regB] = state.mem[state.reg[regA] + offsetField];
            state.pc++;
        }
        else if (opcode == 3)
        {
            int regA = (temp >> 19) & 0x7;
            int regB = (temp >> 16) & 0x7;
            int offsetField = convertNum(temp & 0xFFFF);
            // int temp_var = state.reg[regA] + offsetField;
            // printf("%d",temp_var);
            // state.mem[temp_var] = state.reg[regB];
            state.mem[state.reg[regA] + offsetField] = state.reg[regB];
            state.pc++;
        }
        else if (opcode == 4)
        {
            int regA = (temp >> 19) & 0x7;
            int regB = (temp >> 16) & 0x7;
            int offsetField = convertNum(temp & 0xFFFF);
            if (state.reg[regA] == state.reg[regB])
            {
                state.pc += 1 + offsetField;
            }
            else
            {
                state.pc++;
            }
        }
        else if (opcode == 5)
        {
            int regA = (temp >> 19) & 0x7;
            int regB = (temp >> 16) & 0x7;
            if (state.reg[regB] == state.reg[regA])
            {
                state.pc++;
            }
            else
            {
                state.reg[regB] = state.pc + 1;
                state.pc = state.reg[regA];
            }
        }
        else if (opcode == 7)
        {
            state.pc++;
        }
        else
        {
            printf("Unknown opcode %d at pc %d\n", opcode, state.pc);
            exit(1);
        }
    }

    state.pc++;
    printf("machine halted\n");
    printf("total of %d instructions executed\n", num_instructions);
    printf("final state of machine:\n");

    printState(&state);

    // Your code ends here!

    return (0);
}

/*
 * DO NOT MODIFY ANY OF THE CODE BELOW.
 */

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++)
    {
        printf("\t\tmem[ %d ] 0x%08X\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++)
    {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

// convert a 16-bit number into a 32-bit Linux integer
static inline int convertNum(int num)
{
    return num - ((num & (1 << 15)) ? 1 << 16 : 0);
}

/*
 * Write any helper functions that you wish down here.
 */
