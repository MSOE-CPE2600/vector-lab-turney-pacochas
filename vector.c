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

