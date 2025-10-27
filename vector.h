/*
* Filename: vector.h
* Author: Sebastian Pacocha
* Date: 10/15/2025
* Description: Header file for a vector struct and its associated functions.
*/

#ifndef VECTOR_H
#define VECTOR_H



typedef struct {
    char name;
    double x;
    double y;
    double z;
} Vector;

void add (Vector *v1, const Vector *v2);
void subtract (Vector *v1, const Vector *v2);
void smult (Vector *v1, double scalar);
void list (Vector **vectors, int count);
void clear(Vector **vectors, int count); // clear the vector array
void load(const char *filename, Vector **vectors, int *vector_count); // Load vectors from given file
void save(const char *filename, Vector **vectors, int vector_count); // Save vectors to a file
double dot (const Vector *v1, const Vector *v2); 
Vector cross (const Vector *v1, const Vector *v2); 
Vector *create_or_get_vector(Vector *vectors[], int *count, char name);
Vector *find_vector(Vector *vectors[], int count, char name);


#endif // VECTOR_H

