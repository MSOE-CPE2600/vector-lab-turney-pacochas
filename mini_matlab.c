/*
* Filename: mini_matlab.c
* Author: Sebastian Pacocha
* Date: 10/03/2025
* Description: A simplified version of MATLAB for vector operations.
* Compilation: gcc -o test mini_matlab.c vector.c -lm
* Run: ./test
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "vector.h"

#define MAX_VECTORS 10
void print_help();
// a = 1 2 3
// a + b
int main(void) {
    char *identifier;
    char *operator;
    char *arg1;
    char *arg2;
    char *arg3;
    char *endptr;
    double scalar;
    bool running = true;
    Vector *vectors[MAX_VECTORS]; // Array to store pointers to vectors
    for (int i = 0; i < MAX_VECTORS; i++) {
        vectors[i] = NULL; // Initialize all pointers to NULL
    }
    int vector_count = 0;
    char input[100]; // Input buffer
    print_help();
    while (running) {
    printf(">> ");
    fgets(input, sizeof(input), stdin);
    identifier = strtok(input, " \n");
    operator = strtok(NULL, " \n");
    arg1 = strtok(NULL, " \n");
    arg2 = strtok(NULL, " \n");
    arg3 = strtok(NULL, " \n");
    if (identifier == NULL) {
        continue; // Empty input
    }
    if (strcmp(identifier, "quit") == 0) { // Check for quit command
        running = false;
    } else if (strcmp(identifier, "help") == 0) { // Check for help command
        print_help();
    } else if (strcmp(identifier, "list") == 0) { // Check for list command
        list(vectors, vector_count);
    } else if (strcmp(identifier, "clear") == 0) { // Check for clear command
        clear(vectors, vector_count);
        vector_count = 0;
    } else if (strlen(identifier) == 1 && strcmp(operator, "=") == 0) { // Check for new vector initialization
        if (vector_count >= MAX_VECTORS) {
            printf("Error: Maximum number of vectors reached.\n");
            continue;
        }
        Vector *new_vector = (Vector *)malloc(sizeof(Vector));
        new_vector->name = identifier[0];
        new_vector->x = atof(arg1);
        new_vector->y = atof(arg2);
        new_vector->z = atof(arg3);
        vectors[vector_count] = new_vector;
        vector_count++;
    } else if (strlen(identifier) == 1 && operator != NULL && strcmp(operator, "=") != 0) { // Check for vector operations
        if (vector_count < 2) { 
            printf("Error: Not enough vectors to perform operation.\n");
            continue;
        } else { 
            Vector *v1 = NULL;
            Vector *v2 = NULL;
            for (int i = 0; i < vector_count; i++) {
                if (vectors[i]->name == identifier[0]) {
                    v1 = vectors[i];
                }
                if (vectors[i]->name == arg1[0]) {
                    v2 = vectors[i];
                }
            }
            if (v1 == NULL || v2 == NULL) {
                printf("Error: One or both vectors not found.\n");
                continue;
            } else if (strcmp(operator, "*") == 0) {
                scalar = strtod(arg1, &endptr);
                if (*endptr != '\0') {
                    printf("Error: Invalid scalar value.\n");
                    continue;
                }
                smult(v1, scalar);
            }
            if (strcmp(operator, "+") == 0) {
                add(v1, v2);
            } else if (strcmp(operator, "-") == 0) {
                subtract(v1, v2);
            } else {
                printf("Error: Unknown operator '%s'.\n", operator);
            }
        }
        

    }



    }
    return 0;
}
void print_help() {
    printf("Welcome to Mini-MATLAB!\n");
    printf("You can create up to %d vectors.\n", MAX_VECTORS);
    printf("Commands:\n");
    printf("To create a vector: char = x y z\n");
    printf("To add vectors: char1 + char2\n");
    printf("To subtract vectors: char1 - char2\n");
    printf("To scale a vector: char * scalar\n");
    printf("To list vectors: list\n");
    printf("To clear vectors: clear\n");
    printf("To get help: help\n");
    printf("To exit: quit\n");
}