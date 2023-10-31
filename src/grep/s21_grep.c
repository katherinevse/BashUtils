#include "s21_grep.h"
#include <string.h>

void parcer(int argc, char *argv[], opt *options, regex_t *regex);
void Grep_Printf_i(int argc, char *argv[], opt *options, regex_t *regex);

int main(int argc, char * argv[]){
  regex_t regex; // скомпилированный паттерн лежит 
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
  }
}

void parcer(int argc, char *argv[], opt *options, regex_t *regex) {
  int flag = 0;
  int reg_flag = 0; // установки флагов регулярных выражений, для i, для е
  char pattern[1000]; // для хранения шаблона регулярного выражения. При обработке опции -e, значение этой опции (которое передается через optarg
  while ((flag = getopt_long(argc, argv, "e:ivclnsoh", NULL, NULL)) != (-1)) {
    switch (flag) {
      case 'i':
        options->i = 1;
        reg_flag = REG_ICASE; // поиск должен быть регистронезависимым
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
        strcat(pattern, optarg); // optarg- аргумент в данный момент // optind - индекс  // указатель просто указывает на элемент массива argv[]).   // pattern = argument(name)
        strcat(pattern, "|"); //чтобы создать шаблон, который будет включать в себя несколько подшаблонов, разделенных символом |.
        if (reg_flag == 0) {
          reg_flag = 1; // REG_EXTENDED_FLAG
        }
        pattern[strlen(pattern) - 1] = '\0';// можно удалить //strncat(pattern, "", 1);

        break;
    }
    
  }
  if (options->e) {
    regcomp(regex, pattern, reg_flag);
    //regex: Это указатель на структуру regex_t, в которую будет сохранено скомпилированное регулярное выражение.
    //pattern: Это строка, содержащая само регулярное выражение, которое нужно скомпилировать
    //pattern: Это строка, содержащая само регулярное выражение, которое нужно скомпилировать

  } else {
    regcomp(regex, argv[optind], reg_flag); //Если опция -e не указана, программа просто берет следующий аргумент из командной строки (argv[optind]) и компилирует его как регулярное выражение. 
    optind++; //optind увеличивается для перехода к следующему аргументу
  }
  Grep_Printf_i(argc, argv, &options, &regex);
}

void Grep_Printf_i(int argc, char *argv[], opt *options, regex_t *regex){
  FILE *file;
  char *line = 0;

  int num_files = argc - optind;; // Определяем количество переданных файлов для поиска
  while(optind<argc){
    file = fopen(argv[optind], "r");
    if (file){

    }
  }
}