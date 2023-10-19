#include<stdio.h>

void print_file(char *name);

int main(int argc, char **argv) {
  for (int i = 1; i < argc; i++){ // цикл начинается с argv[1]
    print_file(argv[i]);           // в argv[0] находится "./s21_cat"
  }
  return 0; 
}


void print_file(char *name) {
  FILE *f = fopen(name, "rt");
    
  if (f != NULL) {
  int c = fgetc(f);
  while (c != EOF) {
    putc(c, stdout);  
    c = fgetc(f);
  }
  fclose(f);
  }
}
