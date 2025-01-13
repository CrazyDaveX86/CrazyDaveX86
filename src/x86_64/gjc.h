/*
    Filename: gjc.h
    Description: Generic J Compiler header file
    Author: Crazy Dave
    Date: 2025-13-01
*/

#ifndef GJC_H
#define GJC_H

#include <stdbool.h>
#include <string.h>

typedef enum {
    TOKEN_EOF,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_CHAR,
    TOKEN_OPERATOR,
    TOKEN_KEYWORD,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_FOR,
    TOKEN_RETURN,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_VOID,
    TOKEN_STRUCT
} TokenType;

typedef struct {
    TokenType type;
    char *lexeme;
    int line;
    int column;
} Token;

typedef struct ASTNode {
} ASTNode;

#endif