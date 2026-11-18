#ifndef BASIS_H
#define BASIS_H

void print_basis_and_or_not(const char *vars, int n, const int *f);
void print_basis_pierce(const char *vars, int n, const int *f);
void print_basis_sheffer(const char *vars, int n, const int *f);
void print_zhegalkin(const char *vars, int n, const int *f);

#endif