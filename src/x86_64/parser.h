/*
    Filename: parser.h
    Description: Parser header file
    Author: Crazy Dave
    Date: 2025-13-01
*/

#ifndef PARSER_H
#define PARSER_H

#include "gjc.h"
#include <stdio.h>

void init_parser(FILE *file);
void parse_program();
void parse_statement();
void parse_expression();
void parse_declaration();
void parse_function_definition();
void parse_if_statement();
void parse_while_statement();
void parse_for_statement();
void parse_return_statement();
void parse_block();
void parse_assignment_or_function_call();
void parse_term();
void parse_factor();
void match(TokenType type);
void synchronize();

// AST node creations
ASTNode* create_binary_op_node(ASTNode* left, TokenType op, ASTNode* right);
ASTNode* create_unary_op_node(TokenType op, ASTNode* operand);
ASTNode* create_literal_node(Token token);
ASTNode* create_variable_node(Token token);
ASTNode* create_if_node(ASTNode* condition, ASTNode* then_branch, ASTNode* else_branch);
ASTNode* create_while_node(ASTNode* condition, ASTNode* body);
ASTNode* create_for_node(ASTNode* init, ASTNode* condition, ASTNode* increment, ASTNode* body);
ASTNode* create_function_node(Token name, ASTNode* params, ASTNode* body);
ASTNode* create_return_node(ASTNode* expression);
ASTNode* create_block_node(ASTNode** statements, int statement_count);

#endif