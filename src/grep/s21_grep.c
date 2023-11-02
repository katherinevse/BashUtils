#include "s21_grep.h"

#include <string.h>

int main(int argc, char *argv[]) {
  regex_t regex;  // скомпилированный паттерн лежит
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
    parser(argc, argv, &options, &regex);
  }
  return 0;
}

void parser(int argc, char *argv[], opt *options, regex_t *regex) {
  int flag = 0;
  int reg_flag = 0; 
  char pattern[1000]; //sample 
  while ((flag = getopt_long(argc, argv, "e:ivclnsoh", NULL, NULL)) !=(-1)) { 
    switch (flag) {
      case 'i':
        options->i = 1;
        reg_flag = REG_ICASE;
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
        strcat(pattern,"|"); 
        if (reg_flag == 0) {
          reg_flag = REG_EXTENDED; //1
        }
        pattern[strlen(pattern) - 1] = '\0';
        break;
    }
  }
  if (options->e) {
    regcomp(regex, pattern, reg_flag);
  } else {
    regcomp(regex, argv[optind], reg_flag); //предполагается шаблоном
    //printf("My arg:%s\n",argv[optind]);
    optind++;
  }
  read_file(argc, argv, options, regex);
}

void read_file(int argc, char *argv[], opt *options, regex_t *regex) {
  FILE *file;
  char *line = NULL;
  size_t len = 0;
  int read = 0;  // number of characters read
  int search_reg = 0;
  int str_count = 0;
  int str_count_c = 0;
  int num_files = argc - optind; 
  while (optind < argc) {
    file = fopen(argv[optind], "r");
    if (file) {
      
      int over = 0;  //было ли уже выведено имя файла
      while ((read = getline(&line, &len, file)) != EOF) {
        search_reg =regexec(regex, line, 0, NULL, 0);  
        str_count++;

        if ((options->v && search_reg == REG_NOMATCH) ||
            (search_reg == 0 && (options->v == 0 || options->e))) {

          //проверка на должны ли быть выведены результаты поиска
          if (num_files > 1 && options->h == 0 && options->c == 0 &&
              options->l == 0) {
            printf("%s:", argv[optind]);
          }
          //отработка флага n
          if (options->n && options->c == 0 && options->l == 0) {
            printf("%d:%s", str_count, line);
            if (line[strlen(line) - 1] != '\n') {
              printf("\n");
            }
          }
          //отработка флага с
          else if (options->l && over == 0) {
            printf("%s\n", argv[optind]);
            over++;
          //флаг l    
          } else if (options->l == 0) {
            printf("%s", line);
            if (line[strlen(line) - 1] != '\n') {
              printf("\n");
            }
          }
        }
      }
      // выводит количество строк совпадения в каждом файле, а при наличии
      // нескольких файлов также выводит их имена.
      if (options->c) {
        if (num_files > 1 && options->h == 0) {
          printf("%s:%d\n", argv[optind], str_count_c);
        } else {
          printf("%d\n", str_count_c);
        }
        str_count_c =
            0;  // Сбрасывает счетчик совпадающих строк для следующего файла.
      }
    } else {
      if (options->s == 0) {
        fprintf(stderr, "s21_grep: %s: No such file or directory\n",
                argv[optind]);
      }
    }
    optind++;
  }
  fclose(file);
  exit(0);
  free(line);
  regfree(regex);
}

// v выводит строки, которые НЕ содержат указанный шаблон.
//-h исключит отображение имени файла перед каждой строкой совпадения.
//-с используется для вывода только количества строк, содержащих совпадения, а не самих строк
//  -n в команде grep добавляет номера строк, в каких строках файла были
//  обнаружены совпадения с заданным шаблоном
//-l  выводить только имена файлов где есть шаблон