#include <stdio.h>

void print_file(char *name) {}

int main(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    print_file(argv[i]);
  }
  return 0;
}

void print_file(char *name) {
  FILE *f = fopen(name, "rt");

  if (f != NULL) {
    int c = fgetc(f);
    while (c != EOF) {
      putc(c,
           stdout);  // Исправлено: выводим символы в стандартный вывод (stdout)
      c = fgetc(f);
    }
    fclose(f);

  }
}
