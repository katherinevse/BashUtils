#include "s21_grep.h"

int main(int argc, char *argv[]) {
  regex_t regex; //вопрос 
  if (argc == 1) {
    fprintf(stderr,
            "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
            "[-C[num]]\n");
    fprintf(stderr,
            "\t[-e pattern] [-f file] [--binary-files=value] [--color=when]\n");
    fprintf(stderr,
            "\t[--context[=num]] [--directories=action] [--label] "
            "[--line-buffered]\n");
    fprintf(stderr, "\t[--null] [pattern] [file ...]\n");

  } else if (argc == 2) {
    char cur = getchar();
    while (cur != EOF) {
      cur = getchar();
    }

  } else if ((argc >= 3)) {
    parcer(argc, argv, &options, &regex);
    read_file(argc, argv, &options, &regex);
  }

  return 0;
}

void parcer(int argc, char *argv[], opt *options, regex_t *regex) {
  int flag = 0; // intead of trash
  int reg_flag = 0; // установки флагов регулярных выражений
  char pattern[1000]; // для хранения шаблона регулярного выражения. При обработке опции -e, значение этой опции (которое передается через optarg
  while ((flag = getopt_long(argc, argv, "e:ivclnsoh", NULL, NULL)) != (-1)) {
    switch (flag) {
      case 'i':
        options->i = 1;
        reg_flag = REG_ICASE; // регистронезависимый поиск
        break;
      case 'v':
        options->v = 1;
        break;
      case 'c':
        options->c = 1;
        break;
      case 'l':
        options->l = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 'h':
        options->h = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'e':
        options->e = 1;
        strcat(pattern, optarg); 
        strcat(pattern, "|");
        if (reg_flag == 0) {
          reg_flag = 1;
        }
        pattern[strlen(pattern) - 1] = '\0';
        break;
    }
  }
  if (options->e) {
    regcomp(regex, pattern, reg_flag);
  } else {
    regcomp(regex, argv[optind], reg_flag);
    optind++;
  }
}
