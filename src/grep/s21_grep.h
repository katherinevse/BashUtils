#ifndef S21_CAT
#define S21_CAT

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct options {
    int e;
    int i;
    int v;
    int c;
    int l;
    int n;
    int h;
    int s;
} opt;
opt options = {0};

void parser(int argc, char *argv[], opt *options, regex_t *regex);
void read_file(int argc, char *argv[], opt *options, regex_t *regex);
void flags(char *pattern, int reg_flag, opt *options);
#endif
