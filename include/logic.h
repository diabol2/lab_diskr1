#ifndef LOGIC_H
#define LOGIC_H

#include "eval_rpn.h"

int collect_vars(const char *formula, char vars[26]);
void build_vector(const char *rpn, const char *vars, int n, int *f);
void print_truth_table(const char *vars, int n, const int *f);

#endif