#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "eval_rpn.h"

static int priority(char op) {
    switch (op) {
        case '!': return 5;
        case '&': case '/': return 4;
        case '|': case '^': case '\\': return 3;
        case '<': case '>': return 2;
        case '=': return 1;
        default: return 0;
    }
}

void to_rpn(const char *input, char *output) {
    char *stack = malloc(strlen(input) + 1);
    if (stack == NULL) {
        output[0] = '\0';
        return;
    }
    int stack_counter = 0;
    int output_counter = 0;

    for (int i = 0; input[i] != '\0'; i++) {
        char c = input[i];

        if (isspace((unsigned char)c)) {
            continue;
        } else if (isalpha((unsigned char)c) || c == '0' || c == '1') {
            output[output_counter++] = c;
        } else if (c == '!' || c == '(') {
            stack[stack_counter++] = c;
        } else if (c == ')') {
            while (stack[stack_counter - 1] != '(') {
                output[output_counter++] = stack[--stack_counter];
            }
            stack_counter--;
        } else {
            while (stack_counter > 0 &&
                   priority(stack[stack_counter - 1]) >= priority(c)) {
                output[output_counter++] = stack[--stack_counter];
            }
            stack[stack_counter++] = c;
        }
    }

    while (stack_counter > 0) {
        output[output_counter++] = stack[--stack_counter];
    }
    output[output_counter] = '\0';
    free(stack);
}

int eval(const char *rpn, const int vals[26]) {
    int *stack = malloc((strlen(rpn) + 1) * sizeof(int));
    if (stack == NULL) {
        return 0;
    }
    int stack_counter = 0;

    for (int i = 0; rpn[i] != '\0'; i++) {
        char c = rpn[i];

        if (c == '0' || c == '1') {
            stack[stack_counter++] = c - '0';
        } else if (isalpha((unsigned char)c)) {
            stack[stack_counter++] = vals[tolower((unsigned char)c) - 'a'];
        } else if (c == '!') {
            stack[stack_counter - 1] = !stack[stack_counter - 1];
        } else {
            int b = stack[--stack_counter];
            int a = stack[--stack_counter];

            switch (c) {
                case '&':  stack[stack_counter++] = a & b;    break;
                case '|':  stack[stack_counter++] = a | b;    break;
                case '^':  stack[stack_counter++] = a ^ b;    break;
                case '=':  stack[stack_counter++] = (a == b); break;
                case '>':  stack[stack_counter++] = (!a) | b; break;
                case '<':  stack[stack_counter++] = a | (!b); break;
                case '/':  stack[stack_counter++] = !(a & b); break;
                case '\\': stack[stack_counter++] = !(a | b); break;
            }
        }
    }
    int result = stack[0];
    free(stack);
    return result;
}