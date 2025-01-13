/*
    Filename: lexer.h
    Description: lexer source file
    Author: Crazy Dave
    Date: 2025-13-01
*/

#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "error_handler.h"

#define MAX_TOKEN_LENGTH 100
#include <ctype.h>

static FILE *source_file;
static int current_line = 1;
static int current_column = 0;
static Token peeked_token = {TOKEN_EOF, NULL, 0, 0};
static bool has_peeked_token = false;

void init_lexer(FILE *file) {
    source_file = file;
    current_line = 1;
    current_column = 0;
    has_peeked_token = false;
}

void cleanup_lexer() {
    if (peeked_token.lexeme) {
        free(peeked_token.lexeme);
    }
}

Token get_next_token() {
    if (has_peeked_token) {
        has_peeked_token = false;
        return peeked_token;
    }

    char c;
    while ((c = fgetc(source_file)) != EOF) {
        if (isspace(c)) {
            if (c == '\n') {
                current_line++;
                current_column = 0;
            } else {
                current_column++;
            }
            continue;
        }

        current_column++;

        if (isalpha(c) || c == '_') {
            // Keyword
            char buffer[256] = {c};
            int i = 1;
            while ((c = fgetc(source_file)) != EOF && (isalnum(c) || c == '_')) {
                if (i < 255) buffer[i++] = c;
                current_column++;
            }
            buffer[i] = '\0';
            if (c != EOF) ungetc(c, source_file);

            if (is_keyword(buffer)) {
                return (Token){get_keyword_type(buffer), strdup(buffer), current_line, current_column - i};
            } else {
                return (Token){TOKEN_IDENTIFIER, strdup(buffer), current_line, current_column - i};
            }
        } else if (isdigit(c)) {
            // Number
            char buffer[256] = {c};
            int i = 1;
            while ((c = fgetc(source_file)) != EOF && (isdigit(c) || c == '.')) {
                if (i < 255) buffer[i++] = c;
                current_column++;
            }
            buffer[i] = '\0';
            if (c != EOF) ungetc(c, source_file);

            return (Token){TOKEN_NUMBER, strdup(buffer), current_line, current_column - i};
        } else if (c == '"') {
            // String
            char buffer[256] = {c};
            int i = 1;
            while ((c = fgetc(source_file)) != EOF && c != '"') {
                if (i < 255) buffer[i++] = c;
                current_column++;
                if (c == '\\') {
                    c = fgetc(source_file);
                    if (c != EOF) {
                        if (i < 255) buffer[i++] = c;
                        current_column++;
                    }
                }
            }
            if (c == '"') {
                if (i < 255) buffer[i++] = c;
                current_column++;
            }
            buffer[i] = '\0';

            return (Token){TOKEN_STRING, strdup(buffer), current_line, current_column - i};
        } else if (is_operator(c)) {
            // Operator
            char buffer[3] = {c, '\0', '\0'};
            char next_c = fgetc(source_file);
            if (next_c != EOF && is_operator(next_c)) {
                buffer[1] = next_c;
                current_column++;
            } else if (next_c != EOF) {
                ungetc(next_c, source_file);
            }

            return (Token){TOKEN_OPERATOR, strdup(buffer), current_line, current_column - strlen(buffer)};
        } else {
            // Unrecognized char
            char buffer[2] = {c, '\0'};
            return (Token){TOKEN_OPERATOR, strdup(buffer), current_line, current_column - 1};
        }
    }

    return (Token){TOKEN_EOF, NULL, current_line, current_column};
}

Token peek_next_token() {
    if (!has_peeked_token) {
        peeked_token = get_next_token();
        has_peeked_token = true;
    }
    return peeked_token;
}

void unget_token(Token token) {
    if (has_peeked_token) {
        // Handle error: can't unget when there's already a peeked token
        lexer_error("Attempted to unget token when a peeked token exists");
        return;
    }
    peeked_token = token;
    has_peeked_token = true;
}

bool is_keyword(const char *str) {
    // TODO
    return false;
}

bool is_operator(char c) {
    // TODO
    return false;
}

TokenType get_keyword_type(const char *str) {
    // TODO
    return TOKEN_KEYWORD;
}

void lexer_error(const char *message) {
    fprintf(stderr, "Lexer error at line %d, column %d: %s\n", current_line, current_column, message);
    exit(1);
}

void print_token(Token token) {
    printf("Token: type=%d, lexeme='%s', line=%d, column=%d\n", 
           token.type, token.lexeme, token.line, token.column);
}