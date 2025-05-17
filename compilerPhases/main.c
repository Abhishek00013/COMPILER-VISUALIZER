#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "ir.h"
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
    code[bytesRead] = '\0'; // 🛠️ Important fix here
    fclose(file);

    printf("Code from input.txt:\n%s\n", code);

    tokenize(code); // lexical analysis
    printf("Tokens written to tokens.txt\n");
    writeTokensToFile("tokens.txt");

    parseTokens(tokens, tokenCount); // parsing
    printf("Parsing complete\n");

    checkSemantics(tokens, tokenCount); // semantic analysis
    printf("Semantic check complete\n");

    generateIR(tokens, tokenCount);
    return 0;
}
