#include "s21_grep.h"

int main(int argc, char *argv[]) {
  regex_t regex; //вопрос 

  (void)argv;
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

