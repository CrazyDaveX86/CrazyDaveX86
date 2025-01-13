/*
    Filename: error_handler.c
    Description: Error handler source file
    Author: Crazy Dave
    Date: 2025-13-01
*/

#include "error_handler.h"
#include <stdio.h>
#include <stdlib.h>

void report_error(const char *message, int line, int column) {
    fprintf(stderr, "Error at line %d, column %d: %s\n", line, column, message);
    exit(1);
}