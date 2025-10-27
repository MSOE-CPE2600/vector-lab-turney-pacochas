/*
* Filename: vector.c
* Author: Sebastian Pacocha
* Date: 10/22/2025
* Description: Implementation file for vector operations.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "vector.h"

#define MAX_VECTORS 10

void add (Vector *v1, const Vector *v2) {
    v1->x += v2->x;
    v1->y += v2->y;
    v1->z += v2->z;
}

void subtract (Vector *v1, const Vector *v2) {
    v1->x -= v2->x;
    v1->y -= v2->y;
    v1->z -= v2->z;
}

void smult (Vector *v1, double scalar) {
    v1->x *= scalar;
    v1->y *= scalar;
    v1->z *= scalar;
}

double dot (const Vector *v1, const Vector *v2) {
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

Vector cross (const Vector *v1, const Vector *v2) {
    Vector result;
    result.name = 'C'; // Placeholder name
    result.x = v1->y * v2->z - v1->z * v2->y;
    result.y = v1->z * v2->x - v1->x * v2->z;
    result.z = v1->x * v2->y - v1->y * v2->x;
    return result;
}

void list(Vector **vectors, int count) {
    for (int i = 0; i < count; i++) {
        if (vectors[i] != NULL) {
            printf("- %c = %.2f  %.2f  %.2f\n",
                   vectors[i]->name, vectors[i]->x, vectors[i]->y, vectors[i]->z);
        }
    }
}

/* Free every allocated vector and null the slot to avoid dangling pointers. */
void clear(Vector **vectors, int count) {
    for (int i = 0; i < count; i++) {
        if (vectors[i] != NULL) {
            free(vectors[i]);
            vectors[i] = NULL;
        }
    }
}

/*
 * Load CSV lines of the form:
 *   a,1.00,2.00,3.00
 * Replaces current vectors (clears them first).
 * On allocation failure during load, frees any vectors created during the load
 * and leaves *vector_count set to 0.
 */
void load(const char *filename, Vector **vectors, int *vector_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open '%s'.\n", filename);
        return;
    }

    char line[128];

    /* Clear existing vectors first to avoid leaking them */
    clear(vectors, *vector_count);
    *vector_count = 0;

    while (fgets(line, sizeof(line), file)) {
        char name;
        double x, y, z;
        if (sscanf(line, " %c , %lf , %lf , %lf", &name, &x, &y, &z) == 4) {
            if (*vector_count >= MAX_VECTORS) {
                printf("Warning: reached MAX_VECTORS (%d); skipping remaining lines\n", MAX_VECTORS);
                break;
            }

            Vector *v = malloc(sizeof(Vector));
            if (!v) {
                fprintf(stderr, "Memory error while loading vectors. Cleaning up partial load.\n");
                /* free any allocations done during this load */
                clear(vectors, *vector_count);
                *vector_count = 0;
                fclose(file);
                return;
            }

            v->name = name;
            v->x = x;
            v->y = y;
            v->z = z;
            vectors[*vector_count] = v;
            (*vector_count)++;
        } else {
            /* Skip malformed line but continue reading */
            printf("Warning: skipping malformed line: %s", line);
        }
    }

    fclose(file);
    printf("Loaded %d vectors from '%s'.\n", *vector_count, filename);
}

void save(const char *filename, Vector **vectors, int vector_count) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open '%s' for writing.\n", filename);
        return;
    }

    for (int i = 0; i < vector_count; i++) {
        if (vectors[i] != NULL) {
            fprintf(file, "%c,%.2f,%.2f,%.2f\n",
                    vectors[i]->name, vectors[i]->x, vectors[i]->y, vectors[i]->z);
        }
    }

    fclose(file);
    printf("Saved %d vectors to '%s'.\n", vector_count, filename);
}

Vector *find_vector(Vector *vectors[], int count, char name) {
    for (int i = 0; i < count; i++) {
        if (vectors[i] != NULL && vectors[i]->name == name) {
            return vectors[i];
        }
    }
    return NULL;
}

Vector *create_or_get_vector(Vector *vectors[], int *count, char name) {
    Vector *existing = find_vector(vectors, *count, name);
    if (existing != NULL) {
        return existing;
    }

    if (*count >= MAX_VECTORS) {
        printf("Error: Maximum number of vectors reached.\n");
        return NULL;
    }

    Vector *new_vec = (Vector *)malloc(sizeof(Vector));
    if (!new_vec) {
        fprintf(stderr, "Error: Memory allocation failed when creating vector.\n");
        return NULL;
    }

    new_vec->name = name;
    new_vec->x = 0;
    new_vec->y = 0;
    new_vec->z = 0;
    vectors[(*count)++] = new_vec;
    return new_vec;
}
