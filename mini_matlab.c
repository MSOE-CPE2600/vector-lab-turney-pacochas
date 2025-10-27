/*
* Filename: mini_matlab.c
* Author: Sebastian Pacocha
* Date: 10/22/2025
* Description: An updated version of the mini_matlab program that 
*              saves and loads vectors from a csv file.
* Compilation: gcc -o mini_matlab mini_matlab.c vector.c -lm
* Usage: valgrind ./mini_matlab
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
    bool running = true;
    Vector **vectors = malloc(MAX_VECTORS * sizeof(Vector *));
    int vector_count = 0;

    if (!vectors) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return 1;
    }

    print_help();

    while (running) {
        printf(">> ");
        if (fgets(input, sizeof(input), stdin) == NULL)
            continue;
        input[strcspn(input, "\n")] = '\0'; // remove newline

        // --- Non-vector commands ---
        if (strcmp(input, "quit") == 0) break;

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
            printf("Vectors cleared.\n");
            vector_count = 0;
            continue;
        }

        // --- Load command ---
        if (strncmp(input, "load", 4) == 0) {
            char *filename = input + 4;
            while (isspace((unsigned char)*filename)) filename++;
            if (*filename == '\0') {
                printf("Error: Missing filename for load command.\n");
                continue;
            }
            load(filename, vectors, &vector_count);
            continue;
        }

        // --- Save command ---
        if (strncmp(input, "save", 4) == 0) {
            char *filename = input + 4;
            while (isspace((unsigned char)*filename)) filename++;
            if (*filename == '\0') {
                printf("Error: Missing filename for save command.\n");
                continue;
            }
            save(filename, vectors, vector_count);
            continue;
        }

        // --- Print single vector ---
        if (strlen(input) == 1 && isalpha(input[0])) {
            Vector *v = find_vector(vectors, vector_count, input[0]);
            if (v)
                printf("%c = %.2f %.2f %.2f\n", v->name, v->x, v->y, v->z);
            else
                printf("Error: Vector '%c' not found.\n", input[0]);
            continue;
        }

        // --- Parse command ---
        char *lhs = strtok(input, " ");
        char *eq = strtok(NULL, " ");
        char *rhs1 = strtok(NULL, " ");
        char *rhs2 = strtok(NULL, " ");
        char *rhs3 = strtok(NULL, " ");

        if (!lhs || !eq || !rhs1) {
            printf("Error: Invalid command format.\n");
            continue;
        }

        if (strlen(lhs) != 1 || !isalpha(lhs[0])) {
            printf("Error: Invalid variable name '%s'.\n", lhs);
            continue;
        }

        // --- Create vector manually (a = 1 2 3) ---
        if (rhs1 && rhs2 && rhs3 &&
            strspn(rhs1, "0123456789.-") == strlen(rhs1) &&
            strspn(rhs2, "0123456789.-") == strlen(rhs2) &&
            strspn(rhs3, "0123456789.-") == strlen(rhs3)) {

            double x = atof(rhs1);
            double y = atof(rhs2);
            double z = atof(rhs3);
            Vector *v = create_or_get_vector(vectors, &vector_count, lhs[0]);
            v->x = x; v->y = y; v->z = z;
            printf("%c = %.2f %.2f %.2f\n", lhs[0], x, y, z);
            continue;
        }

        // --- Vector math operations (a = b + c, etc.) ---
        if (!rhs2 || !rhs3) {
            printf("Error: Invalid expression format.\n");
            continue;
        }

        char *op = rhs2;
        Vector *v1 = find_vector(vectors, vector_count, rhs1[0]);
        Vector *v2 = find_vector(vectors, vector_count, rhs3[0]);
        Vector *dest = create_or_get_vector(vectors, &vector_count, lhs[0]);

        // Try parsing scalars
        char *endptr;
        double scalar1 = strtod(rhs1, &endptr);
        bool rhs1_is_scalar = rhs1 && *endptr == '\0';
        double scalar2 = strtod(rhs3, &endptr);
        bool rhs2_is_scalar = rhs3 && *endptr == '\0';

        if (strcmp(op, "+") == 0) {
            if (v1 && v2) {
                *dest = *v1;
                add(dest, v2);
                dest->name = lhs[0];
                printf("%c = %.2f %.2f %.2f\n", lhs[0], dest->x, dest->y, dest->z);
            } else printf("Error: One or both vectors not found.\n");
            continue;
        }

        if (strcmp(op, "-") == 0) {
            if (v1 && v2) {
                *dest = *v1;
                subtract(dest, v2);
                dest->name = lhs[0];
                printf("%c = %.2f %.2f %.2f\n", lhs[0], dest->x, dest->y, dest->z);
            } else printf("Error: One or both vectors not found.\n");
            continue;
        }

        if (strcmp(op, "*") == 0) {
            if (rhs1_is_scalar && v2) {
                *dest = *v2;
                smult(dest, scalar1);
                dest->name = lhs[0];
                printf("%c = %.2f %.2f %.2f\n", lhs[0], dest->x, dest->y, dest->z);
                continue;
            }
            if (v1 && rhs2_is_scalar) {
                *dest = *v1;
                smult(dest, scalar2);
                dest->name = lhs[0];
                printf("%c = %.2f %.2f %.2f\n", lhs[0], dest->x, dest->y, dest->z);
                continue;
            }
            printf("Error: Invalid scalar multiplication format.\n");
            continue;
        }

        if (strcmp(op, ".") == 0) {
            if (v1 && v2) {
                double result = dot(v1, v2);
                printf("%c = %.2f\n", lhs[0], result);
            } else printf("Error: Invalid vectors for dot product.\n");
            continue;
        }

        if (strcmp(op, "x") == 0 || strcmp(op, "X") == 0) {
            if (v1 && v2) {
                Vector cross_result = cross(v1, v2);
                *dest = cross_result;
                dest->name = lhs[0];
                printf("%c = %.2f %.2f %.2f\n", lhs[0], dest->x, dest->y, dest->z);
            } else printf("Error: Invalid vectors for cross product.\n");
            continue;
        }

        printf("Error: Unknown or invalid command.\n");
    }

    // --- Free all allocated memory ---
    for (int i = 0; i < vector_count; i++) {
        free(vectors[i]);
    }
    free(vectors);

    return 0;
}

void print_help(void) {
    printf("Welcome to the upgraded Mini-MATLAB!\n");
    printf("Commands:\n");
    printf(" load <file>       -> load vectors from CSV file\n");
    printf(" save <file>       -> save vectors to CSV file\n");
    printf(" a = x y z         -> create vector\n");
    printf(" a = b + c         -> add vectors\n");
    printf(" a = b - c         -> subtract vectors\n");
    printf(" a = 3 * b or b * 3 -> scalar multiply\n");
    printf(" a = b . c         -> dot product\n");
    printf(" a = b x c         -> cross product\n");
    printf(" list              -> list all vectors\n");
    printf(" clear             -> delete all vectors\n");
    printf(" a                 -> show vector values\n");
    printf(" help              -> show this help\n");
    printf(" quit              -> exit program\n");
}
