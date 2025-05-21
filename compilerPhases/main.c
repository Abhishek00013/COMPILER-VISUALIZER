#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "ir.h"
#include "optimizer.h"
#include "codegen.h"

#define MAX_CODE_SIZE 10000

int main()
{
    printf("main() has started\n");

    FILE *file = fopen("input.txt", "r");
    if (!file)
    {
        perror("Error opening input.txt");
        return 1;
    }

    char code[MAX_CODE_SIZE];
    size_t bytesRead = fread(code, 1, MAX_CODE_SIZE - 1, file);
    code[bytesRead] = '\0';
    fclose(file);

    printf("Code from input.txt:\n%s\n", code);

    tokenize(code); // lexical analysis
    printf("Tokens written to tokens.txt\n");
    writeTokensToFile("tokens.txt");

    parseTokens(tokens, tokenCount); // parsing
    printf("Parsing complete\n");

    checkSemantics(tokens, tokenCount); // semantic analysis
    printf("Semantic check complete: All variables and returns are valid\n");

    generateIR(tokens, tokenCount);

    optimizeIR("ir.txt", "ir_optimized.txt");
    printf("Optimization complete: ir_optimized.txt\n");

    generateCode("ir_optimized.txt", "output.asm");
    printf("Code generation complete: output.asm\n");
    return 0;
}
