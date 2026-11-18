#include <stdio.h>
#include "forms.h"

static int var_value(int row, int n, int i) {
    return (row >> (n - 1 - i)) & 1;
}


void print_sdnf(const char *vars, int n, const int *f) {
    int first = 1;

    printf("SDNF: ");
    for (int row = 0; row < (1 << n); row++) {
        if (f[row] != 1) {
            continue;
        }
        if (!first) {
            printf(" | ");
        }
        first = 0;

        printf("(");
        for (int i = 0; i < n; i++) {
            if (i > 0) {
                printf(" & ");
            }
            if (var_value(row, n, i) == 0) {
                printf("!");
            }
            printf("%c", vars[i]);
        }
        printf(")");
    }

    if (first) {
        printf("Невозможно построить СДНФ (нету строк со значением 1)");
    }
    printf("\n");
}


void print_sknf(const char *vars, int n, const int *f) {
    int first = 1;

    printf("SKNF: ");
    for (int row = 0; row < (1 << n); row++) {
        if (f[row] != 0) {
            continue;
        }
        if (!first) {
            printf(" & ");
        }
        first = 0;

        printf("(");
        for (int i = 0; i < n; i++) {
            if (i > 0) {
                printf(" | ");
            }
            if (var_value(row, n, i) == 1) {
                printf("!");
            }
            printf("%c", vars[i]);
        }
        printf(")");
    }

    if (first) {
        printf("Невозможно построить скнф (нету строк со значением 0))");
    }
    printf("\n");
}


void print_dummy_vars(const char *vars, int n, const int *f) {
    int found = 0;

    printf("Dummy variables: ");
    for (int i = 0; i < n; i++) {
        int bit = 1 << (n - 1 - i);
        int is_dummy = 1;

        for (int row = 0; row < (1 << n); row++) {
            if (f[row] != f[row ^ bit]) {
                is_dummy = 0;
                break;
            }
        }
        if (is_dummy) {
            printf("%c ", vars[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("none");
    }
    printf("\n");
}