#include "s21_grep.h"
#include <string.h>

void parcer(int argc, char *argv[], opt *options, regex_t *regex);
void Grep_Printf_i(int argc, char *argv[], opt *options, regex_t *regex);

void Grep_Printf(int argc, char *argv[]) {

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
    Grep_Printf_i(argc, argv, &options, &regex);
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
    regcomp(regex, pattern, reg_flag);//regex: Это указатель на структуру regex_t, в которую будет сохранено скомпилированное регулярное выражение.
    //pattern: Это строка, содержащая само регулярное выражение, которое нужно скомпилировать
    //pattern: Это строка, содержащая само регулярное выражение, которое нужно скомпилировать

  } else {
    regcomp(regex, argv[optind], reg_flag); //Если опция -e не указана, программа просто берет следующий аргумент из командной строки (argv[optind]) и компилирует его как регулярное выражение. 
    optind++; //optind увеличивается для перехода к следующему аргументу
  }
}
// почему после отработки флага е мы заканчиваем парсить что-либо?

void Grep_Printf_i(int argc, char *argv[], opt *options, regex_t *regex) {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  int read = 0; // возвращает количество считанных символов
  int status = 0;
  int str_count = 0;
  int str_count_c = 0;
  int num_files = 0; // Определяем количество переданных файлов для поиска
  num_files = argc - optind;

  while (optind < argc) {
    fp = fopen(argv[optind], "r");
    if (fp) {
      int over = 0;
      while ((read = getline(&line, &len, fp)) != EOF) { 
        status = regexec(regex, line, 0, NULL, 0); //строка выполняет поиск регулярного выражения в строке, считанной из файла
        str_count++;
        if ((options->v && status == REG_NOMATCH) ||
            (status == 0 && (options->v == 0 || options->e))) {
          if (num_files > 1 && options->h == 0 && options->c == 0 &&
              options->l == 0) {
            printf("%s:", argv[optind]);
          }
          if (options->n && options->c == 0 && options->l == 0) {
            printf("%d:%s", str_count, line);
            if (line[strlen(line) - 1] != '\n') {
              printf("\n");
            }
          } else if (options->c) {
            str_count_c++;
          } else if (options->l && over == 0) {
            printf("%s\n", argv[optind]);
            over++;
          } else if (options->l == 0) {
            printf("%s", line);
            if (line[strlen(line) - 1] != '\n') {
              printf("\n");
            }
          }
        }
      }
      if (options->c) {
        if (num_files > 1 && options->h == 0) {
          printf("%s:%d\n", argv[optind], str_count_c);
        } else {
          printf("%d\n", str_count_c);
        }
        str_count_c = 0;
      }
      if (options->n) {
        str_count = 0;
      }
    } else {
      if (options->s == 0) {
        fprintf(stderr, "s21_grep: %s: No such file or directory\n",
                argv[optind]);
      }
    }
    optind++;
  }
  fclose(fp);
  exit(0);
  free(line);
  regfree(regex);
}