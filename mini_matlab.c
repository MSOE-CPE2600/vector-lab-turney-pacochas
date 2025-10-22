/*
* Filename: mini_matlab.c
* Author: Sebastian Pacocha
* Date: 10/03/2025
* Description: A simplified version of MATLAB for vector operations.
* Compilation: gcc -o lab5 mini_matlab.c vector.c -lm
* Run: ./lab5
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "vector.h"

#define MAX_VECTORS 10

void print_help();

int main(void) {
    char input[100];
    char *lhs, *eq, *rhs1, *op, *rhs2;
    bool running = true;
    Vector *vectors[MAX_VECTORS];
    int vector_count = 0;

    for (int i = 0; i < MAX_VECTORS; i++) {
        vectors[i] = NULL;
    }

    print_help();

    while (running) {
        printf(">> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            continue;
        }

        // Remove newline
        input[strcspn(input, "\n")] = '\0';

        // --- Simple commands ---
        if (strcmp(input, "quit") == 0) {
            running = false;
            continue;
        }

        if (strcmp(input, "help") == 0) {
            print_help();
            continue;
        }

        if (strcmp(input, "list") == 0) {
            list(vectors, vector_count);
            continue;
        }

        if (strcmp(input, "clear") == 0) {
            clear(vectors, vector_count);
            vector_count = 0;
            continue;
        }

        // --- Print single vector ---
        if (strlen(input) == 1 && isalpha(input[0])) {
            Vector *v = find_vector(vectors, vector_count, input[0]);
            if (v != NULL) {
                printf("%c = %.2f %.2f %.2f\n", v->name, v->x, v->y, v->z);
            } else {
                printf("Error: Vector '%c' not found.\n", input[0]);
            }
            continue;
        }

        // --- Tokenize command ---
        lhs = strtok(input, " ");
        eq  = strtok(NULL, " ");
        rhs1 = strtok(NULL, " ");
        char *maybe_next = strtok(NULL, " ");

        if (lhs == NULL || eq == NULL) {
            printf("Error: Invalid command format.\n");
            continue;
        }

        // --- Ensure LHS is a valid variable ---
        if (strlen(lhs) != 1 || !isalpha(lhs[0])) {
            printf("Error: Invalid variable name '%s'.\n", lhs);
            continue;
        }

        // --- Vector creation: a = 1 2 3 ---
        if (rhs1 && maybe_next) {
            char *a2 = maybe_next;
            char *a3 = strtok(NULL, " ");
            if (a3 &&
                strspn(rhs1, "0123456789.-") == strlen(rhs1) &&
                strspn(a2, "0123456789.-") == strlen(a2) &&
                strspn(a3, "0123456789.-") == strlen(a3)) {

                Vector *v = create_or_get_vector(vectors, &vector_count, lhs[0]);
                v->x = atof(rhs1);
                v->y = atof(a2);
                v->z = atof(a3);
                printf("%c = %.2f %.2f %.2f\n", lhs[0], v->x, v->y, v->z);
                continue;
            } else {
                // If not 3 numbers, treat as operator expression
                op = a2;
                rhs2 = a3;
            }
        } else {
            printf("Error: Invalid command format.\n");
            continue;
        }

        // --- Expressions: a = b + c, a = 3 * b, etc. ---
        if (rhs1 && op && rhs2) {
            Vector *v1 = find_vector(vectors, vector_count, rhs1[0]);
            Vector *v2 = find_vector(vectors, vector_count, rhs2[0]);
            Vector *dest = create_or_get_vector(vectors, &vector_count, lhs[0]);

            char *endptr;
            double scalar1 = strtod(rhs1, &endptr);
            bool rhs1_is_scalar = (*endptr == '\0');

            scalar1 = rhs1_is_scalar ? scalar1 : 0;

            double scalar2 = strtod(rhs2, &endptr);
            bool rhs2_is_scalar = (*endptr == '\0');

            scalar2 = rhs2_is_scalar ? scalar2 : 0;

            // --- Addition ---
            if (strcmp(op, "+") == 0) {
                if (v1 != NULL && v2 != NULL) {
                    *dest = *v1;
                    add(dest, v2);
                    dest->name = lhs[0];
                    printf("%c = %.2f %.2f %.2f\n", lhs[0], dest->x, dest->y, dest->z);
                } else {
                    printf("Error: One or both vectors not found for addition.\n");
                }
                continue;
            }

            // --- Subtraction ---
            if (strcmp(op, "-") == 0) {
                if (v1 != NULL && v2 != NULL) {
                    *dest = *v1;
                    subtract(dest, v2);
                    dest->name = lhs[0];
                    printf("%c = %.2f %.2f %.2f\n", lhs[0], dest->x, dest->y, dest->z);
                } else {
                    printf("Error: One or both vectors not found for subtraction.\n");
                }
                continue;
            }

            // --- Scalar Multiplication ---
            if (strcmp(op, "*") == 0) {
                if (rhs1_is_scalar && v2 != NULL) {
                    *dest = *v2;
                    smult(dest, scalar1);
                    dest->name = lhs[0];
                    printf("%c = %.2f %.2f %.2f\n", lhs[0], dest->x, dest->y, dest->z);
                    continue;
                }
                if (v1 != NULL && rhs2_is_scalar) {
                    *dest = *v1;
                    smult(dest, scalar2);
                    dest->name = lhs[0];
                    printf("%c = %.2f %.2f %.2f\n", lhs[0], dest->x, dest->y, dest->z);
                    continue;
                }
                printf("Error: Invalid scalar multiplication format.\n");
                continue;
            }

            // --- Dot Product ---
            if (strcmp(op, ".") == 0) {
                if (v1 != NULL && v2 != NULL) {
                    double result = dot(v1, v2);
                    printf("%c = %.2f\n", lhs[0], result);
                } else {
                    printf("Error: Invalid vectors for dot product.\n");
                }
                continue;
            }

            // --- Cross Product ---
            if (strcmp(op, "x") == 0 || strcmp(op, "X") == 0) {
                if (v1 != NULL && v2 != NULL) {
                    Vector cross_result = cross(v1, v2);
                    *dest = cross_result;
                    dest->name = lhs[0];
                    printf("%c = %.2f %.2f %.2f\n", lhs[0], dest->x, dest->y, dest->z);
                } else {
                    printf("Error: Invalid vectors for cross product.\n");
                }
                continue;
            }

            printf("Error: Unknown operator '%s'.\n", op);
            continue;
        }

        printf("Error: Invalid command format.\n");
    }

    clear(vectors, vector_count);
    return 0;
}

void print_help() {
    printf("Welcome to Mini-MATLAB!\n");
    printf("You can create up to %d vectors.\n", MAX_VECTORS);
    printf("Commands:\n");
    printf("  a = x y z             -> create vector\n");
    printf("  a = b + c             -> add vectors\n");
    printf("  d = a - b             -> subtract vectors\n");
    printf("  e = 3 * b or e = b * 3 -> scalar multiply\n");
    printf("  h = a . b             -> dot product\n");
    printf("  g = a x b             -> cross product\n");
    printf("  list                  -> show all vectors\n");
    printf("  clear                 -> delete all vectors\n");
    printf("  a                     -> show vector values\n");
    printf("  help                  -> show this help\n");
    printf("  quit                  -> exit program\n");
}
