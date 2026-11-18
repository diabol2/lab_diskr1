#ifndef EVAL_RPN_H
#define EVAL_RPN_H

void to_rpn(const char *input, char *output);
int eval(const char *rpn, const int vals[26]);

#endif