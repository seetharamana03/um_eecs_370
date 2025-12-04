/*
 * Project 1
 * EECS 370 LC-2K Instruction-level simulator
 *
 * Make sure to NOT modify printState or any of the associated functions
 */


//gcc -g -O0 cache.c my_p1s_sim.c -lm -o simulator 
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

extern void cache_init(int blockSize, int numSets, int blocksPerSet);
extern int cache_access(int addr, int write_flag, int write_data);
extern void printStats();
static stateType state;
static int num_mem_accesses = 0;
int mem_access(int addr, int write_flag, int write_data)
{
    ++num_mem_accesses;
    if (write_flag)
    {
        state.mem[addr] = write_data;
        if (state.numMemory <= addr)
        {
            state.numMemory = addr + 1;
        }
    }
    return state.mem[addr];
}
int get_num_mem_accesses()
{
    return num_mem_accesses;
}

int main(int argc, char **argv)
{
    char line[MAXLINELENGTH];
    // stateType state;
    FILE *filePtr;
    int num_instructions = 0;

    if (argc != 5)
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

    int blockSize = atoi(argv[2]);
    int numSets = atoi(argv[3]);
    int blocksPerSet = atoi(argv[4]);

    cache_init(blockSize, numSets, blocksPerSet);

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
        // printf("memory[%d]=0x%08X\n", state.numMemory, state.mem[state.numMemory]);
    }

    state.pc = 0;
    for (int i = 0; i < NUMREGS; i++)
    {
        state.reg[i] = 0;
    }

    while (1)
    {
        // printState(&state);
        num_instructions++;

        int temp = cache_access(state.pc, 0, 0); //Instruction Fetch
        int opcode = (temp >> 22) & 0x7;

        if (opcode == 6)
        {
            break;
        }

        if (opcode == 0) // ADD
        {
            int regA = (temp >> 19) & 0x7;
            int regB = (temp >> 16) & 0x7;
            int destReg = temp & 0x7;
            state.reg[destReg] = state.reg[regA] + state.reg[regB];
            state.pc++;
        }
        else if (opcode == 1) // NOR
        {
            int regA = (temp >> 19) & 0x7;
            int regB = (temp >> 16) & 0x7;
            int destReg = temp & 0x7;
            state.reg[destReg] = ~(state.reg[regA] | state.reg[regB]);
            state.pc++;
        }
        else if (opcode == 2) // LW
        {
            int regA = (temp >> 19) & 0x7;
            int regB = (temp >> 16) & 0x7;
            int offsetField = convertNum(temp & 0xFFFF);
            int addr = state.reg[regA] + offsetField;
            state.reg[regB] = cache_access(addr, 0, 0);
            state.pc++;
        }
        else if (opcode == 3) // SW
        {
            int regA = (temp >> 19) & 0x7;
            int regB = (temp >> 16) & 0x7;
            int offsetField = convertNum(temp & 0xFFFF);
            int addr = state.reg[regA] + offsetField;
            cache_access(addr, 1, state.reg[regB]);
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

    printStats();

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
