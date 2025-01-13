/*
    Filename: lexer.h
    Description: Lexer header file
    Author: Crazy Dave
    Date: 2025-13-01
*/

#ifndef LEXER_H
#define LEXER_H

#define _POSIX_C_SOURCE 200809L 

#include <stdio.h>
#include <stdbool.h>
#include "gjc.h"

void init_lexer(FILE *file);
void cleanup_lexer();

Token get_next_token();
Token peek_next_token();
void unget_token(Token token);

bool is_keyword(const char *str);
bool is_operator(char c);
TokenType get_keyword_type(const char *str);

void lexer_error(const char *message);

void print_token(Token token);

#endif // LEXER_H