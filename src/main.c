#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eval_rpn.h"
#include "logic.h"
#include "forms.h"
#include "basis.h"

static char *read_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *text = malloc((size_t)size + 1);
    if (text == NULL) {
        fclose(file);
        return NULL;
    }
    size_t got = fread(text, 1, (size_t)size, file);
    text[got] = '\0';
    fclose(file);
    return text;
}

int main(int argc, char *argv[]) {
    const char *filename = (argc > 1) ? argv[1] : "formula.txt";

    char *formula = read_file(filename);
    if (formula == NULL) {
        perror("Ошибка открытия файла с формулы");
        return 1;
    }

    printf("F = %s", formula);

    char *rpn = malloc(strlen(formula) + 1);
    if (rpn == NULL) {
        printf("Не хватило памяти\n");
        free(formula);
        return 1;
    }
    to_rpn(formula, rpn);

    char vars[26];
    int n = collect_vars(formula, vars);

    int *f = malloc((size_t)(1 << n) * sizeof(int));
    if (f == NULL) {
        printf("Не хватило памяти\n");
        free(rpn);
        free(formula);
        return 1;
    }

    build_vector(rpn, vars, n, f);
    print_truth_table(vars, n, f);

    print_sdnf(vars, n, f);
    print_sknf(vars, n, f);
    print_dummy_vars(vars, n, f);

    print_basis_and_or_not(vars, n, f);
    print_basis_pierce(vars, n, f);
    print_basis_sheffer(vars, n, f);
    print_zhegalkin(vars, n, f);

    free(f);
    free(rpn);
    free(formula);
    return 0;
}