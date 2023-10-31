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
  int o;
} opt;
opt options = {0};

void parcer(int argc, char *argv[], opt *options, regex_t *regex);
void Grep_Printf_i(int argc, char *argv[], opt *options, regex_t *regex);
#endif
