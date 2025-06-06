// parser.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

Token tokens[MAX_TOKENS];
int current = 0;

FILE *dotFile;
int nodeId = 0;

Token peek()
{
    return tokens[current];
}

int match(TokenType expected, const char *value)
{
    if (peek().type == expected && (!value || strcmp(peek().value, value) == 0))
    {
        current++;
        return 1;
    }
    return 0;
}

void syntaxError(const char *message)
{
    printf("Syntax Error: %s at '%s' (line %d)\n", message, peek().value, peek().line);
    exit(1);
}

int newNode(const char *label)
{
    int id = nodeId++;
    fprintf(dotFile, "  node%d [label=\"%s\"];\n", id, label);
    return id;
}

void addEdge(int from, int to)
{
    fprintf(dotFile, "  node%d -> node%d;\n", from, to);
}

int tokenNode(Token token)
{
    char label[128];
    snprintf(label, sizeof(label), "%s\\n[%s]", token.value, getTokenTypeName(token.type));
    return newNode(label);
}

int expr()
{
    int node = newNode("expr");
    if (peek().type == TOKEN_IDENTIFIER || peek().type == TOKEN_NUMBER)
    {
        addEdge(node, tokenNode(tokens[current]));
        current++;
        if (peek().type == TOKEN_OPERATOR)
        {
            addEdge(node, tokenNode(tokens[current]));
            current++;
            addEdge(node, expr());
        }
    }
    else
    {
        syntaxError("Expected identifier or number in expression");
    }
    return node;
}

int decl()
{
    int node = newNode("decl");
    if (!match(TOKEN_KEYWORD, "int"))
        syntaxError("Expected 'int'");
    addEdge(node, tokenNode(tokens[current - 1]));

    if (!match(TOKEN_IDENTIFIER, NULL))
        syntaxError("Expected identifier");
    addEdge(node, tokenNode(tokens[current - 1]));

    if (!match(TOKEN_SEMICOLON, NULL))
        syntaxError("Expected ';'");
    addEdge(node, tokenNode(tokens[current - 1]));

    return node;
}

int stmt()
{
    int node = newNode("stmt");
    if (match(TOKEN_IDENTIFIER, NULL)) // assignment statements
    {
        addEdge(node, tokenNode(tokens[current - 1])); // identifier node

        if (!match(TOKEN_ASSIGN, NULL))
            syntaxError("Expected '='");
        addEdge(node, tokenNode(tokens[current - 1]));

        addEdge(node, expr()); // expression after '='

        if (!match(TOKEN_SEMICOLON, NULL))
            syntaxError("Expected ';'");
        addEdge(node, tokenNode(tokens[current - 1]));
    }
    else if (match(TOKEN_KEYWORD, "return")) // return statements
    {
        addEdge(node, tokenNode(tokens[current - 1]));

        addEdge(node, expr()); // return expression

        if (!match(TOKEN_SEMICOLON, NULL))
            syntaxError("Expected ';'");
        addEdge(node, tokenNode(tokens[current - 1]));
    }
    else if (match(TOKEN_KEYWORD, "if")) // if statements
    {
        addEdge(node, tokenNode(tokens[current - 1]));

        if (!match(TOKEN_LPAREN, NULL))
            syntaxError("Expected '(' after 'if'");
        addEdge(node, tokenNode(tokens[current - 1]));

        addEdge(node, expr()); // condition

        if (!match(TOKEN_RPAREN, NULL))
            syntaxError("Expected ')' after condition");
        addEdge(node, tokenNode(tokens[current - 1]));

        addEdge(node, block()); // 'if' block

        if (match(TOKEN_KEYWORD, "else")) // 'else' block
        {
            addEdge(node, tokenNode(tokens[current - 1]));
            addEdge(node, block()); // block after the 'else'
        }
    }
    else if (match(TOKEN_KEYWORD, "while")) // while statements
    {
        addEdge(node, tokenNode(tokens[current - 1]));

        if (!match(TOKEN_LPAREN, NULL))
            syntaxError("Expected '(' after 'while'");
        addEdge(node, tokenNode(tokens[current - 1]));

        addEdge(node, expr()); // condition

        if (!match(TOKEN_RPAREN, NULL))
            syntaxError("Expected ')' after condition");
        addEdge(node, tokenNode(tokens[current - 1]));

        addEdge(node, block()); // the block after the 'while'
    }
    else
    {
        syntaxError("Unknown statement");
    }
    return node;
}

int block()
{
    int node = newNode("block");

    if (!match(TOKEN_LBRACE, NULL))
        syntaxError("Expected '{'");
    addEdge(node, tokenNode(tokens[current - 1]));

    // declarations inside the block
    while (peek().type == TOKEN_KEYWORD && strcmp(peek().value, "int") == 0)
    {
        addEdge(node, decl());
    }

    // statements inside the block
    while (peek().type == TOKEN_IDENTIFIER || strcmp(peek().value, "return") == 0 ||
           strcmp(peek().value, "if") == 0 || strcmp(peek().value, "while") == 0)
    {
        addEdge(node, stmt());
    }

    if (!match(TOKEN_RBRACE, NULL))
        syntaxError("Expected '}'");
    addEdge(node, tokenNode(tokens[current - 1]));

    return node;
}

int func_def()
{
    int node = newNode("func_def");

    if (!match(TOKEN_KEYWORD, "int"))
        syntaxError("Expected 'int'");
    addEdge(node, tokenNode(tokens[current - 1]));

    if (!match(TOKEN_IDENTIFIER, NULL))
        syntaxError("Expected function name");
    addEdge(node, tokenNode(tokens[current - 1]));

    if (!match(TOKEN_LPAREN, NULL))
        syntaxError("Expected '('");
    addEdge(node, tokenNode(tokens[current - 1]));

    if (!match(TOKEN_RPAREN, NULL))
        syntaxError("Expected ')'");
    addEdge(node, tokenNode(tokens[current - 1]));

    addEdge(node, block());

    return node;
}

int func_def_list()
{
    int node = newNode("func_def_list");
    while (peek().type == TOKEN_KEYWORD && strcmp(peek().value, "int") == 0)
    {
        addEdge(node, func_def());
    }
    return node;
}

int program()
{
    int node = newNode("program");
    addEdge(node, func_def_list());

    if (peek().type != TOKEN_EOF)
        syntaxError("Expected EOF");

    return node;
}

void parseTokens(Token inputTokens[], int count)
{
    memcpy(tokens, inputTokens, sizeof(Token) * count);
    current = 0;
    nodeId = 0;

    dotFile = fopen("tree.dot", "w");
    fprintf(dotFile, "digraph ParseTree {\n");
    fprintf(dotFile, "  node [shape=box, fontname=\"Courier\"];\n");

    int root = program();

    fprintf(dotFile, "}\n");
    fclose(dotFile);

    printf("Parse tree generated: tree.dot\n");
}