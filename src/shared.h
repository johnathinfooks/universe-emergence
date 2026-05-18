#pragma once

// includes
#include <raylib.h>
#include <math.h>

// defines

#define COL 64
#define ROW 64
#define DEPTH 64

// structures

typedef struct {
    float state;
    float densityField;
    // TODO: Consider void field
} Cell;

typedef struct {
    Cell table[COL][ROW][DEPTH];
    Cell next[COL][ROW][DEPTH];
} Environment3D;
