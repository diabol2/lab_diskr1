#include <stdio.h>
#include <ctype.h>
#include "logic.h"

int collect_vars(const char *formula, char vars[26]) {
    int used[26] = {0};
    for (int i = 0; formula[i] != '\0'; i++) {
        if (isalpha((unsigned char)formula[i])) {
            used[tolower((unsigned char)formula[i]) - 'a'] = 1;
        }
    }
    int n = 0;
    for (int i = 0; i < 26; i++) {
        if (used[i]) {
            vars[n++] = 'a' + i;
        }
    }
    return n;
}

void build_vector(const char *rpn, const char *vars, int n, int *f) {
    for (int mask = 0; mask < (1 << n); mask++) {
        int vals[26] = {0};
        for (int i = 0; i < n; i++) {
            vals[vars[i] - 'a'] = (mask >> (n - 1 - i)) & 1;
        }
        f[mask] = eval(rpn, vals);
    }
}

void print_truth_table(const char *vars, int n, const int *f) {
    for (int i = 0; i < n; i++) {
        printf("%c ", vars[i]);
    }
    printf("| F\n");

    for (int mask = 0; mask < (1 << n); mask++) {
        for (int i = 0; i < n; i++) {
            printf("%d ", (mask >> (n - 1 - i)) & 1);
        }
        printf("| %d\n", f[mask]);
    }
}