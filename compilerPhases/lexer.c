#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

Token tokens[MAX_TOKENS];
int tokenCount = 0;

int isKeyword(const char *str)
{
    const char *keywords[] = {"int", "float", "if", "else", "return", "while"};
    for (int i = 0; i < 6; ++i)
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    return 0;
}

void addTokenWithLine(TokenType type, const char *value, int line)
{
    tokens[tokenCount].type = type;
    strcpy(tokens[tokenCount].value, value);
    tokens[tokenCount].line = line;
    tokenCount++;
}

void tokenize(const char *code)
{
    int i = 0;
    int line = 1;
    while (code[i])
    {
        if (isspace(code[i]))
        {
            if (code[i] == '\n')
                line++;
            i++;
        }
        else if (code[i] == '\b') // skip backspace
        {
            printf("Skipping backspace character '\\b' (line %d)\n", line);
            i++;
        }
        else if (isalpha(code[i]) || code[i] == '_')
        {
            char buffer[64];
            int j = 0;
            while (isalnum(code[i]) || code[i] == '_')
                buffer[j++] = code[i++];
            buffer[j] = '\0';
            if (isKeyword(buffer))
                addTokenWithLine(TOKEN_KEYWORD, buffer, line);
            else
                addTokenWithLine(TOKEN_IDENTIFIER, buffer, line);
        }
        else if (isdigit(code[i]))
        {
            char buffer[64];
            int j = 0;
            while (isdigit(code[i]))
                buffer[j++] = code[i++];
            buffer[j] = '\0';
            addTokenWithLine(TOKEN_NUMBER, buffer, line);
        }
        else if (code[i] == '=') // TOKEN_ASSIGN
        {
            addTokenWithLine(TOKEN_ASSIGN, "=", line);
            i++;
        }
        else if (code[i] == '>' || code[i] == '<' || code[i] == '!' || code[i] == '=') // Relational operators
        {
            char buffer[3] = {code[i], 0, 0};
            i++;
            if (code[i] == '=')
            {
                buffer[1] = '=';
                i++;
            }
            addTokenWithLine(TOKEN_OPERATOR, buffer, line);
        }
        else if (code[i] == '+' || code[i] == '-' || code[i] == '*' || code[i] == '/')
        {
            char buffer[2] = {code[i], '\0'};
            addTokenWithLine(TOKEN_OPERATOR, buffer, line);
            i++;
        }
        else if (code[i] == '(')
        {
            addTokenWithLine(TOKEN_LPAREN, "(", line);
            i++;
        }
        else if (code[i] == ')')
        {
            addTokenWithLine(TOKEN_RPAREN, ")", line);
            i++;
        }
        else if (code[i] == '{')
        {
            addTokenWithLine(TOKEN_LBRACE, "{", line);
            i++;
        }
        else if (code[i] == '}')
        {
            addTokenWithLine(TOKEN_RBRACE, "}", line);
            i++;
        }
        else if (code[i] == ';')
        {
            addTokenWithLine(TOKEN_SEMICOLON, ";", line);
            i++;
        }
        else
        {
            printf("Warning: Skipping unknown character '%c' (line %d)\n", code[i], line);
            i++;
        }
    }
    addTokenWithLine(TOKEN_EOF, "EOF", line);
}

const char *getTokenTypeName(TokenType type)
{
    switch (type)
    {
    case TOKEN_KEYWORD:
        return "KEYWORD";
    case TOKEN_IDENTIFIER:
        return "IDENTIFIER";
    case TOKEN_NUMBER:
        return "NUMBER";
    case TOKEN_OPERATOR:
        return "OPERATOR";
    case TOKEN_ASSIGN:
        return "ASSIGN";
    case TOKEN_SEMICOLON:
        return "SEMICOLON";
    case TOKEN_LPAREN:
        return "LPAREN";
    case TOKEN_RPAREN:
        return "RPAREN";
    case TOKEN_LBRACE:
        return "LBRACE";
    case TOKEN_RBRACE:
        return "RBRACE";
    case TOKEN_EOF:
        return "EOF";
    default:
        return "UNKNOWN";
    }
}

void writeTokensToFile(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < tokenCount; i++)
    {
        fprintf(file, "Token Type: %s, Value: %s\n", getTokenTypeName(tokens[i].type), tokens[i].value);
    }

    fclose(file);
}