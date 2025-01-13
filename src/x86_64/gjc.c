/*
    Filename: gjc.c
    Description: Generic J Compiler source file
    Author: Crazy Dave
    Date: 2025-13-01
*/

#include "gjc.h"
#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// bool is_keyword(const char *str) {
//     (void)str; 
//     static const char *keywords[] = {
//         "if", "else", "while", "for", "return", "int", "float", "char", "void", "struct"
//     };
//     static const int keyword_count = sizeof(keywords) / sizeof(keywords[0]);

//     for (int i = 0; i < keyword_count; i++) {
//         if (strcmp(str, keywords[i]) == 0) {
//             return true;
//         }
//     }
//     return false;
// }

// bool is_operator(char c) {
//     (void)c; 
//     return strchr("+-*/=<>!&|^%~", c) != NULL;
// }

// TokenType get_keyword_type(const char *str) {
//     (void)str;
//     if (strcmp(str, "if") == 0) return TOKEN_IF;
//     if (strcmp(str, "else") == 0) return TOKEN_ELSE;
//     if (strcmp(str, "while") == 0) return TOKEN_WHILE;
//     if (strcmp(str, "for") == 0) return TOKEN_FOR;
//     if (strcmp(str, "return") == 0) return TOKEN_RETURN;
//     if (strcmp(str, "int") == 0) return TOKEN_INT;
//     if (strcmp(str, "float") == 0) return TOKEN_FLOAT;
//     if (strcmp(str, "char") == 0) return TOKEN_CHAR;
//     if (strcmp(str, "void") == 0) return TOKEN_VOID;
//     if (strcmp(str, "struct") == 0) return TOKEN_STRUCT;
//     return TOKEN_IDENTIFIER; // Not a keyword
// }

int compile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        return 1;
    }

    init_lexer(file);
    init_parser(file);

    parse_program();

    // TODO: Add semantic analysis
    // TODO: Add code generation

    cleanup_lexer();
    fclose(file);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    return compile(argv[1]);
}