#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_VARS 100
#define MAX_LINES 1000 // Added limit for line count to prevent overflow

char *vars[MAX_VARS];
int varCount = 0;

void addVar(const char *name)
{
    for (int i = 0; i < varCount; i++)
    {
        if (strcmp(vars[i], name) == 0)
            return;
    }
    if (varCount < MAX_VARS)
    {
        vars[varCount++] = strdup(name);
    }
    else
    {
        printf("Error: Maximum number of variables reached\n");
    }
}

int isConstant(const char *s)
{
    if (*s == '-')
        s++; // allow negative
    for (int i = 0; s[i]; i++)
    {
        if (!isdigit((unsigned char)s[i]))
            return 0;
    }
    return 1;
}

char *trimLeading(char *str)
{
    while (*str && isspace((unsigned char)*str))
        str++;
    return str;
}

void generateCode(const char *inputFile, const char *outputAsmFile)
{
    FILE *in = fopen(inputFile, "r");
    if (!in)
    {
        perror("Error opening input file");
        return;
    }

    // Read and store all lines to process twice (for variable tracking & code generation)
    char *lines[MAX_LINES];
    int lineCount = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), in))
    {
        if (lineCount < MAX_LINES)
        {
            lines[lineCount] = strdup(buffer);
            lineCount++;
        }
        else
        {
            printf("Warning: Input exceeds maximum line count of %d\n", MAX_LINES);
            break;
        }
    }
    fclose(in);

    if (lineCount == 0)
    {
        printf("No lines read from input file.\n");
        return;
    }

    // First pass: collect variables
    printf("First pass: Collecting variables...\n");
    for (int i = 0; i < lineCount; i++)
    {
        char trimmedLine[256];
        strcpy(trimmedLine, trimLeading(lines[i]));
        char lhs[64], op1[64], op[4], op2[64];

        if (sscanf(trimmedLine, "%s = %s %s %s", lhs, op1, op, op2) == 4)
        {
            addVar(lhs);
            addVar(op1);
            addVar(op2);
        }
        else if (sscanf(trimmedLine, "%s = %s", lhs, op1) == 2)
        {
            addVar(lhs);
            addVar(op1);
        }
        else if (sscanf(trimmedLine, "if %s %2[<>=!] %s goto %s", op1, op, op2, lhs) == 4)
        {
            addVar(op1);
            addVar(op2);
        }
        else if (strncmp(trimmedLine, "return", 6) == 0)
        {
            char *after = trimmedLine + 6;
            while (*after == ' ' || *after == '\t')
                after++;
            if (*after != '\0')
            {
                sscanf(after, "%s", lhs);
                addVar(lhs);
            }
        }
    }

    // Now open for writing output.asm
    FILE *asmFile = fopen(outputAsmFile, "w");
    if (!asmFile)
    {
        perror("Error creating output ASM file");
        return;
    }

    // Write .data section
    fprintf(asmFile, ".section .data\n");
    for (int i = 0; i < varCount; i++)
    {
        fprintf(asmFile, "%s: .long 0\n", vars[i]);
    }

    // Write .text section
    fprintf(asmFile, "\n.section .text\n.global _start\n_start:\n");

    // Second pass: generate code
    printf("Second pass: Generating assembly code...\n");
    for (int i = 0; i < lineCount; i++)
    {
        char *line = trimLeading(lines[i]);
        line[strcspn(line, "\r\n")] = 0; // remove newline

        char lhs[64], op1[64], op[4], op2[64];

        if (*line == '\0')
            continue;

        // Return
        if (strncmp(line, "return", 6) == 0)
        {
            char retVal[64] = {0};
            char *afterReturn = line + 6;
            while (*afterReturn == ' ' || *afterReturn == '\t')
                afterReturn++;
            if (*afterReturn != '\0')
            {
                sscanf(afterReturn, "%s", retVal);
                fprintf(asmFile, "    # %s\n", line);
                fprintf(asmFile, "    movl %s, %%eax\n", retVal);
                fprintf(asmFile, "    movl $1, %%ebx\n");
                fprintf(asmFile, "    int $0x80\n");
            }
            continue;
        }

        // 3-address code: x = y op z
        if (sscanf(line, "%s = %s %s %s", lhs, op1, op, op2) == 4)
        {
            fprintf(asmFile, "    # %s\n", line);
            if (isConstant(op1))
                fprintf(asmFile, "    movl $%s, %%eax\n", op1);
            else
                fprintf(asmFile, "    movl %s, %%eax\n", op1);

            if (strcmp(op, "+") == 0)
            {
                if (isConstant(op2))
                    fprintf(asmFile, "    addl $%s, %%eax\n", op2);
                else
                    fprintf(asmFile, "    addl %s, %%eax\n", op2);
            }
            else if (strcmp(op, "-") == 0)
            {
                if (isConstant(op2))
                    fprintf(asmFile, "    subl $%s, %%eax\n", op2);
                else
                    fprintf(asmFile, "    subl %s, %%eax\n", op2);
            }
            else if (strcmp(op, "*") == 0)
            {
                if (isConstant(op2))
                    fprintf(asmFile, "    imull $%s, %%eax\n", op2);
                else
                    fprintf(asmFile, "    imull %s, %%eax\n", op2);
            }
            else if (strcmp(op, "/") == 0)
            {
                fprintf(asmFile, "    movl %%eax, %%ebx\n");
                if (isConstant(op2))
                    fprintf(asmFile, "    movl $%s, %%ecx\n", op2);
                else
                    fprintf(asmFile, "    movl %s, %%ecx\n", op2);
                fprintf(asmFile, "    xorl %%edx, %%edx\n");
                fprintf(asmFile, "    idivl %%ecx\n");
                fprintf(asmFile, "    movl %%eax, %%ebx\n");
                fprintf(asmFile, "    movl %%ebx, %%eax\n");
            }
            fprintf(asmFile, "    movl %%eax, %s\n", lhs);
        }

        // Simple assignment
        else if (sscanf(line, "%s = %s", lhs, op1) == 2)
        {
            fprintf(asmFile, "    # %s\n", line);
            if (isConstant(op1))
                fprintf(asmFile, "    movl $%s, %%eax\n", op1);
            else
                fprintf(asmFile, "    movl %s, %%eax\n", op1);
            fprintf(asmFile, "    movl %%eax, %s\n", lhs);
        }

        // Conditional jump
        else if (sscanf(line, "if %s %2[<>=!] %s goto %s", op1, op, op2, lhs) == 4)
        {
            fprintf(asmFile, "    # %s\n", line);
            fprintf(asmFile, "    movl %s, %%eax\n", op1);
            fprintf(asmFile, "    cmpl %s, %%eax\n", op2);

            if (strcmp(op, ">") == 0)
                fprintf(asmFile, "    jg %s\n", lhs);
            else if (strcmp(op, "<") == 0)
                fprintf(asmFile, "    jl %s\n", lhs);
            else if (strcmp(op, ">=") == 0)
                fprintf(asmFile, "    jge %s\n", lhs);
            else if (strcmp(op, "<=") == 0)
                fprintf(asmFile, "    jle %s\n", lhs);
            else if (strcmp(op, "==") == 0)
                fprintf(asmFile, "    je %s\n", lhs);
            else if (strcmp(op, "!=") == 0)
                fprintf(asmFile, "    jne %s\n", lhs);
        }

        // Unconditional jump
        else if (sscanf(line, "goto %s", lhs) == 1)
        {
            fprintf(asmFile, " # %s\n", line);
            fprintf(asmFile, " jmp %s\n", lhs);
        }
        // Label
        else if (sscanf(line, "%[^:]:", lhs) == 1)
        {
            fprintf(asmFile, "%s:\n", lhs);
        }

        else
        {
            fprintf(asmFile, "    # Unrecognized line: %s\n", line);
        }
    }

    fclose(asmFile);
    printf("Assembly code generated: %s\n", outputAsmFile);

    // Free allocated memory for lines
    for (int i = 0; i < lineCount; i++)
    {
        free(lines[i]);
    }
}