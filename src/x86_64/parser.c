/*
    Filename: parser.c
    Description: Parser source file
    Author: Crazy Dave
    Date: 2025-13-01
*/

#include "parser.h"
#include "gjc.h"
#include "error_handler.h"
#include "lexer.h"
#include <stdlib.h>

static Token current_token;
static FILE *source_file;

void init_parser(FILE *file) {
    source_file = file;
    current_token = get_next_token(source_file);
}

void match(TokenType expected_type) {
    if (current_token.type == expected_type) {
        current_token = get_next_token(source_file);
    } else {
        report_error("Expected token type %d, but got %d", current_token.line, current_token.column);
    }
}

void parse_program() {
    while (current_token.type != TOKEN_EOF) {
        if (current_token.type == TOKEN_KEYWORD && strcmp(current_token.lexeme, "func") == 0) {
            parse_function_definition();
        } else {
            parse_statement();
        }
    }
}

void parse_function_definition() {
    match(TOKEN_KEYWORD); // 'func'
    match(TOKEN_IDENTIFIER); // function name
    match(TOKEN_OPERATOR); // '('
    // TODO: Parse parameter list
    match(TOKEN_OPERATOR); // ')'
    parse_block();
}

void parse_block() {
    match(TOKEN_OPERATOR); // '{'
    while (current_token.type != TOKEN_OPERATOR || strcmp(current_token.lexeme, "}") != 0) {
        parse_statement();
    }
    match(TOKEN_OPERATOR); // '}'
}

void parse_statement() {
    switch (current_token.type) {
        case TOKEN_KEYWORD:
            if (strcmp(current_token.lexeme, "if") == 0) {
                parse_if_statement();
            } else if (strcmp(current_token.lexeme, "while") == 0) {
                parse_while_statement();
            } else if (strcmp(current_token.lexeme, "for") == 0) {
                parse_for_statement();
            } else if (strcmp(current_token.lexeme, "return") == 0) {
                parse_return_statement();
            } else {
                report_error("Unexpected keyword: %s", current_token.line, current_token.column);
            }
            break;
        case TOKEN_IDENTIFIER:
            parse_assignment_or_function_call();
            break;
        default:
            report_error("Unexpected token at start of statement", current_token.line, current_token.column);
    }
}

void parse_if_statement() {
    match(TOKEN_KEYWORD); // 'if'
    match(TOKEN_OPERATOR); // '('
    parse_expression();
    match(TOKEN_OPERATOR); // ')'
    parse_block();
    if (current_token.type == TOKEN_KEYWORD && strcmp(current_token.lexeme, "else") == 0) {
        match(TOKEN_KEYWORD); // 'else'
        parse_block();
    }
}

void parse_while_statement() {
    match(TOKEN_KEYWORD); // 'while'
    match(TOKEN_OPERATOR); // '('
    parse_expression();
    match(TOKEN_OPERATOR); // ')'
    parse_block();
}

void parse_for_statement() {
    match(TOKEN_KEYWORD); // 'for'
    match(TOKEN_OPERATOR); // '('
    parse_statement(); // initialization
    match(TOKEN_OPERATOR); // ';'
    parse_expression(); // condition
    match(TOKEN_OPERATOR); // ';'
    parse_statement(); // increment
    match(TOKEN_OPERATOR); // ')'
    parse_block();
}

void parse_return_statement() {
    match(TOKEN_KEYWORD); // 'return'
    if (current_token.type != TOKEN_OPERATOR || strcmp(current_token.lexeme, ";") != 0) {
        parse_expression();
    }
    match(TOKEN_OPERATOR); // ';'
}

void parse_assignment_or_function_call() {
    match(TOKEN_IDENTIFIER);
    if (current_token.type == TOKEN_OPERATOR && strcmp(current_token.lexeme, "=") == 0) {
        match(TOKEN_OPERATOR); // '='
        parse_expression();
    } else if (current_token.type == TOKEN_OPERATOR && strcmp(current_token.lexeme, "(") == 0) {
        match(TOKEN_OPERATOR); // '('
        // TODO: Parse argument list
        match(TOKEN_OPERATOR); // ')'
    } else {
        report_error("Expected '=' or '(' after identifier", current_token.line, current_token.column);
    }
    match(TOKEN_OPERATOR); // ';'
}

void parse_expression() {
    // This is a simplified expression parser maybe i'll improve it later (MAYBE)
    parse_term();
    while (current_token.type == TOKEN_OPERATOR && 
           (strcmp(current_token.lexeme, "+") == 0 || strcmp(current_token.lexeme, "-") == 0)) {
        match(TOKEN_OPERATOR);
        parse_term();
    }
}

void parse_term() {
    parse_factor();
    while (current_token.type == TOKEN_OPERATOR && 
           (strcmp(current_token.lexeme, "*") == 0 || strcmp(current_token.lexeme, "/") == 0)) {
        match(TOKEN_OPERATOR);
        parse_factor();
    }
}

void parse_factor() {
    if (current_token.type == TOKEN_NUMBER) {
        match(TOKEN_NUMBER);
    } else if (current_token.type == TOKEN_IDENTIFIER) {
        match(TOKEN_IDENTIFIER);
        if (current_token.type == TOKEN_OPERATOR && strcmp(current_token.lexeme, "(") == 0) {
            match(TOKEN_OPERATOR); // '('
            // TODO: Parse argument list
            match(TOKEN_OPERATOR); // ')'
        }
    } else if (current_token.type == TOKEN_OPERATOR && strcmp(current_token.lexeme, "(") == 0) {
        match(TOKEN_OPERATOR); // '('
        parse_expression();
        match(TOKEN_OPERATOR); // ')'
    } else {
        report_error("Unexpected token in factor", current_token.line, current_token.column);
    }
}