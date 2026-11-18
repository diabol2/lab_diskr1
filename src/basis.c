#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basis.h"

enum { PIERCE, SHEFFER };

static int var_value(int row, int n, int i) {
    return (row >> (n - 1 - i)) & 1;
}

static void *xmalloc(size_t size) {
    void *p = malloc(size);
    if (p == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    return p;
}

static char *dup_str(const char *s) {
    char *copy = xmalloc(strlen(s) + 1);
    strcpy(copy, s);
    return copy;
}


static char *wrap(const char *l, char op, const char *r) {
    size_t len = strlen(l) + strlen(r) + 8;
    char *s = xmalloc(len);
    snprintf(s, len, "(%s %c %s)", l, op, r);
    return s;
}

static char op_char(int kind) {
    return (kind == PIERCE) ? '\\' : '/';
}

static char *g_not(int kind, const char *x) {
    return wrap(x, op_char(kind), x);
}

static char *g_and(int kind, const char *x, const char *y) {
    char *res;
    if (kind == PIERCE) {
        char *nx = g_not(kind, x);
        char *ny = g_not(kind, y);
        res = wrap(nx, '\\', ny);
        free(nx);
        free(ny);
    } else {
        char *t = wrap(x, '/', y);
        res = g_not(kind, t);
        free(t);
    }
    return res;
}

static char *g_or(int kind, const char *x, const char *y) {
    char *res;
    if (kind == PIERCE) {
        char *t = wrap(x, '\\', y);
        res = g_not(kind, t);
        free(t);
    } else {
        char *nx = g_not(kind, x);
        char *ny = g_not(kind, y);
        res = wrap(nx, '/', ny);
        free(nx);
        free(ny);
    }
    return res;
}


static char *fold(int kind, int is_and, char **items, int lo, int hi) {
    if (hi - lo == 1) {
        return dup_str(items[lo]);
    }
    int mid = (lo + hi) / 2;
    char *l = fold(kind, is_and, items, lo, mid);
    char *r = fold(kind, is_and, items, mid, hi);
    char *res = is_and ? g_and(kind, l, r) : g_or(kind, l, r);
    free(l);
    free(r);
    return res;
}

static char *literal(int kind, char v, int positive) {
    char s[2] = { v, '\0' };
    return positive ? dup_str(s) : g_not(kind, s);
}


static char *build_expr(int kind, const char *vars, int n, const int *f) {
    int size = 1 << n;
    int count = 0;
    for (int row = 0; row < size; row++) {
        count += f[row];
    }

    if (count == 0) {                      
        char *p = literal(kind, vars[0], 1);
        char *q = literal(kind, vars[0], 0);
        char *res = g_and(kind, p, q);
        free(p);
        free(q);
        return res;
    }

    char **terms = xmalloc((size_t)count * sizeof(char *));
    char **lits = xmalloc((size_t)n * sizeof(char *));
    int t = 0;

    for (int row = 0; row < size; row++) {
        if (f[row] != 1) {
            continue;
        }
        for (int i = 0; i < n; i++) {
            lits[i] = literal(kind, vars[i], var_value(row, n, i));
        }
        terms[t++] = fold(kind, 1, lits, 0, n);
        for (int i = 0; i < n; i++) {
            free(lits[i]);
        }
    }

    char *res = fold(kind, 0, terms, 0, count);
    for (int i = 0; i < count; i++) {
        free(terms[i]);
    }
    free(terms);
    free(lits);
    return res;
}

static void print_basis(int kind, const char *title,
                        const char *vars, int n, const int *f) {
    printf("%s: ", title);
    if (n == 0) {
        printf("not applicable (no variables)\n");
        return;
    }
    char *expr = build_expr(kind, vars, n, f);
    printf("%s\n", expr);
    free(expr);
}

void print_basis_pierce(const char *vars, int n, const int *f) {
    print_basis(PIERCE, "Basis { \\ } (Pierce arrow)", vars, n, f);
}

void print_basis_sheffer(const char *vars, int n, const int *f) {
    print_basis(SHEFFER, "Basis { / } (Sheffer stroke)", vars, n, f);
}


void print_basis_and_or_not(const char *vars, int n, const int *f) {
    int first = 1;

    printf("Basis { &, |, ! }: ");
    if (n == 0) {
        printf("not applicable (no variables)\n");
        return;
    }
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
        printf("(%c & !%c)", vars[0], vars[0]);
    }
    printf("\n");
}


void print_zhegalkin(const char *vars, int n, const int *f) {
    int size = 1 << n;
    int *c = xmalloc((size_t)size * sizeof(int));

    for (int row = 0; row < size; row++) {
        c[row] = f[row];
    }
    for (int bit = 1; bit < size; bit <<= 1) {
        for (int row = 0; row < size; row++) {
            if (row & bit) {
                c[row] = c[row] ^ c[row ^ bit];
            }
        }
    }

    printf("Zhegalkin polynomial: ");
    int first = 1;
    for (int row = size - 1; row >= 0; row--) {
        if (c[row] == 0) {
            continue;
        }
        if (!first) {
            printf(" ^ ");
        }
        first = 0;
        if (row == 0) {
            printf("1");
        } else {
            for (int i = 0; i < n; i++) {
                if (var_value(row, n, i)) {
                    printf("%c", vars[i]);
                }
            }
        }
    }
    if (first) {
        printf("0");
    }
    printf("\n");

    free(c);
}