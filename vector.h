/*
* Filename: vector.h
* Author: Sebastian Pacocha
* Date: 10/01/2025
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
void list (Vector *vectors[], int count);
void clear(Vector *vectors[], int count); // clear the vector array
//double dot (Vector v1, Vector v2); extra credit
//Vector cross (Vector v1, Vector v2); extra credit 


#endif // VECTOR_H
