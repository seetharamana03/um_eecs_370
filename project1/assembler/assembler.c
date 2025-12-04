/**
 * Project 1
 * Assembler code fragment for LC-2K
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Every LC2K file will contain less than 1000 lines of assembly.
#define MAXLINELENGTH 1000
#define ADD 0
#define NOR 1
#define LW 2
#define SW 3
#define BEQ 4
#define JALR 5
#define HALT 6
#define NOOP 7

typedef struct
{
    char label[MAXLINELENGTH];
    int address;
} Label;

int num_labels = 0;
Label locations[MAXLINELENGTH];

int readAndParse(FILE *, char *, char *, char *, char *, char *);
static void checkForBlankLinesInCode(FILE *inFilePtr);
static inline int isNumber(char *);
static inline void printHexToFile(FILE *, int);
int findLabelAddress(char *);

int main(int argc, char **argv)
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
        arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3)
    {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
               argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL)
    {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }

    // Check for blank lines in the middle of the code.
    checkForBlankLinesInCode(inFilePtr);

    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL)
    {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    int address = 0;
    rewind(inFilePtr);
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
    {
        if (strlen(label) > 0)
        {
            for (int i = 0; i < num_labels; i++)
            {
                if (strcmp(label, locations[i].label) == 0)
                {
                    // duplicate label error check
                    exit(1);
                }
            }
            strcpy(locations[num_labels].label, label);
            locations[num_labels].address = address;
            num_labels++;
        }
        address++;
    }

    rewind(inFilePtr);
    int pc = 0;
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
    {
        if (strcmp(opcode, ".fill") != 0 && strcmp(opcode, "noop") != 0 && strcmp(opcode, "halt") != 0 && (!isNumber(arg0) || !isNumber(arg1)))
        {
            // non-integer register arguments error check
            exit(1);
        }

        int machine_output = 0;
        int offset = 0;
        if (strcmp(opcode, "add") == 0)
        {
            if (atoi(arg0) < 0 || atoi(arg0) > 7 || atoi(arg1) < 0 || atoi(arg1) > 7 || atoi(arg2) < 0 || atoi(arg2) > 7)
            {
                exit(1);
            }
            machine_output = (ADD << 22) | (atoi(arg0) << 19) | (atoi(arg1) << 16) | atoi(arg2);
        }
        else if (strcmp(opcode, "nor") == 0)
        {
            if (atoi(arg0) < 0 || atoi(arg0) > 7 || atoi(arg1) < 0 || atoi(arg1) > 7 || atoi(arg2) < 0 || atoi(arg2) > 7)
            {
                exit(1);
            }
            machine_output = (NOR << 22) | (atoi(arg0) << 19) | (atoi(arg1) << 16) | atoi(arg2);
        }
        else if (strcmp(opcode, "lw") == 0)
        {
            if (atoi(arg0) < 0 || atoi(arg0) > 7 || atoi(arg1) < 0 || atoi(arg1) > 7)
            {
                exit(1);
            }
            if (isNumber(arg2))
            {
                offset = atoi(arg2);
            }
            else
            {
                offset = findLabelAddress(arg2);
            }
            if (offset < -32768 || offset > 32767)
            {
                // offset field out of range error check
                exit(1);
            }
            if (offset < 0)
            {
                offset = (offset & 0xFFFF);
            }
            machine_output = (LW << 22) | (atoi(arg0) << 19) | (atoi(arg1) << 16) | offset;
        }
        else if (strcmp(opcode, "sw") == 0)
        {
            if (atoi(arg0) < 0 || atoi(arg0) > 7 || atoi(arg1) < 0 || atoi(arg1) > 7)
            {
                exit(1);
            }
            if (isNumber(arg2))
            {
                offset = atoi(arg2);
            }
            else
            {
                offset = findLabelAddress(arg2);
            }
            if (offset < -32768 || offset > 32767)
            {
                // offset field out of range error check
                exit(1);
            }
            if (offset < 0)
            {
                offset = (offset & 0xFFFF);
            }
            machine_output = (SW << 22) | (atoi(arg0) << 19) | (atoi(arg1) << 16) | offset;
        }
        else if (strcmp(opcode, "beq") == 0)
        {
            if (atoi(arg0) < 0 || atoi(arg0) > 7 || atoi(arg1) < 0 || atoi(arg1) > 7)
            {
                exit(1);
            }
            if (atoi(arg0) == atoi(arg1))
            {
                if (!isNumber(arg2))
                {
                    int labelAddress = findLabelAddress(arg2);
                    offset = labelAddress - pc - 1;
                }
                else
                {
                    offset = atoi(arg2) - pc - 1;
                }
            }
            else
            {
                if (!isNumber(arg2))
                {
                    // not sure if this is right
                    int labelAddress = findLabelAddress(arg2);
                    offset = labelAddress - pc - 1;
                }
                else
                {
                    offset = atoi(arg2);
                }
            }
            if (offset < -32768 || offset > 32767)
            {
                // offset field out of range error check
                exit(1);
            }
            if (offset < 0)
            {
                offset = (offset & 0xFFFF);
            }
            machine_output = (BEQ << 22) | (atoi(arg0) << 19) | (atoi(arg1) << 16) | offset;
        }
        else if (strcmp(opcode, "jalr") == 0)
        {
            if (atoi(arg0) < 0 || atoi(arg0) > 7 || atoi(arg1) < 0 || atoi(arg1) > 7)
            {
                exit(1);
            }
            machine_output = (JALR << 22) | (atoi(arg0) << 19) | (atoi(arg1) << 16);
        }
        else if (strcmp(opcode, "noop") == 0)
        {
            machine_output = (NOOP << 22);
        }
        else if (strcmp(opcode, "halt") == 0)
        {
            machine_output = (HALT << 22);
        }
        else if (strcmp(opcode, ".fill") == 0)
        {
            int output;
            if (!isNumber(arg0))
            {
                output = findLabelAddress(arg0);
            }
            else
            {
                output = atoi(arg0);
            }
            machine_output = output;
        }
        else
        {
            // use of undefined label error check
            exit(1);
        }

        printHexToFile(outFilePtr, machine_output);
        pc++;
    }

    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    // rewind(inFilePtr);

    /* after doing a readAndParse, you may want to do the following to test the
        opcode */
    // if (!strcmp(opcode, "add")) {
    //     /* do whatever you need to do for opcode "add" */
    // }

    /* here is an example of using isNumber. "5" is a number, so this will
       return true */
    // if(isNumber("5")) {
    //     printf("It's a number\n");
    // }

    /* here is an example of using printHexToFile. This will print a
       machine code word / number in the proper hex format to the output file */
    // printHexToFile(outFilePtr, 123);

    return (0);
}

