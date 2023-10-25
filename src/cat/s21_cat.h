#ifndef S21_CAT
#define S21_CAT

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct options {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} opt;
opt options = {0};

void parcer(int argc, char *argv[], opt *options);
void read_file(int argc, char *argv[], opt *options);

#endif