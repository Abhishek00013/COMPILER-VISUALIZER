#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_number(const char *s) {
    if (*s == '-' || *s == '+') s++; // Allow negative/positive signs
    for (int i = 0; s[i]; i++) {
        if (!isdigit(s[i])) return 0;
    }
    return 1;
}

// Simplify expressions like "x = 2 + 3" to "x = 5"
void optimizeIR(const char *inputFile, const char *outputFile) {
    FILE *in = fopen(inputFile, "r");
    FILE *out = fopen(outputFile, "w");

    if (!in) {
        perror("Error opening input file");
        return;
    }
    if (!out) {
        perror("Error opening output file");
        fclose(in);
        return;
    }

    printf("Reading from: %s\n", inputFile);
    printf("Writing to: %s\n", outputFile);

    char line[256];
    int lines_processed = 0;
    int lines_optimized = 0;

    while (fgets(line, sizeof(line), in)) {
        lines_processed++;

        // Debug: Print the line to verify reading content
        printf("Read line %d: %s", lines_processed, line);

        char lhs[64], op1[64], op[4], op2[64];

        // Try to match "x = a + b" (basic 3-address format)
        if (sscanf(line, "%s = %s %s %s", lhs, op1, op, op2) == 4) {
            if (is_number(op1) && is_number(op2)) {
                int val1 = atoi(op1);
                int val2 = atoi(op2);
                int result;

                // Perform the arithmetic operation
                if (strcmp(op, "+") == 0) result = val1 + val2;
                else if (strcmp(op, "-") == 0) result = val1 - val2;
                else if (strcmp(op, "*") == 0) result = val1 * val2;
                else if (strcmp(op, "/") == 0 && val2 != 0) result = val1 / val2;
                else {
                    fputs(line, out);  // Unsupported op or divide by 0
                    continue;
                }

                // Write simplified line to output file
                fprintf(out, "%s = %d\n", lhs, result); // Constant folding
                lines_optimized++;
            } else {
                // If operands aren't both constants, write the line as is
                fputs(line, out); 
            }
        } else {
            // If the line doesn't match the expected pattern, write it unchanged
            fputs(line, out);
        }
    }

    fclose(in);
    fclose(out);

    printf("IR optimization complete: %s\n", outputFile);
    printf("Processed %d lines\n", lines_processed);
    printf("Optimized %d lines\n", lines_optimized);

    if (lines_processed == 0) {
        printf("Warning: No lines were processed. Check the input file format.\n");
    }
    if (lines_optimized == 0) {
        printf("Warning: No optimizations were made. Verify the input expressions.\n");
    }
}