// Returns non-zero if the line contains only whitespace.
static int lineIsBlank(char *line)
{
    char whitespace[4] = {'\t', '\n', '\r', ' '};
    int nonempty_line = 0;
    for (int line_idx = 0; line_idx < strlen(line); ++line_idx)
    {
        int line_char_is_whitespace = 0;
        for (int whitespace_idx = 0; whitespace_idx < 4; ++whitespace_idx)
        {
            if (line[line_idx] == whitespace[whitespace_idx])
            {
                line_char_is_whitespace = 1;
                break;
            }
        }
        if (!line_char_is_whitespace)
        {
            nonempty_line = 1;
            break;
        }
    }
    return !nonempty_line;
}

// Exits 2 if file contains an empty line anywhere other than at the end of the file.
// Note calling this function rewinds inFilePtr.
static void checkForBlankLinesInCode(FILE *inFilePtr)
{
    char line[MAXLINELENGTH];
    int blank_line_encountered = 0;
    int address_of_blank_line = 0;
    rewind(inFilePtr);

    for (int address = 0; fgets(line, MAXLINELENGTH, inFilePtr) != NULL; ++address)
    {
        // Check for line too long
        if (strlen(line) >= MAXLINELENGTH - 1)
        {
            printf("error: line too long\n");
            exit(1);
        }

        // Check for blank line.
        if (lineIsBlank(line))
        {
            if (!blank_line_encountered)
            {
                blank_line_encountered = 1;
                address_of_blank_line = address;
            }
        }
        else
        {
            if (blank_line_encountered)
            {
                printf("Invalid Assembly: Empty line at address %d\n", address_of_blank_line);
                exit(2);
            }
        }
    }
    rewind(inFilePtr);
}

/*
 * NOTE: The code defined below is not to be modifed as it is implimented correctly.
 */

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
                 char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL)
    {
        /* reached end of file */
        return (0);
    }

    /* check for line too long */
    if (strlen(line) == MAXLINELENGTH - 1)
    {
        printf("error: line too long\n");
        exit(1);
    }

    // Ignore blank lines at the end of the file.
    if (lineIsBlank(line))
    {
        return 0;
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label))
    {
        /* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
           opcode, arg0, arg1, arg2);

    return (1);
}

static inline int
isNumber(char *string)
{
    int num;
    char c;
    return ((sscanf(string, "%d%c", &num, &c)) == 1);
}

// Prints a machine code word in the proper hex format to the file
static inline void
printHexToFile(FILE *outFilePtr, int word)
{
    fprintf(outFilePtr, "0x%08X\n", word);
}

int findLabelAddress(char *label)
{
    for (int i = 0; i < num_labels; i++)
    {
        if (strcmp(locations[i].label, label) == 0)
        {
            return locations[i].address;
        }
    }
    // use of undefined label
    exit(1);
}