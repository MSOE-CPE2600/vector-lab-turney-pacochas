/*
* Filename: vector.c
* Author: Sebastian Pacocha
* Date: 10/01/2025
* Description: Implementation file for vector operations.
*/

#include "vector.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VECTORS 10

void add (Vector *v1, const Vector *v2) {
    v1->x += v2->x;
    v1->y += v2->y;
    v1->z += v2->z;
    printf("%c = %.2f  %.2f  %.2f\n", v1->name, v1->x, v1->y, v1->z);
    
}

void subtract (Vector *v1, const Vector *v2) {
    v1->x -= v2->x;
    v1->y -= v2->y;
    v1->z -= v2->z;
    printf("%c = %.2f  %.2f  %.2f\n", v1->name, v1->x, v1->y, v1->z);
}

void smult (Vector *v1, double scalar) {
    v1->x *= scalar;
    v1->y *= scalar;
    v1->z *= scalar;
    printf("%c = %.2f  %.2f  %.2f\n", v1->name, v1->x, v1->y, v1->z);
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
    printf("Cross product (%c x %c) = %.2f  %.2f  %.2f\n",
           v1->name, v2->name, result.x, result.y, result.z);
    return result;
}    

void list(Vector *vectors[], int count) {
    count = count < 10 ? count : 10; // Limit to 10 vectors
    for (int i = 0; i < count; i++) {
        printf("- %c = %.2f  %.2f  %.2f\n",
       vectors[i]->name, vectors[i]->x, vectors[i]->y, vectors[i]->z);
    }
}
void clear(Vector *vectors[], int count) {
    for (int i = 0; i < count; i++) {
        free(vectors[i]);
        vectors[i] = NULL;
    }
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
    new_vec->name = name;
    new_vec->x = 0;
    new_vec->y = 0;
    new_vec->z = 0;
    vectors[(*count)++] = new_vec;
    return new_vec;
}

