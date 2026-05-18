#include "shared.h"
#include <math.h>

#define RANGE 4
#define KERNEL_INTENSITY 1
#define KERNEL_STANDARD_DEVIATION 1
#define CAPACITY 1

// NOTE:
// Initiates empty environment
Environment3D initEnvironment3D() {

    Environment3D Env;

    for (int i = 0; i < COL; i++) {
        for (int j = 0; j < ROW; j++) {
            for (int k = 0; k < DEPTH; k++) {
                Env.table[i][j][k].state = 0.0;
                Env.table[i][j][k].densityField = 0.0;
            }
        }
    }

    return Env;
}

// TODO: make this a macro probably
// NOTE:
// Gaussian Distribution proportional to inverse distance
float kernel(int q, int r, int p) {
    return exp(-1.0 * KERNEL_INTENSITY * (q * q + r * r + p * p) /
               (2.0 * KERNEL_STANDARD_DEVIATION * KERNEL_STANDARD_DEVIATION));
}

void localDensityField(Environment3D *Env) {

    for (int i = 0; i < COL; i++) {
        for (int j = 0; j < ROW; j++) {
            for (int k = 0; k < DEPTH; k++) {

                for (int p = -RANGE; p <= RANGE; p++) {
                    for (int r = -RANGE; r <= RANGE; r++) {
                        for (int q = -RANGE; q <= RANGE; q++) {

                            Env->next[i][j][k].densityField +=
                                kernel(q, r, p) * Env->table[i + q][j + r][k + p].state;
                        }
                    }
                }
            }
        }
    }
}

void step(Environment3D *Env) {

    for (int i = 0; i < COL; i++) {
        for (int j = 0; j < ROW; j++) {
            for (int k = 0; k < DEPTH; k++) {

                double u_g_i = 0, u_g_j = 0, u_g_k = 0;

                for (int q = -RANGE; q <= RANGE; q++)
                    u_g_i += Env->table[i + q][j][k].densityField;
                for (int r = -RANGE; r <= RANGE; r++)
                    u_g_j += Env->table[i][j + r][k].densityField;
                for (int p = -RANGE; p <= RANGE; p++)
                    u_g_k += Env->table[i][j][k + p].densityField;

                int ti = i + (int)u_g_i;
                int tj = j + (int)u_g_j;
                int tk = k + (int)u_g_k;

                if (ti < 0 || ti >= COL || tj < 0 || tj >= ROW || tk < 0 || tk >= DEPTH)
                    continue;

                if (Env->table[ti][tj][tk].state == CAPACITY) {
                    Env->next[i][j][k].state = Env->table[i][j][k].state;
                } else {
                    Env->next[i][j][k].state = Env->table[i][j][k].state -
                        (Env->table[ti][tj][tk].state - Env->table[i][j][k].state);
                }

                Env->next[ti][tj][tk].state +=
                    (Env->table[ti][tj][tk].state - Env->table[i][j][k].state);
            }
        }
    }
}
